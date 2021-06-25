#include <header.h>
#include "SqlFolderCatalog.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;

CSqlFolderCatalog::CSqlFolderCatalog()
	: CSqlExecuteRequest(L"RegardsDB"), numCatalogId(0), numFolderCatalogId(0)
{
	typeResult = 1;
}


CSqlFolderCatalog::~CSqlFolderCatalog()
{
}

int64_t CSqlFolderCatalog::GetOrInsertFolderCatalog(const int64_t &numCatalog, const wxString & folderPath)
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

bool CSqlFolderCatalog::InsertFolderCatalog(const int64_t &numCatalog, const wxString & folderPath)
{
    wxString fullpath = folderPath;
    fullpath.Replace("'", "''");
    return ExecuteRequestWithNoResult("INSERT INTO FOLDERCATALOG (NumCatalog, FolderPath) VALUES (" + to_string(numCatalog) + ", '" + fullpath + "')");
}

int64_t CSqlFolderCatalog::GetFolderCatalogId(const int64_t &numCatalog, const wxString & folderPath)
{
    typeResult = 1;
	numFolderCatalogId = -1;
	ExecuteRequest("SELECT NumFolderCatalog FROM FOLDERCATALOG WHERE NumCatalog = " + to_string(numCatalog) + " and FolderPath = '" + folderPath + "'");
	return numFolderCatalogId;
}

wxString CSqlFolderCatalog::GetFolderCatalogPath(const int64_t &numFolder)
{
    typeResult = 1;
	folderPath = "";
	ExecuteRequest("SELECT NumFolderCatalog, NumCatalog, FolderPath FROM FOLDERCATALOG WHERE NumFolderCatalog = " + to_string(numFolder));
	return folderPath;
}

bool CSqlFolderCatalog::DeleteCatalog(const int64_t &numCatalog)
{
	return (ExecuteRequestWithNoResult("DELETE FROM FOLDERCATALOG WHERE numCatalog = " + to_string(numCatalog)) != -1) ? true : false;
}

bool CSqlFolderCatalog::DeleteFolder(const int64_t &numFolder)
{
	return (ExecuteRequestWithNoResult("DELETE FROM FOLDERCATALOG WHERE NumFolderCatalog = " + to_string(numFolder)) != -1) ? true : false;
}

int CSqlFolderCatalog::TraitementResult(CSqlResult * sqlResult)
{
    int nbResult = 0;
    while (sqlResult->Next())
    {
        if (typeResult == 1)
        {
            for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
            {
                switch (i)
                {
                    case 0:
                        numFolderCatalogId = sqlResult->ColumnDataInt(i);
                        break;
                    case 1:
                        numCatalogId = sqlResult->ColumnDataInt(i);;
                        break;
                    case 2:
                        folderPath = sqlResult->ColumnDataText(i);
                        break;
                    default: ;
                }
            }
        }
        nbResult++;
    }

	return nbResult;
}
