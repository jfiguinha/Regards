#include <header.h>
#include "COpenCLAvirResizer.h"
#include "OpenCLContext.h"
#include <cmath>
#include <cstdio>
#include <fstream>
#include <sstream>

using namespace Regards::OpenCL;

namespace
{
	constexpr double AVIR_PI = 3.14159265358979323846;
	constexpr double AVIR_PId2 = AVIR_PI * 0.5;
}

COpenCLAvirResizer::COpenCLAvirResizer(COpenCLContext* openCLContext)
	: openCLContext(openCLContext)
{
	
}

COpenCLAvirResizer::~COpenCLAvirResizer()
{
	ReleaseKernels();
}

void COpenCLAvirResizer::ReleaseKernels()
{
	cl_kernel* kernels[] = {
		&m_kLinearize, &m_kDelinearize, &m_kResizeH, &m_kResizeV,
		&m_kSharpenH, &m_kSharpenV, &m_kDitherRound, &m_kDitherErrDiff
	};

	for (cl_kernel* k : kernels)
	{
		if (*k)
		{
			clReleaseKernel(*k);
			*k = nullptr;
		}
	}
}

cl_kernel COpenCLAvirResizer::CreateKernel(const char* name)
{
	cl_int err = CL_SUCCESS;
	cl_kernel k = clCreateKernel((cl_program)m_program.ptr(), name, &err);
	if (err != CL_SUCCESS)
	{
		fprintf(stderr, "COpenCLAvirResizer: echec creation kernel '%s' (err=%d)\n", name, err);
		return nullptr;
	}
	return k;
}

bool COpenCLAvirResizer::Init()
{
	m_program = openCLContext->GetProgram("IDR_OPENCL_AVIR");

	m_kLinearize     = CreateKernel("k_linearize_srgb");
	m_kDelinearize   = CreateKernel("k_delinearize_srgb");
	m_kResizeH       = CreateKernel("k_resize_h");
	m_kResizeV       = CreateKernel("k_resize_v");
	m_kSharpenH      = CreateKernel("k_sharpen_h");
	m_kSharpenV      = CreateKernel("k_sharpen_v");
	m_kDitherRound   = CreateKernel("k_dither_round");
	m_kDitherErrDiff = CreateKernel("k_dither_errdiffusion");

	isOk = m_kLinearize && m_kDelinearize && m_kResizeH && m_kResizeV &&
		m_kSharpenH && m_kSharpenV && m_kDitherRound && m_kDitherErrDiff;

	isInit = true;

	return isOk;
}

// Fenetre Peaked Cosine d'AVIR, formule equivalente a
// CDSPWindowGenPeakedCosine mais evaluee directement pour un x continu
// (au lieu du generateur recursif a pas entiers d'AVIR, qui suppose un
// echantillonnage a la position entiere) :
//
//   w(x) = cos(pi/2 * x/len2) * (1 - (|x|/len2)^alpha),  pour |x| < len2
//
double COpenCLAvirResizer::PeakedCosineWindow(double x, double len2, double alpha)
{
	if (len2 <= 0.0)
		return 0.0;

	const double ax = std::fabs(x);
	if (ax >= len2)
		return 0.0;

	const double h = std::pow(ax / len2, alpha);
	return std::cos(AVIR_PId2 * x / len2) * (1.0 - h);
}

