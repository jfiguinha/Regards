#include <header.h>
#include "SqlFindYear.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;

CSqlFindYear::CSqlFindYear()
	: CSqlExecuteRequest(L"RegardsDB"), yearVector(nullptr)
{
}


CSqlFindYear::~CSqlFindYear()
{
}

bool CSqlFindYear::SearchUniqueCriteria(vector<wxString>* yearVector)
{
	this->yearVector = yearVector;
	return (ExecuteRequest(
		       "Select distinct SUBSTR(CreateDate, 1, 4) as annee from PHOTOSSEARCHCRITERIA order by annee desc") != -1)
		       ? true
		       : false;
}

int CSqlFindYear::TraitementResult(CSqlResult* sqlResult)
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
			default: ;
			}
		}
		yearVector->push_back(value);
		nbResult++;
	}
	return nbResult;
};
