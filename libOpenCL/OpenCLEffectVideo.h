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

			void SetFlag(const bool &useMemory)
			{
				flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
			};

			virtual ~COpenCLEffectVideo();
			virtual bool IsOk();
			void SetMatrix(cv::Mat& frame);
			cv::Mat GetMatrix(const bool & src = true);
			void LoadRegardsBitmap(CRegardsBitmap * bitmap);
			CRegardsBitmap* GetBitmap(const bool &src = false);
			bool convertToGLTexture2D(GLTexture* glTexture);
			bool CopyPictureToTexture2D(GLTexture* texture, const bool& source, int rgba);
			void AutoContrast();
			void GetYUV420P(uint8_t * & y, uint8_t * & u, uint8_t * & v, const int &widthOut, const int &heightOut);
			void Rotate(CVideoEffectParameter* videoEffectParameter);
			void ApplyVideoEffect(CVideoEffectParameter * effectParameter);
			virtual void InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect &rc, const int &flipH, const int &flipV, const int& angle, const int& bicubic, int ratio);
			virtual void TranscodePicture(const int &widthOut, const int &heightOut, const int &rgba=0) {};
			void HQDn3D(Chqdn3d * hq3d, const double & LumSpac, const double & ChromSpac = 4, const double & LumTmp = 3, const double & ChromTmp = 3);
            void FlipVertical();
			void ConvertToBgr();
			bool StabilizeVideo(Regards::OpenCV::COpenCVStabilization* stabilization);
			void ApplyOpenCVEffect(CVideoEffectParameter * videoEffectParameter, Regards::OpenCV::COpenCVStabilization * openCVStabilization);
			int GetDataSizeWidth(const bool &src);

		protected:

			int GetSizeData();
			COpenCLProgram * GetProgram(const wxString &numProgram);
			void GetRgbaBitmap(cl_mem cl_image, cv::UMat& inputData, GLTexture* texture, int rgba);
			cl_mem_flags  flag;

			COpenCLProgram * openCLProgram;
			COpenCLFilter* openclFilter = nullptr;
			wxString filename;
			cv::UMat paramSrc;
			cv::UMat paramOutput;
			bool interpolatePicture = false;
			bool needToTranscode = false;
		};

	}
}
