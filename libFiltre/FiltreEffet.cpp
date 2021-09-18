#include <header.h>
#include "FiltreEffet.h"
#include <OpenCLContext.h>
#include "FiltreEffetCPU.h"
#include "OpenCLEffect.h"
#include <OpenCLParameter.h>
#include "RedEye.h"
#include <FilterData.h>
#include "WaveFilter.h"
#include <ImageLoadingFormat.h>
#include <config_id.h>
extern float value[256];

using namespace Regards::FiltreEffet;
using namespace std;

/*
void CFiltreEffet::SetLib(const int &lib)
{
	numLib = lib;
}
*/

int CFiltreEffet::GetLib()
{
	return numLib;
}

///// LUT tables
#define LUTMAX 30.0
#define LUTMAXM1 29.0
#define LUTPRECISION 1000.0

#ifndef MAX
#define MAX(i,j) ( (i)<(j) ? (j):(i) )
#endif
#ifndef MIN
#define MIN(i,j) ( (i)<(j) ? (i):(j) )
#endif

int CFiltreEffet::RenderEffect(const int& numEffect, CEffectParameter* effectParameter)
{
	return CFiltreData::RenderEffect(numEffect, this, effectParameter);
}


int CFiltreEffet::OilPaintingEffect(const int& size, const int& dynRatio)
{
	return filtreEffet->OilPaintingEffect(size, dynRatio);
}

int CFiltreEffet::GetWidth()
{
	return filtreEffet->GetWidth();
}

int CFiltreEffet::GetHeight()
{
	return filtreEffet->GetHeight();
}

CRegardsBitmap* CFiltreEffet::GetPtBitmap()
{
	return filtreEffet->GetPtBitmap();
}

int CFiltreEffet::Bm3d(const int& fSigma)
{
	filtreEffet->Bm3d(fSigma);
	return 0;
}

int CFiltreEffet::BokehEffect(const int& radius, const int& boxsize, const int& nbFace, const wxRect & listFace)
{
	filtreEffet->BokehEffect(radius, boxsize, nbFace, listFace);
	return 0;
}


int CFiltreEffet::WaveFilter(int x, int y, short height, int scale, int radius)
{
	filtreEffet->WaveFilter(x, y, height, scale, radius);
	return 0;
}

int CFiltreEffet::BilateralFilter(const int & fSize, const int& sigmaX, const int& sigmaP)
{
	return filtreEffet->BilateralFilter(fSize, sigmaX, sigmaP);
}


int CFiltreEffet::NlmeansFilter(const int& h, const int& templateWindowSize, const int& searchWindowSize)
{
	return filtreEffet->NlmeansFilter(h, templateWindowSize, searchWindowSize);
}

int CFiltreEffet::MeanShift(const float& fSpatialRadius, const float& fColorRadius)
{
	return filtreEffet->MeanShift(fSpatialRadius, fColorRadius);
}


wxImage CFiltreEffet::RenderEffectPreviewwxImage(const int& numEffect, CEffectParameter* effectParameter)
{
	if (RenderEffectPreview(numEffect, effectParameter))
		return this->GetwxImage();
	return wxImage();
}

void CFiltreEffet::SetPreviewMode(const bool& value)
{
	filtreEffet->SetPreviewMode(value);
}

int CFiltreEffet::RenderEffectPreview(const int& numEffect, CEffectParameter* effectParameter)
{
	filtreEffet->SetPreviewMode(true);
	int value = CFiltreData::RenderEffect(numEffect, this, effectParameter, true);
	filtreEffet->SetPreviewMode(false);
	return value;
}

CFiltreEffet::CFiltreEffet(const CRgbaquad& backColor, COpenCLContext* openCLContext, COpenCLParameterClMem* data_mem,
                           const int& width, const int& height)
{
	filtreEffet = nullptr;
	this->backColor = backColor;
	this->openCLContext = openCLContext;
	filename = "";
	this->width = width;
	this->height = height;
	filtreEffet = new COpenCLEffect(backColor, openCLContext, data_mem, width, height);
	this->numLib = LIBOPENCL;
}