void COpenCLAvirResizer::BuildAxisFilters(
	int srcLen, int dstLen,
	const SAvirResizeParams& params,
	std::vector<SResizeTap>& outTaps,
	std::vector<float>& outCoefs) const
{
	const double scale = (double)dstLen / (double)srcLen;
	const bool downsizing = scale < 1.0;

	// Coupure reduite en cas de sous-echantillonnage, pour l'anti-repliement
	// (meme logique qu'AVIR : le filtre est a la fois interpolateur et
	// anti-aliasing).
	const double cutoff = AVIR_PI * (downsizing ? scale : 1.0) * params.cutoffMult;

	// La demi-longueur augmente avec le facteur de reduction : plus on
	// reduit, plus il faut de taps pour bien filtrer avant repliement.
	const double len2 = (params.baseTaps * 0.5) / (downsizing ? scale : 1.0);
	const int fl2 = (int)std::ceil(len2);
	const int tapCount = fl2 * 2 + 1;

	outTaps.resize(dstLen);
	outCoefs.clear();
	outCoefs.reserve((size_t)dstLen * tapCount);

	std::vector<double> tmp(tapCount);

	for (int i = 0; i < dstLen; ++i)
	{
		// Alignement centre-a-centre des grilles source/destination.
		const double srcPos = ((i + 0.5) / scale) - 0.5;
		const int srcPosInt = (int)std::floor(srcPos);
		const double frac = srcPos - srcPosInt;

		const int tapStart = srcPosInt - fl2;
		const size_t coefOffset = outCoefs.size();

		double sum = 0.0;

		for (int k = 0; k < tapCount; ++k)
		{
			// Distance continue (non entiere) entre le tap k et le centre
			// exact de l'echantillon source demande.
			const double t = (double)(k - fl2) - frac;

			double v;
			if (std::fabs(t) < 1e-9)
				v = cutoff / AVIR_PI;
			else
				v = std::sin(cutoff * t) / (AVIR_PI * t);

			v *= PeakedCosineWindow(t, len2, params.alpha);

			tmp[k] = v;
			sum += v;
		}

		// Normalisation a gain DC unitaire (preserve la luminosite globale).
		if (std::fabs(sum) > 1e-12)
		{
			const double inv = 1.0 / sum;
			for (double& v : tmp)
				v *= inv;
		}

		for (int k = 0; k < tapCount; ++k)
			outCoefs.push_back((float)tmp[k]);

		outTaps[i] = { tapStart, tapCount, (cl_int)coefOffset };
	}
}

std::vector<float> COpenCLAvirResizer::BuildSharpenKernel(int klen) const
{
	// Noyau passe-bas symetrique (le "flou" de reference pour l'unsharp
	// mask). Volontairement generique : ce n'est pas un portage du filtre
	// de correction d'AVIR (voir note en tete de fichier .h).
	const int half = klen / 2;
	const double len2 = half + 0.5;
	const double cutoff = AVIR_PId2;

	std::vector<double> lp(klen);
	double sum = 0.0;

	for (int k = 0; k < klen; ++k)
	{
		const double t = (double)(k - half);
		double v = (std::fabs(t) < 1e-9)
			? (cutoff / AVIR_PI)
			: (std::sin(cutoff * t) / (AVIR_PI * t));

		v *= PeakedCosineWindow(t, len2, 1.2);
		lp[k] = v;
		sum += v;
	}

	for (double& v : lp)
		v /= sum;

	std::vector<float> out(klen);
	for (int k = 0; k < klen; ++k)
		out[k] = (float)lp[k];

	return out;
}

