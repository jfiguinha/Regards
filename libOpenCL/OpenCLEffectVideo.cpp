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
#include "OpenCLFilter.h"
#include "hqdn3d.h"
#include <OpenCVEffect.h>
#include <VideoStabilization.h>
#include <boost/compute/core.hpp>
namespace compute = boost::compute;
using namespace Regards::OpenCL;
using namespace Regards::OpenCV;

COpenCLEffectVideo::COpenCLEffectVideo(COpenCLContext * context): widthOut(0), heightOut(0), srcwidth(0), srcheight(0)
{
	openCLProgram = nullptr;
	bool useMemory = (context->GetContext().get_device().gpu == CL_DEVICE_TYPE_GPU) ? false : true;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;

	openCLProgram = nullptr;
	this->context = context;
	paramOutWidth = nullptr;
	paramOutHeight = nullptr;
	//dataIsOk = false;
	paramOutput = nullptr;
	//paramInput = nullptr;
	paramSrc = nullptr;
	paramSrcWidth = nullptr;
	paramSrcHeight = nullptr;
}

COpenCLParameterClMem * COpenCLEffectVideo::GetPtData(const bool &src)
{
	if (src)
		return paramSrc;
	return paramOutput;
}

int COpenCLEffectVideo::GetDataSizeWidth(const bool &src)
{
	if (src)
		return srcwidth * GetSizeData();
	return widthOut * GetSizeData();
}

COpenCLEffectVideo::~COpenCLEffectVideo()
{
	if (paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
	}

	if (paramOutWidth != nullptr)
	{
		paramOutWidth->Release();
		delete paramOutWidth;
	}

	if (paramOutHeight != nullptr)
	{
		paramOutHeight->Release();
		delete paramOutHeight;
	}

	if (paramSrc != nullptr)
	{
		paramSrc->Release();
		delete paramSrc;
	}

	if (paramSrcWidth != nullptr)
	{
		paramSrcWidth->Release();
		delete paramSrcWidth;
	}

	if (paramSrcHeight != nullptr)
	{
		paramSrcHeight->Release();
		delete paramSrcHeight;
	}

	if (openCLProgram != nullptr)
		delete openCLProgram;

	openCLProgram = nullptr;
}


int COpenCLEffectVideo::GetSrcWidth()
{
	return srcwidth;
}

int COpenCLEffectVideo::GetSrcHeight()
{
	return srcheight;
}


cl_mem COpenCLEffectVideo::GetRgbaSourceBitmap(int rgba)
{
	cl_mem memvalue = nullptr;
	if (paramSrcWidth != nullptr && paramSrcHeight != nullptr && paramSrc != nullptr)
	{
		COpenCLProgram* programCL = GetProgram("IDR_OPENCL_BITMAPCONVERSION");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter*> vecParam;
			COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

			paramSrc->SetNoDelete(false);
			vecParam.push_back(paramSrc);

			paramSrcWidth->SetNoDelete(false);
			vecParam.push_back(paramSrcWidth);

			paramSrcHeight->SetNoDelete(false);
			vecParam.push_back(paramSrcHeight);

			COpenCLParameterInt* paramRGBA = new COpenCLParameterInt();
			paramRGBA->SetValue(rgba);
			paramRGBA->SetLibelle("rgba");
			vecParam.push_back(paramRGBA);

			program->SetKeepOutput(true);
			program->SetParameter(&vecParam, srcwidth, srcheight, srcwidth * srcheight * GetSizeData());
			program->ExecuteProgram(programCL->GetProgram(), "BitmapToOpenGLTexture");
			memvalue = program->GetOutput();
			delete program;
			vecParam.clear();
			paramSrc = nullptr;
			paramSrcWidth = nullptr;
			paramSrcHeight = nullptr;

		}
	}
	return memvalue;
}


