#include "FiltreEffetCPU.h"
#include "InterpolationBicubic.h"
#include "InterpolationBilinear.h"
#include "Interpolation.h"
#include "PerlinNoise.h"
#include "LensFlare.h"
#include "MotionBlur.h"
#include "Filtre.h"
#include "Rotation.h"
#include "Histogramme.h"
#include "RedEye.h"
using namespace Regards::FiltreEffet;

CFiltreEffetCPU::CFiltreEffetCPU(CRegardsBitmap * pBitmap, const CRgbaquad &backColor)
	: IFiltreEffet(pBitmap, backColor)
{
	this->pBitmap = pBitmap;
	this->backColor = backColor;
}

int CFiltreEffetCPU::RedEye(const wxRect& rSelectionBox)
{
	CRedEye redeye;
	return redeye.RemoveRedEye(pBitmap, backColor, rSelectionBox);
}

CFiltreEffetCPU::~CFiltreEffetCPU()
{
}

wxImage CFiltreEffetCPU::InterpolationBicubic(const wxImage & imageSrc, const int &widthOut, const int &heightOut)
{
	return imageSrc.ResampleBicubic(widthOut, heightOut);
}

int CFiltreEffetCPU::HistogramLog()
{
	return CHistogramme::HistogramLog(pBitmap);
}

int CFiltreEffetCPU::HistogramNormalize()
{
	return CHistogramme::HistogramNormalize(pBitmap);
}

int CFiltreEffetCPU::HistogramEqualize()
{
	return CHistogramme::HistogramEqualize(pBitmap);
}

int CFiltreEffetCPU::LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity)
{
	if (pBitmap != nullptr)
	{
		CLensFlare * filtre = new CLensFlare();
		filtre->LensFlare(pBitmap, iPosX, iPosY, iPuissance, iType, iIntensity, iColor, iColorIntensity);
		delete filtre;

	}
	return 0;
}

int CFiltreEffetCPU::InterpolationBicubic(CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle)
{
	CInterpolationBicubic interpolation;
	interpolation.Execute(pBitmap, bitmapOut, flipH, flipV, angle);
	return 0;
}

int CFiltreEffetCPU::InterpolationBicubic(CRegardsBitmap * & bitmapOut)
{
	CInterpolationBicubic interpolation;
	interpolation.Execute(pBitmap, bitmapOut);
	return 0;
}

int CFiltreEffetCPU::InterpolationBicubic(CRegardsBitmap * & bitmapOut, const wxRect &rc)
{
	CInterpolationBicubic interpolation;
	interpolation.Execute(pBitmap, bitmapOut, rc);
	return 0;
}

int CFiltreEffetCPU::InterpolationBilinear(CRegardsBitmap * & bitmapOut)
{
	CInterpolationBilinear interpolation;
	interpolation.Execute(pBitmap, bitmapOut);
	return 0;
}

int CFiltreEffetCPU::InterpolationBilinear(CRegardsBitmap * & bitmapOut, const wxRect &rc)
{
	CInterpolationBicubic interpolation;
	interpolation.Execute(pBitmap, bitmapOut, rc);
	return 0;
}

int CFiltreEffetCPU::InterpolationFast(CRegardsBitmap * & bitmapOut)
{
	CInterpolation interpolation;
	interpolation.Execute(pBitmap, bitmapOut);
	return 0;
}

int CFiltreEffetCPU::InterpolationFast(CRegardsBitmap * & bitmapOut, const wxRect &rc)
{
	CInterpolationBicubic interpolation;
	interpolation.Execute(pBitmap, bitmapOut, rc);
	return 0;
}

//---------------------------------------------------------------------
//Effet CRgbaquad 
//---------------------------------------------------------------------
int CFiltreEffetCPU::RGBFilter(const int &red, const int &green, const int &blue)
{
	if (pBitmap != nullptr)
	{
		CRgbFiltre * filtre = new CRgbFiltre(red, green, blue);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;

	}
	return 0;
}

int CFiltreEffetCPU::Solarize(const long &threshold)
{
	if (pBitmap != nullptr)
	{
		CSolarize * filtre = new CSolarize((int)threshold);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;

	}
	return 0;
}

int CFiltreEffetCPU::Posterize(const float &level, const float &gamma)
{
	if (pBitmap != nullptr)
	{
		CPosterize * filtre = new CPosterize(level, gamma);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;

	}
	return 0;
}
//---------------------------------------------------------------------
//Effet Clouds 
//---------------------------------------------------------------------
int CFiltreEffetCPU::CloudsFilter(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave)
{
	if (pBitmap != nullptr)
	{
		CPerlinNoise * m_perlinNoise = new Regards::FiltreEffet::CPerlinNoise();
		m_perlinNoise->Clouds(pBitmap, color1, color2, frequence, amplitude, octave);
		delete m_perlinNoise;
	}
	return 0;
}

