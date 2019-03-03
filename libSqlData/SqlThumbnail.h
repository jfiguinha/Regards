#pragma once
#include <SqlExecuteRequest.h>
#include "SqlResult.h"
#include <PictureData.h>

class CRegardsBitmap;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlThumbnail : public CSqlExecuteRequest
		{
		public:
			CSqlThumbnail();
			~CSqlThumbnail();
			bool InsertThumbnail(const wxString & path, const uint8_t *zBlob, const int &nBlob, const int & width, const int &height, const wxString &hash);
			wxImage GetThumbnail(const wxString & path);
			CRegardsBitmap * GetPictureThumbnail(const wxString & pathh, const int &typePicture = 3);
			bool DeleteThumbnail(const wxString &path);
			bool DeleteThumbnail(const int & numPhoto);
			bool EraseThumbnail();
			bool EraseFolderThumbnail(const int &numFolder);
			bool TestThumbnail(const wxString & path, const wxString &hash);
			bool TestThumbnail(const wxString & path);
			CPictureData * GetJpegThumbnail(const wxString & path);
		private:
			int TraitementResult(CSqlResult * sqlResult);
			wxImage bitmap;
			CRegardsBitmap * regardsBitmap;
			int type;
			bool find;
			CPictureData * picture;
		};
	}
}
