#include <header.h>
#include "OpenCLEffect.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include "RegardsBitmap.h"
#include "RegardsFloatBitmap.h"
#include <MotionBlur.h>
#include "OpenCLFilter.h"
#include "utility.h"
#include <ImageLoadingFormat.h>
#define minmax
#include <ximage.h>
//#include <RegardsConfigParam.h>
//#include <ParamInit.h>
#include "OpenCLBm3D.h"
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <OpenGL/OpenGL.h>
#elif defined(__WXGTK__)
#include <GL/glx.h>
#include <CL/cl_gl.h>
#else
#include <CL/cl_gl.h>
#endif
#include <DeepLearning.h>
using namespace Regards::OpenCL;
using namespace Regards::FiltreEffet;
using namespace Regards::DeepLearning;

#define NONE_FILTER 12

COpenCLEffect::COpenCLEffect(const CRgbaquad& backColor, COpenCLContext* context, CImageLoadingFormat* bitmap)
	: IFiltreEffet(backColor)
{
	//bool useMemory = (context->GetDeviceType() == CL_DEVICE_TYPE_GPU) ? false : true;
	this->backColor = backColor;
	//flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	flag = CL_MEM_COPY_HOST_PTR;
	this->context = context;
	dataIsOk = false;

	input = nullptr;
	paramWidth = nullptr;
	paramHeight = nullptr;

	paramOutput = nullptr;
	widthOut = 0;
	heightOut = 0;

	SetBitmap(bitmap);
}

COpenCLEffect::COpenCLEffect(const CRgbaquad& backColor, COpenCLContext* context, COpenCLParameterClMem* data_mem, const int& width, const int& height)
	: IFiltreEffet(backColor)
{
	//bool useMemory = (context->GetDeviceType() == CL_DEVICE_TYPE_GPU) ? false : true;
	this->backColor = backColor;
	//flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	flag = CL_MEM_COPY_HOST_PTR;
	this->context = context;
	dataIsOk = false;

	input = nullptr;
	paramWidth = nullptr;
	paramHeight = nullptr;

	paramOutput = nullptr;
	widthOut = 0;
	heightOut = 0;

	//Create Memory Buffer for Bitmap
	input = data_mem;
	this->width = width;
	this->height = height;
	paramWidth = new COpenCLParameterInt();
	paramWidth->SetValue(width);
	paramWidth->SetNoDelete(true);

	paramHeight = new COpenCLParameterInt();
	paramHeight->SetValue(height);
	paramHeight->SetNoDelete(true);
}

int COpenCLEffect::GetSizeData() const
{
	if (context->GetDefaultType() == OPENCL_FLOAT)
		return sizeof(cl_float) * 4;

	return sizeof(cl_uint);
}

CRegardsBitmap* COpenCLEffect::GetPtBitmap()
{
	return GetBitmap(false);
}

