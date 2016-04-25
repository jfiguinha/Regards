#include "FiltreEffet.h"
#include "FiltreEffetCPU.h"
#include "OpenCLEffect.h"
#include "InterpolationBicubic.h"
#include <libResource.h>
#include <algorithm>
#include "Rotate.h"
#include "RedEye.h"
#include "effect.h"
#include <RgbEffectParameter.h>
#include <BrightAndContrastEffectParameter.h>
#include <CloudsEffectParameter.h>
#include <FreeRotateEffectParameter.h>
#include <MotionBlurEffectParameter.h>
#include <PhotoFiltreEffectParameter.h>
#include <PosterisationEffectParameter.h>
#include <RgbEffectParameter.h>
#include <SolarisationEffectParameter.h>
#include <LensFlareEffectParameter.h>
#include <SwirlEffectParameter.h>
#include <FilterData.h>
#include "Histogramme.h"
#if defined(__WXMSW__)
#include "../include/config_id.h"
#else
#include <config_id.h>
#endif
using namespace Regards::FiltreEffet;
using namespace std;

void CFiltreEffet::SetLib(const int &lib)
{
	numLib = lib;
}


int CFiltreEffet::RenderEffect(const int &numEffect, CEffectParameter * effectParameter)
{
    switch (numEffect)
    {
        case IDM_FILTRE_SOFTEN:
            Soften();
            break;
            
        case IDM_HISTOGRAMNORMALIZE:
            HistogramNormalize();
            break;
            
        case IDM_HISTOGRAMEQUALIZE:
            HistogramEqualize();
            break;
            
        case IDM_HISTOGRAMLOG:
            HistogramLog();
            break;
            
        case IDM_FILTRE_FLOU:
            Blur();
            break;
            
        case IDM_AJUSTEMENT_SOLARISATION:
        {
            if (effectParameter != nullptr)
            {
                CSolarisationEffectParameter * solarisationEffectParameter = (CSolarisationEffectParameter *)effectParameter;
                Solarize(solarisationEffectParameter->threshold);
            }
        }
            break;
            
        case IDM_FILTRE_FLOUGAUSSIEN:
            GaussianBlur();
            break;
            
        case IDM_FILTREANTIBRUIT:
            Median();
            break;
            
        case IDM_FILTRE_MOTIONBLUR:
        {
            if (effectParameter != nullptr)
            {
                CMotionBlurEffectParameter * motionblurEffectParameter = (CMotionBlurEffectParameter *)effectParameter;
                MotionBlur(motionblurEffectParameter->radius, motionblurEffectParameter->sigma, motionblurEffectParameter->angle);
            }
            break;
        }
            break;
        case IDM_ROTATE_FREE:
        {
            if (effectParameter != nullptr)
            {
                CFreeRotateEffectParameter * freeRotate = (CFreeRotateEffectParameter *)effectParameter;
                RotateFree(freeRotate->angle);
            }
        }
            break;
            
        case IDM_IMAGE_LIGHTCONTRAST:
        {
            if (effectParameter != nullptr)
            {
                CBrightAndContrastEffectParameter  * brightAndContrast = (CBrightAndContrastEffectParameter *)effectParameter;
                BrightnessAndContrast(brightAndContrast->brightness, brightAndContrast->contrast);
            }
        }
            break;
            
        case ID_AJUSTEMENT_PHOTOFILTRE:
        {
            if (effectParameter != nullptr)
            {
                CPhotoFiltreEffectParameter * photoFiltreParameter = (CPhotoFiltreEffectParameter *)effectParameter;
                PhotoFiltre(CRgbaquad(photoFiltreParameter->red, photoFiltreParameter->green, photoFiltreParameter->blue), photoFiltreParameter->intensity);
            }
        }
            break;
        case ID_AJUSTEMENT_POSTERISATION:
        {
            if (effectParameter != nullptr)
            {
                CPosterisationEffectParameter * posterisationFiltreParameter = (CPosterisationEffectParameter *)effectParameter;
                Posterize(posterisationFiltreParameter->level, posterisationFiltreParameter->gamma);
            }
        }
            break;
        case IDM_COLOR_BALANCE:
        {
            if (effectParameter != nullptr)
            {
                CRgbEffectParameter * rgbParameter = (CRgbEffectParameter *)effectParameter;
                RGBFilter(rgbParameter->red, rgbParameter->green, rgbParameter->blue);
            }
        }
            break;
        case IDM_FILTRE_SWIRL:
        {
            if (effectParameter != nullptr)
            {
                CSwirlEffectParameter * swirlParameter = (CSwirlEffectParameter *)effectParameter;
                Swirl(swirlParameter->radius, swirlParameter->angle);
            }
        }
            break;
            
        case IDM_FILTRE_CLOUDS:
        {
            if (effectParameter != nullptr)
            {
                CCloudsEffectParameter * cloudsParameter = (CCloudsEffectParameter *)effectParameter;
                CloudsFilter(cloudsParameter->colorFront, cloudsParameter->colorBack, cloudsParameter->amplitude, cloudsParameter->frequence, cloudsParameter->octave);
            }
        }
            break;
        case IDM_FILTRE_ERODE:
            Erode();
            break;
            
        case IDM_FILTRE_DILATE:
            Dilate();
            break;
            
        case IDM_FILTRE_SHARPEN:
            Sharpen();
            break;
            
        case IDM_FILTRE_SHARPENSTRONG:
            SharpenStrong();
            break;
            
        case IDM_FILTRENOISE:
            Noise();
            break;
            
        case IDM_FILTRE_MOSAIQUE:
            FiltreMosaic();
            break;
            
        case IDM_FILTRE_EMBOSS:
            Emboss();
            break;
            
        case IDM_GREY_LEVEL:
            NiveauDeGris();
            break;
            
        case IDM_IMAGE_SEPIA:
            Sepia();
            break;
            
        case IDM_BLACKANDWHITE:
            NoirEtBlanc();
            break;
            
        case IDM_FILTRE_EDGE:
            FiltreEdge();
            break;
            
        case IDM_NEGATIF:
            Negatif();
            break;
    }
    return 0;
}