void COpenCLEffectVideo::GetRgbaBitmap(cl_mem cl_image, int rgba)
{
	if (paramOutWidth != nullptr && paramOutHeight != nullptr && paramOutput != nullptr)
	{
		COpenCLProgram* programCL = GetProgram("IDR_OPENCL_BITMAPCONVERSION");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter*> vecParam;
			COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

			paramOutput->SetNoDelete(false);
			vecParam.push_back(paramOutput);

			paramOutWidth->SetNoDelete(false);
			vecParam.push_back(paramOutWidth);

			paramOutHeight->SetNoDelete(false);
			vecParam.push_back(paramOutHeight);

			COpenCLParameterInt* paramRGBA = new COpenCLParameterInt();
			paramRGBA->SetValue(rgba);
			paramRGBA->SetLibelle("rgba");
			vecParam.push_back(paramRGBA);

			program->SetKeepOutput(true);
			program->SetParameter(&vecParam, widthOut, heightOut, cl_image);
			program->ExecuteProgram(programCL->GetProgram(), "BitmapToOpenGLTexture");

			delete program;
			vecParam.clear();
			paramOutput = nullptr;
			paramOutWidth = nullptr;
			paramOutHeight = nullptr;

		}
	}
}


void COpenCLEffectVideo::GetRgbaOpenCV(cl_mem cl_image, int rgba)
{
	if (paramSrc != nullptr && paramSrcWidth != nullptr && paramSrcHeight != nullptr)
	{
		COpenCLProgram* programCL = GetProgram("IDR_OPENCL_BITMAPCONVERSION");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter*> vecParam;
			COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);
			vecParam.push_back(paramSrc);
			vecParam.push_back(paramSrcWidth);
			vecParam.push_back(paramSrcHeight);

			COpenCLParameterInt* paramRGBA = new COpenCLParameterInt();
			paramRGBA->SetValue(rgba);
			paramRGBA->SetLibelle("rgba");
			vecParam.push_back(paramRGBA);

			program->SetKeepOutput(true);
			program->SetParameter(&vecParam, widthOut, heightOut, cl_image);
			program->ExecuteProgram(programCL->GetProgram(), "BitmapToOpenGLTexture");

			delete program;
			vecParam.clear();

		}
	}
}

cv::UMat COpenCLEffectVideo::GetOpenCVStruct(const bool &src)
{
	cv::UMat dst;
	cl_mem clImage;
	if (src)
		clImage = paramSrc->GetValue();
	else
		clImage = paramOutput->GetValue();

	cl_mem_object_type mem_type = 0;
	clGetMemObjectInfo(clImage, CL_MEM_TYPE, sizeof(cl_mem_object_type), &mem_type, 0);

	cl_image_format fmt = { 0, 0 };
	clGetImageInfo(clImage, CL_IMAGE_FORMAT, sizeof(cl_image_format), &fmt, 0);

	int depth = CV_8U;
	//if (context->GetDefaultType() == OPENCL_FLOAT)
	//	depth = CV_32F;

	int type = CV_MAKE_TYPE(depth, 4);

	size_t w = src ? srcwidth : widthOut;
	size_t h = src ? srcheight : heightOut;
	dst.create((int)h, (int)w, type);

	if (context->GetDefaultType() == OPENCL_FLOAT)
	{
		cl_mem clBuffer = (cl_mem)dst.handle(cv::ACCESS_RW);
		//cl_mem outputValue = nullptr;
		COpenCLFilter openclFilter(context);
		COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			if (src)
			{
				paramSrc->SetNoDelete(true);
				vecParam.push_back(paramSrc);
				paramSrcWidth->SetNoDelete(true);
				vecParam.push_back(paramSrcWidth);
				paramSrcHeight->SetNoDelete(true);
				vecParam.push_back(paramSrcHeight);
			}
			else
			{
				paramOutput->SetNoDelete(true);
				vecParam.push_back(paramOutput);
				paramOutWidth->SetNoDelete(true);
				vecParam.push_back(paramOutWidth);
				paramOutHeight->SetNoDelete(true);
				vecParam.push_back(paramOutHeight);
			}

			program->SetParameter(&vecParam, w, h, clBuffer);
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "CopyToOpencv");
			delete program;
			vecParam.clear();
		}
	}
	else
	{
		cl_int err = 0;
		dst.create((int)h, (int)w, type);
		cl_mem clBuffer = (cl_mem)dst.handle(cv::ACCESS_RW);
		cl_command_queue q = context->GetCommandQueue();
		err = clEnqueueCopyBuffer(q, clImage, clBuffer, 0, 0, w * h * GetSizeData(), NULL, NULL, NULL);
		Error::CheckError(err);
		clFinish(q);
	}



	return dst;
}

