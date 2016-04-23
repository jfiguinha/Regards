#include "SqlFindLocalisation.h"
using namespace Regards::Sqlite;

CSqlFindLocalisation::CSqlFindLocalisation()
	: CSqlExecuteRequest(L"RegardsDB")
{
}


CSqlFindLocalisation::~CSqlFindLocalisation()
{
}

bool CSqlFindLocalisation::SearchUniqueCriteria(vector<wxString> * localisationVector, const wxString &day, const wxString &month, const wxString &year)
{
	this->localisationVector = localisationVector;
	return (ExecuteRequest("Select distinct Libelle from Criteria WHERE NumCriteria in (Select distinct NumCriteria from PhotosCriteria where NumPhoto in (SELECT PH.NumPhoto FROM PHOTOSSEARCHCRITERIA as PH INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE CR.Libelle = '" + year + "." + month + "." + day + "')) and NumCategorie = 1") != -1) ? true : false;
}

int CSqlFindLocalisation::TraitementResult(CSqlResult * sqlResult)
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
		localisationVector->push_back(value);
		nbResult++;
	}
	return nbResult;
};