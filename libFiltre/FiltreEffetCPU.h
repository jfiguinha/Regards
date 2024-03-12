#pragma once
#include "IFiltreEffet.h"

class CRegardsBitmap;
class Chqdn3d;
using namespace Regards::OpenGL;

class CFiltreEffetCPU : public IFiltreEffet
{
public:
	CFiltreEffetCPU(CRgbaquad back_color, CImageLoadingFormat* bitmap);
	~CFiltreEffetCPU() override;

	int HistogramNormalize() override;
	int HistogramEqualize() override;
	cv::UMat GetUMat() override;
	cv::Mat GetMat() override;
	int RedEye() override;
	int BokehEffect(const int& radius, const int& boxsize, const int& nbFace, const wxRect& listFace) override;
	int HQDn3D(const double& LumSpac = 4, const double& temporalLumaDefault = 6.0, const double& temporalSpatialLumaDefault = 4.0) override;
	int BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP) override;
	int NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize,
	                  const int& searchWindowSize) override;
	void SetBitmap(CImageLoadingFormat* bitmap) override;
	int MeanShift(const float& fSpatialRadius, const float& fColorRadius) override;
	void Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method, int flipH,
	                   int flipV, int angle, int ratio) override;
	int LensDistortionFilter(const int& size) override;
	cv::Mat GetBitmap(const bool& source) override;
	wxImage GetwxImage() override;
	int WaveFilter(int x, int y, short height, int scale, int radius) override;
	int SharpenMasking(const float& sharpness) override;
	int GroundGlassEffect(const double& radius);
	int NiveauDeGris() override;
	int NoirEtBlanc() override;
	int Sepia() override;
	int Soften() override;
	int Blur(const int& radius) override;
	int GaussianBlur(const int& radius, const int& boxSize) override;
	int Emboss() override;
	int SharpenStrong() override;
	int Sharpen() override;
	int Erode() override;
	int Median() override;
	int Noise() override;
	int Dilate() override;
	int Negatif() override;
	int FiltreEdge() override;
	int FiltreMosaic(const int& size) override;
	int FlipVertical() override;
	int FlipHorizontal() override;
	int CartoonifyImage(const int& mode) override;
	int MotionBlur(const double& radius, const double& sigma, const double& angle) override;
	int RotateFree(const double& angle, const int& widthOut, const int& heightOut) override;
	int PhotoFiltre(const CRgbaquad& clValue, const int& intensity) override;
	int Rotate90() override;
	int Rotate270() override;
	int Rotate180() override;
	int BrightnessAndContrast(const double& brightness, const double& contrast) override;
	int RGBFilter(const int& red, const int& green, const int& blue) override;
	int Resize(const int& imageWidth, const int& imageHeight, const int& interpolation);
	int CloudsFilter(const CRgbaquad& color1, const CRgbaquad& color2, const float& amplitude, const float& frequence,
	                 const int& octave, const int& intensity) override;
	int Swirl(const float& radius, const float& angle) override;
	int Posterize(const float& level, const float& gamma) override;
	int Solarize(const long& threshold) override;
	int Fusion(cv::Mat& bitmapSecond, const float& pourcentage) override;
	int LensFlare(const int& iPosX, const int& iPosY, const int& iPuissance, const int& iType, const int& iIntensity,
	              const int& iColor, const int& iColorIntensity) override;
	int GetWidth() override;
	int GetHeight() override;
	int BrightnessAndContrastAuto(float clipHistPercent) override;
	int OilPaintingEffect(const int& size, const int& dynRatio) override;
	int VignetteEffect(const double& radius = 1.0, const double& power = 0.8) override;
	static void BrightnessAndContrastAuto(cv::Mat& image, float clipHistPercent);
	static void LoadAndRotate(const wxString& filePath, const int& rotate);
	bool StabilizeVideo(Regards::OpenCV::COpenCVStabilization* stabilization) override;
	static cv::Mat Interpolation(const cv::Mat& cvImage, const int& widthOut, const int& heightOut, const wxRect& rc,
	                             const int& method, int flipH, int flipV, int angle, int ratio);

	int DetailEnhance(const double& sigma_s, const double& sigma_r) override;
	int EdgePreservingFilter(const int& flags, const double& sigma_s, const double& sigma_r) override;
	int PencilSketch(const double& sigma_s, const double& sigma_r, const double& shade_factor) override;
	int Stylization(const double& sigma_s, const double& sigma_r) override;

private:
	void RotateMatrix(const int& angle, cv::Mat& src);
	void ChangeFacialSkinColor(cv::Mat smallImgBGR, cv::Mat bigEdges);
	void RemovePepperNoise(cv::Mat& mask);

	Chqdn3d* hq3d = nullptr;
	int oldLevelDenoise = 4;
	int oldwidthDenoise = 0;
	int oldheightDenoise = 0;
	wxString filename;

	cv::Mat alphaChannel;
	cv::Mat input;
	cv::Mat paramOutput;
};
