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
	namespace OpenCL
	{
		class COpenCLProgram;
		class COpenCLParameter;
		class COpenCLParameterInt;
		class COpenCLParameterByteArray;
		class COpenCLParameterClMem;
		class COpenCLFilter;

		class COpenCLEffectVideo : public Regards::OpenCV::IEffectVideo
		{
		public:
			COpenCLEffectVideo();

			virtual ~COpenCLEffectVideo();

			int GetType()
			{
				return TYPE_OPENCL;
			}

			virtual bool IsOk();
			void SetMatrix(Regards::Picture::CPictureArray& frame);
			Regards::Picture::CPictureArray GetMatrix(const bool& src = true);

			void AutoContrast();
			virtual void GetYUV420P(uint8_t* & y, uint8_t* & u, uint8_t* & v, const int& widthOut,
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
			void SetNV12(const cv::Mat& yuv);
			void SetNV12(cv::UMat y, uint8_t* bufferUV, int sizeUV, const int& width,
				const int& height, const int& lineSize, const int& widthOut, const int& heightOut,
				const int& colorRange, const int& colorSpace);
			void SetNV12(uint8_t* bufferY, int sizeY, uint8_t* bufferUV, int sizeUV, const int& width,
				const int& height, const int& lineSize, const int& widthOut, const int& heightOut,
				const int& colorRange, const int& colorSpace);
			void SetNV12(const cv::Mat& yuv, const int& linesize, const int& nWidth, const int& nHeight);
			void SetYUV420P(uint8_t* bufferY, int sizeY, uint8_t* bufferU, int sizeU, uint8_t* bufferV, int sizeV,
				const int& width, const int& height, const int& lineSize, const int& widthOut,
				const int& heightOut, const int& colorRange, const int& colorSpace);
			void SetYUV420P(const cv::Mat& y, const cv::Mat& u, const cv::Mat& v, const int& linesize,
				const int& nWidth, const int& nHeight);


			COpenCLFilter* openclFilter = nullptr;
			wxString filename;
			cv::Mat convertSrc;
			cv::UMat paramSrc;
			cv::UMat paramOutput;

			cl_mem_flags flag;
			Chqdn3d* hq3d = nullptr;
			bool interpolatePicture = false;
			bool needToTranscode = false;
			bool isOk = false;
		};
	}
}