//---------------------------------------------------------------------
//Effet Swirl 
//---------------------------------------------------------------------
int CFiltreEffetCPU::Swirl(const float &radius, const float &angle)
{
	if (pBitmap != nullptr)
	{
		CSwirl * filtre = new CSwirl(angle, radius);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//---------------------------------------------------------------------
//Lumière et Contraste 
//---------------------------------------------------------------------
int CFiltreEffetCPU::BrightnessAndContrast(const double &brightness, const double &contrast)
{
	if (pBitmap != nullptr)
	{
		double offset;

		if (contrast == 0)
			Contrast(1, 128);
		else if (contrast > 0)
		{
			offset = contrast;
			offset /= 100;
			offset = 1 + offset;
			Contrast(offset, 128);
		}
		else if (contrast < 0)
		{
			offset = -contrast;
			offset /= 100;
			offset = 1 - offset;
			//lValue = 128 + lContrast;
			Contrast(offset, 128);
		}

		Lightness(brightness);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffetCPU::NiveauDeGris()
{
	if (pBitmap != nullptr)
	{
		CGrayScale * filtre = new CGrayScale();
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffetCPU::NoirEtBlanc()
{
	if (pBitmap != nullptr)
	{
		CBlackAndWhite * filtre = new CBlackAndWhite();
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Sepia()
{
	if (pBitmap != nullptr)
	{
		CSepiaFiltre * filtre = new CSepiaFiltre();
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Soften()
{
	if (pBitmap != nullptr)
	{
		short kernel[] = { 1, 1, 1, 1, 8, 1, 1, 1, 1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 16, 0);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Blur()
{
	if (pBitmap != nullptr)
	{
		short kernel[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 9, 0);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::GaussianBlur()
{
	if (pBitmap != nullptr)
	{
		short kernel[9] = { 5, 15, 31, 49, 56, 49, 31, 15, 5 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 9, 256, 0);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Emboss()
{
	if (pBitmap != nullptr)
	{
		short kernel[] = { -1, 0, 0, 0, 0, 0, 0, 0, 1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 1, 127);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::SharpenStrong()
{
	if (pBitmap != nullptr)
	{
		short kernel[] = { -1, -1, -1, -1, 9, -1, -1, -1, -1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 1, 0);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Sharpen()
{
	if (pBitmap != nullptr)
	{
		short kernel[] = { -1, -1, -1, -1, 16, -1, -1, -1, -1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 8, 0);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Erode()
{
	if (pBitmap != nullptr)
	{
		CErode * filtre = new CErode();
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Median()
{
	if (pBitmap != nullptr)
	{
		CMedian * filtre = new CMedian();
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Noise()
{
	if (pBitmap != nullptr)
	{
		CNoise * filtre = new CNoise();
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Dilate()
{
	if (pBitmap != nullptr)
	{
		CDilate * filtre = new CDilate();
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Negatif()
{
	if (pBitmap != nullptr)
	{
		CNegatif * filtre = new CNegatif();
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Contrast(const double &contrast,const uint8_t & offset)
{
	if (pBitmap != nullptr)
	{
		CContrast * filtre = new CContrast(contrast, offset);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

int CFiltreEffetCPU::Lightness(const double &factor)
{
	if (pBitmap != nullptr)
	{
		CLightness * filtre = new CLightness(factor);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FiltreEdge()
{
	if (pBitmap != nullptr)
	{
		short kernel[] = { -1, -1, -1, -1, 8, -1, -1, -1, -1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 1, 0);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FiltreMosaic()
{
	if (pBitmap != nullptr)
	{
		CMosaic * filtre = new CMosaic(5);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}


	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FlipVertical()
{
	if (pBitmap != nullptr)
	{
		pBitmap->VertFlipBuf();
	}
	return 0;
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FlipHorizontal()
{
	if (pBitmap != nullptr)
	{
		pBitmap->HorzFlipBuf();
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::MotionBlur(const double &radius, const double &sigma, const double &angle)
{
	if (pBitmap != nullptr)
	{
		CMotionBlur * filtre = new CMotionBlur();
		filtre->MotionBlur(pBitmap, radius, sigma, angle);
		delete filtre;

	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::RotateFree(const double &angle, const int &widthOut, const int &heightOut)
{
	if (pBitmap != nullptr)
	{
		CRotation * m_cEffet = new CRotation();
		m_cEffet->Rotate(pBitmap, angle, backColor);
		delete m_cEffet;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::PhotoFiltre(const CRgbaquad &clValue, const int &intensity)
{
	if (pBitmap != nullptr)
	{
		CPhotoFiltre * filtre = new CPhotoFiltre(clValue, intensity);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Rotate90()
{
	if (pBitmap != nullptr)
	{
		CRotation * m_cEffet = new CRotation();
		m_cEffet->Rotate(pBitmap, 90.0f, backColor);
		delete m_cEffet;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Rotate270()
{
	if (pBitmap != nullptr)
	{
		CRotation * m_cEffet = new CRotation();
		m_cEffet->Rotate(pBitmap, 270.0f, backColor);
		delete m_cEffet;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Resize(const int &imageWidth, const int &imageHeight, const int &interpolation)
{
	CRegardsBitmap * scaleBitmap = new CRegardsBitmap(imageWidth, imageHeight);

	CInterpolationBicubic imageScale;
	imageScale.Execute(pBitmap, scaleBitmap);
	pBitmap->SetBitmap(scaleBitmap->GetPtBitmap(), imageWidth, imageHeight);

	delete scaleBitmap;

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Fusion(CRegardsBitmap * bitmapSecond, const float &pourcentage)
{
	if (pBitmap != nullptr)
	{
		pBitmap->FusionBitmap(bitmapSecond, pourcentage);
	}
	return 0;
}