#pragma once
#include <Photos.h>
#include <FolderCatalog.h>
#include <vector>
#include <wx/wx.h>

namespace Regards::Sqlite { class CSqlFindPhotos; }

namespace Regards::Viewer
{
    // Encapsulates all bulk SQL read/write operations used during folder refresh.
    // Separates data-layer concerns from UI and filesystem logic.
    class CSqlBatchOps
    {
    public:
        // Load all photos from the catalog into an output vector.
        // Returns false if the query fails.
        static bool LoadAllPhotos(PhotosVector& out);

        // Load all folder catalog entries into an output vector.
        static bool LoadAllFolders(FolderCatalogVector& out);

        // Load photos filtered by the current criteria/folder selection.
        static bool LoadPhotosByCriteria(PhotosVector& out);

        // Delete a batch of photo records by id in a single transaction.
        // Returns the number of rows actually deleted.
        static int  DeletePhotosBatch(const std::vector<int>& ids);

        // Delete a batch of folder records in a single transaction.
        static int  DeleteFoldersBatch(const std::vector<int>& folderIds);

        // Delete thumbnail entries for a list of paths in a single transaction.
        static void DeleteThumbnailsBatch(const std::vector<wxString>& paths);

        // Import new files from all watched folders.
        // Returns total number of newly inserted files.
        static int  ImportNewFiles(FolderCatalogVector& folders);

        // Refresh the in-memory search result set using the last SQL request.
        static void RefreshSearchResults();
    };
}
