#pragma once
#include <PictureArray.h>

#define TYPE_CPU 1
#define TYPE_OPENCL 2
#ifdef USE_CUDA
#define TYPE_CUDA 3
#endif

namespace cv
{
	class UMat;
	class Mat;
}

namespace Regards
{
	namespace OpenCV
	{

		class COpenCVStabilizationCpu;
		class COpenCVStabilizationOpenCL;
		class COpenCVStabilizationCuda;

		class COpenCVStabilization
		{
		public:
			COpenCVStabilization(const int& nbFrame, const int &type);
			~COpenCVStabilization();

			void AddFrame(Regards::Picture::CPictureArray& pictureData);
			void BufferFrame(Regards::Picture::CPictureArray& pBitmap);
			Regards::Picture::CPictureArray CorrectFrame(Regards::Picture::CPictureArray& image);

			void Init();
			int GetNbFrame();
			int GetNbFrameBuffer();
			void SetNbFrameBuffer(const int& nbFrame);

		private:
			
			COpenCVStabilizationCpu* openCVCpu = nullptr;
			COpenCVStabilizationOpenCL* openCVopenCL = nullptr;
#ifdef USE_CUDA
			COpenCVStabilizationCuda* openCVCuda = nullptr;
#endif
			int type = 0;
		};
	}
}
