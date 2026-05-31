#pragma once
#include <PositionElement.h>
namespace Regards::Window
{
    class CPositionElement;
    class CWindowMain;
}

using namespace Regards::Window;

namespace Regards::Viewer
{
    class CategoryRepository;

    // -------------------------------------------------------------------------
    // CategoryQueryService
    //
    // Responsabilité unique : à partir de l'état des checkboxes de l'arbre,
    // construire la requête SQL de filtrage des photos et la soumettre.
    // Notifie windowMain si la requête a changé.
    // -------------------------------------------------------------------------
    class CategoryQueryService
    {
    public:
        CategoryQueryService(CategoryRepository& repo, CWindowMain* windowMain);

        // Construit et retourne la requête SQL sans l'exécuter
        wxString BuildSqlRequest(PositionElementVector& elements) const;

        // Construit, exécute et notifie (si changement)
        void UpdateAndNotify(PositionElementVector& elements,
                             wxString& inOutOldRequest);

    private:
        CategoryRepository& repo_;
        CWindowMain*        windowMain_;
    };
}
