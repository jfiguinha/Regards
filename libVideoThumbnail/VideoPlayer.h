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
			cv::Mat GetVideoFrame();
			virtual ~CVideoPlayer();

		private:
			CVideoPlayerPimpl* pimpl;

		};
	}
}