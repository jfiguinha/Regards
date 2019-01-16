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
#include "GaussianBlur.h"
#include "InterpolationFilters.h"
#include "Wave.h"
#include "clahe.h"
#include <FilterData.h>
#include <ImageLoadingFormat.h>
#include <RegardsBitmap.h>
using namespace Regards::FiltreEffet;

CFiltreEffetCPU::CFiltreEffetCPU(const CRgbaquad &backColor, CImageLoadingFormat * bitmap)
	: IFiltreEffet(backColor)
{
	this->backColor = backColor;
	bitmapOut = nullptr;
	pBitmap = bitmap->GetRegardsBitmap();
}

void CFiltreEffetCPU::SetBitmap(CImageLoadingFormat * bitmap)
{
    if(pBitmap != nullptr)
        delete pBitmap;
    pBitmap = bitmap->GetRegardsBitmap();   
}

int CFiltreEffetCPU::RedEye(const wxRect& rSelectionBox)
{
	CRedEye redeye;
	return redeye.RemoveRedEye(pBitmap, backColor, rSelectionBox);
}

CRegardsFloatBitmap * CFiltreEffetCPU::GetFloatBitmap(const bool &source)
{
    CImageLoadingFormat imageLoading(false);
    imageLoading.SetPicture(pBitmap);   
    return imageLoading.GetFloatBitmap(true);
}

int CFiltreEffetCPU::WaveFilter(int x, int y, short height, int scale, int radius)
{
	if (pBitmap != nullptr)
	{
		CWaveFilter * waveFilter = new  CWaveFilter();
		waveFilter->ProcessEffect(pBitmap, x, y, height, scale, radius);
		delete waveFilter;
		return 0;
	}
	return -1;
}

int CFiltreEffetCPU::ClaheFilter(int nBins, float clipLevel, int windowSize)
{
	if (pBitmap != nullptr)
	{
		CClahe * clahe = new Regards::FiltreEffet::CClahe();
		CRegardsBitmap * out = clahe->ClaheFilter(pBitmap, nBins, clipLevel, windowSize);
		pBitmap->SetBitmap(out->GetPtBitmap(), out->GetBitmapWidth(), out->GetBitmapHeight());
		delete clahe;
		delete out;
		return 0;
	}
	return -1;
}

int CFiltreEffetCPU::BilateralFilter(int fSize,  float sigmaX, float sigmaP)
{
	if (pBitmap != nullptr)
	{
		CBilateral * bilateral = new Regards::FiltreEffet::CBilateral(fSize, sigmaX, sigmaP);
		bilateral->SetParameter(pBitmap, backColor);
		bilateral->Compute();
		delete bilateral;
		return 0;
	}
	return -1;
}

int CFiltreEffetCPU::NlmeansFilter(int fsize, int bsize, float sigma)
{
	if (pBitmap != nullptr)
	{
		CNlmeans * nlmeans = new Regards::FiltreEffet::CNlmeans(fsize, bsize, sigma);
		nlmeans->SetParameter(pBitmap, backColor);
		nlmeans->Compute();
		delete nlmeans;
		return 0;
	}
	return -1;
}


int CFiltreEffetCPU::SharpenMasking(const float &sharpness)
{
	if (pBitmap != nullptr)
	{
		CSharpenMasking * sharpenMasking = new Regards::FiltreEffet::CSharpenMasking(sharpness);
		sharpenMasking->SetParameter(pBitmap, backColor);
		sharpenMasking->Compute();
		delete sharpenMasking;
		return 0;
	}
	return -1;
}

CFiltreEffetCPU::~CFiltreEffetCPU()
{
	if(bitmapOut != nullptr)
		delete bitmapOut;
	bitmapOut = nullptr;
    
    if(pBitmap != nullptr)
        delete pBitmap;
    pBitmap = nullptr;
}

wxImage CFiltreEffetCPU::GetwxImage()
{
	int width = 0;
	int height = 0;
	unsigned char * data = nullptr;
	if(bitmapOut != nullptr)
	{
		width = bitmapOut->GetBitmapWidth();
		height = bitmapOut->GetBitmapHeight();
		data = bitmapOut->GetPtBitmap();
	}
	else
	{
		width = pBitmap->GetBitmapWidth();
		height = pBitmap->GetBitmapHeight();
		data = pBitmap->GetPtBitmap();
	}
	wxImage anImage(width, height, false);
	unsigned char * dataOut = anImage.GetData();
	int size = width*height;
	//pBitmap->VertFlipBuf();

	if (data != nullptr)
	{
#pragma omp parallel for
		for (int i = 0; i < size; ++i)
		{
			int y = i / width;
			int x = i - (y * width);
			int calcul = (height - y - 1) * width + x;
			dataOut[i * 3] = data[(calcul << 2) + 2]; // R
			dataOut[i * 3 + 1] = data[(calcul << 2) + 1]; // G
			dataOut[i * 3 + 2] = data[(calcul << 2)]; // B
		}

	}

	return anImage;
}

wxImage CFiltreEffetCPU::GetwxImage(CRegardsBitmap * bitmap)
{
	int width = bitmap->GetBitmapWidth();
	int height = bitmap->GetBitmapHeight();
	unsigned char * data = bitmap->GetPtBitmap();
	wxImage anImage(width, height, false);
	unsigned char * dataOut = anImage.GetData();
	int size = width*height;
	//pBitmap->VertFlipBuf();

	if (data != nullptr)
	{
#pragma omp parallel for
		for (int i = 0; i < size; ++i)
		{
			int y = i / width;
			int x = i - (y * width);
			int pos = i * 3;
			int calcul = ((height - y - 1) * width + x) << 2;
			//int calcul = y * width + x;
			dataOut[pos] = data[calcul + 2]; // R
			dataOut[pos + 1] = data[calcul + 1]; // G
			dataOut[pos + 2] = data[calcul]; // B
		}

	}

	return anImage;
}


