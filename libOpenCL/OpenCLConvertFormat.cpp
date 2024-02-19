// ReSharper disable All
#include <header.h>
#include "OpenCLConvertFormat.h"
#include "utility.h"
#include "EffectVideoParameter.h"
#include "OpenCLFilter.h"
#include "hqdn3d.h"
#include "VideoStabilization.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#elif defined(__WXGTK__)
#include <GL/glx.h>
#endif
//#include <RegardsConfigParam.h>
//#include <ParamInit.h>
using namespace Regards::OpenCL;
using namespace Regards::OpenCV;




cv::Mat COpenCLConvertFormat::SetNV12(uint8_t* bufferY, int sizeY, uint8_t* bufferUV, int sizeUV, const int& width,
	const int& height, const int& lineSize, const int& widthOut, const int& heightOut,
	const int& colorRange, const int& colorSpace)
{
	cv::Mat paramSrc;
	cv::UMat out;
	bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
	cl_mem_flags flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;

	vector<COpenCLParameter*> vecParam;
	COpenCLParameterByteArray* inputY = new COpenCLParameterByteArray();
	inputY->SetLibelle("inputY");
	inputY->SetValue((cl_context)cv::ocl::Context::getDefault(false).ptr(), bufferY, sizeY, flag);
	vecParam.push_back(inputY);

	COpenCLParameterByteArray* inputUV = new COpenCLParameterByteArray();
	inputUV->SetLibelle("inputUV");
	inputUV->SetValue((cl_context)cv::ocl::Context::getDefault(false).ptr(), bufferUV, sizeUV, flag);
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

	cv::cvtColor(out, paramSrc, cv::COLOR_BGRA2BGR);

	return paramSrc;
}


cv::Mat COpenCLConvertFormat::SetNV12(const cv::Mat& yuv)
{
	cv::Mat paramSrc;
	cv::cvtColor(yuv, paramSrc, cv::COLOR_RGBA2BGR);
	return paramSrc;
}

cv::Mat COpenCLConvertFormat::SetNV12(const cv::Mat& yuv, const int& linesize, const int& nWidth, const int& nHeight)
{
	cv::Mat paramSrc;
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
	return paramSrc;
}

cv::Mat COpenCLConvertFormat::SetYUV420P(uint8_t* bufferY, int sizeY, uint8_t* bufferU, int sizeU, uint8_t* bufferV,
	int sizeV, const int& width, const int& height, const int& lineSize,
	const int& widthOut, const int& heightOut, const int& colorRange,
	const int& colorSpace)
{

	bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
	cl_mem_flags flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	cv::Mat paramSrc;
	cv::UMat out;

	vector<COpenCLParameter*> vecParam;
	COpenCLParameterByteArray* inputY = new COpenCLParameterByteArray();
	inputY->SetLibelle("inputY");
	inputY->SetValue((cl_context)cv::ocl::Context::getDefault(false).ptr(), bufferY, sizeY, flag);
	vecParam.push_back(inputY);

	COpenCLParameterByteArray* inputU = new COpenCLParameterByteArray();
	inputU->SetLibelle("inputU");
	inputU->SetValue((cl_context)cv::ocl::Context::getDefault(false).ptr(), bufferU, sizeU, flag);
	vecParam.push_back(inputU);

	COpenCLParameterByteArray* inputV = new COpenCLParameterByteArray();
	inputV->SetLibelle("inputV");
	inputV->SetValue((cl_context)cv::ocl::Context::getDefault(false).ptr(), bufferV, sizeV, flag);
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

	cv::cvtColor(out, paramSrc, cv::COLOR_RGBA2BGR);
	return paramSrc;
}

cv::Mat COpenCLConvertFormat::SetYUV420P(const cv::Mat& y, const cv::Mat& u, const cv::Mat& v, const int& linesize,
	const int& nWidth, const int& nHeight)
{
	cv::Mat paramSrc;
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
	return paramSrc;
};

