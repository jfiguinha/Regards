#include "header.h"
#include "videothumb.h"
#include "Metadata.h"
#include "FFmpegDecodeFrame.h"
#include <ImageVideoThumbnail.h>
#include <ImageLoadingFormat.h>
#include <RegardsBitmap.h>
#include <ConvertUtility.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <rapidxml.hpp>
using namespace Regards::Video;


CThumbnailVideo::CThumbnailVideo(const wxString & fileName)
{
	filename = fileName;
	wxString decoder = "";
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		decoder = regardsParam->GetVideoDecoderHardware();
	}

	decodeFrame = new CFFmpegDecodeFrame(fileName, decoder);
}


CThumbnailVideo::~CThumbnailVideo()
{
	if (decodeFrame != nullptr)
		delete decodeFrame;
	decodeFrame = nullptr;
}


void CThumbnailVideo::GetVideoDimensions(int & width, int & height, int &rotation)
{
	decodeFrame->GetVideoInfos(width, height, rotation);
}

bool CThumbnailVideo::IsHardwareDecoderCompatible()
{
	CRegardsBitmap * image = new CRegardsBitmap();
	int ret = decodeFrame->GetFrameBitmapPosition(0, image, 0, 0);
	delete image;
	return (ret != 0 ? false : true);
}

CRegardsBitmap * CThumbnailVideo::GetVideoFrame(int &rotation, const int &percent, int &timePosition, const int & thumbnailWidth, const int & thumbnailHeight)
{
	CRegardsBitmap * image = new CRegardsBitmap();
	double totalTime = decodeFrame->GetTotalTime();
	timePosition = (totalTime * percent) / 100.0f;
	int ret = decodeFrame->GetFrameBitmapPosition(timePosition, image, thumbnailWidth, thumbnailHeight);
	if (ret != 0)
	{
		delete decodeFrame;
		decodeFrame = new CFFmpegDecodeFrame(filename, "");
		ret = decodeFrame->GetFrameBitmapPosition(timePosition, image, thumbnailWidth, thumbnailHeight);
	}

	rotation = decodeFrame->GetRotation();


	return image;
}

CRegardsBitmap * CThumbnailVideo::GetVideoFrame(const int & thumbnailWidth, const int & thumbnailHeight, int &rotation, int percent)
{
	int timePosition = 0;
	CRegardsBitmap * image = GetVideoFrame(rotation, percent, timePosition, thumbnailWidth, thumbnailHeight);
	return image;
}

CRegardsBitmap * CThumbnailVideo::GetVideoFrame(const int & thumbnailWidth, const int & thumbnailHeight, int &rotation)
{
	int timePosition = 0;
	CRegardsBitmap * image = GetVideoFrame(rotation, 10, timePosition, thumbnailWidth, thumbnailHeight);

	return image;
}

int64_t CThumbnailVideo::GetMovieDuration()
{
	return decodeFrame->GetTotalTime();
}

vector<CImageVideoThumbnail *> CThumbnailVideo::GetVideoListFrame(const int &widthThumbnail,const int &heightThumbnail, const bool &compressJpeg)
{
	int rotation = decodeFrame->GetRotation();

	vector<CImageVideoThumbnail *> listPicture;
	for (auto i = 0; i < 100; i += 5)
	{
		CImageVideoThumbnail * cxVideo = new CImageVideoThumbnail();
		cxVideo->rotation = rotation;
		cxVideo->percent = i;
		cxVideo->image = new CImageLoadingFormat();
		int timePosition = 0;
		CRegardsBitmap * picture = GetVideoFrame(rotation, i, timePosition, widthThumbnail, heightThumbnail);
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