void CFiltreEffetCPU::Interpolation(const int &widthOut, const int &heightOut, const int &method, int flipH, int flipV, int angle)
{
	if(bitmapOut != nullptr)
		delete bitmapOut;

	bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	CInterpolationBicubic interpolation;
	interpolation.Execute(pBitmap, bitmapOut, flipH, flipV, angle);
	/*
	switch(method)
	{
	case BOXFILTER:
		{
			CBoxFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case BILINEARFILTER :
		{
			CBilinearFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case GAUSSIANFILTER:
		{
			CGaussianFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case HAMMINGFILTER:
		{
			CHammingFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case CUBICFILTER:
		{
			CCubicFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case BLACKMANFILTER:
		{
			CBlackmanFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case QUADRATICFILTER:
		{
			CQuadraticFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case MITCHELLFILTER:
		{
			CMitchellFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case TRIANGLEFILTER:
		{
			CTriangleFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case SINCFILTER:
		{
			CSincFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case BESSELFILTER:
		{
			CBesselFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case BLACKMANBESSELFILTER:
		{
			CBlackmanBesselFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case BLACKMANSINCFILTER:
		{
			CBlackmanSincFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case LANCZOSFILTER:
		{
			CLanczosFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case HERMITEFILTER:
		{
			CHermiteFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case HANNINGFILTER:
		{
			CHanningFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	case CATROMFILTER:
		{
			CCatromFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	default:
		{
			CCubicFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut);
		}
		break;
	}
	*/
}

void CFiltreEffetCPU::Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int &method, int flipH, int flipV, int angle)
{
	if(bitmapOut != nullptr)
		delete bitmapOut;

	bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	CInterpolationBicubic interpolation;
	interpolation.Execute(pBitmap, bitmapOut, rc);//, flipH, flipV, angle);
	/*
	switch(method)
	{
	case BOXFILTER:
		{
			CBoxFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case BILINEARFILTER :
		{
			CBilinearFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case GAUSSIANFILTER:
		{
			CGaussianFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case HAMMINGFILTER:
		{
			CHammingFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case CUBICFILTER:
		{
			CCubicFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case BLACKMANFILTER:
		{
			CBlackmanFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case QUADRATICFILTER:
		{
			CQuadraticFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case MITCHELLFILTER:
		{
			CMitchellFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case TRIANGLEFILTER:
		{
			CTriangleFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case SINCFILTER:
		{
			CSincFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case BESSELFILTER:
		{
			CBesselFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case BLACKMANBESSELFILTER:
		{
			CBlackmanBesselFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case BLACKMANSINCFILTER:
		{
			CBlackmanSincFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case LANCZOSFILTER:
		{
			CLanczosFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case HERMITEFILTER:
		{
			CHermiteFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case HANNINGFILTER:
		{
			CHanningFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	case CATROMFILTER:
		{
			CCatromFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	default:
		{
			CCubicFilter interpolation;
			interpolation.Execute(pBitmap, bitmapOut,rc);
		}
		break;
	}
	*/
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
int CFiltreEffetCPU::CloudsFilter(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave, const int &intensity)
{
	if (pBitmap != nullptr)
	{
		CPerlinNoise * m_perlinNoise = new Regards::FiltreEffet::CPerlinNoise();
		CRegardsBitmap localBitmap(250,250);
		CRegardsBitmap * bitmapOut = new CRegardsBitmap(pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
		m_perlinNoise->Clouds(&localBitmap, color1, color2, amplitude / 100.0f, frequence / 100.0f, octave);
		delete m_perlinNoise;
		CInterpolationBicubic interpolation;
		interpolation.Execute(&localBitmap, bitmapOut);
		Fusion(bitmapOut, intensity / 100.0f);
		delete bitmapOut;
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
int CFiltreEffetCPU::Blur(const int &radius)
{
	if (pBitmap != nullptr)
	{
		int sizeKernel = radius * 3;
		short * kernel = new short[sizeKernel];
		for(int i = 0;i < sizeKernel;i++)
			kernel[i] = 1;
		//{ 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, radius, sizeKernel, 0);
		filtre->SetParameter(pBitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::GaussianBlur(const int &radius, const int &boxSize)
{
	if (pBitmap != nullptr)
	{
		CGaussianBlur gaussianBlur;
		gaussianBlur.GaussianBlur(pBitmap, radius);
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
		m_cEffet->Rotate(pBitmap, 270.0f, backColor);
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
		m_cEffet->Rotate(pBitmap, 90.0f, backColor);
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

CRegardsBitmap * CFiltreEffetCPU::GetBitmap(const bool &source)
{
	CRegardsBitmap * copy = new CRegardsBitmap();

	if(bitmapOut != nullptr && !source)
	{
		copy->SetBitmap(bitmapOut->GetPtBitmap(), bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight()); 
		//bitmapOut->SaveToBmp("e:\\test2.bmp");
		//copy->SaveToBmp("e:\\test.bmp");
	}
	else
		copy->SetBitmap(pBitmap->GetPtBitmap(), pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight()); 
	return copy;
}