void COpenCLEffectVideo::CopyOpenCVTexture(cv::UMat & dst, const bool &src)
{
	cl_mem clImage;
	if (src)
		clImage = paramSrc->GetValue();
	else
		clImage = paramOutput->GetValue();

	cl_mem clBuffer = (cl_mem)dst.handle(cv::ACCESS_READ);
	if (context->GetDefaultType() == OPENCL_FLOAT)
	{
		cl_mem outputValue = nullptr;
		COpenCLFilter openclFilter(context);
		COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem *	dataImage = new COpenCLParameterClMem();
			dataImage->SetNoDelete(true);
			dataImage->SetValue(clBuffer);
			vecParam.push_back(dataImage);
			if (src)
			{
				vecParam.push_back(paramSrcWidth);
				vecParam.push_back(paramSrcHeight);
			}
			else
			{
				vecParam.push_back(paramOutWidth);
				vecParam.push_back(paramOutHeight);
			}
			program->SetParameter(&vecParam, srcwidth, srcheight, GetSizeData() * srcwidth * srcheight);
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "ImportFromOpencv");
			outputValue = program->GetOutput();

			delete program;

			if (paramSrc == nullptr)
				paramSrc = new COpenCLParameterClMem();
			paramSrc->SetValue(outputValue);

			vecParam.clear();
		}
	}
	else
	{
		cl_int err = 0;
		size_t w = src ? srcwidth : widthOut;
		size_t h = src ? srcheight : heightOut;

		cl_mem cl_buffer = (cl_mem)dst.handle(cv::ACCESS_RW);
		cl_command_queue q = context->GetCommandQueue();
		err = clEnqueueCopyBuffer(q, cl_buffer, clImage, 0, 0, w * h * GetSizeData(), NULL, NULL, NULL);
		Error::CheckError(err);
		clFinish(q);
	}
}


void COpenCLEffectVideo::ApplyOpenCVEffect(CVideoEffectParameter * videoEffectParameter, COpenCVStabilization * openCVStabilization)
{
	context->GetContextForOpenCV().bind();
	bool frameStabilized = false;
	cv::UMat cvImage = GetOpenCVStruct(true);

	if (videoEffectParameter->stabilizeVideo)
	{
		if (openCVStabilization == nullptr)
			openCVStabilization = new COpenCVStabilization(videoEffectParameter->stabilizeImageBuffere);

		openCVStabilization->SetNbFrameBuffer(videoEffectParameter->stabilizeImageBuffere);

		if (openCVStabilization->GetNbFrameBuffer() == 0)
		{
			openCVStabilization->BufferFrame(cvImage);
		}
		else
		{
			frameStabilized = true;
			openCVStabilization->AddFrame(cvImage);
		}

		if (frameStabilized)
		{
			cv::UMat image_local = openCVStabilization->CorrectFrame(cvImage);
			image_local.copyTo(cvImage);
			image_local.release();
		}
	}

	if (videoEffectParameter->autoConstrast)
	{
		Regards::OpenCV::COpenCVEffect::BrightnessAndContrastAuto(cvImage);
		frameStabilized = true;
	}

	if (frameStabilized)
	{
		CopyOpenCVTexture(cvImage, true);
	}

	cvImage.release();
}

