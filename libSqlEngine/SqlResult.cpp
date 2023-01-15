#include "header.h"
#include "SqlResult.h"
#include <wx/ustring.h>
#include <utf8.h>
using namespace std;
using namespace Regards::Sqlite;

CSqlResult::CSqlResult(): pRes(nullptr), m_iColumnCount(0)
{
	//this->pRes = pRes;
}


CSqlResult::~CSqlResult()
{
}

void CSqlResult::SetStatement(sqlite3_stmt* pRes)
{
	this->pRes = pRes;
	m_iColumnCount = sqlite3_column_count(pRes);
}

/*Result Set Definations*/
int CSqlResult::GetColumnCount()
{
	return m_iColumnCount;
}

wxString CSqlResult::NextColumnName(const int& iClmnCount)
{
	if (iClmnCount > m_iColumnCount)
		return "";
	return sqlite3_column_name(pRes, iClmnCount);
}

bool CSqlResult::Next()
{
	return (sqlite3_step(pRes) == SQLITE_ROW) ? true : false;
}

void fix_utf8_string(std::string& str)
{
	std::string temp;
	utf8::replace_invalid(str.begin(), str.end(), back_inserter(temp));
	str = temp;
}

wxString CSqlResult::ColumnDataText(const int& clmNum)
{
	if (clmNum > m_iColumnCount)
		return "";

	char buf[4096];
	/*
	const unsigned char * textValue = sqlite3_column_text(pRes, clmNum);
	wxString utf8 =  wxString::FromUTF8(reinterpret_cast<const char*>(textValue));
	return utf8;
	*/
	int32_t num_bytes = sqlite3_column_bytes(pRes, clmNum);
	memcpy(buf, sqlite3_column_text(pRes, clmNum), num_bytes);
	buf[num_bytes] = '\0';
	//cout << "ColumnDataText buf " << buf << endl;
	wxString utf8 = wxString::FromUTF8(buf);
	/*
	string data = string(buf);
	fix_utf8_string(data);
	
	wxString utf8 =  wxString::FromUTF8(data);
	cout << "ColumnDataText utf8 wxString " << utf8 << endl;
	
	cout << "ColumnDataText utf8 wxCharBuffer " <<  utf8.ToUTF8()  << endl;
	
	string local =  string(utf8.ToUTF8());
	cout << "ColumnDataText utf8 string " <<  local.c_str()  << endl;
	 **/
	return utf8;
}

int CSqlResult::ColumnDataInt(const int& clmNum)
{
	if (clmNum > m_iColumnCount)
		return -1;
	return sqlite3_column_int(pRes, clmNum);
}

int CSqlResult::ColumnDataBlobSize(const int& clmNum)
{
	if (clmNum > m_iColumnCount)
		return -1;

	return sqlite3_column_bytes(pRes, clmNum);
}

int CSqlResult::ColumnDataBlob(const int& clmNum, void* & pzBlob, const int& pnBlob)
{
	if (clmNum > m_iColumnCount)
		return -1;
	memcpy(pzBlob, sqlite3_column_blob(pRes, clmNum), pnBlob);

	return 0;
}

const void* CSqlResult::ColumnDataBlob(const int& clmNum)
{
	if (clmNum > m_iColumnCount)
		return nullptr;
	return sqlite3_column_blob(pRes, clmNum);
}
