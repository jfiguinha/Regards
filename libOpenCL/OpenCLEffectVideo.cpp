// ReSharper disable All
#include <header.h>
#include "OpenCLEffectVideo.h"
#include "EffectVideoParameter.h"
#include "OpenCLFilter.h"
#include "OpenCLKernelBuilder.h"
#include "VideoStabilization.h"
#include <FaceDetector.h>
#include <appcontext.h>
#include "OpenCLContext.h"
extern AppContext application_context;

using namespace Regards::OpenCL;
using namespace Regards::OpenCV;

template<typename F>
void COpenCLEffectVideo::ExecuteSafe(F&& func)
{
	try
	{
		cv::UMat convert;

		if (interpolatePicture)
		{
			convert = paramOutput;
		}
		else
		{
			convert = paramSrc;
		}
		func(convert);
	}
	catch (const cv::Exception& e)
	{
		LogError(e.what());
	}
}



COpenCLEffectVideo::COpenCLEffectVideo(COpenCLContext* openCLContext)
{
	this->openCLContext = openCLContext;
	openclFilter = std::make_unique<COpenCLFilter>(openCLContext);
	openclFilter->SetIsVideo(true);
	bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;

}

void COpenCLEffectVideo::SetMatrix(Regards::Picture::CPictureArray& bitmap)
{
	cv::UMat frame = bitmap.getUMat();

	if (frame.channels() == 4)
		cv::cvtColor(frame, paramSrc, cv::COLOR_BGRA2BGR);
	else
		paramSrc = frame;

	needToTranscode = false;
	isOk = true;
}

void COpenCLEffectVideo::SetMatrix(cv::Mat* frame)
{
	if (frame->channels() == 4)
		cv::cvtColor(*frame, paramSrc, cv::COLOR_BGRA2BGR);
	else
		frame->copyTo(paramSrc);
	needToTranscode = false;
	isOk = true;
}

Regards::Picture::CPictureArray COpenCLEffectVideo::GetMatrix(const bool& src)
{

	if (interpolatePicture)
	{
		return paramOutput;
	}
	return paramSrc;


	
}


void COpenCLEffectVideo::ConvertToBgr()
{

	if (!paramSrc.empty())
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


void COpenCLEffectVideo::ApplyStabilization(CVideoEffectParameter* videoEffectParameter,
	COpenCVStabilization* openCVStabilization)
{

	bool frameStabilized = false;
	Regards::Picture::CPictureArray pictureArray(paramSrc);

	if (videoEffectParameter->stabilizeVideo)
	{
		if (openCVStabilization == nullptr)
			openCVStabilization = new COpenCVStabilization(videoEffectParameter->stabilizeImageBuffere, TYPE_OPENCL);

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
			//image_local.release();
		}
	}
}


void COpenCLEffectVideo::ApplyOpenCVEffect(CVideoEffectParameter* videoEffectParameter)
{

	if (videoEffectParameter->autoConstrast)
	{
		ExecuteSafe([&](cv::UMat& image)
			{
				openclFilter->BrightnessAndContrastAuto(image, 1.0);
			});
	}

	if (videoEffectParameter->filmEnhance || videoEffectParameter->filmcolorisation)
	{
		cv::Mat image;

		ExecuteSafe([&](cv::UMat& image_umat)
			{
				image_umat.copyTo(image);
			});

		if (videoEffectParameter->filmEnhance)
		{
			image = CFaceDetector::SuperResolution(image);
		}
		if (videoEffectParameter->filmcolorisation)
		{

			image = CFaceDetector::Colorisation(image);
		}

		ExecuteSafe([&](cv::UMat& image_umat)
			{
				image.copyTo(image_umat);
			});
	}
}



void COpenCLEffectVideo::InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect& rc,
	const int& flipH, const int& flipV, const int& angle,
	const int& bicubic, int ratio, bool bgraOutput)
{
	if (!openCLContext->GetExecutionContext().empty() && !paramSrc.empty())
	{

		
		paramOutput = openclFilter->Interpolation(widthOutput, heightOutput, rc, bicubic, paramSrc, flipH, flipV, angle,
			ratio, bgraOutput);
		interpolatePicture = true;
	}
}

void COpenCLEffectVideo::AutoContrast()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->BrightnessAndContrastAuto(image, 1.0);
		});

}


