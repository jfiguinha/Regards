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

			void SetFlag(const bool &useMemory)
			{
				flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
			}

			virtual ~COpenCLFilter();

			COpenCLProgram * GetProgram(const wxString &numProgram);
			void Emboss(cv::Mat& inputData);
			
			void Sharpen(cv::Mat& inputData);
			void Edge(cv::Mat& inputData);
			void SharpenStrong(cv::Mat& inputData);
			void HQDn3D(const double & LumSpac, const double & ChromSpac, const double & LumTmp, const double & ChromTmp, cv::Mat & inputData);
			//cv::UMat Denoise(const wxString &functionName, const float &sigma, const float &threshold, const float &kSigma, cv::Mat & inputData);
			void FiltreConvolution(const wxString &programName, const wxString &functionName, cv::Mat& inputData);
			void MotionBlurCompute(const vector<double> & kernelMotion, const vector<wxPoint> & offsets, const int &size, cv::Mat& inputData);
			void SharpenMasking(const float &sharpness, cv::Mat& inputData);
			void PhotoFiltre(const CRgbaquad &clValue, const int &intensity, cv::Mat & inputData);
			void RGBFilter(const int &red, const int &green, const int &blue, cv::Mat & inputData);
			void FiltreMosaic(cv::Mat & inputData);
			void Blur(const int &radius, cv::Mat & inputData);
			void GaussianBlur(const int& radius, const int& boxSize, cv::Mat & inputData);
			void ErodeDilate(const wxString &functionName, cv::Mat & inputData);
			void Posterize(const float &level, const float &gamma, cv::Mat & inputData);
			void Solarize(const long &threshold, cv::Mat & inputData);
			void Median(cv::Mat & inputData);
			void Noise(cv::Mat & inputData);
			void Flip(const wxString &functionName, cv::Mat & inputData);
			void Swirl(const float &radius, const float &angle, cv::Mat & inputData);
			void BrightnessAndContrast(const double &brightness, const double &contrast, cv::Mat & inputData);
			void ColorEffect(const wxString &functionName, cv::Mat & inputData);
			void Rotate(const wxString &functionName, const int &widthOut, const int &heightOut, const double &angle, cv::Mat & inputData);
			cv::Mat Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int& method, cv::Mat & inputData, int flipH, int flipV, int angle, int ratio);
			void Fusion(cv::Mat & inputData, const cv::Mat & secondPictureData, const float &pourcentage);
			void BrightnessAndContrastAuto(cv::Mat & inputData, float clipHistPercent);
			void BilateralEffect(cv::Mat & inputData, const int& fSize, const int& sigmaX, const int& sigmaP);
			void Bm3d(cv::Mat & inputData, const float & fSigma);
			void NlMeans(cv::Mat & inputData, const int& h, const int& hColor, const int& templateWindowSize, const int& searchWindowSize);
			bool convertToGLTexture2D(cv::Mat & inputData, GLTexture* glTexture);

		protected:
			
			CSuperSampling* superSampling;
			cv::Rect CalculRect(int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle, float ratioX, float ratioY, int x, int y, float left, float top);
			int GetSizeData();
			cl_mem_flags  flag;
			Chqdn3d * hq3d = nullptr;
			double oldLevelDenoise = 0;
			int oldwidthDenoise = 0;
			int oldheightDenoise = 0;
		};

	}
}
