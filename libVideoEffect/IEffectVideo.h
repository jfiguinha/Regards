#pragma once
#include "VideoStabilization.h"
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <GLTexture.h>

extern "C" {
#include <libavutil/frame.h>
}

class CVideoEffectParameter;

namespace cv
{
	class UMat;
}




namespace Regards
{
	namespace OpenCV
	{
		class IEffectVideo
		{
		public:

			virtual int GetType() = 0;

			virtual bool IsOk() = 0;
			virtual void SetMatrix(cv::Mat& frame) = 0;
			virtual void SetMatrix(cv::cuda::GpuMat& frame) = 0;
			virtual void SetMatrix(cv::UMat& frame) = 0;
			virtual cv::UMat GetUMat(const bool& src = true) = 0;
			virtual cv::Mat GetMatrix(const bool& src = true) = 0;
			virtual cv::cuda::GpuMat GetGpuMat(const bool& src = true) = 0;

			virtual void AutoContrast() = 0;
			virtual void GetYUV420P(uint8_t*& y, uint8_t*& u, uint8_t*& v, const int& widthOut,
				const int& heightOut) = 0;
			virtual void Rotate(CVideoEffectParameter* videoEffectParameter) = 0;
			virtual void ApplyVideoEffect(CVideoEffectParameter* effectParameter) = 0;
			virtual void InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect& rc,
				const int& flipH, const int& flipV, const int& angle,
				const int& bicubic, int ratio) = 0;


			virtual void SetAVFrame(CVideoEffectParameter* videoEffectParameter, AVFrame*& tmp_frame, int colorSpace, int isLimited) = 0;



			virtual void HQDn3D(const double& LumSpac = 4, const double& temporalLumaDefault = 6.0, const double& temporalSpatialLumaDefault = 4.0) = 0;
			virtual void NLMeansDenoise(const double& coeff, const double& templateWindowSize, const double& searchWindowSize) = 0;
			virtual void FlipVertical() = 0;
			virtual void ConvertToBgr() = 0;

			virtual void ApplyStabilization(CVideoEffectParameter* videoEffectParameter, COpenCVStabilization* openCVStabilization) = 0;
			virtual void ApplyOpenCVEffect(CVideoEffectParameter* videoEffectParameter) = 0;

		};
	}
}
