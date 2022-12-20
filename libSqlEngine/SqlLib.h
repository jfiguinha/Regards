#pragma once

#include <sqlite3.h>
using namespace std;

namespace Regards
{
	namespace Sqlite
	{
        class CSqlResult;
        
		class CSqlLib
		{
		public:
			CSqlLib();
			virtual ~CSqlLib();

			bool OpenConnection(const wxString &databasePath, const bool &readonly, const bool &load_inmemory);
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

		protected:

			int LoadOrSaveDb(sqlite3 *pInMemory, const char *zFilename, int isSave);
			bool isConnected();
	
			mutex sync;
			wxString		 sqLiteDBPath;		   //Databse File Dir
			sqlite3		 *pCon;				   //SQLite Connection Object
			sqlite3_stmt * pRes;
			bool m_bConnected;
			bool readonly;
			bool load_inmemory;
			wxString  m_strLastError;    /*Last Error String*/
		};
	}
}

