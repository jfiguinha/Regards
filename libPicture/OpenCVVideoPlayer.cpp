#include "header.h"
#include "OpenCVVideoPlayer.h"
#include <ImageVideoThumbnail.h>
#include <ImageLoadingFormat.h>
#include "libPicture.h"
#include <opencv2/core/core.hpp>
#include "MediaInfo.h"
#include <ConvertUtility.h>
using namespace Regards::Video;
using namespace cv;

COpenCVVideoPlayer::COpenCVVideoPlayer(const wxString& fileName, const bool& useHardware) : IVideoPlayer(fileName, useHardware)
{
	filename = CConvertUtility::ConvertToStdString(fileName);
	if (useHardware)
		capture = new cv::VideoCapture(filename, cv::CAP_ANY, { cv::CAP_PROP_HW_ACCELERATION,cv::VIDEO_ACCELERATION_ANY });
	else
		capture = new cv::VideoCapture(filename);

	isOpen = capture->isOpened();

}

COpenCVVideoPlayer::~COpenCVVideoPlayer()
{
	delete capture;
}


bool COpenCVVideoPlayer::isOpened()
{
	return isOpen;
}

void COpenCVVideoPlayer::SeekToBegin()
{
	if (isOpen)
		capture->set(CAP_PROP_POS_FRAMES, 0);
}

void COpenCVVideoPlayer::SkipFrame(const int& nbFrame)
{
	if (!isOpen)
		return;

	capture->set(CAP_PROP_POS_FRAMES, nbFrame);

}

int COpenCVVideoPlayer::GetDuration()
{
	double fps = capture->get(CAP_PROP_FPS);
	double frame_count = int(capture->get(CAP_PROP_FRAME_COUNT));
	double duration = frame_count / fps;

	if (!isOpen)
		return 0;

	return duration;
}

int COpenCVVideoPlayer::GetFps()
{
	if(isOpen)
		return capture->get(CAP_PROP_FPS);
	return 0;
}

int COpenCVVideoPlayer::GetTotalFrame()
{
	if (isOpen)
		return capture->get(CAP_PROP_FPS);
	return 0;
}

int COpenCVVideoPlayer::GetWidth()
{
	if (isOpen)
		return capture->get(CAP_PROP_FRAME_WIDTH);
	return 0;
}

int COpenCVVideoPlayer::GetHeight()
{
	if (isOpen)
		return capture->get(CAP_PROP_FRAME_HEIGHT);
	return 0;
}

bool COpenCVVideoPlayer::IsOk()
{
	return isOpen;
}

void COpenCVVideoPlayer::GetAspectRatio(int& ascpectNominator, int& ascpectDenominator)
{
	if (isOpen)
	{
		ascpectNominator = capture->get(CAP_PROP_SAR_NUM);
		ascpectDenominator = capture->get(CAP_PROP_SAR_DEN);
	}
	else
	{
		ascpectNominator = 0;
		ascpectDenominator = 0;
	}
}

int COpenCVVideoPlayer::GetOrientation()
{
	if (!isOpen)
		return 0;
	return capture->get(CAP_PROP_ORIENTATION_META);
}

int COpenCVVideoPlayer::SeekToPos(const int& sec)
{
	if (!isOpen)
		return -1;

	double fps = capture->get(CAP_PROP_FPS);
	double noFrame = fps * sec;
	if (sec != 0)
		capture->set(CAP_PROP_POS_FRAMES, noFrame);

	return 0;
}

cv::Mat COpenCVVideoPlayer::GetVideoFrame(const bool& applyOrientation)
{
	cv::Mat frame;
	*capture >> frame;
	cv::cvtColor(frame, frame, cv::COLOR_BGR2BGRA);

	if(GetOrientation() != 0 && GetOrientation() != 180)
	{
		cv::flip(frame, frame, -1);
	}
	

	return frame;
}