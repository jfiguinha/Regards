#pragma once

#include <wx/datetime.h>
#include <wx/string.h>

/**
 * Centralise les modifications de métadonnées et de critères associées à une photo.
 *
 * La classe ne gère pas les fenêtres wxWidgets ni les notifications UI.
 * Elle effectue uniquement les opérations métier :
 *   - GPS : métadonnées EXIF + critère GPS + table GPS
 *   - Date : métadonnées EXIF + critère Date
 */
class CPhotoCriteriaUtility
{
public:
    CPhotoCriteriaUtility() = delete;

    /**
     * Met à jour les informations GPS d'une photo.
     *
     * @param filename       Fichier concerné.
     * @param latitude       Latitude numérique.
     * @param longitude      Longitude numérique.
     * @param latitudeText   Latitude sous forme texte pour la base GPS.
     * @param longitudeText  Longitude sous forme texte pour la base GPS.
     * @param geoInfos       Libellé du critère GPS.
     * @return true si l'opération métier a pu être réalisée, false en cas d'échec.
     */
    static bool GeolocalizeFile(const wxString& filename,
                                const wxString& latitudeText,
                                const wxString& longitudeText,
                                const wxString& geoInfos,
                                const int& criteriaId = -1);

    /**
     * Met à jour la date d'une photo.
     *
     * @param filename     Fichier concerné.
     * @param newDate      Date wxDateTime utilisée par les APIs natives.
     * @param selectDate   Date texte utilisée pour le critère SQL / EXIF Exiv2.
     * @param criteriaId   ID du critère à mettre à jour.
     * @return true si l'opération métier a pu être réalisée, false en cas d'échec.
     */
    static bool ChangeDateFile(const wxString& filename,
        const wxDateTime& newDate,
        const wxString& selectDate, const int& criteriaId = -1);

private:
    static bool WriteGpsMetadata(const wxString& filename,
                                 double latitude,
                                 double longitude);

    static bool WriteDateMetadata(const wxString& filename,
                                  const wxDateTime& newDate,
                                  const wxString& selectDate);
};
