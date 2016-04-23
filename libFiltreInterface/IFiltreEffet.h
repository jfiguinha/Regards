#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <stdint.h>
#include <RegardsBitmap.h>
#include <RGBAQuad.h>

class IFiltreEffet
{
public:

	IFiltreEffet(CRegardsBitmap * pBitmap, const CRgbaquad &backColor)
	{
		this->pBitmap = pBitmap;
		this->backColor = backColor;
	};
	virtual ~IFiltreEffet(){};

	//Color Conversion
	virtual int RGB24ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size) = 0;
	virtual int YUV420ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size) = 0;
	virtual int NV12ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size) = 0;
	virtual int InterpolationBicubicNV12ToRGB32(uint8_t * buffer, const int &width, const int &height, const int &rectWidth, const int &rectHeight, CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle, const int &size) = 0;
	virtual int InterpolationBicubicRGB32Video(CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle) = 0;
	virtual int HistogramLog() = 0;
	virtual int HistogramNormalize() = 0;
	virtual int HistogramEqualize() = 0;
	virtual int SharpenMasking(const int &sharpness) = 0;
	virtual int NiveauDeGris() = 0;
	virtual int NoirEtBlanc() = 0;
	virtual int Sepia() = 0;
	virtual int Soften() = 0;
	virtual int Blur() = 0;
	virtual int GaussianBlur() = 0;
	virtual int Emboss() = 0;
	virtual int SharpenStrong() = 0;
	virtual int Sharpen() = 0;
	virtual int Erode() = 0;
	virtual int Median() = 0;
	virtual int Noise() = 0;
	virtual int Dilate() = 0;
	virtual int Negatif() = 0;
	virtual int FiltreEdge() = 0;
	virtual int FiltreMosaic() = 0;
	virtual int FlipVertical() = 0;
	virtual int FlipHorizontal() = 0;
	virtual int MotionBlur(const double &radius, const double &sigma, const double &angle) = 0;
	virtual int PhotoFiltre(const CRgbaquad &clValue, const int &intensity) = 0;
	virtual int Rotate90() = 0;
	virtual int Rotate270() = 0;
	virtual int RotateFree(const double &angle, const int &widthOut, const int &heightOut) = 0;
	virtual int BrightnessAndContrast(const double &brightness, const double &contrast) = 0;
	virtual int RGBFilter(const int &red, const int &green, const int &blue) = 0;
	virtual int Resize(const int &imageWidth, const int &imageHeight, const int &interpolation) = 0;
	virtual int CloudsFilter(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave) = 0;
	virtual int Swirl(const float &radius, const float &angle) = 0;
	virtual int Contrast(const double &contrast, const uint8_t &offset) = 0;
	virtual int Lightness(const double &factor) = 0;
	virtual int Posterize(const float &level, const float &gamma) = 0;
	virtual int Solarize(const long &threshold) = 0;
	virtual int LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity) = 0;
	virtual int Fusion(CRegardsBitmap * bitmapSecond, const float &pourcentage) = 0;
	virtual int InterpolationBicubic(CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle) = 0;
	virtual int InterpolationBicubic(CRegardsBitmap * & bitmapOut) = 0;
	virtual wxImage InterpolationBicubic(const wxImage & imageSrc, const int &widthOut, const int &heightOut) = 0;
	virtual int InterpolationBicubic(CRegardsBitmap * & bitmapOut, const wxRect &rc) = 0;
	virtual int InterpolationBilinear(CRegardsBitmap * & bitmapOut) = 0;
	virtual int InterpolationBilinear(CRegardsBitmap * & bitmapOut, const wxRect &rc) = 0;
	virtual int InterpolationFast(CRegardsBitmap * & bitmapOut) = 0;
	virtual int InterpolationFast(CRegardsBitmap * & bitmapOut, const wxRect &rc) = 0;
	virtual int RedEye(const wxRect& rSelectionBox) = 0;
protected:
	CRegardsBitmap * pBitmap;
	CRgbaquad backColor;
};

