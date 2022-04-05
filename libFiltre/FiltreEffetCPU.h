#pragma once
#include "IFiltreEffet.h"
#include <GLTexture.h>

class CRegardsBitmap;
class Chqdn3d;
using namespace Regards::OpenGL;

class CFiltreEffetCPU : public IFiltreEffet
{
public:
	CFiltreEffetCPU(CRgbaquad back_color, CImageLoadingFormat * bitmap);
	~CFiltreEffetCPU();

	bool convertToGLTexture2D(GLTexture* glTexture);
	int HistogramNormalize();
	int HistogramEqualize();

	int RedEye();
	int BokehEffect(const int& radius, const int& boxsize, const int& nbFace, const wxRect & listFace);
	int HQDn3D(const double & LumSpac, const double & ChromSpac, const double & LumTmp, const double & ChromTmp);
	int BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP);
	int NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize, const int& searchWindowSize);
    void SetBitmap(CImageLoadingFormat * bitmap);
	int MeanShift(const float& fSpatialRadius, const float& fColorRadius);
	void Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int &method, int flipH, int flipV, int angle, int ratio);

	CRegardsBitmap * GetBitmap(const bool &source);
	void GetBitmap(CRegardsBitmap * & bitmap, const bool &source);
	CRegardsBitmap * GetPtBitmap();
	wxImage GetwxImage();
	int WaveFilter(int x, int y, short height, int scale, int radius);
	int SharpenMasking(const float &sharpness);
	int GroundGlassEffect(const double& radius);
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
	int CartoonifyImage(const int & mode);
	int MotionBlur(const double &radius, const double &sigma, const double &angle);
	int RotateFree(const double &angle, const int &widthOut, const int &heightOut);
	int PhotoFiltre(const CRgbaquad &clValue, const int &intensity);
	int Rotate90();
	int Rotate270();
	int Rotate180();
	int BrightnessAndContrast(const double &brightness, const double &contrast);
	int RGBFilter(const int &red, const int &green, const int &blue);
	int Resize(const int &imageWidth, const int &imageHeight, const int &interpolation);
	int CloudsFilter(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave, const int &intensity);
	int Swirl(const float &radius, const float &angle);
	int Posterize(const float &level, const float &gamma);
	int Solarize(const long &threshold);
	int Fusion(CRegardsBitmap * bitmapSecond, const float &pourcentage);
	int LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity);
	virtual int GetWidth();
	virtual int GetHeight();
	int BrightnessAndContrastAuto(float clipHistPercent);
	int OilPaintingEffect(const int &size, const int &dynRatio);
	int VignetteEffect(const double& radius = 1.0, const double& power = 0.8);
	static void BrightnessAndContrastAuto(cv::Mat& image, float clipHistPercent);
	static void LoadAndRotate(const wxString& filePath, const int& rotate);
	bool StabilizeVideo(Regards::OpenCV::COpenCVStabilization* stabilization);
	static cv::Mat Interpolation(cv::Mat& cvImage, const int& widthOut, const int& heightOut, const wxRect& rc, const int& method, int flipH, int flipV, int angle, int ratio);
private:


	void RotateMatrix(const int& angle, cv::Mat& src);
	void ChangeFacialSkinColor(cv::Mat smallImgBGR, cv::Mat bigEdges);
	void RemovePepperNoise(cv::Mat &mask);

	Chqdn3d * hq3d = nullptr;
	int oldLevelDenoise = 4;
	int oldwidthDenoise = 0;
	int oldheightDenoise = 0;
	wxString filename;

	cv::Mat alphaChannel;
	cv::Mat input;
	cv::Mat paramOutput;

};

