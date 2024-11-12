#pragma once

class COpenCVStabilizationOpenCLPimpl_;

namespace cv
{
	class UMat;
	class Mat;
}

namespace Regards
{
	namespace OpenCV
	{
		class COpenCVStabilizationOpenCL
		{
		public:
			COpenCVStabilizationOpenCL(const int& nbFrame);
			~COpenCVStabilizationOpenCL();

			void AddFrame(const cv::UMat& pictureData);
			void BufferFrame(const cv::UMat& pBitmap);
			cv::UMat CorrectFrame(cv::UMat& pictureData);

			void Init();
			int GetNbFrame();
			int GetNbFrameBuffer();
			void SetNbFrameBuffer(const int& nbFrame);

		private:
			COpenCVStabilizationOpenCLPimpl_ * pimpl;
			int nbFrame = 0;
			int nbFrameBuffer = 0;
		};
	}
}
