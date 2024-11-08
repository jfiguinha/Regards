#pragma once

class COpenCVStabilizationPimpl_;

namespace cv
{
	class UMat;
	class Mat;
}

namespace Regards
{
	namespace OpenCV
	{
		class COpenCVStabilization
		{
		public:
			COpenCVStabilization(const int& nbFrame);
			~COpenCVStabilization();

			void AddFrame(const cv::Mat& pictureData);
			void BufferFrame(const cv::Mat& pBitmap);
			void CorrectFrame(cv::Mat& image);

			void AddFrame(const cv::UMat& pictureData);
			void BufferFrame(const cv::UMat& pBitmap);
			cv::UMat CorrectFrame(cv::UMat& pictureData);

			void AddFrame(const cv::cuda::GpuMat& pictureData);
			void BufferFrame(const cv::cuda::GpuMat& pBitmap);
			cv::cuda::GpuMat CorrectFrame(cv::cuda::GpuMat& pictureData);

			void Init();
			int GetNbFrame();
			int GetNbFrameBuffer();
			void SetNbFrameBuffer(const int& nbFrame);

		private:
			COpenCVStabilizationPimpl_* pimpl;
			int nbFrame = 0;
			int nbFrameBuffer = 0;
		};
	}
}
