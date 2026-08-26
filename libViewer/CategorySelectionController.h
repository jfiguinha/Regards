#pragma once
#include <PositionElement.h>

namespace Regards::Window
{
    class CTreeDataCategory;
    class CTreeElementCheckBox;
}

using namespace Regards::Window;

namespace Regards::Viewer
{
    // -------------------------------------------------------------------------
    // CategorySelectionController
    //
    // Responsabilité unique : propager les changements de sélection d'une
    // checkbox vers ses enfants et remonter vers ses parents.
    // Opère exclusivement sur vectorPosElementDynamic fourni par la vue.
    // -------------------------------------------------------------------------
    class CategorySelectionController
    {
    public:
        // Applique le clic sur une checkbox et propage la sélection.
        // elements : vectorPosElementDynamic de la vue (modifié en place).
        void OnCheckBoxClicked(CPositionElement* element,
            PositionElementVector& elements);

    private:
        // Propage vers les enfants (récursif)
        void PropagateToChildren(int numParent, bool check,
            PositionElementVector& elements);

        // Propage vers les parents (remonte l'arbre)
        void PropagateToParents(CTreeDataCategory* treeData, bool checkOn,
                                PositionElementVector& elements);

        // Vérifie l'état des enfants d'un parent donné.
        // Retourne le CPositionElement du parent, et indique via isAllSameState
        // si tous les enfants ont l'état checkOn.
        CPositionElement* CheckChildrenState(int numParent, bool& isAllSameState,
                                              bool checkOn,
                                              const PositionElementVector& elements);
    };
}
