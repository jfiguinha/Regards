#include "header.h"
#include "videothumb.h"
#include "videothumbnailer.h"
#include "Metadata.h"
#include "stringoperations.h"
#include "filmstripfilter.h"
#include <ximage.h>
#include <ImageVideoThumbnail.h>
#include <ImageLoadingFormat.h>
#include <RegardsBitmap.h>
#include <ConvertUtility.h>
using namespace Regards::Video;
using namespace ffmpegthumbnailer;

CThumbnailVideo::CThumbnailVideo()
{
}


CThumbnailVideo::~CThumbnailVideo()
{
}


void CThumbnailVideo::GetVideoDimensions(const wxString & fileName, int & width, int & height, int &rotation)
{
	//int     seekPercentage = 10;
	int     thumbnailSize = 0;
	int     imageQuality = 10;
	//bool    filmStripOverlay = false;
	bool    workaroundIssues = false;
	bool    maintainAspectRatio = true;
	bool    smartFrameSelection = false;
	wxString  seekTime;
    std::string path = CConvertUtility::ConvertToUTF8(fileName);
	//Metadata metadata;
	//metadata.generateMetadata(file);
	try
	{
		VideoThumbnailer videoThumbnailer(thumbnailSize, workaroundIssues, maintainAspectRatio, imageQuality, smartFrameSelection);
		videoThumbnailer.DecodeFile(path);
		width = videoThumbnailer.GetWidth();
		height = videoThumbnailer.GetHeight();
		rotation = videoThumbnailer.GetOrientation();
	}
	catch (...)
	{

	}
}

CRegardsBitmap * CThumbnailVideo::GetVideoFrame(const wxString & fileName, int &rotation, const int &percent, int &timePosition, const int & thumbnailWidth, const int & thumbnailHeight)
{
    printf("GetVideoFrame \n");
    std::string path = CConvertUtility::ConvertToUTF8(fileName);
	CRegardsBitmap * image = new CRegardsBitmap();
	int     seekPercentage = percent;
	int     thumbnailSize = 0;
	int     imageQuality = 10;
	bool    filmStripOverlay = false;
	bool    workaroundIssues = false;
	bool    maintainAspectRatio = true;
	bool    smartFrameSelection = false;
	wxString  seekTime;

	try
	{
		VideoThumbnailer videoThumbnailer(thumbnailSize, workaroundIssues, maintainAspectRatio, imageQuality, smartFrameSelection);
		FilmStripFilter* filmStripFilter = nullptr;

		if (filmStripOverlay)
		{
			filmStripFilter = new FilmStripFilter();
			videoThumbnailer.addFilter(filmStripFilter);
		}

		if (!seekTime.empty())
		{
			videoThumbnailer.setSeekTime(seekTime.ToStdString());
		}
		else
		{
			videoThumbnailer.setSeekPercentage(seekPercentage);
		}

		image->SetFilename(fileName);
		videoThumbnailer.generateThumbnail(path, image, thumbnailWidth, thumbnailHeight, rotation);
		
		timePosition = videoThumbnailer.GetTimePosition();
		delete filmStripFilter;
	}
	catch (...)
	{
		if(image != nullptr)
			delete image;
		image = nullptr;
	}
	return image;
}

CRegardsBitmap * CThumbnailVideo::GetVideoFrame(const wxString & fileName, const int & thumbnailWidth, const int & thumbnailHeight, int &rotation)
{
	int timePosition = 0;
	CRegardsBitmap * image = GetVideoFrame(fileName, rotation, 10, timePosition, thumbnailWidth, thumbnailHeight);

	return image;
}

int64_t CThumbnailVideo::GetMovieDuration(const wxString & fileName)
{
    std::string path = CConvertUtility::ConvertToUTF8(fileName);
	return VideoThumbnailer::GetMovieDuration(path);
}

vector<CImageVideoThumbnail *> CThumbnailVideo::GetVideoListFrame(const wxString & fileName,const int &widthThumbnail,const int &heightThumbnail, const bool &compressJpeg)
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

    std::string path = CConvertUtility::ConvertToUTF8(fileName);
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
				timePosition = videoThumbnailer.GetThumbnail(path, picture, movie, cxVideo->percent, widthThumbnail, heightThumbnail, rotation);
			}
			catch(...)
			{
				if(picture != nullptr)
					delete picture;
				picture = nullptr;
			}
			cxVideo->timePosition = timePosition;
			if(compressJpeg)
				cxVideo->image->SetPicturToJpeg(picture);
			else
				cxVideo->image->SetPicture(picture);
			delete picture;
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