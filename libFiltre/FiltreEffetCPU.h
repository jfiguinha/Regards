#pragma once
#include "IFiltreEffet.h"
class CRegardsBitmap;
class Chqdn3d;

namespace cv
{
	class Mat;
}

class CFiltreEffetCPU : public IFiltreEffet
{
public:
	CFiltreEffetCPU(const CRgbaquad &backColor, CImageLoadingFormat * bitmap);
	~CFiltreEffetCPU();
    int Bm3d(const int & fSigma);
	int HistogramLog();
	int HistogramNormalize();
	int HistogramEqualize();
	void GetYUV420P(uint8_t * & y, uint8_t * & u, uint8_t * & v, const int &widthOut, const int &heightOut);
	int RedEye();
	int HQDn3D(const double & LumSpac, const double & ChromSpac, const double & LumTmp, const double & ChromTmp);
	int ClaheFilter(int nBins, float clipLevel, int windowSize);
	int BilateralFilter(int fSize,  float sigmaX, float sigmaP);
	int NlmeansFilter(int fsize, int bsize, float sigma);
	int GetRgbaBitmap(void * cl_image){return -1;};
    void SetBitmap(CImageLoadingFormat * bitmap);
	void Interpolation(const int &widthOut, const int &heightOut, const int &method, int flipH, int flipV, int angle);
	void Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int &method, int flipH, int flipV, int angle);
    CRegardsFloatBitmap * GetFloatBitmap(const bool &source);
	CRegardsBitmap * GetBitmap(const bool &source);
	void GetBitmap(CRegardsBitmap * & bitmap, const bool &source);
	CRegardsBitmap * GetPtBitmap();
	wxImage GetwxImage();
	int WaveFilter(int x, int y, short height, int scale, int radius);
	int SharpenMasking(const float &sharpness);
	int NiveauDeGris();
	int NoirEtBlanc();
	int Sepia();
	int Soften();
	int Blur(const int &radius);
	int GaussianBlur(const int &radius, const int &boxSize);
	int Emboss();
	int SharpenStrong();
	int Sharpen();
	int Erode();
	int Median();
	int Noise();
	int Dilate();
	int Negatif();
	int FiltreEdge();
	int FiltreMosaic();
	int FlipVertical();
	int FlipHorizontal();
	int CartoonifyImage(const bool & sketchMode, const bool & alienMode, const bool & evilMode);
	int MotionBlur(const double &radius, const double &sigma, const double &angle);
	int RotateFree(const double &angle, const int &widthOut, const int &heightOut);
	int PhotoFiltre(const CRgbaquad &clValue, const int &intensity);
	int Rotate90();
	int Rotate270();
	int BrightnessAndContrast(const double &brightness, const double &contrast);
	int RGBFilter(const int &red, const int &green, const int &blue);
	int Resize(const int &imageWidth, const int &imageHeight, const int &interpolation);
	int CloudsFilter(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave, const int &intensity);
	int Swirl(const float &radius, const float &angle);
	int Contrast(const double &contrast, const uint8_t &offset);
	int Lightness(const double &factor);
	int Posterize(const float &level, const float &gamma);
	int Solarize(const long &threshold);
	int Fusion(CRegardsBitmap * bitmapSecond, const float &pourcentage);
	int LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity);
	virtual int GetWidth();
	virtual int GetHeight();
	static wxImage GetwxImage(CRegardsBitmap * bitmap);
	int BrightnessAndContrastAuto(float clipHistPercent);
	int OilPaintingEffect(const int &size, const int &dynRatio);

private:
	void ChangeFacialSkinColor(cv::Mat smallImgBGR, cv::Mat bigEdges);
	void RemovePepperNoise(cv::Mat &mask);
	CRegardsBitmap * bitmapOut;
	CRegardsBitmap * pBitmap;
	Chqdn3d * hq3d = nullptr;
	int oldLevelDenoise = 4;
	int oldwidthDenoise = 0;
	int oldheightDenoise = 0;
	//cv::Mat bitmapOpenCV;
};

