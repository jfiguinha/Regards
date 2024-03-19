// ReSharper disable All
#include <header.h>
#include "OpenCLEffectVideo.h"

#include "utility.h"
#include "EffectVideoParameter.h"
#include "OpenCLFilter.h"
#include "hqdn3d.h"
#include "VideoStabilization.h"


#include <FaceDetector.h>
using namespace Regards::OpenCL;
using namespace Regards::OpenCV;

extern cv::ocl::OpenCLExecutionContext clExecCtx;

COpenCLEffectVideo::COpenCLEffectVideo()
{
	openclFilter = new COpenCLFilter();
	bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	
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

void COpenCLEffectVideo::TransfertData()
{
	/*
	if (convertSrc.channels() == 4)
		cv::cvtColor(convertSrc, paramSrc, cv::COLOR_BGRA2BGR);
	else
		convertSrc.copyTo(paramSrc);
		*/
	
}

cv::UMat COpenCLEffectVideo::GetUMat(const bool& src)
{
	
	cv::UMat output;

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


void COpenCLEffectVideo::SetMatrix(cv::Mat& frame)
{
	
	if (frame.channels() == 4)
		cv::cvtColor(frame, paramSrc, cv::COLOR_BGRA2BGR);
	else
		frame.copyTo(paramSrc);

	needToTranscode = false;
	isOk = true;
}

cv::Mat COpenCLEffectVideo::GetMatrix(const bool& src)
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

COpenCLEffectVideo::~COpenCLEffectVideo()
{
	if (openclFilter != nullptr)
		delete openclFilter;

	if (hq3d != nullptr)
		delete hq3d;
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
}


void COpenCLEffectVideo::ApplyOpenCVEffect(CVideoEffectParameter* videoEffectParameter)
{

	if (videoEffectParameter->autoConstrast)
	{
		if (interpolatePicture)
		{
			openclFilter->BrightnessAndContrastAuto(paramOutput, 1.0);
		}
		else
		{
			openclFilter->BrightnessAndContrastAuto(paramSrc, 1.0);
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



void COpenCLEffectVideo::InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect& rc,
                                                  const int& flipH, const int& flipV, const int& angle,
                                                  const int& bicubic, int ratio)
{
	if (!clExecCtx.empty() && !paramSrc.empty())
	{
		

		paramOutput = openclFilter->Interpolation(widthOutput, heightOutput, rc, bicubic, paramSrc, flipH, flipV, angle,
		                                          ratio);
		interpolatePicture = true;
	}
}

void COpenCLEffectVideo::AutoContrast()
{
	
	if (interpolatePicture)
	{
		openclFilter->BrightnessAndContrastAuto(paramOutput, 1.0);
	}
	else
	{
		openclFilter->BrightnessAndContrastAuto(paramSrc, 1.0);
	}
}


void COpenCLEffectVideo::ApplyVideoEffect(CVideoEffectParameter* videoEffectParameter)
{
	

	if (videoEffectParameter->ColorBoostEnable)
	{
		if (interpolatePicture)
		{
			openclFilter->RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1],
			                        videoEffectParameter->color_boost[2], paramOutput);
		}
		else
		{
			openclFilter->RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1],
			                        videoEffectParameter->color_boost[2], paramSrc);
		}
	}
	if (videoEffectParameter->bandcEnable)
	{
		if (interpolatePicture)
		{
			openclFilter->BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast,
			                                    paramOutput);
		}
		else
		{
			openclFilter->BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast,
			                                    paramSrc);
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
			openclFilter->SharpenMasking(videoEffectParameter->sharpness, paramSrc);
		}
	}
	if (videoEffectParameter->denoiseEnable)
	{
		if (interpolatePicture)
		{
			openclFilter->NlMeans(paramOutput, videoEffectParameter->h, videoEffectParameter->hColor,
			                      videoEffectParameter->templateWindowSize, videoEffectParameter->searchWindowSize);
		}
		else
		{
			openclFilter->NlMeans(paramSrc, videoEffectParameter->h, videoEffectParameter->hColor,
			                      videoEffectParameter->templateWindowSize, videoEffectParameter->searchWindowSize);
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

void COpenCLEffectVideo::SetNV12(uint8_t* bufferY, int sizeY, uint8_t* bufferUV, int sizeUV, const int& width,
                                 const int& height, const int& lineSize, const int& widthOut, const int& heightOut,
                                 const int& colorRange, const int& colorSpace)
{
	
	cv::UMat out;
	vector<COpenCLParameter*> vecParam;
	COpenCLParameterByteArray* inputY = new COpenCLParameterByteArray();
	inputY->SetLibelle("inputY");
	inputY->SetValue((cl_context)clExecCtx.getContext().ptr(), bufferY, sizeY, flag);
	vecParam.push_back(inputY);

	COpenCLParameterByteArray* inputUV = new COpenCLParameterByteArray();
	inputUV->SetLibelle("inputUV");
	inputUV->SetValue((cl_context)clExecCtx.getContext().ptr(), bufferUV, sizeUV, flag);
	vecParam.push_back(inputUV);

	COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(width);
	vecParam.push_back(paramWidth);

	COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(height);
	vecParam.push_back(paramHeight);

	COpenCLParameterInt* paramSrcWidth = new COpenCLParameterInt();
	paramSrcWidth->SetLibelle("widthOut");
	paramSrcWidth->SetValue(widthOut);
	vecParam.push_back(paramSrcWidth);

	COpenCLParameterInt* paramSrcHeight = new COpenCLParameterInt();
	paramSrcHeight->SetLibelle("heightOut");
	paramSrcHeight->SetValue(heightOut);
	vecParam.push_back(paramSrcHeight);

	COpenCLParameterInt* paramLineSize = new COpenCLParameterInt();
	paramLineSize->SetLibelle("LineSize");
	paramLineSize->SetValue(lineSize);
	vecParam.push_back(paramLineSize);

	COpenCLParameterInt* paramColorRange = new COpenCLParameterInt();
	paramColorRange->SetLibelle("ColorRange");
	paramColorRange->SetValue(colorRange);
	vecParam.push_back(paramColorRange);

	COpenCLParameterInt* paramColorSpace = new COpenCLParameterInt();
	paramColorSpace->SetLibelle("ColorSpace");
	paramColorSpace->SetValue(colorSpace);
	vecParam.push_back(paramColorSpace);


	out = COpenCLFilter::ExecuteOpenCLCode("IDR_OPENCL_FFMPEGNV12", "Convert", vecParam, widthOut, heightOut);

	for (COpenCLParameter* parameter : vecParam)
	{
		if (!parameter->GetNoDelete())
		{
			delete parameter;
			parameter = nullptr;
		}
	}

	try
	{
		cv::cvtColor(out, paramSrc, cv::COLOR_BGRA2BGR);
	}
	catch (cv::Exception& e)
	{
		cv::Mat mat = out.getMat(cv::AccessFlag::ACCESS_READ);
		cv::cvtColor(out, paramSrc, cv::COLOR_BGRA2BGR);

		const char* err_msg = e.what();
		std::cout << "CSuperSampling::exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}


void COpenCLEffectVideo::SetNV12(cv::UMat y, uint8_t* bufferUV, int sizeUV, const int& width,
	const int& height, const int& lineSize, const int& widthOut, const int& heightOut,
	const int& colorRange, const int& colorSpace)
{
	
	cv::UMat out;
	vector<COpenCLParameter*> vecParam;

	auto clBuffer = static_cast<cl_mem>(y.handle(cv::ACCESS_READ));
	auto inputY = new COpenCLParameterClMem(true);
	inputY->SetValue(clBuffer);
	inputY->SetLibelle("inputY");
	inputY->SetNoDelete(true);
	vecParam.push_back(inputY);

	//inputY->SetLibelle("inputY");
	//inputY->SetValue((cl_context)local.getContext().ptr(), bufferY, sizeY, flag);
	//vecParam.push_back(inputY);

	COpenCLParameterByteArray* inputUV = new COpenCLParameterByteArray();
	inputUV->SetLibelle("inputUV");
	inputUV->SetValue((cl_context)clExecCtx.getContext().ptr(), bufferUV, sizeUV, flag);
	vecParam.push_back(inputUV);

	COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(width);
	vecParam.push_back(paramWidth);

	COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(height);
	vecParam.push_back(paramHeight);

	COpenCLParameterInt* paramSrcWidth = new COpenCLParameterInt();
	paramSrcWidth->SetLibelle("widthOut");
	paramSrcWidth->SetValue(widthOut);
	vecParam.push_back(paramSrcWidth);

	COpenCLParameterInt* paramSrcHeight = new COpenCLParameterInt();
	paramSrcHeight->SetLibelle("heightOut");
	paramSrcHeight->SetValue(heightOut);
	vecParam.push_back(paramSrcHeight);

	COpenCLParameterInt* paramLineSize = new COpenCLParameterInt();
	paramLineSize->SetLibelle("LineSize");
	paramLineSize->SetValue(lineSize);
	vecParam.push_back(paramLineSize);

	COpenCLParameterInt* paramColorRange = new COpenCLParameterInt();
	paramColorRange->SetLibelle("ColorRange");
	paramColorRange->SetValue(colorRange);
	vecParam.push_back(paramColorRange);

	COpenCLParameterInt* paramColorSpace = new COpenCLParameterInt();
	paramColorSpace->SetLibelle("ColorSpace");
	paramColorSpace->SetValue(colorSpace);
	vecParam.push_back(paramColorSpace);


	out = COpenCLFilter::ExecuteOpenCLCode("IDR_OPENCL_FFMPEGNV12", "Convert", vecParam, widthOut, heightOut);

	for (COpenCLParameter* parameter : vecParam)
	{
		if (!parameter->GetNoDelete())
		{
			delete parameter;
			parameter = nullptr;
		}
	}

	try
	{
		cv::cvtColor(out, paramSrc, cv::COLOR_BGRA2BGR);
	}
	catch (cv::Exception& e)
	{
		cv::Mat mat = out.getMat(cv::AccessFlag::ACCESS_READ);
		cv::cvtColor(out, paramSrc, cv::COLOR_BGRA2BGR);

		const char* err_msg = e.what();
		std::cout << "CSuperSampling::exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLEffectVideo::SetNV12(const cv::Mat& yuv)
{
	
	cv::cvtColor(yuv, paramSrc, cv::COLOR_RGBA2BGR);
}

void COpenCLEffectVideo::SetNV12(const cv::Mat& yuv, const int& linesize, const int& nWidth, const int& nHeight)
{
	
	cv::UMat out;
	
	if (nWidth != linesize)
	{
		cv::cvtColor(yuv, out, cv::COLOR_YUV2BGR_NV12);
		out(cv::Rect(0, 0, nWidth, nHeight)).copyTo(paramSrc);
	}
	else
	{
		cv::cvtColor(yuv, out, cv::COLOR_YUV2BGR_NV12);
		out.copyTo(paramSrc);
	}
}

void COpenCLEffectVideo::SetYUV420P(uint8_t* bufferY, int sizeY, uint8_t* bufferU, int sizeU, uint8_t* bufferV,
                                    int sizeV, const int& width, const int& height, const int& lineSize,
                                    const int& widthOut, const int& heightOut, const int& colorRange,
                                    const int& colorSpace)
{
	cv::UMat out;
	

	vector<COpenCLParameter*> vecParam;
	COpenCLParameterByteArray* inputY = new COpenCLParameterByteArray();
	inputY->SetLibelle("inputY");
	inputY->SetValue((cl_context)clExecCtx.getContext().ptr(), bufferY, sizeY, flag);
	vecParam.push_back(inputY);

	COpenCLParameterByteArray* inputU = new COpenCLParameterByteArray();
	inputU->SetLibelle("inputU");
	inputU->SetValue((cl_context)clExecCtx.getContext().ptr(), bufferU, sizeU, flag);
	vecParam.push_back(inputU);

	COpenCLParameterByteArray* inputV = new COpenCLParameterByteArray();
	inputV->SetLibelle("inputV");
	inputV->SetValue((cl_context)clExecCtx.getContext().ptr(), bufferV, sizeV, flag);
	vecParam.push_back(inputV);

	COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(width);
	vecParam.push_back(paramWidth);

	COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(height);
	vecParam.push_back(paramHeight);

	COpenCLParameterInt* paramSrcWidth = new COpenCLParameterInt();
	paramSrcWidth->SetLibelle("widthOut");
	paramSrcWidth->SetValue(widthOut);
	vecParam.push_back(paramSrcWidth);

	COpenCLParameterInt* paramSrcHeight = new COpenCLParameterInt();
	paramSrcHeight->SetLibelle("heightOut");
	paramSrcHeight->SetValue(heightOut);
	vecParam.push_back(paramSrcHeight);

	COpenCLParameterInt* paramLineSize = new COpenCLParameterInt();
	paramLineSize->SetLibelle("LineSize");
	paramLineSize->SetValue(lineSize);
	vecParam.push_back(paramLineSize);

	COpenCLParameterInt* paramColorRange = new COpenCLParameterInt();
	paramColorRange->SetLibelle("ColorRange");
	paramColorRange->SetValue(colorRange);
	vecParam.push_back(paramColorRange);

	COpenCLParameterInt* paramColorSpace = new COpenCLParameterInt();
	paramColorSpace->SetLibelle("ColorSpace");
	paramColorSpace->SetValue(colorSpace);
	vecParam.push_back(paramColorSpace);


	out = COpenCLFilter::ExecuteOpenCLCode("IDR_OPENCL_FFMPEGYUV420", "Convert", vecParam, widthOut, heightOut);

	for (COpenCLParameter* parameter : vecParam)
	{
		if (!parameter->GetNoDelete())
		{
			delete parameter;
			parameter = nullptr;
		}
	}

	try
	{
		cv::cvtColor(out, paramSrc, cv::COLOR_BGRA2BGR);
	}
	catch (cv::Exception& e)
	{
		cv::Mat mat = out.getMat(cv::AccessFlag::ACCESS_READ);
		cv::cvtColor(out, paramSrc, cv::COLOR_BGRA2BGR);

		const char* err_msg = e.what();
		std::cout << "CSuperSampling::exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLEffectVideo::SetYUV420P(const cv::Mat& y, const cv::Mat& u, const cv::Mat& v, const int& linesize,
                                    const int& nWidth, const int& nHeight)
{
	
	cv::UMat u_resized, v_resized;
	cv::resize(u, u_resized, cv::Size(linesize, nHeight), 0, 0, cv::INTER_NEAREST); //repeat u values 4 times
	cv::resize(v, v_resized, cv::Size(linesize, nHeight), 0, 0, cv::INTER_NEAREST); //repeat v values 4 times
	cv::UMat _y;
	cv::UMat yuv;
	cv::UMat out;

	y.copyTo(_y);

	std::vector<cv::UMat> yuv_channels = {_y, u_resized, v_resized};
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

void COpenCLEffectVideo::GetYUV420P(uint8_t* & y, uint8_t* & u, uint8_t* & v, const int& nWidth, const int& nHeight)
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

void COpenCLEffectVideo::NLMeansDenoise(const double& coeff, const double & templateWindowSize,  const double& searchWindowSize)
{
	if (interpolatePicture)
	{
		openclFilter->NlMeans(paramOutput, coeff, 3, 7, 21);
	}
	else
	{
		openclFilter->NlMeans(paramSrc, coeff, 3, 7, 21);
	}
}



uint8_t* COpenCLEffectVideo::HQDn3D(uint8_t * y, int width, int height, const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault)
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
void COpenCLEffectVideo::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault)
{
	
	try
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
			cv::cvtColor(ycbcr, paramOutput, cv::COLOR_YCrCb2BGR);
		}
		else
		{
			cv::cvtColor(ycbcr, paramSrc, cv::COLOR_YCrCb2BGR);
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

bool COpenCLEffectVideo::IsOk()
{
	if (interpolatePicture)
		return !paramOutput.empty();
	return !paramSrc.empty();
}
