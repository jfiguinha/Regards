#include "header.h"
#include "videothumb.h"
#include <ImageVideoThumbnail.h>
#include <ImageLoadingFormat.h>
#include "libPicture.h"
#include <RegardsBitmap.h>
#include <opencv2/core/core.hpp>
#include "MediaInfo.h"
#include <ConvertUtility.h>
using namespace Regards::Picture;
using namespace cv;

CThumbnailVideo::CThumbnailVideo(const wxString& fileName)
{
	filename = fileName;
	capture = new cv::VideoCapture(CConvertUtility::ConvertToStdString(fileName));
	isOpen = capture->isOpened();
		
}

CThumbnailVideo::~CThumbnailVideo()
{
	delete capture;
}

int CThumbnailVideo::GetVideoOrientation()
{
	if (!isOpen)
		return 0;
	return capture->get(CAP_PROP_ORIENTATION_META);

}

CRegardsBitmap* CThumbnailVideo::GetVideoFrame(const int& timePosition, const int& thumbnailWidth, const int& thumbnailHeight)
{

	Mat frame;
	CRegardsBitmap* regardBitmap = nullptr;

	if (!isOpen)
	{
		frame = cv::Mat(320, 240, CV_8UC4, cv::Scalar(0, 0, 0));
		regardBitmap = new CRegardsBitmap();
		regardBitmap->SetMatrix(frame);
		return regardBitmap;
	}

	try
	{
		
		
		
		Mat resize;
		double fps = capture->get(CAP_PROP_FPS);
		double noFrame = fps * timePosition;
		if (timePosition != 0)
			capture->set(CAP_PROP_POS_FRAMES, noFrame);

		*capture >> frame;
		cv::cvtColor(frame, frame, cv::COLOR_BGR2BGRA);

		regardBitmap = new CRegardsBitmap();
		if (thumbnailWidth != 0 && thumbnailHeight != 0)
		{
            //Calcul Ratio
            	float new_ratio = 1;

            int pictureWidth = frame.cols, pictureHeight = frame.rows;

            if (pictureWidth > pictureHeight)
                new_ratio = static_cast<float>(thumbnailWidth) / static_cast<float>(pictureWidth);
            else
                new_ratio = static_cast<float>(thumbnailHeight) / static_cast<float>(pictureHeight);

            if ((pictureHeight * new_ratio) > thumbnailHeight)
            {
                new_ratio = static_cast<float>(thumbnailHeight) / static_cast<float>(pictureHeight);
            }
            if ((pictureWidth * new_ratio) > thumbnailWidth)
            {
                new_ratio = static_cast<float>(thumbnailWidth) / static_cast<float>(pictureWidth);
            }
            
            
			cv::resize(frame, resize, cv::Size(pictureWidth * new_ratio, pictureHeight * new_ratio), cv::INTER_CUBIC);
            regardBitmap->SetMatrix(resize);
		}
		else
		{

			regardBitmap->SetMatrix(frame);
		}

		regardBitmap->VertFlipBuf();
	}
	catch(...)
	{
		frame = cv::Mat(320, 240, CV_8UC4, cv::Scalar(0, 0, 0));
		regardBitmap = new CRegardsBitmap();
		regardBitmap->SetMatrix(frame);
	}
	return regardBitmap;

}

void CThumbnailVideo::GetVideoDimensions(int& width, int& height, int& rotation)
{
	if (isOpen)
	{
		width = capture->get(CAP_PROP_FRAME_WIDTH);
		height = capture->get(CAP_PROP_FRAME_HEIGHT);
	}
	else
	{
		width = 0;
		height = 0;
	}
	
}

int64_t CThumbnailVideo::GetMovieDuration()
{
	double fps = capture->get(CAP_PROP_FPS);
	double frame_count = int(capture->get(CAP_PROP_FRAME_COUNT));
	double duration = frame_count / fps;

	if (!isOpen)
		return 0;

	return duration;
}

vector<CImageVideoThumbnail*> CThumbnailVideo::GetVideoListFrame(const int& widthThumbnail, const int& heightThumbnail,
                                                                 const bool& compressJpeg)
{
	vector<CImageVideoThumbnail*> listPicture;
	if (!isOpen)
		return listPicture;

	int rotation = GetVideoOrientation();



	double frame_count = int(capture->get(CAP_PROP_FRAME_COUNT));
	int nbFrame = frame_count / 20;
	double fps = capture->get(CAP_PROP_FPS);
	int pos = 0;
	
	for (auto i = 0; i < 100; i += 5)
	{
		Mat frame;
		auto cxVideo = new CImageVideoThumbnail();
		cxVideo->rotation = rotation;
		cxVideo->percent = i;
		cxVideo->image = new CImageLoadingFormat();
		double timePosition = ((double)pos / (double)fps) * 1000;

		bool success = capture->set(CAP_PROP_POS_MSEC, timePosition);
		*capture >> frame;
		CRegardsBitmap* picture = new CRegardsBitmap();

		cv::cvtColor(frame, frame, cv::COLOR_BGR2BGRA);

		picture->SetMatrix(frame);
		picture->VertFlipBuf();
		cxVideo->timePosition = pos / fps;
		cxVideo->image->SetPicture(picture);
		cxVideo->image->SetFilename(filename);
		cxVideo->image->SetOrientation(0);
		listPicture.push_back(cxVideo);

		pos += nbFrame;
	}

	return listPicture;
}
