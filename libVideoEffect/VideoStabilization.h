#pragma once


#define TYPE_CPU 1
#define TYPE_OPENCL 2
#define TYPE_CUDA 3

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

			void AddFrame(const cv::Mat& pictureData);
			void BufferFrame(const cv::Mat& pBitmap);
			void CorrectFrame(cv::Mat& image);

			void AddFrame(const cv::UMat& pictureData);
			void BufferFrame(const cv::UMat& pBitmap);
			cv::UMat CorrectFrame(cv::UMat& image);

			void AddFrame(const cv::cuda::GpuMat& pictureData);
			void BufferFrame(const cv::cuda::GpuMat& pBitmap);
			cv::cuda::GpuMat CorrectFrame(cv::cuda::GpuMat& image);

			void Init();
			int GetNbFrame();
			int GetNbFrameBuffer();
			void SetNbFrameBuffer(const int& nbFrame);

		private:
			
			COpenCVStabilizationCpu* openCVCpu = nullptr;
			COpenCVStabilizationOpenCL* openCVopenCL = nullptr;
			COpenCVStabilizationCuda* openCVCuda = nullptr;
			int type = 0;
		};
	}
}
