#pragma once
#include <RGBAQuad.h>
#include <PictureArray.h>
class CRegardsBitmap;
class CRegardsFloatBitmap;
class CImageLoadingFormat;
class Chqdn3d;
class COpenCVStabilization;


#define TYPE_CPU 1
#define TYPE_OPENCL 2
#define TYPE_CUDA 3

namespace Regards::OpenCV
{
	class COpenCVStabilization;
}


namespace Regards::OpenGL
{
	class GLTexture;
}

using namespace Regards::OpenGL;

class IFiltreEffet
{
public:
	IFiltreEffet(const CRgbaquad& backColor)
	{
		preview = false;
		this->backColor = backColor;
	};

	virtual ~IFiltreEffet()
	{
	};

	virtual void SetPreviewMode(const bool& preview)
	{
		this->preview = preview;
	}

	virtual Regards::Picture::CPictureArray GetMatrix() = 0;
	virtual int BokehEffect(const int& radius, const int& boxsize, const int& nbFace, const wxRect& listFace) = 0;
	virtual int OilPaintingEffect(const int& size, const int& dynRatio) = 0;
	virtual void Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
	                           int flipH, int flipV, int angle, int ratio) = 0;
	virtual int WaveFilter(int x, int y, short height, int scale, int radius) = 0;
	virtual int BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP) = 0;
	virtual int NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize,
	                          const int& searchWindowSize) = 0;
	virtual int HistogramNormalize() = 0;
	virtual int HistogramEqualize() = 0;
	virtual int CartoonifyImage(const int& mode) = 0;
	virtual int SharpenMasking(const float& sharpness) = 0;
	virtual int NiveauDeGris() = 0;
	virtual int HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault) = 0;
	virtual int NoirEtBlanc() = 0;
	virtual int Sepia() = 0;
	virtual int Soften() = 0;
	virtual int Blur(const int& radius) = 0;
	virtual int GaussianBlur(const int& radius, const int& boxsize) = 0;
	virtual int Emboss() = 0;
	virtual int SharpenStrong() = 0;
	virtual int Sharpen() = 0;
	virtual int Erode() = 0;
	virtual int Median() = 0;
	virtual int Noise() = 0;
	virtual int DetailEnhance(const double& sigma_s, const double& sigma_r) = 0;
	virtual int EdgePreservingFilter(const int& flags, const double& sigma_s, const double& sigma_r) = 0;
	virtual int PencilSketch(const double& sigma_s, const double& sigma_r, const double& shade_factor) = 0;
	virtual int Stylization(const double& sigma_s, const double& sigma_r) = 0;

	virtual int LensDistortionFilter(const int& size) = 0;
	virtual int Dilate() = 0;
	virtual int Negatif() = 0;
	virtual int FiltreEdge() = 0;
	virtual int FiltreMosaic(const int& size) = 0;
	virtual int FlipVertical() = 0;
	virtual int FlipHorizontal() = 0;
	virtual int VignetteEffect(const double& radius, const double& power) = 0;
	virtual int MeanShift(const float& fSpatialRadius, const float& fColorRadius) = 0;
	virtual int MotionBlur(const double& radius, const double& sigma, const double& angle) = 0;
	virtual int PhotoFiltre(const CRgbaquad& clValue, const int& intensity) = 0;
	virtual int Rotate90() = 0;
	virtual int Rotate270() = 0;
	virtual int Rotate180() = 0;
	virtual int RotateFree(const double& angle, const int& widthOut, const int& heightOut) = 0;
	virtual int BrightnessAndContrast(const double& brightness, const double& contrast) = 0;
	virtual int RGBFilter(const int& red, const int& green, const int& blue) = 0;
	virtual int CloudsFilter(const CRgbaquad& color1, const CRgbaquad& color2, const float& amplitude,
	                         const float& frequence, const int& octave, const int& intensity) = 0;
	virtual int Swirl(const float& radius, const float& angle) = 0;
	virtual int Posterize(const float& level, const float& gamma) = 0;
	virtual int Solarize(const long& threshold) = 0;
	virtual int LensFlare(const int& iPosX, const int& iPosY, const int& iPuissance, const int& iType,
	                      const int& iIntensity, const int& iColor, const int& iColorIntensity) = 0;
	virtual int Fusion(cv::Mat& bitmapSecond, const float& pourcentage) = 0;
	virtual wxImage GetwxImage() = 0;
	virtual int RedEye() = 0;
	virtual void SetBitmap(CImageLoadingFormat* bitmap) = 0;
	virtual cv::Mat GetBitmap(const bool& source) = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int BrightnessAndContrastAuto(float clipHistPercent) = 0;
	virtual bool StabilizeVideo(Regards::OpenCV::COpenCVStabilization* stabilization) = 0;
	virtual int SuperResolutionNCNN() = 0;
	virtual int Colorization() = 0;
    virtual int Inpaint(const cv::Mat &mask, int algorithm) = 0;
protected:
	//CRegardsBitmap * pBitmap;
	CRgbaquad backColor;
	bool preview;
};