CFiltreEffet::CFiltreEffet(CRegardsBitmap * pBitmap, const CRgbaquad &backColor, const int &numLib)
{
    this->backColor = backColor;
    this->numLib = numLib;
    this->pBitmap = pBitmap;
	filtreEffetCPU = new CFiltreEffetCPU(pBitmap, backColor);

	switch (numLib)
	{
		case LIBOPENCL:
			filtreEffet = new COpenCLEffect(pBitmap, backColor);
			break;

		case LIBCPU:
			filtreEffet = filtreEffetCPU;
			break;
	}	
}


CFiltreEffet::~CFiltreEffet()
{
	delete(filtreEffetCPU);
	if (numLib != LIBCPU)
		delete(filtreEffet);
}


int CFiltreEffet::RedEye(const wxRect& rSelectionBox)
{
	if (filtreEffet->RedEye(rSelectionBox) == -1)
		filtreEffetCPU->RedEye(rSelectionBox);

	return 0;
}

uint8_t * CFiltreEffet::BGRA32ToRGB24()
{
	int width = pBitmap->GetBitmapWidth();
	int height = pBitmap->GetBitmapHeight();
	int widthSrcSize = width * 4;
	unsigned char * data = pBitmap->GetPtBitmap();
	uint8_t * dataOut = new uint8_t[width*height * 3];
	int posData = 0;
	int posDataOut = 0;

	for (int y = 0; y < height; y++)
	{
		posData = ((height - y) * widthSrcSize) - widthSrcSize;
		for (int x = 0; x < width; x++)
		{
			dataOut[posDataOut] = data[posData + 2];
			dataOut[posDataOut + 1] = data[posData + 1];
			dataOut[posDataOut + 2] = data[posData];
			posData += 4;
			posDataOut += 3;
		}
	}

	return dataOut;
}

