#include <header.h>
#include "OpenCLEffectVideoYUV.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLContext.h"
#include "OpenCLProgram.h"
#include "utility.h"
#include <RegardsBitmap.h>
using namespace Regards::OpenCL;

COpenCLEffectVideoYUV::COpenCLEffectVideoYUV(COpenCLContext * context)
	:  COpenCLEffectVideo(context)
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
	inputY->SetValue(context->GetContext(), bufferY, sizeY, flag);

	if (inputU == nullptr)
		inputU = new COpenCLParameterByteArray();
	inputU->SetLibelle("inputUV");
	inputU->SetNoDelete(true);
	inputU->SetValue(context->GetContext(), bufferUV, sizeUV, flag);

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
	inputY->SetValue(context->GetContext(), bufferY, sizeY, flag);
	
	if (inputU == nullptr)
		inputU = new COpenCLParameterByteArray();
	inputU->SetLibelle("inputU");
	inputU->SetNoDelete(true);
	inputU->SetValue(context->GetContext(), bufferU, sizeU, flag);

	if (inputV == nullptr)
		inputV = new COpenCLParameterByteArray();
	inputV->SetNoDelete(true);
	inputV->SetLibelle("inputV");
	inputV->SetValue(context->GetContext(), bufferV, sizeV, flag);

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


void COpenCLEffectVideoYUV::TranscodePicture(const int &widthOut, const int &heightOut)
{
	if (!needToTranscode)
		return;
	srcwidth = widthOut;
	srcheight = heightOut;

	if (context != nullptr)
	{
		COpenCLProgram * programCL = nullptr;
		if (formatData == 1)
		{
			programCL = GetProgram("IDR_OPENCL_FFMPEGNV12");

			if (programCL != nullptr)
			{
				vector<COpenCLParameter *> vecParam;
				COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

				this->widthOut = widthOut;
				this->heightOut = heightOut;

				vecParam.push_back(inputY);
				vecParam.push_back(inputU);
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

				vecParam.push_back(paramLineSize);

				program->SetParameter(&vecParam, widthOut, heightOut, widthOut * heightOut * GetSizeData());
				program->SetKeepOutput(true);
				program->ExecuteProgram(programCL->GetProgram(), "Convert");

				if (paramSrc == nullptr)
					paramSrc = new COpenCLParameterClMem();
				paramSrc->SetValue(program->GetOutput());
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
			programCL = GetProgram("IDR_OPENCL_FFMPEGYUV420");

			if (programCL != nullptr)
			{
				vector<COpenCLParameter *> vecParam;
				COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

				this->widthOut = widthOut;
				this->heightOut = heightOut;

				vecParam.push_back(inputY);
				vecParam.push_back(inputU);
				vecParam.push_back(inputV);
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

				vecParam.push_back(paramLineSize);

				program->SetParameter(&vecParam, widthOut, heightOut, widthOut * heightOut * GetSizeData());
				program->SetKeepOutput(true);
				program->ExecuteProgram(programCL->GetProgram(), "Convert");

				if (paramSrc == nullptr)
					paramSrc = new COpenCLParameterClMem();
				paramSrc->SetValue(program->GetOutput());
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
}

void COpenCLEffectVideoYUV::LoadRegardsBitmap(CRegardsBitmap * bitmap)
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

	if (paramWidth == nullptr)
		paramWidth = new COpenCLParameterInt();
	paramWidth->SetNoDelete(true);
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(bitmap->GetBitmapWidth());

	if (paramHeight == nullptr)
		paramHeight = new COpenCLParameterInt();
	paramHeight->SetNoDelete(true);
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(bitmap->GetBitmapHeight());

	needToTranscode = false;

}