int COpenCLEffect::HQDn3D(const double& LumSpac, const double& ChromSpac, const double& LumTmp, const double& ChromTmp)
{
	if (context != nullptr)
	{
		cl_mem output;
		int _height;
		int _width;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.HQDn3D(LumSpac, ChromSpac, LumTmp, ChromTmp, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.HQDn3D(LumSpac, ChromSpac, LumTmp, ChromTmp, input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}
	return 0;
}

int COpenCLEffect::Bm3d(const int& fSigma)
{
	if (context != nullptr)
	{
		cl_mem y_picture;
		int _height;
		int _width;
		COpenCLFilter opencl_filter(context);
		//int size = 512;

		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			y_picture = opencl_filter.ConvertToY(paramOutput->GetValue(), _width, _height);
		}
		else
		{
			_width = width;
			_height = height;
			y_picture = opencl_filter.ConvertToY(input->GetValue(), _width, _height);
		}

		//int size = 512;

		COpenCLBm3D openclBm3D(context);
		openclBm3D.InitData(y_picture, _width * _height, _width, _height);
		openclBm3D.ExecuteFilter(fSigma);
		y_picture = openclBm3D.GetWienerImage();

		cl_mem output;
		if (preview && paramOutput != nullptr)
		{
			output = opencl_filter.InsertYValue(y_picture, paramOutput->GetValue(), _width, _height);
		}
		else
		{
			output = opencl_filter.InsertYValue(y_picture, input->GetValue(), _width, _height);
		}

		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::WaveFilter(int x, int y, short height, int scale, int radius)
{
	return -1;
}

cl_mem COpenCLEffect::LoadRegardsImage(uint8_t* data, const int& width, const int& height)
{
	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram* programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterByteArray* dataImage = new COpenCLParameterByteArray();
		dataImage->SetValue(context->GetContext(), data, width * height * 4, flag);
		vecParam.push_back(dataImage);

		COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
		program->SetKeepOutput(true);
		program->ExecuteProgram1D(programCL->GetProgram(), "LoadRegardsBitmap");
		outputValue = program->GetOutput();

		delete program;
		vecParam.clear();
	}
	return outputValue;
}


cl_mem COpenCLEffect::LoadFloatImage(float* data, const int& width, const int& height)
{
	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram* programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterFloatArray* dataImage = new COpenCLParameterFloatArray();
		dataImage->SetValue(context->GetContext(), data, width * height * 4, flag);
		vecParam.push_back(dataImage);

		COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
		program->SetKeepOutput(true);
		program->ExecuteProgram1D(programCL->GetProgram(), "LoadFloatBitmap");
		outputValue = program->GetOutput();

		delete program;
		vecParam.clear();
	}
	return outputValue;
}


void COpenCLEffect::SetBitmap(CImageLoadingFormat* bitmap)
{
	if (bitmap != nullptr && bitmap->IsOk())
	{
		cl_mem output_value;
		filename = bitmap->GetFilename();
		const int format = bitmap->GetFormat();
		if (format == TYPE_IMAGE_CXIMAGE)
		{
			CxImage* local = bitmap->GetCxImage(false);
			if (local->GetBpp() == 24)
			{
				if (local->AlphaIsValid())
					output_value = LoadCxImageAlpha(local->GetBits(), local->AlphaGetPointer(), bitmap->GetWidth(), bitmap->GetHeight(), local->GetEffWidth(), !local->IsBGR());
				else
					output_value = LoadCxImage(local->GetBits(), bitmap->GetWidth(), bitmap->GetHeight(), local->GetEffWidth(), !local->IsBGR());

				RefreshMemoryBitmap(output_value, bitmap->GetWidth(), bitmap->GetHeight());
				return;
			}
			else
			{
				CRegardsBitmap* regards_bitmap = bitmap->GetRegardsBitmap(false);
				output_value = LoadRegardsImage(regards_bitmap->GetPtBitmap(), bitmap->GetWidth(), bitmap->GetHeight());
				RefreshMemoryBitmap(output_value, bitmap->GetWidth(), bitmap->GetHeight());
				delete regards_bitmap;
			}
		}
		else if (format == TYPE_IMAGE_WXIMAGE)
		{
			const int eff_width = bitmap->GetWidth() * 3;
			wxImage* local = bitmap->GetwxImage(false);
			if (local->HasAlpha())
				output_value = LoadWxImageAlpha(local->GetData(), local->GetAlpha(), bitmap->GetWidth(), bitmap->GetHeight(), eff_width);
			else
				output_value = LoadWxImage(local->GetData(), bitmap->GetWidth(), bitmap->GetHeight(), eff_width);

			RefreshMemoryBitmap(output_value, bitmap->GetWidth(), bitmap->GetHeight());
			return;
		}
		else if (format == TYPE_IMAGE_REGARDSIMAGE)
		{
			CRegardsBitmap* local = bitmap->GetRegardsBitmap(false);
			if (context->GetDefaultType() == OPENCL_UCHAR)
			{
				COpenCLParameterUintArray uintValue(true);
				uintValue.SetNoDelete(true);
				uintValue.SetValue(context->GetContext(), (unsigned int*)local->GetPtBitmap(), bitmap->GetWidth() * bitmap->GetHeight(), flag);
				output_value = uintValue.GetValue();
			}
			else
			{
				output_value = LoadRegardsImage(local->GetPtBitmap(), bitmap->GetWidth(), bitmap->GetHeight());
			}
			RefreshMemoryBitmap(output_value, bitmap->GetWidth(), bitmap->GetHeight());
		}
		else if (format == TYPE_IMAGE_REGARDSFLOATIMAGE)
		{
			CRegardsFloatBitmap* local = bitmap->GetFloatBitmap(false);
			if (context->GetDefaultType() == OPENCL_FLOAT)
			{
				COpenCLParameterFloatArray floatValue(true);
				floatValue.SetNoDelete(true);
				floatValue.SetValue(context->GetContext(), local->GetData(), local->GetWidth() * 4 * local->GetHeight(), flag);
				output_value = floatValue.GetValue();
			}
			else
			{
				output_value = LoadFloatImage(local->GetData(), local->GetWidth(), local->GetHeight());
			}
			RefreshMemoryBitmap(output_value, bitmap->GetWidth(), bitmap->GetHeight());
		}
	}

}


cl_mem COpenCLEffect::LoadCxImageAlpha(uint8_t* data, uint8_t* alpha, const int& width, const int& height, const int& effwidth, const bool& RGB)
{
	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram* programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);
		//program->SetBuildOptions("-D ITER_NUM=1000");

		//Create Memory Buffer for Bitmap
		COpenCLParameterByteArray* dataImage = new COpenCLParameterByteArray();
		dataImage->SetValue(context->GetContext(), data, effwidth * height, flag);
		vecParam.push_back(dataImage);

		COpenCLParameterByteArray* alphaImage = new COpenCLParameterByteArray();
		alphaImage->SetValue(context->GetContext(), alpha, width * height, flag);
		vecParam.push_back(alphaImage);

		COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		COpenCLParameterInt* parameffwidth = new COpenCLParameterInt();
		parameffwidth->SetLibelle("effwidth");
		parameffwidth->SetValue(effwidth);
		vecParam.push_back(parameffwidth);

		program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
		program->SetKeepOutput(true);
		if (RGB)
			program->ExecuteProgram1D(programCL->GetProgram(), "LoadCxImageAlphaRGB");
		else
			program->ExecuteProgram1D(programCL->GetProgram(), "LoadCxImageAlpha");
		outputValue = program->GetOutput();

		delete program;
		vecParam.clear();

		//SetOutputValue(outputValue,width,height);
	}
	return outputValue;
}

