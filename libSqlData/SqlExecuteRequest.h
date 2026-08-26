#pragma once
#include <SqlResult.h>

namespace Regards
{
	namespace Sqlite
	{

		class CSqlLib;
        class CSqlParameter;

        template<typename T>
        int FillVector(CSqlResult* result, std::vector<T>& vec)
        {
            int nbResult = 0;
            while (result->Next())
            {
                vec.push_back(T::Read(result));
                nbResult++;
            }
            return nbResult;
        }
      
        class CSqlExecuteRequest
        {
        public:
            explicit CSqlExecuteRequest(const wxString& databaseName);
            virtual ~CSqlExecuteRequest() = default;

            // Non-copiable (possède un état de transaction)
            CSqlExecuteRequest(const CSqlExecuteRequest&) = delete;
            CSqlExecuteRequest& operator=(const CSqlExecuteRequest&) = delete;
            int  ExecuteSqlWithStatement(const wxString& query, std::vector<std::unique_ptr<CSqlParameter>>& parameter);
            bool ExecuteSqlWithStatementBool(const wxString& query, std::vector<std::unique_ptr<CSqlParameter>>& parameter);
            int  ExecuteRequest(const wxString& sql);
            int  ExecuteRequestWithNoResult(const wxString& sql);
            bool ExecuteSqlWithStatementNoResult(const wxString& query, std::vector<std::unique_ptr<CSqlParameter>>& parameter);
            bool ExecuteInsertBlobData(const wxString& sql, int numCol,
                const void* blob, int blobSize);

        protected:
            wxString   m_databaseName;
            CSqlLib* m_transaction = nullptr; // non-null uniquement entre Begin/Commit
            bool     m_useTransaction = false;

        private:
            // Récupère le CSqlLib actif (transaction ou singleton)
            // et exécute fn(lib) sous lock si nécessaire.
            // Retourne false si lib est introuvable.
            bool withLib(const std::function<void(CSqlLib&)>& fn);



            virtual int TraitementResult(CSqlResult* result) = 0;

        };
	}
}
