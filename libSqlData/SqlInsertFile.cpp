#include <header.h>
#include "SqlInsertFile.h"
#include <libPicture.h>
#include <wx/dir.h>
#include "SqlResult.h"
#include <algorithm>
#include <mutex>
#include "SqlTransaction.h"
#include <SqlParameter.h>
#include <SqlPhotos.h>

using namespace Regards::Picture;
using namespace Regards::Sqlite;

CSqlInsertFile::CSqlInsertFile()
	: CSqlExecuteRequest(L"RegardsDB"), m_photosVector(nullptr), listPathFile(nullptr), listPhoto(nullptr)
{
	m_resultType = ResultType::SinglePhoto;
}

// ============================================================================
// MÉTHODES PRIVÉES DE FACTORISATION
// ============================================================================

void CSqlInsertFile::GetSortedFilesFromFolder(const wxString& folder, wxArrayString& files)
{
	wxDir::GetAllFiles(folder, &files, wxEmptyString, wxDIR_FILES);
	if (!files.empty())
	{
		std::sort(files.begin(), files.end());
	}
}

bool CSqlInsertFile::InsertPhoto(const int& idFolder, const wxString& filename, CLibPicture& libPicture, int multifile)
{
	int extensionId = libPicture.TestImageFormat(filename);
	if (extensionId <= 0 || GetNumPhoto(filename) != 0)
		return false;

	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(idFolder));
	parameter.push_back(std::make_unique<CSqlString>(filename));
	parameter.push_back(std::make_unique<CSqlInt>(extensionId));
	parameter.push_back(std::make_unique<CSqlInt>(multifile));

	ExecuteSqlWithStatementNoResult(
		"INSERT INTO PHOTOS (NumFolderCatalog, FullPath, CriteriaInsert, Process, ExtensionId, Multifiles) "
		"VALUES (?, ?, 0, 0, ?, ?)", parameter);

	return true;
}

// ============================================================================
// OPÉRATIONS DE MISE À JOUR ET REQUÊTES DIRECTES
// ============================================================================

void CSqlInsertFile::UpdatePhotoProcess(const int& numPhoto)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	ExecuteSqlWithStatementNoResult("Update PHOTOS Set Process = 1 where NumPhoto = ? ", parameter);
}

CPhotos CSqlInsertFile::GetPhotoToProcess()
{
	m_resultType = ResultType::SinglePhoto;
	photoLocal = {};
	ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS where CriteriaInsert = 0 and Process = 0 LIMIT 1");
	return photoLocal;
}

void CSqlInsertFile::GetPhotoToProcessList(PhotosVector* photosVector)
{
	m_resultType = ResultType::VectorPhotos;
	if (photosVector == nullptr)
		return;
	photosVector->clear();
	m_photosVector = photosVector;
	ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS where CriteriaInsert = 0 and Process = 0");
}

CPhotos CSqlInsertFile::GetPhoto(const int& numPhoto)
{
	m_resultType = ResultType::SinglePhoto;
	photoLocal = {};
	ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS where CriteriaInsert = 0");
	return photoLocal;
}

int CSqlInsertFile::GetNbPhotosToProcess()
{
	m_resultType = ResultType::CountPhotos;
	nbPhoto = 0;
	ExecuteRequest("SELECT count(*) as nbphoto FROM PHOTOS where CriteriaInsert = 0 and Process = 0");
	return nbPhoto;
}

int CSqlInsertFile::ReinitPhotosToProcess()
{
	ExecuteRequest("Update PHOTOS SET CriteriaInsert = 0, Process = 0;");
	return 0;
}

int CSqlInsertFile::GetNbPhotos()
{
	m_resultType = ResultType::CountPhotos;
	nbPhoto = 0;
	ExecuteRequest("SELECT count(*) as nbphoto FROM PHOTOS where CriteriaInsert = 0");
	return nbPhoto;
}

int CSqlInsertFile::GetNumPhoto(const wxString& filepath)
{
	m_resultType = ResultType::CountPhotos;
	nbPhoto = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	ExecuteSqlWithStatement("SELECT NumPhoto FROM PHOTOS WHERE FullPath = ?", parameter);
	return nbPhoto;
}

// ============================================================================
// REQUÊTES DE SÉLECTION COMPLÈTES (VECTORS)
// ============================================================================

bool CSqlInsertFile::GetPhotoToAdd(vector<wxString>* listFile)
{
	m_resultType = ResultType::VectorPhotos; // Note: TraitementResult attend m_photosVector pour le cas 1, assurez-vous de la cohérence de listPathFile si nécessaire
	if (listFile == nullptr)
		return false;

	listPathFile = listFile;
	return (ExecuteRequest("SELECT FullPath FROM PHOTOFOLDER WHERE FullPath not in (Select FullPath From PHOTOS)") != -1);
}

bool CSqlInsertFile::GetPhotoToRemove(vector<int>* listFile, const int& idFolder)
{
	m_resultType = ResultType::VectorIds;
	if (listFile == nullptr)
		return false;
	listPhoto = listFile;
	listPhoto->clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(idFolder));
	ExecuteSqlWithStatement("SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = ? and FullPath not in (Select FullPath From PHOTOFOLDER)", parameter);
	return !listPhoto->empty();
}

