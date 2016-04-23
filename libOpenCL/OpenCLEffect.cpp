#include "OpenCLEffect.h"
#include "OpenCLParameter.h"
#include "OpenCLEngine.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include "RegardsBitmap.h"
using namespace Regards::OpenCL;
using namespace Regards::FiltreEffet;

COpenCLEffect::COpenCLEffect(CRegardsBitmap * pBitmap, const CRgbaquad &backColor)
	: IFiltreEffet(pBitmap, backColor)
{
	bool useMemory = false;
	this->pBitmap = pBitmap;
	this->backColor = backColor;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	openCLProgram = nullptr;
}

COpenCLEffect::~COpenCLEffect()
{
	if (openCLProgram != nullptr)
		delete openCLProgram;

	openCLProgram = nullptr;
}

int COpenCLEffect::Negatif()
{
	return ColorEffect("Negatif");
}

int COpenCLEffect::Sepia()
{
	return ColorEffect("Sepia");
}

int COpenCLEffect::NoirEtBlanc()
{
	return ColorEffect("NoirEtBlanc");
}


int COpenCLEffect::NiveauDeGris()
{
	return ColorEffect("GrayLevel");
}

int COpenCLEffect::InterpolationBicubic(CRegardsBitmap * & bitmapOut)
{
	return Interpolation(bitmapOut, "BicubicInterpolation");
}

int COpenCLEffect::InterpolationBicubic(CRegardsBitmap * & bitmapOut, const wxRect &rc)
{
	return Interpolation(bitmapOut, rc, "BicubicInterpolationZone");
}

int COpenCLEffect::FlipVertical()
{
	return Flip("FlipVertical");
}

int COpenCLEffect::FlipHorizontal()
{
	return Flip("FlipHorizontal");
}

int COpenCLEffect::Rotate90()
{
	int widthOut = pBitmap->GetBitmapHeight(); 
	int heightOut = pBitmap->GetBitmapWidth();
	return Rotate("Rotation90", widthOut, heightOut, 90.0f);
}

int COpenCLEffect::Rotate270()
{
	int widthOut = pBitmap->GetBitmapHeight();
	int heightOut = pBitmap->GetBitmapWidth();
	return Rotate("Rotation270", widthOut, heightOut, 270.0f);
}


int COpenCLEffect::RotateFree(const double &angle, const int &widthOut, const int &heightOut)
{
	return Rotate("RotateFree", widthOut, heightOut, angle);
}

int COpenCLEffect::SharpenMasking(const int &sharpness)
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();

	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_SHARPENMASKING");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("width");
	paramWidth->SetValue(widthOut);
	vecParam.push_back(paramWidth);

	COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("height");
	paramHeight->SetValue(heightOut);
	vecParam.push_back(paramHeight);

	COpenCLParameterInt * paramIntensity = new COpenCLParameterInt();
	paramIntensity->SetLibelle("sharpness");
	paramIntensity->SetValue(sharpness);
	vecParam.push_back(paramIntensity);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "SharpenMasking");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::PhotoFiltre(const CRgbaquad &clValue, const int &intensity)
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();

	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramIntensity = new COpenCLParameterInt();
	paramIntensity->SetLibelle("intensity");
	paramIntensity->SetValue(intensity);
	vecParam.push_back(paramIntensity);

	COLORData color = { clValue.GetRed(), clValue.GetGreen(), clValue.GetBlue(), 0 };
	
	COpenCLParameterColorData * paramColor = new COpenCLParameterColorData();
	paramColor->SetLibelle("color");
	paramColor->SetValue(context->GetContext(), &color, flag);
	vecParam.push_back(paramColor);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram1D(programCL->GetProgram(), "PhotoFiltre");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::RGBFilter(const int &red, const int &green, const int &blue)
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();

	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COLORData color = { red, green, blue, 0 };

	COpenCLParameterColorData * paramColor = new COpenCLParameterColorData();
	paramColor->SetLibelle("color");
	paramColor->SetValue(context->GetContext(), &color, flag);
	vecParam.push_back(paramColor);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram1D(programCL->GetProgram(), "RGBFiltre");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::FiltreMosaic()
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();

	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_MOSAIC");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("width");
	paramWidth->SetValue(widthOut);
	vecParam.push_back(paramWidth);

	COpenCLParameterFloat * paramAngle = new COpenCLParameterFloat();
	paramAngle->SetLibelle("fTileSize");
	paramAngle->SetValue(5.0f);
	vecParam.push_back(paramAngle);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "Mosaic");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int COpenCLEffect::Fusion(CRegardsBitmap * bitmapSecond, const float &pourcentage)
{
	int widthOut = pBitmap->GetBitmapWidth(); 
	int heightOut = pBitmap->GetBitmapHeight();
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_FUSION");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterByteArray * inputSecond = new COpenCLParameterByteArray();
	inputSecond->SetLibelle("inputSecond");
	inputSecond->SetValue(context->GetContext(), bitmapSecond->GetPtBitmap(), (int)bitmapSecond->GetBitmapSize(), flag);
	vecParam.push_back(inputSecond);

	COpenCLParameterFloat * prcent = new COpenCLParameterFloat();
	prcent->SetLibelle("pourcentage");
	prcent->SetValue(pourcentage);
	vecParam.push_back(prcent);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram1D(programCL->GetProgram(), "Fusion");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();

	return 0;
}

