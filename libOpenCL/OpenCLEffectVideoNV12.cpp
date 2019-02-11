#include "OpenCLEffectVideoNV12.h"
#include "OpenCLEngine.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include <RegardsBitmap.h>
#include <LoadingResource.h>
using namespace Regards::OpenCL;

COpenCLEffectVideoNV12::COpenCLEffectVideoNV12(COpenCLContext * context)
	:  COpenCLEffectVideo(context)
{
	inputY = nullptr;
	inputUV = nullptr;
	input= nullptr;
	paramwidthPitch= nullptr;
	paramsurfaceHeight= nullptr;
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
}

void COpenCLEffectVideoNV12::SetMemoryData(uint8_t * buffer, int size, const int &width, const int &height, const int &widthPitch, const int &heightSurface, const int &format)
{
	if (paramWidth != nullptr)
	{
		paramWidth->Release();
		delete paramWidth;
	}

	if(paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
	}
		
	if (paramHeight != nullptr)
	{
		paramHeight->Release();
		delete paramHeight;
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
		

	input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetNoDelete(true);
	input->SetValue(context->GetContext(), buffer, size, flag);
	
	paramWidth = new COpenCLParameterInt();
	paramWidth->SetNoDelete(true);
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(width);

	paramHeight = new COpenCLParameterInt();
	paramHeight->SetNoDelete(true);
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(height);

	paramwidthPitch = new COpenCLParameterInt();
	paramwidthPitch->SetNoDelete(true);
	paramwidthPitch->SetLibelle("widthPitch");
	paramwidthPitch->SetValue(widthPitch);

	paramsurfaceHeight = new COpenCLParameterInt();
	paramsurfaceHeight->SetNoDelete(true);
	paramsurfaceHeight->SetLibelle("heightSurface");
	paramsurfaceHeight->SetValue(heightSurface);

	dataIsOk = true;
	typeData = 2;
	formatData = format;
}

void COpenCLEffectVideoNV12::SetMemoryData(uint8_t * bufferY, int sizeY, uint8_t * bufferUV, int sizeUV, const int &width, const int &height, const int &widthPitch, const int &format)
{
	if (paramWidth != nullptr)
	{
		paramWidth->Release();
		delete paramWidth;
	}

	if(paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
	}
		
	if (paramHeight != nullptr)
	{
		paramHeight->Release();
		delete paramHeight;
	}

	if (paramwidthPitch != nullptr)
	{
		paramwidthPitch->Release();
		delete paramwidthPitch;
	}
	
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
		

	inputY = new COpenCLParameterByteArray();
	inputY->SetLibelle("inputY");
	inputY->SetNoDelete(true);
	inputY->SetValue(context->GetContext(), bufferY, sizeY, flag);
	
	inputUV = new COpenCLParameterByteArray();
	inputUV->SetLibelle("inputU");
	inputUV->SetNoDelete(true);
	inputUV->SetValue(context->GetContext(), bufferUV, sizeUV, flag);

	paramWidth = new COpenCLParameterInt();
	paramWidth->SetNoDelete(true);
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(width);

	paramHeight = new COpenCLParameterInt();
	paramHeight->SetNoDelete(true);
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(height);

	paramwidthPitch = new COpenCLParameterInt();
	paramwidthPitch->SetNoDelete(true);
	paramwidthPitch->SetLibelle("widthPitch");
	paramwidthPitch->SetValue(widthPitch);

	dataIsOk = true;
	typeData = 1;
	formatData = format;
}


void COpenCLEffectVideoNV12::InterpolationBicubic(const int &widthOut, const int &heightOut, const int &angle, const int &bicubic)
{
	if(context != nullptr)
	{
		COpenCLProgram * programCL;
		paramOutput = new COpenCLParameterClMem();

		this->widthOut = widthOut;
		this->heightOut = heightOut;
        
        printf("InterpolationBicubicNV12ToRGB32 formatData : %d typeData : %d \n",formatData,typeData);
		programCL = GetProgram("IDR_OPENCL_FFMPEGDXVA2YUV420");

		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			if(typeData == 1)
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

			COpenCLParameterInt * paramwidthOut = new COpenCLParameterInt();
			paramwidthOut->SetLibelle("widthOut");
			paramwidthOut->SetValue(widthOut);
			vecParam.push_back(paramwidthOut);

			COpenCLParameterInt * paramheightOut = new COpenCLParameterInt();
			paramheightOut->SetLibelle("heightOut");
			paramheightOut->SetValue(heightOut);
			vecParam.push_back(paramheightOut);

			COpenCLParameterInt * paramAngle = new COpenCLParameterInt();
			paramAngle->SetLibelle("angle");
			paramAngle->SetValue(angle);
			vecParam.push_back(paramAngle);

			COpenCLParameterInt * paramBicubic = new COpenCLParameterInt();
			paramBicubic->SetLibelle("bicubic");
			paramBicubic->SetValue(bicubic);
			vecParam.push_back(paramBicubic);

			vecParam.push_back(paramwidthPitch);

			if(typeData == 2)
			{
			
				vecParam.push_back(paramsurfaceHeight);
			}

			program->SetParameter(&vecParam, widthOut, heightOut);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "BicubicNV12toRegardsBitmap");
			paramOutput->SetValue(program->GetOutput());
			delete program;

            for (COpenCLParameter * parameter : vecParam)
			{
				if(!parameter->GetNoDelete())
				{
					delete parameter;
					parameter = nullptr;
				}
			}
			vecParam.clear();
		}
	}
}
