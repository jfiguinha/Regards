#include "header.h"
#include "videothumb.h"
#include <ImageVideoThumbnail.h>
#include <ImageLoadingFormat.h>
#include "libPicture.h"
#include <RegardsBitmap.h>
#include <opencv2/core/core.hpp>
#include "MediaInfo.h"
using namespace Regards::Picture;
using namespace cv;

CThumbnailVideo::CThumbnailVideo()
{
	

}

void CThumbnailVideo::SetFilename(const wxString& fileName)
{
	filename = fileName;
}

CThumbnailVideo::~CThumbnailVideo()
{

}

int CThumbnailVideo::GetVideoOrientation()
{
	CMediaInfo mediaInfo;
	return mediaInfo.GetVideoRotation(filename);

}

CRegardsBitmap* CThumbnailVideo::GetVideoFrame(const int& timePosition, const int& thumbnailWidth, const int& thumbnailHeight)
{
	VideoCapture capture(filename.ToStdString());
	Mat frame;
	Mat resize;
	double fps = capture.get(CAP_PROP_FPS);
	double noFrame = fps * timePosition;
	bool success = capture.set(CAP_PROP_POS_FRAMES, noFrame);

	if (!capture.isOpened())
		throw "Error when reading steam_avi";

	capture >> frame;
	cv::cvtColor(frame, frame, cv::COLOR_BGR2BGRA);
	
	CRegardsBitmap* regardBitmap = new CRegardsBitmap();
	if (thumbnailWidth != 0 && thumbnailHeight != 0)
	{
		cv::resize(frame, resize, cv::Size(thumbnailWidth, thumbnailHeight), cv::INTER_CUBIC);
	}
	else
	{
	
		regardBitmap->SetMatrix(frame);
	}

	regardBitmap->VertFlipBuf();
	return regardBitmap;

}

void CThumbnailVideo::GetVideoDimensions(int& width, int& height, int& rotation)
{
	VideoCapture capture(filename.ToStdString());
	width = capture.get(CAP_PROP_FRAME_WIDTH);
	height = capture.get(CAP_PROP_FRAME_HEIGHT);
	
}

int64_t CThumbnailVideo::GetMovieDuration()
{
	VideoCapture capture(filename.ToStdString());
	double fps = capture.get(CAP_PROP_FPS);
	double frame_count = int(capture.get(CAP_PROP_FRAME_COUNT));
	double duration = frame_count / fps;
	return duration;
}

vector<CImageVideoThumbnail*> CThumbnailVideo::GetVideoListFrame(const int& widthThumbnail, const int& heightThumbnail,
                                                                 const bool& compressJpeg)
{
	int exifRotation = 0;
	int rotation = GetVideoOrientation();
	VideoCapture capture(filename.ToStdString());
	if (!capture.isOpened())
		throw "Error when reading steam_avi";

	double frame_count = int(capture.get(CAP_PROP_FRAME_COUNT));
	int nbFrame = frame_count / 20;
	int pos = 0;
	vector<CImageVideoThumbnail*> listPicture;
	for (auto i = 0; i < 100; i += 5)
	{
		Mat frame;
		auto cxVideo = new CImageVideoThumbnail();
		cxVideo->rotation = rotation;
		cxVideo->percent = i;
		cxVideo->image = new CImageLoadingFormat();
		int timePosition = 0;

		bool success = capture.set(CAP_PROP_POS_FRAMES, pos);
		capture >> frame;
		CRegardsBitmap* picture = new CRegardsBitmap();

		cv::cvtColor(frame, frame, cv::COLOR_BGR2BGRA);

		picture->SetMatrix(frame);
		picture->VertFlipBuf();
		cxVideo->timePosition = timePosition;
		cxVideo->image->SetPicture(picture);
		cxVideo->image->SetFilename(filename);
		cxVideo->image->SetOrientation(rotation);
		listPicture.push_back(cxVideo);

		pos += nbFrame;
	}

	return listPicture;
}
