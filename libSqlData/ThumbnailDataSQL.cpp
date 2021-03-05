#include <header.h>
#include "ThumbnailDataSQL.h"
#include "SqlThumbnail.h"
#include "SqlThumbnailVideo.h"
#include <wxmd5.h>
#include <libPicture.h>
#include <ConvertUtility.h>
using namespace Regards::Sqlite;
#include <ImageLoadingFormat.h>
#include <wx/dir.h>
#include <wx/filename.h>
using namespace Regards::Picture;

CThumbnailDataSQL::CThumbnailDataSQL(const wxString & filename, const bool &testValidity)
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
}

int CThumbnailDataSQL::GetNbFrame()
{
    if(isVideo)// && nbFrame < 20)
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
	return sqlThumbnail.TestThumbnail(filename.Clone(), sizeFile.ToString());
}

wxImage CThumbnailDataSQL::GetwxImage()
{


    if(isVideo)// && nbFrame < 20 )
    {
        CSqlThumbnailVideo sqlThumbnailVideo;
        nbFrame = sqlThumbnailVideo.GetNbThumbnail(filename);  
    }
    
	wxImage frameOut;
    if(numFrame == 0 && nbFrame == 0)
    {
        CSqlThumbnail sqlThumbnail;
        //printf("Filename : %s \n",CConvertUtility::ConvertToUTF8(filename));
		frameOut = sqlThumbnail.GetThumbnail(filename.Clone());
    }
    else
    {
        if(numFrame >= nbFrame)
            numFrame = 1;
        
        if(numFrame < nbFrame)
        {
            if(isVideo)
            {
                CSqlThumbnailVideo sqlThumbnailVideo;
                frameOut = sqlThumbnailVideo.GetThumbnail(filename, numFrame);
				if (!frameOut.IsOk())
				{
					numFrame = 1;
					frameOut = sqlThumbnailVideo.GetThumbnail(filename, numFrame);
				}
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
			//pictureLoad = true;

		}
	}
}

