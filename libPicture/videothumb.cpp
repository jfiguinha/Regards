#include "header.h"
#include "videothumb.h"
#include "VideoPlayer.h"
#include "Metadata.h"
#include <ximage.h>
#include "ImageVideoThumbnail.h"
#include <ImageLoadingFormat.h>
#include <RegardsBitmap.h>
#include <ConvertUtility.h>
using namespace Regards::Video;

class CThumbnailVideoPimpl
{
public:
	CThumbnailVideoPimpl(const wxString& fileName, const bool& useHardware)
	{
		filename = CConvertUtility::ConvertToStdString(fileName);
		videoThumbnailer = new CVideoPlayer(filename, useHardware);
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

	void GetThumbnail(CRegardsBitmap * & image, const int & thumbnailWidth, const int& thumbnailHeight)
	{
		if (m_seekTimeInSecond > 0)
			videoThumbnailer->SeekToPos(m_seekTimeInSecond);

		cv::Mat out = videoThumbnailer->GetVideoFrame();

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

				cv::resize(out, out, cv::Size(scaledWidth, scaledHeight));
			}
			else
			{
				int scaledSize = 0;
				bool maintainAspectRatio = true;

				int scaledWidth = thumbnailWidth;
				int scaledHeight = thumbnailHeight;
				calculateDimensions(scaledSize, maintainAspectRatio, scaledWidth, scaledHeight);

				cv::resize(out, out, cv::Size(scaledHeight, scaledWidth));
			}
		}

		image->SetMatrix(out);
		image->ConvertToBgr();
		image->VertFlipBuf();

	}

	int64 m_videoMovieDuration = 0;
	int64 m_seekTimeInSecond = 0;
	CVideoPlayer * videoThumbnailer = nullptr;
	int width = 0;
	int height = 0;
	int rotation = 0;
	string filename = "";
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

void CThumbnailVideo::GetVideoDimensions(int & width, int & height)
{
	width = pimpl->width;
	height = pimpl->height;
}

CRegardsBitmap* CThumbnailVideo::GetVideoFrame(const int& thumbnailWidth, const int& thumbnailHeight)
{
	CRegardsBitmap* image = new CRegardsBitmap();

	try
	{
		image->SetFilename(fileName);
		pimpl->SetMoviePos(0);
		pimpl->GetThumbnail(image, thumbnailWidth, thumbnailHeight);


	}
	catch (...)
	{
		if (image != nullptr)
			delete image;
		image = nullptr;
	}
	return image;
}

CRegardsBitmap* CThumbnailVideo::GetVideoFramePos(const int64& timePosition, const int& thumbnailWidth, const int& thumbnailHeight)
{
	CRegardsBitmap* image = new CRegardsBitmap();

	try
	{
		image->SetFilename(fileName);
		pimpl->SetMoviePos(timePosition);
		pimpl->GetThumbnail(image, thumbnailWidth, thumbnailHeight);

		
	}
	catch (...)
	{
		if (image != nullptr)
			delete image;
		image = nullptr;
	}
	return image;
}

CRegardsBitmap * CThumbnailVideo::GetVideoFramePercent(const int &percent, const int & thumbnailWidth, const int & thumbnailHeight)
{
	CRegardsBitmap * image = new CRegardsBitmap();

	try
	{
		pimpl->SetPercent(percent);
		image->SetFilename(fileName);
		pimpl->GetThumbnail(image, thumbnailWidth, thumbnailHeight);
	}
	catch (...)
	{
		if(image != nullptr)
			delete image;
		image = nullptr;
	}
	return image;
}


int64_t CThumbnailVideo::GetMovieDuration()
{
	return pimpl->m_videoMovieDuration;
}

vector<CImageVideoThumbnail *> CThumbnailVideo::GetVideoListFrame(const int &widthThumbnail,const int &heightThumbnail)
{
	vector<CImageVideoThumbnail *> listPicture;

	int duration = pimpl->m_videoMovieDuration;

	if (duration < 20)
	{
		for (auto i = 0; i < duration; i++)
		{
			try
			{
				CImageVideoThumbnail* cxVideo = new CImageVideoThumbnail();
				CRegardsBitmap* picture = nullptr;
				int timePosition = 0;


				cxVideo->rotation = 0;
				cxVideo->image = new CImageLoadingFormat();


				try
				{
					picture = new CRegardsBitmap();
					pimpl->SetMoviePos(i);
					pimpl->GetThumbnail(picture, widthThumbnail, heightThumbnail);
				}
				catch (...)
				{
					if (picture != nullptr)
						delete picture;
					picture = nullptr;
				}
				cxVideo->timePosition = i;
				cxVideo->image->SetPicture(picture);
				cxVideo->image->SetFilename(fileName);
				cxVideo->image->SetOrientation(0);
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
				CImageVideoThumbnail* cxVideo = new CImageVideoThumbnail();
				CRegardsBitmap* picture = nullptr;
				int timePosition = 0;


				cxVideo->rotation = 0;
				cxVideo->percent = i;
				cxVideo->image = new CImageLoadingFormat();


				try
				{
					picture = new CRegardsBitmap();
					pimpl->SetPercent(cxVideo->percent);
					pimpl->GetThumbnail(picture, widthThumbnail, heightThumbnail);
				}
				catch (...)
				{
					if (picture != nullptr)
						delete picture;
					picture = nullptr;
				}
				cxVideo->timePosition = pimpl->m_seekTimeInSecond;
				cxVideo->image->SetPicture(picture);
				cxVideo->image->SetFilename(fileName);
				cxVideo->image->SetOrientation(0);
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