int COpenCLEffect::Soften()
{
	return FiltreConvolution("Soften");
}

int COpenCLEffect::Blur()
{
	return FiltreConvolution("Blur");
}

int COpenCLEffect::GaussianBlur()
{
	return FiltreConvolution("GaussianBlur");
}

int COpenCLEffect::Emboss()
{
	return FiltreConvolution("Emboss");
}

int COpenCLEffect::SharpenStrong()
{
	return FiltreConvolution("SharpenStrong");
}

int COpenCLEffect::Sharpen()
{
	return FiltreConvolution("Sharpen");
}

int COpenCLEffect::InterpolationBicubicRGB32Video(CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle)
{
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLORCONVERSION");
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidthOut = new COpenCLParameterInt();
	paramWidthOut->SetLibelle("widthOut");
	paramWidthOut->SetValue(bitmapOut->GetBitmapWidth());
	vecParam.push_back(paramWidthOut);

	COpenCLParameterInt * paramHeightOut = new COpenCLParameterInt();
	paramHeightOut->SetLibelle("heightOut");
	paramHeightOut->SetValue(bitmapOut->GetBitmapHeight());
	vecParam.push_back(paramHeightOut);

	COpenCLParameterInt * paramflipH = new COpenCLParameterInt();
	paramflipH->SetLibelle("flipH");
	paramflipH->SetValue(flipH);
	vecParam.push_back(paramflipH);

	COpenCLParameterInt * paramflipV = new COpenCLParameterInt();
	paramflipV->SetLibelle("flipV");
	paramflipV->SetValue(flipV);
	vecParam.push_back(paramflipV);

	COpenCLParameterInt * paramAngle = new COpenCLParameterInt();
	paramAngle->SetLibelle("angle");
	paramAngle->SetValue(angle);
	vecParam.push_back(paramAngle);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "BicubicRGB32Video");

	delete program;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::InterpolationBicubicNV12ToRGB32(uint8_t * buffer, const int &width, const int &height, const int &rectWidth, const int &rectHeight, CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle, const int &size)
{
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLORCONVERSION");
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), buffer, size, flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("width");
	paramWidth->SetValue(width);
	vecParam.push_back(paramWidth);

	COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("height");
	paramHeight->SetValue(height);
	vecParam.push_back(paramHeight);

	COpenCLParameterInt * paramRectWidth = new COpenCLParameterInt();
	paramRectWidth->SetLibelle("rectWidth");
	paramRectWidth->SetValue(rectWidth);
	vecParam.push_back(paramRectWidth);

	COpenCLParameterInt * paramRectHeight = new COpenCLParameterInt();
	paramRectHeight->SetLibelle("rectHeight");
	paramRectHeight->SetValue(rectHeight);
	vecParam.push_back(paramRectHeight);

	COpenCLParameterInt * paramWidthOut = new COpenCLParameterInt();
	paramWidthOut->SetLibelle("widthOut");
	paramWidthOut->SetValue(bitmapOut->GetBitmapWidth());
	vecParam.push_back(paramWidthOut);

	COpenCLParameterInt * paramHeightOut = new COpenCLParameterInt();
	paramHeightOut->SetLibelle("heightOut");
	paramHeightOut->SetValue(bitmapOut->GetBitmapHeight());
	vecParam.push_back(paramHeightOut);

	COpenCLParameterInt * paramflipH = new COpenCLParameterInt();
	paramflipH->SetLibelle("flipH");
	paramflipH->SetValue(flipH);
	vecParam.push_back(paramflipH);

	COpenCLParameterInt * paramflipV = new COpenCLParameterInt();
	paramflipV->SetLibelle("flipV");
	paramflipV->SetValue(flipV);
	vecParam.push_back(paramflipV);

	COpenCLParameterInt * paramAngle = new COpenCLParameterInt();
	paramAngle->SetLibelle("angle");
	paramAngle->SetValue(angle);
	vecParam.push_back(paramAngle);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "BicubicNV12toRGB32");

	delete program;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::RGB24ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size)
{ 
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLORCONVERSION");
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), buffer, size, flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("width");
	paramWidth->SetValue(bitmapOut->GetBitmapWidth());
	vecParam.push_back(paramWidth);

	COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("height");
	paramHeight->SetValue(bitmapOut->GetBitmapHeight());
	vecParam.push_back(paramHeight);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "rgb24_to_rgb32");

	delete program;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}



