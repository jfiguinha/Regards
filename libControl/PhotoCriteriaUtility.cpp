#include <header.h>
#include "PhotoCriteriaUtility.h"

#include <libPicture.h>
#include <SqlCriteria.h>
#include <SqlGps.h>
#include <SqlPhotoCriteria.h>
#include <SqlPhotos.h>

#ifdef EXIV2
#include <MetadataExiv2.h>
#elif defined(__APPLE__)
#include <AppleReadExif.h>
#elif defined(WIN32)
#include <SetMetadataGps.h>
#include <SetMetadataDate.h>
#endif

#include <cmath>

using namespace Regards::exiv2;
using namespace Regards::Picture;
using namespace Regards::Sqlite;

namespace
{
    constexpr int GPS_CATEGORY_ID = 1;
    constexpr int DATE_CATEGORY_ID = 3;
    constexpr int MAIN_CRITERIA_ID = 1;

    wxString FormatCoordinate(const double value)
    {
        return wxString::Format("%.8f", std::fabs(value));
    }

    wxString GetLatitudeReference(const double latitude)
    {
        return latitude < 0.0 ? "S" : "N";
    }

    wxString GetLongitudeReference(const double longitude)
    {
        return longitude < 0.0 ? "W" : "E";
    }
}

bool CPhotoCriteriaUtility::WriteGpsMetadata(const wxString& filename,
                                              const double latitude,
                                              const double longitude)
{
    CLibPicture libPicture;
    if (!libPicture.TestIsExifCompatible(filename))
        return true;

    const wxString latitudeRef = GetLatitudeReference(latitude);
    const wxString longitudeRef = GetLongitudeReference(longitude);
    const wxString latitudeValue = FormatCoordinate(latitude);
    const wxString longitudeValue = FormatCoordinate(longitude);

    try
    {
#ifdef EXIV2
        CMetadataExiv2 metadata(filename);
        metadata.SetGpsInfos(latitudeRef,
                             longitudeRef,
                             latitudeValue,
                             longitudeValue);
#elif defined(__APPLE__)
        CAppleReadExif appleReadExif;
        appleReadExif.WriteGps(filename, latitude, longitude);
#elif defined(WIN32)
        CSetMetadataGps metadata(latitudeRef,
                                 longitudeRef,
                                 latitudeValue,
                                 longitudeValue);
        metadata.SetMetadata(filename);
#else
        (void)filename;
        (void)latitude;
        (void)longitude;
#endif
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool CPhotoCriteriaUtility::WriteDateMetadata(const wxString& filename,
                                               const wxDateTime& newDate,
                                               const wxString& selectDate)
{
    CLibPicture libPicture;
    if (!libPicture.TestIsExifCompatible(filename))
        return true;

    try
    {
#ifdef EXIV2
        CMetadataExiv2 metadata(filename);
        metadata.SetDateTime(selectDate);
#elif defined(__APPLE__)
        CAppleReadExif appleReadExif;
        appleReadExif.WriteDateTime(filename, newDate);
#elif defined(WIN32)
        CSetMetadataDate metadata(newDate.FormatDate());
        metadata.SetMetadata(filename);
#else
        (void)filename;
        (void)newDate;
        (void)selectDate;
#endif
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool CPhotoCriteriaUtility::GeolocalizeFile(const wxString& filename,
                                            const wxString& latitudeText,
                                            const wxString& longitudeText,
                                            const wxString& geoInfos, const int& criteriaId)
{
    if (filename.empty() || geoInfos.empty())
        return false;

    double latitude = 0.0;
    double longitude = 0.0;
    const bool validLatitude = latitudeText.ToDouble(&latitude);
    const bool validLongitude = longitudeText.ToDouble(&longitude);

    if (!validLatitude || !validLongitude)
        return false;

    if (!std::isfinite(latitude) || !std::isfinite(longitude) ||
        latitude < -90.0 || latitude > 90.0 ||
        longitude < -180.0 || longitude > 180.0)
    {
        return false;
    }

    if (!WriteGpsMetadata(filename, latitude, longitude))
        return false;

    CSqlPhotos sqlPhotos;
    const int numPhotoId = sqlPhotos.GetPhotoId(filename);
    if (numPhotoId < 0)
        return false;

    bool isNew = false;
    CSqlCriteria sqlCriteria;
    CSqlGps sqlGps;
    CSqlPhotoCriteria sqlPhotoCriteria;

	const int oldCriteriaId = criteriaId == -1 ? sqlCriteria.GetCriteriaIdByCategorie(numPhotoId, GPS_CATEGORY_ID) : criteriaId;

    const int numCriteriaId =
        sqlCriteria.GetOrInsertCriteriaId(MAIN_CRITERIA_ID,
                                          GPS_CATEGORY_ID,
                                          geoInfos,
                                          isNew);

    if (numCriteriaId < 0)
        return false;

    sqlPhotoCriteria.InsertPhotoCriteria(numPhotoId, numCriteriaId);

    if (oldCriteriaId != -1 && numCriteriaId != oldCriteriaId)
        sqlPhotoCriteria.DeletePhotoCriteria(numPhotoId, oldCriteriaId);

    sqlGps.DeleteGps(filename);
    sqlGps.InsertGps(filename, latitudeText, longitudeText);

    return true;
}

bool CPhotoCriteriaUtility::ChangeDateFile(const wxString& filename,
                                           const wxDateTime& newDate,
                                           const wxString& selectDate, const int& criteriaId)
{
    if (filename.empty() || selectDate.empty() || !newDate.IsValid())
        return false;

    if (!WriteDateMetadata(filename, newDate, selectDate))
        return false;

    CSqlCriteria sqlCriteria;
    CSqlPhotoCriteria sqlPhotoCriteria;
    CSqlPhotos sqlPhotos;

    const int numPhotoId = sqlPhotos.GetPhotoId(filename);
    if (numPhotoId < 0)
        return false;

    const int oldCriteriaId = criteriaId == -1 ? sqlCriteria.GetCriteriaIdByCategorie(numPhotoId, DATE_CATEGORY_ID) : criteriaId;

    bool isNew = false;
    const int numCriteriaId =
        sqlCriteria.GetOrInsertCriteriaId(MAIN_CRITERIA_ID,
                                          DATE_CATEGORY_ID,
                                          selectDate,
                                          isNew);

    if (numCriteriaId < 0)
        return false;

    sqlPhotoCriteria.InsertPhotoCriteria(numPhotoId, numCriteriaId);

    if (oldCriteriaId != -1 && numCriteriaId != oldCriteriaId)
        sqlPhotoCriteria.DeletePhotoCriteria(numPhotoId, oldCriteriaId);

    return true;
}
