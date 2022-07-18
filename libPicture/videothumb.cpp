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

	void GetThumbnail(CRegardsBitmap * & image, const int & thumbnailWidth, const int& thumbnailHeight)
	{
		if (m_seekTimeInSecond > 0)
			videoThumbnailer->SeekToPos(m_seekTimeInSecond);

		cv::Mat out = videoThumbnailer->GetVideoFrame();
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

CThumbnailVideo::CThumbnailVideo(const wxString& fileName)
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
	return pimpl->rotation;
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
	for (auto i = 0; i < 100; i += 5)
	{
		try
		{
			CImageVideoThumbnail * cxVideo = new CImageVideoThumbnail();
			CRegardsBitmap * picture = nullptr;
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
			catch(...)
			{
				if(picture != nullptr)
					delete picture;
				picture = nullptr;
			}
			cxVideo->timePosition = pimpl->m_seekTimeInSecond;
			cxVideo->image->SetPicture(picture);
			cxVideo->image->SetFilename(fileName);
			cxVideo->image->SetOrientation(0);
			listPicture.push_back(cxVideo);
		}
		catch(...)
		{
			printf("error \n");
		}
	}

	return listPicture;
}