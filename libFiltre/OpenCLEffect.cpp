#include <header.h>
#include "OpenCLEffect.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include "RegardsBitmap.h"
#include "RegardsFloatBitmap.h"
#include <MotionBlur.h>
#include "OpenCLFilter.h"
#include "utility.h"
#include <ImageLoadingFormat.h>
#define minmax
#include <ximage.h>
//#include <RegardsConfigParam.h>
//#include <ParamInit.h>
#include "OpenCLBm3D.h"
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <OpenGL/OpenGL.h>
#elif defined(__WXGTK__)
#include <GL/glx.h>
#include <CL/cl_gl.h>
#else
#include <CL/cl_gl.h>
#endif
#include <DeepLearning.h>
#include <opencv2/core/opengl.hpp>
using namespace Regards::OpenCL;
using namespace Regards::FiltreEffet;
using namespace Regards::DeepLearning;

#define NONE_FILTER 12

COpenCLEffect::COpenCLEffect(const CRgbaquad& backColor, COpenCLContext* context, CImageLoadingFormat* bitmap)
	: IFiltreEffet(backColor)
{
	this->backColor = backColor;
	flag = CL_MEM_COPY_HOST_PTR;
	this->context = context;
	SetBitmap(bitmap);
}

COpenCLEffect::COpenCLEffect(const CRgbaquad& backColor, COpenCLContext* context, COpenCLParameterClMem* data_mem, const int& width, const int& height)
	: IFiltreEffet(backColor)
{
	//bool useMemory = (context->GetDeviceType() == CL_DEVICE_TYPE_GPU) ? false : true;
	this->backColor = backColor;
	//flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	flag = CL_MEM_COPY_HOST_PTR;
	this->context = context;

}

bool COpenCLEffect::StabilizeVideo(Regards::OpenCV::COpenCVStabilization* stabilizationt)
{
	return true;
}

int COpenCLEffect::GetSizeData() const
{
	return sizeof(cl_uint) * 4;
}

/*
CRegardsBitmap* COpenCLEffect::GetPtBitmap()
{
	return GetBitmap(false);
}
*/

int COpenCLEffect::GetWidth()
{
	if (preview && !paramOutput.empty())
	{
		return paramOutput.cols;
	}
	return input.cols;
}

int COpenCLEffect::GetHeight()
{
	if (preview && !paramOutput.empty())
	{
		return paramOutput.rows;
	}
	return input.rows;
}

int COpenCLEffect::HQDn3D(const double& LumSpac, const double& ChromSpac, const double& LumTmp, const double& ChromTmp)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.HQDn3D(LumSpac, ChromSpac, LumTmp, ChromTmp, paramOutput);
		}
		else
		{
			openclFilter.HQDn3D(LumSpac, ChromSpac, LumTmp, ChromTmp, input);
		}
	}
	return 0;
}

int COpenCLEffect::Bm3d(const int& fSigma)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.Bm3d(paramOutput, fSigma);
		}
		else
		{
			 openclFilter.Bm3d(input, fSigma);
		}
	}
	return 0;
}

int COpenCLEffect::WaveFilter(int x, int y, short height, int scale, int radius)
{
	return -1;
}


void COpenCLEffect::SetBitmap(CImageLoadingFormat* bitmap)
{
	if (bitmap != nullptr && bitmap->IsOk())
	{
		CRegardsBitmap* _bitmap = bitmap->GetRegardsBitmap(true);
		cv::Mat local = _bitmap->GetMatrix();
		filename = bitmap->GetFilename();
		local.copyTo(input);
		delete _bitmap;

	}

}

COpenCLEffect::~COpenCLEffect()
{
	input.release();
	paramOutput.release();
}

