// ReSharper disable All
#include <header.h>
#include "CudaEffectVideo.h"
#include "utility.h"
#include "EffectVideoParameter.h"
#include "CudaFilter.h"
#include "hqdn3d.h"
#include "VideoStabilization.h"
#include <FaceDetector.h>
#ifndef __APPLE__
#include "opencv2/cudaimgproc.hpp"
#include <opencv2/cudaarithm.hpp>
#include "opencv2/cudawarping.hpp"
#endif
using namespace Regards::Cuda;
using namespace Regards::OpenCV;
extern string platformName;


CCudaEffectVideo::CCudaEffectVideo()
{
	cudaFilter = new CCudaFilter();

}

void CCudaEffectVideo::SetMatrix(cv::cuda::GpuMat& frame)
{

	if (frame.channels() == 4)
		cv::cuda::cvtColor(frame, paramSrc, cv::COLOR_BGRA2BGR);
	else
		paramSrc = frame;

	needToTranscode = false;
	isOk = true;
}

void CCudaEffectVideo::SetMatrix(cv::UMat& frame)
{
	if (frame.channels() == 4)
		cv::cuda::cvtColor(frame, paramSrc, cv::COLOR_BGRA2BGR);
	else
		frame.copyTo(paramSrc);

	needToTranscode = false;
	isOk = true;
}

cv::cuda::GpuMat CCudaEffectVideo::GetGpuMat(const bool& src)
{
	cv::cuda::GpuMat output;

	if (src)
	{
		cv::cuda::cvtColor(paramSrc, output, cv::COLOR_BGR2RGBA);
	}
	else if (interpolatePicture)
	{
		cv::cuda::cvtColor(paramOutput, output, cv::COLOR_BGR2RGBA);
	}
	else
	{
		cv::cuda::cvtColor(paramSrc, output, cv::COLOR_BGR2RGBA);
	}


	return output;
}

cv::UMat CCudaEffectVideo::GetUMat(const bool& src)
{
	cv::UMat output;

	if (src)
	{
		cv::cuda::cvtColor(paramSrc, output, cv::COLOR_BGR2RGBA);
	}
	else if (interpolatePicture)
	{
		cv::cuda::cvtColor(paramOutput, output, cv::COLOR_BGR2RGBA);
	}
	else
	{
		cv::cuda::cvtColor(paramSrc, output, cv::COLOR_BGR2RGBA);
	}


	return output;
}

void CCudaEffectVideo::SetMatrix(cv::Mat& frame)
{


	if (frame.channels() == 4)
	{
		cv::cuda::GpuMat mat;
		mat.upload(frame);
		cv::cuda::cvtColor(mat, paramSrc, cv::COLOR_BGRA2BGR);
	}
	else
		paramSrc.upload(frame);

	needToTranscode = false;
	isOk = true;
}

cv::Mat CCudaEffectVideo::GetMatrix(const bool& src)
{
	cv::cuda::GpuMat convert;
	cv::Mat output;

	if (src)
	{
		cv::cuda::cvtColor(paramSrc, convert, cv::COLOR_BGR2BGRA);
		//paramSrc.copyTo(output);
	}
	else if (interpolatePicture)
	{
		cv::cuda::cvtColor(paramOutput, convert, cv::COLOR_BGR2BGRA);
		//paramOutput.copyTo(output);
	}
	else
	{
		cv::cuda::cvtColor(paramSrc, convert, cv::COLOR_BGR2BGRA);

	}

	convert.download(output);

	//convert.copyTo(output);

	return output;
}

CCudaEffectVideo::~CCudaEffectVideo()
{
	if (cudaFilter != nullptr)
		delete cudaFilter;

	if (hq3d != nullptr)
		delete hq3d;
}

void CCudaEffectVideo::ConvertToBgr()
{

	if (!paramSrc.empty())
		cv::cuda::cvtColor(paramSrc, paramSrc, cv::COLOR_RGBA2BGRA);

}

