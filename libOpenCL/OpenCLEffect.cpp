#include "OpenCLEffect.h"
#include "OpenCLEngine.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include "RegardsBitmap.h"
#include "RegardsFloatBitmap.h"
#include <LoadingResource.h>
#include <MotionBlur.h>
#include "OpenCLFilter.h"
#include "utility.h"
#include <ImageLoadingFormat.h>
#include <ximage.h>
#include "OpenCLBm3D.h"
using namespace Regards::OpenCL;
using namespace Regards::FiltreEffet;

COpenCLEffect::COpenCLEffect(const CRgbaquad &backColor, COpenCLContext * context, CImageLoadingFormat * bitmap)
	: IFiltreEffet(backColor)
{
	bool useMemory = (context->GetDeviceType() == CL_DEVICE_TYPE_GPU) ? false : true;
	this->backColor = backColor;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
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

int COpenCLEffect::Bm3d(const int & fSigma)
{
	int _width = 256;
	int _height = 256;
	
	cl_mem yPicture = nullptr;
	
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		int size = 512;
/*
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.bilat2(fSize,sigmaX,sigmaP, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.bilat2(fSize,sigmaX,sigmaP, input->GetValue(), width, height);
		}		
        */
        
		if (preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
            yPicture = openclFilter.ConvertToY(paramOutput->GetValue(), _width, _height);
		}
		else
		{
			_width = width;
			_height = height;
			yPicture = openclFilter.ConvertToY(input->GetValue(), _width, _height);
		}


		

		
		//int size = 512;
		
		COpenCLBm3D openclBm3D(context);
		openclBm3D.InitData(yPicture, _width * _height, _width, _height);
		openclBm3D.ExecuteFilter(fSigma);
		yPicture = openclBm3D.GetWienerImage();
		
		
		/*
		int nbPictureX = _width / size;
		int nbPictureY = _height / size;
		if (nbPictureX * size < _width)
			nbPictureX++;
		if (nbPictureY * size < _height)
			nbPictureY++;

		for (int y = 0; y < nbPictureY; y++)
		{
			for (int x = 0; x < nbPictureX; x++)
			{		

				int marge = 16;			
				int sizeTraitement = size + marge * 2;
				cl_mem wiener = nullptr;
				cl_mem block_size = nullptr;

				COpenCLBm3D openclBm3D(context);
				block_size = openclFilter.ExtractBlocSize(yPicture, size, marge, _width, _height, x, y);
				openclBm3D.InitData(block_size, sizeTraitement * sizeTraitement, sizeTraitement, sizeTraitement);
				openclBm3D.ExecuteFilter(fSigma);
				wiener = openclBm3D.GetWienerImage();
				openclFilter.InsertBlockSize(yPicture, wiener, size, marge, _width, _height, x, y);
			}
		}
		*/

		cl_mem output = nullptr;
		if (preview && paramOutput != nullptr)
		{
			output = openclFilter.InsertYValue(yPicture, paramOutput->GetValue(), _width, _height);
		}
		else
		{
			output = openclFilter.InsertYValue(yPicture, input->GetValue(), _width, _height);
		}

		SetOutputValue(output, _width, _height);
	}
	return 0;  
}

int COpenCLEffect::WaveFilter(int x, int y, short height, int scale, int radius)
{
	return -1;
}

