#include "header.h"
#include "videothumb.h"
#include "ImageVideoThumbnail.h"
#include "FFmpegVideoThumb.h"
#include <libPicture.h>
#include <ConvertUtility.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
using namespace Regards::Video;
using namespace Regards::Picture;



extern wxImage defaultPicture;

class CVideoThumbPimpl
{
public:
	CVideoThumbPimpl(const wxString& fileName)
	{
		this->filename = fileName;
        videoThumbnailer = std::make_unique<CFFmpegVideoThumb>(fileName);
            
        isOk = videoThumbnailer->IsOk();
        if(isOk)
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

	bool IsOpen()
	{
		return isOk;
	}

	~CVideoThumbPimpl()
	{

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
    
    AspectRatio GetAspectRatio()
    {
       return videoThumbnailer->GetAspectRatio();
    }

	void GetThumbnail(cv::Mat& image, const int& thumbnailWidth, const int& thumbnailHeight, const bool& applyOrientation, const bool& invertRotation)
	{
		if(videoThumbnailer == nullptr)
            return;

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
			

		image = videoThumbnailer->GetVideoFrame(applyOrientation, invertRotation);
		if (image.empty())
			image = CLibPicture::mat_from_wx(defaultPicture);
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

CVideoThumb::CVideoThumb(const wxString& fileName, const bool& applyOrientation, const bool& invertRotation)
{
	this->applyOrientation = applyOrientation;
	this->invertRotation = invertRotation;
	this->fileName = fileName;
    pimpl = std::make_unique<CVideoThumbPimpl>(fileName);
}

bool CVideoThumb::isOk()
{
    if(pimpl != nullptr)
        return pimpl->IsOpen();
    return false;
}

CVideoThumb::~CVideoThumb()
{
}

AspectRatio CVideoThumb::GetAspectRatio()
{
    if(pimpl != nullptr)
        return pimpl->GetAspectRatio();
    return AspectRatio();
}

int CVideoThumb::GetOrientation()
{
	int rotation = pimpl->rotation;
	if (rotation == -90)
		return 90;
	if (rotation == -270)
		return 270;
	return rotation;
}

void CVideoThumb::GetVideoDimensions(int& width, int& height)
{
	width = pimpl->width;
	height = pimpl->height;
}

cv::Mat CVideoThumb::GetVideoFrame(const int& thumbnailWidth, const int& thumbnailHeight)
{
	cv::Mat image;
    if(pimpl != nullptr)
    {
        try
        {
            pimpl->SetMoviePos(0);
            pimpl->GetThumbnail(image, thumbnailWidth, thumbnailHeight, applyOrientation, invertRotation);
        }
        catch (...)
        {
        }
    
    }
	return image;
}

cv::Mat CVideoThumb::GetVideoFramePos(const int64& timePosition, const int& thumbnailWidth,
                                          const int& thumbnailHeight)
{
	cv::Mat image;
    if(pimpl != nullptr)
    {
        try
        {
            pimpl->SetMoviePos(timePosition);
            pimpl->GetThumbnail(image, thumbnailWidth, thumbnailHeight, applyOrientation, invertRotation);
        }
        catch (...)
        {
        }
    }
	return image;
}

cv::Mat CVideoThumb::GetVideoFramePercent(const int& percent, const int& thumbnailWidth, const int& thumbnailHeight)
{
	cv::Mat image;
    if(pimpl != nullptr)
    {
        try
        {
            pimpl->SetPercent(percent);
            pimpl->GetThumbnail(image, thumbnailWidth, thumbnailHeight, applyOrientation, invertRotation);
        }
        catch (...)
        {
        }
    }
	return image;
}


int64_t CVideoThumb::GetMovieDuration()
{
	return pimpl->m_videoMovieDuration;
}

void CVideoThumb::GetVideoListFrame(vector<CImageVideoThumbnail*> & listPicture, const int& widthThumbnail, const int& heightThumbnail)
{
	

	int duration = pimpl->m_videoMovieDuration;

	if (duration < 20)
	{
		for (auto i = 0; i < duration; i++)
		{
			try
			{
				auto cxVideo = new CImageVideoThumbnail();
				cxVideo->rotation = 0;
				cxVideo->filename = fileName;


				try
				{
					pimpl->SetMoviePos(i);
					pimpl->GetThumbnail(cxVideo->image, widthThumbnail, heightThumbnail, applyOrientation, invertRotation);
				}
				catch (...)
				{
				}
				cxVideo->timePosition = i;
				listPicture.push_back(cxVideo);
			}
			catch (...)
			{
				printf("error CImageVideoThumbnail creation\n");
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
				cxVideo->rotation = 0;
				cxVideo->percent = i;
				cxVideo->filename = fileName;

				try
				{
					pimpl->SetPercent(cxVideo->percent);
					pimpl->GetThumbnail(cxVideo->image, widthThumbnail, heightThumbnail, applyOrientation, invertRotation);
				}
				catch (...)
				{
				}
				cxVideo->timePosition = pimpl->m_seekTimeInSecond;
				listPicture.push_back(cxVideo);
			}
			catch (...)
			{
				printf("error CImageVideoThumbnail creation\n");
			}
		}
	}
}
