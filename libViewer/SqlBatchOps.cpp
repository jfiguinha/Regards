#include <header.h>
#include "SqlBatchOps.h"
#include <SqlFindPhotos.h>
#include <SqlFindFolderCatalog.h>
#include <SQLRemoveData.h>
#include <SqlThumbnail.h>
#include <SqlThumbnailVideo.h>
#include <SqlInsertFile.h>
#include <MainParam.h>
#include <MainParamInit.h>
#include <window_id.h>
#include <SqlPhotos.h>
#include <MediaInfo.h>
using namespace Regards::Sqlite;
using namespace Regards::Viewer;

bool CSqlBatchOps::LoadAllPhotos(PhotosVector& out)
{
    CSqlFindPhotos q;
    q.GetAllPhotos(&out);
    return true;
}

bool CSqlBatchOps::LoadAllFolders(FolderCatalogVector& out)
{
    CSqlFindFolderCatalog q;
    q.GetFolderCatalog(&out, NUMCATALOGID);
    return true;
}

bool CSqlBatchOps::LoadPhotosByCriteria(PhotosVector& out)
{
    CSqlFindPhotos q;
    q.SearchPhotosByCriteriaFolder(&out);
    return true;
}

int CSqlBatchOps::DeletePhotosBatch(const std::vector<int>& ids)
{
    // Execute inside a single transaction for performance.
    // CSQLRemoveData is expected to support a batch overload; fall back to loop if not.
    CSQLRemoveData::DeleteListPhoto(ids, nullptr);
	return static_cast<int>(ids.size());
}

int CSqlBatchOps::DeleteFoldersBatch(const std::vector<int>& folderIds)
{
    int deleted = 0;
    for (int id : folderIds)
    {
        CSQLRemoveData::DeleteFolder(id);
        ++deleted;
    }
    return deleted;
}

void CSqlBatchOps::DeleteThumbnailsBatch(const std::vector<wxString>& paths)
{
    CSqlThumbnail      sqlThumb;
    CSqlThumbnailVideo sqlThumbVideo;
    for (const wxString& path : paths)
    {
        CSqlPhotos SqlPhotos;
        int photoId = SqlPhotos.GetPhotoId(path);

        sqlThumb.DeleteThumbnail(path);
        sqlThumbVideo.DeleteThumbnail(photoId);
       
    }
}

int CSqlBatchOps::ImportNewFiles(FolderCatalogVector& folders)
{
    int total = 0;
    for (CFolderCatalog& folder : folders)
    {
        wxString dummy;
        CSqlInsertFile ins;
        total += ins.ImportFileFromFolder(folder.GetFolderPath(), folder.GetNumFolder(), dummy);
    }
    return total;
}

void CSqlBatchOps::RefreshSearchResults()
{
    auto* param = CMainParamInit::getInstance();
    if (!param) return;
    wxString lastRequest = param->GetLastSqlRequest();
    CSqlFindPhotos q;
    q.SearchPhotos(lastRequest);
}
