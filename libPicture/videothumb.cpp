#include "header.h"
#include "videothumb.h"
#include "VideoPlayer.h"
#include "ImageVideoThumbnail.h"
#include "OpenCVVideoPlayer.h"
#include <libPicture.h>
#include <ConvertUtility.h>
using namespace Regards::Video;
using namespace Regards::Picture;

class CThumbnailVideoPimpl
{
public:
	CThumbnailVideoPimpl(const wxString& fileName, const bool& useHardware) 
	{
		this->filename = fileName;
		printf("Filename : %s \n", CConvertUtility::ConvertToUTF8(filename));
#ifdef __WXGTK__
		videoThumbnailer = new COpenCVVideoPlayer(filename, false);
#else
		videoThumbnailer = new CVideoPlayer(filename, false);
#endif

		width = videoThumbnailer->GetWidth();
		height = videoThumbnailer->GetHeight();
		rotation = videoThumbnailer->GetOrientation();
		m_videoMovieDuration = videoThumbnailer->GetDuration();
	}

	~CThumbnailVideoPimpl()
	{
		//delete movieDecoder;
		delete videoThumbnailer;
	}

	void SetPercent(const int& percent)
	{
		int secondToSeekTo = (m_videoMovieDuration * percent) / 100;
		m_seekTimeInSecond = secondToSeekTo;
	}

	void SetMoviePos(const int64& secondToSeekTo)
	{
		m_seekTimeInSecond = secondToSeekTo;
	}

	void calculateDimensions(int squareSize, bool maintainAspectRatio, int& destWidth, int& destHeight)
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
			int srcWidth = width;
			int srcHeight = height;
			int ascpectNominator = 0;
			int ascpectDenominator = 0;
			videoThumbnailer->GetAspectRatio(ascpectNominator, ascpectDenominator);

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

	void GetThumbnail(cv::Mat& image, const int& thumbnailWidth, const int& thumbnailHeight)
	{
		if (m_seekTimeInSecond > 0)
		{
			try
			{
				videoThumbnailer->SeekToPos(m_seekTimeInSecond);
			}
			catch(...)
			{
				return;
			}
		}
			

		image = videoThumbnailer->GetVideoFrame();

		if (thumbnailWidth > 0 && thumbnailHeight > 0)
		{
			int rotation = videoThumbnailer->GetOrientation();
			if (rotation == 0 || rotation == 180)
			{
				int scaledSize = 0;
				bool maintainAspectRatio = true;

				int scaledWidth = thumbnailWidth;
				int scaledHeight = thumbnailHeight;
				calculateDimensions(scaledSize, maintainAspectRatio, scaledWidth, scaledHeight);

				resize(image, image, cv::Size(scaledWidth, scaledHeight));

			}
			else
			{
				int scaledSize = 0;
				bool maintainAspectRatio = true;


				int scaledWidth = thumbnailWidth;
				int scaledHeight = thumbnailHeight;


				calculateDimensions(scaledSize, maintainAspectRatio, scaledWidth, scaledHeight);

				resize(image, image, cv::Size(scaledHeight, scaledWidth));
			}
		}

		//CPictureUtility::ApplyTransform(image);
	}

	int64 m_videoMovieDuration = 0;
	int64 m_seekTimeInSecond = 0;
	IVideoPlayer* videoThumbnailer = nullptr;
	int width = 0;
	int height = 0;
	int rotation = 0;
	wxString filename = "";
};

CThumbnailVideo::CThumbnailVideo(const wxString& fileName, const bool& useHardware)
{
	this->fileName = fileName;
	pimpl = new CThumbnailVideoPimpl(fileName, false);
}


CThumbnailVideo::~CThumbnailVideo()
{
	delete pimpl;
}

int CThumbnailVideo::GetOrientation()
{
	int rotation = pimpl->rotation;
	if (rotation == -90)
		return 90;
	if (rotation == -270)
		return 270;
	return rotation;
}

void CThumbnailVideo::GetVideoDimensions(int& width, int& height)
{
	width = pimpl->width;
	height = pimpl->height;
}

cv::Mat CThumbnailVideo::GetVideoFrame(const int& thumbnailWidth, const int& thumbnailHeight)
{
	cv::Mat image;
	try
	{
		pimpl->SetMoviePos(0);
		pimpl->GetThumbnail(image, thumbnailWidth, thumbnailHeight);
	}
	catch (...)
	{
	}
	return image;
}

cv::Mat CThumbnailVideo::GetVideoFramePos(const int64& timePosition, const int& thumbnailWidth,
                                          const int& thumbnailHeight)
{
	cv::Mat image;
	try
	{
		pimpl->SetMoviePos(timePosition);
		pimpl->GetThumbnail(image, thumbnailWidth, thumbnailHeight);
	}
	catch (...)
	{
	}
	return image;
}

cv::Mat CThumbnailVideo::GetVideoFramePercent(const int& percent, const int& thumbnailWidth, const int& thumbnailHeight)
{
	cv::Mat image;

	try
	{
		pimpl->SetPercent(percent);
		pimpl->GetThumbnail(image, thumbnailWidth, thumbnailHeight);
	}
	catch (...)
	{
	}
	return image;
}


int64_t CThumbnailVideo::GetMovieDuration()
{
	return pimpl->m_videoMovieDuration;
}

vector<CImageVideoThumbnail*> CThumbnailVideo::GetVideoListFrame(const int& widthThumbnail, const int& heightThumbnail)
{
	vector<CImageVideoThumbnail*> listPicture;

	int duration = pimpl->m_videoMovieDuration;

	if (duration < 20)
	{
		for (auto i = 0; i < duration; i++)
		{
			try
			{
				auto cxVideo = new CImageVideoThumbnail();
				cv::Mat image;
				int timePosition = 0;


				cxVideo->rotation = 0;
				cxVideo->filename = fileName;


				try
				{
					pimpl->SetMoviePos(i);
					pimpl->GetThumbnail(image, widthThumbnail, heightThumbnail);
				}
				catch (...)
				{
				}
				cxVideo->timePosition = i;
				cxVideo->image = CLibPicture::ConvertRegardsBitmapToWXImage(image);
				listPicture.push_back(cxVideo);
			}
			catch (...)
			{
				printf("error \n");
			}
		}
	}
	else
	{
		for (auto i = 0; i < 100; i += 5)
		{
			try
			{
				auto cxVideo = new CImageVideoThumbnail();
				cv::Mat image;
				int timePosition = 0;


				cxVideo->rotation = 0;
				cxVideo->percent = i;
				cxVideo->filename = fileName;


				try
				{
					pimpl->SetPercent(cxVideo->percent);
					pimpl->GetThumbnail(image, widthThumbnail, heightThumbnail);
				}
				catch (...)
				{
				}
				cxVideo->timePosition = pimpl->m_seekTimeInSecond;
				cxVideo->image = CLibPicture::ConvertRegardsBitmapToWXImage(image);
				listPicture.push_back(cxVideo);
			}
			catch (...)
			{
				printf("error \n");
			}
		}
	}

	return listPicture;
}
