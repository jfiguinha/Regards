#include <header.h>
#include "SqlFindFolderCatalog.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;


CSqlFindFolderCatalog::CSqlFindFolderCatalog()
	: CSqlExecuteRequest(L"RegardsDB"), m_catalogFolderVector(nullptr), nbCatalog(0), typeRequest(0)
{
}


CSqlFindFolderCatalog::~CSqlFindFolderCatalog()
{
}

int CSqlFindFolderCatalog::GetNbFolderCatalog(const int& numCatalog)
{
	nbCatalog = 0;
	typeRequest = 0;
	ExecuteRequest("SELECT count(*) as nb FROM FOLDERCATALOG WHERE NumCatalog = " + to_string(numCatalog));
	return nbCatalog;
}

bool CSqlFindFolderCatalog::GetFolderCatalog(FolderCatalogVector* catalogfolderVector, const int& numCatalog)
{
	typeRequest = 1;
	m_catalogFolderVector = catalogfolderVector;
	return (ExecuteRequest(
		       "SELECT NumFolderCatalog, NumCatalog, FolderPath FROM FOLDERCATALOG WHERE NumCatalog = " + to_string(
			       numCatalog)) != -1)
		       ? true
		       : false;
}

int CSqlFindFolderCatalog::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		if (typeRequest == 0)
		{
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					nbCatalog = sqlResult->ColumnDataInt(i);
					break;
				default: ;
				}
			}
		}
		else
		{
			CFolderCatalog _foldercatalog;
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					_foldercatalog.SetNumFolder(sqlResult->ColumnDataInt(i));
					break;
				case 1:
					_foldercatalog.SetNumCatalog(sqlResult->ColumnDataInt(i));
					break;
				case 2:
					_foldercatalog.SetFolderPath(sqlResult->ColumnDataText(i));
					break;
				default: ;
				}
			}
			m_catalogFolderVector->push_back(_foldercatalog);
		}
		nbResult++;
	}
	return nbResult;
}
