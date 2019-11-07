#include <header.h>
#include "SqlInsertFile.h"
#include <libPicture.h>
#include <wx/dir.h>
#include <wx/progdlg.h>
using namespace Regards::Sqlite;

CSqlInsertFile::CSqlInsertFile()
	: CSqlExecuteRequest(L"RegardsDB")
{
	numPhoto = 0;
	type = 0;
}


CSqlInsertFile::~CSqlInsertFile()
{

}

void CSqlInsertFile::UpdatePhotoProcess(const int &numPhoto)
{
    ExecuteRequestWithNoResult("Update PHOTOS Set Process = 1 where NumPhoto = " + to_string(numPhoto));
}

CPhotos CSqlInsertFile::GetPhotoToProcess()
{
    type = 4;
    this->numPhoto = 0;
    photoLocal.SetId(-1);
    ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS where CriteriaInsert = 0 and Process = 0");
    return photoLocal;
}

CPhotos CSqlInsertFile::GetPhoto(const int &numPhoto)
{
    type = 4;
    this->numPhoto = numPhoto;
    photoLocal.SetId(-1);
    ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS where CriteriaInsert = 0");
    return photoLocal;
}

int CSqlInsertFile::GetNbPhotosToProcess()
{
    type = 3;
    numPhoto = 0;
    ExecuteRequest("SELECT count(*) as nbphoto FROM PHOTOS where CriteriaInsert = 0 and Process = 0");
    return numPhoto;
}

int CSqlInsertFile::ReinitPhotosToProcess()
{
	numPhoto = 0;
	ExecuteRequest("Update PHOTOS SET CriteriaInsert = 0, Process = 0;");
	return 0;
}


int CSqlInsertFile::GetNbPhotos()
{
    type = 3;
    numPhoto = 0;
    ExecuteRequest("SELECT count(*) as nbphoto FROM PHOTOS where CriteriaInsert = 0");
    return numPhoto;
}
/*
void CSqlInsertFile::UpdateFolder(const vector<wxString> &listFile, const int &idFolder)
{
    BeginTransaction();
    
    ExecuteRequestWithNoResult("DELETE FROM PHOTOFOLDER");
    
	for (wxString file : listFile)
    {
        file.Replace("'", "''");
        ExecuteRequestWithNoResult("INSERT INTO PHOTOFOLDER (FullPath) VALUES ('" + file + "')");
    }
    
    CommitTransection();
    
    
    //Photo To add
    CLibPicture libPicture;
    vector<wxString> listAddFile;
    if(GetPhotoToAdd(&listAddFile))
    {
        BeginTransaction();
        
        for (wxString filename : listAddFile)
        {
            int extensionId = libPicture.TestImageFormat(filename);
            filename.Replace("'", "''");
            ExecuteRequestWithNoResult("INSERT INTO PHOTOS (NumFolderCatalog, FullPath, CriteriaInsert, Process, ExtensionId) VALUES (" + to_string(idFolder) + ", '" + filename + "', 0, 0, " + to_string(extensionId) + ")");
        }
        
        CommitTransection();
    }
    
    vector<int> listRemoveFile;
    if(GetPhotoToRemove(&listRemoveFile, idFolder))
    {
        BeginTransaction();
        
        for (auto numPhoto : listRemoveFile)
        {
            ExecuteRequestWithNoResult("DELETE FROM PHOTOS WHERE NumPhoto = " + to_string(numPhoto));
        }
        
        CommitTransection();
    }
   
}
*/
void CSqlInsertFile::ImportFileFromFolder(const vector<wxString> &listFile, const int &idFolder)
{
	BeginTransaction();
    CLibPicture libPicture;
    int updatesize = 0;
#ifndef __WXGTK__    
    wxProgressDialog dialog("Import File", "Checking...", listFile.size(), NULL, wxPD_APP_MODAL);
    wxString msg = "in progress";
    dialog.Update(updatesize, msg);    
#endif
	for (wxString filename : listFile)
	{
        updatesize++;
		if (GetNumPhoto(filename) == 0)
		{            
            int extensionId = libPicture.TestImageFormat(filename);
			filename.Replace("'", "''");
			ExecuteRequestWithNoResult("INSERT INTO PHOTOS (NumFolderCatalog, FullPath, CriteriaInsert, Process, ExtensionId) VALUES (" + to_string(idFolder) + ", '" + filename + "', 0, 0, " + to_string(extensionId) + ")");
              
		}
#ifndef __WXGTK__           
        if (false == dialog.Update(updatesize, msg))
        {
            break;
        }
#endif        
	}
	ExecuteRequestWithNoResult("INSERT INTO PHOTOSSEARCHCRITERIA (NumPhoto,FullPath) SELECT NumPhoto, FullPath FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder) + " and NumPhoto not in (SELECT NumPhoto FROM PHOTOSSEARCHCRITERIA)");
#ifndef __WXGTK__    
    dialog.Destroy();
#endif
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

int CSqlInsertFile::GetNumPhoto(const wxString &filepath)
{
	numPhoto = 0;
	type = 3;
	wxString filename = filepath;
	filename.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE FullPath = '" + filename + "'");
	return numPhoto;
}


bool CSqlInsertFile::GetPhotoToRemove(vector<int> * listFile, const int &idFolder)
{
	type = 2;
	listPhoto = listFile;
	return (ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder) + " and FullPath not in(Select FullPath From PHOTOFOLDER)") != -1) ? true : false;
}