cl_mem COpenCLEffect::LoadCxImage(uint8_t* data, const int& width, const int& height, const int& effwidth, const bool& RGB)
{
	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram* programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);
		//program->SetBuildOptions("-D ITER_NUM=1000");

		//Create Memory Buffer for Bitmap
		COpenCLParameterByteArray* dataImage = new COpenCLParameterByteArray();
		dataImage->SetValue(context->GetContext(), data, effwidth * height, flag);
		vecParam.push_back(dataImage);

		COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		COpenCLParameterInt* parameffwidth = new COpenCLParameterInt();
		parameffwidth->SetLibelle("effwidth");
		parameffwidth->SetValue(effwidth);
		vecParam.push_back(parameffwidth);

		program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
		program->SetKeepOutput(true);
		if (RGB)
			program->ExecuteProgram1D(programCL->GetProgram(), "LoadCxImageRGB");
		else
			program->ExecuteProgram1D(programCL->GetProgram(), "LoadCxImage");
		outputValue = program->GetOutput();

		delete program;
		vecParam.clear();

		//SetOutputValue(outputValue,width,height);
	}
	return outputValue;
}

cl_mem COpenCLEffect::LoadWxImageAlpha(uint8_t* data, uint8_t* alpha, const int& width, const int& height, const int& effwidth)
{
	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram* programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);
		//program->SetBuildOptions("-D ITER_NUM=1000");

		//Create Memory Buffer for Bitmap
		COpenCLParameterByteArray* dataImage = new COpenCLParameterByteArray();
		dataImage->SetValue(context->GetContext(), data, effwidth * height, flag);
		vecParam.push_back(dataImage);

		COpenCLParameterByteArray* alphaImage = new COpenCLParameterByteArray();
		alphaImage->SetValue(context->GetContext(), alpha, width * height, flag);
		vecParam.push_back(alphaImage);

		COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		COpenCLParameterInt* parameffwidth = new COpenCLParameterInt();
		parameffwidth->SetLibelle("effwidth");
		parameffwidth->SetValue(effwidth);
		vecParam.push_back(parameffwidth);

		program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
		program->SetKeepOutput(true);
		program->ExecuteProgram1D(programCL->GetProgram(), "LoadWxImageAlpha");
		outputValue = program->GetOutput();

		delete program;
		vecParam.clear();

		//SetOutputValue(outputValue,width,height);
	}
	return outputValue;
}

cl_mem COpenCLEffect::LoadWxImage(uint8_t* data, const int& width, const int& height, const int& effwidth)
{
	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram* programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);
		//program->SetBuildOptions("-D ITER_NUM=1000");

		//Create Memory Buffer for Bitmap
		COpenCLParameterByteArray* dataImage = new COpenCLParameterByteArray();
		dataImage->SetValue(context->GetContext(), data, effwidth * height, flag);
		vecParam.push_back(dataImage);

		COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		COpenCLParameterInt* parameffwidth = new COpenCLParameterInt();
		parameffwidth->SetLibelle("effwidth");
		parameffwidth->SetValue(effwidth);
		vecParam.push_back(parameffwidth);

		program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
		program->SetKeepOutput(true);
		program->ExecuteProgram1D(programCL->GetProgram(), "LoadWxImage");
		outputValue = program->GetOutput();

		delete program;
		vecParam.clear();
	}
	return outputValue;
}

COpenCLEffect::~COpenCLEffect()
{
	if (paramOutput != nullptr)
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

	if (input != nullptr)
	{
		input->Release();
		delete input;
	}
}


void COpenCLEffect::RefreshMemoryBitmap(cl_mem bitmapOut, const int& widthOutput, const int& heightOutput)
{
	width = widthOutput;
	height = heightOutput;

	if (paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
		paramOutput = nullptr;
	}

	if (paramWidth != nullptr)
	{
		paramWidth->Release();
		delete paramWidth;
		paramWidth = nullptr;
	}

	if (paramHeight != nullptr)
	{
		paramHeight->Release();
		delete paramHeight;
		paramHeight = nullptr;
	}

	if (input != nullptr)
	{
		input->Release();
		delete input;
		input = nullptr;
	}

	//Create Memory Buffer for Bitmap
	input = new COpenCLParameterClMem();
	COpenCLParameterClMem* valueInput = (COpenCLParameterClMem*)input;
	valueInput->SetValue(bitmapOut);
	valueInput->SetNoDelete(true);

	paramWidth = new COpenCLParameterInt();
	paramWidth->SetValue(width);
	paramWidth->SetNoDelete(true);

	paramHeight = new COpenCLParameterInt();
	paramHeight->SetValue(height);
	paramHeight->SetNoDelete(true);


}


