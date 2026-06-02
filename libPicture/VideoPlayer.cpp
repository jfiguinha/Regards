/*
 * Copyright (c) 2017 Jun Zhao
 * Copyright (c) 2017 Kaixuan Liu
 *
 * HW Acceleration API (video decoding) decode sample
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file
 * HW-Accelerated decoding example.
 *
 * @example hw_decode.c
 * This example shows how to do HW-accelerated decoding with output
 * frames from the HW video surfaces.
 */

#include <header.h>
#include "VideoPlayer.h"
#include "moviedecoder.h"
#include <stdio.h>

#include <ConvertUtility.h>


using namespace Regards::Video;


class CVideoPlayerPimpl
{
public:

	MovieDecoder* movieDecoder;

	void GetAspectRatio(int& ascpectNominator, int& ascpectDenominator)
	{
		movieDecoder->GetAspectRatio(ascpectNominator, ascpectDenominator);
	}

	void SeekToPos(const int& timeInSeconds)
	{
		movieDecoder->seek(timeInSeconds);

	}

	int GetWidth()
	{
		return movieDecoder->getWidth();
	}

	int GetHeight()
	{
		return movieDecoder->getHeight();
	}	



	CVideoPlayerPimpl()
	{
		movieDecoder = new MovieDecoder();
	}

	~CVideoPlayerPimpl()
	{
		delete movieDecoder;
	}

	bool isOpened()
	{
		return movieDecoder->IsOpened();
	}


	int OpenVideoFile(const char* videoFilename)
	{
		movieDecoder->initialize(videoFilename, true);
		return 1;
	}
    
	int GetDuration()
	{
		return movieDecoder->getDuration();
	}

	void SeekToBegin()
	{
		movieDecoder->seek(0);
	}

	cv::Mat GetVideoThumbnailFrame(const int& thumbnailWidth, const int& thumbnailHeight)
	{
		return movieDecoder->getScaledVideoFrame(std::to_string(thumbnailWidth), true);
	}

	cv::Mat GetVideoFrame(const bool& applyOrientation, const bool& invertRotation)
	{
		return movieDecoder->getScaledVideoFrame(std::to_string(movieDecoder->getWidth()), true);
	}

	int GetOrientation()
	{
		return movieDecoder->GetRotation();
	}

	int GetFps()
	{
		return movieDecoder->getFps();
	}

	void Seek(const int& nbSeconds)
	{
		movieDecoder->seek(nbSeconds);
	}
};

void CVideoPlayer::SeekToBegin()
{
	if (IsOk())
		pimpl->SeekToBegin();
}

AspectRatio CVideoPlayer::GetAspectRatio()
{
	int ascpectNominator = 0, ascpectDenominator = 0;
	pimpl->GetAspectRatio(ascpectNominator, ascpectDenominator);
    AspectRatio aspectRatio;
    aspectRatio.num = ascpectNominator;
    aspectRatio.den = ascpectDenominator;
    return aspectRatio;
}

void CVideoPlayer::SkipFrame(const int& nbFrame)
{
	if (IsOk())
		pimpl->Seek(nbFrame / pimpl->GetFps());
}

bool CVideoPlayer::IsOk()
{
	return pimpl->isOpened();
}

int CVideoPlayer::GetFps()
{
	if (IsOk())
		return pimpl->GetFps();
	return -1;
}

int CVideoPlayer::GetTotalFrame()
{
	if (IsOk())
		return pimpl->GetFps() * pimpl->GetDuration();
	return -1;
}

int CVideoPlayer::GetWidth()
{
	if (IsOk())
		return pimpl->GetWidth();
	return -1;
}

int CVideoPlayer::GetHeight()
{
	if (IsOk())
		return pimpl->GetHeight();
	return -1;
}

void CVideoPlayer::GetAspectRatio(int& ascpectNominator, int& ascpectDenominator)
{
	if (IsOk())
		pimpl->GetAspectRatio(ascpectNominator, ascpectDenominator);
}

int CVideoPlayer::GetDuration()
{
	if (IsOk())
		return pimpl->GetDuration();
	return -1;
}

int CVideoPlayer::GetOrientation()
{
	if (IsOk())
		return pimpl->GetOrientation();
	return -1;
}

bool CVideoPlayer::isOpened()
{
	return pimpl->isOpened();
}

int CVideoPlayer::SeekToPos(const int& sec)
{
	if (IsOk())
		pimpl->SeekToPos(sec);
		return 0;

	return -1;
}

CVideoPlayer::CVideoPlayer(const wxString& filename) : IVideoPlayer(filename)
{
	int ret = 0;
	pimpl = new CVideoPlayerPimpl();
	ret = pimpl->OpenVideoFile(CConvertUtility::ConvertToUTF8(filename));
	this->filename = filename;
	try
	{
		pimpl->SeekToPos(0);
	}
	catch (...)
	{

	}
}

cv::Mat CVideoPlayer::GetVideoFrame(const bool& applyOrientation, const bool& invertRotation)
{
	if (IsOk())
	{
		cv::Mat image = pimpl->GetVideoFrame(applyOrientation, invertRotation);

		if (applyOrientation)
		{
			int orientation = pimpl->GetOrientation();
            switch (orientation)
            {
            case -270:
            case 90:
                cv::rotate(image, image, invertRotation ? cv::ROTATE_90_CLOCKWISE : cv::ROTATE_90_COUNTERCLOCKWISE);
                break;
            case 180:
                cv::rotate(image, image, cv::ROTATE_180);
                break;
            case -90:
            case 270:
                cv::rotate(image, image, invertRotation ? cv::ROTATE_90_COUNTERCLOCKWISE : cv::ROTATE_90_CLOCKWISE);
                break;
            }

		}

		return image;

	}
	return cv::Mat();

}

cv::Mat CVideoPlayer::GetVideoThumbnailFrame(const int& thumbnailWidth, const int& thumbnailHeight)
{
	if (IsOk())
	{
		return pimpl->GetVideoThumbnailFrame(thumbnailWidth, thumbnailHeight);
	}
	return cv::Mat();
}

CVideoPlayer::~CVideoPlayer()
{
	if(pimpl != nullptr)
		delete pimpl;
}