bool CSqlInsertFile::GetPhotos(PhotosVector* photosVector)
{
	m_resultType = ResultType::VectorPhotos;
	m_photosVector = photosVector;
	return (ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS WHERE CriteriaInsert = 0") != -1);
}

bool CSqlInsertFile::GetAllPhotos(PhotosVector* photosVector)
{
	m_resultType = ResultType::VectorPhotos;
	m_photosVector = photosVector;
	return (ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS") != -1);
}

bool CSqlInsertFile::GetPhotos(PhotosVector* photosVector, const int64_t& numFolder)
{
	m_resultType = ResultType::VectorPhotos;
	m_photosVector = photosVector;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFolder));
	return ExecuteSqlWithStatement("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS WHERE CriteriaInsert = 0 and NumFolderCatalog = ?", parameter);
}

// ============================================================================
// IMPORTATIONS ET SYNCHRONISATIONS DE DOSSIERS (RÉPETITIONS CORRIGÉES)
// ============================================================================

void CSqlInsertFile::ImportFileFromFolder(const vector<wxString>& listFile, const int& idFolder)
{
	CSqlTransaction sqlTransaction(m_databaseName);
	CLibPicture libPicture;

	for (const wxString& filename : listFile)
	{
		InsertPhoto(idFolder, filename, libPicture, 0);
	}
	sqlTransaction.commit();
}

void CSqlInsertFile::InsertPhotoFolderToRefresh(const wxString& folder)
{
	CLibPicture libPicture;
	CSqlTransaction sqlTransaction(m_databaseName);

	ExecuteRequestWithNoResult("DELETE FROM PHOTOFOLDER");

	wxArrayString files;
	GetSortedFilesFromFolder(folder, files);

	for (const wxString& file : files)
	{
		if (libPicture.TestImageFormat(file) != 0)
		{
			std::vector<std::unique_ptr<CSqlParameter>> parameter;
			parameter.push_back(std::make_unique<CSqlString>(file));
			ExecuteSqlWithStatementNoResult("INSERT INTO PHOTOFOLDER (FullPath) VALUES (?)", parameter);
		}
	}
	sqlTransaction.commit();
}

int CSqlInsertFile::AddFileFromFolder(wxWindow* parent, wxProgressDialog* dialog, wxArrayString& files,
	const wxString& folder, const int& idFolder, wxString& firstFile)
{
	if (files.empty())
		return 0;

	CSqlTransaction sqlTransaction(m_databaseName);
	CLibPicture libPicture;
	int insertedCount = 0;

	for (size_t i = 0; i < files.size(); ++i)
	{
		const wxString& file = files[i];

		if (InsertPhoto(idFolder, file, libPicture, 0))
		{
			if (insertedCount == 0)
			{
				firstFile = file;
				insertedCount++;
			}
		}

		if (dialog != nullptr && i % 100 == 0)
		{
			wxString message = "In progress : " + to_string(i) + "/" + to_string(files.Count());
			dialog->Update(i, message);
		}
	}

	if (dialog != nullptr)
	{
		wxString message = "In progress : " + to_string(files.Count()) + "/" + to_string(files.Count());
		dialog->Update(files.Count(), message);
	}

	sqlTransaction.commit();
	return files.size();
}

int CSqlInsertFile::ImportFileFromFolder(const wxString& folder, const int& idFolder, wxString& firstFile)
{
	CLibPicture libPicture;
	CSqlTransaction sqlTransaction(m_databaseName);
	wxArrayString files;

	GetSortedFilesFromFolder(folder, files);
	int processedCount = 0;

	for (const wxString& file : files)
	{
		if (InsertPhoto(idFolder, file, libPicture, 0))
		{
			if (processedCount == 0)
				firstFile = file;
			processedCount++;
		}
	}
	sqlTransaction.commit();
	return processedCount;
}

int CSqlInsertFile::CheckFolderToRefresh(FolderCatalogVector& folders)
{
	int nbNewFile = 0;
	CLibPicture libPicture;
	CSqlTransaction sqlTransaction(m_databaseName);
	CSqlPhotos sqlPhotos;

	for (CFolderCatalog& folder : folders)
	{
		wxArrayString files;
		GetSortedFilesFromFolder(folder.GetFolderPath(), files);

		for (const wxString& file : files)
		{
			if (libPicture.TestImageFormat(file) != 0 && sqlPhotos.GetPhotoId(file) == -1)
			{
				if (InsertPhoto(folder.GetNumFolder(), file, libPicture, 0))
				{
					nbNewFile++;
				}
			}
		}
	}
	sqlTransaction.commit();
	return nbNewFile;
}

// ============================================================================
// TRAITEMENT DES RÉSULTATS DE REQUÊTES
// ============================================================================

int CSqlInsertFile::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (m_resultType)
		{
		case ResultType::SinglePhoto:
			photoLocal.SetId(sqlResult->ColumnDataInt(0));
			photoLocal.SetPath(sqlResult->ColumnDataText(1));
			photoLocal.SetFolderId(sqlResult->ColumnDataInt(2));
			photoLocal.SetIsCriteriaInsert(sqlResult->ColumnDataInt(3));
			break;

		case ResultType::VectorPhotos:
		{
			CPhotos _cPhoto;
			_cPhoto.SetId(sqlResult->ColumnDataInt(0));
			_cPhoto.SetPath(sqlResult->ColumnDataText(1));
			_cPhoto.SetFolderId(sqlResult->ColumnDataInt(2));
			_cPhoto.SetIsCriteriaInsert(sqlResult->ColumnDataInt(3)); 
			m_photosVector->push_back(_cPhoto);
			break;
		}
		case ResultType::CountPhotos:
			nbPhoto = sqlResult->ColumnDataInt(0); 
			break; 
		case ResultType::VectorIds:
			listPhoto->push_back(sqlResult->ColumnDataInt(0));
			break;
		}
		nbResult++;
	}
	return nbResult;
}