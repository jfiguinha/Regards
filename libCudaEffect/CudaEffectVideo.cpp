// ReSharper disable All
#include <header.h>
#include "CudaEffectVideo.h"
#ifndef __APPLE__
#include "utility.h"
#include "EffectVideoParameter.h"
#include "CudaFilter.h"
#include "hqdn3d.h"
#include "VideoStabilization.h"
#include <FaceDetector.h>
#include "opencv2/cudaimgproc.hpp"
#include <opencv2/cudaarithm.hpp>
#include "opencv2/cudawarping.hpp"
#include "videoFilter.h"
using namespace Regards::Cuda;
using namespace Regards::OpenCV;
extern string platformName;


CCudaEffectVideo::CCudaEffectVideo()
{
	cudaFilter = new CCudaFilter();
    paramSrc = cv::cuda::GpuMat(32, 32, CV_8UC4);
}

void CCudaEffectVideo::SetMatrix(Regards::Picture::CPictureArray& frame)
{
	paramSrc = frame.getGpuMat();
	needToTranscode = false;
	isOk = true;
}

Regards::Picture::CPictureArray CCudaEffectVideo::GetMatrix(const bool& src)
{
	cv::cuda::GpuMat output;

	if (interpolatePicture)
	{
		paramOutput.copyTo(output);
		//cv::cuda::cvtColor(paramOutput, output, cv::COLOR_BGR2RGBA);
	}
	else
	{
		paramSrc.copyTo(output);
		//cv::cuda::cvtColor(paramSrc, output, cv::COLOR_BGR2RGBA);
	}


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
	Regards::Picture::CPictureArray pictureArray(paramSrc);
	bool frameStabilized = false;

	if (videoEffectParameter->stabilizeVideo)
	{
		if (openCVStabilization == nullptr)
			openCVStabilization = new Regards::OpenCV::COpenCVStabilization(videoEffectParameter->stabilizeImageBuffere, TYPE_CUDA);

		openCVStabilization->SetNbFrameBuffer(videoEffectParameter->stabilizeImageBuffere);

		if (openCVStabilization->GetNbFrameBuffer() == 0)
		{
			openCVStabilization->BufferFrame(pictureArray);
		}
		else
		{
			frameStabilized = true;
			openCVStabilization->AddFrame(pictureArray);
		}

		if (frameStabilized)
		{
			Regards::Picture::CPictureArray image_local = openCVStabilization->CorrectFrame(pictureArray);
			image_local.copyTo(paramSrc);
			///image_local.release();
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
			paramOutput.download(image);
		}
		else
		{
			paramSrc.download(image);
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
			paramOutput.upload(image);
		}
		else
		{
			paramSrc.upload(image);
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
    printf("CCudaEffectVideo::SetAVFrame \n");
	int nWidth = tmp_frame->width;
	int nHeight = tmp_frame->height;
	try
	{
        if (paramSrc.size().height != nHeight || paramSrc.size().width != nWidth)
        {
            paramSrc.release();
            paramSrc = cv::cuda::GpuMat(nHeight, nWidth, CV_8UC4);
        }

        if (tmp_frame->format == AV_PIX_FMT_NV12)
        {
            //muBitmap.lock();
            //Test if denoising Effect
            if (videoEffectParameter != nullptr && (videoEffectParameter->denoiseEnable && videoEffectParameter->effectEnable))
            {
                uint8_t* outData = HQDn3D(tmp_frame->data[0], tmp_frame->linesize[0], nHeight, videoEffectParameter->denoisingLevel, videoEffectParameter->templateWindowSize, videoEffectParameter->searchWindowSize);
                convertNV12toRGB32(paramSrc, outData, tmp_frame->linesize[0] * nHeight, tmp_frame->data[1],
                    tmp_frame->linesize[1] * (nHeight / 2), tmp_frame->width, tmp_frame->height,
                    tmp_frame->linesize[0], nWidth, nHeight, isLimited, colorSpace);

            }
            else
                convertNV12toRGB32(paramSrc, tmp_frame->data[0], tmp_frame->linesize[0] * nHeight, tmp_frame->data[1],
                    tmp_frame->linesize[1] * (nHeight / 2), tmp_frame->width, tmp_frame->height,
                    tmp_frame->linesize[0], nWidth, nHeight, isLimited, colorSpace);
            //muBitmap.unlock();
        }
        else if (tmp_frame->format == AV_PIX_FMT_YUV420P)
        {
            //muBitmap.lock();
            if (videoEffectParameter != nullptr && (videoEffectParameter->denoiseEnable && videoEffectParameter->effectEnable))
            {
                uint8_t* outData = HQDn3D(tmp_frame->data[0], tmp_frame->linesize[0], nHeight, videoEffectParameter->denoisingLevel, videoEffectParameter->templateWindowSize, videoEffectParameter->searchWindowSize);
                convertYUVtoRGB32(paramSrc, outData, tmp_frame->linesize[0] * nHeight, tmp_frame->data[1],
                    tmp_frame->linesize[1] * (nHeight / 2), tmp_frame->data[2],
                    tmp_frame->linesize[2] * (nHeight / 2), tmp_frame->width, tmp_frame->height,
                    tmp_frame->linesize[0], nWidth, nHeight, isLimited, colorSpace);
            }
            else
            {
                convertYUVtoRGB32(paramSrc, tmp_frame->data[0], tmp_frame->linesize[0] * nHeight, 
					tmp_frame->data[1], tmp_frame->linesize[1] * (nHeight / 2), 
					tmp_frame->data[2], tmp_frame->linesize[2] * (nHeight / 2), 
					tmp_frame->width, tmp_frame->height,
                    tmp_frame->linesize[0], nWidth, nHeight, isLimited, colorSpace);
            }
        }
	}
	catch (cv::Exception& e)
	{
		//cv::Mat mat = out.getMat(cv::AccessFlag::ACCESS_READ);
		//cv::cvtColor(out, paramSrc, cv::COLOR_BGRA2BGR);

		const char* err_msg = e.what();
		std::cout << "CSuperSampling::exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	
	/*
	try
	{
		cv::cuda::cvtColor(out, paramSrc, cv::COLOR_RGBA2BGR);
	}
	catch (cv::Exception& e)
	{
		//cv::Mat mat = out.getMat(cv::AccessFlag::ACCESS_READ);
		//cv::cvtColor(out, paramSrc, cv::COLOR_BGRA2BGR);

		const char* err_msg = e.what();
		std::cout << "CSuperSampling::exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
	

	/*

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
	*/
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
		cv::Mat gpuPicture;
		cv::Mat ycbcr;
		cv::Mat yChannel;
		int width = 0;
		int height = 0;

		

		if (interpolatePicture)
		{
			width = paramOutput.cols;
			height = paramOutput.rows;
			paramOutput.download(gpuPicture);
		}
		else
		{
			width = paramSrc.cols;
			height = paramSrc.rows;
			paramSrc.download(gpuPicture);
			
		}

		cv::cvtColor(gpuPicture, ycbcr, cv::COLOR_BGR2YCrCb);

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
		cv::cvtColor(ycbcr, gpuPicture, cv::COLOR_YCrCb2BGR);

		if (interpolatePicture)
		{
			paramOutput.upload(gpuPicture);
		}
		else
		{
			paramSrc.upload(gpuPicture);
		}

		ycbcr.release();
		yChannel.release();
		gpuPicture.release();
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

#endif