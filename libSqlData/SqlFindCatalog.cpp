#include <header.h>
#include "SqlFindCatalog.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;

CSqlFindCatalog::CSqlFindCatalog()
	: CSqlExecuteRequest(L"RegardsDB")
{
}


CSqlFindCatalog::~CSqlFindCatalog()
{
}

bool CSqlFindCatalog::GetCatalog(CatalogVector* catalogVector)
{
	m_catalogVector = catalogVector;
	return (ExecuteRequest("SELECT NumCatalog, LibelleCatalog FROM CATALOG") != -1) ? true : false;
}

int CSqlFindCatalog::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		CPhotoCatalog _catalog;
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			switch (i)
			{
			case 0:
				_catalog.SetNumCatalog(sqlResult->ColumnDataInt(i));
				break;
			case 1:
				_catalog.SetLibelle(sqlResult->ColumnDataText(i));
				break;
			default: ;
			}
		}
		m_catalogVector->push_back(_catalog);
		nbResult++;
	}
	return nbResult;
};