int COpenCLEffect::NV12ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size)
{
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLORCONVERSION");
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), buffer, size, flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("width");
	paramWidth->SetValue(width);
	vecParam.push_back(paramWidth);

	COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("height");
	paramHeight->SetValue(height);
	vecParam.push_back(paramHeight);

	COpenCLParameterInt * paramWidthOut = new COpenCLParameterInt();
	paramWidthOut->SetLibelle("widthOut");
	paramWidthOut->SetValue(bitmapOut->GetBitmapWidth());
	vecParam.push_back(paramWidthOut);

	COpenCLParameterInt * paramHeightOut = new COpenCLParameterInt();
	paramHeightOut->SetLibelle("heightOut");
	paramHeightOut->SetValue(bitmapOut->GetBitmapHeight());
	vecParam.push_back(paramHeightOut);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "NV12_to_RGB32");

	delete program;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::YUV420ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size)
{ 
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLORCONVERSION");
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), buffer, size, flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("width");
	paramWidth->SetValue(bitmapOut->GetBitmapWidth());
	vecParam.push_back(paramWidth);

	COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("height");
	paramHeight->SetValue(bitmapOut->GetBitmapHeight());
	vecParam.push_back(paramHeight);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "YUV420_to_RGB32");

	delete program;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}


int COpenCLEffect::FiltreConvolution(const wxString &functionName)
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();

	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_CONVOLUTION");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("width");
	paramWidth->SetValue(widthOut);
	vecParam.push_back(paramWidth);

	COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("height");
	paramHeight->SetValue(heightOut);
	vecParam.push_back(paramHeight);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), functionName);

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::FiltreEdge()
{
	return FiltreConvolution("Edge");
}

int COpenCLEffect::Erode()
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();

	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_DILATEERODE");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("width");
	paramWidth->SetValue(widthOut);
	vecParam.push_back(paramWidth);

	COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("height");
	paramHeight->SetValue(heightOut);
	vecParam.push_back(paramHeight);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "Erode");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::Dilate()
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();

	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_DILATEERODE");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("width");
	paramWidth->SetValue(widthOut);
	vecParam.push_back(paramWidth);

	COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("height");
	paramHeight->SetValue(heightOut);
	vecParam.push_back(paramHeight);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "Dilate");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::Posterize(const float &level, const float &gamma)
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramLevel = new COpenCLParameterInt();
	paramLevel->SetLibelle("leve");
	paramLevel->SetValue(level);
	vecParam.push_back(paramLevel);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram1D(programCL->GetProgram(), "Posterisation");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}


