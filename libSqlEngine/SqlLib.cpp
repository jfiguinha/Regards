#include "SqlLib.h"
#include <wx/strconv.h>
using namespace std;
using namespace Regards::Sqlite;

CSqlLib::CSqlLib()
{
	pCon = nullptr;
	m_bConnected = false;
}


CSqlLib::~CSqlLib()
{
	if (pCon)
		CloseConnection();
}


int64_t CSqlLib::GetLastId()
{
	return sqlite3_last_insert_rowid(pCon);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CSqlLib::OpenConnection(const wxString &databasePath, const bool &readonly)
{
    // allocate buffer for the UTF-16 string
    
	sqLiteDBPath = databasePath.ToStdString();

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

    int rc = 0;
    
    if(readonly)
        rc = sqlite3_open_v2(sqLiteDBPath.c_str(), &(pCon), SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX, NULL);
    else
        rc = sqlite3_open(sqLiteDBPath.c_str(), &(pCon));
    //rc = sqlite3_open_v2(sqLiteDBPath.c_str(), &(pCon), SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL);
        
    //rc = sqlite3_open(sqLiteDBPath.c_str(), &(pCon));

	m_strLastError = sqlite3_errmsg(pCon);

	if (!rc)
	{
		if (m_strLastError.find("not an error") == string::npos)
			m_bConnected = false;
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
	if (pCon)
	{
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

bool CSqlLib::ExecuteSQLBlobInsert(const wxString &query, const int &numCol , const void * zBlob, const int &nBlob, CSqlResult * sqlResult)
{
	if (!isConnected())
		return 0;

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
	if (sqlite3_prepare(pCon, query.c_str(), -1, &pRes, nullptr) != SQLITE_OK)
	{
		
		m_strLastError = sqlite3_errmsg(pCon);
		sqlite3_finalize(pRes);

	}
	else
	{
		int rc = sqlite3_bind_blob(pRes, 1, zBlob, nBlob, SQLITE_STATIC);
		if (rc != SQLITE_OK) {
			m_strLastError = sqlite3_errmsg(pCon);
		}
		else {
			rc = sqlite3_step(pRes);
			if (rc != SQLITE_DONE)
				m_strLastError = sqlite3_errmsg(pCon);
		}

		m_bReturn = false;
	}

	//sync.unlock();
	return m_bReturn;
}

bool CSqlLib::ExecuteSQLSelect(const wxString &query, CSqlResult * sqlResult)
{
	if (!isConnected())
		return 0;

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
	if (sqlite3_prepare(pCon, query.c_str(), -1, &pRes, nullptr) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(pCon);
		sqlite3_finalize(pRes);
		m_bReturn = false;
	}
	else
		sqlResult->SetStatement(pRes);

	//sync.unlock();
	return m_bReturn;
}

int CSqlLib::ExecuteSQLWithNoResult(const wxString &query)
{
	int totalChange = -1;

	if (!isConnected())
		return totalChange;

	m_strLastError = "";

	char * err = nullptr;
	
	//sync.lock();

	if (sqlite3_exec(pCon, query.c_str(), nullptr, 0, &err) != SQLITE_OK)
		m_strLastError = (char *)sqlite3_errmsg(pCon);
	else
		totalChange = sqlite3_total_changes(pCon);

	//sync.unlock();

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
