#include <header.h>
#include "SqlPhotos.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;

CSqlPhotos::CSqlPhotos(CSqlLib * _sqlLibTransaction, const bool &useTransaction)
	: CSqlExecuteRequest(L"RegardsDB")
{
	photoId = -1;
	criteriaVector = nullptr;
	typeResult = 0;
	this->_sqlLibTransaction = _sqlLibTransaction;
	this->useTransaction = useTransaction;
}


CSqlPhotos::~CSqlPhotos()
{
}

bool CSqlPhotos::InsertPhoto(const wxString & filepath, const int64_t &idFolder)
{
	wxString fullpath = filepath;
	fullpath.Replace("'", "''");
	return (ExecuteRequestWithNoResult("INSERT INTO PHOTOS (NumFolderCatalog, FullPath) VALUES (" + to_string(idFolder) + ", '" + fullpath + "')") != -1) ? true : false;
}

bool CSqlPhotos::UpdatePhotoCriteria(const int64_t &numPhoto)
{
	return (ExecuteRequestWithNoResult("UPDATE PHOTOS SET CriteriaInsert = 1 WHERE NumPhoto = " + to_string(numPhoto)) != -1) ? true : false;
}

int  CSqlPhotos::GetCriteriaInsert(const wxString & filepath)
{
	typeResult = 0;
	photoId = -1;
	wxString fullpath = filepath;
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT CriteriaInsert FROM PHOTOS WHERE FullPath = '" + fullpath + "'");
	return (int)photoId;
}

int64_t CSqlPhotos::GetOrInsertPhoto(const wxString & filepath, const int64_t &idFolder)
{
	typeResult = 0;
	int64_t photoId = GetPhotoId(filepath, idFolder);
	if (photoId == -1)
	{
		InsertPhoto(filepath, idFolder);
		photoId = GetPhotoId(filepath, idFolder);
		//photoId = GetLastId();
	}
	return photoId;
}

void CSqlPhotos::GetPhotoCriteria(CriteriaVector * criteriaVector, const wxString & filepath)
{
	typeResult = 1;
	this->criteriaVector = criteriaVector;
	wxString fullpath = filepath;
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT distinct C.NumCriteria, NumCategorie, Libelle FROM CRITERIA C INNER JOIN PHOTOSCRITERIA PC ON C.NUMCRITERIA = PC.NUMCRITERIA INNER JOIN PHOTOS P ON P.NUMPHOTO = PC.NUMPHOTO AND Libelle != 'Not Geolocalized' and FullPath = '" + fullpath + "'");
}

void CSqlPhotos::GetPhotoCriteriaByCategorie(CriteriaVector * criteriaVector, const wxString & filepath, const int &numCategorie)
{
    typeResult = 1;
    this->criteriaVector = criteriaVector;
    wxString fullpath = filepath;
    fullpath.Replace("'", "''");
    ExecuteRequest("SELECT distinct C.NumCriteria, NumCategorie, Libelle FROM CRITERIA C INNER JOIN PHOTOSCRITERIA PC ON C.NUMCRITERIA = PC.NUMCRITERIA INNER JOIN PHOTOS P ON P.NUMPHOTO = PC.NUMPHOTO AND FullPath = '" + fullpath + "' WHERE NumCategorie = " + to_string(numCategorie));
}

wxString CSqlPhotos::GetPhotoPath(const int64_t &numPhoto)
{
	typeResult = 3;
	ExecuteRequest("SELECT FullPath FROM PHOTOS WHERE NumPhoto = " + to_string(numPhoto));
	return photoPath;
}

int64_t CSqlPhotos::GetPhotoId(const wxString & filepath, const int64_t &idFolder)
{
	typeResult = 0;
	photoId = -1;
	wxString fullpath = filepath;
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder) + " and FullPath = '" + fullpath + "'");
	return photoId;
}

int64_t CSqlPhotos::GetPhotoId(const wxString & filepath)
{
    typeResult = 0;
    photoId = -1;
    wxString fullpath = filepath;
    fullpath.Replace("'", "''");
    ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE FullPath = '" + fullpath + "'");
    return photoId;
}

void CSqlPhotos::DeletePhotoSearch()
{
	ExecuteRequestWithNoResult("DELETE FROM PHOTOSSEARCHCRITERIA");
}

bool CSqlPhotos::DeletePhoto(const int64_t &numPhoto)
{
	ExecuteRequestWithNoResult("DELETE FROM PHOTOSSEARCHCRITERIA WHERE NumPhoto = " + to_string(numPhoto));
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOS WHERE NumPhoto = " + to_string(numPhoto)) != -1) ? true : false;
}

bool CSqlPhotos::DeletePhotoFolder(const int64_t &idFolder)
{
	ExecuteRequestWithNoResult("DELETE FROM PHOTOSSEARCHCRITERIA WHERE NumPhoto in (SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder) + ")");
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder)) != -1) ? true : false;
}

bool CSqlPhotos::DeletePhotoCatalog(const int64_t &idCatalog)
{
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOS WHERE NUMFOLDERCATALOG in (SELECT NUMFOLDERCATALOG FROM FOLDERCATALOG WHERE NumCatalog = " + to_string(idCatalog) + ")") != -1) ? true : false;
}

vector<wxString> CSqlPhotos::GetPhotoFromFolder(const int64_t &idFolder)
{
	typeResult = 2;
	listPhoto.clear();
	ExecuteRequest("SELECT FullPath FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder));
	return listPhoto;
}


int CSqlPhotos::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		if (typeResult == 0)
		{
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					photoId = sqlResult->ColumnDataInt(i);
					break;
				}
			}
		}
		else if (typeResult == 1)
		{
			CCriteria criteria;

			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					criteria.SetId(sqlResult->ColumnDataInt(i));
					break;
				case 1:
					criteria.SetCategorieId(sqlResult->ColumnDataInt(i));
					break;
				case 2:
					criteria.SetLibelle(sqlResult->ColumnDataText(i));
					break;
				}
			}
			criteriaVector->push_back(criteria);
		}
		else if(typeResult == 2)
		{
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					listPhoto.push_back(sqlResult->ColumnDataText(i));
					break;
				}
			}
		}
		else if (typeResult == 3)
		{
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					photoPath = sqlResult->ColumnDataText(i);
					break;
				}
			}
		}
		nbResult++;
	}
	return nbResult;
}