void COpenCLEffectVideo::InterpolationBicubic(const int& widthOutput, const int& heightOutput, const int &flipH, const int &flipV, const int& angle, const int& bicubic)
{
	if (paramSrc != nullptr && paramSrcWidth != nullptr && paramSrcHeight != nullptr)
	{
		widthOut = widthOutput;
		heightOut = heightOutput;
		COpenCLProgram* programCL = GetProgram("IDR_OPENCL_INTERPOLATION");
		if (programCL != nullptr)
		{
			cl_mem memvalue = nullptr;
			vector<COpenCLParameter*> vecParam;
			COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

			paramSrc->SetLibelle("input");
			paramSrc->SetNoDelete(true);
			vecParam.push_back(paramSrc);

			paramSrcWidth->SetLibelle("width");
			vecParam.push_back(paramSrcWidth);

			paramSrcHeight->SetLibelle("height");
			vecParam.push_back(paramSrcHeight);

			if (paramOutWidth == nullptr)
				paramOutWidth = new COpenCLParameterInt();
			paramOutWidth->SetLibelle("widthOutput");
			paramOutWidth->SetValue(widthOutput);
			paramOutWidth->SetNoDelete(true);
			vecParam.push_back(paramOutWidth);

			if (paramOutHeight == nullptr)
				paramOutHeight = new COpenCLParameterInt();
			paramOutHeight->SetLibelle("heightOutput");
			paramOutHeight->SetValue(heightOutput);
			paramOutHeight->SetNoDelete(true);
			vecParam.push_back(paramOutHeight);

			COpenCLParameterInt* paramflipH = new COpenCLParameterInt();
			paramflipH->SetLibelle("flipH");
			paramflipH->SetValue(flipH);
			vecParam.push_back(paramflipH);

			COpenCLParameterInt* paramflipV = new COpenCLParameterInt();
			paramflipV->SetLibelle("flipV");
			paramflipV->SetValue(flipV);
			vecParam.push_back(paramflipV);

			COpenCLParameterInt* paramangle = new COpenCLParameterInt();
			paramangle->SetLibelle("angle");
			paramangle->SetValue(angle);
			vecParam.push_back(paramangle);

			COpenCLParameterInt* paramtype = new COpenCLParameterInt();
			paramtype->SetLibelle("type");
			paramtype->SetValue(bicubic);
			vecParam.push_back(paramtype);

			try
			{
				program->SetParameter(&vecParam, widthOutput, heightOutput, widthOutput * heightOutput * GetSizeData());
				program->SetKeepOutput(true);
				program->ExecuteProgram(programCL->GetProgram(), "Interpolation");
				memvalue = program->GetOutput();
			}
			catch (...)
			{
				memvalue = nullptr;
			}

			delete program;
			vecParam.clear();

			if (paramOutput == nullptr)
				paramOutput = new COpenCLParameterClMem();
			paramOutput->SetValue(memvalue);
		}
	}


}

void COpenCLEffectVideo::InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect &rc, const int &flipH, const int &flipV, const int& angle, const int& bicubic)
{
	if (paramSrc != nullptr && paramSrcWidth != nullptr && paramSrcHeight != nullptr)
	{
		widthOut = widthOutput;
		heightOut = heightOutput;
		COpenCLProgram* programCL = GetProgram("IDR_OPENCL_INTERPOLATION");
		if (programCL != nullptr)
		{
			cl_mem memvalue = nullptr;
			vector<COpenCLParameter*> vecParam;
			COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

			paramSrc->SetLibelle("input");
			paramSrc->SetNoDelete(true);
			vecParam.push_back(paramSrc);

			paramSrcWidth->SetLibelle("width");
			vecParam.push_back(paramSrcWidth);

			paramSrcHeight->SetLibelle("height");
			vecParam.push_back(paramSrcHeight);

			if (paramOutWidth == nullptr)
				paramOutWidth = new COpenCLParameterInt();
			paramOutWidth->SetLibelle("widthOutput");
			paramOutWidth->SetValue(widthOutput);
			paramOutWidth->SetNoDelete(true);
			vecParam.push_back(paramOutWidth);

			if (paramOutHeight == nullptr)
				paramOutHeight = new COpenCLParameterInt();
			paramOutHeight->SetLibelle("heightOutput");
			paramOutHeight->SetValue(heightOutput);
			paramOutHeight->SetNoDelete(true);
			vecParam.push_back(paramOutHeight);

			COpenCLParameterFloat * left = new COpenCLParameterFloat();
			left->SetLibelle("left");
			left->SetValue(rc.x);  // NOLINT(clang-diagnostic-implicit-int-float-conversion)
			vecParam.push_back(left);

			COpenCLParameterFloat * top = new COpenCLParameterFloat();
			top->SetLibelle("top");
			top->SetValue(rc.y);
			vecParam.push_back(top);

			COpenCLParameterFloat * bitmapWidth = new COpenCLParameterFloat();
			bitmapWidth->SetLibelle("bitmapWidth");
			bitmapWidth->SetValue(rc.width);
			vecParam.push_back(bitmapWidth);

			COpenCLParameterFloat * bitmapHeight = new COpenCLParameterFloat();
			bitmapHeight->SetLibelle("bitmapHeight");
			bitmapHeight->SetValue(rc.height);
			vecParam.push_back(bitmapHeight);

			COpenCLParameterInt* paramflipH = new COpenCLParameterInt();
			paramflipH->SetLibelle("flipH");
			paramflipH->SetValue(flipH);
			vecParam.push_back(paramflipH);

			COpenCLParameterInt* paramflipV = new COpenCLParameterInt();
			paramflipV->SetLibelle("flipV");
			paramflipV->SetValue(flipV);
			vecParam.push_back(paramflipV);

			COpenCLParameterInt* paramangle = new COpenCLParameterInt();
			paramangle->SetLibelle("angle");
			paramangle->SetValue(angle);
			vecParam.push_back(paramangle);

			COpenCLParameterInt* paramtype = new COpenCLParameterInt();
			paramtype->SetLibelle("type");
			paramtype->SetValue(bicubic);
			vecParam.push_back(paramtype);

			try
			{
				program->SetParameter(&vecParam, widthOutput, heightOutput, widthOutput * heightOutput * GetSizeData());
				program->SetKeepOutput(true);
				program->ExecuteProgram(programCL->GetProgram(), "InterpolationZone");
				memvalue = program->GetOutput();
			}
			catch (...)
			{
				memvalue = nullptr;
			}

			delete program;
			vecParam.clear();

			if (paramOutput == nullptr)
				paramOutput = new COpenCLParameterClMem();
			paramOutput->SetValue(memvalue);
		}
	}
}