int CSqlInsertFile::AddFileFromFolder(const wxString &folder, const int &idFolder, wxString &firstFile)
{
	CLibPicture libPicture;

	int i = 0;
	wxArrayString files;

	wxDir::GetAllFiles(folder, &files, wxEmptyString, wxDIR_FILES);

	if (files.size() > 0)
	{
#ifndef __WXGTK__         
		wxProgressDialog dialog("Import File", "Checking...", files.size(), NULL, wxPD_APP_MODAL);
		wxString msg = "in progress";
		//dialog.Update(files.size(), msg);
#endif
		BeginTransaction();
		for (wxString file : files)
		{
			if (libPicture.TestImageFormat(file) != 0 && GetNumPhoto(file) == 0)
			{
				int extensionId = libPicture.TestImageFormat(file);
				if (i == 0)
					firstFile = file;
				file.Replace("'", "''");
				ExecuteRequestWithNoResult("INSERT INTO PHOTOS (NumFolderCatalog, FullPath, CriteriaInsert, Process, ExtensionId) VALUES (" + to_string(idFolder) + ",'" + file + "', 0, 0, " + to_string(extensionId) + ")");
				i++;
#ifndef __WXGTK__                       
				dialog.Update(i, msg);
#endif                
			}
		}
#ifndef __WXGTK__               
		dialog.Destroy();
#endif
		CommitTransection();
	}
	return i;

	//ExecuteRequestWithNoResult("INSERT INTO PHOTOSSEARCHCRITERIA (NumPhoto,FullPath) SELECT NumPhoto, FullPath FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder));

	//
}


int CSqlInsertFile::ImportFileFromFolder(const wxString &folder, const int &idFolder, wxString &firstFile)
{
	CLibPicture libPicture;
	
	int i = 0;
	wxArrayString files;

	wxDir::GetAllFiles(folder, &files, wxEmptyString, wxDIR_FILES);


	BeginTransaction();
	for (wxString file : files)
	{
		if (libPicture.TestImageFormat(file) != 0 && GetNumPhoto(file) == 0)
		{
            int extensionId = libPicture.TestImageFormat(file);
			if (i == 0)
				firstFile = file;
			file.Replace("'", "''");
			ExecuteRequestWithNoResult("INSERT INTO PHOTOS (NumFolderCatalog, FullPath, CriteriaInsert, Process, ExtensionId) VALUES (" + to_string(idFolder) + ",'" + file + "', 0, 0, " + to_string(extensionId) + ")");
			i++;
		}
	}
	CommitTransection();
	return i;

	//ExecuteRequestWithNoResult("INSERT INTO PHOTOSSEARCHCRITERIA (NumPhoto,FullPath) SELECT NumPhoto, FullPath FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder));
	
	//
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

bool CSqlInsertFile::GetAllPhotos(PhotosVector * photosVector)
{
    m_photosVector = photosVector;
    return (ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS") != -1) ? true : false;
}

bool CSqlInsertFile::GetPhotos(PhotosVector * photosVector, const int64_t &numFolder)
{
	m_photosVector = photosVector;
	return (ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderCatalog, CriteriaInsert FROM PHOTOS WHERE CriteriaInsert = 0 and NumFolderCatalog = " + to_string(numFolder) + "") != -1) ? true : false;
}

int CSqlInsertFile::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (type)
		{
            
			case 4:
			{
				if(nbResult == numPhoto)
                {
                    for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
                    {

                        switch (i)
                        {
                        case 0:
                            photoLocal.SetId(sqlResult->ColumnDataInt(i));
                            break;
                        case 1:
                            photoLocal.SetPath(sqlResult->ColumnDataText(i));
                            break;
                        case 2:
                            photoLocal.SetFolderId(sqlResult->ColumnDataInt(i));
                            break;
                        case 3:
                            photoLocal.SetIsCriteriaInsert(sqlResult->ColumnDataInt(i));
                            break;
                        }
                    } 
                    return nbResult;      
                }


			}
			break;
            
			case 0:
			{
				CPhotos _cPhoto;
				for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
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
					for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
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
					for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
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

			case 3:
			{
				for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
				{

					switch (i)
					{
					case 0:
						numPhoto = sqlResult->ColumnDataInt(i);
						break;
					}
				}
			}
			break;
		}
		nbResult++;
	}
	return nbResult;
}