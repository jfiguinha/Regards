#include "header.h"
#include "SqlLib.h"
#include "SqlResult.h"
#include <ConvertUtility.h>
#include "sqlite3recover.h"
#include "SqlParameter.h"
using namespace std;
using namespace Regards::Sqlite;
using namespace Regards::Sqlite;

// RAII pour sqlite3_stmt — remplace le pRes membre de classe
struct StmtGuard
{
    sqlite3_stmt* stmt = nullptr;
    ~StmtGuard() { if (stmt) sqlite3_finalize(stmt); }
};

// RAII pour sqlite3* temporaires (fichiers ouverts localement)
struct DbGuard
{
    sqlite3* db = nullptr;
    ~DbGuard() { if (db) sqlite3_close(db); }
};

CSqlLib::CSqlLib(ErrorCallback onError)
    : m_onError(std::move(onError))
{}

CSqlLib::~CSqlLib()
{
    if (pCon)
        CloseConnection();
}

void CSqlLib::reportError(const std::string& msg)
{
    if (m_onError)
        m_onError(msg);
    // Sinon silencieux — c'est l'appelant qui décide
}

// ─── Recover ────────────────────────────────────────────────────────────────

int CSqlLib::recoverDatabase(sqlite3* db)
{
    const char* zLAF = "lost_and_found";
    int bFreelist = 1;
    int bRowids = 1;

    sqlite3_recover* p = sqlite3_recover_init_sql(db, "main",
        [](void*, const char*) { return SQLITE_OK; }, nullptr);

    sqlite3_recover_config(p, SQLITE_RECOVER_LOST_AND_FOUND, (void*)zLAF);
    sqlite3_recover_config(p, SQLITE_RECOVER_ROWIDS, (void*)&bRowids);
    sqlite3_recover_config(p, SQLITE_RECOVER_FREELIST_CORRUPT, (void*)&bFreelist);
    sqlite3_recover_run(p);

    int errCode = sqlite3_recover_errcode(p);
    if (errCode != SQLITE_OK)
        reportError(sqlite3_recover_errmsg(p));

    return sqlite3_recover_finish(p);
}

bool CSqlLib::RecoverDatabaseFile(const wxString& filename)
{
    DbGuard g;
    int rc = sqlite3_open(CConvertUtility::ConvertToStdString(filename).c_str(), &g.db);
    if (rc != SQLITE_OK)
        return false;

    return recoverDatabase(g.db) == SQLITE_OK;
    // g.db fermé automatiquement
}

// ─── LoadOrSaveDb ───────────────────────────────────────────────────────────

int CSqlLib::LoadOrSaveDb(sqlite3* pInMemory, const char* zFilename, int isSave)
{
    DbGuard g;
    int rc = sqlite3_open(zFilename, &g.db);
    if (rc != SQLITE_OK)
    {
        reportError("LoadOrSaveDb: impossible d'ouvrir " + std::string(zFilename));
        return rc;
    }

    sqlite3* pFrom = isSave ? pInMemory : g.db;
    sqlite3* pTo = isSave ? g.db : pInMemory;

    sqlite3_backup* backup = sqlite3_backup_init(pTo, "main", pFrom, "main");
    if (backup)
    {
        sqlite3_backup_step(backup, -1);
        sqlite3_backup_finish(backup);
    }
    return sqlite3_errcode(pTo);
    // g.db fermé automatiquement
}

// ─── OpenConnection ─────────────────────────────────────────────────────────

bool CSqlLib::OpenConnection(const wxString& path, bool readonly, bool load_inmemory)
{
    m_dbPath = path;
    m_readonly = readonly;
    m_loadInMemory = load_inmemory;
    m_bConnected = false;

    int rc = SQLITE_OK;

    if (load_inmemory)
    {
        rc = sqlite3_open(":memory:", &pCon);
        if (rc != SQLITE_OK)
        {
            reportError("Impossible d'ouvrir la DB en mémoire");
            return false;
        }
        if (wxFileExists(m_dbPath))
            rc = LoadOrSaveDb(pCon, CConvertUtility::ConvertToStdString(m_dbPath).c_str(), 0);
    }
    else
    {
        int flags = readonly
            ? SQLITE_OPEN_READONLY | SQLITE_OPEN_FULLMUTEX
            : SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX;

        rc = sqlite3_open_v2(CConvertUtility::ConvertToStdString(m_dbPath).c_str(), &pCon, flags, nullptr);
    }

    if (rc != SQLITE_OK)
    {
        // Tentative unique de récupération — pas de récursion
        reportError("Ouverture échouée, tentative de récupération…");
        if (pCon) { sqlite3_close(pCon); pCon = nullptr; }

        if (!RecoverDatabaseFile(m_dbPath))
        {
            reportError("Récupération impossible pour : " + m_dbPath.utf8_string());
            return false;
        }

        // Réessai une seule fois après récupération
        int flags = readonly
            ? SQLITE_OPEN_READONLY | SQLITE_OPEN_FULLMUTEX
            : SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX;
        rc = sqlite3_open_v2(CConvertUtility::ConvertToStdString(m_dbPath).c_str(), &pCon, flags, nullptr);

        if (rc != SQLITE_OK)
        {
            reportError("Échec définitif de l'ouverture de la base");
            return false;
        }
    }

    m_bConnected = true;
    return true;
}

