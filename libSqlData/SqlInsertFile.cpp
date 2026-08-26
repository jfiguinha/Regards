#include <header.h>
#include "SqlInsertFile.h"
#include <libPicture.h>
#include <wx/dir.h>
#include "SqlResult.h"
#include <algorithm>
#include <mutex>
#include "SqlTransaction.h"
#include <SqlParameter.h>
using namespace Regards::Picture;
using namespace Regards::Sqlite;


CSqlInsertFile::CSqlInsertFile()
	: CSqlExecuteRequest(L"RegardsDB"), m_photosVector(nullptr), listPathFile(nullptr), listPhoto(nullptr)
{
	type = 0;
}

void CSqlInsertFile::UpdatePhotoProcess(const int& numPhoto)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	ExecuteSqlWithStatementNoResult("Update PHOTOS Set Process = 1 where NumPhoto = ? ", parameter);
}

CPhotos CSqlInsertFile::GetPhotoToProcess()
{
	type = 0;
	photoLocal = {};
	ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS where CriteriaInsert = 0 and Process = 0 LIMIT 1");
	return photoLocal;
}

void CSqlInsertFile::GetPhotoToProcessList(PhotosVector* photosVector)
{
 	type = 1;
	if (photosVector == nullptr)
		return;
	photosVector->clear();
    m_photosVector = photosVector;
	ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS where CriteriaInsert = 0 and Process = 0"); 
}

CPhotos CSqlInsertFile::GetPhoto(const int& numPhoto)
{
	type = 0;
	photoLocal = {};
	ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS where CriteriaInsert = 0");
	return photoLocal;
}

int CSqlInsertFile::GetNbPhotosToProcess()
{
	type = 3;
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
	type = 2;
	nbPhoto = 0;
	ExecuteRequest("SELECT count(*) as nbphoto FROM PHOTOS where CriteriaInsert = 0");
	return nbPhoto;
}

void CSqlInsertFile::ImportFileFromFolder(const vector<wxString>& listFile, const int& idFolder)
{
	CSqlTransaction sqlTransaction;
	CLibPicture libPicture;

	for (wxString filename : listFile)
	{
		if (GetNumPhoto(filename) == 0)
		{
			bool isValid = true;
            int multifile = 0;
			int extensionId = libPicture.TestImageFormat(filename);

			if (extensionId > 0)
			{
				std::vector<std::unique_ptr<CSqlParameter>> parameter;
				parameter.push_back(std::make_unique<CSqlInt>(idFolder));
				parameter.push_back(std::make_unique<CSqlString>(filename));
				parameter.push_back(std::make_unique<CSqlInt>(extensionId));
				parameter.push_back(std::make_unique<CSqlInt>(multifile));
				ExecuteSqlWithStatementNoResult("INSERT INTO PHOTOS (NumFolderCatalog, FullPath, CriteriaInsert, Process, ExtensionId, Multifiles) VALUES (?, ?, 0, 0, ?, ?)", parameter);
			}
		}
	}

	sqlTransaction.commit();
}


void CSqlInsertFile::InsertPhotoFolderToRefresh(const wxString& folder)
{
	CLibPicture libPicture;
	CSqlTransaction sqlTransaction;

	ExecuteRequestWithNoResult("DELETE FROM PHOTOFOLDER");

	wxArrayString files;

	wxDir::GetAllFiles(folder, &files, wxEmptyString, wxDIR_FILES);
	if (files.size() > 0)
		sort(files.begin(), files.end());

	for (wxString file : files)
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

bool CSqlInsertFile::GetPhotoToAdd(vector<wxString>* listFile)
{
	type = 1;
	if (listFile == nullptr)
		return false;

	listPathFile = listFile;
	return (ExecuteRequest("SELECT FullPath FROM PHOTOFOLDER WHERE FullPath not in (Select FullPath From PHOTOS)") != -
		       1)
		       ? true
		       : false;
}

int CSqlInsertFile::GetNumPhoto(const wxString& filepath)
{
	type = 2;
	nbPhoto = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	ExecuteSqlWithStatement("SELECT NumPhoto FROM PHOTOS WHERE FullPath = ?", parameter);
	return nbPhoto;
}


bool CSqlInsertFile::GetPhotoToRemove(vector<int>* listFile, const int& idFolder)
{
	type = 3;
	if (listFile == nullptr)
		return false;
	listPhoto = listFile;
	listPhoto->clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(idFolder));
	ExecuteSqlWithStatement("SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = ? and FullPath not in (Select FullPath From PHOTOFOLDER)", parameter);
	return listPhoto->size() > 0 ? true : false;
}