cl_mem COpenCLEffect::LoadRegardsImage(uint8_t * data, const int &width, const int &height)
{
	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_REGARDBITMAP");
	if(programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterByteArray *	dataImage = new COpenCLParameterByteArray();
		dataImage->SetValue(context->GetContext(), data, width * height * 4, flag);
		vecParam.push_back(dataImage);

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		program->SetParameter(&vecParam,  width, height, sizeof(float) * 4 * width * height);
		program->SetKeepOutput(true);
		program->ExecuteProgram1D(programCL->GetProgram(), "LoadRegardsBitmap");
		outputValue = program->GetOutput();

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
	return outputValue;
}


void COpenCLEffect::SetBitmap(CImageLoadingFormat * bitmap)
{
	cl_mem outputValue = nullptr;
	if(bitmap != nullptr && bitmap->IsOk())
	{
		int format = bitmap->GetFormat();
		if(format == TYPE_IMAGE_CXIMAGE)
		{
			CxImage * local = bitmap->GetCxImage(false);
			if(local->GetBpp() == 24)
			{
				if(local->AlphaIsValid())
					outputValue = LoadCxImageAlpha(local->GetBits(), local->AlphaGetPointer(), bitmap->GetWidth(), bitmap->GetHeight(), local->GetEffWidth(), !local->IsBGR());
				else
					outputValue = LoadCxImage(local->GetBits(), bitmap->GetWidth(), bitmap->GetHeight(), local->GetEffWidth(), !local->IsBGR());

				RefreshMemoryBitmap(outputValue, bitmap->GetWidth(), bitmap->GetHeight());
				return;
			}
			else
			{
				CRegardsBitmap * local = bitmap->GetRegardsBitmap(false);
				outputValue = LoadRegardsImage(local->GetPtBitmap(), bitmap->GetWidth(), bitmap->GetHeight());
				RefreshMemoryBitmap(outputValue, bitmap->GetWidth(), bitmap->GetHeight());
				delete local;
			}
		}
		else if(format == TYPE_IMAGE_WXIMAGE)
		{
			int effWidth = bitmap->GetWidth() * 3;
			wxImage * local = bitmap->GetwxImage(false);
			if(local->HasAlpha())
				outputValue = LoadWxImageAlpha(local->GetData(), local->GetAlpha(), bitmap->GetWidth(), bitmap->GetHeight(), effWidth);
			else
				outputValue = LoadWxImage(local->GetData(), bitmap->GetWidth(), bitmap->GetHeight(), effWidth);

			RefreshMemoryBitmap(outputValue, bitmap->GetWidth(), bitmap->GetHeight());
			return;
		}
		else if(format == TYPE_IMAGE_REGARDSIMAGE)
		{
			CRegardsBitmap * local = bitmap->GetRegardsBitmap(false);
			outputValue = LoadRegardsImage(local->GetPtBitmap(), bitmap->GetWidth(), bitmap->GetHeight());
			RefreshMemoryBitmap(outputValue, bitmap->GetWidth(), bitmap->GetHeight());
		}
        else if(format == TYPE_IMAGE_REGARDSFLOATIMAGE)
        {
            CRegardsFloatBitmap * local = bitmap->GetFloatBitmap(false);
            COpenCLParameterFloatArray floatValue(true);
            floatValue.SetNoDelete(true);
            floatValue.SetValue(context->GetContext(), local->GetData(), local->GetWidth() * 4 *  local->GetHeight(), flag);
            outputValue = floatValue.GetValue();
            RefreshMemoryBitmap(outputValue, bitmap->GetWidth(), bitmap->GetHeight());
        }
	}

}


cl_mem COpenCLEffect::LoadCxImageAlpha(uint8_t * data, uint8_t * alpha, const int &width, const int &height, const int &effwidth, const bool & RGB)
{
	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_CXIMAGE");
	if(programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
		//program->SetBuildOptions("-D ITER_NUM=1000");

		//Create Memory Buffer for Bitmap
		COpenCLParameterByteArray *	dataImage = new COpenCLParameterByteArray();
		dataImage->SetValue(context->GetContext(), data, effwidth * height, flag);
		vecParam.push_back(dataImage);

		COpenCLParameterByteArray *	alphaImage = new COpenCLParameterByteArray();
		alphaImage->SetValue(context->GetContext(), alpha, width * height, flag);
		vecParam.push_back(alphaImage);

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		COpenCLParameterInt * parameffwidth = new COpenCLParameterInt();
		parameffwidth->SetLibelle("effwidth");
		parameffwidth->SetValue(effwidth);
		vecParam.push_back(parameffwidth);

		program->SetParameter(&vecParam,  width, height, sizeof(float) * 4 * width * height);
		program->SetKeepOutput(true);
		if(RGB)
			program->ExecuteProgram1D(programCL->GetProgram(), "LoadCxImageAlphaRGB");
		else
			program->ExecuteProgram1D(programCL->GetProgram(), "LoadCxImageAlpha");
		outputValue = program->GetOutput();

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

		//SetOutputValue(outputValue,width,height);
	}
	return outputValue;
}

cl_mem COpenCLEffect::LoadCxImage(uint8_t * data, const int &width, const int &height, const int &effwidth, const bool & RGB)
{
	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_CXIMAGE");
	if(programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
		//program->SetBuildOptions("-D ITER_NUM=1000");

		//Create Memory Buffer for Bitmap
		COpenCLParameterByteArray *	dataImage = new COpenCLParameterByteArray();
		dataImage->SetValue(context->GetContext(), data, effwidth * height, flag);
		vecParam.push_back(dataImage);

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		COpenCLParameterInt * parameffwidth = new COpenCLParameterInt();
		parameffwidth->SetLibelle("effwidth");
		parameffwidth->SetValue(effwidth);
		vecParam.push_back(parameffwidth);

		program->SetParameter(&vecParam,  width, height, sizeof(float) * 4 * width * height);
		program->SetKeepOutput(true);
		if(RGB)
			program->ExecuteProgram1D(programCL->GetProgram(), "LoadCxImageRGB");
		else
			program->ExecuteProgram1D(programCL->GetProgram(), "LoadCxImage");
		outputValue = program->GetOutput();

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

		//SetOutputValue(outputValue,width,height);
	}
	return outputValue;
}

cl_mem COpenCLEffect::LoadWxImageAlpha(uint8_t * data, uint8_t * alpha, const int &width, const int &height, const int &effwidth)
{
	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_CXIMAGE");
	if(programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
		//program->SetBuildOptions("-D ITER_NUM=1000");

		//Create Memory Buffer for Bitmap
		COpenCLParameterByteArray *	dataImage = new COpenCLParameterByteArray();
		dataImage->SetValue(context->GetContext(), data, effwidth * height, flag);
		vecParam.push_back(dataImage);

		COpenCLParameterByteArray *	alphaImage = new COpenCLParameterByteArray();
		alphaImage->SetValue(context->GetContext(), alpha, width * height, flag);
		vecParam.push_back(alphaImage);

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		COpenCLParameterInt * parameffwidth = new COpenCLParameterInt();
		parameffwidth->SetLibelle("effwidth");
		parameffwidth->SetValue(effwidth);
		vecParam.push_back(parameffwidth);

		program->SetParameter(&vecParam,  width, height, sizeof(float) * 4 * width * height);
		program->SetKeepOutput(true);
		program->ExecuteProgram1D(programCL->GetProgram(), "LoadWxImageAlpha");
		outputValue = program->GetOutput();

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

		//SetOutputValue(outputValue,width,height);
	}
	return outputValue;
}

cl_mem COpenCLEffect::LoadWxImage(uint8_t * data, const int &width, const int &height, const int &effwidth)
{
	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_CXIMAGE");
	if(programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
		//program->SetBuildOptions("-D ITER_NUM=1000");

		//Create Memory Buffer for Bitmap
		COpenCLParameterByteArray *	dataImage = new COpenCLParameterByteArray();
		dataImage->SetValue(context->GetContext(), data, effwidth * height, flag);
		vecParam.push_back(dataImage);

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		COpenCLParameterInt * parameffwidth = new COpenCLParameterInt();
		parameffwidth->SetLibelle("effwidth");
		parameffwidth->SetValue(effwidth);
		vecParam.push_back(parameffwidth);

		program->SetParameter(&vecParam,  width, height, sizeof(float) * 4 * width * height);
		program->SetKeepOutput(true);
		program->ExecuteProgram1D(programCL->GetProgram(), "LoadWxImage");
		outputValue = program->GetOutput();

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

		//SetOutputValue(outputValue,width,height);
	}
	return outputValue;
}
/*
void COpenCLEffect::SetBitmap(CRegardsBitmap * & pBitmap)
{
	RefreshMemoryBitmap(pBitmap);
}
*/
COpenCLEffect::~COpenCLEffect()
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
		
	if (input != nullptr)
	{
		input->Release();
		delete input;
	}
}


