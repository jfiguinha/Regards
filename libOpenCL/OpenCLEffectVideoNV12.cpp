#include <header.h>
#include "OpenCLEffectVideoNV12.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include "OpenCLContext.h"
using namespace Regards::OpenCL;

/**
 * \brief 
 * \param context 
 */
COpenCLEffectVideoNV12::COpenCLEffectVideoNV12(COpenCLContext * context)
	: COpenCLEffectVideo(context), formatData(0)
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
	input->SetValue(context->GetContext(), buffer, size, flag);
	
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
	inputY->SetValue(context->GetContext(), bufferY, sizeY, flag);
	
	if (inputUV != nullptr)
		inputUV = new COpenCLParameterByteArray();
	inputUV->SetLibelle("inputU");
	inputUV->SetNoDelete(true);
	inputUV->SetValue(context->GetContext(), bufferUV, sizeUV, flag);

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

void COpenCLEffectVideoNV12::TranscodePicture(const int &widthOut, const int &heightOut)
{
	if (!needToTranscode)
		return;

	srcwidth = widthOut;
	srcheight = heightOut;

	if (context != nullptr)
	{
		COpenCLProgram * programCL;

		this->widthOut = widthOut;
		this->heightOut = heightOut;

		programCL = GetProgram("IDR_OPENCL_FFMPEGDXVA2YUV420");

		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

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

			if (paramSrcWidth == nullptr)
				paramSrcWidth = new COpenCLParameterInt();
			paramSrcWidth->SetLibelle("widthOut");
			paramSrcWidth->SetNoDelete(true);
			paramSrcWidth->SetValue(widthOut);
			vecParam.push_back(paramSrcWidth);

			if (paramSrcHeight == nullptr)
				paramSrcHeight = new COpenCLParameterInt();
			paramSrcHeight->SetLibelle("heightOut");
			paramSrcHeight->SetValue(heightOut);
			paramSrcHeight->SetNoDelete(true);
			vecParam.push_back(paramSrcHeight);

			vecParam.push_back(paramwidthPitch);

			if (typeData == 2)
			{

				vecParam.push_back(paramsurfaceHeight);
			}

			program->SetParameter(&vecParam, widthOut, heightOut);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "Convert");


			if (paramSrc == nullptr)
				paramSrc = new COpenCLParameterClMem();
			paramSrc->SetValue(program->GetOutput());
			delete program;
			vecParam.clear();
		}
	}

}