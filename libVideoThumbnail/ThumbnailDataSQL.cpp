#include "ThumbnailDataSQL.h"
#include "SqlThumbnail.h"
#include <wxmd5.h>
#include <wx/mstream.h>
#include <jpge.h>
#if defined(__WXMSW__)
#include "../include/config_id.h"
#else
#include <config_id.h>
#endif
#include <libPicture.h>
#include <wx/filename.h>
#include <ConvertUtility.h>
using namespace jpge;
using namespace Regards::Sqlite;
#include <ImageLoadingFormat.h>
#ifdef TURBOJPEG
#include <turbojpeg.h>
#endif

CThumbnailDataSQL::CThumbnailDataSQL(const wxString & filename, const bool &testValidity)
	: CThumbnailData(filename)
{
	pictureLoad = TestBitmap();
}

CThumbnailDataSQL::~CThumbnailDataSQL(void)
{
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
	CSqlThumbnail sqlThumbnail;
    printf("Filename : %s \n",CConvertUtility::ConvertToUTF8(filename));
	return sqlThumbnail.GetThumbnail(filename);
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

