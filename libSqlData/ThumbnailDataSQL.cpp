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
#include <ConvertUtility.h>
#include <OpenCVVideoPlayer.h>
using namespace Regards::Video;
using namespace Regards::Picture;

CThumbnailDataSQL::CThumbnailDataSQL(const wxString& filename, const bool& testValidity)
	: CThumbnailData(filename)
{

	videoCaptureCV = nullptr;
	CLibPicture libPicture;
	if (libPicture.TestIsVideo(filename) || libPicture.TestIsPDF(filename) || libPicture.TestIsAnimation(filename))
	{
		isVideo = true;
	}

	if(isVideo)
	{
		if (libPicture.TestIsVideo(filename))
			nbFrame = 20;
		else
		{
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
	if (isVideo && videoCaptureCV == nullptr)
	{
		videoCaptureCV = new cv::VideoCapture(CConvertUtility::ConvertToUTF8(filename), cv::CAP_FFMPEG);
		videoCaptureCV->set(cv::CAP_PROP_POS_FRAMES, videoFramePos);
	}

	mouseOn = true;
}

void CThumbnailDataSQL::SetMouseOut()
{
	mouseOn = false;

	if (videoCaptureCV != nullptr)
		delete videoCaptureCV;
	videoCaptureCV = nullptr;

	oldVideoFrame = videoFramePos;
}

bool CThumbnailDataSQL::TestBitmap()
{
	CSqlThumbnail sqlThumbnail;
	wxFileName file(filename);
	wxULongLong sizeFile = file.GetSize();
	return sqlThumbnail.TestThumbnail(filename.Clone(), sizeFile.ToString());
}

wxImage CThumbnailDataSQL::GetwxImage()
{
	//CLibPicture libPicture;

	if (numFrame >= nbFrame)
		numFrame = 0;

	//numFrame = max(numFrame, 0);


	if (numFrame == 0 && nbFrame == 0)
	{
		CSqlThumbnail sqlThumbnail;
		//printf("Filename : %s \n",CConvertUtility::ConvertToUTF8(filename));
		frameOut = sqlThumbnail.GetThumbnail(filename.Clone());
	}
	else
	{
		if (numFrame < nbFrame)
		{
			bool grabbed = false;
			if (mouseOn)
			{
				if (!videoCaptureCV->read(cvImg))
				{
					videoFramePos = 0;
					videoCaptureCV->set(cv::CAP_PROP_POS_MSEC, 0);
					grabbed = videoCaptureCV->read(cvImg);
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
				frameOut = sqlThumbnailVideo.GetThumbnail(filename, numFrame);
				if (!frameOut.IsOk())
				{
					frameOut = sqlThumbnailVideo.GetThumbnail(filename, 0);
				}

				if (!frameOut.IsOk())
				{
					numFrame = 0;
#ifdef WIN32
					wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "\\photo_cancel.png";
#else
					wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "/photo_cancel.png";
#endif
					frameOut.LoadFile(photoCancel, wxBITMAP_TYPE_PNG);
				}
			}
		}
	}
	return frameOut;
}

void CThumbnailDataSQL::SetBitmap(wxImage bitmap)
{
	if (bitmap.IsOk())
	{
		//Enregistrement en base de données
		CSqlThumbnail sqlThumbnail;
		if (!TestBitmap())
		{
			int compressMethod = 0;
			unsigned long outputsize = 0;
			//bitmap->Flip();
			wxMemoryOutputStream memOut;
			bitmap.SaveFile(memOut, wxBITMAP_TYPE_JPEG);
			std::vector<uchar> buffer(memOut.GetLength());
			memOut.CopyTo(&buffer.at(0), memOut.GetLength());
			wxFileName file(filename);
			wxULongLong sizeFile = file.GetSize();
			wxString hash = sizeFile.ToString();
			if (memOut.GetSize() > 0)
				sqlThumbnail.InsertThumbnail(filename, buffer, bitmap.GetWidth(), bitmap.GetHeight(), hash);
		}
	}
}
