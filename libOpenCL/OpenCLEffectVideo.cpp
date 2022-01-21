// ReSharper disable All
#include <header.h>
#include "OpenCLEffectVideo.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/opengl.hpp>
#include <opencv2/core/ocl.hpp>
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include <RegardsBitmap.h>
#include "utility.h"
#include "EffectVideoParameter.h"
#include "OpenclFilter.h"
#include "hqdn3d.h"
#include <OpenCVEffect.h>
#include <VideoStabilization.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <OpenGL/OpenGL.h>
#elif defined(__WXGTK__)
#include <GL/glx.h>
#include <CL/cl_gl.h>
#else
#include <CL/cl_gl.h>
#endif
//#include <RegardsConfigParam.h>
//#include <ParamInit.h>
using namespace Regards::OpenCL;
using namespace Regards::OpenCV;

COpenCLEffectVideo::COpenCLEffectVideo(COpenCLContext * context)
{
	openCLProgram = nullptr;
	bool useMemory = (context->GetDeviceType() == CL_DEVICE_TYPE_GPU) ? false : true;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	openCLProgram = nullptr;
	this->context = context;
	openclFilter = new COpenCLFilter(context);
}

int COpenCLEffectVideo::GetDataSizeWidth(const bool &src)
{
	return paramSrc.cols * GetSizeData();
}

COpenCLEffectVideo::~COpenCLEffectVideo()
{
	if (openCLProgram != nullptr)
		delete openCLProgram;

	openCLProgram = nullptr;

	if (openclFilter != nullptr)
		delete openclFilter;
}



void COpenCLEffectVideo::CopyPictureToTexture2D(GLTexture* texture, const bool& source)
{
	try
	{
		if (source)
		{
			cv::ogl::convertToGLTexture2D(paramSrc, *texture->GetGLTexture());
		}
		else if (interpolatePicture && !paramOutput.empty())
		{
			cv::ogl::convertToGLTexture2D(paramOutput, *texture->GetGLTexture());
		}
		else
		{
			cv::ogl::convertToGLTexture2D(paramSrc, *texture->GetGLTexture());
		}
	}
	catch (...)
	{

	}
}

void COpenCLEffectVideo::ApplyOpenCVEffect(CVideoEffectParameter * videoEffectParameter, COpenCVStabilization * openCVStabilization)
{
	//context->GetContextForOpenCV().bind();
	bool frameStabilized = false;

	if (videoEffectParameter->stabilizeVideo)
	{
		if (openCVStabilization == nullptr)
			openCVStabilization = new COpenCVStabilization(videoEffectParameter->stabilizeImageBuffere);

		openCVStabilization->SetNbFrameBuffer(videoEffectParameter->stabilizeImageBuffere);

		if (openCVStabilization->GetNbFrameBuffer() == 0)
		{
			openCVStabilization->BufferFrame(paramSrc);
		}
		else
		{
			frameStabilized = true;
			openCVStabilization->AddFrame(paramSrc);
		}

		if (frameStabilized)
		{
			cv::UMat image_local = openCVStabilization->CorrectFrame(paramSrc);
			image_local.copyTo(paramSrc);
			image_local.release();
		}
	}

	if (videoEffectParameter->autoConstrast)
	{
		Regards::OpenCV::COpenCVEffect::BrightnessAndContrastAuto(paramSrc);
		frameStabilized = true;
	}
}


void COpenCLEffectVideo::InterpolationBicubic(const int& widthOutput, const int& heightOutput, const int &flipH, const int &flipV, const int& angle, const int& bicubic)
{
	if (context != nullptr && !paramSrc.empty())
	{
		
		paramOutput = openclFilter->Interpolation(widthOutput, heightOutput, "Interpolation", bicubic, paramSrc, flipH, flipV, angle);
		interpolatePicture = true;
	}
}

void COpenCLEffectVideo::InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect &rc, const int &flipH, const int &flipV, const int& angle, const int& bicubic)
{
	if (context != nullptr && !paramSrc.empty())
	{
		
		paramOutput = openclFilter->Interpolation(widthOutput, heightOutput, rc, "InterpolationZone", bicubic, paramSrc, flipH, flipV, angle);
		interpolatePicture = true;
	}
}


void COpenCLEffectVideo::LoadRegardsBitmap(CRegardsBitmap * bitmap)
{
	if (bitmap != nullptr)
	{
		cv::Mat local = bitmap->GetMatrix();
		filename = bitmap->GetFilename();
		local.copyTo(paramSrc);

	}
	interpolatePicture = false;
	needToTranscode = false;

}

void COpenCLEffectVideo::AutoContrast()
{
	if (interpolatePicture)
	{
		COpenCVEffect::BrightnessAndContrastAuto(paramOutput);
	}
	else
	{
		COpenCVEffect::BrightnessAndContrastAuto(paramSrc);
	}
	
}

