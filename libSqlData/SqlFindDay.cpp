#include "SqlFindDay.h"
using namespace Regards::Sqlite;

CSqlFindDay::CSqlFindDay()
	: CSqlExecuteRequest(L"RegardsDB")
{
}


CSqlFindDay::~CSqlFindDay()
{
}

bool CSqlFindDay::SearchUniqueCriteria(vector<wxString> * dayVector, const wxString &month, const wxString &year)
{
	this->dayVector = dayVector;
	return (ExecuteRequest("Select distinct SUBSTR(libelle, 9, 2) as day, SUBSTR(libelle, 6, 2) as mois, SUBSTR(libelle, 1, 4) as annee from criteria CR INNER JOIN PHOTOSCRITERIA as PHCR ON CR.NumCriteria = PHCR.NumCriteria INNER JOIN PHOTOSSEARCHCRITERIA as PH ON PH.NumPhoto = PHCR.NumPhoto  where NumCategorie = 3  and mois = '" + month + "' and annee = '" + year + "' order by day desc") != -1) ? true : false;
}

int CSqlFindDay::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		wxString value;
		for (int i = 0; i < sqlResult->GetColumnCount(); i++)
		{

			switch (i)
			{
			case 0:
				value = sqlResult->ColumnDataText(i);
				break;
			}
		}
		dayVector->push_back(value);
		nbResult++;
	}
	return nbResult;
};