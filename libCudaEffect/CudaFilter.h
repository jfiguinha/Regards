#pragma once

#ifndef __APPLE__

#include <RGBAQuad.h>
#include <vector>
#include <hqdn3d.h>
#include <GLTexture.h>
using namespace Regards::OpenGL;
class CRegardsBitmap;


namespace Regards
{
	namespace Cuda
	{
		class CSuperSampling;

		class CCudaFilter
		{
		public:
			CCudaFilter();

			virtual ~CCudaFilter();

			void Emboss(cv::cuda::GpuMat& inputData);

			void Sharpen(cv::cuda::GpuMat& inputData);
			void Edge(cv::cuda::GpuMat& inputData);
			void SharpenStrong(cv::cuda::GpuMat& inputData);
			void HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault,
				cv::cuda::GpuMat& inputData);
			uint8_t* HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault,
				cv::Mat& inputData);
			//cv::cuda::GpuMat Denoise(const wxString &functionName, const float &sigma, const float &threshold, const float &kSigma, cv::cuda::GpuMat & inputData);
			void FiltreConvolution(const wxString& programName, const wxString& functionName, cv::cuda::GpuMat& inputData);
			void MotionBlurCompute(const vector<double>& kernelMotion, const vector<wxPoint>& offsets, const int& kernelSize,
				cv::cuda::GpuMat& inputData);
			void SharpenMasking(const float& sharpness, cv::cuda::GpuMat& inputData);
			void PhotoFiltre(const CRgbaquad& clValue, const int& intensity, cv::cuda::GpuMat& inputData);
			void RGBFilter(const int& red, const int& green, const int& blue, cv::cuda::GpuMat& inputData);
			void FiltreMosaic(cv::cuda::GpuMat& inputData, const int& size);
			void Blur(const int& radius, cv::cuda::GpuMat& inputData);
			void GaussianBlur(const int& radius, const int& boxSize, cv::cuda::GpuMat& inputData);
			void ErodeDilate(const wxString& functionName, cv::cuda::GpuMat& inputData);
			void Posterize(const float& level, const float& gamma, cv::cuda::GpuMat& inputData);
			void Solarize(const long& threshold, cv::cuda::GpuMat& inputData);
			void Median(cv::cuda::GpuMat& inputData);
			void Noise(cv::cuda::GpuMat& inputData);
			void Flip(const wxString& functionName, cv::cuda::GpuMat& inputData);
			void LensDistortion(const float& strength, cv::cuda::GpuMat& inputData);
			void Swirl(const float& radius, const float& angle, cv::cuda::GpuMat& inputData);
			void BrightnessAndContrast(const double& brightness, const double& contrast, cv::cuda::GpuMat& inputData);
			void ColorEffect(const wxString& functionName, cv::cuda::GpuMat& inputData);
			void Rotate(const wxString& functionName, const int& widthOut, const int& heightOut, const double& angle,
				cv::cuda::GpuMat& inputData);
			cv::cuda::GpuMat Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
				cv::cuda::GpuMat& inputData, int flipH, int flipV, int angle, int ratio);
			void Fusion(cv::cuda::GpuMat& inputData, const cv::cuda::GpuMat& secondPictureData, const float& pourcentage);
			void BrightnessAndContrastAuto(cv::cuda::GpuMat& inputData, float clipHistPercent);
			void BilateralEffect(cv::cuda::GpuMat& inputData, const int& fSize, const int& sigmaX, const int& sigmaP);
			void Bm3d(cv::cuda::GpuMat& inputData, const float& fSigma);

			void DetailEnhance(cv::cuda::GpuMat& inputData, const double& sigma_s, const double& sigma_r);
			void EdgePreservingFilter(cv::cuda::GpuMat& inputData, const int& flags, const double& sigma_s, const double& sigma_r);
			void PencilSketch(cv::cuda::GpuMat& inputData, const double& sigma_s, const double& sigma_r, const double& shade_factor);
			void Stylization(cv::cuda::GpuMat& inputData, const double& sigma_s, const double& sigma_r);


			void NlMeans(cv::cuda::GpuMat& inputData, const int& h, const int& hColor, const int& templateWindowSize,
				const int& searchWindowSize);

			static bool isUsed;
			static int numTexture;

		protected:

			Regards::Cuda::CSuperSampling* superSampling;
			cv::Rect CalculRect(int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle,
				float ratioX, float ratioY, int x, int y, float left, float top);

			Chqdn3d* hq3d = nullptr;
			cv::cuda::GpuMat out;
			double oldLevelDenoise = 0;
			int oldwidthDenoise = 0;
			int oldheightDenoise = 0;


		};
	}
}

#endif