int CFiltreEffet::HistogramLog()
{
	if (filtreEffet->HistogramLog() == -1)
		filtreEffetCPU->HistogramLog();

	return 0;
}

int CFiltreEffet::HistogramNormalize()
{
	if(filtreEffet->HistogramNormalize() == -1)
		filtreEffetCPU->HistogramNormalize();

	return 0;
}

int CFiltreEffet::HistogramEqualize()
{
	if (filtreEffet->HistogramEqualize() == -1)
		filtreEffetCPU->HistogramEqualize();
	return 0;
}

int CFiltreEffet::LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity)
{
	int puissance = (int)((float)(pBitmap->GetBitmapWidth() / 2) * ((float)iPuissance / 100.0f));
	int intensity = (int)((float)(pBitmap->GetBitmapWidth() / 2) * ((float)iIntensity / 100.0f));

	int value = filtreEffet->LensFlare(iPosX, iPosY, puissance, iType, intensity, iColor, iColorIntensity);
	if (value == -1)
	{
		value = filtreEffetCPU->LensFlare(iPosX, iPosY, puissance, iType, intensity, iColor, iColorIntensity);
	}
	return value;
}

int CFiltreEffet::SharpenMasking(const int &sharpness)
{
	return filtreEffet->SharpenMasking(sharpness);
}

int CFiltreEffet::InterpolationBicubic(CRegardsBitmap * & bitmapOut)
{
	return filtreEffet->InterpolationBicubic(bitmapOut);
}

int CFiltreEffet::InterpolationBicubic(CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle)
{
	return filtreEffet->InterpolationBicubic(bitmapOut, flipH, flipV, angle);
}

int CFiltreEffet::InterpolationBicubic(CRegardsBitmap * & bitmapOut, const wxRect &rc)
{
	return filtreEffet->InterpolationBicubic(bitmapOut, rc);
}

int CFiltreEffet::InterpolationBilinear(CRegardsBitmap * & bitmapOut)
{
	return filtreEffet->InterpolationBilinear(bitmapOut);
}

int CFiltreEffet::InterpolationBilinear(CRegardsBitmap * & bitmapOut, const wxRect &rc)
{
	return filtreEffet->InterpolationBilinear(bitmapOut, rc);
}

int CFiltreEffet::InterpolationFast(CRegardsBitmap * & bitmapOut)
{
	return filtreEffet->InterpolationFast(bitmapOut);
}

int CFiltreEffet::InterpolationFast(CRegardsBitmap * & bitmapOut, const wxRect &rc)
{
	return filtreEffet->InterpolationFast(bitmapOut, rc);
}

wxImage CFiltreEffet::InterpolationBicubic(const wxImage & imageSrc, const int &widthOut, const int &heightOut)
{
	return filtreEffet->InterpolationBicubic(imageSrc, widthOut, heightOut);
}

//---------------------------------------------------------------------
//Effet RGB 
//---------------------------------------------------------------------
int CFiltreEffet::RGBFilter(const int &red, const int &green, const int &blue)
{
	return filtreEffet->RGBFilter(red, green, blue);
}

//Color Conversion
int CFiltreEffet::RGB24ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size)
{
	return filtreEffet->RGB24ToRGB32(buffer, width, height, bitmapOut, size);
}

int CFiltreEffet::NV12ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size)
{
	return filtreEffet->NV12ToRGB32(buffer, width, height, bitmapOut, size);
}

int CFiltreEffet::YUV420ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size)
{
	return filtreEffet->YUV420ToRGB32(buffer, width, height, bitmapOut, size);
}

int CFiltreEffet::InterpolationBicubicNV12ToRGB32(uint8_t * buffer, const int &width, const int &height, const int &rectWidth, const int &rectHeight, CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle, const int &size)
{
	return filtreEffet->InterpolationBicubicNV12ToRGB32(buffer, width, height, rectWidth, rectHeight, bitmapOut, flipH, flipV, angle, size);
}

