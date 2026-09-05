#pragma once
#include <wx/string.h>

namespace Regards
{
    namespace Sqlite
    {
        class CSqlLib;

        class CSqlTransaction
        {
        public:
            // CORRECTION CRITIQUE : Le constructeur requiert désormais explicitement la base cible
            explicit CSqlTransaction(const wxString& databaseName)
                : m_databaseName(databaseName)
                , m_transaction(nullptr)
                , m_useTransaction(false)
                , m_committed(false)
            {
                BeginTransaction();
            }

            ~CSqlTransaction()
            {
                if (!m_committed && m_useTransaction)
                    RollbackTransaction();
            }

            void commit()
            {
                if (!m_committed && m_useTransaction)
                {
                    CommitTransaction();
                    m_committed = true;
                }
            }

        private:
            void BeginTransaction();
            void CommitTransaction();
            void RollbackTransaction();

            wxString m_databaseName;
            CSqlLib* m_transaction;
            bool m_useTransaction;
            bool m_committed;
        };
    }
}
