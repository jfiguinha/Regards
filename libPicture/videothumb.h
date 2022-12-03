#pragma once

class CImageVideoThumbnail;
class CThumbnailVideoPimpl;

namespace Regards{
	namespace Video{
		class CThumbnailVideo
		{
		public:
			CThumbnailVideo(const wxString& fileName, const bool& useHardware);
			~CThumbnailVideo();
			cv::Mat GetVideoFrame( const int& thumbnailWidth, const int& thumbnailHeight);
			cv::Mat GetVideoFramePos(const int64& timePosition, const int & thumbnailWidth, const int & thumbnailHeight);
			cv::Mat GetVideoFramePercent(const int& percent, const int & thumbnailWidth, const int & thumbnailHeight);
			void GetVideoDimensions(int & width, int & height);
			int GetOrientation();
			vector<CImageVideoThumbnail *> GetVideoListFrame(const int &widthThumbnail,const int &heightThumbnail);
			int64_t GetMovieDuration();

		private:
			CThumbnailVideoPimpl* pimpl;
			wxString fileName;
		};
	}
}

