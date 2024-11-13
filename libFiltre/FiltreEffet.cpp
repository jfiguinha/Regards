#include <header.h>
#include "FiltreEffet.h"
#include "FiltreEffetCPU.h"
#include "OpenCLEffect.h"
#include <OpenCLParameter.h>
#include <RegardsConfigParam.h>
#include <FilterData.h>
#include "WaveFilter.h"
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <ImageLoadingFormat.h>
#include <config_id.h>
#include <ParamInit.h>

#ifndef __APPLE__
#include "CudaEffect.h"
#endif

extern float value[256];

using namespace Regards::FiltreEffet;
using namespace std;


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

int CFiltreEffet::SuperResolutionNCNN()
{
	return filtreEffet->SuperResolutionNCNN();
}

int CFiltreEffet::Inpaint(const cv::Mat &mask, int algorithm)
{
    return filtreEffet->Inpaint(mask, algorithm);
}

int CFiltreEffet::Colorization()
{
	return filtreEffet->Colorization();
}

int CFiltreEffet::GetHeight()
{
	return filtreEffet->GetHeight();
}

bool CFiltreEffet::StabilizeVideo(Regards::OpenCV::COpenCVStabilization* stabilization)
{
	return filtreEffet->StabilizeVideo(stabilization);
}

int CFiltreEffet::DetailEnhance(const double& sigma_s, const double& sigma_r)
{
	return filtreEffet->DetailEnhance(sigma_s, sigma_r / 100.0f);
}

int CFiltreEffet::EdgePreservingFilter(const int& flags, const double& sigma_s, const double& sigma_r)
{
	return filtreEffet->EdgePreservingFilter(flags, sigma_s, sigma_r / 100.0f);
}

int CFiltreEffet::PencilSketch(const double& sigma_s, const double& sigma_r, const double& shade_factor)
{
	return filtreEffet->PencilSketch(sigma_s, sigma_r / 100.0f, shade_factor / 100.0f);
}

int CFiltreEffet::Stylization(const double& sigma_s, const double& sigma_r)
{
	return filtreEffet->Stylization(sigma_s, sigma_r /100.0f);
}


int CFiltreEffet::BokehEffect(const int& radius, const int& boxsize, const int& nbFace, const wxRect& listFace)
{
	filtreEffet->BokehEffect(radius, boxsize, nbFace, listFace);
	return 0;
}

cv::UMat CFiltreEffet::GetUMat()
{
	return filtreEffet->GetUMat();
}

cv::cuda::GpuMat CFiltreEffet::GetGpuMat()
{
	return filtreEffet->GetGpuMat();
}

int CFiltreEffet::LensDistortionFilter(const int& size)
{
	filtreEffet->LensDistortionFilter(size);
	return 0;
}

int CFiltreEffet::WaveFilter(int x, int y, short height, int scale, int radius)
{
	filtreEffet->WaveFilter(x, y, height, scale, radius);
	return 0;
}

int CFiltreEffet::BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP)
{
	return filtreEffet->BilateralFilter(fSize, sigmaX, sigmaP);
}


int CFiltreEffet::NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize,
                                const int& searchWindowSize)
{
	return filtreEffet->NlmeansFilter(h, hColor, templateWindowSize, searchWindowSize);
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

CFiltreEffet::CFiltreEffet(const CRgbaquad& backColor, const bool& useOpenCL, const bool& useCuda, CImageLoadingFormat* bitmap)
{
	filtreEffet = nullptr;
	this->backColor = backColor;
	this->numLib = LIBCPU;

	filename = bitmap->GetFilename();
	width = bitmap->GetWidth();
	height = bitmap->GetHeight();


	CRegardsConfigParam* regardsParam = CParamInit::getInstance();

	bool local_useOpenCL = false;

#ifndef __APPLE__

	//(regardsParam->GetIsUseCuda()
	//bool useCuda = regardsParam->GetIsUseCuda();
	bool supportCuda = regardsParam->GetIsCudaSupport();
	if (useCuda && supportCuda)
	{
		filtreEffet = new CCudaEffect(backColor, bitmap);
		this->numLib = LIBCUDA;
	}
	else
		local_useOpenCL = regardsParam->GetIsOpenCLSupport();
#else

	local_useOpenCL = regardsParam->GetIsOpenCLSupport();

#endif

	
	

	if (local_useOpenCL && useOpenCL)
	{
		if (cv::ocl::haveOpenCL())
		{
			filtreEffet = new COpenCLEffect(backColor, bitmap);
			this->numLib = LIBOPENCL;
		}
	}

	if (this->numLib == LIBCPU)
	{
		filtreEffet = new CFiltreEffetCPU(backColor, bitmap);
	}
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

int CFiltreEffet::RedEye()
{
	filtreEffet->RedEye();
	return 0;
}

wxImage CFiltreEffet::GetwxImage()
{
	return filtreEffet->GetwxImage();
}

cv::Mat CFiltreEffet::GetBitmap(const bool& source)
{
	return filtreEffet->GetBitmap(source);
}

cv::Mat CFiltreEffet::GetMat()
{
	return filtreEffet->GetMat();
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
	int value = filtreEffet->LensFlare(iPosX, iPosY, iPuissance, iType, iIntensity, iColor, iColorIntensity);
	return value;
}

int CFiltreEffet::SharpenMasking(const float& sharpness)
{
	return filtreEffet->SharpenMasking(sharpness);
}


void CFiltreEffet::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
                                 int flipH, int flipV, int angle, int ratio)
{
#ifdef _CALCU_DIFF_TIME
	LARGE_INTEGER start_time;
	LARGE_INTEGER end_time;
#endif
	filtreEffet->Interpolation(widthOut, heightOut, rc, method, flipH, flipV, angle, ratio);

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
int CFiltreEffet::FiltreEdge()
{
	return filtreEffet->FiltreEdge();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::FiltreMosaic(const int& size)
{
	return filtreEffet->FiltreMosaic(size);
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
int CFiltreEffet::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault)
{
	int value = filtreEffet->HQDn3D(LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
	return value;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::MotionBlur(const double& radius, const double& sigma, const double& angle)
{
	//return filtreEffet->MotionBlur(radius, sigma, angle);
	int puissance = static_cast<int>(static_cast<float>(width / 2) * (static_cast<float>(sigma) / 100.0f));
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
	/*
	if (angle == 180.0)
	{
		return filtreEffet->Rotate180();
	}
	else if (angle == 90.0)
	{
		return filtreEffet->Rotate90();
	}
	else if (angle == 270.0)
	{
		return filtreEffet->Rotate270();
	}
	*/
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
int CFiltreEffet::Fusion(cv::Mat& bitmapSecond, const float& pourcentage)
{
	return filtreEffet->Fusion(bitmapSecond, pourcentage);
}
