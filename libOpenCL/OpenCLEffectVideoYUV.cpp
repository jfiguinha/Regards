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
	inputY = nullptr;
	inputU = nullptr;
	inputV = nullptr;
	paramLineSize = nullptr;
}


COpenCLEffectVideoYUV::~COpenCLEffectVideoYUV()
{
	if (paramLineSize != nullptr)
	{
		paramLineSize->Release();
		delete paramLineSize;
	}
	
	if (inputU != nullptr)
	{
		inputU->Release();
		delete inputU;
	}

	if (inputY != nullptr)
	{
		inputY->Release();
		delete inputY;
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

void COpenCLEffectVideoYUV::SetMemoryDataNV12(uint8_t * bufferY, int sizeY, uint8_t * bufferUV, int sizeUV, const int &width, const int &height, const int &lineSize)
{
	if (inputY == nullptr)
		inputY = new COpenCLParameterByteArray();
	inputY->SetLibelle("inputY");
	inputY->SetNoDelete(true);
	inputY->SetValue(openclContext->GetContext(), bufferY, sizeY, flag);

	if (inputU == nullptr)
		inputU = new COpenCLParameterByteArray();
	inputU->SetLibelle("inputUV");
	inputU->SetNoDelete(true);
	inputU->SetValue(openclContext->GetContext(), bufferUV, sizeUV, flag);

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


void COpenCLEffectVideoYUV::SetMemoryData(uint8_t * bufferY, int sizeY, uint8_t * bufferU, int sizeU, uint8_t * bufferV, int sizeV, const int &width, const int &height, const int &lineSize)
{
	if (inputY == nullptr)
		inputY = new COpenCLParameterByteArray();
	inputY->SetLibelle("inputY");
	inputY->SetNoDelete(true);
	inputY->SetValue(openclContext->GetContext(), bufferY, sizeY, flag);
	
	if (inputU == nullptr)
		inputU = new COpenCLParameterByteArray();
	inputU->SetLibelle("inputU");
	inputU->SetNoDelete(true);
	inputU->SetValue(openclContext->GetContext(), bufferU, sizeU, flag);

	if (inputV == nullptr)
		inputV = new COpenCLParameterByteArray();
	inputV->SetNoDelete(true);
	inputV->SetLibelle("inputV");
	inputV->SetValue(openclContext->GetContext(), bufferV, sizeV, flag);

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

int COpenCLEffectVideoYUV::GetSizeData()
{
	return sizeof(float) * 4;
}


void COpenCLEffectVideoYUV::TranscodePicture(const int &widthOut, const int &heightOut, const int& rgba)
{
	if (!needToTranscode)
		return;

	cv::UMat dest;
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

				vecParam.push_back(inputY);
				vecParam.push_back(inputU);
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

				vecParam.push_back(paramLineSize);


				int depth = (openclContext->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
				int type = CV_MAKE_TYPE(depth, 4);
				dest.create((int)heightOut, (int)widthOut, type);
				cl_mem clBuffer = (cl_mem)dest.handle(cv::ACCESS_RW);
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

				vecParam.push_back(inputY);
				vecParam.push_back(inputU);
				vecParam.push_back(inputV);
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

				int depth = (openclContext->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
				int type = CV_MAKE_TYPE(depth, 4);
				dest.create((int)heightOut, (int)widthOut, type);
				cl_mem clBuffer = (cl_mem)dest.handle(cv::ACCESS_RW);
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
	cvtColor(dest, paramSrc, cv::COLOR_RGBA2BGR);
}

