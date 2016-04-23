#include "SqlPhotos.h"
using namespace Regards::Sqlite;

CSqlPhotos::CSqlPhotos(CSqlLib * _sqlLibTransaction, const bool &useTransaction)
	: CSqlExecuteRequest(L"RegardsDB")
{
	photoId = -1;
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
		photoId = GetLastId();
	}
	return photoId;
}

void CSqlPhotos::GetPhotoCriteria(CriteriaVector * criteriaVector, const wxString & filepath)
{
	typeResult = 1;
	this->criteriaVector = criteriaVector;
	wxString fullpath = filepath;
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT C.NumCriteria, NumCategorie, Libelle FROM CRITERIA C INNER JOIN PHOTOSCRITERIA PC ON C.NUMCRITERIA = PC.NUMCRITERIA INNER JOIN PHOTOS P ON P.NUMPHOTO = PC.NUMPHOTO AND Libelle != 'Not Geolocalized' and FullPath = '" + fullpath + "'");
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
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOS as PH inner join FOLDERCATALOG as FC on PH.NumFolderCatalog = FC.NumFolderCatalog WHERE FC.NumCatalog = " + to_string(idCatalog)) != -1) ? true : false;
}

int CSqlPhotos::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		if (typeResult == 0)
		{
			for (int i = 0; i < sqlResult->GetColumnCount(); i++)
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

			for (int i = 0; i < sqlResult->GetColumnCount(); i++)
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
		nbResult++;
	}
	return nbResult;
}