void COpenCLEffect::RefreshMemoryBitmap(CRegardsBitmap* bitmapOut)
{
	if (bitmapOut != nullptr)
	{
		cl_mem outputValue = LoadRegardsImage(bitmapOut->GetPtBitmap(), bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight());
		RefreshMemoryBitmap(outputValue, bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight());
	}
}

void COpenCLEffect::GetBitmap(CRegardsBitmap*& bitmap, cl_mem input, const int& width, const int& height)
{
	if (input != nullptr)
	{
		COpenCLFilter openclFilter(context);
		COpenCLProgram* programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
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
			vecParam.clear();
		}
	}
}

CRegardsBitmap* COpenCLEffect::GetBitmap(cl_mem input, const int& width, const int& height)
{
	CRegardsBitmap* bitmap = nullptr;

	if (input != nullptr)
	{
		bitmap = new CRegardsBitmap(width, height);
		COpenCLFilter openclFilter(context);
		COpenCLProgram* programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
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
			vecParam.clear();
		}
	}
	return bitmap;
}


CRegardsFloatBitmap* COpenCLEffect::GetFloatBitmap(cl_mem input, const int& width, const int& height)
{
	CRegardsFloatBitmap* bitmap = nullptr;

	if (input != nullptr)
	{
		bitmap = new CRegardsFloatBitmap(width, height);
		COpenCLFilter openclFilter(context);
		COpenCLProgram* programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
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
			paramWidth->SetValue(bitmap->GetWidth());
			vecParam.push_back(paramWidth);

			COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
			paramHeight->SetLibelle("height");
			paramHeight->SetValue(bitmap->GetHeight());
			vecParam.push_back(paramHeight);

			program->SetParameter(&vecParam, bitmap);
			program->ExecuteProgram1D(programCL->GetProgram(), "GetFloatBitmap");

			delete program;
			vecParam.clear();
		}
	}
	return bitmap;
}

CRegardsFloatBitmap* COpenCLEffect::GetFloatBitmap(const bool& source)
{
	CRegardsFloatBitmap* bitmapOut = nullptr;
	if (context->GetDefaultType() == OPENCL_FLOAT)
	{
		if (paramOutput != nullptr && !source)
		{
			bitmapOut = new CRegardsFloatBitmap(widthOut, heightOut);
			if (context != nullptr)
			{
				context->GetOutputData(paramOutput->GetValue(), bitmapOut->GetData(), bitmapOut->GetSize(), flag);  // NOLINT(clang-diagnostic-shorten-64-to-32)
			}
		}
		else
		{
			bitmapOut = new CRegardsFloatBitmap(width, height);
			if (context != nullptr)
			{
				context->GetOutputData(input->GetValue(), bitmapOut->GetData(), bitmapOut->GetSize(), flag);
			}
		}
	}
	else
	{
		if (paramOutput != nullptr && !source)
		{
			if (context != nullptr)
			{
				bitmapOut = GetFloatBitmap(paramOutput->GetValue(), widthOut, heightOut);
			}
		}
		else
		{
			if (context != nullptr)
			{
				bitmapOut = GetFloatBitmap(input->GetValue(), width, height);
			}
		}
	}
	return bitmapOut;
}

void COpenCLEffect::GetBitmap(CRegardsBitmap*& bitmap, const bool& source)
{
	if (bitmap == nullptr)
		return;

	if (context->GetDefaultType() == OPENCL_UCHAR)
	{
		if (paramOutput != nullptr && !source)
		{
			if (context != nullptr)
			{
				context->GetOutputData(paramOutput->GetValue(), bitmap->GetPtBitmap(), bitmap->GetBitmapSize(), flag);
			}
		}
		else
		{
			if (context != nullptr)
			{
				context->GetOutputData(input->GetValue(), bitmap->GetPtBitmap(), bitmap->GetBitmapSize(), flag);
			}
		}
	}
	else
	{

		if (paramOutput != nullptr && !source)
		{
			if (context != nullptr)
			{
				GetBitmap(bitmap, paramOutput->GetValue(), widthOut, heightOut);
			}
		}
		else
		{
			if (context != nullptr)
			{
				GetBitmap(bitmap, input->GetValue(), width, height);
			}
		}
	}
	if (bitmap != nullptr)
		bitmap->SetFilename(filename);
}

void COpenCLEffect::CopyPictureToTexture2D(void* cl_image)
{
	cl_mem cl_picture = (cl_mem)cl_image;
	if (context != nullptr)
	{
		try
		{
			cl_command_queue q = context->GetCommandQueue();
			cl_int err;
			err = clEnqueueAcquireGLObjects(q, 1, &cl_picture, 0, nullptr, nullptr);
			Error::CheckError(err);
			GetRgbaBitmap(cl_picture);
			err = clEnqueueReleaseGLObjects(q, 1, &cl_picture, 0, nullptr, nullptr);
			Error::CheckError(err);
			err = clFlush(q);
			Error::CheckError(err);
		}
		catch (...)
		{

		}
	}
}


