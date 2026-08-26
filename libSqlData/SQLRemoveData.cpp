#include <header.h>
#include "SQLRemoveData.h"
#include "SqlFolderCatalog.h"
#include "SqlPhotos.h"
#include "SqlCriteria.h"
#include "SqlPhotoCriteria.h"
#include "SqlThumbnail.h"
#include "SqlThumbnailVideo.h"
#include "SqlFindCriteria.h"
#include "SqlFacePhoto.h"
#include "SqlFaceLabel.h"
#include "SqlFaceRecognition.h"
#include "SqlPhotoCategorieUsenet.h"
#include "SqlTransaction.h"
using namespace Regards::Sqlite;
using namespace Regards::Sqlite;



// ── Suppression face (factorisée) ───────────────────────────────────────────

bool CSQLRemoveData::DeleteFaceData()
{
    bool ok = true;

    CSqlFacePhoto sqlFacePhoto;
    ok &= sqlFacePhoto.DeleteFaceDatabase();
    ok &= sqlFacePhoto.DeleteFaceTreatmentDatabase();

    CSqlFaceRecognition sqlFaceRecognition;
    ok &= sqlFaceRecognition.DeleteFaceRecognitionDatabase();

    CSqlFaceLabel sqlFaceLabel;
    ok &= sqlFaceLabel.DeleteFaceLabelDatabase();

    return ok;
}

// ── DeleteCatalog ───────────────────────────────────────────────────────────

bool CSQLRemoveData::DeleteCatalog(int numCatalog)
{
    bool ok = true;
    CSqlTransaction sqlTransaction;

    ok &= CSqlThumbnail().EraseThumbnail();
    ok &= CSqlThumbnailVideo().EraseThumbnail();

    ok &= CSqlCriteria().DeleteCriteriaCatalog(numCatalog);
    ok &= CSqlPhotoCriteria().DeletePhotoCriteria();

    CSqlPhotos sqlPhoto;
    ok &= sqlPhoto.DeletePhotoCatalog(numCatalog);

    ok &= CSqlFolderCatalog().DeleteCatalog(numCatalog);
    ok &= DeleteFaceData();
    ok &= CSqlPhotoCategorieUsenet().DeletePhotoProcessingDatabase();

    sqlTransaction.commit();
    return ok;
}

// ── DeleteFaceDatabase ──────────────────────────────────────────────────────

bool CSQLRemoveData::DeleteFaceDatabase()
{
    CSqlTransaction sqlTransaction;
    bool ok = DeleteFaceData();
    sqlTransaction.commit();
    return ok;
}

// ── DeleteFolder ────────────────────────────────────────────────────────────

bool CSQLRemoveData::DeleteFolder(int numFolder)
{
    CSqlTransaction sqlTransaction;
    bool ok = true;

    ok &= CSqlThumbnail().EraseFolderThumbnail(numFolder);
    ok &= CSqlThumbnailVideo().EraseFolderThumbnail(numFolder);
    ok &= CSqlPhotoCriteria().DeleteFolderCriteria(numFolder);
    ok &= CSqlFolderCatalog().DeleteFolder(numFolder);

    CSqlPhotos sqlPhoto;
    const std::vector<wxString> listPhoto = sqlPhoto.GetPhotoFromFolder(numFolder);
    ok &= sqlPhoto.DeletePhotoFolder(numFolder);

    ok &= CSqlFacePhoto().DeleteListOfPhoto(listPhoto);
    ok &= CSqlCriteria().DeleteCriteriaAlone();

    CSqlPhotoCategorieUsenet photoCategorie;
    for (const wxString& photoPath : listPhoto)   // const& : pas de copie
        ok &= photoCategorie.DeletePhotoProcessing(photoPath);

    sqlTransaction.commit();
    return ok;
}

// ── DeleteListPhoto ─────────────────────────────────────────────────────────

bool CSQLRemoveData::DeleteListPhoto(const std::vector<int>& listPhoto,
    CriteriaVector* criteriaVector)
{
    CSqlTransaction sqlTransaction;
    bool ok = true;

    CSqlFacePhoto().DeleteListOfPhoto(listPhoto);

    CSqlThumbnail        sqlThumbnail;
    CSqlThumbnailVideo   sqlThumbnailVideo;
    CSqlPhotoCriteria    sqlPhotoCriteria;
    CSqlPhotos           sqlPhoto;

    for (int photoId : listPhoto)
    {
        ok &= sqlThumbnail.DeleteThumbnail(photoId);
        ok &= sqlThumbnailVideo.DeleteThumbnail(photoId);
        ok &= sqlPhotoCriteria.DeletePhoto(photoId);
        ok &= sqlPhoto.DeletePhoto(photoId);
    }

    // Instanciés une seule fois, hors boucle
    if (criteriaVector)
        CSqlFindCriteria().SearchCriteriaAlone(criteriaVector);

    ok &= CSqlCriteria().DeleteCriteriaAlone();

    sqlTransaction.commit();
    return ok;
}

// ── DeletePhoto ─────────────────────────────────────────────────────────────
// Délègue à DeleteListPhoto : plus de duplication de logique

bool CSQLRemoveData::DeletePhoto(int idPhoto)
{
    return DeleteListPhoto({ idPhoto }, nullptr);
}