bool COpenCLAvirResizer::Resize(const cv::UMat& src, cv::UMat& dest,
	const SAvirResizeParams& params)
{
	std::vector<SResizeTap> tapsH;
	std::vector<float> coefsH;
	std::vector<SResizeTap> tapsV;
	std::vector<float> coefsV;
	cv::UMat dstFloat;

	if (openCLContext == nullptr || src.empty() || dest.empty())
		return false;

	if (src.type() != CV_8UC4 || dest.type() != CV_8UC4)
		return false;

	const int srcWidth = src.cols;
	const int srcHeight = src.rows;
	const int dstWidth = dest.cols;
	const int dstHeight = dest.rows;

	if (srcWidth <= 0 || srcHeight <= 0 ||
		dstWidth <= 0 || dstHeight <= 0)
		return false;

	if (m_program.empty())
		return false;

	cl_context context = openCLContext->GetContext();
	cl_command_queue queue = openCLContext->GetCommandQueue();

	if (context == nullptr || queue == nullptr)
		return false;

	// ============================================================
	// CV_8UC4 BGRA -> CV_32FC4 BGRA
	// ============================================================

	cv::UMat srcFloat;
	src.convertTo(srcFloat, CV_32FC4, 1.0 / 255.0);

	cl_mem bufSrc =
		static_cast<cl_mem>(srcFloat.handle(cv::ACCESS_READ));

	if (bufSrc == nullptr)
		return false;

	// ============================================================
	// Taille des buffers
	// ============================================================

	const size_t srcCount =
		static_cast<size_t>(srcWidth) *
		static_cast<size_t>(srcHeight);

	const size_t midCount =
		static_cast<size_t>(dstWidth) *
		static_cast<size_t>(srcHeight);

	const size_t dstCount =
		static_cast<size_t>(dstWidth) *
		static_cast<size_t>(dstHeight);

	cl_int err = CL_SUCCESS;

	// ============================================================
	// Buffer intermédiaire horizontal
	// ============================================================

	cl_mem bufMid = clCreateBuffer(
		context,
		CL_MEM_READ_WRITE,
		midCount * sizeof(cl_float4),
		nullptr,
		&err);

	if (err != CL_SUCCESS || bufMid == nullptr)
		return false;

	// ============================================================
	// Buffer résultat vertical
	// ============================================================

	cl_mem bufDst = clCreateBuffer(
		context,
		CL_MEM_READ_WRITE,
		dstCount * sizeof(cl_float4),
		nullptr,
		&err);

	if (err != CL_SUCCESS || bufDst == nullptr)
	{
		clReleaseMemObject(bufMid);
		return false;
	}

	// Fonction locale de nettoyage
	auto cleanup = [&]()
	{
		if (bufMid != nullptr)
		{
			clReleaseMemObject(bufMid);
			bufMid = nullptr;
		}

		if (bufDst != nullptr)
		{
			clReleaseMemObject(bufDst);
			bufDst = nullptr;
		}
	};

	// ============================================================
	// Linearisation sRGB
	// ============================================================

	if (params.linearizeGamma)
	{
		const int count = static_cast<int>(srcCount);

		err = clSetKernelArg(
			m_kLinearize,
			0,
			sizeof(cl_mem),
			&bufSrc);

		if (err == CL_SUCCESS)
		{
			err = clSetKernelArg(
				m_kLinearize,
				1,
				sizeof(int),
				&count);
		}

		if (err != CL_SUCCESS)
		{
			cleanup();
			return false;
		}

		const size_t global = srcCount;

		err = clEnqueueNDRangeKernel(
			queue,
			m_kLinearize,
			1,
			nullptr,
			&global,
			nullptr,
			0,
			nullptr,
			nullptr);

		if (err != CL_SUCCESS)
		{
			cleanup();
			return false;
		}
	}

	// ============================================================
	// Filtre horizontal
	// ============================================================

	BuildAxisFilters(
		srcWidth,
		dstWidth,
		params,
		tapsH,
		coefsH);

	cl_mem bufTapsH = clCreateBuffer(
		context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		tapsH.size() * sizeof(SResizeTap),
		tapsH.data(),
		&err);

	if (err != CL_SUCCESS || bufTapsH == nullptr)
	{
		cleanup();
		return false;
	}

	cl_mem bufCoefsH = clCreateBuffer(
		context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		coefsH.size() * sizeof(float),
		coefsH.data(),
		&err);

	if (err != CL_SUCCESS || bufCoefsH == nullptr)
	{
		clReleaseMemObject(bufTapsH);
		cleanup();
		return false;
	}

	{
		const int sw = srcWidth;
		const int sh = srcHeight;
		const int dw = dstWidth;

		err = clSetKernelArg(
			m_kResizeH,
			0,
			sizeof(cl_mem),
			&bufSrc);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kResizeH,
				1,
				sizeof(int),
				&sw);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kResizeH,
				2,
				sizeof(int),
				&sh);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kResizeH,
				3,
				sizeof(cl_mem),
				&bufMid);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kResizeH,
				4,
				sizeof(int),
				&dw);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kResizeH,
				5,
				sizeof(cl_mem),
				&bufTapsH);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kResizeH,
				6,
				sizeof(cl_mem),
				&bufCoefsH);

		if (err == CL_SUCCESS)
		{
			const size_t global[2] =
			{
				static_cast<size_t>(dstWidth),
				static_cast<size_t>(srcHeight)
			};

			err = clEnqueueNDRangeKernel(
				queue,
				m_kResizeH,
				2,
				nullptr,
				global,
				nullptr,
				0,
				nullptr,
				nullptr);
		}
	}

	clReleaseMemObject(bufTapsH);
	clReleaseMemObject(bufCoefsH);

	if (err != CL_SUCCESS)
	{
		cleanup();
		return false;
	}

	// ============================================================
	// Filtre vertical
	// ============================================================

	BuildAxisFilters(
		srcHeight,
		dstHeight,
		params,
		tapsV,
		coefsV);

	cl_mem bufTapsV = clCreateBuffer(
		context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		tapsV.size() * sizeof(SResizeTap),
		tapsV.data(),
		&err);

	if (err != CL_SUCCESS || bufTapsV == nullptr)
	{
		cleanup();
		return false;
	}

	cl_mem bufCoefsV = clCreateBuffer(
		context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		coefsV.size() * sizeof(float),
		coefsV.data(),
		&err);

	if (err != CL_SUCCESS || bufCoefsV == nullptr)
	{
		clReleaseMemObject(bufTapsV);
		cleanup();
		return false;
	}

	{
		const int mw = dstWidth;
		const int mh = srcHeight;
		const int dh = dstHeight;

		err = clSetKernelArg(
			m_kResizeV,
			0,
			sizeof(cl_mem),
			&bufMid);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kResizeV,
				1,
				sizeof(int),
				&mw);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kResizeV,
				2,
				sizeof(int),
				&mh);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kResizeV,
				3,
				sizeof(cl_mem),
				&bufDst);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kResizeV,
				4,
				sizeof(int),
				&dh);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kResizeV,
				5,
				sizeof(cl_mem),
				&bufTapsV);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kResizeV,
				6,
				sizeof(cl_mem),
				&bufCoefsV);

		if (err == CL_SUCCESS)
		{
			const size_t global[2] =
			{
				static_cast<size_t>(dstWidth),
				static_cast<size_t>(dstHeight)
			};

			err = clEnqueueNDRangeKernel(
				queue,
				m_kResizeV,
				2,
				nullptr,
				global,
				nullptr,
				0,
				nullptr,
				nullptr);
		}
	}

	clReleaseMemObject(bufTapsV);
	clReleaseMemObject(bufCoefsV);

	if (err != CL_SUCCESS)
	{
		cleanup();
		return false;
	}

	clReleaseMemObject(bufMid);
	bufMid = nullptr;

	// ============================================================
	// Sharpen
	// ============================================================

	{
		const bool enlarging =
			dstWidth > srcWidth ||
			dstHeight > srcHeight;

		if (params.sharpen && enlarging)
		{
			const std::vector<float> sk =
				BuildSharpenKernel(5);


			cl_mem bufKernel =
				clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					sk.size() * sizeof(float), (void*)sk.data(), &err);

			if (err != CL_SUCCESS || bufKernel == nullptr)
			{
				cleanup();
				return false;
			}

			cl_mem bufTmp = clCreateBuffer(
				context,
				CL_MEM_READ_WRITE,
				dstCount * sizeof(cl_float4),
				nullptr,
				&err);

			if (err != CL_SUCCESS || bufTmp == nullptr)
			{
				clReleaseMemObject(bufKernel);
				cleanup();
				return false;
			}

			const int w = dstWidth;
			const int h = dstHeight;
			const int klen = static_cast<int>(sk.size());
			const float amount = params.sharpenAmount;

			const size_t global[2] =
			{
				static_cast<size_t>(w),
				static_cast<size_t>(h)
			};

			// --------------------------------------------------------
			// Sharpen horizontal
			// --------------------------------------------------------

			err = clSetKernelArg(
				m_kSharpenH,
				0,
				sizeof(cl_mem),
				&bufDst);

			if (err == CL_SUCCESS)
				err = clSetKernelArg(
					m_kSharpenH,
					1,
					sizeof(int),
					&w);

			if (err == CL_SUCCESS)
				err = clSetKernelArg(
					m_kSharpenH,
					2,
					sizeof(int),
					&h);

			if (err == CL_SUCCESS)
				err = clSetKernelArg(
					m_kSharpenH,
					3,
					sizeof(cl_mem),
					&bufTmp);

			if (err == CL_SUCCESS)
				err = clSetKernelArg(
					m_kSharpenH,
					4,
					sizeof(cl_mem),
					&bufKernel);

			if (err == CL_SUCCESS)
				err = clSetKernelArg(
					m_kSharpenH,
					5,
					sizeof(int),
					&klen);

			if (err == CL_SUCCESS)
				err = clSetKernelArg(
					m_kSharpenH,
					6,
					sizeof(float),
					&amount);

			if (err == CL_SUCCESS)
			{
				err = clEnqueueNDRangeKernel(
					queue,
					m_kSharpenH,
					2,
					nullptr,
					global,
					nullptr,
					0,
					nullptr,
					nullptr);
			}

			// --------------------------------------------------------
			// Sharpen vertical
			// --------------------------------------------------------

			if (err == CL_SUCCESS)
			{
				err = clSetKernelArg(
					m_kSharpenV,
					0,
					sizeof(cl_mem),
					&bufTmp);

				if (err == CL_SUCCESS)
					err = clSetKernelArg(
						m_kSharpenV,
						1,
						sizeof(int),
						&w);

				if (err == CL_SUCCESS)
					err = clSetKernelArg(
						m_kSharpenV,
						2,
						sizeof(int),
						&h);

				if (err == CL_SUCCESS)
					err = clSetKernelArg(
						m_kSharpenV,
						3,
						sizeof(cl_mem),
						&bufDst);

				if (err == CL_SUCCESS)
					err = clSetKernelArg(
						m_kSharpenV,
						4,
						sizeof(cl_mem),
						&bufKernel);

				if (err == CL_SUCCESS)
					err = clSetKernelArg(
						m_kSharpenV,
						5,
						sizeof(int),
						&klen);

				if (err == CL_SUCCESS)
					err = clSetKernelArg(
						m_kSharpenV,
						6,
						sizeof(float),
						&amount);

				if (err == CL_SUCCESS)
				{
					err = clEnqueueNDRangeKernel(
						queue,
						m_kSharpenV,
						2,
						nullptr,
						global,
						nullptr,
						0,
						nullptr,
						nullptr);
				}
			}

			clReleaseMemObject(bufKernel);
			clReleaseMemObject(bufTmp);

			if (err != CL_SUCCESS)
			{
				cleanup();
				return false;
			}
		}
	}

	// ============================================================
	// Delinearisation
	// ============================================================

	if (params.linearizeGamma)
	{
		const int count =
			static_cast<int>(dstCount);

		err = clSetKernelArg(
			m_kDelinearize,
			0,
			sizeof(cl_mem),
			&bufDst);

		if (err == CL_SUCCESS)
			err = clSetKernelArg(
				m_kDelinearize,
				1,
				sizeof(int),
				&count);

		if (err != CL_SUCCESS)
		{
			cleanup();
			return false;
		}

		const size_t global = dstCount;

		err = clEnqueueNDRangeKernel(
			queue,
			m_kDelinearize,
			1,
			nullptr,
			&global,
			nullptr,
			0,
			nullptr,
			nullptr);

		if (err != CL_SUCCESS)
		{
			cleanup();
			return false;
		}
	}

	// ============================================================
	// Conversion OpenCL -> UMat CV_32FC4
	// ============================================================

	dstFloat.create(
		dstHeight,
		dstWidth,
		CV_32FC4);

	cl_mem dstFloatBuffer =
		static_cast<cl_mem>(
			dstFloat.handle(cv::ACCESS_WRITE));

	if (dstFloatBuffer == nullptr)
	{
		cleanup();
		return false;
	}

	// ============================================================
	// Copie GPU -> UMat GPU
	// ============================================================

	err = clEnqueueCopyBuffer(
		queue,
		bufDst,
		dstFloatBuffer,
		0,
		0,
		dstCount * sizeof(cl_float4),
		0,
		nullptr,
		nullptr);

	if (err != CL_SUCCESS)
	{
		cleanup();
		return false;
	}

	// ============================================================
	// Synchronisation
	// ============================================================

	err = clFinish(queue);

	if (err != CL_SUCCESS)
	{
		cleanup();
		return false;
	}

	// ============================================================
	// CV_32FC4 -> CV_8UC4
	// ============================================================

	dstFloat.convertTo(
		dest,
		CV_8UC4,
		255.0);

	// ============================================================
	// Nettoyage
	// ============================================================

	cleanup();

	return true;
}