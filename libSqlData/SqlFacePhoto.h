#pragma once
#include "SqlExecuteRequest.h"

class CImageLoadingFormat;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlFacePhoto : public CSqlExecuteRequest
		{
		public:
			CSqlFacePhoto();
			~CSqlFacePhoto();
			int InsertFaceTreatment(const wxString & path);
			int InsertFace(const wxString & path, const int &numberface, const int & width, const int &height, const double &pertinence, const uint8_t *zBlob, const int &nBlob);
			int GetNumFace(const wxString & path, const int &numberface);
			int UpdateVideoFace(const int & numFace, const int &videoPosition);
			int GetVideoFacePosition(const int & numFace);
			vector<wxString> GetPhotoList();
			vector<wxString> GetPhotoListTreatment();
			vector<CImageLoadingFormat * > GetAllFace();
			vector<CImageLoadingFormat *> GetAllFace(const int &numFace);
			CImageLoadingFormat * GetFacePicture(const int &numFace);

			int GetFaceCompatibleRecognition(const int& numFace);
			vector<int> GetAllNumFaceRecognition();
			vector<int> GetAllNumFaceRecognition(const int& numFace);

			vector<int> GetAllNumFace();
			vector<int> GetAllNumFace(const int &numFace);
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
			//int width;
			//int height;
			wxString filename;
			vector<wxString> listPhoto;
			vector<int> listFace;
			//vector<CPictureData *> listFace;
			//CPictureData * facePicture;
			vector<int> listFaceIndex;
			//wxImage bitmap;
		};
	}
}