void COpenCLEffectVideo::ApplyVideoEffect(CVideoEffectParameter* videoEffectParameter)
{


	if (videoEffectParameter->ColorBoostEnable)
	{
		ExecuteSafe([&](cv::UMat& image)
			{
				openclFilter->RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1],
					videoEffectParameter->color_boost[2], image);
			});
	}
	if (videoEffectParameter->bandcEnable)
	{
		ExecuteSafe([&](cv::UMat& image)
			{
				openclFilter->BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast,
					image);
			});
	}
	if (videoEffectParameter->SharpenEnable)
	{
		ExecuteSafe([&](cv::UMat& image)
			{
				openclFilter->SharpenMasking(videoEffectParameter->sharpness, image);
			});

	}
	if (videoEffectParameter->denoiseEnable)
	{
		ExecuteSafe([&](cv::UMat& image)
			{
				openclFilter->NlMeans(image, videoEffectParameter->h, videoEffectParameter->hColor,
					videoEffectParameter->templateWindowSize, videoEffectParameter->searchWindowSize);
			});

	}
	if (videoEffectParameter->sepiaEnable)
	{
		ExecuteSafe([&](cv::UMat& image)
			{
			openclFilter->ColorEffect("Sepia", image);
			});
	}
	if (videoEffectParameter->grayEnable)
	{
		ExecuteSafe([&](cv::UMat& image)
			{
			openclFilter->ColorEffect("GrayLevel", image);
			});
	}
	if (videoEffectParameter->filmgrainenable)
	{
		ExecuteSafe([&](cv::UMat& image)
			{
			openclFilter->Noise(image);
			});
	}
}

void COpenCLEffectVideo::SetNV12(uint8_t* bufferY, int sizeY, uint8_t* bufferUV, int sizeUV, const int& width,
	const int& height, const int& lineSize, const int& widthOut, const int& heightOut,
	const int& colorRange, const int& colorSpace, bool bgraOutput)
{
	OpenCLKernelBuilder builder;

	builder
		.ByteArray("inputY", (cl_context)openCLContext->GetExecutionContext().getContext().ptr(), bufferY, sizeY, flag)
		.ByteArray("inputUV", (cl_context)openCLContext->GetExecutionContext().getContext().ptr(), bufferUV, sizeUV, flag)
		.Int("widthIn", width)
		.Int("heightIn", height)
		.Int("widthOut", widthOut)
		.Int("heightOut", heightOut)
		.Int("LineSize", lineSize)
		.Int("ColorRange", colorRange)
		.Int("ColorSpace", colorSpace);
	auto params = builder.GetParameters();

	paramSrc = openclFilter->ExecuteOpenCLCode(
		"IDR_OPENCL_FFMPEGNV12", "Convert",
		params,
		widthOut, heightOut);

	if (!bgraOutput)
		TestBgraOutput();
}

