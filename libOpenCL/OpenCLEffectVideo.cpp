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
	paramOutWidth = nullptr;
	paramOutHeight = nullptr;
	//dataIsOk = false;
	paramOutput = nullptr;
	//paramInput = nullptr;
	paramSrc = nullptr;
	paramSrcWidth = nullptr;
	paramSrcHeight = nullptr;
}


COpenCLEffectVideo::~COpenCLEffectVideo()
{
	if(paramOutput != nullptr)
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

			for (COpenCLParameter* parameter : vecParam)
			{
				if (!parameter->GetNoDelete())
				{
					delete parameter;
					parameter = nullptr;
				}
			}
			vecParam.clear();
			paramOutput = nullptr;
			paramOutWidth = nullptr;
			paramOutHeight = nullptr;

		}
	}


}


void COpenCLEffectVideo::InterpolationBicubic(const int& widthOutput, const int& heightOutput, const int &flipH, const int &flipV, const int& angle, const int& bicubic)
{
	if (paramSrc != nullptr && paramSrcWidth != nullptr && paramSrcHeight != nullptr)
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

			if(paramOutWidth == nullptr)
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


			for (COpenCLParameter* parameter : vecParam)
			{
				if (!parameter->GetNoDelete())
				{
					delete parameter;
					parameter = nullptr;
				}
			}
			vecParam.clear();

			if(paramOutput == nullptr)
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

	return bitmap;

}

int COpenCLEffectVideo::CopyOpenGLTexture(cl_mem cl_openglTexture, const int& width, const int& height)
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

		if (paramSrc == nullptr)
			paramSrc = new COpenCLParameterClMem();
		paramSrc->SetValue(memvalue);
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

}

void COpenCLEffectVideo::ApplyVideoEffect(CVideoEffectParameter * videoEffectParameter)
{
	if(videoEffectParameter->effectEnable)
	{
		COpenCLFilter openclFilter(context);

		if(videoEffectParameter->bandcEnable)
		{
			if (paramOutput != nullptr)
			{
				cl_mem output = openclFilter.BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast, paramOutput->GetValue(), widthOut, heightOut);
				paramOutput->SetValue(output);
			}
		}
	
		if(videoEffectParameter->ColorBoostEnable)
		{
			if (paramOutput != nullptr)
			{
				cl_mem output = openclFilter.RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1], videoEffectParameter->color_boost[2], paramOutput->GetValue(), widthOut, heightOut);
				paramOutput->SetValue(output);
			}

		}

	
		if(videoEffectParameter->SharpenEnable)
		{
			if (paramOutput != nullptr)
			{
				cl_mem output = openclFilter.SharpenMasking(videoEffectParameter->sharpness, paramOutput->GetValue(), widthOut, heightOut);
				paramOutput->SetValue(output);
			}

		}
		
		if(videoEffectParameter->denoiseEnable)
		{
			if (paramOutput != nullptr)
			{
				cl_mem output = openclFilter.Denoise("Denoise", videoEffectParameter->uSigma, videoEffectParameter->uThreshold / 100.0f, videoEffectParameter->uKSigma, paramOutput->GetValue(), widthOut, heightOut);
				paramOutput->SetValue(output);
			}
		}
		
		if(videoEffectParameter->grayEnable)
		{
			if (paramOutput != nullptr)
			{
				cl_mem output = openclFilter.ColorEffect("GrayLevel", paramOutput->GetValue(), widthOut, heightOut);
				paramOutput->SetValue(output);
			}

		}

		if(videoEffectParameter->sepiaEnable)
		{
			if (paramOutput != nullptr)
			{
				cl_mem output = openclFilter.ColorEffect("Sepia", paramOutput->GetValue(), widthOut, heightOut);
				paramOutput->SetValue(output);
			}

		}
	}
}


bool COpenCLEffectVideo::IsOk()
{
	if(paramOutput != nullptr)
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