CRegardsBitmap* COpenCLEffectVideo::GetBitmap(const bool &src)
{
	CRegardsBitmap* bitmapOut = new CRegardsBitmap();

	cv::Mat output;

	if (src)
	{
		paramSrc.copyTo(output);
	}
	else if (interpolatePicture)
	{
		paramOutput.copyTo(output);
	}
	else
	{
		paramSrc.copyTo(output);
	}

	bitmapOut->SetMatrix(output);

	return bitmapOut;

}

void COpenCLEffectVideo::ApplyVideoEffect(CVideoEffectParameter * videoEffectParameter)
{

	

	if (videoEffectParameter->ColorBoostEnable)
	{
		if (interpolatePicture)
		{
			openclFilter->RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1], videoEffectParameter->color_boost[2], paramOutput);
		}
		else
		{
			openclFilter->RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1], videoEffectParameter->color_boost[2],paramSrc);
		}
	}
	if (videoEffectParameter->bandcEnable)
	{
		if (interpolatePicture)
		{
			openclFilter->BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast, paramOutput);
		}
		else
		{
			openclFilter->BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast,paramSrc);
		}
	}
	if (videoEffectParameter->SharpenEnable)
	{
		if (interpolatePicture)
		{
			openclFilter->SharpenMasking(videoEffectParameter->sharpness, paramOutput);
		}
		else
		{
			openclFilter->SharpenMasking(videoEffectParameter->sharpness,paramSrc);
		}
	}
	if (videoEffectParameter->denoiseEnable)
	{
		if (interpolatePicture)
		{
			openclFilter->NlMeans(paramOutput, videoEffectParameter->h, videoEffectParameter->hColor, videoEffectParameter->templateWindowSize, videoEffectParameter->searchWindowSize);
		}
		else
		{
			openclFilter->NlMeans(paramSrc, videoEffectParameter->h, videoEffectParameter->hColor, videoEffectParameter->templateWindowSize, videoEffectParameter->searchWindowSize);
		}
	}
	if (videoEffectParameter->sepiaEnable)
	{
		if (interpolatePicture)
		{
			openclFilter->ColorEffect("Sepia", paramOutput);
		}
		else
		{
			openclFilter->ColorEffect("Sepia", paramSrc);
		}
	}
	if (videoEffectParameter->grayEnable)
	{
		if (interpolatePicture)
		{
			openclFilter->ColorEffect("GrayLevel", paramOutput);
		}
		else
		{
			openclFilter->ColorEffect("GrayLevel", paramSrc);
		}
	}
	if (videoEffectParameter->filmgrainenable)
	{
		if (interpolatePicture)
		{
			openclFilter->Noise(paramOutput);
		}
		else
		{
			openclFilter->Noise(paramSrc);
		}
	}

}


