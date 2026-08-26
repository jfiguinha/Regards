#include <header.h>
#include "SqlFindFolderCatalog.h"
#include "SqlResult.h"
#include <SqlParameter.h>
using namespace Regards::Sqlite;


CSqlFindFolderCatalog::CSqlFindFolderCatalog()
	: CSqlExecuteRequest(L"RegardsDB"), m_catalogFolderVector(nullptr), nbCatalog(0), typeRequest(0)
{
}

int CSqlFindFolderCatalog::GetNbFolderCatalog(const int& numCatalog)
{
	nbCatalog = 0;
	typeRequest = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	ExecuteSqlWithStatement("SELECT count(*) as nb FROM FOLDERCATALOG WHERE NumCatalog = ?", parameter);
	return nbCatalog;
}

bool CSqlFindFolderCatalog::GetFolderCatalog(FolderCatalogVector* catalogfolderVector, const int& numCatalog)
{
	typeRequest = 1;
	m_catalogFolderVector = catalogfolderVector;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	int nbResult = ExecuteSqlWithStatement("SELECT NumFolderCatalog, NumCatalog, FolderPath FROM FOLDERCATALOG WHERE NumCatalog = ?", parameter);
	return nbResult > 0 ? true : false;
}

int CSqlFindFolderCatalog::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (typeRequest)
		{
		case 0:
			nbCatalog = sqlResult->ColumnDataInt(0);
			break;
		case 1:
			{
				CFolderCatalog _foldercatalog;
				_foldercatalog.SetNumFolder(sqlResult->ColumnDataInt(0));
				_foldercatalog.SetNumCatalog(sqlResult->ColumnDataInt(1));
				_foldercatalog.SetFolderPath(sqlResult->ColumnDataText(2));
				m_catalogFolderVector->push_back(_foldercatalog);
			}
			break;
		}
		nbResult++;
	}
	return nbResult;
}
