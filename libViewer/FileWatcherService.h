#pragma once
#include <wx/wx.h>
#include <wx/fswatcher.h>
#include <FolderCatalog.h>
#include <memory>

namespace Regards::Viewer
{
    // Surveille les répertoires du catalogue et notifie le owner via wxEVT_FSWATCHER.
    // Lance aussi la validation initiale de la base (dossiers/photos/thumbnails/faces).
    class CFileWatcherService
    {
    public:
        explicit CFileWatcherService(wxEvtHandler* owner);
        ~CFileWatcherService() = default;

        // Crée le watcher et vérifie le catalogue si ce n'est pas déjà fait.
        void EnsureStarted();

        bool AddPath(const wxString& dirPath);
        bool RemovePath(const wxString& dirPath);

        bool IsStarted() const { return watcher_ != nullptr; }

    private:
        // Valide les entrées BDD (dossiers, photos, thumbnails, faces) et
        // lance une re-recherche si des entrées ont été supprimées.
        bool ValidateDatabase(FolderCatalogVector& folderList);

        wxEvtHandler* owner_ = nullptr;
        std::unique_ptr<wxFileSystemWatcher> watcher_;
        FolderCatalogVector folderList_;
    };
}
