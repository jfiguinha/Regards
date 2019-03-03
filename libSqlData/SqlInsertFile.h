#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"
#include <Photos.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSqlInsertFile : public CSqlExecuteRequest
		{
		public:

			CSqlInsertFile();
			~CSqlInsertFile();
			bool RemovePhotos(const int &idFolder);
			void InsertPhotoFolderToRefresh(const wxString &folder);
            void UpdateFolder(const vector<wxString> &listFile, const int &idFolder);
			bool GetPhotoToAdd(vector<wxString> * listFile);
			bool GetPhotoToRemove(vector<int> * listFile, const int &idFolder);
			int ImportFileFromFolder(const wxString &folder, const int &idFolder, wxString &firstFile);
            bool GetPhotos(PhotosVector * photosVector);
            bool GetAllPhotos(PhotosVector * photosVector);
            bool GetPhotos(PhotosVector * photosVector, const int64_t &numFolder);
            int GetNbPhotos();
            int GetNbPhotosToProcess();
            void UpdatePhotoProcess(const int &numPhoto);
            CPhotos GetPhotoToProcess();
            CPhotos GetPhoto(const int &numPhoto);
			void ImportFileFromFolder(const vector<wxString> &listFile, const int &idFolder);
			int GetNumPhoto(const wxString &filepath);
			int ReinitPhotosToProcess();
		private:
			int TraitementResult(CSqlResult * sqlResult);
			PhotosVector * m_photosVector;
            CPhotos photoLocal;
			vector<wxString> * listPathFile;
			vector<int> * listPhoto;
			int numPhoto;
			int type;
		};
	}
}

