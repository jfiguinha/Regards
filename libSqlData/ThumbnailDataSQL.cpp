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
#include <VideoPlayer.h>
using namespace Regards::Video;
using namespace Regards::Picture;

CThumbnailDataSQL::CThumbnailDataSQL(const wxString& filename, const bool& testValidity)
	: CThumbnailData(filename)
{
	CLibPicture libPicture;
	if (libPicture.TestIsVideo(filename) || libPicture.TestIsPDF(filename) || libPicture.TestIsAnimation(filename))
	{
		isVideo = true;
		
	}

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
		CLibPicture libPicture;
		if (libPicture.TestIsVideo(filename))
			nbFrame = 20;
		else
		{
			CSqlThumbnailVideo sqlThumbnailVideo;
			sqlThumbnailVideo.GetNbThumbnail(filename);
		}
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
		CLibPicture libPicture;
		if (libPicture.TestIsVideo(filename))
			nbFrame = 20;
		else
		{
			CSqlThumbnailVideo sqlThumbnailVideo;
			nbFrame = sqlThumbnailVideo.GetNbThumbnail(filename);
		}
	}
    
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
			if (libPicture.TestIsVideo(filename) && videoCapture == nullptr)
			{
				if(useOpenCV)
					videoCaptureCV = new cv::VideoCapture(CConvertUtility::ConvertToUTF8(filename));
				else
					videoCapture = new CVideoPlayer(filename,false);
				//fps = videoCapture->get(cv::CAP_PROP_FPS);
			}

            bool grabbed = false;
			if(videoCapture != nullptr && videoCapture->IsOk())
			{
				
				if (oldnumFrame != numFrame)
				{
					if (useOpenCV)
					{
						if (!videoCaptureCV->read(cvImg))
						{
							videoCaptureCV->set(cv::CAP_PROP_POS_MSEC, 0);
							grabbed = videoCaptureCV->read(cvImg);
							//time_pos = 0;
						}
						else
							grabbed = true;

						if (grabbed)
						{
							int w = cvImg.cols;
							int h = cvImg.rows;
							cv::cvtColor(cvImg, cvImg, cv::COLOR_BGR2RGB);
							frameOut = wxImage(w, h, cvImg.data, true);
							oldnumFrame = numFrame;
						}
					}
					else
					{
						/*
						time_pos +=10;
						if (videoCapture->SeekToPos(time_pos) == -1)
						{
							videoCapture->SeekToBegin();
							time_pos = 0;
						}*/

						//videoCapture->SkipFrame(20);
						cvImg = videoCapture->GetVideoFrame();
						if (cvImg.empty())
						{
							videoCapture->SeekToBegin();
							cvImg = videoCapture->GetVideoFrame();
							grabbed = true;
							//time_pos = 0;
						}
						else
							grabbed = true;

						if (cvImg.empty())
							grabbed = false;

						if (grabbed)
						{
							int w = cvImg.cols;
							int h = cvImg.rows;
							//cv::cvtColor(cvImg, cvImg, cv::COLOR_BGR2RGB);
							frameOut = CLibPicture::ConvertRegardsBitmapToWXImage(cvImg);
							oldnumFrame = numFrame;
						}
					}
				}
                else
                    grabbed = true;
			}
			
            if(!grabbed)
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
