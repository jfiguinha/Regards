#include <header.h>
#include "SqlPhotos.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;

CSqlPhotos::CSqlPhotos(CSqlLib * _sqlLibTransaction, const bool &useTransaction)
	: CSqlExecuteRequest(L"RegardsDB"), exif(0)
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

void CSqlPhotos::DeletePhotoExif(const wxString& filepath)
{
	wxString fullpath = filepath;
	fullpath.Replace("'", "''");
	ExecuteRequestWithNoResult("DELETE FROM PHOTOS WHERE FullPath = '" + fullpath + "'");
}

int64_t CSqlPhotos::GetPhotoExif(const wxString& filepath)
{
	typeResult = 4;
	exif = -1;
	wxString fullpath = filepath;
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT Exif FROM PHOTO_EXIF WHERE FullPath = '" + fullpath + "'");
	return (int)exif;
}

bool CSqlPhotos::UpdatePhotoExif(const wxString& filepath, const int64_t& exif)
{
	wxString fullpath = filepath;
	fullpath.Replace("'", "''");
	return (ExecuteRequestWithNoResult("UPDATE PHOTO_EXIF SET Exif = " + to_string(exif) + " WHERE FullPath = '" + fullpath + "'") != -1) ? true : false;

}

bool CSqlPhotos::InsertPhotoExif(const wxString& filepath, const int64_t& exif)
{
	wxString fullpath = filepath;
	fullpath.Replace("'", "''");
	return (ExecuteRequestWithNoResult("INSERT INTO PHOTO_EXIF (FullPath, Exif) VALUES ('" + fullpath + "', " + to_string(exif) + ")") != -1) ? true : false;
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

int CSqlPhotos::GetExifFromAngleAndFlip(const int& angle, const int& flipH, const int& flipV)
{
	if (angle == 0 && flipH == 0 && flipV == 0)
		return 1;
	if (angle == 0 && flipH == 1 && flipV == 0)
		return 2;

	if (angle == 180 && flipH == 0 && flipV == 0)
		return 3;
	if (angle == 180 && flipH == 1 && flipV == 0)
		return 4;

	if (angle == 90 && flipH == 0 && flipV == 0)
		return 5;
	if (angle == 90 && flipH == 1 && flipV == 0)
		return 6;

	if (angle == 270 && flipH == 0 && flipV == 0)
		return 7;
	if (angle == 270 && flipH == 1 && flipV == 0)
		return 8;

	if (angle == 0 && flipH == 0 && flipV == 1)
		return 4;
	if (angle == 0 && flipH == 1 && flipV == 1)
		return 3;

	if (angle == 180 && flipH == 0 && flipV == 1)
		return 2;
	if (angle == 180 && flipH == 1 && flipV == 1)
		return 1;

	if (angle == 90 && flipH == 0 && flipV == 1)
		return 6;
	if (angle == 90 && flipH == 1 && flipV == 1)
		return 7;

	if (angle == 270 && flipH == 0 && flipV == 1)
		return 8;
	if (angle == 270 && flipH == 1 && flipV == 1)
		return 5;

	return 0;
}

void CSqlPhotos::GetAngleAndFlip(const int64_t& exif, int& angle, int& flipH, int& flipV)
{
	switch (exif)
	{
	case 1:
		angle = 0;
		flipH = 0;
		flipV = 0;
		break;

	case 2:
		angle = 0;
		flipH = 1;
		flipV = 0;
		break;

	case 3:
		angle = 180;
		flipH = 0;
		flipV = 0;
		break;

	case 4:
		angle = 180;
		flipH = 1;
		flipV = 0;
		break;

	case 5:
		angle = 90;
		flipH = 0;
		flipV = 0;
		break;

	case 6:
		angle = 90;
		flipH = 1;
		flipV = 0;
		break;

	case 7:
		angle = 270;
		flipH = 0
		flipV = 0;
		break;

	case 8:
		angle = 270;
		flipH = 1;
		flipV = 0;
		break;
	}
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
	//ExecuteRequestWithNoResult("DELETE FROM PHOTOSSEARCHCRITERIA");
}

bool CSqlPhotos::DeletePhoto(const int64_t &numPhoto)
{
	//ExecuteRequestWithNoResult("DELETE FROM PHOTOSSEARCHCRITERIA WHERE NumPhoto = " + to_string(numPhoto));
	ExecuteRequestWithNoResult("DELETE FROM PHOTO_EXIF WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumPhoto = " + to_string(numPhoto) + ")");
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOS WHERE NumPhoto = " + to_string(numPhoto)) != -1) ? true : false;
}

bool CSqlPhotos::DeletePhotoFolder(const int64_t &idFolder)
{
	//ExecuteRequestWithNoResult("DELETE FROM PHOTOSSEARCHCRITERIA WHERE NumPhoto in (SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder) + ")");
	ExecuteRequestWithNoResult("DELETE FROM PHOTO_EXIF WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumFolderCatalog = " + to_string(idFolder) + ")");

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
				default: ;
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
				default: ;
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
				default: ;
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
				default: ;
				}
			}
		}
		else if (typeResult == 4)
		{
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					exif = sqlResult->ColumnDataInt(i);
					break;
				default: ;
				}
			}
		}
		nbResult++;
	}
	return nbResult;
}