void COpenCLEffect::CopyPictureToTexture2D(GLTexture * texture, const bool &source)
{
	try
	{
		if (source)
		{
			cv::ogl::convertToGLTexture2D(input, *texture->GetGLTexture());
		}
		else if (preview && !paramOutput.empty())
		{
			cv::ogl::convertToGLTexture2D(paramOutput, *texture->GetGLTexture());
		}
		else
		{
			cv::ogl::convertToGLTexture2D(input, *texture->GetGLTexture());
		}
	}
	catch (...)
	{

	}
}

CRegardsBitmap* COpenCLEffect::GetBitmap(const bool& source)
{
	CRegardsBitmap* bitmapOut = new CRegardsBitmap();

	cv::Mat output;

	if (source)
	{
		input.copyTo(output);
	}
	else if (preview && !paramOutput.empty())
	{
		paramOutput.copyTo(output);
	}
	else
	{
		input.copyTo(output);
	}

	bitmapOut->SetMatrix(output);

	if (bitmapOut != nullptr)
		bitmapOut->SetFilename(filename);
	return bitmapOut;

}
//-----------------------------------------------------------------------------------------------
//Get Output
//-----------------------------------------------------------------------------------------------

wxImage COpenCLEffect::GetwxImage()
{
	cv::Mat output;

	if (preview && !paramOutput.empty())
	{
		paramOutput.copyTo(output);
	}
	else
	{
		input.copyTo(output);
	}

	wxImage anImage(output.cols, output.rows, false);
	unsigned char* dataOut = anImage.GetData();
	if (output.data != nullptr)
	{
		int size = output.cols * output.rows;
		tbb::parallel_for(
			0, size, 1, [=](int i)
			{
				const int y = i / output.cols;
				const int x = i - (y * output.cols);
				const int calcul = (output.rows - y - 1) * output.cols + x;
				dataOut[i * 3] = output.data[(calcul << 2) + 2]; // R
				dataOut[i * 3 + 1] = output.data[(calcul << 2) + 1]; // G
				dataOut[i * 3 + 2] = output.data[(calcul << 2)]; // B
			});
	}

	return anImage;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

int COpenCLEffect::BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP)
{
	CDeepLearning::LockOpenCLDnn();
	
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.BilateralEffect(paramOutput, fSize, sigmaX, sigmaP);
		}
		else
		{
			openclFilter.BilateralEffect(input, fSize, sigmaX, sigmaP);
		}
	}

	CDeepLearning::UnlockOpenCLDnn();
	return 0;
}

int COpenCLEffect::BrightnessAndContrast(const double& brightness, const double& contrast)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.BrightnessAndContrast(brightness, contrast, paramOutput);
		}
		else
		{
			openclFilter.BrightnessAndContrast(brightness, contrast, input);
		}
	}
	return 0;
}

int COpenCLEffect::Swirl(const float& radius, const float& angle)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.Swirl(radius, angle, paramOutput);
		}
		else
		{
			openclFilter.Swirl(radius, angle, input);
		}
	}
	return 0;
}

int COpenCLEffect::NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize, const int& searchWindowSize)
{
	CDeepLearning::LockOpenCLDnn();
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.NlMeans(paramOutput, h, hColor, templateWindowSize, searchWindowSize);
		}
		else
		{
			openclFilter.NlMeans(input, h, hColor, templateWindowSize, searchWindowSize);
		}
	}
	CDeepLearning::UnlockOpenCLDnn();
	return 0;
}

int COpenCLEffect::Posterize(const float& level, const float& gamma)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.Posterize(level, gamma, paramOutput);
		}
		else
		{
			openclFilter.Posterize(level, gamma, input);
		}
	}
	return 0;
}

