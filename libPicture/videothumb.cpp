#include "header.h"
#include "videothumb.h"
#include "ImageVideoThumbnail.h"

#include <libPicture.h>
#include <appcontext.h>
extern AppContext application_context;
using namespace Regards::Video;
using namespace Regards::Picture;


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
        pimpl->SetMoviePos(0);
        pimpl->GetThumbnail(image, thumbnailWidth, thumbnailHeight, applyOrientation, invertRotation);   
    }
	return image;
}

cv::Mat CVideoThumb::GetVideoFramePos(const int64& timePosition, const int& thumbnailWidth,
                                          const int& thumbnailHeight)
{
	cv::Mat image;
    if(pimpl != nullptr)
    {
		pimpl->SetMoviePos(timePosition);
		pimpl->GetThumbnail(image, thumbnailWidth, thumbnailHeight, applyOrientation, invertRotation);
    }
	return image;
}

cv::Mat CVideoThumb::GetVideoFramePercent(const int& percent, const int& thumbnailWidth, const int& thumbnailHeight)
{
	cv::Mat image;
    if(pimpl != nullptr)
    {
        pimpl->SetPercent(percent);
        pimpl->GetThumbnail(image, thumbnailWidth, thumbnailHeight, applyOrientation, invertRotation);
    }
	return image;
}


int64_t CVideoThumb::GetMovieDuration()
{
	return pimpl->m_videoMovieDuration;
}

void CVideoThumb::GetVideoListFrame(std::vector<std::unique_ptr<CImageVideoThumbnail>> & listPicture, const int& widthThumbnail, const int& heightThumbnail)
{
	

	int duration = pimpl->m_videoMovieDuration;

	if (duration < 20)
	{
		for (auto i = 0; i < duration; i++)
		{
			try
			{
				auto cxVideo = std::make_unique<CImageVideoThumbnail>();
				cxVideo->rotation = 0;
				cxVideo->filename = fileName;
				pimpl->SetMoviePos(i);
				pimpl->GetThumbnail(cxVideo->image, widthThumbnail, heightThumbnail, applyOrientation, invertRotation);
				cxVideo->timePosition = i;
				listPicture.push_back(std::move(cxVideo));
			}
			catch (std::exception e)
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
				auto cxVideo = std::make_unique<CImageVideoThumbnail>();
				cxVideo->rotation = 0;
				cxVideo->percent = i;
				cxVideo->filename = fileName;
				pimpl->SetPercent(cxVideo->percent);
				pimpl->GetThumbnail(cxVideo->image, widthThumbnail, heightThumbnail, applyOrientation, invertRotation);
				cxVideo->timePosition = pimpl->m_seekTimeInSecond;
				listPicture.push_back(std::move(cxVideo));
			}
			catch (std::exception e)
			{
				printf("error CImageVideoThumbnail creation\n");
			}
		}
	}
}
