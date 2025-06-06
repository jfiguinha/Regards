#include "header.h"
#include "SqlLib.h"
#include "SqlResult.h"
#include <ConvertUtility.h>
#include "sqlite3recover.h"
using namespace std;
using namespace Regards::Sqlite;

CSqlLib::CSqlLib()
{
	load_inmemory = false;
	pCon = nullptr;
	m_bConnected = false;
	readonly = false;
	pCon = nullptr; //SQLite Connection Object
	pRes = nullptr;
}


CSqlLib::~CSqlLib()
{
	if (pCon)
		CloseConnection();
}

/* Verbosity level for the dbsqlfuzz test runner */
static int eVerbosity = 0;

/*
** This function is used as a callback by the recover extension. Simply
** print the supplied SQL statement to stdout.
*/
static int recoverSqlCb(void *pCtx, const char *zSql){
  if( eVerbosity>=2 ){
    printf("%s\n", zSql);
  }
  return SQLITE_OK;
}

/*
** This function is called to recover data from the database.
*/
int CSqlLib::recoverDatabase(sqlite3 *db){
  int rc;                                 /* Return code from this routine */
  const char *zLAF = "lost_and_found";    /* Name of "lost_and_found" table */
  int bFreelist = 1;                      /* True to scan the freelist */
  int bRowids = 1;                        /* True to restore ROWID values */
  sqlite3_recover *p;                     /* The recovery object */

  p = sqlite3_recover_init_sql(db, "main", recoverSqlCb, 0);
  sqlite3_recover_config(p, SQLITE_RECOVER_LOST_AND_FOUND, (void*)zLAF);
  sqlite3_recover_config(p, SQLITE_RECOVER_ROWIDS, (void*)&bRowids);
  sqlite3_recover_config(p, SQLITE_RECOVER_FREELIST_CORRUPT,(void*)&bFreelist);
  sqlite3_recover_run(p);
  if( sqlite3_recover_errcode(p)!=SQLITE_OK ){
    const char *zErr = sqlite3_recover_errmsg(p);
    int errCode = sqlite3_recover_errcode(p);
    if( eVerbosity>0 ){
      printf("recovery error: %s (%d)\n", zErr, errCode);
    }
  }
  rc = sqlite3_recover_finish(p);
  if( eVerbosity>0 && rc ){
     printf("recovery returns error code %d\n", rc);
  }
  return rc;
}


int64_t CSqlLib::GetLastId()
{
	return sqlite3_last_insert_rowid(pCon);
}


