#pragma once
#include <SqlExecuteRequest.h>

class CImageLoadingFormat;
class CPictureData;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;
		
		class CSqlThumbnail : public CSqlExecuteRequest
		{
		public:
			CSqlThumbnail();
			~CSqlThumbnail();
			bool InsertThumbnail(const wxString & path, const uint8_t *zBlob, const int &nBlob, const int & width, const int &height, const wxString &hash);
			wxImage GetThumbnail(const wxString & path);
			CImageLoadingFormat * GetPictureThumbnail(const wxString & path);
			bool DeleteThumbnail(const wxString &path);
			bool DeleteThumbnail(const int & numPhoto);
			bool EraseThumbnail();
			void EraseThumbnail(const int& numPhoto);
			bool EraseFolderThumbnail(const int &numFolder);
			bool TestThumbnail(const wxString & path, const wxString &hash);
			bool TestThumbnail(const wxString & path);
			vector<int> GetAllPhotoThumbnail();
			//CPictureData * GetJpegThumbnail(const wxString & path);
		private:
			int TraitementResult(CSqlResult * sqlResult);
			vector<int> listPhoto;
			//wxImage bitmap;
			//CRegardsBitmap * regardsBitmap;
			int type;
			bool find;
			int numPhoto = 0;
			//CPictureData * picture;
		};
	}
}