void CCudaEffectVideo::Rotate(CVideoEffectParameter* videoEffectParameter)
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


void CCudaEffectVideo::ApplyStabilization(CVideoEffectParameter* videoEffectParameter,
	Regards::OpenCV::COpenCVStabilization * openCVStabilization)
{

	bool frameStabilized = false;

	if (videoEffectParameter->stabilizeVideo)
	{
		if (openCVStabilization == nullptr)
			openCVStabilization = new Regards::OpenCV::COpenCVStabilization(videoEffectParameter->stabilizeImageBuffere);

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
			cv::cuda::GpuMat image_local = openCVStabilization->CorrectFrame(paramSrc);
			image_local.copyTo(paramSrc);
			image_local.release();
		}
	}
}


void CCudaEffectVideo::ApplyOpenCVEffect(CVideoEffectParameter* videoEffectParameter)
{

	if (videoEffectParameter->autoConstrast)
	{
		if (interpolatePicture)
		{
			cudaFilter->BrightnessAndContrastAuto(paramOutput, 1.0);
		}
		else
		{
			cudaFilter->BrightnessAndContrastAuto(paramSrc, 1.0);
		}
	}

	if (videoEffectParameter->filmEnhance || videoEffectParameter->filmcolorisation)
	{
		cv::Mat img_up;
		cv::Mat image;

		if (interpolatePicture)
		{
			paramOutput.copyTo(image);
		}
		else
		{
			paramSrc.copyTo(image);
		}


		if (videoEffectParameter->filmEnhance)
		{
			image = CFaceDetector::SuperResolution(image);
		}
		if (videoEffectParameter->filmcolorisation)
		{

			image = CFaceDetector::Colorisation(image);
		}

		if (interpolatePicture)
		{
			image.copyTo(paramOutput);
		}
		else
		{
			image.copyTo(paramSrc);
		}
	}
}



void CCudaEffectVideo::InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect& rc,
	const int& flipH, const int& flipV, const int& angle,
	const int& bicubic, int ratio)
{
	if (!paramSrc.empty())
	{


		paramOutput = cudaFilter->Interpolation(widthOutput, heightOutput, rc, bicubic, paramSrc, flipH, flipV, angle,
			ratio);
		interpolatePicture = true;
	}
}

void CCudaEffectVideo::AutoContrast()
{

	if (interpolatePicture)
	{
		cudaFilter->BrightnessAndContrastAuto(paramOutput, 1.0);
	}
	else
	{
		cudaFilter->BrightnessAndContrastAuto(paramSrc, 1.0);
	}
}


void CCudaEffectVideo::ApplyVideoEffect(CVideoEffectParameter* videoEffectParameter)
{


	if (videoEffectParameter->ColorBoostEnable)
	{
		if (interpolatePicture)
		{
			cudaFilter->RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1],
				videoEffectParameter->color_boost[2], paramOutput);
		}
		else
		{
			cudaFilter->RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1],
				videoEffectParameter->color_boost[2], paramSrc);
		}
	}
	if (videoEffectParameter->bandcEnable)
	{
		if (interpolatePicture)
		{
			cudaFilter->BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast,
				paramOutput);
		}
		else
		{
			cudaFilter->BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast,
				paramSrc);
		}
	}
	if (videoEffectParameter->SharpenEnable)
	{
		if (interpolatePicture)
		{
			cudaFilter->SharpenMasking(videoEffectParameter->sharpness, paramOutput);
		}
		else
		{
			cudaFilter->SharpenMasking(videoEffectParameter->sharpness, paramSrc);
		}
	}
	if (videoEffectParameter->denoiseEnable)
	{
		if (interpolatePicture)
		{
			cudaFilter->NlMeans(paramOutput, videoEffectParameter->h, videoEffectParameter->hColor,
				videoEffectParameter->templateWindowSize, videoEffectParameter->searchWindowSize);
		}
		else
		{
			cudaFilter->NlMeans(paramSrc, videoEffectParameter->h, videoEffectParameter->hColor,
				videoEffectParameter->templateWindowSize, videoEffectParameter->searchWindowSize);
		}
	}
	if (videoEffectParameter->sepiaEnable)
	{
		if (interpolatePicture)
		{
			cudaFilter->ColorEffect("Sepia", paramOutput);
		}
		else
		{
			cudaFilter->ColorEffect("Sepia", paramSrc);
		}
	}
	if (videoEffectParameter->grayEnable)
	{
		if (interpolatePicture)
		{
			cudaFilter->ColorEffect("GrayLevel", paramOutput);
		}
		else
		{
			cudaFilter->ColorEffect("GrayLevel", paramSrc);
		}
	}
	if (videoEffectParameter->filmgrainenable)
	{
		if (interpolatePicture)
		{
			cudaFilter->Noise(paramOutput);
		}
		else
		{
			cudaFilter->Noise(paramSrc);
		}
	}
}

