#include <header.h>
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
//BM3D
#include "bm3dfilter.h"

extern float value[256];
using namespace Regards::FiltreEffet;

CFiltreEffetCPU::CFiltreEffetCPU(const CRgbaquad &backColor, CImageLoadingFormat * bitmap)
	: IFiltreEffet(backColor)
{
	this->backColor = backColor;
	bitmapOut = nullptr;
	pBitmap = bitmap->GetRegardsBitmap();
}

int CFiltreEffetCPU::GetWidth()
{
	if (preview)
	{
		if (bitmapOut != nullptr)
			return bitmapOut->GetBitmapWidth();
	}
	else
	{
		if (pBitmap != nullptr)
			return pBitmap->GetBitmapWidth();
	}
}

int CFiltreEffetCPU::GetHeight()
{
	if (preview)
	{
		if (bitmapOut != nullptr)
			return bitmapOut->GetBitmapHeight();
	}
	else
	{
		if (pBitmap != nullptr)
			return pBitmap->GetBitmapHeight();
	}
}

void CFiltreEffetCPU::SetBitmap(CImageLoadingFormat * bitmap)
{
	if (preview)
	{
		if (bitmapOut != nullptr)
			delete bitmapOut;
		bitmapOut = bitmap->GetRegardsBitmap();
	}
	else
	{
		if (pBitmap != nullptr)
			delete pBitmap;
		pBitmap = bitmap->GetRegardsBitmap();
	}

}

int CFiltreEffetCPU::RedEye(const wxRect& rSelectionBox)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CRedEye redeye;
		return redeye.RemoveRedEye(bitmap, backColor, rSelectionBox);
	}
	return 0;
}

CRegardsFloatBitmap * CFiltreEffetCPU::GetFloatBitmap(const bool &source)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

    CImageLoadingFormat imageLoading(false);
    imageLoading.SetPicture(bitmap);
    return imageLoading.GetFloatBitmap(true);
}

int CFiltreEffetCPU::WaveFilter(int x, int y, short height, int scale, int radius)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CWaveFilter * waveFilter = new  CWaveFilter();
		waveFilter->ProcessEffect(bitmap, x, y, height, scale, radius);
		delete waveFilter;
		return 0;
	}
	return -1;
}

int CFiltreEffetCPU::Bm3d(const int & fSigma)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
        //Copie
		int nbProcess = thread::hardware_concurrency();
        CBm3DFilter * bm3dFilter = new CBm3DFilter(bitmap, value[fSigma]);
		bm3dFilter->DetermineData(nbProcess - 1);
		bm3dFilter->ExecuteFilter();
		CRegardsBitmap * copyBitmap = bm3dFilter->GetResult();
		memcpy(bitmap->GetPtBitmap(), copyBitmap->GetPtBitmap(), copyBitmap->GetBitmapSize());
		delete bm3dFilter;

        return 0; 
	}
	return -1;
}

int CFiltreEffetCPU::ClaheFilter(int nBins, float clipLevel, int windowSize)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CClahe * clahe = new Regards::FiltreEffet::CClahe();
		CRegardsBitmap * out = clahe->ClaheFilter(bitmap, nBins, clipLevel, windowSize);
		bitmap->SetBitmap(out->GetPtBitmap(), out->GetBitmapWidth(), out->GetBitmapHeight());
		delete clahe;
		delete out;
		return 0;
	}
	return -1;
}

int CFiltreEffetCPU::BilateralFilter(int fSize,  float sigmaX, float sigmaP)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CBilateral * bilateral = new Regards::FiltreEffet::CBilateral(fSize, sigmaX, sigmaP);
		bilateral->SetParameter(bitmap, backColor);
		bilateral->Compute();
		delete bilateral;
		return 0;
	}
	return -1;
}

int CFiltreEffetCPU::NlmeansFilter(int fsize, int bsize, float sigma)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CNlmeans * nlmeans = new Regards::FiltreEffet::CNlmeans(fsize, bsize, sigma);
		nlmeans->SetParameter(bitmap, backColor);
		nlmeans->Compute();
		delete nlmeans;
		return 0;
	}
	return -1;
}


int CFiltreEffetCPU::SharpenMasking(const float &sharpness)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CSharpenMasking * sharpenMasking = new Regards::FiltreEffet::CSharpenMasking(sharpness);
		sharpenMasking->SetParameter(bitmap, backColor);
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
	CInterpolation interpolation;
	interpolation.Execute(pBitmap, bitmapOut, flipH, flipV, angle);

}

void CFiltreEffetCPU::Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int &method, int flipH, int flipV, int angle)
{
	if(bitmapOut != nullptr)
		delete bitmapOut;

	bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	//CInterpolationBicubic interpolation;
	CInterpolation interpolation;
	interpolation.Execute(pBitmap, bitmapOut, rc, flipH, flipV, angle);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CLensFlare * filtre = new CLensFlare();
		filtre->LensFlare(bitmap, iPosX, iPosY, iPuissance, iType, iIntensity, iColor, iColorIntensity);
		delete filtre;

	}
	return 0;
}

