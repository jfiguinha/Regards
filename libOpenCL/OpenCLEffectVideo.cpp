// ReSharper disable All
#include <header.h>
#include "OpenCLEffectVideo.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include <RegardsBitmap.h>
#include "utility.h"
#include "EffectVideoParameter.h"
#include "OpenCLFilter.h"
#include "hqdn3d.h"
#include <VideoStabilization.h>
#include <OpenCLContext.h>
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

extern COpenCLContext* openclContext;

COpenCLEffectVideo::COpenCLEffectVideo()
{
	openCLProgram = nullptr;
	bool useMemory = (openclContext->GetDeviceType() == CL_DEVICE_TYPE_GPU) ? false : true;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	openCLProgram = nullptr;
	openclFilter = new COpenCLFilter();
}

void COpenCLEffectVideo::SetMatrix(cv::UMat& frame)
{
	if (frame.channels() == 4)
		cv::cvtColor(frame, paramSrc, cv::COLOR_BGRA2BGR);
	else
		paramSrc = frame;

	needToTranscode = false;
	isOk = true;
}



void COpenCLEffectVideo::SetMatrix(cv::Mat& frame)
{
	if (frame.channels() == 4)
		cv::cvtColor(frame, paramSrc, cv::COLOR_BGRA2BGR);
	else
		frame.copyTo(paramSrc);

	needToTranscode = false;
	isOk = true;

}

cv::Mat COpenCLEffectVideo::GetMatrix(const bool & src)
{
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

	cv::cvtColor(output, output, cv::COLOR_BGR2BGRA);

	return output;
}

bool COpenCLEffectVideo::convertToGLTexture2D(GLTexture* glTexture)
{
	bool isOk = false;
	if (interpolatePicture && !paramOutput.empty())
	{
		glTexture->SetData(paramOutput);
		//isOk = openclFilter->convertToGLTexture2D(paramOutput, glTexture);
	}
	else
	{
		glTexture->SetData(paramSrc);
		//isOk = openclFilter->convertToGLTexture2D(paramSrc, glTexture);
	}
	return isOk;
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

void COpenCLEffectVideo::ConvertToBgr()
{
	if(!paramSrc.empty())
		cvtColor(paramSrc, paramSrc, cv::COLOR_RGBA2BGRA);
}

void COpenCLEffectVideo::Rotate(CVideoEffectParameter* videoEffectParameter)
{
	if (videoEffectParameter->rotation != 0)
	{
		if (videoEffectParameter->rotation == 90 || videoEffectParameter->rotation == -270)
		{
			cv::rotate(paramSrc, paramSrc, cv::ROTATE_90_CLOCKWISE);
		}
		else if (videoEffectParameter->rotation == 270 || videoEffectParameter->rotation == -90)
		{
			cv::rotate(paramSrc, paramSrc, cv::ROTATE_90_COUNTERCLOCKWISE);
		}
		else if (videoEffectParameter->rotation == 180)
		{
			cv::rotate(paramSrc, paramSrc, cv::ROTATE_180);
		}
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
		openclFilter->BrightnessAndContrastAuto(paramSrc,1.0);
		frameStabilized = true;
	}

}


void COpenCLEffectVideo::InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect &rc, const int &flipH, const int &flipV, const int& angle, const int& bicubic, int ratio)
{
	if (openclContext != nullptr && !paramSrc.empty())
	{
		
		paramOutput = openclFilter->Interpolation(widthOutput, heightOutput, rc, bicubic, paramSrc, flipH, flipV, angle, ratio);
		interpolatePicture = true;
	}
}


void COpenCLEffectVideo::LoadRegardsBitmap(CRegardsBitmap * bitmap)
{
	if (bitmap != nullptr)
	{
		cv::Mat local = bitmap->GetMatrix();
		filename = bitmap->GetFilename();
		cv::cvtColor(local, paramSrc, cv::COLOR_BGRA2BGR);

	}
	interpolatePicture = false;
	needToTranscode = false;

}