int COpenCLEffect::Solarize(const long &threshold)
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramThreshold = new COpenCLParameterInt();
	paramThreshold->SetLibelle("threshold");
	paramThreshold->SetValue((int)threshold);
	vecParam.push_back(paramThreshold);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram1D(programCL->GetProgram(), "Solarization");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::Median()
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();

	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_MEDIAN");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("width");
	paramWidth->SetValue(widthOut);
	vecParam.push_back(paramWidth);

	COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("height");
	paramHeight->SetValue(heightOut);
	vecParam.push_back(paramHeight);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "Median");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::Noise()
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();

	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_NOISE");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
	paramWidth->SetLibelle("width");
	paramWidth->SetValue(widthOut);
	vecParam.push_back(paramWidth);

	COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
	paramHeight->SetLibelle("height");
	paramHeight->SetValue(heightOut);
	vecParam.push_back(paramHeight);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "Noise");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;


}


int COpenCLEffect::InterpolationBilinear(CRegardsBitmap * & bitmapOut)
{
	return Interpolation(bitmapOut, "BilinearInterpolation");
}

int COpenCLEffect::InterpolationBilinear(CRegardsBitmap * & bitmapOut, const wxRect &rc)
{
	return Interpolation(bitmapOut, "BilinearInterpolationZone");
}

int COpenCLEffect::InterpolationFast(CRegardsBitmap * & bitmapOut)
{
	return Interpolation(bitmapOut, "FastInterpolation");
}

int COpenCLEffect::InterpolationFast(CRegardsBitmap * & bitmapOut, const wxRect &rc)
{
	return Interpolation(bitmapOut, rc, "FastInterpolationZone");
}

int COpenCLEffect::InterpolationBicubic(CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle)
{
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLORCONVERSION");
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * widthIn = new COpenCLParameterInt();
	widthIn->SetLibelle("width");
	widthIn->SetValue(pBitmap->GetBitmapWidth());
	vecParam.push_back(widthIn);

	COpenCLParameterInt * heightIn = new COpenCLParameterInt();
	heightIn->SetLibelle("height");
	heightIn->SetValue(pBitmap->GetBitmapHeight());
	vecParam.push_back(heightIn);

	COpenCLParameterInt * paramWidthOut = new COpenCLParameterInt();
	paramWidthOut->SetLibelle("widthOut");
	paramWidthOut->SetValue(bitmapOut->GetBitmapWidth());
	vecParam.push_back(paramWidthOut);

	COpenCLParameterInt * paramHeightOut = new COpenCLParameterInt();
	paramHeightOut->SetLibelle("heightOut");
	paramHeightOut->SetValue(bitmapOut->GetBitmapHeight());
	vecParam.push_back(paramHeightOut);

	COpenCLParameterInt * paramflipH = new COpenCLParameterInt();
	paramflipH->SetLibelle("flipH");
	paramflipH->SetValue(flipH);
	vecParam.push_back(paramflipH);

	COpenCLParameterInt * paramflipV = new COpenCLParameterInt();
	paramflipV->SetLibelle("flipV");
	paramflipV->SetValue(flipV);
	vecParam.push_back(paramflipV);

	COpenCLParameterInt * paramAngle = new COpenCLParameterInt();
	paramAngle->SetLibelle("angle");
	paramAngle->SetValue(angle);
	vecParam.push_back(paramAngle);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "BicubicRGB32");

	delete program;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::Interpolation(CRegardsBitmap * & bitmapOut, const wxRect &rc, const wxString &functionName)
{
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_INTERPOLATION");
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * widthIn = new COpenCLParameterInt();
	widthIn->SetLibelle("widthIn");
	widthIn->SetValue(pBitmap->GetBitmapWidth());
	vecParam.push_back(widthIn);

	COpenCLParameterInt * heightIn = new COpenCLParameterInt();
	heightIn->SetLibelle("heightIn");
	heightIn->SetValue(pBitmap->GetBitmapHeight());
	vecParam.push_back(heightIn);

	COpenCLParameterInt * paramWidthOut = new COpenCLParameterInt();
	paramWidthOut->SetLibelle("widthOut");
	paramWidthOut->SetValue(bitmapOut->GetBitmapWidth());
	vecParam.push_back(paramWidthOut);

	COpenCLParameterInt * paramHeightOut = new COpenCLParameterInt();
	paramHeightOut->SetLibelle("heightOut");
	paramHeightOut->SetValue(bitmapOut->GetBitmapHeight());
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

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), functionName);

	delete program;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

