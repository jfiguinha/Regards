#pragma once
#include "IFiltreEffet.h"
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
class CRegardsBitmap;
class CRegardsFloatBitmap;

namespace Regards::OpenCL
{
	class COpenCLProgram;
	class COpenCLParameter;
	class COpenCLParameterInt;
	class COpenCLParameterByteArray;
	class COpenCLParameterClMem;
	class COpenCLFilter;
}

using namespace Regards::OpenCL;
using namespace Regards::OpenGL;

namespace Regards::FiltreEffet
{
	class COpenCLEffect : public IFiltreEffet
	{
	public:
		COpenCLEffect(const CRgbaquad& backColor, CImageLoadingFormat* bitmap);

		void SetFlag(const bool& useMemory)
		{
			flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
		};

		cv::UMat GetUMat() override;
		cv::Mat GetMat() override;
		~COpenCLEffect() override;
		int LensDistortionFilter(const int& size) override;

		int BokehEffect(const int& radius, const int& boxsize, const int& nbFace, const wxRect& listFace) override
		{
			return -1;
		};
		int VignetteEffect(const double& radius = 1.0, const double& power = 0.8) override { return -1; };
		int MeanShift(const float& fSpatialRadius, const float& fColorRadius) override { return -1; };
		int BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP) override;
		int NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize,
		                  const int& searchWindowSize) override;
		int OilPaintingEffect(const int& size, const int& dynRatio) override { return -1; };
		void Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
		                   int flipH, int flipV, int angle, int ratio) override;
		int CartoonifyImage(const int& mode) override { return -1; };
		int NiveauDeGris() override;
		int RedEye() override { return -1; };
		int HistogramNormalize() override { return -1; };
		int HistogramEqualize() override { return -1; };
		int HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault) override;
		wxImage GetwxImage() override;
		void SetBitmap(CImageLoadingFormat* bitmap) override;
		int FlipVertical() override;
		int FlipHorizontal() override;
		int WaveFilter(int x, int y, short height, int scale, int radius) override;
		int Rotate90() override;
		int Rotate270() override;
		int Rotate180() override;
		int Negatif() override;
		int Sepia() override;
		int NoirEtBlanc() override;
		int Soften() override;
		int Blur(const int& radius) override;
		int GaussianBlur(const int& radius, const int& boxSize) override;
		int Emboss() override;
		int SharpenStrong() override;
		int Sharpen() override;
		int SharpenMasking(const float& sharpness) override;
		int FiltreMosaic(const int& size) override;
		int FiltreEdge() override;
		int Erode() override;
		int Noise() override;
		int Median() override;
		int Dilate() override;
		int GetWidth() override;
		int GetHeight() override;

		int RotateFree(const double& angle, const int& widthOut, const int& heightOut) override;
		int Posterize(const float& level, const float& gamma) override;
		int Solarize(const long& threshold) override;
		int PhotoFiltre(const CRgbaquad& clValue, const int& intensity) override;
		int Fusion(cv::Mat& bitmapSecond, const float& pourcentage) override;
		int BrightnessAndContrast(const double& brightness, const double& contrast) override;
		int RGBFilter(const int& red, const int& green, const int& blue) override;
		int Swirl(const float& radius, const float& angle) override;


		int MotionBlur(const double& radius, const double& sigma, const double& angle) override;

		int CloudsFilter(const CRgbaquad& color1, const CRgbaquad& color2, const float& amplitude,
		                 const float& frequence, const int& octave, const int& intensity) override { return -1; };
		int Contrast(const double& contrast, const uint8_t& offset) { return -1; };
		int Lightness(const double& factor) { return -1; };

		int LensFlare(const int& iPosX, const int& iPosY, const int& iPuissance, const int& iType,
		              const int& iIntensity, const int& iColor, const int& iColorIntensity) override { return -1; };

		int HistogramLog(cv::Mat& bitmap) { return -1; }
		int HistogramNormalize(cv::Mat& bitmap) { return -1; }
		int HistogramEqualize(cv::Mat& bitmap) { return -1; }
		int SuperResolutionNCNN() { return -1; }
		int Colorization() { return -1; }
        int Inpaint(const cv::Mat &mask) { return -1; }
		int BrightnessAndContrastAuto(float clipHistPercent) override;
		cv::Mat GetBitmap(const bool& source) override;

		bool StabilizeVideo(OpenCV::COpenCVStabilization* stabilization) override;

		int DetailEnhance(const double& sigma_s, const double& sigma_r) override;
		int EdgePreservingFilter(const int& flags, const double& sigma_s, const double& sigma_r) override;
		int PencilSketch(const double& sigma_s, const double& sigma_r, const double& shade_factor) override;
		int Stylization(const double& sigma_s, const double& sigma_r) override;
        
	protected:
		COpenCLFilter* openclFilter;
		wxImage GetwxImage(cv::UMat& input);
		int GetSizeData() const;
		cl_mem_flags flag;
		wxString filename;
		cv::Mat alphaChannel;
		cv::UMat input;
		cv::UMat paramOutput;
	};
}