void COpenCLEffectVideo::AutoContrast()
{
	if (interpolatePicture)
	{
		openclFilter->BrightnessAndContrastAuto(paramOutput,1.0);
	}
	else
	{
		openclFilter->BrightnessAndContrastAuto(paramSrc,1.0);
	}
	
}

CRegardsBitmap* COpenCLEffectVideo::GetBitmap(const bool &src)
{
	CRegardsBitmap* bitmapOut = new CRegardsBitmap();

	if (src)
	{
		bitmapOut->SetMatrix(paramSrc);
	}
	else if (interpolatePicture)
	{
		bitmapOut->SetMatrix(paramOutput);
	}
	else
	{
		bitmapOut->SetMatrix(paramSrc);
	}


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

void COpenCLEffectVideo::SetNV12(const cv::Mat& yuv)
{
	cv::cvtColor(yuv, paramSrc, cv::COLOR_YUV2BGR_NV12);
}

void COpenCLEffectVideo::SetYUV420P(const cv::Mat& y, const cv::Mat& u, const cv::Mat& v, const int& linesize, const int& nWidth, const int& nHeight)
{
	cv::Mat u_resized, v_resized;
	cv::resize(u, u_resized, cv::Size(linesize, nHeight), 0, 0, cv::INTER_NEAREST); //repeat u values 4 times
	cv::resize(v, v_resized, cv::Size(linesize, nHeight), 0, 0, cv::INTER_NEAREST); //repeat v values 4 times

	cv::Mat yuv;

	std::vector<cv::Mat> yuv_channels = { y,u_resized, v_resized };
	cv::merge(yuv_channels, yuv);

	

	if (nWidth != linesize)
	{
		cv::UMat out;
		cv::cvtColor(yuv, out, cv::COLOR_YUV2BGR);
		out(cv::Rect(0, 0, nWidth, nHeight)).copyTo(paramSrc);
	}
	else
	{
		cv::cvtColor(yuv, paramSrc, cv::COLOR_YUV2BGR);
	}
};

void COpenCLEffectVideo::GetYUV420P(uint8_t * & y, uint8_t * & u, uint8_t * & v, const int & nWidth, const int & nHeight)
{

	cv::Mat _y = cv::Mat(cv::Size(nWidth, nHeight), CV_8UC1, y);
	cv::Mat _u = cv::Mat(cv::Size(nWidth / 2, nHeight / 2), CV_8UC1, u);
	cv::Mat _v = cv::Mat(cv::Size(nWidth / 2, nHeight / 2), CV_8UC1, v);

	cv::UMat src;
	if (interpolatePicture)
	{

		src = paramOutput;
	}
	else
	{
		src = paramSrc;
	}

	cv::UMat ycbcr;


	cvtColor(src, ycbcr, cv::COLOR_BGR2YUV);
	vector<cv::UMat> yuv;
	cv::split(ycbcr, yuv);
	yuv[0].copyTo(_y);

	cv::resize(yuv[1], _u, cv::Size(nWidth / 2, nHeight / 2), 0, 0, cv::INTER_NEAREST); //repeat u values 4 times
	cv::resize(yuv[2], _v, cv::Size(nWidth / 2, nHeight / 2), 0, 0, cv::INTER_NEAREST); //repeat v values 4 times
	

	/*
	int middleWidth = widthOut / 2;
	int middleHeight = heightOut / 2;

	COpenCLParameterByteArray *	inputY = new COpenCLParameterByteArray();
	inputY->SetLibelle("inputY");
	inputY->SetNoDelete(true);
	inputY->SetValue(openclContext->GetContext(), y, widthOut * heightOut, flag);

	COpenCLParameterByteArray *	inputU = new COpenCLParameterByteArray();
	inputU->SetLibelle("inputU");
	inputU->SetNoDelete(true);
	inputU->SetValue(openclContext->GetContext(), u, middleWidth * middleHeight, flag);

	COpenCLParameterByteArray *	inputV = new COpenCLParameterByteArray();
	inputV->SetNoDelete(true);
	inputV->SetLibelle("inputV");
	inputV->SetValue(openclContext->GetContext(), v, middleWidth * middleHeight, flag);

	COpenCLParameterInt *	paramWidth = new COpenCLParameterInt();
	paramWidth->SetNoDelete(true);
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(widthOut);

	COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
	paramHeight->SetNoDelete(true);
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(heightOut);

	cv::UMat cvDest;

	if (interpolatePicture)
	{
		cv::cvtColor(paramOutput, cvDest, cv::COLOR_BGR2BGRA);
	}
	else
	{
		cv::cvtColor(paramSrc, cvDest, cv::COLOR_BGR2BGRA);
	}

	//paramSrc->SetNoDelete(true);

	if (openclContext != nullptr)
	{
		COpenCLProgram * programCL = nullptr;

		programCL = GetProgram("IDR_OPENCL_CONVERTTOY");

		if (programCL != nullptr)
		{
			cl_mem clBuffer;
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(flag);

			vecParam.push_back(inputY);
			vecParam.push_back(inputU);
			vecParam.push_back(inputV);

			clBuffer = (cl_mem)cvDest.handle(cv::ACCESS_RW);


			COpenCLParameterClMem * inputSrc = new COpenCLParameterClMem();
			inputSrc->SetNoDelete(true);
			inputSrc->SetLibelle("input");
			inputSrc->SetValue(clBuffer);
			vecParam.push_back(inputSrc);

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

			cl_int err = clEnqueueReadBuffer(openclContext->GetCommandQueue(), inputY->GetValue(), CL_TRUE, 0, widthOut * heightOut, y, 0, nullptr, nullptr);
			Error::CheckError(err);
			err = clFinish(openclContext->GetCommandQueue());
			Error::CheckError(err);

			err = clEnqueueReadBuffer(openclContext->GetCommandQueue(), inputU->GetValue(), CL_TRUE, 0, middleWidth * middleHeight, u, 0, nullptr, nullptr);
			Error::CheckError(err);
			err = clFinish(openclContext->GetCommandQueue());
			Error::CheckError(err);

			err = clEnqueueReadBuffer(openclContext->GetCommandQueue(), inputV->GetValue(), CL_TRUE, 0, middleWidth * middleHeight, v, 0, nullptr, nullptr);
			Error::CheckError(err);
			err = clFinish(openclContext->GetCommandQueue());
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
	*/
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
	cv::UMat ycbcr;
	cv::Mat yChannel;
	int width = 0;
	int height = 0;

	if (interpolatePicture)
	{
		width = paramOutput.cols;
		height = paramOutput.rows;
		cvtColor(paramOutput, ycbcr, cv::COLOR_BGR2YCrCb);
	}
	else
	{
		width = paramSrc.cols;
		height = paramSrc.rows;
		cvtColor(paramSrc, ycbcr, cv::COLOR_BGR2YCrCb);

	}

	

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
	

	if (interpolatePicture)
	{
		cv::cvtColor(ycbcr, paramOutput, cv::COLOR_YCrCb2BGR);
	}
	else
	{
		cv::cvtColor(ycbcr, paramSrc, cv::COLOR_YCrCb2BGR);
	}

	ycbcr.release();
	yChannel.release();
}

bool COpenCLEffectVideo::IsOk()
{
	if (interpolatePicture)
		return !paramOutput.empty();
	return !paramSrc.empty();
}

COpenCLProgram * COpenCLEffectVideo::GetProgram(const wxString &numProgram)
{
	if (openclContext != nullptr)
		return openclContext->GetProgram(numProgram, openclContext->GetDefaultType());
	return nullptr;
}


int COpenCLEffectVideo::GetSizeData()
{
	//For video is float if is shared context
	//if (context->GetDefaultType() == OPENCL_FLOAT)
	//	return sizeof(cl_float) * 4;

	return sizeof(cl_uint);
}
