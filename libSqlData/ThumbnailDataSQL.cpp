#include <header.h>
#include "ThumbnailDataSQL.h"
#include "SqlThumbnail.h"
#include "SqlThumbnailVideo.h"
#include <libPicture.h>
#include <wx/filename.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <FFmpegVideoThumb.h>
#include <SqlPhotos.h>

using namespace Regards::Video;
using namespace Regards::Picture;
using namespace Regards::Sqlite;

CThumbnailDataSQL::CThumbnailDataSQL(const wxString& filename, const bool& testValidity, const bool& generateVideoPlayer)
	: CThumbnailData(filename)
{
	frameOut = GetDefaultPicture();
	this->generateVideoPlayer = generateVideoPlayer;

	if (testValidity && !TestBitmap())
	{
		this->generateVideoPlayer = true;
	}

	CLibPicture libPicture;
	if (libPicture.TestIsVideo(filename) || libPicture.TestIsPDF(filename) || libPicture.TestIsAnimation(filename))
	{
		isVideo = true;
	}

	if (isVideo)
	{
		CSqlPhotos SqlPhotos;
		photoId = SqlPhotos.GetPhotoId(filename);

		if (libPicture.TestIsVideoValid(filename))
		{
			nbFrame = 20;
			this->generateVideoPlayer = true;
		}
		else
		{
			isAnimation = true;
			isVideo = false;

			CSqlThumbnailVideo sqlThumbnailVideo;
			nbFrame = sqlThumbnailVideo.GetNbThumbnail(filename);
		}
	}
}

CThumbnailDataSQL::~CThumbnailDataSQL(void)
{
	frameOut.release();
}

int CThumbnailDataSQL::GetNbFrame()
{
	if (nbFrame > 0)
		return nbFrame;
	return 1;
}

void CThumbnailDataSQL::SetMouseOn()
{
	mouseOn = true;
	// CORRECTION : Reprise automatique de la lecture vidéo là où l'utilisateur s'était arrêté
	videoFramePos = oldVideoFrame;
	if (videoCaptureCV != nullptr && videoFramePos > 0)
	{
		// Optionnel : Ajouter une méthode de Seek si votre décodeur FFmpeg le supporte à l'avenir
	}
}

void CThumbnailDataSQL::SetMouseOut()
{
	mouseOn = false;
	oldVideoFrame = videoFramePos;
}

bool CThumbnailDataSQL::TestBitmap()
{
	CSqlThumbnail sqlThumbnail;
	wxFileName file(filename);
	wxULongLong sizeFile = file.GetSize();
	return sqlThumbnail.TestThumbnail(filename, sizeFile.ToString());
}

cv::Mat CThumbnailDataSQL::GetImage(bool& isDefault)
{
	CSqlThumbnailVideo sqlThumbnailVideo;
	CSqlThumbnail sqlThumbnail;

	if (isAnimation && nbFrame == 0)
	{
		nbFrame = sqlThumbnailVideo.GetNbThumbnail(filename);
	}

	if (numFrame >= nbFrame)
		numFrame = 0;

	if (!defaultPicture)
	{
		if (isVideo && generateVideoPlayer && !mouseOn && !frameOut.empty())
		{
			return frameOut.clone();
		}
	}

	if (!isVideo && numFrame == 0 && nbFrame == 0)
	{
		frameOut = sqlThumbnail.GetThumbnail(filename, isDefault);
		if (isDefault)
			frameOut = GetDefaultPicture();
	}
	else if (isVideo && generateVideoPlayer)
	{
		isDefault = false;

		if (videoCaptureCV == nullptr)
		{
			videoCaptureCV = std::make_unique<CFFmpegVideoThumb>(filename);
		}

		if (videoCaptureCV == nullptr)
		{
			isDefault = frameOut.empty();
			return frameOut.clone();
		}

		if (numFrame < nbFrame)
		{
			bool grabbed = false;

			if (mouseOn && videoCaptureCV != nullptr)
			{
				bool invertRotation = false;

				frameOut = videoCaptureCV->GetVideoFrame(true, invertRotation);
				if (frameOut.empty())
				{
					videoFramePos = 0;
					videoCaptureCV->SeekToBegin();
					frameOut = videoCaptureCV->GetVideoFrame(true, invertRotation);
					grabbed = true;
				}
				else
				{
					videoFramePos++;
					grabbed = true;
				}
			}

			if (!grabbed)
			{
				frameOut = sqlThumbnailVideo.GetThumbnail(photoId, filename, numFrame, isDefault);
			}
		}
	}
	else if (isVideo && !generateVideoPlayer)
	{
		frameOut = sqlThumbnailVideo.GetThumbnail(photoId, filename, numFrame, isDefault);
		if (isDefault && numFrame != 0)
		{
			frameOut = sqlThumbnailVideo.GetThumbnail(photoId, filename, 0, isDefault);
		}
	}
	else if (isAnimation)
	{
		frameOut = sqlThumbnailVideo.GetThumbnail(photoId, filename, numFrame, isDefault);
		if (isDefault && numFrame != 0)
		{
			frameOut = sqlThumbnailVideo.GetThumbnail(photoId, filename, 0, isDefault);
		}
	}

	if (frameOut.empty())
		isDefault = true;

	defaultPicture = isDefault;
	if (isDefault)
	{
		frameOut = GetDefaultPicture();
	}

	return frameOut.clone();
}
