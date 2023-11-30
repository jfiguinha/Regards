#pragma once

class CImageVideoThumbnail;
class CVideoThumbPimpl;

namespace Regards
{
	namespace Video
	{
		class CVideoThumb
		{
		public:
			CVideoThumb(const wxString& fileName, const bool& useOpenCV);
			~CVideoThumb();
			bool isOk();
			cv::Mat GetVideoFrame(const int& thumbnailWidth, const int& thumbnailHeight);
			cv::Mat GetVideoFramePos(const int64& timePosition, const int& thumbnailWidth, const int& thumbnailHeight);
			cv::Mat GetVideoFramePercent(const int& percent, const int& thumbnailWidth, const int& thumbnailHeight);
			void GetVideoDimensions(int& width, int& height);
			int GetOrientation();
			vector<CImageVideoThumbnail*> GetVideoListFrame(const int& widthThumbnail, const int& heightThumbnail);
			int64_t GetMovieDuration();

		private:
			CVideoThumbPimpl* pimpl;
			wxString fileName;
		};
	}
}
