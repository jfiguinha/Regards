#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <mutex>
#include "SqlResult.h"
using namespace std;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlLib
		{
		public:
			CSqlLib();
			virtual ~CSqlLib();

			bool OpenConnection(const wxString &databasePath, const bool &readonly);
			void CloseConnection();

			virtual bool InitDatabase(const wxString &lpFilename) = 0;
            virtual bool CheckVersion(const wxString &lpFilename)
            {
                return false;
            }
            int ExecuteSQLWithNoResult(const wxString &query);
			bool ExecuteSQLSelect(const wxString &query, CSqlResult * sqlResult);
			bool ExecuteSQLBlobInsert(const wxString &query, const int &numCol, const void * zBlob, const int &nBlob, CSqlResult * sqlResult);
			void BeginTransaction();
			void CommitTransection();
			void lock();
			void unlock();
			void Release();

			int64_t GetLastId();
			sqlite3 * GetDB()
			{
				return pCon;
			}

		private:

			bool isConnected();
	
			std::mutex sync;
			wxString		 sqLiteDBPath;		   //Databse File Dir
			sqlite3		 *pCon;				   //SQLite Connection Object
			sqlite3_stmt * pRes;
			bool m_bConnected;
			wxString  m_strLastError;    /*Last Error String*/
		};
	}
}

