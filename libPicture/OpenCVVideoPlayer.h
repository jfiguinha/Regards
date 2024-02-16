#pragma once
#include "VideoPlayerInterface.h"

namespace cv
{
	class Mat;
	class VideoCapture;
}

namespace Regards
{
	namespace Video
	{
		class COpenCVVideoPlayer : public IVideoPlayer
		{
		public:
			COpenCVVideoPlayer(const wxString& filename);
			virtual ~COpenCVVideoPlayer() override;

			bool isOpened() override;
			void SeekToBegin() override;

			void SkipFrame(const int& nbFrame) override;
			int SeekToPos(const int& sec) override;

			int GetDuration() override;
			int GetFps() override;
			int GetTotalFrame() override;
			int GetWidth() override;
			int GetHeight() override; 
            
            AspectRatio GetAspectRatio() override;

			bool IsOk() override;

			void GetAspectRatio(int& ascpectNominator, int& ascpectDenominator) override;

			int GetOrientation() override;
			cv::Mat GetVideoFrame(const bool& applyOrientation = true, const bool& invertRotation = true) override;
			

		private:

			std::string filename;
			cv::VideoCapture* capture;
			bool isOpen = false;
			int height = 0;
			int width = 0;
		};
	}
}
