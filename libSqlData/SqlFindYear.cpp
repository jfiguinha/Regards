#include "SqlFindYear.h"
using namespace Regards::Sqlite;

CSqlFindYear::CSqlFindYear()
	: CSqlExecuteRequest(L"RegardsDB")
{
}


CSqlFindYear::~CSqlFindYear()
{
}

bool CSqlFindYear::SearchUniqueCriteria(vector<wxString> * yearVector)
{
	this->yearVector = yearVector;
	return (ExecuteRequest("Select distinct SUBSTR(libelle, 1, 4) as annee from criteria CR INNER JOIN PHOTOSCRITERIA as PHCR ON CR.NumCriteria = PHCR.NumCriteria INNER JOIN PHOTOSSEARCHCRITERIA as PH ON PH.NumPhoto = PHCR.NumPhoto where NumCategorie = 3 order by annee desc") != -1) ? true : false;
}

int CSqlFindYear::TraitementResult(CSqlResult * sqlResult)
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
		yearVector->push_back(value);
		nbResult++;
	}
	return nbResult;
};