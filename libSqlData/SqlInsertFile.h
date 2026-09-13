#pragma once
#include "SqlExecuteRequest.h"
#include <Photos.h>
#include <wx/progdlg.h>
#include <FolderCatalog.h>

namespace Regards
{
	namespace Picture { class CLibPicture; } // Forward declaration si nécessaire

	namespace Sqlite
	{
		class CSqlResult;

		class CSqlInsertFile : public CSqlExecuteRequest
		{
		public:
			CSqlInsertFile();
			~CSqlInsertFile() = default;

			int CheckFolderToRefresh(FolderCatalogVector& folders);
			void InsertPhotoFolderToRefresh(const wxString& folder);
			bool GetPhotoToAdd(vector<wxString>* listFile);
			bool GetPhotoToRemove(vector<int>* listFile, const int& idFolder);
			int AddFileFromFolder(wxWindow* parent, wxProgressDialog* dialog, wxArrayString& files,
				const wxString& folder, const int& idFolder, wxString& firstFile);
			int ImportFileFromFolder(const wxString& folder, const int& idFolder, wxString& firstFile);
			bool GetPhotos(PhotosVector* photosVector);
			bool GetAllPhotos(PhotosVector* photosVector);
			bool GetPhotos(PhotosVector* photosVector, const int64_t& numFolder);
			int GetNbPhotos();
			int GetNbPhotosToProcess();
			void UpdatePhotoProcess(const int& numPhoto);
			CPhotos GetPhotoToProcess();
			void GetPhotoToProcessList(PhotosVector* photosVector);
			CPhotos GetPhoto(const int& numPhoto);
			void ImportFileFromFolder(const vector<wxString>& listFile, const int& idFolder);
			int GetNumPhoto(const wxString& filepath);
			int ReinitPhotosToProcess();

		private:
			enum class ResultType {
				SinglePhoto = 0,
				VectorPhotos = 1,
				CountPhotos = 2,
				VectorIds = 3
			};

			int TraitementResult(CSqlResult* sqlResult) override;

			// Méthodes privées d'aide à la factorisation
			void GetSortedFilesFromFolder(const wxString& folder, wxArrayString& files);
			bool InsertPhoto(const int& idFolder, const wxString& filename, Regards::Picture::CLibPicture& libPicture, int multifile = 0);

			PhotosVector* m_photosVector;
			CPhotos photoLocal;
			vector<wxString>* listPathFile;
			vector<int>* listPhoto;
			ResultType m_resultType;
			int nbPhoto = 0;
		};
	}
}