#include <header.h>
#include "OpenCLEffectVideo.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include <RegardsBitmap.h>
#include "utility.h"
#include "EffectVideoParameter.h"
#include "OpenCLFilter.h"
#include "hqdn3d.h"
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

COpenCLParameterClMem * COpenCLEffectVideo::GetPtData()
{
	return paramSrc;
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

			//program->SetParameter(&vecParam, middleWidth, middleHeight, middleWidth * middleHeight * GetSizeData());
			//program->SetKeepOutput(true);
			program->ExecuteProgram2D(programCL->GetProgram(), "RgbaToYUV420P", &vecParam, middleWidth, middleHeight);

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
		((COpenCLParameterByteArray *)memDataOut)->SetLibelle("input");
		((COpenCLParameterByteArray *)memDataOut)->SetValue(context->GetContext(), (uint8_t *)dataOut, size, flag);


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