CFiltreEffet::CFiltreEffet(const CRgbaquad& backColor, COpenCLContext* openCLContext, CImageLoadingFormat* bitmap)
{
	filtreEffet = nullptr;
	this->backColor = backColor;
	this->numLib = LIBCPU;
	this->openCLContext = openCLContext;
	filename = bitmap->GetFilename();
	width = bitmap->GetWidth();
	height = bitmap->GetHeight();

	if (openCLContext != nullptr)
	{
		filtreEffet = new COpenCLEffect(backColor, openCLContext, bitmap);
		this->numLib = LIBOPENCL;
	}
	
	if (this->numLib == LIBCPU)
	{
		filtreEffet = new CFiltreEffetCPU(backColor, bitmap);
	}
}


bool CFiltreEffet::OpenCLHasEnoughMemory()
{
	printf("OpenCLHasEnoughMemory \n");
	if (openCLContext != nullptr)
	{
		uint64_t memsizeMax = openCLContext->GetMaxMemoryAllocable();
		if (openCLContext->GetDefaultType() == OPENCL_UCHAR)
		{
			if (memsizeMax > width * height * sizeof(cl_uint))
			{
				printf("OpenCLHasEnoughMemory UCHAR return true \n");
				return true;
			}
		}
		else
		{
			if (memsizeMax > width * height * 4 * sizeof(float))
			{
				printf("OpenCLHasEnoughMemory CL_FLOAT return true \n");
				return true;
			}
		}
	}
	printf("OpenCLHasEnoughMemory return false \n");
	return false;
}

void CFiltreEffet::SetBitmap(CImageLoadingFormat* bitmap)
{
	if (filtreEffet != nullptr)
		filtreEffet->SetBitmap(bitmap);
}

CFiltreEffet::~CFiltreEffet()
{
	if (filtreEffet != nullptr)
		delete(filtreEffet);
}

void CFiltreEffet::GetRgbaBitmap(void* cl_image)
{
	filtreEffet->GetRgbaBitmap(cl_image);
}

int CFiltreEffet::RedEye()
{
	filtreEffet->RedEye();
	return 0;
}

wxImage CFiltreEffet::GetwxImage()
{
	return filtreEffet->GetwxImage();
}

CRegardsBitmap* CFiltreEffet::GetBitmap(const bool& source)
{
	return filtreEffet->GetBitmap(source);
}

void CFiltreEffet::GetYUV420P(uint8_t* & y, uint8_t* & u, uint8_t* & v, const int& widthOut, const int& heightOut)
{
	filtreEffet->GetYUV420P(y, u, v, widthOut, heightOut);
}

void CFiltreEffet::GetBitmap(CRegardsBitmap* & bitmap, const bool& source)
{
	filtreEffet->GetBitmap(bitmap, source);
}

CRegardsFloatBitmap* CFiltreEffet::GetFloatBitmap(const bool& source)
{
	return filtreEffet->GetFloatBitmap(source);
}

int CFiltreEffet::HistogramNormalize()
{
	filtreEffet->HistogramNormalize();
	return 0;
}

int CFiltreEffet::HistogramEqualize()
{
	filtreEffet->HistogramEqualize();
	return 0;
}

int CFiltreEffet::BrightnessAndContrastAuto(float clipHistPercent)
{
	filtreEffet->BrightnessAndContrastAuto(clipHistPercent);
	return 0;
}

int CFiltreEffet::LensFlare(const int& iPosX, const int& iPosY, const int& iPuissance, const int& iType,
                            const int& iIntensity, const int& iColor, const int& iColorIntensity)
{
	int value = filtreEffet->LensFlare(iPosX, height - iPosY, iPuissance, iType, iIntensity, iColor, iColorIntensity);
	return value;
}

int CFiltreEffet::SharpenMasking(const float& sharpness)
{
	return filtreEffet->SharpenMasking(sharpness);
}

void CFiltreEffet::Interpolation(const int& widthOut, const int& heightOut, const int& method, int flipH, int flipV,
                                 int angle)
{
#ifdef _CALCU_DIFF_TIME
	LARGE_INTEGER start_time;
	LARGE_INTEGER end_time;
#endif
	filtreEffet->Interpolation(widthOut, heightOut, method, flipH, flipV, angle);

#ifdef _CALCU_DIFF_TIME
#if defined(WIN32) && defined(_DEBUG)
	::QueryPerformanceCounter((LARGE_INTEGER*)&end_time);
	float gm_diffTime = (float)(((double)end_time.QuadPart - (double)start_time.QuadPart) / 1000000);
	wxString data = to_string(gm_diffTime);
	OutputDebugString(L"cuda time : " + data + "\n");

#endif
#endif
}

