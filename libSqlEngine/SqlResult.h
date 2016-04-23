#pragma once
#include <sqlite3.h>
#include <string>
#include "wx/strconv.h"
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
			
			void SetStatement(sqlite3_stmt * pRes);
			/*Result Set Definations*/
			int	GetColumnCount();
			bool Next();

			wxString NextColumnName(const int &iClmnCount);
			wxString ColumnDataText(const int & clmNum);
			int ColumnDataInt(const int & clmNum);
			int ColumnDataBlob(const int & clmNum, void * & pzBlob, const int & pnBlob);
			int ColumnDataBlobSize(const int & clmNum);
			const void * ColumnDataBlob(const int & clmNum);
		private:
			sqlite3_stmt * pRes;
			int m_iColumnCount;
		};
	}
}

