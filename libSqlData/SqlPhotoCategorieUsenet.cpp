#include <header.h>
#include "SqlCriteria.h"
#include "SqlPhotoCategorieUsenet.h"
#include "SqlPhotoCriteria.h"
#include "SqlResult.h"
#include <PhotoCategorieUsenet.h>
#include <SqlParameter.h>
using namespace Regards::Sqlite;


CSqlPhotoCategorieUsenet::CSqlPhotoCategorieUsenet()
	: CSqlExecuteRequest(L"RegardsDB"), type(0)
{
}


//--------------------------------------------------------
//Chargement de toutes les données d'un album
//--------------------------------------------------------
bool CSqlPhotoCategorieUsenet::InsertPhotoProcessing(const wxString& path)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(path));
	return ExecuteSqlWithStatementNoResult("INSERT INTO PHOTO_CATEGORIE_USENET_PROCESSING (FullPath) VALUES (?)", parameter);
}

vector<wxString> CSqlPhotoCategorieUsenet::GetPhotoListTreatment()
{
	type = 0;
	listPhoto.clear();
	ExecuteRequest("SELECT FullPath FROM PHOTOS WHERE FullPath not in (select FullPath FROM PHOTO_CATEGORIE_USENET_PROCESSING)");
	return listPhoto;
}

bool CSqlPhotoCategorieUsenet::InsertPhotoCategorie(const int& numPhoto, const int& categorie, const wxString& label)
{
	bool isNew = false;
	CSqlCriteria sqlCriteria;
	CSqlPhotoCriteria photoCriteria;
	int idCriteria = sqlCriteria.GetOrInsertCriteriaId(1, 5, label, isNew);
	return photoCriteria.InsertPhotoCriteria(numPhoto, idCriteria);
}

bool CSqlPhotoCategorieUsenet::DeletePhotoProcessing(const wxString& path)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(path));
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTO_CATEGORIE_USENET_PROCESSING WHERE FullPath = ?", parameter);
}

bool CSqlPhotoCategorieUsenet::DeletePhotoProcessingDatabase()
{
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTO_CATEGORIE_USENET_PROCESSING") != -1) ? true : false;
}

int CSqlPhotoCategorieUsenet::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;

	while (sqlResult->Next())
	{
		switch (type)
		{
		case 0:
			listPhoto.push_back(sqlResult->ColumnDataText(0));
			break;
		}	

		nbResult++;
	}
	return nbResult;
}
