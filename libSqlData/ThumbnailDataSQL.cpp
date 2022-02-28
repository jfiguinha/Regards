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
using namespace Regards::Picture;

CThumbnailDataSQL::CThumbnailDataSQL(const wxString& filename, const bool& testValidity)
	: CThumbnailData(filename)
{
	CLibPicture libPicture;
	if (libPicture.TestIsVideo(filename) || libPicture.TestIsPDF(filename) || libPicture.TestIsAnimation(filename))
	{
		isVideo = true;
		
	}
	if (libPicture.TestIsVideo(filename))
	{
		videoCapture = new cv::VideoCapture(filename.ToStdString(), cv::CAP_ANY, { cv::CAP_PROP_HW_ACCELERATION,cv::VIDEO_ACCELERATION_ANY });
		//fps = videoCapture->get(cv::CAP_PROP_FPS);
	}
	//if (libPicture.TestIsVideo(filename))
	//	videoCapture = new cv::VideoCapture(filename.ToStdString(), cv::CAP_ANY, { cv::CAP_PROP_HW_ACCELERATION,cv::VIDEO_ACCELERATION_ANY });

}

CThumbnailDataSQL::~CThumbnailDataSQL(void)
{
	if (videoCapture != nullptr)
		delete videoCapture;
}

int CThumbnailDataSQL::GetNbFrame()
{
	if (isVideo) // && nbFrame < 20)
	{
		CSqlThumbnailVideo sqlThumbnailVideo;
		nbFrame = sqlThumbnailVideo.GetNbThumbnail(filename);
	}
	if (nbFrame > 0)
		return nbFrame;
	return 1;
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
	CLibPicture libPicture;
	if (isVideo) // && nbFrame < 20 )
	{
		CSqlThumbnailVideo sqlThumbnailVideo;
		nbFrame = sqlThumbnailVideo.GetNbThumbnail(filename);
	}
    
    if (numFrame >= nbFrame)
		numFrame = 0;
        
   // numFrame = max(numFrame, 0);

	
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

			if(videoCapture != nullptr)
			{
				bool grabbed = false;
				if (oldnumFrame != numFrame)
				{
					if (!videoCapture->read(cvImg))
					{
						videoCapture->set(cv::CAP_PROP_POS_MSEC, 0);
						grabbed = videoCapture->read(cvImg);
					}
					else
						grabbed = true;

					if (grabbed)
					{
						int w = cvImg.cols;
						int h = cvImg.rows;
						cv::cvtColor(cvImg, cvImg, cv::COLOR_BGR2RGB);
						frameOut = wxImage(w, h, cvImg.data, true);
					}
					else
					{
#ifdef WIN32
						wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "\\photo_cancel.png";
#else
						wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "/photo_cancel.png";
#endif
						frameOut.LoadFile(photoCancel, wxBITMAP_TYPE_PNG);
					}
					oldnumFrame = numFrame;
				}


			}
			else if(isVideo)
			{
				CSqlThumbnailVideo sqlThumbnailVideo;
				frameOut = sqlThumbnailVideo.GetThumbnail(filename, numFrame);
				if (!frameOut.IsOk())
				{
					numFrame = 1;
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

void CThumbnailDataSQL::SetBitmap(CImageLoadingFormat* bitmap)
{
	if (bitmap == nullptr)
		return;

	if (bitmap->IsOk())
	{
		//Enregistrement en base de données
		CSqlThumbnail sqlThumbnail;
		wxString filename = bitmap->GetFilename();
		if (!TestBitmap())
		{
			int compressMethod = 0;
			unsigned long outputsize = 0;
			bitmap->ConvertToRGB24(true);
			uint8_t* dest = bitmap->GetJpegData(outputsize);
			//wxString hash = wxMD5::GetFileMD5(filename);
			wxFileName file(filename);
			wxULongLong sizeFile = file.GetSize();
			wxString hash = sizeFile.ToString();
			if (dest != nullptr)
				sqlThumbnail.InsertThumbnail(filename, dest, outputsize, bitmap->GetWidth(), bitmap->GetHeight(), hash);

			bitmap->DestroyJpegData(dest);

			dest = nullptr;

			this->filename = filename;
			//pictureLoad = true;
		}
	}
}
