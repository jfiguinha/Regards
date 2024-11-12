#pragma once

class COpenCVStabilizationCudaPimpl_;

namespace cv::cuda
{
	class GpuMat;
}

namespace Regards
{
	namespace OpenCV
	{
		class COpenCVStabilizationCuda
		{
		public:
			COpenCVStabilizationCuda(const int& nbFrame);
			~COpenCVStabilizationCuda();

			void AddFrame(const cv::cuda::GpuMat& pictureData);
			void BufferFrame(const cv::cuda::GpuMat& pBitmap);
			cv::cuda::GpuMat CorrectFrame(cv::cuda::GpuMat& pictureData);

			void Init();
			int GetNbFrame();
			int GetNbFrameBuffer();
			void SetNbFrameBuffer(const int& nbFrame);

		private:
			COpenCVStabilizationCudaPimpl_* pimpl;
			int nbFrame = 0;
			int nbFrameBuffer = 0;
		};
	}
}