void COpenCLEffect::GetYUV420P(uint8_t*& y, uint8_t*& u, uint8_t*& v, const int& widthOut, const int& heightOut)
{
	int middleWidth = widthOut / 2;
	int middleHeight = heightOut / 2;
	bool isnotdelete = false;
	COpenCLParameterByteArray* inputY = new COpenCLParameterByteArray();
	inputY->SetLibelle("inputY");
	inputY->SetNoDelete(true);
	inputY->SetValue(context->GetContext(), y, widthOut * heightOut, flag);

	COpenCLParameterByteArray* inputU = new COpenCLParameterByteArray();
	inputU->SetLibelle("inputU");
	inputU->SetNoDelete(true);
	inputU->SetValue(context->GetContext(), u, middleWidth * middleHeight, flag);

	COpenCLParameterByteArray* inputV = new COpenCLParameterByteArray();
	inputV->SetNoDelete(true);
	inputV->SetLibelle("inputV");
	inputV->SetValue(context->GetContext(), v, middleWidth * middleHeight, flag);

	COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
	paramWidth->SetNoDelete(true);
	paramWidth->SetLibelle("widthIn");
	paramWidth->SetValue(widthOut);

	COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
	paramHeight->SetNoDelete(true);
	paramHeight->SetLibelle("heightIn");
	paramHeight->SetValue(heightOut);

	//paramOutput->SetNoDelete(true);

	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		COpenCLProgram* programCL = openclFilter.GetProgram("IDR_OPENCL_CONVERTTOY");

		if (programCL != nullptr)
		{
			vector<COpenCLParameter*> vecParam;
			COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

			vecParam.push_back(inputY);
			vecParam.push_back(inputU);
			vecParam.push_back(inputV);

			if (preview && paramOutput != nullptr)
			{
				isnotdelete = paramOutput->GetNoDelete();
				paramOutput->SetNoDelete(true);
				vecParam.push_back(paramOutput);
			}
			else
			{
				isnotdelete = input->GetNoDelete();
				input->SetNoDelete(true);
				vecParam.push_back(input);
			}
			vecParam.push_back(paramWidth);
			vecParam.push_back(paramHeight);

			//program->SetParameter(&vecParam, middleWidth, middleHeight, middleWidth * middleHeight * GetSizeData());
			//program->SetKeepOutput(true);
			program->ExecuteProgram2D(programCL->GetProgram(), "RgbaToYUV420P", &vecParam, middleWidth, middleHeight);

			delete program;
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

	if (preview && paramOutput != nullptr)
	{
		paramOutput->SetNoDelete(isnotdelete);
	}
	else
	{
		input->SetNoDelete(isnotdelete);
	}
}

CRegardsBitmap* COpenCLEffect::GetBitmap(const bool& source)
{
	CRegardsBitmap* bitmapOut = nullptr;
	if (context->GetDefaultType() == OPENCL_UCHAR)
	{
		if (paramOutput != nullptr && !source)
		{
			bitmapOut = new CRegardsBitmap(widthOut, heightOut);
			if (context != nullptr)
			{
				context->GetOutputData(paramOutput->GetValue(), bitmapOut->GetPtBitmap(), bitmapOut->GetBitmapSize(), flag);
			}
		}
		else
		{
			bitmapOut = new CRegardsBitmap(width, height);
			if (context != nullptr)
			{
				context->GetOutputData(input->GetValue(), bitmapOut->GetPtBitmap(), bitmapOut->GetBitmapSize(), flag);
			}
		}
	}
	else
	{

		if (paramOutput != nullptr && !source)
		{
			if (context != nullptr)
			{
				bitmapOut = GetBitmap(paramOutput->GetValue(), widthOut, heightOut);
			}
		}
		else
		{
			if (context != nullptr)
			{
				bitmapOut = GetBitmap(input->GetValue(), width, height);
			}
		}
	}
	if (bitmapOut != nullptr)
		bitmapOut->SetFilename(filename);
	return bitmapOut;

}
//-----------------------------------------------------------------------------------------------
//Get Output
//-----------------------------------------------------------------------------------------------

wxImage COpenCLEffect::GetwxImage()
{
	wxImage anImage(widthOut, heightOut, false);

	if (paramOutput != nullptr)
	{
		COpenCLFilter openclFilter(context);
		COpenCLProgram* programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter*> vecParam;
			COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);
			//program->SetBuildOptions("-D ITER_NUM=1000");

			paramOutput->SetLibelle("input");
			vecParam.push_back(paramOutput);

			COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
			paramWidth->SetLibelle("widthIn");
			paramWidth->SetValue(widthOut);
			vecParam.push_back(paramWidth);

			COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
			paramHeight->SetLibelle("heightIn");
			paramHeight->SetValue(heightOut);
			vecParam.push_back(paramHeight);

			program->SetParameter(&vecParam, &anImage);
			program->ExecuteProgram1D(programCL->GetProgram(), "BitmapToWxImage");

			delete program;
			vecParam.clear();

			paramOutput = nullptr;
		}
	}
	return anImage;
}