CRegardsBitmap* COpenCLEffectVideo::GetBitmap(cl_mem input, const int& width, const int& height)
{
	CRegardsBitmap* bitmap = nullptr;

	if (input != nullptr)
	{
		bitmap = new CRegardsBitmap(width, height);
		COpenCLProgram* programCL = GetProgram("IDR_OPENCL_BITMAPCONVERSION");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter*> vecParam;
			COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem* valueInput = new COpenCLParameterClMem();
			valueInput->SetValue(input);
			valueInput->SetNoDelete(true);
			vecParam.push_back(valueInput);

			COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
			paramWidth->SetLibelle("width");
			paramWidth->SetValue(bitmap->GetBitmapWidth());
			vecParam.push_back(paramWidth);

			COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
			paramHeight->SetLibelle("height");
			paramHeight->SetValue(bitmap->GetBitmapHeight());
			vecParam.push_back(paramHeight);

			program->SetParameter(&vecParam, bitmap);
			program->ExecuteProgram1D(programCL->GetProgram(), "GetRegardsBitmap");

			delete program;
			vecParam.clear();
		}
	}
	return bitmap;
}


void COpenCLEffectVideo::GetBitmap(CRegardsBitmap * bitmap, cl_mem input, const int& width, const int& height)
{
	if (input != nullptr)
	{
		COpenCLProgram* programCL = GetProgram("IDR_OPENCL_BITMAPCONVERSION");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter*> vecParam;
			COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem* valueInput = new COpenCLParameterClMem();
			valueInput->SetValue(input);
			valueInput->SetNoDelete(true);
			vecParam.push_back(valueInput);

			COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
			paramWidth->SetLibelle("width");
			paramWidth->SetValue(bitmap->GetBitmapWidth());
			vecParam.push_back(paramWidth);

			COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
			paramHeight->SetLibelle("height");
			paramHeight->SetValue(bitmap->GetBitmapHeight());
			vecParam.push_back(paramHeight);

			program->SetParameter(&vecParam, bitmap);
			program->ExecuteProgram1D(programCL->GetProgram(), "GetRegardsBitmap");

			delete program;
			vecParam.clear();
		}
	}
}


CRegardsBitmap* COpenCLEffectVideo::GetBitmap(cl_mem cl_image)
{
	CRegardsBitmap* bitmap = nullptr;
	cl_mem ptData = cl_image;
	int width = widthOut;
	int height = heightOut;

	if (context->GetDefaultType() == OPENCL_FLOAT)
	{
		bitmap = GetBitmap(ptData, width, height);
	}
	else
	{
		bitmap = new  CRegardsBitmap(width, height);
		cl_int err = clEnqueueReadBuffer(context->GetCommandQueue(), ptData, CL_TRUE, 0, bitmap->GetBitmapSize(), bitmap->GetPtBitmap(), 0, nullptr, nullptr);
		Error::CheckError(err);
		err = clFinish(context->GetCommandQueue());
		Error::CheckError(err);
	}

	return bitmap;
}