void CFiltreEffet::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
                                 int flipH, int flipV, int angle)
{
#ifdef _CALCU_DIFF_TIME
	LARGE_INTEGER start_time;
	LARGE_INTEGER end_time;
#endif
	filtreEffet->Interpolation(widthOut, heightOut, rc, method, flipH, flipV, angle);

#ifdef _CALCU_DIFF_TIME
	::QueryPerformanceCounter((LARGE_INTEGER*)&end_time);
	float gm_diffTime = (float)(((double)end_time.QuadPart - (double)start_time.QuadPart) / 1000000);
	wxString data = to_string(gm_diffTime);
	OutputDebugString(L"cuda time : " + data + "\n");

#endif
}

//---------------------------------------------------------------------
//Effet RGB 
//---------------------------------------------------------------------
int CFiltreEffet::RGBFilter(const int& red, const int& green, const int& blue)
{
	return filtreEffet->RGBFilter(red, green, blue);
}

int CFiltreEffet::Posterize(const float& level, const float& gamma)
{
	return filtreEffet->Posterize(level, gamma);
}

int CFiltreEffet::Solarize(const long& threshold)
{
	return filtreEffet->Solarize(threshold);
}

//---------------------------------------------------------------------
//Effet Clouds 
//---------------------------------------------------------------------
int CFiltreEffet::CloudsFilter(const CRgbaquad& color1, const CRgbaquad& color2, const float& amplitude,
                               const float& frequence, const int& octave, const int& intensity)
{
	int value = filtreEffet->CloudsFilter(color1, color2, amplitude, frequence, octave, intensity);
	return value;
}

//---------------------------------------------------------------------
//Effet Swirl 
//---------------------------------------------------------------------
int CFiltreEffet::Swirl(const float& radius, const float& angle)
{
	return filtreEffet->Swirl(radius, angle);
}

//---------------------------------------------------------------------
//Effet Cartoonify Image 
//---------------------------------------------------------------------
int CFiltreEffet::CartoonifyImage(const int& mode)
{
	return filtreEffet->CartoonifyImage(mode);
}

