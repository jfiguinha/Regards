#include "ThumbnailDataSQL.h"
#include "SqlThumbnail.h"
#include <wxmd5.h>
#include <wx/mstream.h>
#include <jpge.h>
#include <FiltreEffet.h>
#if defined(__WXMSW__)
#include "../include/config_id.h"
#else
#include <config_id.h>
#endif
using namespace jpge;
using namespace Regards::Sqlite;

CThumbnailDataSQL::CThumbnailDataSQL(const wxString & filename)
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
	wxString md5Value = wxMD5::GetFileMD5(filename);
	return sqlThumbnail.TestThumbnail(filename, md5Value);

}

wxImage CThumbnailDataSQL::GetwxImage()
{
	CSqlThumbnail sqlThumbnail;
    printf("Filename : %s \n",filename.ToStdString().c_str());
	return sqlThumbnail.GetThumbnail(filename);
}

CRegardsBitmap * CThumbnailDataSQL::GetRegardsBitmap(const int &type )
{
	CSqlThumbnail sqlThumbnail;
	return sqlThumbnail.GetPictureThumbnail(filename, type);
}

void CThumbnailDataSQL::SetBitmap(CRegardsBitmap * bitmap)
{
	//Enregistrement en base de données
	CSqlThumbnail sqlThumbnail;
	wxString filename = bitmap->GetFilename();
	int size = (int)bitmap->GetBitmapSize();
	if (!TestBitmap())
	{
		uint8_t * dest = new uint8_t[size];
        wxString hash = wxMD5::GetFileMD5(filename);
		int outputsize = size;

		//Convert to 24bits -> RGB
		CFiltreEffet filtre = CFiltreEffet(bitmap, CRgbaquad(0, 0, 0), LIBCPU);
		uint8_t * rgb24 = filtre.BGRA32ToRGB24();

		bool result = compress_image_to_jpeg_file_in_memory(dest, outputsize, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), 3, rgb24);
		if (result)
			sqlThumbnail.InsertThumbnail(filename, dest, outputsize, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), hash);

		delete[] dest;
		delete[] rgb24;
		this->filename = filename;
		pictureLoad = true;

	}
}

