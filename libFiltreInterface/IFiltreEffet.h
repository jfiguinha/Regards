#pragma once
#include <RGBAQuad.h>
class CRegardsBitmap;
class CRegardsFloatBitmap;
class CImageLoadingFormat;
class Chqdn3d;
class IFiltreEffet
{
public:

	IFiltreEffet(const CRgbaquad &backColor)
	{
		preview = false;
		this->backColor = backColor;
	};
	virtual ~IFiltreEffet(){};

	virtual void SetPreviewMode(const bool &preview)
	{
		this->preview = preview;
	}
	virtual void GetYUV420P(uint8_t * & y, uint8_t * & u, uint8_t * & v, const int &widthOut, const int &heightOut) = 0;
    virtual int Bm3d(const int & fSigma) = 0;
	virtual int OilPaintingEffect(const int &size, const int &dynRatio) = 0;
	virtual void Interpolation(const int &widthOut, const int &heightOut, const int &method, int flipH, int flipV, int angle) = 0;
	virtual void Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int &method, int flipH, int flipV, int angle) = 0;
	virtual int WaveFilter(int x, int y, short height, int scale, int radius) = 0;
	virtual int BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP) = 0;
	virtual int NlmeansFilter(const int& h, const int& templateWindowSize, const int& searchWindowSize) = 0;
	virtual int GetRgbaBitmap(void * cl_image) = 0; 
	virtual int HistogramNormalize() = 0;
	virtual int HistogramEqualize() = 0;
	virtual int CartoonifyImage(const int & mode) = 0;
	virtual int SharpenMasking(const float &sharpness) = 0;
	virtual int NiveauDeGris() = 0;
	virtual int HQDn3D(const double & LumSpac, const double & ChromSpac, const double & LumTmp, const double & ChromTmp) = 0;
	virtual int NoirEtBlanc() = 0;
	virtual int Sepia() = 0;
	virtual int Soften() = 0;
	virtual int Blur(const int &radius) = 0;
	virtual int GaussianBlur(const int &radius, const int &boxsize) = 0;
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
	virtual int VignetteEffect(const double& radius, const double& power) = 0;
	virtual int MeanShift(const float& fSpatialRadius, const float& fColorRadius) = 0;
	virtual int MotionBlur(const double &radius, const double &sigma, const double &angle) = 0;
	virtual int PhotoFiltre(const CRgbaquad &clValue, const int &intensity) = 0;
	virtual int Rotate90() = 0;
	virtual int Rotate270() = 0;
	virtual int RotateFree(const double &angle, const int &widthOut, const int &heightOut) = 0;
	virtual int BrightnessAndContrast(const double &brightness, const double &contrast) = 0;
	virtual int RGBFilter(const int &red, const int &green, const int &blue) = 0;
	virtual int CloudsFilter(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave, const int &intensity) = 0;
	virtual int Swirl(const float &radius, const float &angle) = 0;
	virtual int Contrast(const double &contrast, const uint8_t &offset) = 0;
	virtual int Lightness(const double &factor) = 0;
	virtual int Posterize(const float &level, const float &gamma) = 0;
	virtual int Solarize(const long &threshold) = 0;
	virtual int LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity) = 0;
	virtual int Fusion(CRegardsBitmap * bitmapSecond, const float &pourcentage) = 0;
	virtual wxImage GetwxImage() = 0;
	virtual CRegardsBitmap * GetBitmap(const bool &source) = 0;
	virtual CRegardsBitmap * GetPtBitmap() = 0;
	virtual void GetBitmap(CRegardsBitmap * & bitmap, const bool &source) = 0;
    virtual CRegardsFloatBitmap * GetFloatBitmap(const bool &source) = 0;
	virtual int RedEye() = 0;
    virtual void SetBitmap(CImageLoadingFormat * bitmap) = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int BrightnessAndContrastAuto(float clipHistPercent) = 0;
protected:
	//CRegardsBitmap * pBitmap;
	CRgbaquad backColor;
	bool preview;
};

