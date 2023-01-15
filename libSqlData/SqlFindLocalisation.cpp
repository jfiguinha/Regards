#include <header.h>
#include "SqlFindLocalisation.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;

CSqlFindLocalisation::CSqlFindLocalisation()
	: CSqlExecuteRequest(L"RegardsDB"), localisationVector(nullptr)
{
}


CSqlFindLocalisation::~CSqlFindLocalisation()
{
}

bool CSqlFindLocalisation::SearchUniqueCriteria(vector<wxString>* localisationVector, const wxString& day,
                                                const wxString& month, const wxString& year)
{
	this->localisationVector = localisationVector;
	return (ExecuteRequest(
		       "Select distinct GeoGps from PHOTOSSEARCHCRITERIA WHERE CreateDate = '" + year + "." + month + "." + day
		       + "'") != -1)
		       ? true
		       : false;
}

int CSqlFindLocalisation::TraitementResult(CSqlResult* sqlResult)
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
		localisationVector->push_back(value);
		nbResult++;
	}
	return nbResult;
};
