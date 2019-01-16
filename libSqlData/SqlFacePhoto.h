#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"
class CRegardsBitmap;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlFacePhoto : public CSqlExecuteRequest
		{
		public:
			CSqlFacePhoto();
			~CSqlFacePhoto();
			int InsertFaceTreatment(const wxString & path);
			int InsertFace(const wxString & path, const int &numberface, const int & width, const int &height, const double &pertinence, const uint8_t *zBlob, const int &nBlob);
			int GetNumFace(const wxString & path, const int &numberface);
			vector<wxString> GetPhotoList();
			vector<wxString> GetPhotoListTreatment();
			wxImage GetFace(const int &numFace);
			bool DeleteFaceDatabase();
			bool DeletePhotoFaceDatabase(const wxString & path);
			bool DeleteListOfPhoto(const vector<wxString> & listPhoto);
			bool DeleteListOfPhoto(const vector<int> & listNumPhoto);
			bool DeleteFaceTreatmentDatabase();
		private:
            void DeleteFaceNameAlone();
			void RebuildLink();
			int TraitementResult(CSqlResult * sqlResult);
			int64_t numFace;
			int type;
			int width;
			int height;
			wxString filename;
			vector<wxString> listPhoto;
			wxImage bitmap;
		};
	}
}