int COpenCLEffect::GetRgbaBitmap(void* cl_image)
{
	COpenCLFilter openclFilter(context);
	COpenCLProgram* programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

		/*

		*/

		if (paramOutput != nullptr)
		{
			paramOutput->SetLibelle("input");
			paramOutput->SetNoDelete(true);
			vecParam.push_back(paramOutput);

			COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(widthOut);
			paramWidth->SetLibelle("width");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(heightOut);
			paramHeight->SetLibelle("height");
			vecParam.push_back(paramHeight);
		}
		else
		{
			input->SetLibelle("input");
			input->SetNoDelete(true);
			vecParam.push_back(input);

			COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(width);
			paramWidth->SetLibelle("width");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(height);
			paramHeight->SetLibelle("height");
			vecParam.push_back(paramHeight);
		}


		COpenCLParameterInt* paramRGBA = new COpenCLParameterInt();

#if defined(__x86_64__) || defined(_M_AMD64)	
		paramRGBA->SetValue(0);
#elif defined(__APPLE__)
		paramRGBA->SetValue(1);
#endif 
		paramRGBA->SetLibelle("rgba");
		vecParam.push_back(paramRGBA);

		program->SetKeepOutput(true);
		if (paramOutput != nullptr)
			program->SetParameter(&vecParam, widthOut, heightOut, (cl_mem)cl_image);
		else
			program->SetParameter(&vecParam, width, height, (cl_mem)cl_image);
		program->ExecuteProgram(programCL->GetProgram(), "BitmapToOpenGLTexture");

		delete program;
		vecParam.clear();
	}

	return 0;
}


void COpenCLEffect::SetOutputValue(cl_mem output, int widthOutput, int heightOutput)
{
	if (output != nullptr)
	{
		if (preview)
		{
			if (paramOutput != nullptr)
			{
				paramOutput->Release();
				delete paramOutput;
			}

			paramOutput = new COpenCLParameterClMem();
			paramOutput->SetValue(output);
			this->widthOut = widthOutput;
			this->heightOut = heightOutput;
		}
		else
		{
			RefreshMemoryBitmap(output, widthOutput, heightOutput);
		}
	}
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

int COpenCLEffect::BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP)
{
	CDeepLearning::LockOpenCLDnn();
	
	if (context != nullptr)
	{
		int _height;
		int _width;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.BilateralEffect(paramOutput->GetValue(), widthOut, heightOut, fSize, sigmaX, sigmaP);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.BilateralEffect(input->GetValue(), width, height, fSize, sigmaX, sigmaP);
		}

		SetOutputValue(output, _width, _height);
	}

	CDeepLearning::UnlockOpenCLDnn();
	return 0;
}

int COpenCLEffect::BrightnessAndContrast(const double& brightness, const double& contrast)
{
	if (context != nullptr)
	{
		cl_mem output;
		int _height;
		int _width;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.BrightnessAndContrast(brightness, contrast, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.BrightnessAndContrast(brightness, contrast, input->GetValue(), width, height);
		}

		SetOutputValue(output, _width, _height);
	}
	return 0;
}

int COpenCLEffect::Swirl(const float& radius, const float& angle)
{
	if (context != nullptr)
	{
		cl_mem output;
		int _height;
		int _width;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Swirl(radius, angle, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Swirl(radius, angle, input->GetValue(), width, height);
		}

		SetOutputValue(output, _width, _height);
	}
	return 0;
}

int COpenCLEffect::NlmeansFilter(const int& h, const int& templateWindowSize, const int& searchWindowSize)
{
	CDeepLearning::LockOpenCLDnn();
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.NlMeans(paramOutput->GetValue(), widthOut, heightOut, h, templateWindowSize, searchWindowSize);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.NlMeans(input->GetValue(), width, height, h, templateWindowSize, searchWindowSize);
		}
		SetOutputValue(output, _width, _height);
	}
	CDeepLearning::UnlockOpenCLDnn();
	return 0;
}

int COpenCLEffect::Posterize(const float& level, const float& gamma)
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Posterize(level, gamma, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Posterize(level, gamma, input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}
	return 0;
}

int COpenCLEffect::MotionBlur(const double& radius, const double& sigma, const double& angle)
{
	//CRegardsBitmap * bitmapOut = new CRegardsBitmap(width, height);
	if (context != nullptr)
	{
		vector<double> kernel;
		vector<wxPoint> offsets;

		if (sigma == 0.0)
			return 0;

		kernel = CMotionBlur::GetMotionBlurKernel(radius, sigma);

		if (kernel.size() < 3)
			return false;

		if (context != nullptr)
		{
			int _width;
			int _height;
			cl_mem output;
			offsets = CMotionBlur::GetOffsetKernel(kernel.size(), angle);
			COpenCLFilter openclFilter(context);
			if (preview && paramOutput != nullptr)
			{
				_width = widthOut;
				_height = heightOut;
				output = openclFilter.MotionBlurCompute(kernel, offsets, kernel.size(), paramOutput->GetValue(), widthOut, heightOut);
			}
			else
			{
				_width = width;
				_height = height;
				output = openclFilter.MotionBlurCompute(kernel, offsets, kernel.size(), input->GetValue(), width, height);
			}
			SetOutputValue(output, _width, _height);
		}
	}
	//delete bitmapOut;
	return 0;
}


