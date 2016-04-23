#include "SqlCatalog.h"
using namespace Regards::Sqlite;

CSqlCatalog::CSqlCatalog()
	: CSqlExecuteRequest(L"RegardsDB")
{
	numCatalogId = -1;
}


CSqlCatalog::~CSqlCatalog()
{
}


bool CSqlCatalog::InsertCatalog(const wxString & libelle)
{
	return (ExecuteRequestWithNoResult("INSERT INTO CATALOG (LibelleCatalog) VALUES ('" + libelle + "')") != -1) ? true : false;
}

bool CSqlCatalog::UpdateCatalog(const int64_t &numCatalog, const wxString & libelle)
{
	return (ExecuteRequestWithNoResult("UPDATE CATALOG SET LibelleCatalog = '" + libelle + "' WHERE NumCatalog = " + to_string(numCatalog)) != -1) ? true : false;
}

int64_t CSqlCatalog::GetCatalogId(const wxString & libelle)
{
	ExecuteRequest("SELECT NumCatalog FROM CATALOG WHERE LibelleCatalog = '" + libelle + "'");
	return numCatalogId;
}

wxString CSqlCatalog::GetCatalogLibelle(const int64_t &numCatalog)
{
	ExecuteRequest("SELECT NumCatalog, LibelleCatalog FROM CATALOG WHERE NumCatalog = " + to_string(numCatalog));
	return libelle;
}

bool CSqlCatalog::DeleteCatalog(const int64_t &numCatalog)
{
	return (ExecuteRequestWithNoResult("DELETE FROM CATALOG WHERE NumCatalog = " + to_string(numCatalog)) != -1) ? true : false;
}

int CSqlCatalog::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		for (int i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			switch (i)
			{
			case 0:
				numCatalogId = sqlResult->ColumnDataInt(i);
				break;
			case 1:
				libelle = sqlResult->ColumnDataText(i);
				break;
			}
		}
		nbResult++;
	}
	return nbResult;
};