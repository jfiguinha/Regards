#include <header.h>
#include "SqlFolderCatalog.h"
#include "SqlResult.h"
#include <SqlParameter.h>
using namespace Regards::Sqlite;

CSqlFolderCatalog::CSqlFolderCatalog()
	: CSqlExecuteRequest(L"RegardsDB"), numCatalogId(0), numFolderCatalogId(0)
{
	typeResult = 1;
}


int64_t CSqlFolderCatalog::GetOrInsertFolderCatalog(const int64_t& numCatalog, const wxString& folderPath)
{
	int64_t numFolderId = GetFolderCatalogId(numCatalog, folderPath);
	if (numFolderId == -1)
	{
		InsertFolderCatalog(numCatalog, folderPath);
		//numFolderId = GetLastId();
		numFolderId = GetFolderCatalogId(numCatalog, folderPath);
	}
	return numFolderId;
}

bool CSqlFolderCatalog::InsertFolderCatalog(const int64_t& numCatalog, const wxString& folderPath)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	parameter.push_back(std::make_unique<CSqlString>(folderPath));
	return ExecuteSqlWithStatementNoResult("INSERT INTO FOLDERCATALOG (NumCatalog, FolderPath) VALUES (?, ?)", parameter);
}

int64_t CSqlFolderCatalog::GetFolderCatalogId(const int64_t& numCatalog, const wxString& folderPath)
{

	typeResult = 0;
	numFolderCatalogId = -1;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	parameter.push_back(std::make_unique<CSqlString>(folderPath));

	ExecuteSqlWithStatement("SELECT NumFolderCatalog FROM FOLDERCATALOG WHERE NumCatalog = ? and FolderPath = ?", parameter);
	return numFolderCatalogId;
}

wxString CSqlFolderCatalog::GetFolderCatalogPath(const int64_t& numFolder)
{
	typeResult = 1;
	folderPath = "";
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFolder));
	ExecuteSqlWithStatement("SELECT FolderPath FROM FOLDERCATALOG WHERE NumFolderCatalog = ?", parameter);
	return folderPath;
}

bool CSqlFolderCatalog::DeleteCatalog(const int64_t& numCatalog)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	return ExecuteSqlWithStatementNoResult("DELETE FROM FOLDERCATALOG WHERE numCatalog = ? ", parameter);
}

bool CSqlFolderCatalog::DeleteFolder(const int64_t& numFolder)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFolder));
	return ExecuteSqlWithStatementNoResult("DELETE FROM FOLDERCATALOG WHERE NumFolderCatalog = ?", parameter);
}

int CSqlFolderCatalog::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (typeResult)
		{
		case 0:
			numFolderCatalogId = sqlResult->ColumnDataInt(0);
			break;
		case 1:
			folderPath = sqlResult->ColumnDataText(0);
			break;
		}

		nbResult++;
	}

	return nbResult;
}