int COpenCLEffect::Median()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Median(paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Median(input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}
	return 0;
}

int COpenCLEffect::Negatif()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.ColorEffect("Negatif", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.ColorEffect("Negatif", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::BrightnessAndContrastAuto(float clipHistPercent)
{
	CDeepLearning::LockOpenCLDnn();
    try
    {
        if (context != nullptr)
        {
			int _width;
			int _height;
			cl_mem output;
            COpenCLFilter openclFilter(context);
            if (preview && paramOutput != nullptr)
            {
                _width = widthOut;
                _height = heightOut;
                output = openclFilter.BrightnessAndContrastAuto(paramOutput->GetValue(), widthOut, heightOut, clipHistPercent);
            }
            else
            {
                _width = width;
                _height = height;
                output = openclFilter.BrightnessAndContrastAuto(input->GetValue(), width, height, clipHistPercent);
            }
            SetOutputValue(output, _width, _height);
        }
    }
    catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
	CDeepLearning::UnlockOpenCLDnn();
	return 0;
}




int COpenCLEffect::GetWidth()
{
	if (preview)
	{
		return widthOut;
	}
	else
	{
		return width;
	}
}

int COpenCLEffect::GetHeight()
{
	if (preview && paramOutput != nullptr)
	{
		return heightOut;
	}
	else
	{
		return height;
	}
}

int COpenCLEffect::Sepia()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.ColorEffect("Sepia", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.ColorEffect("Sepia", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::NoirEtBlanc()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.ColorEffect("NoirEtBlanc", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.ColorEffect("NoirEtBlanc", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::NiveauDeGris()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.ColorEffect("GrayLevel", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.ColorEffect("GrayLevel", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::FlipVertical()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Flip("FlipVertical", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Flip("FlipVertical", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::FlipHorizontal()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter opencl_filter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = opencl_filter.Flip("FlipHorizontal", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = opencl_filter.Flip("FlipHorizontal", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::Rotate90()
{
	if (context != nullptr)
	{
		int _widthOut;
		int _heightOut;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_widthOut = heightOut;
			_heightOut = widthOut;
			output = openclFilter.Rotate("Rotation90", _widthOut, _heightOut, 90.0f, paramOutput->GetValue(), widthOut,
			                             heightOut);
		}
		else
		{
			_widthOut = height;
			_heightOut = width;
			output = openclFilter.Rotate("Rotation90", _widthOut, _heightOut, 90.0f, input->GetValue(), width, height);
		}
		SetOutputValue(output, _widthOut, _heightOut);
	}

	return 0;
}

int COpenCLEffect::Rotate270()
{
	if (context != nullptr)
	{
		int _widthOut;
		int _heightOut;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_widthOut = heightOut;
			_heightOut = widthOut;
			output = openclFilter.Rotate("Rotation270", _widthOut, _heightOut, 270.0f, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_widthOut = height;
			_heightOut = width;
			output = openclFilter.Rotate("Rotation270", _widthOut, _heightOut, 270.0f, input->GetValue(), width, height);
		}
		SetOutputValue(output, _widthOut, _heightOut);
	}
	return 0;
}


int COpenCLEffect::RotateFree(const double& angle, const int& widthOut, const int& heightOut)
{
	if (context != nullptr)
	{
		int _widthOut;
		int _heightOut;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_widthOut = widthOut;
			_heightOut = heightOut;
			output = openclFilter.Rotate("RotateFree", widthOut, heightOut, angle, paramOutput->GetValue(), this->widthOut, this->heightOut);
		}
		else
		{
			_widthOut = widthOut;
			_heightOut = heightOut;
			output = openclFilter.Rotate("RotateFree", widthOut, heightOut, angle, input->GetValue(), width, height);
		}
		SetOutputValue(output, _widthOut, _heightOut);
	}
	return 0;
}

int COpenCLEffect::SharpenMasking(const float& sharpness)
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.SharpenMasking(sharpness, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.SharpenMasking(sharpness, input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::PhotoFiltre(const CRgbaquad& clValue, const int& intensity)
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.PhotoFiltre(clValue, intensity, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.PhotoFiltre(clValue, intensity, input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::Solarize(const long& threshold)
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Solarize(threshold, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Solarize(threshold, input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::RGBFilter(const int& red, const int& green, const int& blue)
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.RGBFilter(red, green, blue, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.RGBFilter(red, green, blue, input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::FiltreMosaic()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.FiltreMosaic(paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.FiltreMosaic(input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int COpenCLEffect::Fusion(CRegardsBitmap* bitmapSecond, const float& pourcentage)
{
	const cl_mem input_second = LoadRegardsImage(bitmapSecond->GetPtBitmap(), bitmapSecond->GetBitmapWidth(),
	                                             bitmapSecond->GetBitmapHeight());
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Fusion(paramOutput->GetValue(), input_second, pourcentage, widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Fusion(input->GetValue(), input_second, pourcentage, width, height);
		}
		SetOutputValue(output, _width, _height);
	}
	return 0;
}

int COpenCLEffect::Soften()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.FiltreConvolution("IDR_OPENCL_SOFTEN", "Soften", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.FiltreConvolution("IDR_OPENCL_SOFTEN", "Soften", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::Noise()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Noise(paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Noise(input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::Blur(const int& radius)
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Blur(radius, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Blur(radius, input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::Emboss()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.FiltreConvolution("IDR_OPENCL_EMBOSS", "Emboss", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.FiltreConvolution("IDR_OPENCL_EMBOSS", "Emboss", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}


int COpenCLEffect::SharpenStrong()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.FiltreConvolution("IDR_OPENCL_SHARPENSTRONG", "SharpenStrong", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.FiltreConvolution("IDR_OPENCL_SHARPENSTRONG", "SharpenStrong", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::Sharpen()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.FiltreConvolution("IDR_OPENCL_SHARPEN", "Sharpen", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.FiltreConvolution("IDR_OPENCL_SHARPEN", "Sharpen", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::FiltreEdge()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.FiltreConvolution("IDR_OPENCL_EDGE", "Edge", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.FiltreConvolution("IDR_OPENCL_EDGE", "Edge", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}

	return 0;
}

int COpenCLEffect::Erode()
{
	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter openclFilter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.ErodeDilate("Erode", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.ErodeDilate("Erode", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}
	return 0;
}

int COpenCLEffect::Dilate()
{

	if (context != nullptr)
	{
		int _width;
		int _height;
		cl_mem output;
		COpenCLFilter opencl_filter(context);
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = opencl_filter.ErodeDilate("Dilate", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = opencl_filter.ErodeDilate("Dilate", input->GetValue(), width, height);
		}
		SetOutputValue(output, _width, _height);
	}
	return 0;
}

int COpenCLEffect::GaussianBlur(const int& r, const int& boxSize)
{
	//CRegardsBitmap * bitmapOut = new CRegardsBitmap(width, height);
	if (context != nullptr)
	{
		if (boxSize > 0)
		{
			const double wIdeal = sqrt((double)(12 * r * r / boxSize + 1));
			int wl = static_cast<int>(floor(wIdeal));
			if (wl % 2 == 0)
				wl--;
			int wu = wl + 2;
			const double mIdeal = (12 * r * r - boxSize * wl * wl - 4 * boxSize * wl - 3 * boxSize) / (-4 * wl - 4);
			int m = round(mIdeal);
			vector<int> gaussCoeff;
			gaussCoeff.reserve(boxSize);
			for (auto i = 0; i < boxSize; i++)
				gaussCoeff.push_back(i < m ? wl : wu);

			bool noDeleteData = true;
			int _width = 0;
			int _height = 0;
			cl_mem output = nullptr;

			COpenCLFilter opencl_filter(context);
			if (paramOutput != nullptr)
			{
				_width = widthOut;
				_height = heightOut;
			}
			else
			{
				_width = width;
				_height = height;
			}
			if (preview && paramOutput != nullptr)
				output = paramOutput->GetValue();
			else
				output = input->GetValue();

			output = opencl_filter.BoxBlur((gaussCoeff[0] - 1) / 2, "BoxBlurH", output, _width, _height, noDeleteData);
			noDeleteData = false;
			output = opencl_filter.BoxBlur((gaussCoeff[0] - 1) / 2, "BoxBlurV", output, _width, _height, noDeleteData);

			for (auto i = 1; i < boxSize; i++)
			{
				output = opencl_filter.BoxBlur((gaussCoeff[i] - 1) / 2, "BoxBlurH", output, _width, _height, noDeleteData);
				output = opencl_filter.BoxBlur((gaussCoeff[i] - 1) / 2, "BoxBlurV", output, _width, _height, noDeleteData);
			}

			SetOutputValue(output, _width, _height);

		}
	}
	return 0;
}

//-----------------------------------------------------------------------------------------------
//Execute INTERPOLATION
//-----------------------------------------------------------------------------------------------

void COpenCLEffect::Interpolation(const int& widthOut, const int& heightOut, const int& method, int flipH, int flipV, int angle)
{
	if (paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
	}

	cl_mem output;

	try
	{
		COpenCLFilter openclFilter(context);
		output = openclFilter.Interpolation(widthOut, heightOut, "Interpolation", method, input->GetValue(), width, height, flipH, flipV, angle);
	}
	catch (...)
	{
		output = nullptr;
	}

	if (output != nullptr)
	{
		paramOutput = new COpenCLParameterClMem();
		paramOutput->SetValue(output);
		this->widthOut = widthOut;
		this->heightOut = heightOut;
	}
}

void COpenCLEffect::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method, int flipH, int flipV, int angle)
{
	if (paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
	}

	cl_mem output;

	try
	{
		COpenCLFilter openclFilter(context);
		output = openclFilter.Interpolation(widthOut, heightOut, rc, "InterpolationZone", method, input->GetValue(), width, height, flipH, flipV, angle);
	}
	catch (...)
	{
		output = nullptr;
	}

	paramOutput = new COpenCLParameterClMem();
	paramOutput->SetValue(output);
	this->widthOut = widthOut;
	this->heightOut = heightOut;


}