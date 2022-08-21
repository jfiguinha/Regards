#pragma once

class CVideoPlayerPimpl;

namespace cv
{
	class Mat;
}

namespace Regards {
	namespace Video {
		class CVideoPlayer
		{
		public:
			CVideoPlayer(const wxString &filename, const bool &useHardware = true);
			bool isOpened();
			void SeekToBegin();

			void SkipFrame(const int& nbFrame);
			int SeekToPos(const int& sec);
			
			int GetDuration();
			int GetFps();
			int GetTotalFrame();
			int GetWidth();
			int GetHeight();

			bool IsOk();

			void GetAspectRatio(int & ascpectNominator, int & ascpectDenominator);

			int GetOrientation();
			cv::Mat GetVideoFrame(const bool& applyOrientation = true);
			virtual ~CVideoPlayer();

		private:
			CVideoPlayerPimpl* pimpl;
			bool isOk = false;
		};
	}
}