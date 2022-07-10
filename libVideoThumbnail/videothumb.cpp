#include "header.h"
#include "videothumb.h"
#include "videothumbnailer.h"
#include "Metadata.h"
#include "stringoperations.h"
#include "filmstripfilter.h"
#include <ximage.h>
#include "ImageVideoThumbnail.h"
#include <ImageLoadingFormat.h>
#include <RegardsBitmap.h>
#include "moviedecoder.h"
#include <ConvertUtility.h>
using namespace Regards::Video;
using namespace ffmpegthumbnailer;

class CThumbnailVideoPimpl
{
public:
	CThumbnailVideoPimpl(const wxString& fileName)
	{
		filename = CConvertUtility::ConvertToStdString(fileName);
		videoThumbnailer = new VideoThumbnailer(thumbnailSize, workaroundIssues, maintainAspectRatio, imageQuality, smartFrameSelection);
		movieDecoder = new MovieDecoder(filename, NULL);
		movieDecoder->decodeVideoFrame(); //before seeking, a frame has to be decoded
		width = movieDecoder->getWidth();
		height = movieDecoder->getHeight();
		rotation = movieDecoder->getOrientation();
	}

	~CThumbnailVideoPimpl()
	{
		delete movieDecoder;
		delete videoThumbnailer;
	}

	void SetPercent(const int& percent)
	{
		int secondToSeekTo = (movieDecoder->getDuration() * percent) / 100;
		m_seekTimeInSecond = secondToSeekTo;
	}

	void SetMoviePos(const int64& secondToSeekTo)
	{
		m_seekTimeInSecond = secondToSeekTo;
	}

	void GetThumbnail(CRegardsBitmap * & image, const int & thumbnailWidth, const int& thumbnailHeight)
	{
		if (m_seekTimeInSecond == -1)
			videoThumbnailer->GetThumbnail(image, movieDecoder, thumbnailWidth, thumbnailHeight, rotation);
		else
			videoThumbnailer->GetThumbnailPos(image, movieDecoder, m_seekTimeInSecond, thumbnailWidth, thumbnailHeight, rotation);
	}

	int64 m_seekTimeInSecond = 0;
	MovieDecoder* movieDecoder = nullptr;
	VideoThumbnailer* videoThumbnailer = nullptr;
	int     thumbnailSize = 0;
	int     imageQuality = 10;
	bool    filmStripOverlay = false;
	bool    workaroundIssues = false;
	bool    maintainAspectRatio = true;
	bool    smartFrameSelection = false;
	int width = 0;
	int height = 0;
	int rotation = 0;
	string filename = "";
};

CThumbnailVideo::CThumbnailVideo(const wxString& fileName)
{
	this->fileName = fileName;
	pimpl = new CThumbnailVideoPimpl(fileName);
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
		pimpl->SetMoviePos(-1);
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
	return pimpl->m_seekTimeInSecond;
}

vector<CImageVideoThumbnail *> CThumbnailVideo::GetVideoListFrame(const wxString & fileName,const int &widthThumbnail,const int &heightThumbnail)
{
	
	int     thumbnailSize = 0;
	int     imageQuality = 10;
	bool    filmStripOverlay = false;
	bool    workaroundIssues = false;
	bool    maintainAspectRatio = true;
	bool    smartFrameSelection = false;
	wxString  seekTime;

	VideoThumbnailer videoThumbnailer(thumbnailSize, workaroundIssues, maintainAspectRatio, imageQuality, smartFrameSelection);
	FilmStripFilter* filmStripFilter = nullptr;

	if (filmStripOverlay)
	{
		filmStripFilter = new FilmStripFilter();
		videoThumbnailer.addFilter(filmStripFilter);
	}

    std::string path = CConvertUtility::ConvertToStdString(fileName);
	MovieDecoder * movie = videoThumbnailer.MovieDecoderThumbnail(path);

	int rotation = videoThumbnailer.GetOrientation();
	//width = videoThumbnailer.GetWidth();
	//height = videoThumbnailer.GetHeight();

	vector<CImageVideoThumbnail *> listPicture;
	for (auto i = 0; i < 100; i += 5)
	{
		try
		{
			CImageVideoThumbnail * cxVideo = new CImageVideoThumbnail();
			CRegardsBitmap * picture = nullptr;
			int timePosition = 0;

		
			cxVideo->rotation = rotation;
			cxVideo->percent = i;
			cxVideo->image = new CImageLoadingFormat();
		

			try
			{
				picture = new CRegardsBitmap();
				timePosition = videoThumbnailer.GetThumbnail(picture, movie, cxVideo->percent, widthThumbnail, heightThumbnail, rotation);
			}
			catch(...)
			{
				if(picture != nullptr)
					delete picture;
				picture = nullptr;
			}
			cxVideo->timePosition = timePosition;
			cxVideo->image->SetPicture(picture);
			cxVideo->image->SetFilename(fileName);
			cxVideo->image->SetOrientation(rotation);
			listPicture.push_back(cxVideo);
		}
		catch(...)
		{
			printf("error \n");
		}
	}

	videoThumbnailer.DestroyMovieDecoder(movie);
	delete filmStripFilter;
	return listPicture;
}