//---------------------------------------------------------------------
//Effet CRgbaquad 
//---------------------------------------------------------------------
int CFiltreEffetCPU::RGBFilter(const int &red, const int &green, const int &blue)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CRgbFiltre * filtre = new CRgbFiltre(red, green, blue);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;

	}
	return 0;
}

int CFiltreEffetCPU::Solarize(const long &threshold)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CSolarize * filtre = new CSolarize((int)threshold);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;

	}
	return 0;
}

int CFiltreEffetCPU::Posterize(const float &level, const float &gamma)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CPosterize * filtre = new CPosterize(level, gamma);
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CPerlinNoise * m_perlinNoise = new Regards::FiltreEffet::CPerlinNoise();
		CRegardsBitmap localBitmap(250,250);
		CRegardsBitmap * _local = new CRegardsBitmap(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		m_perlinNoise->Clouds(&localBitmap, color1, color2, amplitude / 100.0f, frequence / 100.0f, octave);
		delete m_perlinNoise;
		CInterpolationBicubic interpolation;
		interpolation.Execute(&localBitmap, _local);
		Fusion(_local, intensity / 100.0f);
		delete _local;
	}
	return 0;
}

//---------------------------------------------------------------------
//Effet Swirl 
//---------------------------------------------------------------------
int CFiltreEffetCPU::Swirl(const float &radius, const float &angle)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CSwirl * filtre = new CSwirl(angle, radius);
		filtre->SetParameter(bitmap, backColor);
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

	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffetCPU::NiveauDeGris()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CGrayScale * filtre = new CGrayScale();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffetCPU::NoirEtBlanc()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CBlackAndWhite * filtre = new CBlackAndWhite();
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CSepiaFiltre * filtre = new CSepiaFiltre();
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		short kernel[] = { 1, 1, 1, 1, 8, 1, 1, 1, 1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 16, 0);
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		int sizeKernel = radius * 3;
		short * kernel = new short[sizeKernel];
		for(int i = 0;i < sizeKernel;i++)
			kernel[i] = 1;
		//{ 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, radius, sizeKernel, 0);
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CGaussianBlur gaussianBlur;
		gaussianBlur.GaussianBlur(bitmap, radius);
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Emboss()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		short kernel[] = { -1, 0, 0, 0, 0, 0, 0, 0, 1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 1, 127);
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		short kernel[] = { -1, -1, -1, -1, 9, -1, -1, -1, -1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 1, 0);
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		short kernel[] = { -1, -1, -1, -1, 16, -1, -1, -1, -1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 8, 0);
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CErode * filtre = new CErode();
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CMedian * filtre = new CMedian();
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CNoise * filtre = new CNoise();
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CDilate * filtre = new CDilate();
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CNegatif * filtre = new CNegatif();
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CContrast * filtre = new CContrast(contrast, offset);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

int CFiltreEffetCPU::Lightness(const double &factor)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CLightness * filtre = new CLightness(factor);
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		short kernel[] = { -1, -1, -1, -1, 8, -1, -1, -1, -1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 1, 0);
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CMosaic * filtre = new CMosaic(5);
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		bitmap->VertFlipBuf();
	}
	return 0;
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FlipHorizontal()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		bitmap->HorzFlipBuf();
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::MotionBlur(const double &radius, const double &sigma, const double &angle)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CMotionBlur * filtre = new CMotionBlur();
		filtre->MotionBlur(bitmap, radius, sigma, angle);
		delete filtre;

	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::RotateFree(const double &angle, const int &widthOut, const int &heightOut)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CRotation * m_cEffet = new CRotation();
		m_cEffet->Rotate(bitmap, angle, backColor);
		delete m_cEffet;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::PhotoFiltre(const CRgbaquad &clValue, const int &intensity)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CPhotoFiltre * filtre = new CPhotoFiltre(clValue, intensity);
		filtre->SetParameter(bitmap, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CRotation * m_cEffet = new CRotation();
		m_cEffet->Rotate(bitmap, 270.0f, backColor);
		delete m_cEffet;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Rotate270()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CRotation * m_cEffet = new CRotation();
		m_cEffet->Rotate(bitmap, 90.0f, backColor);
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
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		bitmap->FusionBitmap(bitmapSecond, pourcentage);
	}
	return 0;
}

CRegardsBitmap * CFiltreEffetCPU::GetBitmap(const bool &source)
{
	CRegardsBitmap * copy = new CRegardsBitmap();
	if (bitmapOut != nullptr && !source)
	{
		copy->SetFilename(bitmapOut->GetFilename());
		copy->SetBitmap(bitmapOut->GetPtBitmap(), bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight());
	}
	else
	{
		copy->SetFilename(pBitmap->GetFilename());
		copy->SetBitmap(pBitmap->GetPtBitmap(), pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
	}
	return copy;
}