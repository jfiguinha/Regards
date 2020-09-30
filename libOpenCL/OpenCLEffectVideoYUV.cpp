#include <header.h>
#include "OpenCLEffectVideoYUV.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLContext.h"
#include "OpenCLProgram.h"
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
}

void COpenCLEffectVideoYUV::SetMemoryData(uint8_t * bufferY, int sizeY, uint8_t * bufferU, int sizeU, uint8_t * bufferV, int sizeV, const int &width, const int &height, const int &lineSize, const int &format)
{
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

	if (paramLineSize != nullptr)
	{
		paramLineSize->Release();
		delete paramLineSize;
	}

	if(paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
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

	inputY = new COpenCLParameterByteArray();
	inputY->SetLibelle("inputY");
	inputY->SetNoDelete(true);
	inputY->SetValue(context->GetContext(), bufferY, sizeY, flag);
	
	inputU = new COpenCLParameterByteArray();
	inputU->SetLibelle("inputU");
	inputU->SetNoDelete(true);
	inputU->SetValue(context->GetContext(), bufferU, sizeU, flag);

	inputV = new COpenCLParameterByteArray();
	inputV->SetNoDelete(true);
	inputV->SetLibelle("inputV");
	inputV->SetValue(context->GetContext(), bufferV, sizeV, flag);

	paramWidth = new COpenCLParameterInt();
	paramWidth->SetNoDelete(true);
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(width);

	paramHeight = new COpenCLParameterInt();
	paramHeight->SetNoDelete(true);
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(height);

	paramLineSize = new COpenCLParameterInt();
	paramLineSize->SetNoDelete(true);
	paramLineSize->SetLibelle("LineSize");
	paramLineSize->SetValue(lineSize);

	dataIsOk = true;
	formatData = format;
}

int COpenCLEffectVideoYUV::GetSizeData()
{
	return sizeof(float) * 4;
}

void COpenCLEffectVideoYUV::InterpolationBicubic(const int &widthOut, const int &heightOut, const int &angle, const int &bicubic)
{
	if(context != nullptr)
	{

		COpenCLProgram * programCL = nullptr;
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
			paramOutput = new COpenCLParameterClMem();

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

			vecParam.push_back(paramLineSize);

			program->SetParameter(&vecParam, widthOut, heightOut, widthOut * heightOut * GetSizeData());
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "BicubicYUVtoRegardsBitmap");
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

void COpenCLEffectVideoYUV::DeleteData()
{
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

	if (paramLineSize != nullptr)
	{
		paramLineSize->Release();
		delete paramLineSize;
	}

	if(paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
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

	inputY = nullptr;
	inputU = nullptr;
	inputV = nullptr;
	paramWidth = nullptr;
	paramHeight = nullptr;
	paramLineSize = nullptr;
	dataIsOk = false;
}