int COpenCLEffect::MotionBlur(const double& radius, const double& sigma, const double& angle)
{
	//CRegardsBitmap * bitmapOut = new CRegardsBitmap(width, height);
	if (context != nullptr)
	{
		vector<double> kernel;
		vector<wxPoint> offsets;

		if (sigma == 0.0)
			return 0;

		kernel = CMotionBlur::GetMotionBlurKernel(radius, sigma);

		if (kernel.size() < 3)
			return false;

		if (context != nullptr)
		{
			offsets = CMotionBlur::GetOffsetKernel(kernel.size(), angle);
			COpenCLFilter openclFilter(context);
			if (preview && !paramOutput.empty())
			{
				openclFilter.MotionBlurCompute(kernel, offsets, kernel.size(), paramOutput);
			}
			else
			{
				openclFilter.MotionBlurCompute(kernel, offsets, kernel.size(), input);
			}
		}
	}
	//delete bitmapOut;
	return 0;
}


int COpenCLEffect::Median()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.Median(paramOutput);
		}
		else
		{
			openclFilter.Median(input);
		}
	}
	return 0;
}

int COpenCLEffect::Negatif()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.ColorEffect("Negatif", paramOutput);
		}
		else
		{
			openclFilter.ColorEffect("Negatif", input);
		}
	}

	return 0;
}

int COpenCLEffect::BrightnessAndContrastAuto(float clipHistPercent)
{
	CDeepLearning::LockOpenCLDnn();
    try
    {
        if (context != nullptr)
        {
            COpenCLFilter openclFilter(context);
            if (preview && !paramOutput.empty())
            {
                openclFilter.BrightnessAndContrastAuto(paramOutput, clipHistPercent);
            }
            else
            {
                openclFilter.BrightnessAndContrastAuto(input, clipHistPercent);
            }
        }
    }
    catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
	CDeepLearning::UnlockOpenCLDnn();
	return 0;
}

/*
int COpenCLEffect::GetWidth()
{
	if (preview && !paramOutput.empty())
	{
		return paramOutput.cols;
	}
	else
	{
		return input.cols;
	}
}

int COpenCLEffect::GetHeight()
{
	if (preview && !paramOutput.empty())
	{
		return paramOutput.rows;
	}
	else
	{
		return input.rows;
	}
}
*/

int COpenCLEffect::Sepia()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.ColorEffect("Sepia", paramOutput);
		}
		else
		{
			openclFilter.ColorEffect("Sepia", input);
		}

	}

	return 0;
}

int COpenCLEffect::NoirEtBlanc()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.ColorEffect("NoirEtBlanc", paramOutput);
		}
		else
		{
			openclFilter.ColorEffect("NoirEtBlanc", input);
		}

	}

	return 0;
}

int COpenCLEffect::NiveauDeGris()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.ColorEffect("GrayLevel", paramOutput);
		}
		else
		{
			openclFilter.ColorEffect("GrayLevel", input);
		}

	}

	return 0;
}

int COpenCLEffect::FlipVertical()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.Flip("FlipVertical", paramOutput);
		}
		else
		{
			openclFilter.Flip("FlipVertical", input);
		}
	}

	return 0;
}

int COpenCLEffect::FlipHorizontal()
{
	if (context != nullptr)
	{
		COpenCLFilter opencl_filter(context);
		if (preview && !paramOutput.empty())
		{
			opencl_filter.Flip("FlipHorizontal", paramOutput);
		}
		else
		{

			opencl_filter.Flip("FlipHorizontal", input);
		}
	}

	return 0;
}

int COpenCLEffect::Rotate90()
{
	if (context != nullptr)
	{
		int _widthOut;
		int _heightOut;
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			_widthOut = paramOutput.rows;
			_heightOut = paramOutput.cols;
			openclFilter.Rotate("Rotation90", _widthOut, _heightOut, 90.0f, paramOutput);
		}
		else
		{
			_widthOut = input.rows;
			_heightOut = input.cols;
			openclFilter.Rotate("Rotation90", _widthOut, _heightOut, 90.0f, input);
		}
	}

	return 0;
}

int COpenCLEffect::Rotate270()
{
	if (context != nullptr)
	{
		int _widthOut;
		int _heightOut;
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			_widthOut = paramOutput.rows;
			_heightOut = paramOutput.cols;
			openclFilter.Rotate("Rotation270", _widthOut, _heightOut, 270.0f, paramOutput);
		}
		else
		{
			_widthOut = input.rows;
			_heightOut = input.cols;
			openclFilter.Rotate("Rotation270", _widthOut, _heightOut, 270.0f, input);
		}
	}
	return 0;
}


