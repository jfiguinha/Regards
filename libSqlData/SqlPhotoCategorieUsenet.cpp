#include <header.h>
#include "SqlPhotoCategorieUsenet.h"
using namespace Regards::Sqlite;



CSqlPhotoCategorieUsenet::CSqlPhotoCategorieUsenet()
	: CSqlExecuteRequest(L"RegardsDB")
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
	ExecuteRequest("SELECT FullPath FROM PHOTOSTHUMBNAIL WHERE FullPath not in (select FullPath FROM PHOTO_CATEGORIE_USENET_PROCESSING)");
	return listPhoto;
}

vector<CPhotoCategorieUsenet> CSqlPhotoCategorieUsenet::GetPhotoListCategorie(const int &numPhoto)
{
	type = 2;
	listCategoriePhoto.clear();
	ExecuteRequest("SELECT NumPhoto,NumCategorie,libelle) FROM PHOTO_CATEGORIE_USENET WHERE NumPhoto = " + to_string(numPhoto));
	return listCategoriePhoto;
}

bool CSqlPhotoCategorieUsenet::InsertPhotoCategorie(const int &numPhoto, const int &categorie, const wxString &label)
{
	return (ExecuteRequestWithNoResult("INSERT INTO PHOTO_CATEGORIE_USENET (NumPhoto,NumCategorie,libelle) VALUES (" + to_string(numPhoto) + "," + to_string(categorie) + ",'" + label + "')") != -1) ? true : false;
}

bool CSqlPhotoCategorieUsenet::DeletePhotoProcessing(const wxString &path)
{
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSGPS WHERE FullPath = '" + fullpath + "'") != -1) ? true : false;
}

bool CSqlPhotoCategorieUsenet::DeletePhotoProcessingDatabase()
{
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSGPS") != -1) ? true : false;
}

bool CSqlPhotoCategorieUsenet::DeletePhotoCategorie(const int &numPhoto)
{
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTO_CATEGORIE_USENET WHERE NumPhoto = '" + to_string(numPhoto) + "'") != -1) ? true : false;
}

bool CSqlPhotoCategorieUsenet::DeletePhotoCategorieDatabase()
{
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTO_CATEGORIE_USENET") != -1) ? true : false;
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
				}
			}
			else if (type == 2)
			{
				
				switch (i)
				{
				case 0:
					photoCategorie.numPhoto = sqlResult->ColumnDataInt(i);
					break;
				case 1:
					photoCategorie.numCategorie = sqlResult->ColumnDataInt(i);
					break;
				case 2:
					photoCategorie.libelle = sqlResult->ColumnDataText(i);
					break;
				}
				
			}
		}
		if (type == 1)
			listPhoto.push_back(filename);
		if (type == 2)
			listCategoriePhoto.push_back(photoCategorie);
		nbResult++;
	}
	return nbResult;
}