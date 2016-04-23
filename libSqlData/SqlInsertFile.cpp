#include "SqlInsertFile.h"
#include "SqlLib.h"
#include <wx/dir.h>
#include <libPicture.h>
using namespace Regards::Sqlite;


CSqlInsertFile::CSqlInsertFile()
	: CSqlExecuteRequest(L"RegardsDB")
{

}


CSqlInsertFile::~CSqlInsertFile()
{

}

void CSqlInsertFile::ImportFileFromFolder(const vector<wxString> &listFile, const int &idFolder)
{
	BeginTransaction();

	for (wxString filename : listFile)
	{
		filename.Replace("'", "''");
		ExecuteRequestWithNoResult("INSERT INTO PHOTOS (NumFolderCatalog, FullPath, CriteriaInsert) VALUES (" + to_string(idFolder) + ", '" + filename + "', 0)");
	}
	ExecuteRequestWithNoResult("INSERT INTO PHOTOSSEARCHCRITERIA (NumPhoto,FullPath) SELECT NumPhoto, FullPath FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder) + " and NumPhoto not in(SELECT NumPhoto FROM PHOTOSSEARCHCRITERIA)");

	CommitTransection();
}

void CSqlInsertFile::InsertPhotoFolderToRefresh(const wxString &folder)
{
	CLibPicture libPicture;

	BeginTransaction();

	ExecuteRequestWithNoResult("DELETE FROM PHOTOFOLDER");

	wxArrayString files;

	wxDir::GetAllFiles(folder, &files, wxEmptyString, wxDIR_FILES);
	for (wxString file : files)
	{
		if (libPicture.TestImageFormat(file) != 0)
		{
			file.Replace("'", "''");
			ExecuteRequestWithNoResult("INSERT INTO PHOTOFOLDER (FullPath) VALUES ('" + file + "')");
		}
	}

	CommitTransection();
}

bool CSqlInsertFile::GetPhotoToAdd(vector<wxString> * listFile)
{
	type = 1;
	listPathFile = listFile;
	return (ExecuteRequest("SELECT FullPath FROM PHOTOFOLDER WHERE FullPath not in (Select FullPath From PHOTOS)") != -1) ? true : false;
}

bool CSqlInsertFile::GetPhotoToRemove(vector<int> * listFile, const int &idFolder)
{
	type = 2;
	listPhoto = listFile;
	return (ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder) + " and FullPath not in(Select FullPath From PHOTOFOLDER)") != -1) ? true : false;
}


void CSqlInsertFile::ImportFileFromFolder(const wxString &folder, const int &idFolder)
{
	CLibPicture libPicture;
	//BeginTransaction();

	wxArrayString files;

	wxDir::GetAllFiles(folder, &files, wxEmptyString, wxDIR_FILES);
	for (wxString file : files)
	{
		if (libPicture.TestImageFormat(file) != 0)
		{
			file.Replace("'", "''");
			ExecuteRequestWithNoResult("INSERT INTO PHOTOS (NumFolderCatalog, FullPath, CriteriaInsert) VALUES (" + to_string(idFolder) + ",'" + file + "', 0)");
		}
	}


	//ExecuteRequestWithNoResult("INSERT INTO PHOTOSSEARCHCRITERIA (NumPhoto,FullPath) SELECT NumPhoto, FullPath FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder));
	
	//CommitTransection();
}

bool CSqlInsertFile::RemovePhotos(const int &idFolder)
{
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSSEARCHCRITERIA WHERE NumPhoto in (SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder) + ")") != -1) ? true : false;
}

bool CSqlInsertFile::GetPhotos(PhotosVector * photosVector)
{
    m_photosVector = photosVector;
	return (ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS WHERE CriteriaInsert = 0") != -1) ? true : false;
}


int CSqlInsertFile::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (type)
		{
			case 0:
			{
				CPhotos _cPhoto;
				for (int i = 0; i < sqlResult->GetColumnCount(); i++)
				{

					switch (i)
					{
					case 0:
						_cPhoto.SetId(sqlResult->ColumnDataInt(i));
						break;
					case 1:
						_cPhoto.SetPath(sqlResult->ColumnDataText(i));
						break;
					case 2:
						_cPhoto.SetFolderId(sqlResult->ColumnDataInt(i));
						break;
					case 3:
						_cPhoto.SetIsCriteriaInsert(sqlResult->ColumnDataInt(i));
						break;
					}
				}
				m_photosVector->push_back(_cPhoto);
			}
			break;

			case 1:
				{
					wxString path;
					for (int i = 0; i < sqlResult->GetColumnCount(); i++)
					{

						switch (i)
						{
						case 0:
							path = sqlResult->ColumnDataText(i);
							break;
						}
					}
					listPathFile->push_back(path);
				}
				break;

			case 2:
			{
				int id;
				for (int i = 0; i < sqlResult->GetColumnCount(); i++)
				{

					switch (i)
					{
					case 0:
						id = sqlResult->ColumnDataInt(i);
						break;
					}
				}
				listPhoto->push_back(id);
			}
			break;
		}
		nbResult++;
	}
	return nbResult;
}