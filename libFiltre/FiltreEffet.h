#pragma once
#include <RegardsBitmap.h>
#include <EffectParameter.h>
#include <IFiltreEffet.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif



class CFiltreEffet
{
public:
	CFiltreEffet(CRegardsBitmap * pBitmap, const CRgbaquad &backColor, const int &numLib);
		
	virtual ~CFiltreEffet();

	int HistogramLog();
	int HistogramNormalize();
	int HistogramEqualize();
	//Color Conversion
	uint8_t * BGRA32ToRGB24();
	int RGB24ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size);
	int YUV420ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size);
	int NV12ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size);
	int InterpolationBicubicNV12ToRGB32(uint8_t * buffer, const int &width, const int &height, const int &rectWidth, const int &rectHeight, CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle, const int &size);
	int InterpolationBicubicRGB32Video(CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle);
	int NiveauDeGris();
	int NoirEtBlanc();
	int SharpenMasking(const int &sharpness);
	int Sepia();
	int Soften();
	int Blur();
	int GaussianBlur();
	int Emboss();
	int SharpenStrong();
	int Sharpen();
	int Erode();
	int Median();
	int Noise();
	int Dilate();
	int Negatif();
	int RedEye(const wxRect& rSelectionBox);
	int FiltreEdge();
	int FiltreMosaic();
	int FlipVertical();
	int FlipHorizontal();
	int MotionBlur(const double &radius, const double &sigma, const double &angle);
	int RotateFree(const double &angle);
	int PhotoFiltre(const CRgbaquad &clValue, const int &intensity);
	int Rotate90();
	int Rotate270();
	int BrightnessAndContrast(const double &brightness, const double &contrast);
	int RGBFilter(const int &red, const int &green, const int &blue);
	int Resize(const int &imageWidth, const int &imageHeight, const int &interpolation);
	int CloudsFilter(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave);
	int Swirl(const float &radius, const float &angle);
	int Contrast(const double &contrast, const uint8_t &offset);
	int Lightness(const double &factor);
	int Fusion(CRegardsBitmap * bitmapSecond, const float &pourcentage);
	int Posterize(const float &level, const float &gamma);
	int Solarize(const long &threshold);
	wxImage InterpolationBicubic(const wxImage & imageSrc, const int &widthOut, const int &heightOut);
	int InterpolationBicubic(CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle);
	int InterpolationBicubic(CRegardsBitmap * & bitmapOut);
	int InterpolationBicubic(CRegardsBitmap * & bitmapOut, const wxRect &rc);
	int InterpolationBilinear(CRegardsBitmap * & bitmapOut);
	int InterpolationBilinear(CRegardsBitmap * & bitmapOut, const wxRect &rc);
	int InterpolationFast(CRegardsBitmap * & bitmapOut);
	int InterpolationFast(CRegardsBitmap * & bitmapOut, const wxRect &rc);
	int LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity);
    int RenderEffect(const int &numEffect, CEffectParameter * effectParameter);
	void SetLib(const int &numLib);

	IFiltreEffet * GetInstance()
	{
		return filtreEffet;
	};

private:

	void CalculNewSize(const int32_t &x, const int32_t &y, const double &angle, int &width, int &height);

	CRegardsBitmap * pBitmap = nullptr;
	IFiltreEffet * filtreEffet = nullptr;
	IFiltreEffet * filtreEffetCPU = nullptr;
	CRgbaquad backColor;
	int numLib;
};
