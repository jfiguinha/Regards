#pragma once
#include <IEffectVideo.h>

class CRegardsBitmap;
class CRegardsFloatBitmap;
class CVideoEffectParameter;
class Chqdn3d;

using namespace Regards::OpenGL;
using namespace Regards::OpenCV;

namespace Regards
{
	namespace OpenCV
	{
		class COpenCVStabilization;
	}

	namespace Cuda
	{
		
		class CCudaFilter;

		class CCudaEffectVideo : public Regards::OpenCV::IEffectVideo
		{
		public:
			CCudaEffectVideo();

			virtual ~CCudaEffectVideo();
			virtual bool IsOk();
			void SetMatrix(cv::Mat& frame);
			void SetMatrix(cv::cuda::GpuMat& frame);
			void SetMatrix(cv::UMat& frame);
			cv::cuda::GpuMat GetGpuMat(const bool& src = true);
			cv::UMat GetUMat(const bool& src = true);
			cv::Mat GetMatrix(const bool& src = true);

			void AutoContrast();
			virtual void GetYUV420P(uint8_t*& y, uint8_t*& u, uint8_t*& v, const int& widthOut,
				const int& heightOut);
			void Rotate(CVideoEffectParameter* videoEffectParameter);
			void ApplyVideoEffect(CVideoEffectParameter* effectParameter);
			virtual void InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect& rc,
				const int& flipH, const int& flipV, const int& angle,
				const int& bicubic, int ratio);

			void SetAVFrame(CVideoEffectParameter* videoEffectParameter, AVFrame*& tmp_frame, int colorSpace, int isLimited);


			
			void HQDn3D(const double& LumSpac = 4, const double& temporalLumaDefault = 6.0, const double& temporalSpatialLumaDefault = 4.0);
			void NLMeansDenoise(const double& coeff, const double& templateWindowSize, const double& searchWindowSize);
			void FlipVertical();
			void ConvertToBgr();

			void ApplyStabilization(CVideoEffectParameter* videoEffectParameter, Regards::OpenCV::COpenCVStabilization* openCVStabilization);
			void ApplyOpenCVEffect(CVideoEffectParameter* videoEffectParameter);


		protected:

			uint8_t* HQDn3D(uint8_t* y, int width, int height, const double& LumSpac = 4, const double& temporalLumaDefault = 6.0, const double& temporalSpatialLumaDefault = 4.0);
			CCudaFilter * cudaFilter = nullptr;
			wxString filename;
			cv::Mat convertSrc;
			cv::cuda::GpuMat paramSrc;
			cv::cuda::GpuMat paramOutput;

			uint8_t* src = nullptr;
			int sizesrc = 0;

			Chqdn3d* hq3d = nullptr;
			bool interpolatePicture = false;
			bool needToTranscode = false;
			bool isOk = false;
		};
	}
}
