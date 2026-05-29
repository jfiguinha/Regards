#include <header.h>
#include "FileWatcherService.h"
#include <SqlFindFolderCatalog.h>
#include <SqlFindPhotos.h>
#include <SqlThumbnail.h>
#include <SqlFacePhoto.h>
#include <FileUtility.h>
#include "SQLRemoveData.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <wx/filename.h>
#include <window_id.h>
#include <wx/busyinfo.h>

using namespace Regards::Viewer;
using namespace Regards::Sqlite;

CFileWatcherService::CFileWatcherService(wxEvtHandler* owner)
    : owner_(owner)
{
}

void CFileWatcherService::EnsureStarted()
{
    if (watcher_ != nullptr)
        return;

    watcher_ = std::make_unique<wxFileSystemWatcher>();
    watcher_->SetOwner(owner_);

    CSqlFindFolderCatalog folderCatalog;
    folderCatalog.GetFolderCatalog(&folderList_, NUMCATALOGID);
    ValidateDatabase(folderList_);
}

bool CFileWatcherService::AddPath(const wxString& dirPath)
{
    if (!watcher_ || !wxDirExists(dirPath))
        return false;

    const wxFileName dirname(dirPath, "");
    return watcher_->AddTree(
        dirname,
        wxFSW_EVENT_CREATE | wxFSW_EVENT_DELETE | wxFSW_EVENT_RENAME | wxFSW_EVENT_MODIFY);
}

bool CFileWatcherService::RemovePath(const wxString& dirPath)
{
    if (!watcher_ || !wxDirExists(dirPath))
        return false;

    const wxFileName dirname(dirPath, "");
    return watcher_->Remove(dirname);
}

bool CFileWatcherService::ValidateDatabase(FolderCatalogVector& folderList)
{
    wxBusyCursor busy;
    bool folderChange = false;

    // Validation des répertoires
    for (auto& folder : folderList)
    {
        if (!wxDirExists(folder.GetFolderPath()))
        {
            CSQLRemoveData::DeleteFolder(folder.GetNumFolder());
            folderChange = true;
        }
        else
        {
            AddPath(folder.GetFolderPath());
        }
    }

    // Validation des photos
    PhotosVector photoList;
    CSqlFindPhotos findPhotos;
    findPhotos.GetAllPhotos(&photoList);
    for (auto& photo : photoList)
    {
        if (!wxFileExists(photo.GetPath()))
        {
            CSQLRemoveData::DeletePhoto(photo.GetId());
            folderChange = true;
        }
    }

    // Validation des thumbnails photos
    CSqlThumbnail sqlThumbnail;
    for (int numPhoto : sqlThumbnail.GetAllPhotoThumbnail())
    {
        if (!wxFileExists(CFileUtility::GetThumbnailPath(std::to_string(numPhoto))))
            sqlThumbnail.EraseThumbnail(numPhoto);
    }

    // Validation des thumbnails visages
    CSqlFacePhoto sqlFacePhoto;
    for (int numPhoto : sqlFacePhoto.GetAllThumbnailFace())
    {
        if (!wxFileExists(CFileUtility::GetFaceThumbnailPath(numPhoto)))
            sqlFacePhoto.EraseFace(numPhoto);
    }

    if (folderChange)
    {
        auto* viewerParam = CMainParamInit::getInstance();
        if (viewerParam)
        {
            CSqlFindPhotos sqlFindPhotos;
            sqlFindPhotos.SearchPhotos(viewerParam->GetLastSqlRequest());
        }
    }

    return folderChange;
}
