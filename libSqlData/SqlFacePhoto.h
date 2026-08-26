#pragma once
#include "SqlExecuteRequest.h"

class CImageLoadingFormat;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CFaceRecognitionData
		{
		public:
			CFaceRecognitionData() = default;

			CFaceRecognitionData(
				int numFace,
				int numFaceCompatible,
				const cv::Mat& feature)
				: numFace(numFace),
				numFaceCompatible(numFaceCompatible),
				feature(feature.clone())
			{}

			int numFace = 0;
			int numFaceCompatible = 0;

			cv::Mat feature;
		};

		class CSqlFacePhoto : public CSqlExecuteRequest
		{
		public:
			CSqlFacePhoto();
			~CSqlFacePhoto() = default;
			int InsertFaceTreatment(const wxString& path);
			int InsertFace(const wxString& path, const wxString& gender, const wxString& age, const int& numberface, const int& width, const int& height,
			               const double& pertinence, const uint8_t* zBlob, const int& nBlob);
			int GetNumFace(const wxString& path, const int& numberface);
			int UpdateVideoFace(const int& numFace, const int& videoPosition);
			int GetVideoFacePosition(const int& numFace);
			vector<wxString> GetPhotoList();
			vector<wxString> GetPhotoListTreatment();

			CImageLoadingFormat* GetFacePicture(const int& numFace);
			void DeleteNumFace(const int& numFace);
			void EraseFace(const int& numFace);
			int GetFaceCompatibleRecognition(const int& numFace);
			vector<CFaceRecognitionData> GetAllNumFaceRecognition();
			//vector<int> GetAllNumFaceRecognition(const int& numFace);
			bool DeleteNumFaceMaster(const int& idFaceMaster);
			vector<int> GetAllNumFace();
			vector<int> GetAllThumbnailFace();
			vector<int> GetAllNumFace(const int& numFace);
			cv::Mat GetFace(const int& numFace, bool &isDefault);
			bool DeleteFaceDatabase();
			bool DeletePhotoFaceDatabase(const wxString& path);
			bool DeleteListOfPhoto(const vector<wxString>& listPhoto);
			bool DeleteListOfPhoto(const vector<int>& listNumPhoto);
			bool DeleteFaceTreatmentDatabase();
			void RebuildLink();

		private:
			void DeleteFaceNameAlone();

			int TraitementResult(CSqlResult* sqlResult) override;

			wxString filename;
			vector<wxString> listPhoto;
			vector<int> listFace;
			vector<CFaceRecognitionData> listFaceRecognition;
			int videoPosition = 0;
			int64_t numFace = 0;
			int type = 0;

		};
	}
}
