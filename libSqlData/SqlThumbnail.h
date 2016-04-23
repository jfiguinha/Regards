#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <RegardsBitmap.h>
#include <SqlExecuteRequest.h>
#include "SqlResult.h"
#include <vector>
using namespace std;

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
			bool EraseThumbnail();
			bool TestThumbnail(const wxString & path, const wxString &hash);
		private:
			int TraitementResult(CSqlResult * sqlResult);
			wxImage bitmap;
			CRegardsBitmap * regardsBitmap = nullptr;
			int type = 0;
			bool find = false;
		};
	}
}