int CFiltreEffet::InterpolationBicubicRGB32Video(CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle)
{
	return filtreEffet->InterpolationBicubicRGB32Video(bitmapOut, flipH, flipV, angle);
}

int CFiltreEffet::Posterize(const float &level, const float &gamma)
{
	return filtreEffet->Posterize(level, gamma);
}

int CFiltreEffet::Solarize(const long &threshold)
{
	return filtreEffet->Solarize(threshold);
}
//---------------------------------------------------------------------
//Effet Clouds 
//---------------------------------------------------------------------
int CFiltreEffet::CloudsFilter(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave)
{
	int value = filtreEffet->CloudsFilter(color1, color2, amplitude, frequence, octave);
	if (value == -1)
	{
		value = filtreEffetCPU->CloudsFilter(color1, color2, amplitude, frequence, octave);
	}
	return value;
}

//---------------------------------------------------------------------
//Effet Swirl 
//---------------------------------------------------------------------
int CFiltreEffet::Swirl(const float &radius, const float &angle)
{
	return filtreEffet->Swirl(radius, angle);
}

//---------------------------------------------------------------------
//Lumière et Contraste 
//---------------------------------------------------------------------
int CFiltreEffet::BrightnessAndContrast(const double &brightness, const double &contrast)
{
	return filtreEffet->BrightnessAndContrast(brightness, contrast);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffet::NiveauDeGris()
{
	return filtreEffet->NiveauDeGris();
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffet::NoirEtBlanc()
{
	return filtreEffet->NoirEtBlanc();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Sepia()
{
	return filtreEffet->Sepia();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Soften()
{
	return filtreEffet->Soften();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Blur()
{
	return filtreEffet->Blur();
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::GaussianBlur()
{
	return filtreEffet->GaussianBlur();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Emboss()
{
	return filtreEffet->Emboss();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::SharpenStrong()
{
	return filtreEffet->SharpenStrong();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Sharpen()
{
	return filtreEffet->Sharpen();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::Erode()
{
	return filtreEffet->Erode();
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
int CFiltreEffet::Contrast(const double &contrast,const uint8_t & offset)
{
	return filtreEffet->Contrast(contrast, offset);
}

int CFiltreEffet::Lightness(const double &factor)
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

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::MotionBlur(const double &radius, const double &sigma, const double &angle)
{
	//return filtreEffet->MotionBlur(radius, sigma, angle);
	int puissance = (int)((float)(pBitmap->GetBitmapWidth() / 2) * ((float)sigma / 100.0f));
	int value = filtreEffet->MotionBlur(radius, puissance, angle);
	if (value == -1)
	{
		value = filtreEffetCPU->MotionBlur(radius, puissance, angle);
	}
	return value;
}


void CFiltreEffet::CalculNewSize(const int32_t &x, const int32_t &y, const double &angle, int &width, int &height)
{
	double dAngle = angle;
	if (dAngle > 90.0 && dAngle < 180.0)
	{
		dAngle = 180.0 - angle;
	}
	else if (dAngle > 180.0  && dAngle < 270.0)
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
int CFiltreEffet::RotateFree(const double &angle)
{
	int widthOut;
	int heightOut;
	CalculNewSize(pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight(), angle, widthOut, heightOut);
	return filtreEffet->RotateFree(angle, widthOut, heightOut);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffet::PhotoFiltre(const CRgbaquad &clValue, const int &intensity)
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
//
//----------------------------------------------------------------------------
int CFiltreEffet::Resize(const int &imageWidth, const int &imageHeight, const int &interpolation)
{
	return filtreEffet->Resize(imageWidth, imageHeight, interpolation);
}

//----------------------------------------------------------------------------
//Fusion de deux bitmaps
//----------------------------------------------------------------------------
int CFiltreEffet::Fusion(CRegardsBitmap * bitmapSecond, const float &pourcentage)
{
	return filtreEffet->Fusion(bitmapSecond, pourcentage);
}