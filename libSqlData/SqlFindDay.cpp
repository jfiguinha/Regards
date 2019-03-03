#include <header.h>
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
	return (ExecuteRequest("Select distinct SUBSTR(CreateDate, 9, 2) as day, SUBSTR(CreateDate, 6, 2) as mois, SUBSTR(CreateDate, 1, 4) as annee from PHOTOSSEARCHCRITERIA  where mois = '" + month + "' and annee = '" + year + "' order by day desc") != -1) ? true : false;
}

int CSqlFindDay::TraitementResult(CSqlResult * sqlResult)
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
		dayVector->push_back(value);
		nbResult++;
	}
	return nbResult;
};