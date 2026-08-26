#include "header.h"
#include "SqlExecuteRequest.h"
#include "SqlLib.h"
#include "SqlEngine.h"
#include "SqlResult.h"
#include <SqlParameter.h>
using namespace Regards::Sqlite;

// ── RAII lock/unlock ────────────────────────────────────────────────────────
// Évite tout unlock() manquant, même en cas de return anticipé
struct LibLockGuard
{
    CSqlLib& lib;
    bool     owns;

    LibLockGuard(CSqlLib& l, bool shouldLock)
        : lib(l), owns(shouldLock)
    {
        if (owns) lib.lock();
    }

    ~LibLockGuard()
    {
        if (owns) lib.unlock();
    }
};

// ── Constructeur ────────────────────────────────────────────────────────────

CSqlExecuteRequest::CSqlExecuteRequest(const wxString& databaseName)
    : m_databaseName(databaseName)
{}

// ── Helper central : supprime toute la duplication ──────────────────────────

bool CSqlExecuteRequest::withLib(const std::function<void(CSqlLib&)>& fn)
{
    if (m_useTransaction)
    {
        // Déjà locké par BeginTransaction
        if (!m_transaction) return false;
        fn(*m_transaction);
        return true;
    }

    CSqlLib* lib = CSqlEngine::getInstance(m_databaseName);
    if (!lib) return false;

    LibLockGuard guard(*lib, true);  // lock → unlock garanti à la sortie
    fn(*lib);
    return true;
}


// ── Requêtes ────────────────────────────────────────────────────────────────

int CSqlExecuteRequest::ExecuteRequest(const wxString& sql)
{
    int nbResult = 0;

    withLib([&](CSqlLib& lib)
        {
            CSqlResult sqlResult;
            if(lib.ExecuteSQLSelect(sql, &sqlResult) != -1)
                nbResult = TraitementResult(&sqlResult);
    });

    return nbResult;
}

bool CSqlExecuteRequest::ExecuteSqlWithStatementBool(const wxString& query, std::vector<std::unique_ptr<CSqlParameter>>& parameter)
{
    int nbResult = 0;

    withLib([&](CSqlLib& lib)
        {
            CSqlResult sqlResult;
            if (lib.ExecuteSqlWithStatement(query, parameter, &sqlResult) != -1)
                nbResult = TraitementResult(&sqlResult);
        });

    return nbResult != -1 ? true : false;
}


int CSqlExecuteRequest::ExecuteSqlWithStatement(const wxString& query, std::vector<std::unique_ptr<CSqlParameter>>& parameter)
{
    int nbResult = 0;

    withLib([&](CSqlLib& lib)
        {
            CSqlResult sqlResult;
            if (lib.ExecuteSqlWithStatement(query, parameter, &sqlResult) != -1)
                nbResult = TraitementResult(&sqlResult);
        });

    return nbResult;
}

bool CSqlExecuteRequest::ExecuteSqlWithStatementNoResult(const wxString& query, std::vector<std::unique_ptr<CSqlParameter>>& parameter)
{
    int nbResult = 0;

    withLib([&](CSqlLib& lib)
        {
            nbResult = lib.ExecuteSqlWithStatement(query, parameter);
        });

    return nbResult != -1 ? true : false;
}

int CSqlExecuteRequest::ExecuteRequestWithNoResult(const wxString& sql)
{
    int nbResult = 0;

    withLib([&](CSqlLib& lib)
        {
            nbResult = lib.ExecuteSQLWithNoResult(sql);
        });

    return nbResult;
}



bool CSqlExecuteRequest::ExecuteInsertBlobData(const wxString& sql, int numCol,
    const void* blob, int blobSize)
{
    bool success = false;

    withLib([&](CSqlLib& lib)
        {
            CSqlResult sqlResult;
            success = lib.ExecuteSQLBlobInsert(sql, numCol, blob, blobSize, &sqlResult);
        });

    return success;
}