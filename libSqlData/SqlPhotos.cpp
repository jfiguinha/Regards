#include <header.h>
#include "SqlPhotos.h"
#include "SqlResult.h"
#include <SqlParameter.h>
using namespace Regards::Sqlite;

CSqlPhotos::CSqlPhotos(CSqlLib* _sqlLibTransaction, const bool& useTransaction)
	: CSqlExecuteRequest(L"RegardsDB"), exif(0)
{
	photoId = -1;
	criteriaVector = nullptr;
	typeResult = 0;
	this->m_transaction = _sqlLibTransaction;
	this->m_useTransaction = useTransaction;
}

bool CSqlPhotos::InsertPhoto(const wxString& filepath, const int64_t& idFolder)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(idFolder));
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	return ExecuteSqlWithStatementNoResult("INSERT INTO PHOTOS (NumFolderCatalog, FullPath) VALUES (? , ?)", parameter);
}

void CSqlPhotos::DeletePhotoExif(const wxString& filepath)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOS WHERE FullPath = ?", parameter);
}

int64_t CSqlPhotos::GetPhotoExif(const wxString& filepath)
{
	typeResult = 0;
	exif = -1;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	ExecuteSqlWithStatement("SELECT Exif FROM PHOTO_EXIF WHERE FullPath = ?", parameter);
	return static_cast<int>(exif);
}

bool CSqlPhotos::UpdatePhotoExif(const wxString& filepath, const int64_t& exif)
{
	int numIndex = GetPhotoExif(filepath);
	if (numIndex == -1)
		return InsertPhotoExif(filepath, exif);

	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(exif));
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	return ExecuteSqlWithStatementNoResult("UPDATE PHOTO_EXIF SET Exif = ? WHERE FullPath = ?", parameter);
}

bool CSqlPhotos::InsertPhotoExif(const wxString& filepath, const int64_t& exif)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	parameter.push_back(std::make_unique<CSqlInt>(exif));

	return ExecuteSqlWithStatementNoResult("INSERT INTO PHOTO_EXIF (FullPath, Exif) VALUES (?, ?)", parameter);
}

bool CSqlPhotos::UpdatePhotoCriteria(const int64_t& numPhoto)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	return ExecuteSqlWithStatementNoResult("UPDATE PHOTOS SET CriteriaInsert = 1 WHERE NumPhoto = ?", parameter);
}

int CSqlPhotos::GetCriteriaInsert(const wxString& filepath)
{
	typeResult = 1;
	criteriaInsert = -1;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	ExecuteSqlWithStatement("SELECT CriteriaInsert FROM PHOTOS WHERE FullPath = ?", parameter);
	return static_cast<int>(criteriaInsert);
}

int64_t CSqlPhotos::GetOrInsertPhoto(const wxString& filepath, const int64_t& idFolder)
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

void CSqlPhotos::GetPhotoCriteria(CriteriaVector* criteriaVector, const wxString& filepath)
{
	typeResult = 2;
	this->criteriaVector = criteriaVector;
	if (criteriaVector != nullptr)
	{
		criteriaVector->clear();
		std::vector<std::unique_ptr<CSqlParameter>> parameter;
		parameter.push_back(std::make_unique<CSqlString>(filepath));
		ExecuteSqlWithStatement("SELECT distinct C.NumCriteria, NumCategorie, Libelle FROM CRITERIA C INNER JOIN PHOTOSCRITERIA PC ON C.NUMCRITERIA = PC.NUMCRITERIA INNER JOIN PHOTOS P ON P.NUMPHOTO = PC.NUMPHOTO AND Libelle != 'Not Geolocalized' and FullPath = ?", parameter);
	}
}

void CSqlPhotos::GetPhotoCriteriaByCategorie(CriteriaVector* criteriaVector, const wxString& filepath,
                                             const int& numCategorie)
{
	typeResult = 2;
	this->criteriaVector = criteriaVector;
	if (criteriaVector != nullptr)
	{
		criteriaVector->clear();
		std::vector<std::unique_ptr<CSqlParameter>> parameter;
		parameter.push_back(std::make_unique<CSqlString>(filepath));
		parameter.push_back(std::make_unique<CSqlInt>(numCategorie));
		ExecuteSqlWithStatement("SELECT distinct C.NumCriteria, NumCategorie, Libelle FROM CRITERIA C INNER JOIN PHOTOSCRITERIA PC ON C.NUMCRITERIA = PC.NUMCRITERIA INNER JOIN PHOTOS P ON P.NUMPHOTO = PC.NUMPHOTO AND FullPath = ? WHERE NumCategorie = ?", parameter);
	}
}


wxString CSqlPhotos::GetPhotoPath(const int64_t& numPhoto)
{
	typeResult = 3;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	ExecuteSqlWithStatement("SELECT FullPath FROM PHOTOS WHERE NumPhoto = ?", parameter);
	return photoPath;
}

int64_t CSqlPhotos::GetPhotoId(const wxString& filepath, const int64_t& idFolder)
{
	typeResult = 4;
	photoId = -1;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	parameter.push_back(std::make_unique<CSqlInt>(idFolder));
	ExecuteSqlWithStatement("SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = ? and FullPath = ?", parameter);
	return photoId;
}

int64_t CSqlPhotos::GetPhotoId(const wxString& filepath)
{
	typeResult = 4;
	photoId = -1;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	ExecuteSqlWithStatement("SELECT NumPhoto FROM PHOTOS WHERE FullPath = ?", parameter);
	return photoId;
}

bool CSqlPhotos::DeletePhoto(const int64_t& numPhoto)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));

	ExecuteSqlWithStatementNoResult("DELETE FROM PHOTO_EXIF WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumPhoto = ?)", parameter);
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOS WHERE NumPhoto = ?", parameter);
}

bool CSqlPhotos::DeletePhotoFolder(const int64_t& idFolder)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(idFolder));

	ExecuteSqlWithStatementNoResult("DELETE FROM PHOTO_EXIF WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumFolderCatalog = ?)", parameter);
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOS WHERE NumFolderCatalog = ?", parameter);
}

bool CSqlPhotos::DeletePhotoCatalog(const int64_t& idCatalog)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(idCatalog));
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOS WHERE NUMFOLDERCATALOG in (SELECT NUMFOLDERCATALOG FROM FOLDERCATALOG WHERE NumCatalog = ?)", parameter);
}

vector<wxString> CSqlPhotos::GetPhotoFromFolder(const int64_t& idFolder)
{
	typeResult = 5;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(idFolder));
	listPhoto.clear();
	ExecuteSqlWithStatement("SELECT FullPath FROM PHOTOS WHERE NumFolderCatalog = ?", parameter);
	return listPhoto;
}


int CSqlPhotos::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (typeResult)
		{
		case 0:
			exif = sqlResult->ColumnDataInt(0);
			break;
		case 1:
			criteriaInsert = sqlResult->ColumnDataInt(0);
			break;
		case 2:
			{
				CCriteria criteria;
				criteria.SetId(sqlResult->ColumnDataInt(0));
				criteria.SetCategorieId(sqlResult->ColumnDataInt(1));
				criteria.SetLibelle(sqlResult->ColumnDataText(2));
				criteriaVector->push_back(criteria);
			}
			break;
		case 3:
			photoPath = sqlResult->ColumnDataText(0);
			break;
		case 4:
			photoId = sqlResult->ColumnDataInt(0);
			break;
		case 5:
			listPhoto.push_back(sqlResult->ColumnDataText(0));
			break;
		}
		nbResult++;
	}
	return nbResult;
}