void CCudaEffectVideo::SetAVFrame(CVideoEffectParameter* videoEffectParameter, AVFrame*& tmp_frame, int colorSpace, int isLimited)
{
	int nWidth = tmp_frame->width;
	int nHeight = tmp_frame->height;

	if (tmp_frame->format == AV_PIX_FMT_NV12)
	{
		try
		{

			int sizeData = (nHeight + nHeight / 2) * tmp_frame->linesize[0];
			if (sizeData != sizesrc && src != nullptr)
			{
				delete[] src;
				src = nullptr;
			}

			if (src == nullptr)
			{
				src = new uint8_t[sizeData];
				sizesrc = sizeData;
			}

			int size = nHeight * tmp_frame->linesize[0];
			memcpy(src, tmp_frame->data[0], size);
			memcpy(src + size, tmp_frame->data[1], (tmp_frame->linesize[0] * (nHeight / 2)));
			cv::cuda::GpuMat yuv = cv::cuda::GpuMat(nHeight + nHeight / 2, tmp_frame->linesize[0], CV_8UC1, src);

			cv::cuda::cvtColor(yuv, paramSrc, cv::COLOR_RGBA2BGR);
			//openclEffectVideo.SetNV12(yuv, tmp_frame->linesize[0], nWidth, nHeight);
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}
	}
	else if (tmp_frame->format == AV_PIX_FMT_YUV420P)
	{
		try
		{
			//cv::cuda::GpuMat _y;
			cv::Mat y = cv::Mat(cv::Size(tmp_frame->linesize[0], nHeight), CV_8UC1, tmp_frame->data[0]);
			cv::Mat u = cv::Mat(cv::Size(tmp_frame->linesize[1], nHeight / 2), CV_8UC1, tmp_frame->data[1]);
			cv::Mat v = cv::Mat(cv::Size(tmp_frame->linesize[2], nHeight / 2), CV_8UC1, tmp_frame->data[2]);
			//openclEffectVideo.SetYUV420P(y, u, v, tmp_frame->linesize[0], nWidth, nHeight);

			int linesize = tmp_frame->linesize[0];
			cv::cuda::GpuMat u_resized, v_resized, _y;

			_y.upload(y);
			u_resized.upload(u);
			v_resized.upload(v);


			cv::cuda::resize(u_resized, u_resized, cv::Size(linesize, nHeight)); //repeat u values 4 times
			cv::cuda::resize(v_resized, v_resized, cv::Size(linesize, nHeight)); //repeat v values 4 times
			
			cv::cuda::GpuMat yuv;
			//cv::cuda::GpuMat out;

			//y.copyTo(_y);

			std::vector<cv::cuda::GpuMat> yuv_channels = { _y, u_resized, v_resized };
			cv::cuda::merge(yuv_channels, yuv);

			if (nWidth != linesize)
			{
				cv::cuda::GpuMat out;
				cv::cuda::cvtColor(yuv, out, cv::COLOR_YUV2RGB);
				out(cv::Rect(0, 0, nWidth, nHeight)).copyTo(paramSrc);
			}
			else
			{
				cv::cuda::cvtColor(yuv, paramSrc, cv::COLOR_YUV2RGB);
				//out.copyTo(paramSrc);
			}
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}
	}
}