void COpenCLEffectVideo::TestBgraOutput()
{
	try
	{
		cv::cvtColor(paramSrc, paramSrc, cv::COLOR_BGRA2BGR);
	}
	catch (cv::Exception& e)
	{
		cv::Mat mat = paramSrc.getMat(cv::AccessFlag::ACCESS_READ);
		cv::cvtColor(mat, paramSrc, cv::COLOR_BGRA2BGR);

		const char* err_msg = e.what();
		std::cout << "CSuperSampling::exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
	
}


void COpenCLEffectVideo::SetNV12(cv::UMat y, uint8_t* bufferUV, int sizeUV, const int& width,
	const int& height, const int& lineSize, const int& widthOut, const int& heightOut,
	const int& colorRange, const int& colorSpace, bool bgraOutput)
{

	auto clBuffer = static_cast<cl_mem>(y.handle(cv::ACCESS_READ));

	OpenCLKernelBuilder builder;

	builder
		.Image("inputY", clBuffer)
		.ByteArray("inputUV", (cl_context)openCLContext->GetExecutionContext().getContext().ptr(), bufferUV, sizeUV, flag)
		.Int("widthIn", width)
		.Int("heightIn", height)
		.Int("widthOut", widthOut)
		.Int("heightOut", heightOut)
		.Int("LineSize", lineSize)
		.Int("ColorRange", colorRange)
		.Int("ColorSpace", colorSpace);
	auto params = builder.GetParameters();

	paramSrc = openclFilter->ExecuteOpenCLCode(
		"IDR_OPENCL_FFMPEGNV12", "Convert",
		params,
		widthOut, heightOut);

	if (!bgraOutput)
		TestBgraOutput();
}


void COpenCLEffectVideo::SetYUV420P(uint8_t* bufferY, int sizeY, uint8_t* bufferU, int sizeU, uint8_t* bufferV,
	int sizeV, const int& width, const int& height, const int& lineSize,
	const int& widthOut, const int& heightOut, const int& colorRange,
	const int& colorSpace, bool bgraOutput)
{

	OpenCLKernelBuilder builder;

	builder
		.ByteArray("inputY", (cl_context)openCLContext->GetExecutionContext().getContext().ptr(), bufferY, sizeY, flag)
		.ByteArray("inputU", (cl_context)openCLContext->GetExecutionContext().getContext().ptr(), bufferU, sizeU, flag)
		.ByteArray("inputV", (cl_context)openCLContext->GetExecutionContext().getContext().ptr(), bufferV, sizeV, flag)
		.Int("widthIn", width)
		.Int("heightIn", height)
		.Int("widthOut", widthOut)
		.Int("heightOut", heightOut)
		.Int("LineSize", lineSize)
		.Int("ColorRange", colorRange)
		.Int("ColorSpace", colorSpace);
	auto params = builder.GetParameters();


	paramSrc = openclFilter->ExecuteOpenCLCode(
		"IDR_OPENCL_FFMPEGYUV420", "Convert",
		params,
		widthOut, heightOut);

	if (!bgraOutput)
		TestBgraOutput();
}


void COpenCLEffectVideo::SetAVFrame(CVideoEffectParameter* videoEffectParameter, AVFrame*& tmp_frame, int colorSpace, int isLimited, bool bgraOutput)
{
	int nWidth = tmp_frame->width;
	int nHeight = tmp_frame->height;
	interpolatePicture = false;

	if (tmp_frame->format == AV_PIX_FMT_NV12)
	{
		//muBitmap.lock();
		//Test if denoising Effect
		if (videoEffectParameter != nullptr && (videoEffectParameter->denoiseEnable && videoEffectParameter->effectEnable))
		{
			uint8_t* outData = HQDn3D(tmp_frame->data[0], tmp_frame->linesize[0], nHeight, videoEffectParameter->denoisingLevel, videoEffectParameter->templateWindowSize, videoEffectParameter->searchWindowSize);
			SetNV12(outData, tmp_frame->linesize[0] * nHeight, tmp_frame->data[1],
				tmp_frame->linesize[1] * (nHeight / 2), tmp_frame->linesize[0], nHeight,
				tmp_frame->linesize[0], nWidth, nHeight, isLimited, colorSpace, bgraOutput);

		}
		else
			SetNV12(tmp_frame->data[0], tmp_frame->linesize[0] * nHeight, tmp_frame->data[1],
				tmp_frame->linesize[1] * (nHeight / 2), tmp_frame->linesize[0], nHeight,
				tmp_frame->linesize[0], nWidth, nHeight, isLimited, colorSpace, bgraOutput);
		//muBitmap.unlock();
	}
	else if (tmp_frame->format == AV_PIX_FMT_YUV420P)
	{
		//muBitmap.lock();
		if (videoEffectParameter != nullptr && (videoEffectParameter->denoiseEnable && videoEffectParameter->effectEnable))
		{
			uint8_t* outData = HQDn3D(tmp_frame->data[0], tmp_frame->linesize[0], nHeight, videoEffectParameter->denoisingLevel, videoEffectParameter->templateWindowSize, videoEffectParameter->searchWindowSize);
			SetYUV420P(outData, tmp_frame->linesize[0] * nHeight, tmp_frame->data[1],
				tmp_frame->linesize[1] * (nHeight / 2), tmp_frame->data[2],
				tmp_frame->linesize[2] * (nHeight / 2), tmp_frame->linesize[0], nHeight,
				tmp_frame->linesize[0], nWidth, nHeight, isLimited, colorSpace, bgraOutput);
		}
		else
		{
			SetYUV420P(tmp_frame->data[0], tmp_frame->linesize[0] * nHeight, tmp_frame->data[1],
				tmp_frame->linesize[1] * (nHeight / 2), tmp_frame->data[2],
				tmp_frame->linesize[2] * (nHeight / 2), tmp_frame->linesize[0], nHeight,
				tmp_frame->linesize[0], nWidth, nHeight, isLimited, colorSpace, bgraOutput);
		}

		//muBitmap.unlock();
	}
}



void COpenCLEffectVideo::SetYUV420P(const cv::Mat& y, const cv::Mat& u, const cv::Mat& v, const int& linesize,
	const int& nWidth, const int& nHeight, bool bgraOutput)
{

	cv::UMat u_resized, v_resized;
	cv::resize(u, u_resized, cv::Size(linesize, nHeight), 0, 0, cv::INTER_NEAREST); //repeat u values 4 times
	cv::resize(v, v_resized, cv::Size(linesize, nHeight), 0, 0, cv::INTER_NEAREST); //repeat v values 4 times
	cv::UMat _y;
	cv::UMat yuv;
	cv::UMat out;

	y.copyTo(_y);

	std::vector<cv::UMat> yuv_channels = { _y, u_resized, v_resized };
	cv::merge(yuv_channels, yuv);

	if (nWidth != linesize)
	{
		cv::cvtColor(yuv, out, cv::COLOR_YUV2BGR);
		out(cv::Rect(0, 0, nWidth, nHeight)).copyTo(paramSrc);
	}
	else
	{
		cv::cvtColor(yuv, out, cv::COLOR_YUV2BGR);
		out.copyTo(paramSrc);
	}
};

void COpenCLEffectVideo::GetYUV420P(uint8_t*& y, uint8_t*& u, uint8_t*& v, const int& nWidth, const int& nHeight)
{

	cv::Mat _y = cv::Mat(cv::Size(nWidth, nHeight), CV_8UC1, y);
	cv::Mat _u = cv::Mat(cv::Size(nWidth / 2, nHeight / 2), CV_8UC1, u);
	cv::Mat _v = cv::Mat(cv::Size(nWidth / 2, nHeight / 2), CV_8UC1, v);

	cv::UMat src;
	ExecuteSafe([&](cv::UMat& image)
		{
			src = image;
		});

	cv::UMat ycbcr;


	cvtColor(src, ycbcr, cv::COLOR_BGR2YUV);
	vector<cv::UMat> yuv;
	cv::split(ycbcr, yuv);
	yuv[0].copyTo(_y);

	cv::resize(yuv[1], _u, cv::Size(nWidth / 2, nHeight / 2), 0, 0, cv::INTER_NEAREST); //repeat u values 4 times
	cv::resize(yuv[2], _v, cv::Size(nWidth / 2, nHeight / 2), 0, 0, cv::INTER_NEAREST); //repeat v values 4 times
}


void COpenCLEffectVideo::FlipVertical()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->Flip("FlipVertical", image);
		});
}

