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
			CVideoPlayer(const wxString &filename);
			void SeekToBegin();
			int SeekToPos(const int& sec);
			int GetDuration();
			cv::Mat GetVideoFrame();
			virtual ~CVideoPlayer();

		private:
			CVideoPlayerPimpl* pimpl;

		};
	}
}