wxImage COpenCLEffect::InterpolationBicubic(const wxImage & imageSrc, const int &widthOut, const int &heightOut)
{
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_INTERPOLATION_RGB");
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");
	wxImage imageOut = wxImage(widthOut, heightOut);

	int bitmapSize = imageSrc.GetWidth() * imageSrc.GetHeight() * 3;

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), imageSrc.GetData(), bitmapSize, flag);
	vecParam.push_back(input);

	COpenCLParameterInt * widthIn = new COpenCLParameterInt();
	widthIn->SetLibelle("widthIn");
	widthIn->SetValue(imageSrc.GetWidth());
	vecParam.push_back(widthIn);

	COpenCLParameterInt * heightIn = new COpenCLParameterInt();
	heightIn->SetLibelle("heightIn");
	heightIn->SetValue(imageSrc.GetHeight());
	vecParam.push_back(heightIn);

	COpenCLParameterInt * paramWidthOut = new COpenCLParameterInt();
	paramWidthOut->SetLibelle("widthOut");
	paramWidthOut->SetValue(widthOut);
	vecParam.push_back(paramWidthOut);

	COpenCLParameterInt * paramHeightOut = new COpenCLParameterInt();
	paramHeightOut->SetLibelle("heightOut");
	paramHeightOut->SetValue(heightOut);
	vecParam.push_back(paramHeightOut);

	program->SetParameter(&vecParam, &imageOut);
	program->ExecuteProgram(programCL->GetProgram(), "BicubicInterpolation");

	delete program;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return imageOut;
}

int COpenCLEffect::Interpolation(CRegardsBitmap * & bitmapOut, const wxString &functionName)
{
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_INTERPOLATION");
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * widthIn = new COpenCLParameterInt();
	widthIn->SetLibelle("widthIn");
	widthIn->SetValue(pBitmap->GetBitmapWidth());
	vecParam.push_back(widthIn);

	COpenCLParameterInt * heightIn = new COpenCLParameterInt();
	heightIn->SetLibelle("heightIn");
	heightIn->SetValue(pBitmap->GetBitmapHeight());
	vecParam.push_back(heightIn);

	COpenCLParameterInt * paramWidthOut = new COpenCLParameterInt();
	paramWidthOut->SetLibelle("widthOut");
	paramWidthOut->SetValue(bitmapOut->GetBitmapWidth());
	vecParam.push_back(paramWidthOut);

	COpenCLParameterInt * paramHeightOut = new COpenCLParameterInt();
	paramHeightOut->SetLibelle("heightOut");
	paramHeightOut->SetValue(bitmapOut->GetBitmapHeight());
	vecParam.push_back(paramHeightOut);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), functionName);

	delete program;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::Flip(const wxString &functionName)
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_FLIP");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");
	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidthIn = new COpenCLParameterInt();
	paramWidthIn->SetLibelle("widthIn");
	paramWidthIn->SetValue(pBitmap->GetBitmapWidth());
	vecParam.push_back(paramWidthIn);

	COpenCLParameterInt * paramHeightIn = new COpenCLParameterInt();
	paramHeightIn->SetLibelle("heightIn");
	paramHeightIn->SetValue(pBitmap->GetBitmapHeight());
	vecParam.push_back(paramHeightIn);

	COpenCLParameterInt * paramWidthOut = new COpenCLParameterInt();
	paramWidthOut->SetLibelle("widthOut");
	paramWidthOut->SetValue(widthOut);
	vecParam.push_back(paramWidthOut);

	COpenCLParameterInt * paramHeightOut = new COpenCLParameterInt();
	paramHeightOut->SetLibelle("heightOut");
	paramHeightOut->SetValue(heightOut);
	vecParam.push_back(paramHeightOut);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), functionName);

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}