int CSqlLib::LoadOrSaveDb(sqlite3* pInMemory, const char* zFilename, int isSave)
{
	int rc; /* Function return code */
	sqlite3* pFile; /* Database connection opened on zFilename */
	sqlite3_backup* pBackup; /* Backup object used to copy data */
	sqlite3* pTo; /* Database to copy to (pFile or pInMemory) */
	sqlite3* pFrom; /* Database to copy from (pFile or pInMemory) */

	/* Open the database file identified by zFilename. Exit early if this fails
	** for any reason. */
	rc = sqlite3_open(zFilename, &pFile);
	if (rc == SQLITE_OK)
	{
		/* If this is a 'load' operation (isSave==0), then data is copied
		** from the database file just opened to database pInMemory.
		** Otherwise, if this is a 'save' operation (isSave==1), then data
		** is copied from pInMemory to pFile.  Set the variables pFrom and
		** pTo accordingly. */
		pFrom = (isSave ? pInMemory : pFile);
		pTo = (isSave ? pFile : pInMemory);

		/* Set up the backup procedure to copy from the "main" database of
		** connection pFile to the main database of connection pInMemory.
		** If something goes wrong, pBackup will be set to NULL and an error
		** code and message left in connection pTo.
		**
		** If the backup object is successfully created, call backup_step()
		** to copy data from pFile to pInMemory. Then call backup_finish()
		** to release resources associated with the pBackup object.  If an
		** error occurred, then an error code and message will be left in
		** connection pTo. If no error occurred, then the error code belonging
		** to pTo is set to SQLITE_OK.
		*/
		pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
		if (pBackup)
		{
			(void)sqlite3_backup_step(pBackup, -1);
			(void)sqlite3_backup_finish(pBackup);
		}
		rc = sqlite3_errcode(pTo);
	}
    else
    {
        recoverDatabase(pFile);
        /* Open the database file identified by zFilename. Exit early if this fails
        ** for any reason. */
        rc = sqlite3_open(zFilename, &pFile);
        if (rc != SQLITE_OK)
        {
            wxMessageBox("Database Error","Information", wxICON_ERROR);
            exit(0);
        }
    }

	/* Close the database connection opened on database file zFilename
	** and return the result of this function. */
	(void)sqlite3_close(pFile);


	return rc;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CSqlLib::OpenConnection(const wxString& databasePath, const bool& readonly, const bool& load_inmemory)
{
	// allocate buffer for the UTF-16 string

	sqLiteDBPath = databasePath;

	m_bConnected = true;

	/*
	const wxChar *widestr = SQLiteDBPath.c_str();
	const size_t widelen = SQLiteDBPath.length();
	
	// allocate buffer for the UTF-16 string:
	wxMBConvUTF16 conv;
	const size_t utf16bufLen = conv.FromWChar(NULL, 0, widestr, widelen);
	wxCharBuffer utf16buf(utf16bufLen);
	
	// and convert wxString to UTF-16 (=UChar*):
	const char16_t * str = (const char16_t *)utf16buf.data();
	size_t len = conv.FromWChar(utf16buf.data(), utf16bufLen, widestr, widelen) / 2;
	*/
	//const char16_t * database =u"/Users/jacques/developpement/test.db";
	//const char * fichier = CConvertUtility::ConvertFromwxString(sqLiteDBPath);
	int rc;
	this->readonly = readonly;
	this->load_inmemory = load_inmemory;

	if (load_inmemory)
	{
		rc = sqlite3_open(":memory:", &(pCon));
		if (wxFileExists(sqLiteDBPath))
		{
			rc = LoadOrSaveDb(pCon, sqLiteDBPath, 0);
			m_strLastError = sqlite3_errmsg(pCon);
		}
		else
			return true;
	}
	else
	{
		if (readonly)
			rc = sqlite3_open_v2(CConvertUtility::ConvertToUTF8(sqLiteDBPath), &(pCon),
			                     SQLITE_OPEN_READONLY | SQLITE_OPEN_FULLMUTEX, nullptr);
		else
			rc = sqlite3_open_v2(CConvertUtility::ConvertToUTF8(sqLiteDBPath), &(pCon),
			                     SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, nullptr);
		m_strLastError = sqlite3_errmsg(pCon);
	}
    
    if (rc != SQLITE_OK)
    {
        recoverDatabase(pCon);
        m_bConnected = OpenConnection(databasePath, readonly, load_inmemory);
        if (!m_bConnected)
        {
            wxMessageBox("Database Error - Unable to loaded Regards.db data information","Information", wxICON_ERROR);
            exit(0);
        }        
    }
    else
    {
        if (!rc)
        {
            if (m_strLastError.find("not an error") == string::npos)
                m_bConnected = false;
        }

    }
	return m_bConnected;
}


void CSqlLib::lock()
{
	sync.lock();
}

void CSqlLib::unlock()
{
	sync.unlock();
}

void CSqlLib::CloseConnection()
{
	//const char * fichier = CConvertUtility::ConvertFromwxString(sqLiteDBPath);
	if (pCon)
	{
		if (!readonly)
			LoadOrSaveDb(pCon, sqLiteDBPath, 1);
		sqlite3_close(pCon);
		pCon = nullptr;
	}
}

void CSqlLib::BeginTransaction()
{
	if (pCon)
		sqlite3_exec(pCon, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
}

void CSqlLib::CommitTransection()
{
	if (pCon)
		sqlite3_exec(pCon, "COMMIT TRANSACTION", nullptr, nullptr, nullptr);
}

bool CSqlLib::ExecuteSQLBlobInsert(const wxString& query, const int& numCol, const void* zBlob, const int& nBlob,
                                   CSqlResult* sqlResult)
{
	if (!isConnected())
		return false;

	bool m_bReturn = true;

	/*
	const wxChar *widestr = query.c_str();
	const size_t widelen = query.length();
	
	// allocate buffer for the UTF-16 string:
	wxMBConvUTF16 conv;
	const size_t utf16bufLen = conv.FromWChar(NULL, 0, widestr, widelen);
	wxCharBuffer utf16buf(utf16bufLen);
	
	// and convert wxString to UTF-16 (=UChar*):
	const char16_t * str = (const char16_t *)utf16buf.data();
	size_t len = conv.FromWChar(utf16buf.data(), utf16bufLen, widestr, widelen) / 2;
	 */
	if (sqlite3_prepare(pCon, CConvertUtility::ConvertToUTF8(query), -1, &pRes, nullptr) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(pCon);
		sqlite3_finalize(pRes);
	}
	else
	{
		int rc = sqlite3_bind_blob(pRes, 1, zBlob, nBlob, SQLITE_STATIC);
		if (rc != SQLITE_OK)
		{
			m_strLastError = sqlite3_errmsg(pCon);
		}
		else
		{
			rc = sqlite3_step(pRes);
			if (rc != SQLITE_DONE)
				m_strLastError = sqlite3_errmsg(pCon);
		}

		m_bReturn = false;
	}

	//sync.unlock();
	return m_bReturn;
}

bool CSqlLib::ExecuteSQLSelect(const wxString& query, CSqlResult* sqlResult)
{
	if (!isConnected())
		return false;

	bool m_bReturn = true;

	/*
	const wxChar *widestr = query.c_str();
	const size_t widelen = query.length();
	
	// allocate buffer for the UTF-16 string:
	wxMBConvUTF16 conv;
	const size_t utf16bufLen = conv.FromWChar(NULL, 0, widestr, widelen);
	wxCharBuffer utf16buf(utf16bufLen);
	
	// and convert wxString to UTF-16 (=UChar*):
	const char16_t * str = (const char16_t *)utf16buf.data();
	size_t len = conv.FromWChar(utf16buf.data(), utf16bufLen, widestr, widelen) / 2;
	 */
	if (sqlite3_prepare(pCon, CConvertUtility::ConvertToUTF8(query), -1, &pRes, nullptr) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(pCon);
		sqlite3_finalize(pRes);
		m_bReturn = false;
	}
	else
		sqlResult->SetStatement(pRes);

    //printf("CSqlLib::ExecuteSQLSelect : %s \n", m_strLastError.ToStdString().c_str());
	//sync.unlock();
	return m_bReturn;
}

int CSqlLib::ExecuteSQLWithNoResult(const wxString& query)
{
	int totalChange = -1;

	if (!isConnected())
		return totalChange;

	m_strLastError = "";

	char* err = nullptr;

	//sync.lock();
	//const char * sqlquery = CConvertUtility::ConvertFromwxString(query);
	//printf("Sql Query : %s \n",sqlquery);

	if (sqlite3_exec(pCon, CConvertUtility::ConvertToUTF8(query), nullptr, nullptr, &err) != SQLITE_OK)
		m_strLastError = sqlite3_errmsg(pCon);
	else
		totalChange = sqlite3_total_changes(pCon);

	//sync.unlock();
	//printf("Sql Query totalChange : %d \n",totalChange);
	return totalChange;
}

bool CSqlLib::isConnected()
{
	return m_bConnected;
}

void CSqlLib::Release()
{
	sqlite3_finalize(pRes);
	//m_strLastError = L"";
}
