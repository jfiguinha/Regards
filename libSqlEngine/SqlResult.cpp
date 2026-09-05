#include "header.h"
#include "SqlResult.h"
#include <algorithm>

using namespace std;
using namespace Regards::Sqlite;

CSqlResult::CSqlResult() : pRes(nullptr), m_iColumnCount(0) {}

CSqlResult::~CSqlResult()
{
	if (pRes != nullptr)
		sqlite3_finalize(pRes);
}

void CSqlResult::SetStatement(sqlite3_stmt* pRes)
{
	this->pRes = pRes;
	m_iColumnCount = sqlite3_column_count(pRes);
}

int CSqlResult::GetColumnCount() { return m_iColumnCount; }

wxString CSqlResult::NextColumnName(const int& iClmnCount)
{
	if (iClmnCount >= m_iColumnCount) return "";
	return wxString::FromUTF8(sqlite3_column_name(pRes, iClmnCount));
}

bool CSqlResult::Next()
{
	return (sqlite3_step(pRes) == SQLITE_ROW);
}

int CSqlResult::GetColumnIndex(const wxString& name)
{
	for (int i = 0; i < GetColumnCount(); i++)
	{
		if (name == GetColumnName(i)) return i;
	}
	return -1;
}

int CSqlResult::GetInt(const wxString& name) { return ColumnDataInt(GetColumnIndex(name)); }
wxString CSqlResult::GetText(const wxString& name) { return ColumnDataText(GetColumnIndex(name)); }
wxString CSqlResult::GetColumnName(int index) { return wxString::FromUTF8(sqlite3_column_name(pRes, index)); }
int CSqlResult::GetInt(int index) { return ColumnDataInt(index); }
wxString CSqlResult::GetText(int index) { return ColumnDataText(index); }

wxString CSqlResult::ColumnDataText(const int& clmNum)
{
	if (clmNum < 0 || clmNum >= m_iColumnCount)
		return "";

	const unsigned char* textValue = sqlite3_column_text(pRes, clmNum);
	if (textValue == nullptr)
		return "";

	int num_bytes = sqlite3_column_bytes(pRes, clmNum);
	// CORRECTION CRITIQUE : Plus de copie de tampon fixe, élimination de 100% des risques d'overflows
	return wxString::FromUTF8(reinterpret_cast<const char*>(textValue), num_bytes);
}

int CSqlResult::ColumnDataInt(const int& clmNum)
{
	if (clmNum < 0 || clmNum >= m_iColumnCount) return -1;
	return sqlite3_column_int(pRes, clmNum);
}

int CSqlResult::ColumnDataBlobSize(const int& clmNum)
{
	if (clmNum < 0 || clmNum >= m_iColumnCount) return -1;
	return sqlite3_column_bytes(pRes, clmNum);
}

int CSqlResult::ColumnDataBlob(const int& clmNum, void*& pzBlob, const int& pnBlob)
{
	if (clmNum < 0 || clmNum >= m_iColumnCount || pzBlob == nullptr)
		return -1;

	const void* sqliteBlob = sqlite3_column_blob(pRes, clmNum);
	if (!sqliteBlob) return -1;

	int realSize = sqlite3_column_bytes(pRes, clmNum);
	int bytesToCopy = std::min(realSize, pnBlob); // CORRECTION CRITIQUE : Empêche l'écriture hors-bornes

	std::memcpy(pzBlob, sqliteBlob, bytesToCopy);
	return 0;
}

const void* CSqlResult::ColumnDataBlob(const int& clmNum)
{
	if (clmNum < 0 || clmNum >= m_iColumnCount) return nullptr;
	return sqlite3_column_blob(pRes, clmNum);
}