// ─── CloseConnection ────────────────────────────────────────────────────────

void CSqlLib::CloseConnection()
{
    if (!pCon) return;

    if (!m_readonly && m_loadInMemory)
        LoadOrSaveDb(pCon, CConvertUtility::ConvertToStdString(m_dbPath).c_str(), 1);

    sqlite3_close(pCon);
    pCon = nullptr;
    m_bConnected = false;
}

// ─── Transactions ───────────────────────────────────────────────────────────

void CSqlLib::BeginTransaction()
{
    if (pCon) sqlite3_exec(pCon, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
}

void CSqlLib::CommitTransaction()
{
    if (pCon) sqlite3_exec(pCon, "COMMIT TRANSACTION", nullptr, nullptr, nullptr);
}

// ─── Requêtes ───────────────────────────────────────────────────────────────

int CSqlLib::ExecuteSQLWithNoResult(const wxString& query)
{
    if (!isConnected()) return -1;

    char* err = nullptr;
    if (sqlite3_exec(pCon, CConvertUtility::ConvertToStdString(query).c_str(),
        nullptr, nullptr, &err) != SQLITE_OK)
    {
        m_lastError = err ? err : "unknown error";
        sqlite3_free(err);
        return -1;
    }
    return sqlite3_total_changes(pCon);
}

wxString CSqlLib::escapeSqlite(const wxString& str)
{
    wxString result;

    for (wxChar c : str)
    {
        result += c;
        if (c == '\'')
            result += '\'';
    }

    return result;
}

int CSqlLib::ExecuteSqlWithStatement(
    const wxString& query,
    std::vector<std::unique_ptr<CSqlParameter>>& parameters, CSqlResult* result)
{

    if (!isConnected()) 
        return false;

    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(
        pCon,
        query.ToUTF8().data(),
        -1,
        &stmt,
        nullptr);

    if (rc != SQLITE_OK)
    {
        wxLogError(
            "sqlite3_prepare_v2 failed : %s",
            sqlite3_errmsg(pCon));

        return false;
    }

    int num_param = 1;

    for (auto& param : parameters)
    {
        rc = param->Bind(stmt, num_param);

        if (rc != SQLITE_OK)
        {
            wxLogError(
                "sqlite3_bind failed (param %d) : %s",
                num_param,
                sqlite3_errmsg(pCon));

            sqlite3_finalize(stmt);
            return false;
        }

        ++num_param;
    }

    if (result != nullptr)
    {
        result->SetStatement(stmt); // CSqlResult prend ownership
        return sqlite3_total_changes(pCon);
    }

    rc = sqlite3_step(stmt);

    bool success =
        (rc == SQLITE_DONE || rc == SQLITE_ROW);

    if (!success)
    {
        wxLogError(
            "sqlite3_step failed : %s",
            sqlite3_errmsg(pCon));
    }

    sqlite3_finalize(stmt);
    return success;
}



int CSqlLib::ExecuteSQLSelect(const wxString& query, CSqlResult* result)
{
    if (!isConnected() || !result) return false;

    // stmt local — plus de pRes membre partagé
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(pCon, CConvertUtility::ConvertToStdString(query).c_str(),
        -1, &stmt, nullptr) != SQLITE_OK)
    {
        m_lastError = sqlite3_errmsg(pCon);
        sqlite3_finalize(stmt);
        return -1;
    }
    result->SetStatement(stmt); // CSqlResult prend ownership
    return sqlite3_total_changes(pCon);
}

bool CSqlLib::ExecuteSQLBlobInsert(const wxString& query, int /*numCol*/,
    const void* blob, int blobSize, CSqlResult* /*result*/)
{
    if (!isConnected()) return false;

    StmtGuard g;
    if (sqlite3_prepare_v2(pCon, CConvertUtility::ConvertToStdString(query).c_str(),
        -1, &g.stmt, nullptr) != SQLITE_OK)
    {
        m_lastError = sqlite3_errmsg(pCon);
        return false;
    }

    if (sqlite3_bind_blob(g.stmt, 1, blob, blobSize, SQLITE_STATIC) != SQLITE_OK)
    {
        m_lastError = sqlite3_errmsg(pCon);
        return false;
    }

    if (sqlite3_step(g.stmt) != SQLITE_DONE)
    {
        m_lastError = sqlite3_errmsg(pCon);
        return false;
    }

    return true;
    // g.stmt finalisé automatiquement
}

// ─── Divers ─────────────────────────────────────────────────────────────────

int64_t CSqlLib::GetLastId() { return sqlite3_last_insert_rowid(pCon); }
void    CSqlLib::lock() { m_sync.lock(); }
void    CSqlLib::unlock() { m_sync.unlock(); }
void    CSqlLib::Release() { /* stmt maintenant locaux, rien à faire ici */ }