int COpenCLEffect::Swirl(const float &radius, const float &angle)
{
	int widthOut = pBitmap->GetBitmapWidth();
	int heightOut = pBitmap->GetBitmapHeight();
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_SWIR");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidthIn = new COpenCLParameterInt();
	paramWidthIn->SetLibelle("width");
	paramWidthIn->SetValue(pBitmap->GetBitmapWidth());
	vecParam.push_back(paramWidthIn);

	COpenCLParameterInt * paramHeightIn = new COpenCLParameterInt();
	paramHeightIn->SetLibelle("height");
	paramHeightIn->SetValue(pBitmap->GetBitmapHeight());
	vecParam.push_back(paramHeightIn);

	COpenCLParameterFloat * paramRadius = new COpenCLParameterFloat();
	paramRadius->SetLibelle("radius");
	paramRadius->SetValue(radius);
	vecParam.push_back(paramRadius);

	COpenCLParameterFloat * paramAngle = new COpenCLParameterFloat();
	paramAngle->SetLibelle("angle");
	paramAngle->SetValue(angle);
	vecParam.push_back(paramAngle);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), "Swir");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int COpenCLEffect::BrightnessAndContrast(const double &brightness, const double &contrast)
{
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterFloat * paramConstrast = new COpenCLParameterFloat();
	paramConstrast->SetLibelle("contrast");
	paramConstrast->SetValue(contrast);
	vecParam.push_back(paramConstrast);

	COpenCLParameterFloat * paramLightness = new COpenCLParameterFloat();
	paramLightness->SetLibelle("light");
	paramLightness->SetValue(brightness);
	vecParam.push_back(paramLightness);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram1D(programCL->GetProgram(), "LightAndContrast");

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

COpenCLProgram * COpenCLEffect::GetProgram(const wxString &numProgram)
{
	return COpenCLEngine::GetProgram(numProgram);
}

COpenCLContext * COpenCLEffect::GetContext()
{
	return COpenCLEngine::getInstance();
}

int COpenCLEffect::ColorEffect(const wxString &functionName)
{
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram1D(programCL->GetProgram(), functionName);

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}

int  COpenCLEffect::Rotate(const wxString &functionName, const int &widthOut, const int &heightOut, const double &angle)
{
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_ROTATION");
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	vector<COpenCLParameter *> vecParam;
	COpenCLContext * context = GetContext();
	COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
	//program->SetBuildOptions("-D ITER_NUM=1000");

	COpenCLParameterByteArray * input = new COpenCLParameterByteArray();
	input->SetLibelle("input");
	input->SetValue(context->GetContext(), pBitmap->GetPtBitmap(), (int)pBitmap->GetBitmapSize(), flag);
	vecParam.push_back(input);

	COpenCLParameterInt * paramWidthIn = new COpenCLParameterInt();
	paramWidthIn->SetLibelle("widthIn");
	paramWidthIn->SetValue(pBitmap->GetBitmapWidth());
	vecParam.push_back(paramWidthIn);

	COpenCLParameterInt * paramHeightIn = new COpenCLParameterInt();
	paramHeightIn->SetLibelle("heightIn");
	paramHeightIn->SetValue(pBitmap->GetBitmapHeight());
	vecParam.push_back(paramHeightIn);

	COpenCLParameterInt * paramWidthOut = new COpenCLParameterInt();
	paramWidthOut->SetLibelle("widthOut");
	paramWidthOut->SetValue(widthOut);
	vecParam.push_back(paramWidthOut);

	COpenCLParameterInt * paramHeightOut = new COpenCLParameterInt();
	paramHeightOut->SetLibelle("heightOut");
	paramHeightOut->SetValue(heightOut);
	vecParam.push_back(paramHeightOut);

	COpenCLParameterFloat * paramAngle = new COpenCLParameterFloat();
	paramAngle->SetLibelle("angle");
	paramAngle->SetValue(angle);
	vecParam.push_back(paramAngle);

	program->SetParameter(&vecParam, bitmapOut);
	program->ExecuteProgram(programCL->GetProgram(), functionName);

	delete program;

	pBitmap->SetBitmap(bitmapOut->GetPtBitmap(), widthOut, heightOut);

	delete bitmapOut;

	for (COpenCLParameter * parameter : vecParam)
	{
		delete parameter;
		parameter = nullptr;
	}
	vecParam.clear();
	return 0;
}
