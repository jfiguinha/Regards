#include "SqlCountry.h"
using namespace Regards::Sqlite;

CSqlCountry::CSqlCountry()
	: CSqlExecuteRequest(L"RegardsDB")
{
}


CSqlCountry::~CSqlCountry()
{
}

/////////////////////////////////////////////////////////////////
//Chargement des informations sur les attributs
/////////////////////////////////////////////////////////////////
bool CSqlCountry::GetCountry(CountryVector * countryVector)
{
	m_countryVector = countryVector;
	return (ExecuteRequest("SELECT NumCountry, CodeCountry, LibelleContinent, LibelleCountry FROM COUNTRY") != -1) ? true : false;;
}

int CSqlCountry::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		CCountry _cCountry;
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{

			switch (i)
			{
			case 0:
				_cCountry.SetId(sqlResult->ColumnDataInt(i));
				break;
			case 1:
				_cCountry.SetCode(sqlResult->ColumnDataText(i));
				break;
			case 2:
				_cCountry.SetContinent(sqlResult->ColumnDataText(i));
				break;
			case 3:
				_cCountry.SetLibelle(sqlResult->ColumnDataText(i));
				break;
			}
		}
		m_countryVector->push_back(_cCountry);
		nbResult++;
	}
	return nbResult;
};
