#pragma once

#include <sqlite3.h>
using namespace std;

namespace Regards
{
	namespace Sqlite
	{
        class CSqlResult;
        class CSqlParameter;

        // Callback pour les erreurs, évite wxMessageBox dans la logique métier
        using ErrorCallback = std::function<void(const std::string& message)>;

        class CSqlLib
        {
        public:
            explicit CSqlLib(ErrorCallback onError = nullptr);
            virtual ~CSqlLib();

            // Non-copiable
            CSqlLib(const CSqlLib&) = delete;
            CSqlLib& operator=(const CSqlLib&) = delete;

            bool OpenConnection(const wxString& path, bool readonly, bool load_inmemory);
            void CloseConnection();

            virtual bool InitDatabase(const wxString& filename) = 0;
            virtual bool CheckVersion(const wxString& filename) { return false; }

            bool RecoverDatabaseFile(const wxString& filename);
            int  ExecuteSqlWithStatement(const wxString& query, std::vector<std::unique_ptr<CSqlParameter>>& parameter, CSqlResult* result = nullptr);
            int  ExecuteSQLWithNoResult(const wxString& query);
            int  ExecuteSQLSelect(const wxString& query, CSqlResult* result);
            bool ExecuteSQLBlobInsert(const wxString& query, int numCol,
                const void* blob, int blobSize, CSqlResult* result);

            void BeginTransaction();
            void CommitTransaction(); // Faute d'orthographe corrigée
            void lock();
            void unlock();
            void Release();

            int64_t  GetLastId();
            sqlite3* GetDB() { return pCon; }
            bool     isConnected() const { return m_bConnected; }

        protected:
            int  LoadOrSaveDb(sqlite3* pInMemory, const char* zFilename, int isSave);
            int  recoverDatabase(sqlite3* db);
            void reportError(const std::string& msg);
            wxString escapeSqlite(const wxString& str);

            ErrorCallback  m_onError;
            wxString       m_dbPath;
            sqlite3* pCon = nullptr;
            bool           m_bConnected = false;
            bool           m_readonly = false;
            bool           m_loadInMemory = false;
            wxString       m_lastError;
            std::mutex     m_sync;
            // pRes retiré des membres → déclaré localement dans chaque méthode
        };
	}
}
