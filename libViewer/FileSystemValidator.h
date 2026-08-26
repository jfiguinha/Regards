#pragma once
#include <Photos.h>
#include <FolderCatalog.h>
#include <wx/filename.h>
#include <vector>

namespace Regards::Viewer
{
    // Pure filesystem validation — no SQL, no UI events.
    // Returns lists of stale ids so the caller can batch-delete them via CSqlBatchOps.
    class CFileSystemValidator
    {
    public:
        // Returns the ids of photos whose paths no longer exist on disk.
        static std::vector<int> FindStalePhotos(PhotosVector& photos);

        // Returns the folder ids whose paths no longer exist on disk.
        static std::vector<int> FindStaleFolders(FolderCatalogVector& folders);

        // Returns paths of photos whose thumbnails are invalid (size hash mismatch).
        // Only called when thumbnail validity checking is enabled in config.
        static std::vector<wxString> FindInvalidThumbnails(PhotosVector& photos);

    private:
        // Compute the lightweight hash used for thumbnail validity (file size as string).
        static wxString ComputeFileHash(const wxString& path);
    };
}
