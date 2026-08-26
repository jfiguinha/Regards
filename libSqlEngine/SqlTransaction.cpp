#include <header.h>
#include "SqlTransaction.h"
using namespace Regards::Sqlite;

// ── Transactions ────────────────────────────────────────────────────────────

void CSqlTransaction::BeginTransaction()
{
#ifdef USE_TRANSACTION
    m_transaction = CSqlEngine::getInstance(m_databaseName);
    if (!m_transaction) return;

    m_transaction->lock();
    m_transaction->BeginTransaction();
    m_m_useTransaction = true;
#endif
}

// SqlExecuteRequest.cpp
void CSqlTransaction::RollbackTransaction()
{
#ifdef USE_TRANSACTION
    if (!m_transaction) return;

    // Exécution du ROLLBACK directement sur la connexion
    m_transaction->ExecuteSQLWithNoResult("ROLLBACK TRANSACTION");

    m_transaction->unlock();
    m_transaction = nullptr;
    m_useTransaction = false;
#endif
}

void CSqlTransaction::CommitTransaction()
{
#ifdef USE_TRANSACTION
    if (!m_transaction) return;

    m_transaction->CommitTransaction();
    m_transaction->unlock();
    m_transaction = nullptr;
    m_m_useTransaction = false;
#endif
}