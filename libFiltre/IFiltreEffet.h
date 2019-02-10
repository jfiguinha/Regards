#pragma once
#include <RGBAQuad.h>
class CRegardsBitmap;
class CRegardsFloatBitmap;
class CImageLoadingFormat;

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
    virtual int Bm3d(const int & fSigma) = 0;
	virtual void Interpolation(const int &widthOut, const int &heightOut, const int &method, int flipH, int flipV, int angle) = 0;
	virtual void Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int &method, int flipH, int flipV, int angle) = 0;
	virtual int WaveFilter(int x, int y, short height, int scale, int radius) = 0;
	virtual int ClaheFilter(int nBins, float clipLevel, int windowSize) = 0;
	virtual int BilateralFilter(int fSize,  float sigmaX, float sigmaP) = 0;
	virtual int NlmeansFilter(int fsize, int bsize, float sigma) = 0;
	virtual int GetRgbaBitmap(void * cl_image) = 0; 
	virtual int HistogramLog() = 0;
	virtual int HistogramNormalize() = 0;
	virtual int HistogramEqualize() = 0;
	virtual int SharpenMasking(const float &sharpness) = 0;
	virtual int NiveauDeGris() = 0;
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
    virtual CRegardsFloatBitmap * GetFloatBitmap(const bool &source) = 0;
	virtual int RedEye(const wxRect& rSelectionBox) = 0;
    virtual void SetBitmap(CImageLoadingFormat * bitmap) = 0;

protected:
	//CRegardsBitmap * pBitmap;
	CRgbaquad backColor;
	bool preview;
};

