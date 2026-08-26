#pragma once
#include <sqlite3.h>
using namespace std;


namespace Regards
{
	namespace Sqlite
	{


		class CSqlResult
		{
		public:
			CSqlResult();
			~CSqlResult();

			int GetInt(const wxString& name);
			wxString GetText(const wxString& name);
			int GetInt(int index);
			wxString GetText(int index);

			void SetStatement(sqlite3_stmt* pRes);
			/*Result Set Definations*/
			int GetColumnCount();
			bool Next();

			wxString NextColumnName(const int& iClmnCount);
			wxString ColumnDataText(const int& clmNum);
			int ColumnDataInt(const int& clmNum);
			int ColumnDataBlob(const int& clmNum, void* & pzBlob, const int& pnBlob);
			int ColumnDataBlobSize(const int& clmNum);
			const void* ColumnDataBlob(const int& clmNum);

		private:

			int GetColumnIndex(const wxString& name);
			wxString GetColumnName(int index);


			sqlite3_stmt* pRes;
			int m_iColumnCount;
		};
	}
}
