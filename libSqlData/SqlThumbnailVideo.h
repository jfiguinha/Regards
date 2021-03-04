#pragma once
#include <SqlExecuteRequest.h>

class CImageVideoThumbnail;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlThumbnailVideo : public CSqlExecuteRequest
		{
		public:
			CSqlThumbnailVideo();
			~CSqlThumbnailVideo();
            int GetNbThumbnail(const wxString & path);
			bool InsertThumbnail(const wxString & path, const uint8_t *zBlob, const int &nBlob, const int & width, const int &height, const int &numPicture, const int &rotation, const int &percent, const int &timePosition);
			wxImage GetThumbnail(const wxString & path, const int &numVideo);
            CImageVideoThumbnail * GetPictureThumbnail(const wxString & path, const int &numVideo);
			bool DeleteThumbnail(const wxString &path);
			bool DeleteThumbnail(const int & numPhoto);
			bool EraseThumbnail();
			bool EraseFolderThumbnail(const int &numFolder);
			bool TestThumbnail(const int & numPhoto, const int &numVideo);
		private:
			int TraitementResult(CSqlResult * sqlResult);
			CImageVideoThumbnail * videoThumbnail = nullptr;
			vector<int> listPhoto;
			int type;
			bool find;
            int nbElement;
			int numPhoto;
		};
	}
}
