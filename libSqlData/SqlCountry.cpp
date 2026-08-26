#include <header.h>
#include "SqlCountry.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;

CSqlCountry::CSqlCountry()
	: CSqlExecuteRequest(L"RegardsDB"), m_countryVector(nullptr)
{
}

/////////////////////////////////////////////////////////////////
//Chargement des informations sur les attributs
/////////////////////////////////////////////////////////////////
bool CSqlCountry::GetCountry(CountryVector* countryVector)
{
	if (countryVector == nullptr)
		return false;

	countryVector->clear();
	m_countryVector = countryVector;
	return (ExecuteRequest("SELECT NumCountry, CodeCountry, LibelleContinent, LibelleCountry FROM COUNTRY") != -1)
		       ? true
		       : false;
}

int CSqlCountry::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		CCountry _cCountry;
		_cCountry.SetId(sqlResult->ColumnDataInt(0));
		_cCountry.SetCode(sqlResult->ColumnDataText(1));
		_cCountry.SetContinent(sqlResult->ColumnDataText(2));
		_cCountry.SetLibelle(sqlResult->ColumnDataText(3));
		m_countryVector->push_back(_cCountry);
		nbResult++;
	}
	return nbResult;
};
