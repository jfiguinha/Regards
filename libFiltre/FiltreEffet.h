#pragma once
#include <RGBAQuad.h>
#include <effect_id.h>
#include <GLTexture.h>
using namespace Regards::OpenGL;

class Chqdn3d;
class CImageLoadingFormat;
class CRegardsFloatBitmap;
class CRegardsBitmap;
class CEffectParameter;
class IFiltreEffet;

namespace Regards::OpenCV
{
	class COpenCVStabilization;
}


class CFiltreEffet
{
public:
	CFiltreEffet(const CRgbaquad& backColor, const bool& useOpenCL, CImageLoadingFormat* bitmap);
	virtual ~CFiltreEffet();

	void SetPreviewMode(const bool& value);
	void SetBitmap(CImageLoadingFormat* bitmap);
	void SetPreview(const bool& value);
	int RenderEffect(const int& numEffect, CEffectParameter* effectParameter);
	int RenderEffectPreview(const int& numEffect, CEffectParameter* effectParameter);
	wxImage RenderEffectPreviewwxImage(const int& numEffect, CEffectParameter* effectParameter);
	int SharpenMasking(const float& sharpness);
	int BokehEffect(const int& radius, const int& boxsize, const int& nbFace, const wxRect& listFace);
	int HistogramNormalize();
	int HistogramEqualize();
	int WaveFilter(int x, int y, short height, int scale, int radius);
	int NiveauDeGris();
	int NoirEtBlanc();
	int VignetteEffect(const double& radius, const double& power);
	int Sepia();
	int Soften();
	int Blur(const int& radius);
	int GaussianBlur(const int& radius, const int& boxSize);
	int Emboss();
	int SharpenStrong();
	int Sharpen();
	int Erode();
	cv::UMat GetUMat();
	cv::Mat GetMat();
	int LensDistortionFilter(const int& size);
	int OilPaintingEffect(const int& size = 10, const int& dynRatio = 1);
	int Median();
	int CartoonifyImage(const int& mode);
	int Noise();
	int Dilate();
	int Negatif();
	int RedEye();
	int FiltreEdge();
	int FiltreMosaic(const int& size);
	int FlipVertical();
	int FlipHorizontal();
	int Rotate90();
	int Rotate270();
	int HQDn3D(const double& LumSpac, const double& ChromSpac, const double& LumTmp, const double& ChromTmp);
	int MotionBlur(const double& radius, const double& sigma, const double& angle);
	int RotateFree(const double& angle);
	int PhotoFiltre(const CRgbaquad& clValue, const int& intensity);
	int BrightnessAndContrast(const double& brightness, const double& contrast);
	int RGBFilter(const int& red, const int& green, const int& blue);
	int CloudsFilter(const CRgbaquad& color1, const CRgbaquad& color2, const float& amplitude, const float& frequence,
	                 const int& octave, const int& intensity);
	int Swirl(const float& radius, const float& angle);
	int Fusion(cv::Mat& bitmapSecond, const float& pourcentage);
	int Posterize(const float& level, const float& gamma);
	int Solarize(const long& threshold);
	int BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP);
	int NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize, const int& searchWindowSize);

	void Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method, int flipH,
	                   int flipV, int angle, int ratio);
	int MeanShift(const float& fSpatialRadius, const float& fColorRadius);
	int LensFlare(const int& iPosX, const int& iPosY, const int& iPuissance, const int& iType, const int& iIntensity,
	              const int& iColor, const int& iColorIntensity);
	int GetLib();
	wxImage GetwxImage();
	cv::Mat GetBitmap(const bool& source);

	IFiltreEffet* GetInstance()
	{
		return filtreEffet;
	};

	int GetWidth();
	int GetHeight();
	int BrightnessAndContrastAuto(float clipHistPercent);
	bool StabilizeVideo(Regards::OpenCV::COpenCVStabilization* stabilization);

private:
	void CalculNewSize(const int32_t& x, const int32_t& y, const double& angle, int& width, int& height);
	//CRegardsBitmap * pBitmap;
	IFiltreEffet* filtreEffet;
	CRgbaquad backColor;
	int numLib;
	wxString filename;
	int width;
	int height;
};
