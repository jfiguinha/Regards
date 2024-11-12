#pragma once

class COpenCVStabilizationCpuPimpl_;

namespace cv
{
	class UMat;
	class Mat;
}

namespace Regards
{
	namespace OpenCV
	{
		class COpenCVStabilizationCpu
		{
		public:
			COpenCVStabilizationCpu(const int& nbFrame);
			~COpenCVStabilizationCpu();

			void AddFrame(const cv::Mat& pictureData);
			void BufferFrame(const cv::Mat& pBitmap);
			void CorrectFrame(cv::Mat& image);

			void Init();
			int GetNbFrame();
			int GetNbFrameBuffer();
			void SetNbFrameBuffer(const int& nbFrame);

		private:
			COpenCVStabilizationCpuPimpl_* pimpl;
			int nbFrame = 0;
			int nbFrameBuffer = 0;
		};
	}
}
