#include <header.h>
#include "SqlFindCatalog.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;

CSqlFindCatalog::CSqlFindCatalog()
	: CSqlExecuteRequest(L"RegardsDB"), m_catalogVector(nullptr)
{
}


bool CSqlFindCatalog::GetCatalog(CatalogVector* catalogVector)
{
	if (catalogVector == nullptr)
		return false;

	catalogVector->clear();
	m_catalogVector = catalogVector;
	return (ExecuteRequest("SELECT NumCatalog, LibelleCatalog FROM CATALOG") != -1) ? true : false;
}

int CSqlFindCatalog::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		CPhotoCatalog _catalog;
		_catalog.SetNumCatalog(sqlResult->ColumnDataInt(0));
		_catalog.SetLibelle(sqlResult->ColumnDataText(1));
		m_catalogVector->push_back(_catalog);
		nbResult++;
	}
	return nbResult;
};