void CCudaEffectVideo::GetYUV420P(uint8_t*& y, uint8_t*& u, uint8_t*& v, const int& nWidth, const int& nHeight)
{

	cv::Mat _y = cv::Mat(cv::Size(nWidth, nHeight), CV_8UC1, y);
	cv::Mat _u = cv::Mat(cv::Size(nWidth / 2, nHeight / 2), CV_8UC1, u);
	cv::Mat _v = cv::Mat(cv::Size(nWidth / 2, nHeight / 2), CV_8UC1, v);

	cv::cuda::GpuMat src;
	if (interpolatePicture)
	{
		src = paramOutput;
	}
	else
	{
		src = paramSrc;
	}

	cv::cuda::GpuMat ycbcr;


	cv::cuda::cvtColor(src, ycbcr, cv::COLOR_BGR2YUV);
	vector<cv::cuda::GpuMat> yuv;
	cv::split(ycbcr, yuv);
	yuv[0].copyTo(_y);

	cv::resize(yuv[1], _u, cv::Size(nWidth / 2, nHeight / 2), 0, 0, cv::INTER_NEAREST); //repeat u values 4 times
	cv::resize(yuv[2], _v, cv::Size(nWidth / 2, nHeight / 2), 0, 0, cv::INTER_NEAREST); //repeat v values 4 times
}


void CCudaEffectVideo::FlipVertical()
{

	if (interpolatePicture)
	{
		cudaFilter->Flip("FlipVertical", paramOutput);
	}
	else
	{
		cudaFilter->Flip("FlipVertical", paramSrc);
	}
}

void CCudaEffectVideo::NLMeansDenoise(const double& coeff, const double& templateWindowSize, const double& searchWindowSize)
{
	if (interpolatePicture)
	{
		cudaFilter->NlMeans(paramOutput, coeff, 3, 7, 21);
	}
	else
	{
		cudaFilter->NlMeans(paramSrc, coeff, 3, 7, 21);
	}
}



uint8_t* CCudaEffectVideo::HQDn3D(uint8_t* y, int width, int height, const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault)
{
	uint8_t* dataOut = nullptr;
	try
	{

		if (hq3d == nullptr)
			hq3d = new Chqdn3d(width, height, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
		else if (hq3d != nullptr)
		{
			hq3d->UpdateParameter(width, height, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
		}

		dataOut = hq3d->ApplyDenoise3D(y, width, height);

	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	return dataOut;
}

// LumSpac, temporalLumaDefault, temporalSpatialLumaDefault
void CCudaEffectVideo::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault)
{

	try
	{
		cv::cuda::GpuMat ycbcr;
		cv::Mat yChannel;
		int width = 0;
		int height = 0;

		if (interpolatePicture)
		{
			width = paramOutput.cols;
			height = paramOutput.rows;
			cv::cuda::cvtColor(paramOutput, ycbcr, cv::COLOR_BGR2YCrCb);
		}
		else
		{
			width = paramSrc.cols;
			height = paramSrc.rows;
			cv::cuda::cvtColor(paramSrc, ycbcr, cv::COLOR_BGR2YCrCb);
		}

		if (hq3d == nullptr)
			hq3d = new Chqdn3d(width, height, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
		else if (hq3d != nullptr)
		{
			hq3d->UpdateParameter(width, height, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
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
			cv::cuda::cvtColor(ycbcr, paramOutput, cv::COLOR_YCrCb2BGR);
		}
		else
		{
			cv::cuda::cvtColor(ycbcr, paramSrc, cv::COLOR_YCrCb2BGR);
		}

		ycbcr.release();
		yChannel.release();
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

bool CCudaEffectVideo::IsOk()
{
	if (interpolatePicture)
		return !paramOutput.empty();
	return !paramSrc.empty();
}