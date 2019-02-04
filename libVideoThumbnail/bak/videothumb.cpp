#include "videothumb.h"
#include "videothumbnailer.h"
#include "Metadata.h"
#include "stringoperations.h"
#include "filmstripfilter.h"
#include <ximage.h>
#include <ImageVideoThumbnail.h>
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

	//Metadata metadata;
	//metadata.generateMetadata(file);
	try
	{
		VideoThumbnailer videoThumbnailer(thumbnailSize, workaroundIssues, maintainAspectRatio, imageQuality, smartFrameSelection);
		videoThumbnailer.DecodeFile(fileName.ToStdString());
		width = videoThumbnailer.GetWidth();
		height = videoThumbnailer.GetHeight();
		rotation = videoThumbnailer.GetOrientation();
	}
	catch (...)
	{

	}
}

CxImage * CThumbnailVideo::GetVideoFrame(const wxString & fileName, int &rotation, const int &percent, int &timePosition)
{
	CxImage * image = new CxImage();
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

		videoThumbnailer.generateThumbnail(fileName.ToStdString(), image);
		rotation = videoThumbnailer.GetOrientation();
		timePosition = videoThumbnailer.GetTimePosition();
		delete filmStripFilter;
	}
	catch (...)
	{

	}
	return image;
}

CxImage * CThumbnailVideo::GetVideoFrame(const wxString & fileName, int &rotation)
{
	int timePosition = 0;
	return GetVideoFrame(fileName, rotation, 10, timePosition);
}

vector<CImageVideoThumbnail *> CThumbnailVideo::GetVideoListFrame(const wxString & fileName, int &width, int &height)
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

	MovieDecoder * movie = videoThumbnailer.MovieDecoderThumbnail(fileName.ToStdString());

	int rotation = videoThumbnailer.GetOrientation();
	width = videoThumbnailer.GetWidth();
	height = videoThumbnailer.GetHeight();

	vector<CImageVideoThumbnail *> listPicture;
	for (int i = 0; i <= 100; i += 5)
	{
		CImageVideoThumbnail * cxVideo = new CImageVideoThumbnail();
		cxVideo->image = new CxImage();
		cxVideo->rotation = rotation;
		cxVideo->percent = i;
		cxVideo->timePosition = videoThumbnailer.GetThumbnail(fileName.ToStdString(), cxVideo->image, movie, cxVideo->percent);
		listPicture.push_back(cxVideo);
	}

	videoThumbnailer.DestroyMovieDecoder(movie);
	delete filmStripFilter;
	return listPicture;
}