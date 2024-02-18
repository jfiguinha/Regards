#include <header.h>
#include "ThumbnailDataSQL.h"
#include "SqlThumbnail.h"
#include "SqlThumbnailVideo.h"
#include <wxmd5.h>
#include <libPicture.h>
using namespace Regards::Sqlite;
#include <ImageLoadingFormat.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <FileUtility.h>
#include <opencv2/videoio.hpp>
#include <LoadingResource.h>
#include <ConvertUtility.h>
#include <OpenCVVideoPlayer.h>
using namespace Regards::Video;
using namespace Regards::Picture;

CThumbnailDataSQL::CThumbnailDataSQL(const wxString& filename, const bool& testValidity, const bool & generateVideoPlayer)
	: CThumbnailData(filename)
{

    this->generateVideoPlayer = generateVideoPlayer;
	CLibPicture libPicture;
	if (libPicture.TestIsVideo(filename) || libPicture.TestIsPDF(filename) || libPicture.TestIsAnimation(filename))
	{
		isVideo = true;
	}

	if(isVideo)
	{
		if (libPicture.TestIsVideo(filename))
		{
			nbFrame = 20;
            this->generateVideoPlayer = true;
            videoCaptureCV = new COpenCVVideoPlayer(filename);
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
	if (videoCaptureCV != nullptr)
		delete videoCaptureCV;
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
	return sqlThumbnail.TestThumbnail(filename.Clone(), sizeFile.ToString());
}

wxImage CThumbnailDataSQL::GetwxImage(bool& isDefault)
{
	if (isAnimation && nbFrame == 0)
	{
		CSqlThumbnailVideo sqlThumbnailVideo;
		nbFrame = sqlThumbnailVideo.GetNbThumbnail(filename);
	}

	if (numFrame >= nbFrame)
		numFrame = 0;

	//numFrame = max(numFrame, 0);
    if (isVideo && generateVideoPlayer && !mouseOn && frameOut.IsOk())
        return frameOut;

	if (numFrame == 0 && nbFrame == 0)
	{
		CSqlThumbnail sqlThumbnail;
		//printf("Filename : %s \n",CConvertUtility::ConvertToUTF8(filename));
		frameOut = sqlThumbnail.GetThumbnail(filename.Clone(), isDefault);
		if (isDefault)
			frameOut = GetDefaultPicture();
	}
	else if (isVideo && generateVideoPlayer)
	{
		isDefault = false;

		if (isVideo && videoCaptureCV == nullptr && frameOut.IsOk())
			return frameOut;


		if (numFrame < nbFrame)
		{
			bool grabbed = false;
			if (mouseOn && isVideo && videoCaptureCV != nullptr)
			{
                cvImg = videoCaptureCV->GetVideoFrame(true, true);
				if (cvImg.empty())
				{
					videoFramePos = 0;
					videoCaptureCV->SeekToBegin();
					cvImg = videoCaptureCV->GetVideoFrame(true, true);
					grabbed = true;
				}
				else
				{
					videoFramePos++;
					grabbed = true;
				}

				if (grabbed)
				{
					int w = cvImg.cols;
					int h = cvImg.rows;
					cvtColor(cvImg, cvImg, cv::COLOR_BGR2RGB);
					frameOut = wxImage(w, h, cvImg.data, true);
				}
			}


			if (!grabbed)
			{
				CSqlThumbnailVideo sqlThumbnailVideo;
				frameOut = sqlThumbnailVideo.GetThumbnail(filename, numFrame, isDefault);
				if (isDefault)
				{
					frameOut = sqlThumbnailVideo.GetThumbnail(filename, 0, isDefault);
				}
			}
		}
	}
    else if(isVideo && !generateVideoPlayer)
    {
        CSqlThumbnailVideo sqlThumbnailVideo;
        frameOut = sqlThumbnailVideo.GetThumbnail(filename, numFrame, isDefault);
        if (isDefault)
        {
            frameOut = sqlThumbnailVideo.GetThumbnail(filename, 0, isDefault);
        }
    }
	else if (isAnimation)
	{
		CSqlThumbnailVideo sqlThumbnailVideo;
		frameOut = sqlThumbnailVideo.GetThumbnail(filename, numFrame, isDefault);
		if (isDefault)
		{
			frameOut = sqlThumbnailVideo.GetThumbnail(filename, 0, isDefault);
		}
	}


	if (isDefault)
	{
		frameOut.Clear();
		
		return GetDefaultPicture();
	}
		

	return frameOut;
}
