#include <header.h>
#include "SqlResult.h"
#include "SqlCatalog.h"
#include <SqlParameter.h>
using namespace Regards::Sqlite;

CSqlCatalog::CSqlCatalog()
	: CSqlExecuteRequest(L"RegardsDB")
{
	numCatalogId = -1;
}

bool CSqlCatalog::InsertCatalog(const wxString& libelle)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(libelle));
	return ExecuteSqlWithStatementNoResult("INSERT INTO CATALOG (LibelleCatalog) VALUES (?)", parameter);
}

bool CSqlCatalog::UpdateCatalog(const int64_t& numCatalog, const wxString& libelle)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(libelle));
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	return ExecuteSqlWithStatementNoResult("UPDATE CATALOG SET LibelleCatalog = ? WHERE NumCatalog = ?", parameter);
}

int64_t CSqlCatalog::GetCatalogId(const wxString& libelle)
{
	type = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(libelle));
	ExecuteSqlWithStatement("SELECT NumCatalog FROM CATALOG WHERE LibelleCatalog = ?", parameter);
	return numCatalogId;
}

wxString CSqlCatalog::GetCatalogLibelle(const int64_t& numCatalog)
{
	type = 1;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	ExecuteSqlWithStatement("SELECT LibelleCatalog FROM CATALOG WHERE NumCatalog = ?", parameter);
	return libelle;
}

bool CSqlCatalog::DeleteCatalog(const int64_t& numCatalog)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	return ExecuteSqlWithStatementNoResult("DELETE FROM CATALOG WHERE NumCatalog = ?", parameter);
}

int CSqlCatalog::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
{
		switch (type)
		{
		case 0:
			numCatalogId = sqlResult->ColumnDataInt(0);
			break;
		case 1:
			libelle = sqlResult->ColumnDataText(0);
			break;
		default: ;
		}

		nbResult++;
	}
	return nbResult;
};