void COpenCLEffectVideo::NLMeansDenoise(const double& coeff, const double& templateWindowSize, const double& searchWindowSize)
{
	ExecuteSafe([&](cv::UMat& image)
		{
		openclFilter->NlMeans(image, coeff, 3, 7, 21);
		});
}



uint8_t* COpenCLEffectVideo::HQDn3D(uint8_t* y, int width, int height, const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault)
{
	uint8_t* dataOut = nullptr;
	try
	{

		if (hq3d == nullptr)
			hq3d = std::make_unique<Chqdn3d>(width, height, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
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
void COpenCLEffectVideo::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault)
{

	try
	{
		cv::UMat ycbcr;
		cv::Mat yChannel;
		int width = 0;
		int height = 0;

		ExecuteSafe([&](cv::UMat& image)
			{
				width = image.cols;
				height = image.rows;
				cvtColor(image, ycbcr, cv::COLOR_BGR2YCrCb);
			});

		if (hq3d == nullptr)
			hq3d = std::make_unique<Chqdn3d>(width, height, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
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

		ExecuteSafe([&](cv::UMat& image_umat)
			{
				cv::cvtColor(ycbcr, image_umat, cv::COLOR_YCrCb2BGR);
			});
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

bool COpenCLEffectVideo::IsOk()
{
	bool isEmpty = false;
	ExecuteSafe([&](cv::UMat& image_umat)
		{
			isEmpty = image_umat.empty();
		});

	return !isEmpty;
}