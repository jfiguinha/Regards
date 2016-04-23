#include "SqlFolderCatalog.h"
#ifdef __APPLE__
#include <AppleSelectFile.h>
#endif
using namespace Regards::Sqlite;

CSqlFolderCatalog::CSqlFolderCatalog()
	: CSqlExecuteRequest(L"RegardsDB")
{
}


CSqlFolderCatalog::~CSqlFolderCatalog()
{
}


int64_t CSqlFolderCatalog::GetOrInsertFolderCatalog(const int64_t &numCatalog, const wxString & folderPath)
{
	int64_t numFolderId = GetFolderCatalogId(numCatalog, folderPath);
	if (numFolderId == -1)
	{
#ifdef __APPLE__
        CAppleSelectFile appleSelectFile(folderPath);
        CAppleSecurityScopeData * securityScope = appleSelectFile.GetFileSecurityScope();
		InsertFolderCatalog(numCatalog, folderPath, securityScope);
        delete securityScope;
#else
        InsertFolderCatalog(numCatalog, folderPath);
#endif
		numFolderId = GetLastId();
	}
	return numFolderId;
}

#ifdef __APPLE__
bool CSqlFolderCatalog::InsertFolderCatalog(const int64_t &numCatalog, const wxString & folderPath, CAppleSecurityScopeData * appleSecurityScopeData)
{
    wxString fullpath = folderPath;
    fullpath.Replace("'", "''");
    return ExecuteInsertBlobData("INSERT INTO FOLDERCATALOG (NumCatalog, FolderPath, SecurityBookmark) VALUES (" + to_string(numCatalog) + ", '" + fullpath + "',?)", 2, appleSecurityScopeData->data, appleSecurityScopeData->size );
}

CAppleSecurityScopeData * CSqlFolderCatalog::GetDataSecurityScope(const wxString & folderPath)
{
    typeResult = 2;
    wxString fullpath = folderPath;
    fullpath.Replace("'", "''");
    ExecuteRequest("SELECT FolderPath, SecurityBookmark FROM FOLDERCATALOG WHERE FolderPath = '" + fullpath + "'");
    return appleSecurityScopeData;
}
#else

bool CSqlFolderCatalog::InsertFolderCatalog(const int64_t &numCatalog, const wxString & folderPath)
{
    wxString fullpath = folderPath;
    fullpath.Replace("'", "''");
    return ExecuteRequestWithNoResult("INSERT INTO FOLDERCATALOG (NumCatalog, FolderPath) VALUES (" + to_string(numCatalog) + ", '" + fullpath + "')");
}

#endif

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
            for (int i = 0; i < sqlResult->GetColumnCount(); i++)
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
                }
            }
        }
#ifdef __APPLE__
        else if (typeResult == 2)
        {
            appleSecurityScopeData = new CAppleSecurityScopeData();
            
            for (int i = 0; i < sqlResult->GetColumnCount(); i++)
            {
                switch (i)
                {
                    case 0:
                        appleSecurityScopeData->filePath = sqlResult->ColumnDataText(i);
                        break;
                    case 1:
                    {
                        appleSecurityScopeData->size = sqlResult->ColumnDataBlobSize(i);
                        if (appleSecurityScopeData->size > 0)
                        {
                            appleSecurityScopeData->data = new uint8_t[appleSecurityScopeData->size];
                            sqlResult->ColumnDataBlob(i, (void * &)appleSecurityScopeData->data, appleSecurityScopeData->size);
                            
                        }
                        break;
                    }
                }
            }
        }
#endif
        nbResult++;
    }

	return nbResult;
}