int CSqlInsertFile::AddFileFromFolder(wxWindow* parent, wxProgressDialog* dialog, wxArrayString& files,
                                      const wxString& folder, const int& idFolder, wxString& firstFile)
{
	if (files.size() > 0)
	{




		CSqlTransaction sqlTransaction;
		int i = 0;
		for(wxString file : files)
		{
			CLibPicture libPicture;
            int extensionId = libPicture.TestImageFormat(file);
			if (extensionId != 0 && GetNumPhoto(file) == 0)
			{
				std::vector<std::unique_ptr<CSqlParameter>> parameter;
				parameter.push_back(std::make_unique<CSqlInt>(idFolder));
				parameter.push_back(std::make_unique<CSqlString>(file));
				parameter.push_back(std::make_unique<CSqlInt>(extensionId));
				ExecuteSqlWithStatementNoResult("INSERT INTO PHOTOS (NumFolderCatalog, FullPath, CriteriaInsert, Process, ExtensionId) VALUES (?, ?, 0, 0, ?)", parameter);
			}

			if (dialog != nullptr)
			{
				wxString message = "In progress : " + to_string(++i) + "/" + to_string(files.Count());
				dialog->Update(i, message);
			}
		}

		sqlTransaction.commit();


		CLibPicture libPicture;
		bool first = true;
		for (size_t i = 0; i < files.size(); i++)
		{
			wxString file = files[i];
			if (libPicture.TestImageFormat(file) != 0)
			{
				if (first)
				{
					firstFile = file;
					break;
				}
			}
		}
	}
	return files.size();
}


int CSqlInsertFile::ImportFileFromFolder(const wxString& folder, const int& idFolder, wxString& firstFile)
{
	CLibPicture libPicture;
	CSqlTransaction sqlTransaction;

	int i = 0;
	wxArrayString files;

	wxDir::GetAllFiles(folder, &files, wxEmptyString, wxDIR_FILES);

	if (files.size() > 0)
		sort(files.begin(), files.end());

	for (wxString file : files)
	{
		if (libPicture.TestImageFormat(file) != 0 && GetNumPhoto(file) == 0)
		{
			bool isValid = true;
			int extensionId = libPicture.TestImageFormat(file);
			if (i == 0)
				firstFile = file;

			if (extensionId > 0)
			{
				std::vector<std::unique_ptr<CSqlParameter>> parameter;
				parameter.push_back(std::make_unique<CSqlInt>(idFolder));
				parameter.push_back(std::make_unique<CSqlString>(file));
				parameter.push_back(std::make_unique<CSqlInt>(extensionId));
				ExecuteSqlWithStatementNoResult("INSERT INTO PHOTOS (NumFolderCatalog, FullPath, CriteriaInsert, Process, ExtensionId) VALUES (? , ?, 0, 0, ?)", parameter);
			}

			i++;
		}
	}
	sqlTransaction.commit();
	return i;
}


bool CSqlInsertFile::GetPhotos(PhotosVector* photosVector)
{
	type = 1;
	m_photosVector = photosVector;
	return (ExecuteRequest(
			       "SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS WHERE CriteriaInsert = 0")
		       != -
		       1)
		       ? true
		       : false;
}

bool CSqlInsertFile::GetAllPhotos(PhotosVector* photosVector)
{
	type = 1;
	m_photosVector = photosVector;
	return (ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS") != -1)
		       ? true
		       : false;
}

bool CSqlInsertFile::GetPhotos(PhotosVector* photosVector, const int64_t& numFolder)
{
	type = 1;
	m_photosVector = photosVector;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFolder));
	return  ExecuteSqlWithStatement("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS WHERE CriteriaInsert = 0 and NumFolderCatalog = ?", parameter);
}

int CSqlInsertFile::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (type)
		{
		case 0:
			photoLocal.SetId(sqlResult->ColumnDataInt(0));
			photoLocal.SetPath(sqlResult->ColumnDataText(1));
			photoLocal.SetFolderId(sqlResult->ColumnDataInt(2));
			photoLocal.SetIsCriteriaInsert(sqlResult->ColumnDataInt(3));
			break;
		case 1:
		{
			CPhotos _cPhoto;
			_cPhoto.SetId(sqlResult->ColumnDataInt(0));
			_cPhoto.SetPath(sqlResult->ColumnDataText(1));
			_cPhoto.SetFolderId(sqlResult->ColumnDataInt(2));
			_cPhoto.SetIsCriteriaInsert(sqlResult->ColumnDataInt(3));
			m_photosVector->push_back(_cPhoto);
		}
		break;
		case 2:
			nbPhoto = sqlResult->ColumnDataInt(0);
			break;
		case 3:
			listPhoto->push_back(sqlResult->ColumnDataInt(0));
			break;
		}
		nbResult++;
	}
	return nbResult;
}
