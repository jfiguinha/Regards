#pragma once
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <GLTexture.h>

namespace cv
{
	class UMat;
}

namespace Regards
{
	namespace OpenCV
	{
		class COpenCVStabilization;
	}
}

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

		class COpenCLEffectVideo
		{
		public:

			COpenCLEffectVideo();

			virtual ~COpenCLEffectVideo();
			virtual bool IsOk();
			void SetMatrix(cv::Mat& frame);
			void SetMatrix(cv::UMat& frame);
            cv::UMat GetUMat(const bool & src = true);
			cv::Mat GetMatrix(const bool & src = true);

			void AutoContrast();
			virtual void GetYUV420P(uint8_t * & y, uint8_t * & u, uint8_t * & v, const int &widthOut, const int &heightOut);
			void Rotate(CVideoEffectParameter* videoEffectParameter);
			void ApplyVideoEffect(CVideoEffectParameter * effectParameter);
			virtual void InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect &rc, const int &flipH, const int &flipV, const int& angle, const int& bicubic, int ratio);

			void SetNV12(const cv::Mat& yuv);
			void SetNV12(uint8_t* bufferY, int sizeY, uint8_t* bufferUV, int sizeUV, const int& width, const int& height, const int& lineSize, const int& widthOut, const int& heightOut, const int &colorRange, const int &colorSpace);
			void SetNV12(const cv::Mat& yuv, const int& linesize, const int& nWidth, const int& nHeight);
			void SetYUV420P(uint8_t* bufferY, int sizeY, uint8_t* bufferU, int sizeU, uint8_t* bufferV, int sizeV, const int& width, const int& height, const int& lineSize, const int& widthOut, const int& heightOut, const int& colorRange, const int& colorSpace);
			void SetYUV420P(const cv::Mat& y, const cv::Mat& u, const cv::Mat& v, const int& linesize, const int& nWidth, const int& nHeight);

			void HQDn3D( const double & LumSpac, const double & ChromSpac = 4, const double & LumTmp = 3, const double & ChromTmp = 3);
            void FlipVertical();
			void ConvertToBgr();
			bool StabilizeVideo(Regards::OpenCV::COpenCVStabilization* stabilization);
			void ApplyOpenCVEffect(CVideoEffectParameter * videoEffectParameter, Regards::OpenCV::COpenCVStabilization * openCVStabilization);
			int GetDataSizeWidth(const bool &src);

		protected:

			void GetRgbaBitmap(cl_mem cl_image, cv::UMat& inputData, GLTexture* texture, int rgba);
			COpenCLFilter* openclFilter = nullptr;
			wxString filename;
			cv::UMat paramSrc;
			cv::UMat paramOutput;
			cl_mem_flags  flag;
			Chqdn3d* hq3d = nullptr;
			bool interpolatePicture = false;
			bool needToTranscode = false;
			bool isOk = false;
		};

	}
}
