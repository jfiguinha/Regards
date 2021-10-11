#include "header.h"
#include "videothumb.h"
#include "FFmpegDecodeFrame.h"
#include <ImageVideoThumbnail.h>
#include <ImageLoadingFormat.h>
#include "libPicture.h"
#include <RegardsBitmap.h>
using namespace Regards::Picture;

CThumbnailVideo::CThumbnailVideo(const wxString& fileName)
{
	filename = fileName;
	decodeFrame = new CFFmpegDecodeFrame();
	decodeFrame->OpenFile(fileName);
}

bool CThumbnailVideo::IsOk()
{
	return decodeFrame->IsOk();
}


CThumbnailVideo::~CThumbnailVideo()
{
	if (decodeFrame != nullptr)
		delete decodeFrame;
	decodeFrame = nullptr;
}

CRegardsBitmap* CThumbnailVideo::GetVideoFrame(const int& timePosition, const int& thumbnailWidth, const int& thumbnailHeight, const bool &copy)
{
	int ret = decodeFrame->GetFrameBitmapPosition(timePosition, thumbnailWidth, thumbnailHeight);
	if (ret != 0)
	{
		delete decodeFrame;
		decodeFrame = new CFFmpegDecodeFrame();
		decodeFrame->OpenFile(filename);
		decodeFrame->GetFrameBitmapPosition(timePosition, thumbnailWidth, thumbnailHeight);
	}
	return decodeFrame->GetBitmap(copy);
}

void CThumbnailVideo::GetVideoDimensions(int& width, int& height, int& rotation)
{
	decodeFrame->GetVideoInfos(width, height, rotation);
}

bool CThumbnailVideo::IsHardwareDecoderCompatible()
{
	int ret = decodeFrame->GetFrameBitmapPosition(0, 0, 0);
	return (ret != 0 ? false : true);
}

CRegardsBitmap* CThumbnailVideo::TestIsValidBitmap(CRegardsBitmap* in)
{
	CImageLoadingFormat* imageFormat = nullptr;
	if (in == nullptr)
		imageFormat = CLibPicture::GetCancelPhoto(filename);
	else
		return in;

	CRegardsBitmap* out = imageFormat->GetRegardsBitmap();
	delete imageFormat;
	return out;

}

CRegardsBitmap* CThumbnailVideo::GetVideoFrame(int& rotation, const int& percent, int& timePosition,
                                               const int& thumbnailWidth, const int& thumbnailHeight)
{
	double totalTime = decodeFrame->GetTotalTime();
	int _timePosition = (totalTime * percent) / 100.0f;
	CRegardsBitmap* bitmap = GetVideoFrame(_timePosition, thumbnailWidth, thumbnailHeight, true);
	timePosition = _timePosition;
	/*
	int ret = decodeFrame->GetFrameBitmapPosition(timePosition, thumbnailWidth, thumbnailHeight);
	if (ret != 0)
	{
		delete decodeFrame;
		decodeFrame = new CFFmpegDecodeFrame("");
		decodeFrame->OpenFile(filename);
		decodeFrame->GetFrameBitmapPosition(timePosition, thumbnailWidth, thumbnailHeight);
	}

	rotation = decodeFrame->GetRotation();
	*/

	return TestIsValidBitmap(bitmap);
}

CRegardsBitmap* CThumbnailVideo::GetVideoFrame(const int& thumbnailWidth, const int& thumbnailHeight, int& rotation,
                                               int percent)
{
	int timePosition = 0;
	CRegardsBitmap* image = GetVideoFrame(rotation, percent, timePosition, thumbnailWidth, thumbnailHeight);
	return TestIsValidBitmap(image);
}


int64_t CThumbnailVideo::GetMovieDuration()
{
	return decodeFrame->GetTotalTime();
}

vector<CImageVideoThumbnail*> CThumbnailVideo::GetVideoListFrame(const int& widthThumbnail, const int& heightThumbnail,
                                                                 const bool& compressJpeg)
{
	int rotation = decodeFrame->GetRotation();

	vector<CImageVideoThumbnail*> listPicture;
	for (auto i = 0; i < 100; i += 5)
	{
		auto cxVideo = new CImageVideoThumbnail();
		cxVideo->rotation = rotation;
		cxVideo->percent = i;
		cxVideo->image = new CImageLoadingFormat();
		int timePosition = 0;
		CRegardsBitmap* picture = TestIsValidBitmap(GetVideoFrame(rotation, i, timePosition, widthThumbnail, heightThumbnail));

		cxVideo->timePosition = timePosition;
		if (compressJpeg)
		{
			cxVideo->image->SetPicturToJpeg(picture, false);
			delete picture;
		}
		else
			cxVideo->image->SetPicture(picture);
		cxVideo->image->SetFilename(filename);
		cxVideo->image->SetOrientation(rotation);
		listPicture.push_back(cxVideo);
	}

	return listPicture;
}