void COpenCLEffect::RefreshMemoryBitmap(cl_mem bitmapOut, const int & widthOutput, const int &  heightOutput)
{
	width = widthOutput;
	height = heightOutput;

	if(paramOutput != nullptr)
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
	COpenCLParameterClMem * valueInput = (COpenCLParameterClMem *)input;
	valueInput->SetValue(bitmapOut);
	valueInput->SetNoDelete(true);

	paramWidth = new COpenCLParameterInt();
	paramWidth->SetValue(width);
	paramWidth->SetNoDelete(true);

	paramHeight = new COpenCLParameterInt();
	paramHeight->SetValue(height);
	paramHeight->SetNoDelete(true);


}


void COpenCLEffect::RefreshMemoryBitmap(CRegardsBitmap * bitmapOut)
{
	if(bitmapOut != nullptr)
	{
		cl_mem outputValue = LoadRegardsImage(bitmapOut->GetPtBitmap(), bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight());
		RefreshMemoryBitmap(outputValue, bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight());
	}
}


CRegardsBitmap * COpenCLEffect::GetBitmap(cl_mem input, const int &width, const int &height)
{
	CRegardsBitmap * bitmap = nullptr;

	if(input != nullptr)
	{
		bitmap = new CRegardsBitmap(width, height);
		COpenCLFilter openclFilter(context);
		COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_REGARDBITMAP");
		if(programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem * valueInput = new COpenCLParameterClMem();
			valueInput->SetValue(input);
			valueInput->SetNoDelete(true);
			vecParam.push_back(valueInput);

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetLibelle("width");
			paramWidth->SetValue(bitmap->GetBitmapWidth());
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetLibelle("height");
			paramHeight->SetValue(bitmap->GetBitmapHeight());
			vecParam.push_back(paramHeight);

			program->SetParameter(&vecParam, bitmap);
			program->ExecuteProgram1D(programCL->GetProgram(), "GetRegardsBitmap");

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
	return bitmap;
}

CRegardsFloatBitmap * COpenCLEffect::GetFloatBitmap(const bool &source)
{
	CRegardsFloatBitmap * bitmapOut = nullptr;
	if(paramOutput != nullptr && !source)
	{
		bitmapOut = new CRegardsFloatBitmap(widthOut, heightOut);
		if(context != nullptr)
		{
			context->GetOutputData(paramOutput->GetValue(), bitmapOut->GetData(), bitmapOut->GetSize(), flag);
		}
	}
	else
	{
		bitmapOut = new CRegardsFloatBitmap(width, height);
		if(context != nullptr)
		{
			context->GetOutputData(input->GetValue(), bitmapOut->GetData(), bitmapOut->GetSize(), flag);
		}
	}
	return bitmapOut;
}

CRegardsBitmap * COpenCLEffect::GetBitmap(const bool &source)
{
	CRegardsBitmap * bitmapOut = nullptr;
	if(paramOutput != nullptr && !source)
	{
		//bitmapOut = new CRegardsBitmap(widthOut, heightOut);
		if(context != nullptr)
		{
			bitmapOut = GetBitmap(paramOutput->GetValue(), widthOut, heightOut);
			//context->GetOutputData(paramOutput->GetValue(), bitmapOut->GetPtBitmap(), bitmapOut->GetBitmapSize(), flag);
		}
	}
	else
	{
		//bitmapOut = new CRegardsBitmap(width, height);
		if(context != nullptr)
		{
			bitmapOut = GetBitmap(input->GetValue(), width, height);
			//context->GetOutputData(input->GetValue(), bitmapOut->GetPtBitmap(), bitmapOut->GetBitmapSize(), flag);
		}
	}
	return bitmapOut;

}
//-----------------------------------------------------------------------------------------------
//Get Output
//-----------------------------------------------------------------------------------------------

wxImage COpenCLEffect::GetwxImage()
{
	wxImage anImage(widthOut, heightOut, false);

	if(paramOutput != nullptr)
	{
		COpenCLFilter openclFilter(context);
		COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_WXWIDGET");
		if(programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
			//program->SetBuildOptions("-D ITER_NUM=1000");

			paramOutput->SetLibelle("input");
			vecParam.push_back(paramOutput);

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetLibelle("widthIn");
			paramWidth->SetValue(widthOut);
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetLibelle("heightIn");
			paramHeight->SetValue(heightOut);
			vecParam.push_back(paramHeight);

			program->SetParameter(&vecParam, &anImage);
			program->ExecuteProgram1D(programCL->GetProgram(), "BitmapToWxImage");

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

			paramOutput = nullptr;
		}
	}
	return anImage;
}


int COpenCLEffect::GetRgbaBitmap(void * cl_image)
{
	COpenCLFilter openclFilter(context);
	COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_TEXTURE");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		/*

		*/

		if(paramOutput != nullptr)
		{
			paramOutput->SetLibelle("input");
			paramOutput->SetNoDelete(false);
			vecParam.push_back(paramOutput);	

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(widthOut);
			paramWidth->SetLibelle("width");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(heightOut);
			paramHeight->SetLibelle("height");
			vecParam.push_back(paramHeight);
		}
		else
		{
			input->SetLibelle("input");
			input->SetNoDelete(false);
			vecParam.push_back(input);	

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(width);
			paramWidth->SetLibelle("width");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(height);
			paramHeight->SetLibelle("height");
			vecParam.push_back(paramHeight);
		}


		COpenCLParameterInt * paramRGBA = new COpenCLParameterInt();
		paramRGBA->SetValue(0);
		paramRGBA->SetLibelle("rgba");
		vecParam.push_back(paramRGBA);

		program->SetKeepOutput(true);
		if(paramOutput != nullptr)
			program->SetParameter(&vecParam, widthOut, heightOut, (cl_mem)cl_image);
		else
			program->SetParameter(&vecParam, width, height, (cl_mem)cl_image);
		program->ExecuteProgram(programCL->GetProgram(), "BitmapToOpenGLTexture");

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

	if(paramOutput != nullptr)
		paramOutput = nullptr;
	else
		input = nullptr;
	return 0;
}


void COpenCLEffect::SetOutputValue(cl_mem output, int widthOutput, int heightOutput)
{
	if(output != nullptr)
	{
		if(preview)
		{
			if(paramOutput != nullptr)
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

int COpenCLEffect::BilateralFilter(int fSize,  float sigmaX, float sigmaP)
{
	cl_mem output = nullptr;
	int _width = 0;
	int _height = 0;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.bilat2(fSize,sigmaX,sigmaP, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.bilat2(fSize,sigmaX,sigmaP, input->GetValue(), width, height);
		}
		
		SetOutputValue(output,_width,_height);
	}
	return 0;
}

int COpenCLEffect::BrightnessAndContrast(const double &brightness, const double &contrast)
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
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

		SetOutputValue(output,_width,_height);
	}
	return 0;
}

int COpenCLEffect::Swirl(const float &radius, const float &angle)
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Swirl(radius,angle, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Swirl(radius,angle, input->GetValue(), width, height);
		}

		SetOutputValue(output,_width,_height);
	}
	return 0;
}

