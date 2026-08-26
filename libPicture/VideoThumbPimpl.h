#pragma once
#include "FFmpegVideoThumb.h"


namespace Regards
{
	namespace Video
	{
		class CVideoThumbPimpl
		{
		public:

			CVideoThumbPimpl(const wxString& fileName);

			bool IsOpen();

			~CVideoThumbPimpl() = default;

			void SetPercent(const int& percent);
			void SetMoviePos(const int64& secondToSeekTo);
			void calculateDimensions(int squareSize, bool maintainAspectRatio, int& destWidth, int& destHeight);
			AspectRatio GetAspectRatio();
			void GetThumbnail(cv::Mat& image, const int& thumbnailWidth, const int& thumbnailHeight, const bool& applyOrientation, const bool& invertRotation);

			int ascpectNominator = 0;
			int ascpectDenominator = 0;
			int64 m_videoMovieDuration = 0;
			int64 m_seekTimeInSecond = 0;
			std::unique_ptr<IVideoPlayer> videoThumbnailer;
			int width = 0;
			int height = 0;
			int videowidth = 0;
			int videoheight = 0;
			int rotation = 0;
			bool isOk = false;
			wxString filename = "";
		};
	}
}