#pragma once
#include <RGBAQuad.h>
#include "OpenCLParameter.h"
#include <vector>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <hqdn3d.h>
#include <GLTexture.h>
using namespace Regards::OpenGL;
class CRegardsBitmap;
class CSuperSampling;

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLProgram;

		class COpenCLFilter
		{
		public:
			COpenCLFilter();

			virtual ~COpenCLFilter();

			COpenCLProgram* GetProgram(const wxString& numProgram);
			void Emboss(cv::UMat& inputData);

			void Sharpen(cv::UMat& inputData);
			void Edge(cv::UMat& inputData);
			void SharpenStrong(cv::UMat& inputData);
			void HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault,
			            cv::UMat& inputData);
			uint8_t* HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault,
				cv::Mat& inputData);
			//cv::UMat Denoise(const wxString &functionName, const float &sigma, const float &threshold, const float &kSigma, cv::UMat & inputData);
			void FiltreConvolution(const wxString& programName, const wxString& functionName, cv::UMat& inputData);
			void MotionBlurCompute(const vector<double>& kernelMotion, const vector<wxPoint>& offsets, const int& size,
			                       cv::UMat& inputData);
			void SharpenMasking(const float& sharpness, cv::UMat& inputData);
			void PhotoFiltre(const CRgbaquad& clValue, const int& intensity, cv::UMat& inputData);
			void RGBFilter(const int& red, const int& green, const int& blue, cv::UMat& inputData);
			void FiltreMosaic(cv::UMat& inputData, const int& size);
			void Blur(const int& radius, cv::UMat& inputData);
			void GaussianBlur(const int& radius, const int& boxSize, cv::UMat& inputData);
			void ErodeDilate(const wxString& functionName, cv::UMat& inputData);
			void Posterize(const float& level, const float& gamma, cv::UMat& inputData);
			void Solarize(const long& threshold, cv::UMat& inputData);
			void Median(cv::UMat& inputData);
			void Noise(cv::UMat& inputData);
			void Flip(const wxString& functionName, cv::UMat& inputData);
			void LensDistortion(const float& strength, cv::UMat& inputData);
			void Swirl(const float& radius, const float& angle, cv::UMat& inputData);
			void BrightnessAndContrast(const double& brightness, const double& contrast, cv::UMat& inputData);
			void ColorEffect(const wxString& functionName, cv::UMat& inputData);
			void Rotate(const wxString& functionName, const int& widthOut, const int& heightOut, const double& angle,
			            cv::UMat& inputData);
			cv::UMat Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
			                       cv::UMat& inputData, int flipH, int flipV, int angle, int ratio);
			void Fusion(cv::UMat& inputData, const cv::UMat& secondPictureData, const float& pourcentage);
			void BrightnessAndContrastAuto(cv::UMat& inputData, float clipHistPercent);
			void BilateralEffect(cv::UMat& inputData, const int& fSize, const int& sigmaX, const int& sigmaP);
			void Bm3d(cv::UMat& inputData, const float& fSigma);
			void NlMeans(cv::UMat& inputData, const int& h, const int& hColor, const int& templateWindowSize,
			             const int& searchWindowSize);
			static cv::UMat ExecuteOpenCLCode(const wxString& programName, const wxString& functionName,
			                                  vector<COpenCLParameter*>& vecParam, const int& width, const int& height);

		protected:
			static void ExecuteOpenCLCode(const wxString& programName, const wxString& functionName,
			                              vector<COpenCLParameter*>& vecParam, const int& width, const int& height,
			                              cl_mem& outBuffer);

			void ReallocMat(cv::UMat& inputData);
			int GetRgbaBitmap(cl_mem cl_image, cv::UMat& u);
			CSuperSampling* superSampling;
			cv::Rect CalculRect(int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle,
			                    float ratioX, float ratioY, int x, int y, float left, float top);

			cl_mem_flags flag;
			Chqdn3d* hq3d = nullptr;
			double oldLevelDenoise = 0;
			int oldwidthDenoise = 0;
			int oldheightDenoise = 0;
		};
	}
}
