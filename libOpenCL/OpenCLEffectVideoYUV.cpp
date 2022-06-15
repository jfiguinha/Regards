#include <header.h>
#include "OpenCLEffectVideoYUV.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLContext.h"
#include "OpenCLProgram.h"

using namespace Regards::OpenCL;
extern COpenCLContext* openclContext;

COpenCLEffectVideoYUV::COpenCLEffectVideoYUV()
	:  COpenCLEffectVideo()
{
	paramLineSize = nullptr;
}


COpenCLEffectVideoYUV::~COpenCLEffectVideoYUV()
{
	if (paramLineSize != nullptr)
	{
		paramLineSize->Release();
		delete paramLineSize;
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

void COpenCLEffectVideoYUV::SetMemoryDataNV12(const cv::Mat& bufferY, const cv::Mat& bufferUV, const int &width, const int &height, const int &lineSize)
{
	bufferY.copyTo(inputY);
	bufferUV.copyTo(inputU);

	if (paramWidth == nullptr)
		paramWidth = new COpenCLParameterInt();
	paramWidth->SetNoDelete(true);
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(width);

	if (paramHeight == nullptr)
		paramHeight = new COpenCLParameterInt();
	paramHeight->SetNoDelete(true);
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(height);

	if (paramLineSize == nullptr)
		paramLineSize = new COpenCLParameterInt();
	paramLineSize->SetNoDelete(true);
	paramLineSize->SetLibelle("LineSize");
	paramLineSize->SetValue(lineSize);
	isOk = true;
	formatData = 1;
	needToTranscode = true;
}


void COpenCLEffectVideoYUV::SetMemoryData(const cv::Mat& bufferY, const cv::Mat& bufferU, const cv::Mat& bufferV, const int &width, const int &height, const int &lineSize)
{
	bufferY.copyTo(inputY);
	bufferU.copyTo(inputU);
	bufferV.copyTo(inputV);

	if (paramWidth == nullptr)
		paramWidth = new COpenCLParameterInt();
	paramWidth->SetNoDelete(true);
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(width);

	if (paramHeight == nullptr)
		paramHeight = new COpenCLParameterInt();
	paramHeight->SetNoDelete(true);
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(height);

	if (paramLineSize == nullptr)
		paramLineSize = new COpenCLParameterInt();
	paramLineSize->SetNoDelete(true);
	paramLineSize->SetLibelle("LineSize");
	paramLineSize->SetValue(lineSize);
	isOk = true;
	formatData = 0;
	needToTranscode = true;
}

bool COpenCLEffectVideoYUV::IsOk()
{
	return isOk;
}



void COpenCLEffectVideoYUV::TranscodePicture(const int &widthOut, const int &heightOut, const int& rgba)
{
	if (!needToTranscode)
		return;

	if (openclContext != nullptr)
	{
		COpenCLProgram * program_cl;
		if (formatData == 1)
		{
			program_cl = GetProgram("IDR_OPENCL_FFMPEGNV12");

			if (program_cl != nullptr)
			{
				vector<COpenCLParameter *> vecParam;
				COpenCLExecuteProgram * program = new COpenCLExecuteProgram(flag);

				COpenCLParameterClMem* y = new COpenCLParameterClMem(true);
				y->SetValue((cl_mem)inputY.handle(cv::ACCESS_READ));
				y->SetLibelle("inputY");
				y->SetNoDelete(true);
				vecParam.push_back(y);

				COpenCLParameterClMem* uv = new COpenCLParameterClMem(true);
				uv->SetValue((cl_mem)inputU.handle(cv::ACCESS_READ));
				uv->SetLibelle("inputU");
				uv->SetNoDelete(true);
				vecParam.push_back(uv);

				vecParam.push_back(paramWidth);
				vecParam.push_back(paramHeight);

				COpenCLParameterInt * paramSrcWidth = new COpenCLParameterInt();
				paramSrcWidth->SetLibelle("widthOut");
				paramSrcWidth->SetValue(widthOut);
				vecParam.push_back(paramSrcWidth);

				COpenCLParameterInt * paramSrcHeight = new COpenCLParameterInt();
				paramSrcHeight->SetLibelle("heightOut");
				paramSrcHeight->SetValue(heightOut);
				vecParam.push_back(paramSrcHeight);

				COpenCLParameterInt* paramRgba = new COpenCLParameterInt();
				paramRgba->SetLibelle("rgba");
				paramRgba->SetValue(rgba);
				vecParam.push_back(paramRgba);

				vecParam.push_back(paramLineSize);

				paramSrc.release();
				int depth = (openclContext->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
				int type = CV_MAKE_TYPE(depth, 4);
				paramSrc.create((int)heightOut, (int)widthOut, type);
				cl_mem clBuffer = (cl_mem)paramSrc.handle(cv::ACCESS_WRITE);
				program->SetParameter(&vecParam, widthOut, heightOut, clBuffer);
				program->SetKeepOutput(true);
				program->ExecuteProgram(program_cl->GetProgram(), "Convert");
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
			}
		}
		else
		{
			program_cl = GetProgram("IDR_OPENCL_FFMPEGYUV420");

			if (program_cl != nullptr)
			{
				vector<COpenCLParameter *> vecParam;
				COpenCLExecuteProgram * program = new COpenCLExecuteProgram(flag);
				COpenCLParameterClMem* y = new COpenCLParameterClMem(true);
				y->SetValue((cl_mem)inputY.handle(cv::ACCESS_READ));
				y->SetLibelle("inputY");
				y->SetNoDelete(true);
				vecParam.push_back(y);

				COpenCLParameterClMem* u = new COpenCLParameterClMem(true);
				u->SetValue((cl_mem)inputU.handle(cv::ACCESS_READ));
				u->SetLibelle("inputU");
				u->SetNoDelete(true);
				vecParam.push_back(u);

				COpenCLParameterClMem* v = new COpenCLParameterClMem(true);
				v->SetValue((cl_mem)inputV.handle(cv::ACCESS_READ));
				v->SetLibelle("inputV");
				v->SetNoDelete(true);
				vecParam.push_back(v);

				vecParam.push_back(paramWidth);
				vecParam.push_back(paramHeight);

				COpenCLParameterInt * paramSrcWidth = new COpenCLParameterInt();
				paramSrcWidth->SetLibelle("widthOut");
				paramSrcWidth->SetValue(widthOut);
				vecParam.push_back(paramSrcWidth);

				COpenCLParameterInt * paramSrcHeight = new COpenCLParameterInt();
				paramSrcHeight->SetLibelle("heightOut");
				paramSrcHeight->SetValue(heightOut);
				vecParam.push_back(paramSrcHeight);

				COpenCLParameterInt* paramRgba = new COpenCLParameterInt();
				paramRgba->SetLibelle("rgba");
				paramRgba->SetValue(rgba);
				vecParam.push_back(paramRgba);

				vecParam.push_back(paramLineSize);

				paramSrc.release();
				int depth = (openclContext->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
				int type = CV_MAKE_TYPE(depth, 4);
				paramSrc.create((int)heightOut, (int)widthOut, type);
				cl_mem clBuffer = (cl_mem)paramSrc.handle(cv::ACCESS_RW);
				program->SetParameter(&vecParam, widthOut, heightOut, clBuffer);
				program->SetKeepOutput(true);
				program->ExecuteProgram(program_cl->GetProgram(), "Convert");
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
			}
		}

	}
	//imwrite("d:\\test.jpg", paramSrc);
	cvtColor(paramSrc, paramSrc, cv::COLOR_RGBA2BGR);
	//imwrite("d:\\test.jpg", paramSrc);
}

