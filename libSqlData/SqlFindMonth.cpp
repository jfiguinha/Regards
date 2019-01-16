#include "SqlFindMonth.h"
using namespace Regards::Sqlite;

CSqlFindMonth::CSqlFindMonth()
	: CSqlExecuteRequest(L"RegardsDB")
{
}


CSqlFindMonth::~CSqlFindMonth()
{
}

bool CSqlFindMonth::SearchUniqueCriteria(vector<wxString> * monthVector, const wxString &year)
{
	this->monthVector = monthVector;
	return (ExecuteRequest("Select distinct SUBSTR(CreateDate, 6, 2) as mois, SUBSTR(CreateDate, 1, 4) as annee from PHOTOSSEARCHCRITERIA where annee = '" + year + "' order by mois desc") != -1) ? true : false;
}

int CSqlFindMonth::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		wxString value;
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{

			switch (i)
			{
			case 0:
				value = sqlResult->ColumnDataText(i);
				break;
			}
		}
		monthVector->push_back(value);
		nbResult++;
	}
	return nbResult;
};