void COpenCLEffectVideo::GetYUV420P(uint8_t * & y, uint8_t * & u, uint8_t * & v, const int &widthOut, const int &heightOut)
{
	int middleWidth = widthOut / 2;
	int middleHeight = heightOut / 2;

	COpenCLParameterByteArray *	inputY = new COpenCLParameterByteArray();
	inputY->SetLibelle("inputY");
	inputY->SetNoDelete(true);
	inputY->SetValue(context->GetContext(), y, widthOut * heightOut, flag);

	COpenCLParameterByteArray *	inputU = new COpenCLParameterByteArray();
	inputU->SetLibelle("inputU");
	inputU->SetNoDelete(true);
	inputU->SetValue(context->GetContext(), u, middleWidth * middleHeight, flag);

	COpenCLParameterByteArray *	inputV = new COpenCLParameterByteArray();
	inputV->SetNoDelete(true);
	inputV->SetLibelle("inputV");
	inputV->SetValue(context->GetContext(), v, middleWidth * middleHeight, flag);

	COpenCLParameterInt *	paramWidth = new COpenCLParameterInt();
	paramWidth->SetNoDelete(true);
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(widthOut);

	COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
	paramHeight->SetNoDelete(true);
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(heightOut);

	//paramSrc->SetNoDelete(true);

	if (context != nullptr)
	{
		COpenCLProgram * programCL = nullptr;

		programCL = GetProgram("IDR_OPENCL_CONVERTTOY");

		if (programCL != nullptr)
		{
			cl_mem clBuffer;
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			vecParam.push_back(inputY);
			vecParam.push_back(inputU);
			vecParam.push_back(inputV);
			if (interpolatePicture)
			{
				clBuffer = (cl_mem)paramOutput.handle(cv::ACCESS_RW);
			}
			else
			{
				clBuffer = (cl_mem)paramSrc.handle(cv::ACCESS_RW);
			}

			COpenCLParameterClMem * paramSrc = new COpenCLParameterClMem();
			paramSrc->SetNoDelete(true);
			paramSrc->SetLibelle("input");
			paramSrc->SetValue(clBuffer);


			vecParam.push_back(paramWidth);
			vecParam.push_back(paramHeight);

			program->ExecuteProgram2D(programCL->GetProgram(), "RgbaToYUV420P", &vecParam, middleWidth, middleHeight);

			delete program;

			for (COpenCLParameter * parameter : vecParam)
			{
				if (!parameter->GetNoDelete())
				{
					delete parameter;
					parameter = nullptr;
				}
			}
			vecParam.clear();

			cl_int err = clEnqueueReadBuffer(context->GetCommandQueue(), inputY->GetValue(), CL_TRUE, 0, widthOut * heightOut, y, 0, nullptr, nullptr);
			Error::CheckError(err);
			err = clFinish(context->GetCommandQueue());
			Error::CheckError(err);

			err = clEnqueueReadBuffer(context->GetCommandQueue(), inputU->GetValue(), CL_TRUE, 0, middleWidth * middleHeight, u, 0, nullptr, nullptr);
			Error::CheckError(err);
			err = clFinish(context->GetCommandQueue());
			Error::CheckError(err);

			err = clEnqueueReadBuffer(context->GetCommandQueue(), inputV->GetValue(), CL_TRUE, 0, middleWidth * middleHeight, v, 0, nullptr, nullptr);
			Error::CheckError(err);
			err = clFinish(context->GetCommandQueue());
			Error::CheckError(err);
		}
	}
	if (inputY != nullptr)
	{
		inputY->Release();
		delete inputY;
	}

	if (inputU != nullptr)
	{
		inputU->Release();
		delete inputU;
	}

	if (inputV != nullptr)
	{
		inputV->Release();
		delete inputV;
	}

	if (paramWidth != nullptr)
	{
		paramWidth->Release();
		delete paramWidth;
	}

	if (paramHeight != nullptr)
	{
		paramHeight->Release();
		delete paramHeight;
	}
}


void COpenCLEffectVideo::FlipVertical()
{
	
	if (interpolatePicture)
	{
		openclFilter->Flip("FlipVertical", paramOutput);

	}
	else
	{
		openclFilter->Flip("FlipVertical", paramSrc);
	}
}

void COpenCLEffectVideo::HQDn3D(Chqdn3d * hq3d, const double & LumSpac, const double & ChromSpac, const double & LumTmp, const double & ChromTmp)
{
	cv::UMat cvDest;
	cv::UMat ycbcr;
	cv::Mat yChannel;
	int width = 0;
	int height = 0;

	if (interpolatePicture)
	{
		width = paramOutput.cols;
		height = paramOutput.rows;
		cv::cvtColor(paramOutput, cvDest, cv::COLOR_BGRA2BGR);
	}
	else
	{
		width = paramSrc.cols;
		height = paramSrc.rows;

		cv::cvtColor(paramSrc, cvDest, cv::COLOR_BGRA2BGR);

	}

	cvtColor(cvDest, ycbcr, cv::COLOR_BGR2YCrCb);

	std::vector<cv::Mat> planes(3);
	cv::split(ycbcr, planes);

	// Extract the Y channel
	//cv::extractChannel(ycbcr, yChannel, 0);

	uint8_t* dataOut = hq3d->ApplyDenoise3D(planes[0].data, width, height);

	memcpy(planes[0].data, dataOut, width * height);

	// Merge the the color planes back into an Lab image
	//cv::insertChannel(yChannel, ycbcr, 0);
	cv::merge(planes, ycbcr);
	// convert back to RGB
	cv::cvtColor(ycbcr, cvDest, cv::COLOR_YCrCb2BGR);

	if (interpolatePicture)
	{
		cv::cvtColor(cvDest, paramOutput, cv::COLOR_BGR2BGRA);
	}
	else
	{
		cv::cvtColor(cvDest, paramSrc, cv::COLOR_BGR2BGRA);
	}
}

bool COpenCLEffectVideo::IsOk()
{
	if (interpolatePicture)
		return !paramOutput.empty();
	return !paramSrc.empty();
}

COpenCLProgram * COpenCLEffectVideo::GetProgram(const wxString &numProgram)
{
	if (context != nullptr)
		return context->GetProgram(numProgram, context->GetDefaultType());
	return nullptr;
}


int COpenCLEffectVideo::GetSizeData()
{
	//For video is float if is shared context
	//if (context->GetDefaultType() == OPENCL_FLOAT)
	//	return sizeof(cl_float) * 4;

	return sizeof(cl_uint);
}