int COpenCLEffect::NlmeansFilter(int fsize, int bsize, float sigma)
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.run2d(fsize,bsize,sigma, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.run2d(fsize,bsize,sigma, input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::Posterize(const float &level, const float &gamma)
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
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
		SetOutputValue(output,_width,_height);
	}
	return 0;
}

int COpenCLEffect::Median()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
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
		SetOutputValue(output,_width,_height);
	}
	return 0;
}

int COpenCLEffect::Negatif()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
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
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::Sepia()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
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
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::NoirEtBlanc()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
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
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::NiveauDeGris()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
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
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::FlipVertical()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
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
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::FlipHorizontal()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Flip("FlipHorizontal", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Flip("FlipHorizontal", input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::Rotate90()
{
	int _widthOut = 0; 
	int _heightOut = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_widthOut = heightOut; 
			_heightOut = widthOut;
			output = openclFilter.Rotate("Rotation90", _widthOut, _heightOut, 90.0f, paramOutput->GetValue(), widthOut, heightOut);
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
	int _widthOut = 0; 
	int _heightOut = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
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


int COpenCLEffect::RotateFree(const double &angle, const int &widthOut, const int &heightOut)
{
	int _widthOut = 0; 
	int _heightOut = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
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

int COpenCLEffect::SharpenMasking(const float &sharpness)
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
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
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::PhotoFiltre(const CRgbaquad &clValue, const int &intensity)
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.PhotoFiltre(clValue, intensity, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.PhotoFiltre(clValue, intensity,  input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::Solarize(const long &threshold)
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Solarize(threshold, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Solarize(threshold,  input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::RGBFilter(const int &red, const int &green, const int &blue)
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.RGBFilter(red, green, blue, paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.RGBFilter(red, green, blue,  input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::FiltreMosaic()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.FiltreMosaic(paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.FiltreMosaic( input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int COpenCLEffect::Fusion(CRegardsBitmap * bitmapSecond, const float &pourcentage)
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	cl_mem inputSecond = LoadRegardsImage(bitmapSecond->GetPtBitmap(), bitmapSecond->GetBitmapWidth(), bitmapSecond->GetBitmapHeight());
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Fusion(paramOutput->GetValue(), inputSecond, pourcentage, widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Fusion( input->GetValue(), inputSecond,  pourcentage, width, height);
		}
		SetOutputValue(output,_width,_height);
	}
	return 0;
}

int COpenCLEffect::Soften()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.FiltreConvolution("Soften",paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.FiltreConvolution("Soften", input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::Noise()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Noise(paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Noise( input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::Blur(const int &radius)
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.Blur(radius,paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.Blur(radius, input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::MotionBlur(const double &radius, const double &sigma, const double &angle)
{
	//CRegardsBitmap * bitmapOut = new CRegardsBitmap(width, height);
	if(context != nullptr)
	{
		vector<double> kernel;
		vector<wxPoint> offsets;

		if (sigma == 0.0)
			return 0;

		kernel = CMotionBlur::GetMotionBlurKernel(radius, sigma);

		if (kernel.size() < 3)
			return false;


		int _width = 0;
		int _height = 0;
		cl_mem output = nullptr;
		if(context != nullptr)
		{
			offsets = CMotionBlur::GetOffsetKernel(kernel.size(), angle);
			COpenCLFilter openclFilter(context);
			if(preview && paramOutput != nullptr)
			{
				_width = widthOut;
				_height = heightOut;
				output = openclFilter.MotionBlurCompute(kernel, offsets, kernel.size(),paramOutput->GetValue(), widthOut, heightOut);
			}
			else
			{
				_width = width;
				_height = height;
				output = openclFilter.MotionBlurCompute(kernel, offsets, kernel.size(), input->GetValue(), width, height);
			}
			SetOutputValue(output,_width,_height);
		}
	}
	//delete bitmapOut;
	return 0;
}

int COpenCLEffect::Emboss()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.FiltreConvolution("Emboss", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.FiltreConvolution("Emboss",  input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::SharpenStrong()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.FiltreConvolution("SharpenStrong", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.FiltreConvolution("SharpenStrong", input->GetValue(),  width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::Sharpen()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.FiltreConvolution("Sharpen", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.FiltreConvolution("Sharpen",  input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::FiltreEdge()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.FiltreConvolution("Edge", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.FiltreConvolution("Edge",  input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}

	return 0;
}

int COpenCLEffect::Erode()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.ErodeDilate("Erode", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.ErodeDilate("Erode",  input->GetValue(), width, height);
		}
		SetOutputValue(output,_width,_height);
	}
	return 0;
}

int COpenCLEffect::Dilate()
{
	int _width = 0;
	int _height = 0;
	cl_mem output = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if(preview && paramOutput != nullptr)
		{
			_width = widthOut;
			_height = heightOut;
			output = openclFilter.ErodeDilate("Dilate", paramOutput->GetValue(), widthOut, heightOut);
		}
		else
		{
			_width = width;
			_height = height;
			output = openclFilter.ErodeDilate("Dilate", input->GetValue(),  width, height);
		}
		SetOutputValue(output,_width,_height);
	}
	return 0;
}

int COpenCLEffect::GaussianBlur(const int &r, const int &boxSize)
{
	//CRegardsBitmap * bitmapOut = new CRegardsBitmap(width, height);
	if(context != nullptr)
	{
		if (boxSize > 0)
		{
			double wIdeal = sqrt((double)((12 * r*r / boxSize) + 1));
			int wl = (int)floor(wIdeal);
			if (wl % 2 == 0)
				wl--;
			int wu = wl + 2;
			double mIdeal = (12 * r*r - boxSize*wl*wl - 4 * boxSize*wl - 3 * boxSize) / (-4 * wl - 4);
			int m = round(mIdeal);
			vector<int> gaussCoeff;
			for (auto i = 0; i < boxSize; i++)
				gaussCoeff.push_back(i < m ? wl : wu);

			bool noDeleteData = true;
			int _width = 0;
			int _height = 0;
			cl_mem output = nullptr;

			COpenCLFilter openclFilter(context);
			_width = widthOut;
			_height = heightOut;
			if(preview && paramOutput != nullptr)
				output = paramOutput->GetValue();
			else
				output = input->GetValue();

			output = openclFilter.BoxBlur((gaussCoeff[0] - 1) / 2, "BoxBlurH", output, widthOut, heightOut, noDeleteData);
			noDeleteData = false;
			output = openclFilter.BoxBlur((gaussCoeff[0] - 1) / 2, "BoxBlurV", output, widthOut, heightOut, noDeleteData);

			for (auto i = 1; i < boxSize; i++)
			{
				output = openclFilter.BoxBlur((gaussCoeff[i] - 1) / 2, "BoxBlurH", output, widthOut, heightOut, noDeleteData);
				output = openclFilter.BoxBlur((gaussCoeff[i] - 1) / 2, "BoxBlurV", output, widthOut, heightOut, noDeleteData);
			}

			SetOutputValue(output,_width,_height);

		}
	}
	return 0;
}

//-----------------------------------------------------------------------------------------------
//Execute INTERPOLATION
//-----------------------------------------------------------------------------------------------

void COpenCLEffect::Interpolation(const int &widthOut, const int &heightOut, const int &method, int flipH, int flipV, int angle)
{
	if(paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
	}

	cl_mem output = nullptr;

	try
	{
		COpenCLFilter openclFilter(context);
		output = openclFilter.Interpolation(widthOut, heightOut, method, input->GetValue(), width, height, flipH, flipV, angle);
	}
	catch(...)
	{
		output = nullptr;
	}

	if(output != nullptr)
	{
		paramOutput = new COpenCLParameterClMem();
		paramOutput->SetValue(output);
		this->widthOut = widthOut;
		this->heightOut = heightOut;
	}
}

void COpenCLEffect::Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int &method, int flipH, int flipV, int angle)
{
	if(paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
	}

	COpenCLFilter openclFilter(context);
	cl_mem output = openclFilter.Interpolation(widthOut, heightOut, rc, method, input->GetValue(), width, height, flipH, flipV, angle);

	paramOutput = new COpenCLParameterClMem();
	paramOutput->SetValue(output);
	this->widthOut = widthOut;
	this->heightOut = heightOut;


}


float COpenCLEffect::Filter(const float &f)
{

	return (f < -2.0 || f > 2.0) ? 0.0 : (
		(f < -1.0) ? (2.0 + f)*(2.0 + f)*(2.0 + f) / 6.0 : (
		(f < 0.0) ? (4.0 + f*f*(-6.0 - 3.0*f)) / 6.0 : (
		(f < 1.0) ? (4.0 + f*f*(-6.0 + 3.0*f)) / 6.0 : (2.0 - f)*(2.0 - f)*(2.0 - f) / 6.0)));

}
