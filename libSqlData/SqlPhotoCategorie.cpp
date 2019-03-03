#include <header.h>
#include "SqlPhotoCategorie.h"
using namespace Regards::Sqlite;


CSqlPhotoCategorie::CSqlPhotoCategorie()
	: CSqlExecuteRequest(L"RegardsDB")
{
}


CSqlPhotoCategorie::~CSqlPhotoCategorie()
{
}

//--------------------------------------------------------
//Chargement de toutes les données d'un album
//--------------------------------------------------------
bool CSqlPhotoCategorie::LoadPhotoCategorie(PhotoCategorieVector * photoCategorieVector, const int &numLangue)
{
	m_PhotoCategorieVector = photoCategorieVector;
	return (ExecuteRequest("SELECT NumCategorie, Libelle FROM CATEGORIE WHERE NumLangue = " + to_string(numLangue)) != -1) ? true : false;
}

int CSqlPhotoCategorie::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		CPhotoCategorie _photoCategorie;
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{

			switch (i)
			{
			case 0:
				_photoCategorie.SetId(sqlResult->ColumnDataInt(i));
				break;
			case 1:
				_photoCategorie.SetLibelle(sqlResult->ColumnDataText(i));
				break;
			}
		}
		m_PhotoCategorieVector->push_back(_photoCategorie);
		nbResult++;
	}
	return nbResult;
}