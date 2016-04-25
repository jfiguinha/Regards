#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"
#include <string>
#include <Photos.h>
using namespace std;

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
			void ImportFileFromFolder(const wxString &folder, const int &idFolder);
			bool GetPhotos(PhotosVector * photosVector);
            bool GetAllPhotos(PhotosVector * photosVector);
			bool GetPhotos(PhotosVector * photosVector, const int64_t &numFolder);
			void ImportFileFromFolder(const vector<wxString> &listFile, const int &idFolder);
			int GetNumPhoto(const wxString &filepath);
		private:
			int TraitementResult(CSqlResult * sqlResult);
			PhotosVector * m_photosVector;
			vector<wxString> * listPathFile;
			vector<int> * listPhoto;
			int numPhoto = 0;
			int type = 0;
		};
	}
}