int COpenCLEffect::RotateFree(const double& angle, const int& widthOut, const int& heightOut)
{
	if (context != nullptr)
	{
		int _widthOut;
		int _heightOut;
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			_widthOut = widthOut;
			_heightOut = heightOut;
			openclFilter.Rotate("RotateFree", widthOut, heightOut, angle, paramOutput);
		}
		else
		{
			_widthOut = widthOut;
			_heightOut = heightOut;
			openclFilter.Rotate("RotateFree", widthOut, heightOut, angle, input);
		}
	}
	return 0;
}

int COpenCLEffect::SharpenMasking(const float& sharpness)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.SharpenMasking(sharpness, paramOutput);
		}
		else
		{
			openclFilter.SharpenMasking(sharpness, input);
		}
	}

	return 0;
}

int COpenCLEffect::PhotoFiltre(const CRgbaquad& clValue, const int& intensity)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.PhotoFiltre(clValue, intensity, paramOutput);
		}
		else
		{
			openclFilter.PhotoFiltre(clValue, intensity, input);
		}
	}

	return 0;
}

int COpenCLEffect::Solarize(const long& threshold)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.Solarize(threshold, paramOutput);
		}
		else
		{
			openclFilter.Solarize(threshold, input);
		}
	}

	return 0;
}

int COpenCLEffect::RGBFilter(const int& red, const int& green, const int& blue)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.RGBFilter(red, green, blue, paramOutput);
		}
		else
		{
			openclFilter.RGBFilter(red, green, blue, input);
		}
		
	}

	return 0;
}

int COpenCLEffect::FiltreMosaic()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.FiltreMosaic(paramOutput);
		}
		else
		{
			openclFilter.FiltreMosaic(input);
		}
		
	}

	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int COpenCLEffect::Fusion(CRegardsBitmap* bitmapSecond, const float& pourcentage)
{
	if (context != nullptr)
	{
		cv::UMat second;
		bitmapSecond->GetMatrix().copyTo(second);
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.Fusion(paramOutput, second, pourcentage);
		}
		else
		{
			openclFilter.Fusion(input, second, pourcentage);
		}
		
	}
	return 0;
}

int COpenCLEffect::Soften()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.FiltreConvolution("IDR_OPENCL_SOFTEN", "Soften", paramOutput);
		}
		else
		{
			openclFilter.FiltreConvolution("IDR_OPENCL_SOFTEN", "Soften", input);
		}
		
	}

	return 0;
}

int COpenCLEffect::Noise()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.Noise(paramOutput);
		}
		else
		{
			openclFilter.Noise(input);
		}
		
	}

	return 0;
}

int COpenCLEffect::Blur(const int& radius)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.Blur(radius, paramOutput);
		}
		else
		{
			openclFilter.Blur(radius, input);
		}
		
	}

	return 0;
}

int COpenCLEffect::Emboss()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.FiltreConvolution("IDR_OPENCL_EMBOSS", "Emboss", paramOutput);
		}
		else
		{
			openclFilter.FiltreConvolution("IDR_OPENCL_EMBOSS", "Emboss", input);
		}
		
	}

	return 0;
}


int COpenCLEffect::SharpenStrong()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.FiltreConvolution("IDR_OPENCL_SHARPENSTRONG", "SharpenStrong", paramOutput);
		}
		else
		{
			openclFilter.FiltreConvolution("IDR_OPENCL_SHARPENSTRONG", "SharpenStrong", input);
		}
		
	}

	return 0;
}

int COpenCLEffect::Sharpen()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.FiltreConvolution("IDR_OPENCL_SHARPEN", "Sharpen", paramOutput);
		}
		else
		{
			openclFilter.FiltreConvolution("IDR_OPENCL_SHARPEN", "Sharpen", input);
		}
		
	}

	return 0;
}