cl_mem COpenCLEffectVideo::LoadRegardsImage(uint8_t * data, const int &width, const int &height)
{
	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterByteArray *	dataImage = new COpenCLParameterByteArray();
		dataImage->SetValue(context->GetContext(), data, width * height * 4, flag);
		vecParam.push_back(dataImage);

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
		program->SetKeepOutput(true);
		program->ExecuteProgram1D(programCL->GetProgram(), "LoadRegardsBitmap");
		outputValue = program->GetOutput();

		delete program;
		vecParam.clear();
	}
	return outputValue;
}

void COpenCLEffectVideo::LoadRegardsBitmap(CRegardsBitmap * bitmap)
{
	cl_mem outputValue;

	if (paramSrc == nullptr)
		paramSrc = new COpenCLParameterClMem();

	if (context->GetDefaultType() == OPENCL_UCHAR)
	{
		COpenCLParameterUintArray uintValue(true);
		uintValue.SetNoDelete(true);
		uintValue.SetValue(context->GetContext(), (unsigned int*)bitmap->GetPtBitmap(), bitmap->GetBitmapWidth() * bitmap->GetBitmapHeight(), flag);
		outputValue = uintValue.GetValue();
	}
	else
	{
		outputValue = LoadRegardsImage(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
	}

	paramSrc->SetValue(outputValue);

	if (paramSrcWidth == nullptr)
		paramSrcWidth = new COpenCLParameterInt();
	paramSrcWidth->SetNoDelete(true);
	paramSrcWidth->SetLibelle("widthIn");
	paramSrcWidth->SetValue(bitmap->GetBitmapWidth());

	if (paramSrcHeight == nullptr)
		paramSrcHeight = new COpenCLParameterInt();
	paramSrcHeight->SetNoDelete(true);
	paramSrcHeight->SetLibelle("heightIn");
	paramSrcHeight->SetValue(bitmap->GetBitmapHeight());

	needToTranscode = false;

}

void COpenCLEffectVideo::AutoContrast()
{
	CRegardsBitmap * bitmap = GetRgbaBitmap();
	COpenCVEffect::BrightnessAndContrastAuto(bitmap);
	cl_mem output = LoadRegardsImage(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
	paramOutput->SetValue(output);
	delete bitmap;
}

CRegardsBitmap* COpenCLEffectVideo::GetRgbaBitmap(const bool &src)
{
	CRegardsBitmap* bitmap = nullptr;
	cl_mem ptData = nullptr;
	int width = widthOut;
	int height = heightOut;

	if (src)
	{
		ptData = paramSrc->GetValue();
		width = srcwidth;
		height = srcheight;
	}
	else
		ptData = paramOutput->GetValue();

	if (context->GetDefaultType() == OPENCL_FLOAT)
	{
		bitmap = GetBitmap(ptData, width, height);
	}
	else
	{
		bitmap = new  CRegardsBitmap(width, height);
		cl_int err = clEnqueueReadBuffer(context->GetCommandQueue(), ptData, CL_TRUE, 0, bitmap->GetBitmapSize(), bitmap->GetPtBitmap(), 0, nullptr, nullptr);
		Error::CheckError(err);
		err = clFinish(context->GetCommandQueue());
		Error::CheckError(err);
	}

	return bitmap;

}

void COpenCLEffectVideo::GetBitmap(CRegardsBitmap * bitmap, const bool &src)
{
	cl_mem ptData = nullptr;
	int width = widthOut;
	int height = heightOut;

	if (src || paramOutput == nullptr)
	{
		ptData = paramSrc->GetValue();
		width = srcwidth;
		height = srcheight;
	}
	else
		ptData = paramOutput->GetValue();

	if (context->GetDefaultType() == OPENCL_FLOAT)
	{
		GetBitmap(bitmap, ptData, width, height);
	}
	else
	{
		//bitmap = new  CRegardsBitmap(width, height);
		cl_int err = clEnqueueReadBuffer(context->GetCommandQueue(), ptData, CL_TRUE, 0, bitmap->GetBitmapSize(), bitmap->GetPtBitmap(), 0, nullptr, nullptr);
		Error::CheckError(err);
		err = clFinish(context->GetCommandQueue());
		Error::CheckError(err);
	}
}

void COpenCLEffectVideo::ApplyVideoEffect(CVideoEffectParameter * videoEffectParameter)
{

	COpenCLFilter openclFilter(context);
	cl_mem output = nullptr;

	if (videoEffectParameter->ColorBoostEnable)
	{
		if (paramOutput != nullptr)
		{
			output = openclFilter.RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1], videoEffectParameter->color_boost[2], paramOutput->GetValue(), widthOut, heightOut);
			paramOutput->SetValue(output);
		}
		else
		{
			output = openclFilter.RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1], videoEffectParameter->color_boost[2], paramSrc->GetValue(), widthOut, heightOut);
			paramSrc->SetValue(output);
		}
	}
	if (videoEffectParameter->bandcEnable)
	{
		if (paramOutput != nullptr)
		{
			output = openclFilter.BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast, paramOutput->GetValue(), widthOut, heightOut);
			paramOutput->SetValue(output);
		}
		else
		{
			output = openclFilter.BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast, paramSrc->GetValue(), widthOut, heightOut);
			paramSrc->SetValue(output);
		}
	}
	if (videoEffectParameter->SharpenEnable)
	{
		if (paramOutput != nullptr)
		{
			output = openclFilter.SharpenMasking(videoEffectParameter->sharpness, paramOutput->GetValue(), widthOut, heightOut);
			paramOutput->SetValue(output);
		}
		else
		{
			output = openclFilter.SharpenMasking(videoEffectParameter->sharpness, paramSrc->GetValue(), widthOut, heightOut);
			paramSrc->SetValue(output);
		}
	}
	if (videoEffectParameter->denoiseEnable)
	{
		if (paramOutput != nullptr)
		{
			output = openclFilter.HQDn3D(videoEffectParameter->denoisingLevel, 4, 3, 3, paramOutput->GetValue(), widthOut, heightOut);
			paramOutput->SetValue(output);
		}
		else
		{
			output = openclFilter.HQDn3D(videoEffectParameter->denoisingLevel, 4, 3, 3, paramSrc->GetValue(), srcwidth, srcheight);
			paramSrc->SetValue(output);
		}
	}
	if (videoEffectParameter->sepiaEnable)
	{
		if (paramOutput != nullptr)
		{
			output = openclFilter.ColorEffect("Sepia", paramOutput->GetValue(), widthOut, heightOut);
			paramOutput->SetValue(output);
		}
		else
		{
			output = openclFilter.ColorEffect("Sepia", paramSrc->GetValue(), srcwidth, srcheight);
			paramSrc->SetValue(output);
		}
	}
	if (videoEffectParameter->grayEnable)
	{
		if (paramOutput != nullptr)
		{
			output = openclFilter.ColorEffect("GrayLevel", paramOutput->GetValue(), widthOut, heightOut);
			paramOutput->SetValue(output);
		}
		else
		{
			output = openclFilter.ColorEffect("GrayLevel", paramSrc->GetValue(), srcwidth, srcheight);
			paramSrc->SetValue(output);
		}
	}
	if (videoEffectParameter->filmgrainenable)
	{
		if (paramOutput != nullptr)
		{
			output = openclFilter.Noise(paramOutput->GetValue(), widthOut, heightOut);
			paramOutput->SetValue(output);
		}
		else
		{
			output = openclFilter.Noise(paramSrc->GetValue(), srcwidth, srcheight);
			paramSrc->SetValue(output);
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

	paramSrc->SetNoDelete(true);

	if (context != nullptr)
	{
		COpenCLProgram * programCL = nullptr;

		programCL = GetProgram("IDR_OPENCL_CONVERTTOY");

		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			vecParam.push_back(inputY);
			vecParam.push_back(inputU);
			vecParam.push_back(inputV);
			if (paramOutput != nullptr)
				vecParam.push_back(paramOutput);
			else
				vecParam.push_back(paramSrc);
			vecParam.push_back(paramWidth);
			vecParam.push_back(paramHeight);

			program->ExecuteProgram2D(programCL->GetProgram(), "RgbaToYUV420P", &vecParam, middleWidth, middleHeight);

			delete program;
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

int COpenCLEffectVideo::CopyOpenGLTexture(cl_mem cl_openglTexture, const int& width, const int& height, const bool &src)
{
	srcwidth = width;
	srcheight = height;

	COpenCLProgram* programCL = GetProgram("IDR_OPENCL_FFMPEG");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem *	openglSrc = new COpenCLParameterClMem(true);
		openglSrc->SetLibelle("input");
		openglSrc->SetNoDelete(true);
		openglSrc->SetValue(cl_openglTexture);
		vecParam.push_back(openglSrc);

		if (src)
		{
			if (paramSrcWidth == nullptr)
				paramSrcWidth = new COpenCLParameterInt();
			paramSrcWidth->SetValue(width);
			paramSrcWidth->SetNoDelete(true);
			paramSrcWidth->SetLibelle("width");
			vecParam.push_back(paramSrcWidth);

			if (paramSrcHeight == nullptr)
				paramSrcHeight = new COpenCLParameterInt();

			paramSrcHeight->SetValue(height);
			paramSrcHeight->SetNoDelete(true);
			paramSrcHeight->SetLibelle("height");
			vecParam.push_back(paramSrcHeight);
		}
		else
		{
			if (paramOutWidth == nullptr)
				paramOutWidth = new COpenCLParameterInt();
			paramOutWidth->SetValue(width);
			paramOutWidth->SetNoDelete(true);
			paramOutWidth->SetLibelle("width");
			vecParam.push_back(paramOutWidth);

			if (paramOutHeight == nullptr)
				paramOutHeight = new COpenCLParameterInt();

			paramOutHeight->SetValue(height);
			paramOutHeight->SetNoDelete(true);
			paramOutHeight->SetLibelle("height");
			vecParam.push_back(paramOutHeight);
		}


		program->SetParameter(&vecParam, width, height, width * height * GetSizeData());
		program->SetKeepOutput(true);
		program->ExecuteProgram(programCL->GetProgram(), "CopyFromOpenGLTexture");
		cl_mem memvalue = program->GetOutput();

		delete program;
		vecParam.clear();

		if (src)
		{
			if (paramSrc == nullptr)
				paramSrc = new COpenCLParameterClMem();
			paramSrc->SetValue(memvalue);
		}
		else
		{
			if (paramOutput == nullptr)
				paramOutput = new COpenCLParameterClMem();
			paramOutput->SetValue(memvalue);
		}
	}
	return 0;
}

void COpenCLEffectVideo::FlipVertical()
{
	COpenCLFilter openclFilter(context);
	if (paramOutput != nullptr)
	{
		cl_mem output = openclFilter.Flip("FlipVertical", paramOutput->GetValue(), widthOut, heightOut);
		paramOutput->SetValue(output);
	}
	else
	{
		cl_mem output = openclFilter.Flip("FlipVertical", paramSrc->GetValue(), srcwidth, srcheight);
		paramSrc->SetValue(output);
	}
}

void COpenCLEffectVideo::HQDn3D(Chqdn3d * hq3d, const double & LumSpac, const double & ChromSpac, const double & LumTmp, const double & ChromTmp)
{
	int _width = 0;
	int _height = 0;

	cl_mem yPicture = nullptr;

	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			yPicture = openclFilter.ConvertToY(paramOutput->GetValue(), _width, _height, "ConvertToYUchar");
		}


		long size = _width * _height;
		uint8_t * data_picture = new uint8_t[_width * _height];
		if (context != nullptr)
		{
			context->GetOutputData(yPicture, data_picture, size, flag);


		}
		uint8_t * dataOut = hq3d->ApplyDenoise3D(data_picture, _width, _height);

		COpenCLParameterByteArray * memDataOut = new COpenCLParameterByteArray();
		memDataOut->SetLibelle("input");
		memDataOut->SetValue(context->GetContext(), dataOut, size, flag);


		cl_mem output = nullptr;
		if (paramOutput != nullptr)
		{
			output = openclFilter.InsertYValue(memDataOut->GetValue(), paramOutput->GetValue(), _width, _height, "InsertYValueFromUchar");
		}

		paramOutput->SetValue(output);

		cl_int err;
		err = clReleaseMemObject(yPicture);
		Error::CheckError(err);
		yPicture = nullptr;

		delete[] data_picture;
		delete memDataOut;

	}

}

bool COpenCLEffectVideo::IsOk()
{
	if (paramOutput != nullptr)
		return true;
	return false;
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
	if (context->GetDefaultType() == OPENCL_FLOAT)
		return sizeof(cl_float) * 4;

	return sizeof(cl_uint);
}


int COpenCLEffectVideo::GetThumbnailWidth()
{
	return widthOut;
}

int COpenCLEffectVideo::GetThumbnailHeight()
{
	return heightOut;
}