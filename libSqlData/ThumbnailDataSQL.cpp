#include "ThumbnailDataSQL.h"
#include "SqlThumbnail.h"
#include "SqlThumbnailVideo.h"
#include <wxmd5.h>
#include <jpge.h>
#include <libPicture.h>
#include <ConvertUtility.h>
using namespace jpge;
using namespace Regards::Sqlite;
#include <ImageLoadingFormat.h>
#ifdef TURBOJPEG
#include <turbojpeg.h>
#endif
#include <wx/dir.h>
#include <wx/filename.h>


CThumbnailDataSQL::CThumbnailDataSQL(const wxString & filename, const bool &testValidity)
	: CThumbnailData(filename)
{
	pictureLoad = TestBitmap();
    /*
    param.filename = filename;
    param.listVideo = &listVideo;
     */
    CLibPicture libPicture;
    if(libPicture.TestIsVideo(filename) || libPicture.TestIsAnimation(filename))
    {
        isVideo = true;
    }
    /*
    if(libPicture.TestIsAnimation(filename))
    {
        isAnimation = true;
        listAnimation = libPicture.LoadAllVideoThumbnail(filename);
    }
    */
}

CThumbnailDataSQL::~CThumbnailDataSQL(void)
{
    /*
    if(listAnimation.size() > 0)
    {
        for(CImageVideoThumbnail * frame : listAnimation)
        {
            if(frame !=nullptr)
                delete frame;
        }
    }
     * */
}
/*
int CThumbnailDataSQL::LoadAllThumbnailPictureFromDatabase()
{
    CSqlThumbnailVideo sqlThumbnailVideo;
    int nbResult = sqlThumbnailVideo.GetNbThumbnail(param.filename);
    for(int i= 0;i < nbResult;i++)
    {
        CImageVideoThumbnail * videoThumbnail = sqlThumbnailVideo.GetThumbnail(param.filename, i);
        if(videoThumbnail != nullptr)
            listVideo.push_back(videoThumbnail);
    }
        
    return nbResult;
}
*/
int CThumbnailDataSQL::GetNbFrame()
{
    if(isVideo && nbFrame == 0)
    {
        CSqlThumbnailVideo sqlThumbnailVideo;
        nbFrame = sqlThumbnailVideo.GetNbThumbnail(filename);  
    }
    if(nbFrame > 0)
        return nbFrame;
    return 1;
}

bool CThumbnailDataSQL::TestBitmap()
{
	CSqlThumbnail sqlThumbnail;
	wxFileName file(filename);
	wxULongLong sizeFile = file.GetSize();
	return sqlThumbnail.TestThumbnail(filename, sizeFile.ToString());
}

wxImage CThumbnailDataSQL::GetwxImage()
{
    if(isVideo && nbFrame == 0)
    {
        CSqlThumbnailVideo sqlThumbnailVideo;
        nbFrame = sqlThumbnailVideo.GetNbThumbnail(filename);  
    }
    
	wxImage frameOut;
    if(numFrame == 0 && nbFrame == 0)
    {
        CSqlThumbnail sqlThumbnail;
        printf("Filename : %s \n",CConvertUtility::ConvertToUTF8(filename));
		frameOut = sqlThumbnail.GetThumbnail(filename);
    }
    else
    {
        if(numFrame >= nbFrame)
            numFrame = 0;
        
        if(numFrame < nbFrame)
        {
            if(isVideo)
            {
                CSqlThumbnailVideo sqlThumbnailVideo;
                frameOut = sqlThumbnailVideo.GetThumbnail(filename, numFrame);
            }
        }
    }
	return frameOut;
}

void CThumbnailDataSQL::SetBitmap(CImageLoadingFormat * bitmap)
{
	if(bitmap == nullptr)
		return;

	if(bitmap->IsOk())
	{
		//Enregistrement en base de données
		CSqlThumbnail sqlThumbnail;
		wxString filename = bitmap->GetFilename();
		if (!TestBitmap())
		{
			int compressMethod = 0;
			unsigned long outputsize = 0;
			bitmap->ConvertToRGB24(true);
			uint8_t * dest = bitmap->GetJpegData(outputsize, compressMethod);
			//wxString hash = wxMD5::GetFileMD5(filename);
			wxFileName file(filename);
			wxULongLong sizeFile = file.GetSize();
			wxString hash = sizeFile.ToString();
			if (dest != nullptr)
				sqlThumbnail.InsertThumbnail(filename, dest, outputsize, bitmap->GetWidth(), bitmap->GetHeight(), hash);

			bitmap->DestroyJpegData(dest, compressMethod);

			dest = nullptr;
		
			this->filename = filename;
			pictureLoad = true;

		}
	}
}