int COpenCLEffect::FiltreEdge()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.FiltreConvolution("IDR_OPENCL_EDGE", "Edge", paramOutput);
		}
		else
		{
			openclFilter.FiltreConvolution("IDR_OPENCL_EDGE", "Edge", input);
		}
		
	}

	return 0;
}

int COpenCLEffect::Erode()
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (preview && !paramOutput.empty())
		{
			openclFilter.ErodeDilate("Erode", paramOutput);
		}
		else
		{
			openclFilter.ErodeDilate("Erode", input);
		}
	}
	return 0;
}

int COpenCLEffect::Dilate()
{
	COpenCLFilter openclFilter(context);
	if (preview && !paramOutput.empty())
	{
		openclFilter.ErodeDilate("Dilate", paramOutput);
	}
	else
	{
		openclFilter.ErodeDilate("Dilate", input);
	}
	return 0;
}

int COpenCLEffect::GaussianBlur(const int& r, const int& boxSize)
{
	//CRegardsBitmap * bitmapOut = new CRegardsBitmap(width, height);
	if (context != nullptr)
	{
		if (boxSize > 0)
		{
			const double wIdeal = sqrt((double)(12 * r * r / boxSize + 1));
			int wl = static_cast<int>(floor(wIdeal));
			if (wl % 2 == 0)
				wl--;
			int wu = wl + 2;
			const double mIdeal = (12 * r * r - boxSize * wl * wl - 4 * boxSize * wl - 3 * boxSize) / (-4 * wl - 4);
			int m = round(mIdeal);
			vector<int> gaussCoeff;
			gaussCoeff.reserve(boxSize);
			for (auto i = 0; i < boxSize; i++)
				gaussCoeff.push_back(i < m ? wl : wu);

			bool noDeleteData = true;

			COpenCLFilter opencl_filter(context);
			if (preview && !paramOutput.empty())
			{
				opencl_filter.BoxBlur((gaussCoeff[0] - 1) / 2, "BoxBlurH", paramOutput, noDeleteData);
				noDeleteData = false;
				opencl_filter.BoxBlur((gaussCoeff[0] - 1) / 2, "BoxBlurV", paramOutput, noDeleteData);

				for (auto i = 1; i < boxSize; i++)
				{
					opencl_filter.BoxBlur((gaussCoeff[i] - 1) / 2, "BoxBlurH", paramOutput, noDeleteData);
					opencl_filter.BoxBlur((gaussCoeff[i] - 1) / 2, "BoxBlurV", paramOutput, noDeleteData);
				}
			}
			else
			{
				opencl_filter.BoxBlur((gaussCoeff[0] - 1) / 2, "BoxBlurH", input, noDeleteData);
				noDeleteData = false;
				opencl_filter.BoxBlur((gaussCoeff[0] - 1) / 2, "BoxBlurV", input, noDeleteData);

				for (auto i = 1; i < boxSize; i++)
				{
					opencl_filter.BoxBlur((gaussCoeff[i] - 1) / 2, "BoxBlurH", input, noDeleteData);
					opencl_filter.BoxBlur((gaussCoeff[i] - 1) / 2, "BoxBlurV", input, noDeleteData);
				}
			}
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------------------------
//Execute INTERPOLATION
//-----------------------------------------------------------------------------------------------

void COpenCLEffect::Interpolation(const int& widthOut, const int& heightOut, const int& method, int flipH, int flipV, int angle)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		paramOutput = openclFilter.Interpolation(widthOut, heightOut, "Interpolation", method, input, flipH, flipV, angle);
		preview = true;
	}
}

void COpenCLEffect::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method, int flipH, int flipV, int angle)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		paramOutput = openclFilter.Interpolation(widthOut, heightOut, rc, "InterpolationZone", method, input, flipH, flipV, angle);
		preview = true;
	}
}