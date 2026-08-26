#include <header.h>
#include "SqlPhotoCategorie.h"
#include "SqlResult.h"
#include <SqlParameter.h>
using namespace Regards::Sqlite;


CSqlPhotoCategorie::CSqlPhotoCategorie()
	: CSqlExecuteRequest(L"RegardsDB")
{
}

//--------------------------------------------------------
//Chargement de toutes les données d'un album
//--------------------------------------------------------
bool CSqlPhotoCategorie::LoadPhotoCategorie(PhotoCategorieVector* photoCategorieVector, const int& numLangue)
{
	m_PhotoCategorieVector = photoCategorieVector;
	if (m_PhotoCategorieVector != nullptr)
	{
		m_PhotoCategorieVector->clear();
		std::vector<std::unique_ptr<CSqlParameter>> parameter;
		parameter.push_back(std::make_unique<CSqlInt>(numLangue));
		return ExecuteSqlWithStatement("SELECT NumCategorie, Libelle FROM CATEGORIE WHERE NumLangue = ?", parameter);
	}
	return false;
}

int CSqlPhotoCategorie::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		CPhotoCategorie _photoCategorie;
		_photoCategorie.SetId(sqlResult->ColumnDataInt(0));
		_photoCategorie.SetLibelle(sqlResult->ColumnDataText(1));
		m_PhotoCategorieVector->push_back(_photoCategorie);
		nbResult++;
	}
	return nbResult;
}
