#include <header.h>
#include "OpenCLEffectVideoNV12.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include "OpenCLContext.h"
using namespace Regards::OpenCL;
extern COpenCLContext* openclContext;
/**
 * \brief 
 * \param context 
 */
COpenCLEffectVideoNV12::COpenCLEffectVideoNV12()
	: COpenCLEffectVideo(), formatData(0)
{
	inputY = nullptr;
	inputUV = nullptr;
	input = nullptr;
	paramwidthPitch = nullptr;
	paramsurfaceHeight = nullptr;
	typeData = 2;
}


COpenCLEffectVideoNV12::~COpenCLEffectVideoNV12()
{
	if (inputUV != nullptr)
	{
		inputUV->Release();
		delete inputUV;
	}

	if (inputY != nullptr)
	{
		inputY->Release();
		delete inputY;
	}
		
	if (input != nullptr)
	{
		input->Release();
		delete input;
	}

	if (paramwidthPitch != nullptr)
	{
		paramwidthPitch->Release();
		delete paramwidthPitch;
	}
	
	if (paramsurfaceHeight != nullptr)
	{
		paramsurfaceHeight->Release();
		delete paramsurfaceHeight;
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

void COpenCLEffectVideoNV12::SetMemoryData(uint8_t * buffer, int size, const int &width, const int &height, const int &widthPitch, const int &heightSurface, const int &format)
{
	if(input != nullptr)
		input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetNoDelete(true);
	input->SetValue(openclContext->GetContext(), buffer, size, flag);
	
	if (paramWidth != nullptr)
		paramWidth = new COpenCLParameterInt();
	paramWidth->SetNoDelete(true);
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(width);

	if (paramHeight != nullptr)
		paramHeight = new COpenCLParameterInt();
	paramHeight->SetNoDelete(true);
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(height);

	if (paramwidthPitch != nullptr)
		paramwidthPitch = new COpenCLParameterInt();
	paramwidthPitch->SetNoDelete(true);
	paramwidthPitch->SetLibelle("widthPitch");
	paramwidthPitch->SetValue(widthPitch);

	if (paramsurfaceHeight != nullptr)
		paramsurfaceHeight = new COpenCLParameterInt();
	paramsurfaceHeight->SetNoDelete(true);
	paramsurfaceHeight->SetLibelle("heightSurface");
	paramsurfaceHeight->SetValue(heightSurface);

	typeData = 2;
	formatData = format;
	needToTranscode = true;
}

void COpenCLEffectVideoNV12::SetMemoryData(uint8_t * bufferY, int sizeY, uint8_t * bufferUV, int sizeUV, const int &width, const int &height, const int &widthPitch, const int &format)
{
	if (inputY != nullptr)
		inputY = new COpenCLParameterByteArray();
	inputY->SetLibelle("inputY");
	inputY->SetNoDelete(true);
	inputY->SetValue(openclContext->GetContext(), bufferY, sizeY, flag);
	
	if (inputUV != nullptr)
		inputUV = new COpenCLParameterByteArray();
	inputUV->SetLibelle("inputU");
	inputUV->SetNoDelete(true);
	inputUV->SetValue(openclContext->GetContext(), bufferUV, sizeUV, flag);

	if (paramWidth != nullptr)
		paramWidth = new COpenCLParameterInt();
	paramWidth->SetNoDelete(true);
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(width);

	if (paramHeight != nullptr)
		paramHeight = new COpenCLParameterInt();
	paramHeight->SetNoDelete(true);
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(height);

	if (paramwidthPitch != nullptr)
		paramwidthPitch = new COpenCLParameterInt();
	paramwidthPitch->SetNoDelete(true);
	paramwidthPitch->SetLibelle("widthPitch");
	paramwidthPitch->SetValue(widthPitch);
	isOk = true;
	typeData = 1;
	formatData = format;
	needToTranscode = true;
}


bool COpenCLEffectVideoNV12::IsOk()
{
	return isOk;
}

void COpenCLEffectVideoNV12::TranscodePicture(const int &widthOut, const int &heightOut, const int& rgba)
{
	if (!needToTranscode)
		return;

	if (openclContext != nullptr)
	{
		COpenCLProgram * programCL;

		programCL = GetProgram("IDR_OPENCL_FFMPEGDXVA2YUV420");

		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(flag);

			if (typeData == 1)
			{
				vecParam.push_back(inputY);
				vecParam.push_back(inputUV);

			}
			else
			{
				vecParam.push_back(input);
			}

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

			vecParam.push_back(paramwidthPitch);

			if (typeData == 2)
			{

				vecParam.push_back(paramsurfaceHeight);
			}


			paramSrc.release();
			int depth = (openclContext->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			paramSrc.create((int)heightOut, (int)widthOut, type);
			cl_mem clBuffer = (cl_mem)paramSrc.handle(cv::ACCESS_RW);
			program->SetParameter(&vecParam, widthOut, heightOut, clBuffer);

			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "Convert");


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
	cv::cvtColor(paramSrc, paramSrc, cv::COLOR_BGRA2BGR);
}