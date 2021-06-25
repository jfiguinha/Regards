#include <header.h>
#include "SqlCriteria.h"
#include "SqlPhotoCategorieUsenet.h"
#include "SqlPhotoCriteria.h"
#include "SqlResult.h"
#include <PhotoCategorieUsenet.h>
using namespace Regards::Sqlite;



CSqlPhotoCategorieUsenet::CSqlPhotoCategorieUsenet()
	: CSqlExecuteRequest(L"RegardsDB"), type(0)
{
}


CSqlPhotoCategorieUsenet::~CSqlPhotoCategorieUsenet()
{
}

//--------------------------------------------------------
//Chargement de toutes les données d'un album
//--------------------------------------------------------
bool CSqlPhotoCategorieUsenet::InsertPhotoProcessing(const wxString &path)
{
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	return (ExecuteRequestWithNoResult("INSERT INTO PHOTO_CATEGORIE_USENET_PROCESSING (FullPath) VALUES ('" + fullpath + "')") != -1) ? true : false;
}

vector<wxString> CSqlPhotoCategorieUsenet::GetPhotoListTreatment()
{
	type = 1;
	listPhoto.clear();
	ExecuteRequest("SELECT FullPath FROM PHOTOS WHERE FullPath not in (select FullPath FROM PHOTO_CATEGORIE_USENET_PROCESSING)");
	return listPhoto;
}

bool CSqlPhotoCategorieUsenet::InsertPhotoCategorie(const int &numPhoto, const int &categorie, const wxString &label)
{
	bool isNew = false;
	CSqlCriteria sqlCriteria;
	CSqlPhotoCriteria photoCriteria;
	int idCriteria = sqlCriteria.GetOrInsertCriteriaId(1, 5, label, isNew);
	return photoCriteria.InsertPhotoCriteria(numPhoto, idCriteria);
}

bool CSqlPhotoCategorieUsenet::DeletePhotoProcessing(const wxString &path)
{
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTO_CATEGORIE_USENET_PROCESSING WHERE FullPath = '" + fullpath + "'") != -1) ? true : false;
}

bool CSqlPhotoCategorieUsenet::DeletePhotoProcessingDatabase()
{
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTO_CATEGORIE_USENET_PROCESSING") != -1) ? true : false;
}

int CSqlPhotoCategorieUsenet::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	wxString filename;
	while (sqlResult->Next())
	{
		CPhotoCategorieUsenet photoCategorie;
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			if (type == 1)
			{
				switch (i)
				{
				case 0:
					filename = sqlResult->ColumnDataText(i);
					break;
				default: ;
				}
			}
		}
		if (type == 1)
			listPhoto.push_back(filename);

		nbResult++;
	}
	return nbResult;
}