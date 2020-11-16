#include <header.h>
#include "OpenCLEffectVideo.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include <RegardsBitmap.h>
#include "utility.h"
#include "EffectVideoParameter.h"
#include "OpenCLFilter.h"
//#include <RegardsConfigParam.h>
//#include <ParamInit.h>
using namespace Regards::OpenCL;

COpenCLEffectVideo::COpenCLEffectVideo(COpenCLContext * context)
{
	openCLProgram = nullptr;
	bool useMemory =  (context->GetDeviceType() == CL_DEVICE_TYPE_GPU) ? false : true;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
    
	openCLProgram = nullptr;
	this->context = context;
	paramWidth = nullptr;
	paramHeight = nullptr;
	dataIsOk = false;
	paramOutput = nullptr;
	paramInput = nullptr;
	paramSrc = nullptr;
	paramSrcWidth = nullptr;
	paramSrcHeight = nullptr;
}


COpenCLEffectVideo::~COpenCLEffectVideo()
{
	if(dataIsOk)
	{
		if(paramOutput != nullptr)
		{
			paramOutput->Release();
			delete paramOutput;
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

		if (paramInput != nullptr)
		{
			paramInput->Release();
			delete paramInput;
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
	}
	
	if (openCLProgram != nullptr)
		delete openCLProgram;

	openCLProgram = nullptr;
}


int COpenCLEffectVideo::GetWidth()
{
	return width;
}

int COpenCLEffectVideo::GetHeight()
{
	return height;
}


void COpenCLEffectVideo::GetRgbaBitmap(cl_mem cl_image, int rgba)
{
	COpenCLProgram* programCL = GetProgram("IDR_OPENCL_BITMAPCONVERSION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

		if (paramOutput == nullptr)
		{
			paramOutput = new COpenCLParameterClMem();
			paramOutput->SetValue(cl_image);
			paramOutput->SetNoDelete(true);
		}
		else
			paramOutput->SetNoDelete(true);
		paramOutput->SetLibelle("input");
		
		vecParam.push_back(paramOutput);

		COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(widthOut);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(heightOut);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterInt* paramRGBA = new COpenCLParameterInt();
		paramRGBA->SetValue(rgba);
		paramRGBA->SetLibelle("rgba");
		vecParam.push_back(paramRGBA);

		program->SetKeepOutput(true);
		program->SetParameter(&vecParam, widthOut, heightOut, cl_image);
		program->ExecuteProgram(programCL->GetProgram(), "BitmapToOpenGLTexture");

		delete program;

		for (COpenCLParameter* parameter : vecParam)
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


void COpenCLEffectVideo::InterpolationBicubic(const int& widthOutput, const int& heightOutput, const int &flipH, const int &flipV, const int& angle, const int& bicubic)
{
	widthOut = widthOutput;
	heightOut = heightOutput;
	cl_mem memvalue = nullptr;
	COpenCLProgram* programCL = GetProgram("IDR_OPENCL_INTERPOLATION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

		paramSrc->SetLibelle("input");
		paramSrc->SetNoDelete(true);
		vecParam.push_back(paramSrc);

		paramSrcWidth->SetLibelle("width");
		vecParam.push_back(paramSrcWidth);

		paramSrcHeight->SetLibelle("height");
		vecParam.push_back(paramSrcHeight);

		COpenCLParameterInt* paramWidthOut = new COpenCLParameterInt();
		paramWidthOut->SetLibelle("widthOutput");
		paramWidthOut->SetValue(widthOutput);
		vecParam.push_back(paramWidthOut);

		COpenCLParameterInt* paramHeightOut = new COpenCLParameterInt();
		paramHeightOut->SetLibelle("heightOutput");
		paramHeightOut->SetValue(heightOutput);
		vecParam.push_back(paramHeightOut);

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


		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();


		paramOutput = new COpenCLParameterClMem();
		paramOutput->SetValue(memvalue);
	}
}

void COpenCLEffectVideo::InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect &rc, const int &flipH, const int &flipV, const int& angle, const int& bicubic)
{
	widthOut = widthOutput;
	heightOut = heightOutput;
	cl_mem memvalue = nullptr;
	COpenCLProgram* programCL = GetProgram("IDR_OPENCL_INTERPOLATION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

		paramSrc->SetLibelle("input");
		paramSrc->SetNoDelete(true);
		vecParam.push_back(paramSrc);

		paramSrcWidth->SetLibelle("width");
		vecParam.push_back(paramSrcWidth);

		paramSrcHeight->SetLibelle("height");
		vecParam.push_back(paramSrcHeight);

		COpenCLParameterInt* paramWidthOut = new COpenCLParameterInt();
		paramWidthOut->SetLibelle("widthOutput");
		paramWidthOut->SetValue(widthOutput);
		vecParam.push_back(paramWidthOut);

		COpenCLParameterInt* paramHeightOut = new COpenCLParameterInt();
		paramHeightOut->SetLibelle("heightOutput");
		paramHeightOut->SetValue(heightOutput);
		vecParam.push_back(paramHeightOut);

		COpenCLParameterFloat * left = new COpenCLParameterFloat();
		left->SetLibelle("left");
		left->SetValue(rc.x);
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


		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();


		paramOutput = new COpenCLParameterClMem();
		paramOutput->SetValue(memvalue);
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


			for (COpenCLParameter* parameter : vecParam)
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
	return bitmap;
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
		cl_int err = clEnqueueReadBuffer(context->GetCommandQueue(), ptData, CL_TRUE, 0,bitmap->GetBitmapSize(), bitmap->GetPtBitmap(), 0, nullptr, nullptr);
		Error::CheckError(err);
		err = clFinish(context->GetCommandQueue());
		Error::CheckError(err);
	}

	//paramOutput->Release();
	//delete paramOutput;
	//paramOutput = nullptr;
	return bitmap;

}

int COpenCLEffectVideo::CopyOpenGLTexture(cl_mem cl_openglTexture, const int& width, const int& height)
{
	srcwidth = width;
	srcheight = height;

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

	if (paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
		paramOutput = nullptr;
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

	COpenCLProgram* programCL = GetProgram("IDR_OPENCL_FFMPEG");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

		paramSrc = new COpenCLParameterClMem(true);
		paramSrc->SetLibelle("input");
		paramSrc->SetNoDelete(true);
		paramSrc->SetValue(cl_openglTexture);
		vecParam.push_back(paramSrc);

		paramSrcWidth = new COpenCLParameterInt();
		paramSrcWidth->SetValue(width);
		paramSrcWidth->SetNoDelete(true);
		paramSrcWidth->SetLibelle("width");
		vecParam.push_back(paramSrcWidth);

		paramSrcHeight = new COpenCLParameterInt();
		paramSrcHeight->SetValue(height);
		paramSrcHeight->SetNoDelete(true);
		paramSrcHeight->SetLibelle("height");
		vecParam.push_back(paramSrcHeight);

		program->SetParameter(&vecParam, width, height, width * height * GetSizeData());
		program->SetKeepOutput(true);
		program->ExecuteProgram(programCL->GetProgram(), "CopyFromOpenGLTexture");
		cl_mem memvalue = program->GetOutput();

		delete program;

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();

		delete paramSrc;
		paramSrc = new COpenCLParameterClMem();
		paramSrc->SetValue(memvalue);
		dataIsOk = true;
	}
	return 0;
}

/*
int COpenCLEffectVideo::InterpolationBicubicOpenGLTexture(cl_mem cl_openglTexture, const int& width, const int& height, const int& widthOutput, const int& heightOutput, const int & flipH, const int &flipV, const int& angle, const int& bicubic)
{

	widthOut = widthOutput;
	heightOut = heightOutput;

	if (paramWidth != nullptr)
	{
		paramWidth->Release();
		delete paramWidth;
	}

	if (paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
	}

	if (paramHeight != nullptr)
	{
		paramHeight->Release();
		delete paramHeight;
	}

	COpenCLProgram* programCL = GetProgram("IDR_OPENCL_FFMPEG");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

		paramOutput = new COpenCLParameterClMem(true);
		paramOutput->SetLibelle("input");
		paramOutput->SetNoDelete(true);
		paramOutput->SetValue(cl_openglTexture);
		vecParam.push_back(paramOutput);

		paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetNoDelete(true);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetNoDelete(true);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterInt* paramWidthOut = new COpenCLParameterInt();
		paramWidthOut->SetLibelle("widthOutput");
		paramWidthOut->SetValue(widthOutput);
		vecParam.push_back(paramWidthOut);

		COpenCLParameterInt* paramHeightOut = new COpenCLParameterInt();
		paramHeightOut->SetLibelle("heightOutput");
		paramHeightOut->SetValue(heightOutput);
		vecParam.push_back(paramHeightOut);

		COpenCLParameterInt* paramflipH = new COpenCLParameterInt();
		paramflipH->SetLibelle("flipH");
		paramflipH->SetValue(flipH);
		vecParam.push_back(paramflipH);

		COpenCLParameterInt* paramflipV = new COpenCLParameterInt();
		paramflipV->SetLibelle("flipV");
		paramflipV->SetValue(flipV);
		vecParam.push_back(paramflipV);

		COpenCLParameterInt* paramAngle = new COpenCLParameterInt();
		paramAngle->SetLibelle("angle");
		paramAngle->SetValue(angle);
		vecParam.push_back(paramAngle);


		COpenCLParameterInt* parambicubic = new COpenCLParameterInt();
		parambicubic->SetLibelle("bicubic");
		parambicubic->SetValue(bicubic);
		vecParam.push_back(parambicubic);

		program->SetParameter(&vecParam, widthOutput, heightOutput, widthOutput * heightOutput * GetSizeData());
		program->SetKeepOutput(true);
		program->ExecuteProgram(programCL->GetProgram(), "InterpolationFromOpenGLTexture");
		cl_mem memvalue = program->GetOutput();

		delete program;

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();

		delete paramOutput;
		paramOutput = new COpenCLParameterClMem();
		paramOutput->SetValue(memvalue);
		dataIsOk = true;
	}
	return 0;
}


int COpenCLEffectVideo::InterpolationBicubicZoneOpenGLTexture(cl_mem cl_openglTexture, const int& width, const int& height, const int& widthOutput, const int& heightOutput, const wxRect &rc, const int & flipH, const int &flipV, const int& angle, const int& bicubic)
{

	widthOut = widthOutput;
	heightOut = heightOutput;

	if (paramWidth != nullptr)
	{
		paramWidth->Release();
		delete paramWidth;
	}

	if (paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
	}

	if (paramHeight != nullptr)
	{
		paramHeight->Release();
		delete paramHeight;
	}

	COpenCLProgram* programCL = GetProgram("IDR_OPENCL_FFMPEG");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

		paramOutput = new COpenCLParameterClMem(true);
		paramOutput->SetLibelle("input");
		paramOutput->SetNoDelete(true);
		paramOutput->SetValue(cl_openglTexture);
		vecParam.push_back(paramOutput);

		paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetNoDelete(true);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetNoDelete(true);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterInt* paramWidthOut = new COpenCLParameterInt();
		paramWidthOut->SetLibelle("widthOutput");
		paramWidthOut->SetValue(widthOutput);
		vecParam.push_back(paramWidthOut);

		COpenCLParameterInt* paramHeightOut = new COpenCLParameterInt();
		paramHeightOut->SetLibelle("heightOutput");
		paramHeightOut->SetValue(heightOutput);
		vecParam.push_back(paramHeightOut);

		COpenCLParameterFloat * left = new COpenCLParameterFloat();
		left->SetLibelle("left");
		left->SetValue(rc.x);
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

		COpenCLParameterInt* paramAngle = new COpenCLParameterInt();
		paramAngle->SetLibelle("angle");
		paramAngle->SetValue(angle);
		vecParam.push_back(paramAngle);


		COpenCLParameterInt* parambicubic = new COpenCLParameterInt();
		parambicubic->SetLibelle("bicubic");
		parambicubic->SetValue(bicubic);
		vecParam.push_back(parambicubic);

		program->SetParameter(&vecParam, widthOutput, heightOutput, widthOutput * heightOutput * GetSizeData());
		program->SetKeepOutput(true);
		program->ExecuteProgram(programCL->GetProgram(), "InterpolationZone");
		cl_mem memvalue = program->GetOutput();

		delete program;

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();

		delete paramOutput;
		paramOutput = new COpenCLParameterClMem();
		paramOutput->SetValue(memvalue);
		dataIsOk = true;
	}
	return 0;
}
*/
void COpenCLEffectVideo::FlipVertical()
{
	COpenCLFilter openclFilter(context);
	cl_mem output = nullptr;
	output = openclFilter.Flip("FlipVertical", paramOutput->GetValue(), widthOut, heightOut);

	if (paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
	}

	paramOutput = new COpenCLParameterClMem();
	paramOutput->SetValue(output);
}

void COpenCLEffectVideo::ApplyVideoEffect(CVideoEffectParameter * videoEffectParameter)
{
	if(videoEffectParameter->effectEnable)
	{
		COpenCLFilter openclFilter(context);

		if(videoEffectParameter->bandcEnable)
		{
			cl_mem output = nullptr;
			output = openclFilter.BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast, paramOutput->GetValue(), widthOut, heightOut);

			if(paramOutput != nullptr)
			{
				paramOutput->Release();
				delete paramOutput;
			}

			paramOutput = new COpenCLParameterClMem();
			paramOutput->SetValue(output);
		}
	
		if(videoEffectParameter->ColorBoostEnable)
		{
			cl_mem output = nullptr;
			output = openclFilter.RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1], videoEffectParameter->color_boost[2], paramOutput->GetValue(), widthOut, heightOut);

			if(paramOutput != nullptr)
			{
				paramOutput->Release();
				delete paramOutput;
			}

			paramOutput = new COpenCLParameterClMem();
			paramOutput->SetValue(output);
		}

	
		if(videoEffectParameter->SharpenEnable)
		{
			cl_mem output = nullptr;
			output = openclFilter.SharpenMasking(videoEffectParameter->sharpness, paramOutput->GetValue(), widthOut, heightOut);

			if(paramOutput != nullptr)
			{
				paramOutput->Release();
				delete paramOutput;
			}

			paramOutput = new COpenCLParameterClMem();
			paramOutput->SetValue(output);
		}
		
		if(videoEffectParameter->denoiseEnable)
		{
			cl_mem output = nullptr;
			output = openclFilter.Denoise("Denoise", videoEffectParameter->uSigma, videoEffectParameter->uThreshold / 100.0f, videoEffectParameter->uKSigma, paramOutput->GetValue(), widthOut, heightOut);

			if(paramOutput != nullptr)
			{
				paramOutput->Release();
				delete paramOutput;
			}

			paramOutput = new COpenCLParameterClMem();
			paramOutput->SetValue(output);
		}
		
		if(videoEffectParameter->grayEnable)
		{
			cl_mem output = nullptr;
			output = openclFilter.ColorEffect("GrayLevel", paramOutput->GetValue(), widthOut, heightOut);

			if(paramOutput != nullptr)
			{
				paramOutput->Release();
				delete paramOutput;
			}

			paramOutput = new COpenCLParameterClMem();
			paramOutput->SetValue(output);
		}

		if(videoEffectParameter->sepiaEnable)
		{
			cl_mem output = nullptr;
			output = openclFilter.ColorEffect("Sepia", paramOutput->GetValue(), widthOut, heightOut);

			if(paramOutput != nullptr)
			{
				paramOutput->Release();
				delete paramOutput;
			}

			paramOutput = new COpenCLParameterClMem();
			paramOutput->SetValue(output);
		}
	}
}


bool COpenCLEffectVideo::IsOk()
{
	return dataIsOk;
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