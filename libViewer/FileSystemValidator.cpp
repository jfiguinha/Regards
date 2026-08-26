#include <header.h>
#include "FileSystemValidator.h"
#include <SqlThumbnail.h>
#include <wx/wx.h>

using namespace Regards::Sqlite;
using namespace Regards::Viewer;

std::vector<int> CFileSystemValidator::FindStalePhotos(PhotosVector& photos)
{
    std::vector<int> staleIds;
    staleIds.reserve(photos.size() / 4); // typical churn is small
    for (CPhotos& photo : photos)
    {
        if (!wxFileExists(photo.GetPath()))
            staleIds.push_back(photo.GetId());
    }
    return staleIds;
}

std::vector<int> CFileSystemValidator::FindStaleFolders(FolderCatalogVector& folders)
{
    std::vector<int> staleIds;
    for (CFolderCatalog& folder : folders)
    {
        if (!wxDirExists(folder.GetFolderPath()))
            staleIds.push_back(folder.GetNumFolder());
    }
    return staleIds;
}

std::vector<wxString> CFileSystemValidator::FindInvalidThumbnails(PhotosVector& photos)
{
    std::vector<wxString> invalidPaths;
    CSqlThumbnail sqlThumbnail;

    for (CPhotos& photo : photos)
    {
        if (!wxFileName::FileExists(photo.GetPath()))
            continue; // already handled by FindStalePhotos

        wxString hash = ComputeFileHash(photo.GetPath());
        if (!sqlThumbnail.TestThumbnail(photo.GetPath(), hash))
            invalidPaths.push_back(photo.GetPath());
    }
    return invalidPaths;
}

wxString CFileSystemValidator::ComputeFileHash(const wxString& path)
{
    wxFileName file(path);
    wxULongLong size = file.GetSize();
    return size.ToString();
}