//---------------------------------------------------------------------
//Lumière et Contraste 
//---------------------------------------------------------------------
int CFiltreEffet::BrightnessAndContrast(const double& brightness, const double& contrast)
{
	return filtreEffet->BrightnessAndContrast(brightness, contrast);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffet::NiveauDeGris()
{
	try
	{
		return filtreEffet->NiveauDeGris();
	}
	catch (...)
	{
	}
	return -1;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffet::NoirEtBlanc()
{
	try
	{
		return filtreEffet->NoirEtBlanc();
	}
	catch (...)
	{
	}
	return -1;
}

int CFiltreEffet::VignetteEffect(const double& radius, const double& power)
{
	try
	{
		return filtreEffet->VignetteEffect(radius, power);
	}
	catch (...)
	{
	}
	return -1;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Sepia()
{
	try
	{
		return filtreEffet->Sepia();
	}
	catch (...)
	{
	}
	return -1;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Soften()
{
	try
	{
		return filtreEffet->Soften();
	}
	catch (...)
	{
	}
	return -1;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Blur(const int& radius)
{
	try
	{
		return filtreEffet->Blur(radius);
	}
	catch (...)
	{
	}
	return -1;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::GaussianBlur(const int& radius, const int& boxsize)
{
	try
	{
		return filtreEffet->GaussianBlur(radius, boxsize);
	}
	catch (...)
	{
	}
	return -1;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Emboss()
{
	try
	{
		return filtreEffet->Emboss();
	}
	catch (...)
	{
	}
	return -1;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::SharpenStrong()
{
	try
	{
		return filtreEffet->SharpenStrong();
	}
	catch (...)
	{
	}
	return -1;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Sharpen()
{
	try
	{
		return filtreEffet->Sharpen();
	}
	catch (...)
	{
	}
	return -1;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Erode()
{
	try
	{
		return filtreEffet->Erode();
	}
	catch (...)
	{
	}
	return -1;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Median()
{
	return filtreEffet->Median();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Noise()
{
	return filtreEffet->Noise();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Dilate()
{
	return filtreEffet->Dilate();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Negatif()
{
	return filtreEffet->Negatif();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Contrast(const double& contrast, const uint8_t& offset)
{
	return filtreEffet->Contrast(contrast, offset);
}

int CFiltreEffet::Lightness(const double& factor)
{
	return filtreEffet->Lightness(factor);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::FiltreEdge()
{
	return filtreEffet->FiltreEdge();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::FiltreMosaic()
{
	return filtreEffet->FiltreMosaic();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::FlipVertical()
{
	return filtreEffet->FlipVertical();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::FlipHorizontal()
{
	return filtreEffet->FlipHorizontal();
}

void CFiltreEffet::SetPreview(const bool& value)
{
	filtreEffet->SetPreviewMode(value);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::HQDn3D(const double& LumSpac, const double& ChromSpac, const double& LumTmp, const double& ChromTmp)
{
	int value = filtreEffet->HQDn3D(LumSpac, ChromSpac, LumTmp, ChromTmp);
	return value;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::MotionBlur(const double& radius, const double& sigma, const double& angle)
{
	//return filtreEffet->MotionBlur(radius, sigma, angle);
	int puissance = static_cast<int>((float)(width / 2) * ((float)sigma / 100.0f));
	int value = filtreEffet->MotionBlur(radius, puissance, angle);
	return value;
}


void CFiltreEffet::CalculNewSize(const int32_t& x, const int32_t& y, const double& angle, int& width, int& height)
{
	double dAngle = angle;
	if (dAngle > 90.0 && dAngle < 180.0)
	{
		dAngle = 180.0 - angle;
	}
	else if (dAngle > 180.0 && dAngle < 270.0)
	{
		dAngle = angle - 180.0;
	}
	else if (dAngle > 270.0 && dAngle < 360.0)
	{
		dAngle = 360.0 - angle;
	}
	else if (dAngle == 90.0 || dAngle == 270.0)
	{
		width = y;
		height = x;
		return;
	}
	else if (dAngle == 180.0 || dAngle == 360.0 || dAngle == 0.0)
	{
		width = x;
		height = y;
		return;
	}


	float radians = dAngle * 0.017453292519943;
	float cosine = cos(radians);
	float sine = sin(radians);
	width = x * cosine + y * sine;
	height = x * sine + y * cosine;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::RotateFree(const double& angle)
{
	int widthOut;
	int heightOut;
	CalculNewSize(filtreEffet->GetWidth(), filtreEffet->GetHeight(), angle, widthOut, heightOut);
	return filtreEffet->RotateFree(angle, widthOut, heightOut);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::PhotoFiltre(const CRgbaquad& clValue, const int& intensity)
{
	return filtreEffet->PhotoFiltre(clValue, intensity);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Rotate90()
{
	return filtreEffet->Rotate90();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Rotate270()
{
	return filtreEffet->Rotate270();
}

//----------------------------------------------------------------------------
//Fusion de deux bitmaps
//----------------------------------------------------------------------------
int CFiltreEffet::Fusion(CRegardsBitmap* bitmapSecond, const float& pourcentage)
{
	return filtreEffet->Fusion(bitmapSecond, pourcentage);
}

/*
void CFiltreEffet::SetBitmap(CImageLoadingFormat * bitmap, int numLib)
{
	if(bitmap != nullptr && bitmap->IsOk())
	{
		filename = bitmap->GetFilename();
		width = bitmap->GetWidth();
		height = bitmap->GetHeight();

		if(filtreEffet != filtreEffetCPU)
		{
			filtreEffet->SetBitmap(bitmap);
		}
	}
}

void CFiltreEffet::SetBitmap(CRegardsBitmap * & pBitmap, int numLib)
{
	filename = pBitmap->GetFilename();
	width = pBitmap->GetBitmapWidth();
	height = pBitmap->GetBitmapHeight();


	if(numLib == 0)
	{
		if(filtreEffet != filtreEffetCPU)
		{
			filtreEffet->SetBitmap(pBitmap);
			filtreEffetCPU->SetBitmap(pBitmap);
		}
		else
		{
			filtreEffetCPU->SetBitmap(pBitmap);
		}
	}
	else if(numLib == LIBCPU)
	{
		filtreEffetCPU->SetBitmap(pBitmap);
	}
	else
	{
		filtreEffet->SetBitmap(pBitmap);
	}
}
*/
