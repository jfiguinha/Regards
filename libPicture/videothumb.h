#pragma once
#include "FFmpegVideoThumb.h"
#include "VideoThumbPimpl.h"

class CImageVideoThumbnail;
struct AspectRatio;

namespace Regards
{
	namespace Video
	{
		class CVideoThumb
		{
		public:
			CVideoThumb(const wxString& fileName, const bool& applyOrientation = true, const bool& invertRotation = true);
			~CVideoThumb() = default;
			bool isOk();
			cv::Mat GetVideoFrame(const int& thumbnailWidth, const int& thumbnailHeight);
			cv::Mat GetVideoFramePos(const int64& timePosition, const int& thumbnailWidth, const int& thumbnailHeight);
			cv::Mat GetVideoFramePercent(const int& percent, const int& thumbnailWidth, const int& thumbnailHeight);
			void GetVideoDimensions(int& width, int& height);
			int GetOrientation();
			void GetVideoListFrame(std::vector<std::unique_ptr<CImageVideoThumbnail>> & listPicture, const int& widthThumbnail, const int& heightThumbnail);
			int64_t GetMovieDuration();
            AspectRatio GetAspectRatio();

		private:
			std::unique_ptr<CVideoThumbPimpl> pimpl;
			wxString fileName;
			bool applyOrientation = true;
			bool invertRotation = true;
		};
	}
}
