#include <header.h>
#include "VideoThumbPimpl.h"
#include <appcontext.h>
extern AppContext application_context;

using namespace Regards::Video;

CVideoThumbPimpl::CVideoThumbPimpl(const wxString& fileName)
{
	this->filename = fileName;
	videoThumbnailer = std::make_unique<CFFmpegVideoThumb>(fileName);

	isOk = videoThumbnailer->IsOk();
	if (isOk)
	{
		width = videoThumbnailer->GetWidth();
		height = videoThumbnailer->GetHeight();
		videowidth = videoThumbnailer->GetWidth();
		videoheight = videoThumbnailer->GetHeight();
		rotation = videoThumbnailer->GetOrientation();
		m_videoMovieDuration = videoThumbnailer->GetDuration();
		videoThumbnailer->GetAspectRatio(ascpectNominator, ascpectDenominator);
	}


}

bool CVideoThumbPimpl::IsOpen()
{
	return isOk;
}


void CVideoThumbPimpl::SetPercent(const int& percent)
{
	int secondToSeekTo = (m_videoMovieDuration * percent) / 100;
	m_seekTimeInSecond = secondToSeekTo;
}

void CVideoThumbPimpl::SetMoviePos(const int64& secondToSeekTo)
{
	m_seekTimeInSecond = secondToSeekTo;
}

void CVideoThumbPimpl::calculateDimensions(int squareSize, bool maintainAspectRatio, int& destWidth, int& destHeight)
{
	if (squareSize == 0)
	{
		// use original video size
		squareSize = max(destWidth, destHeight);
	}

	if (!maintainAspectRatio)
	{
		destWidth = squareSize;
		destHeight = squareSize;
	}
	else
	{
		int srcWidth = videowidth;
		int srcHeight = videoheight;


		if (ascpectNominator != 0 && ascpectDenominator != 0)
		{
			srcWidth = srcWidth * ascpectNominator / ascpectDenominator;
		}

		if (srcWidth > srcHeight)
		{
			destWidth = squareSize;
			destHeight = static_cast<int>(static_cast<float>(squareSize) / srcWidth * srcHeight);
		}
		else
		{
			destWidth = static_cast<int>(static_cast<float>(squareSize) / srcHeight * srcWidth);
			destHeight = squareSize;
		}
	}
}

AspectRatio CVideoThumbPimpl::GetAspectRatio()
{
	return videoThumbnailer->GetAspectRatio();
}

void CVideoThumbPimpl::GetThumbnail(cv::Mat& image, const int& thumbnailWidth, const int& thumbnailHeight, const bool& applyOrientation, const bool& invertRotation)
{
	if (videoThumbnailer == nullptr)
		return;

	if (m_seekTimeInSecond > 0)
	{
		try
		{
			videoThumbnailer->SeekToPos(m_seekTimeInSecond);
		}
		catch (...)
		{
			return;
		}
	}


	image = videoThumbnailer->GetVideoFrame(applyOrientation, invertRotation);
	if (image.empty())
		image = application_context.GetDefaultPicture();
	else
	{
		videowidth = image.size().width;
		videoheight = image.size().height;
		//int rotation = videoThumbnailer->GetOrientation();

		if (thumbnailWidth > 0 && thumbnailHeight > 0)
		{
			int scaledSize = 0;
			bool maintainAspectRatio = true;

			int scaledWidth = thumbnailWidth;
			int scaledHeight = thumbnailHeight;
			calculateDimensions(scaledSize, maintainAspectRatio, scaledWidth, scaledHeight);

			resize(image, image, cv::Size(scaledWidth, scaledHeight));

		}
	}

}
