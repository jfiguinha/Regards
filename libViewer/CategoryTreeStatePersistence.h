#pragma once
#include <PositionElement.h>

using namespace Regards::Window;
namespace Regards::Viewer
{
    class CMainParam;
    // -------------------------------------------------------------------------
    // CategoryTreeStatePersistence
    //
    // Responsabilité unique : sérialiser / désérialiser l'état visuel de
    // l'arbre (cases cochées + triangles ouverts) vers/depuis CMainParam.
    //
    // Format de stockage (chaîne délimitée par ';') :
    //   exifKey:key:1;exifKey:key:0;...
    // -------------------------------------------------------------------------
    class CategoryTreeStatePersistence
    {
    public:
        explicit CategoryTreeStatePersistence(CMainParam* config);

        // Chargement depuis la configuration persistante
        void Load();

        // Sauvegarde de l'état des checkboxes
        // vectorPosElement doit être la collection vectorPosElementDynamic de la vue.
        void SaveCheckState(const PositionElementVector& elements);

        // Sauvegarde de l'état des triangles
        void SaveTriangleState(const PositionElementVector& elements);

        // Lecture de l'état persisté
        bool GetCheckState(const wxString& exifKey, const wxString& key, int numCategorie) const;
        bool GetTriangleState(const wxString& exifKey, const wxString& key) const;

    private:
        static bool LookupState(const wxString& stateString,
                                const wxString& exifKey,
                                const wxString& key,
                                bool defaultValue);

        CMainParam* config_       = nullptr;
        wxString    stateValue_;          // état checkboxes sérialisé
        wxString    stateTriangleValue_;  // état triangles sérialisé
    };
}
