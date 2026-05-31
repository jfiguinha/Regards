#include <header.h>
#include "CategorySelectionController.h"
#include "TreeDataCategory.h"
#include <PositionElement.h>
#include <TreeElementCheckBox.h>
using namespace Regards::Window;
using namespace Regards::Viewer;

// -----------------------------------------------------------------------------
void CategorySelectionController::OnCheckBoxClicked(
    CPositionElement* element,
    PositionElementVector& elements)
{
    auto* treeData = static_cast<CTreeDataCategory*>(element->GetTreeData());
    auto* checkBox = dynamic_cast<CTreeElementCheckBox*>(element->GetTreeElement());

    const bool newState = !checkBox->GetCheckState();
    checkBox->SetCheckState(newState);

    if (treeData->IsParent())
    {
        // Catalogue-racine coché/décoché : tout basculer
        for (CPositionElement* v : elements)
        {
            if (v && v->GetType() == ELEMENT_CHECKBOX)
                static_cast<CTreeElementCheckBox*>(v->GetTreeElement())
                    ->SetCheckState(newState);
        }
    }
    else
    {
        // Propagation vers les enfants
        PropagateToChildren(treeData->GetNumElement(), newState, elements);
        // Propagation vers les parents
        PropagateToParents(treeData, newState, elements);
    }
}

// -----------------------------------------------------------------------------
void CategorySelectionController::PropagateToChildren(
    int numParent, bool check,
    PositionElementVector& elements)
{
    for (CPositionElement* v : elements)
    {
        if (!v || v->GetType() != ELEMENT_CHECKBOX) continue;
        auto* td = static_cast<CTreeDataCategory*>(v->GetTreeData());
        if (td->GetNumParent() == numParent)
        {
            auto* cb = dynamic_cast<CTreeElementCheckBox*>(v->GetTreeElement());
            cb->SetCheckState(check);
            PropagateToChildren(td->GetNumElement(), check, elements);
        }
    }
}

// -----------------------------------------------------------------------------
void CategorySelectionController::PropagateToParents(
    CTreeDataCategory* treeData, bool checkOn,
    PositionElementVector& elements)
{
    int numParent = treeData->GetNumParent();
    bool allSame  = false;

    do
    {
        CPositionElement* parentElt = CheckChildrenState(numParent, allSame, checkOn, elements);
        if (!allSame || !parentElt) break;

        auto* parentData = static_cast<CTreeDataCategory*>(parentElt->GetTreeData());
        if (!parentData) break;

        if (parentData->GetIdElement() != -1)
        {
            auto* cb = static_cast<CTreeElementCheckBox*>(parentElt->GetTreeElement());
            cb->SetCheckState(checkOn);
            numParent = parentData->GetNumParent();
        }
        else
        {
            // Parent intermédiaire sans idElement : chercher par numéro
            for (CPositionElement* v : elements)
            {
                if (!v || v->GetType() != ELEMENT_CHECKBOX) continue;
                auto* td = static_cast<CTreeDataCategory*>(v->GetTreeData());
                if (td->GetNumElement() == numParent)
                {
                    auto* cb = dynamic_cast<CTreeElementCheckBox*>(v->GetTreeElement());
                    if (cb) cb->SetCheckState(checkOn);
                    numParent = td->GetNumParent();
                    break;
                }
            }
        }
    }
    while (allSame && numParent > 0);

    // Cas particulier : si checkOn et le parent direct n'est pas encore coché
    if (checkOn)
    {
        CPositionElement* parentElt = CheckChildrenState(numParent, allSame, checkOn, elements);
        if (parentElt)
        {
            auto* cb = dynamic_cast<CTreeElementCheckBox*>(parentElt->GetTreeElement());
            if (cb && !cb->GetCheckState())
                cb->SetCheckState(true);
        }
    }
}

// -----------------------------------------------------------------------------
CPositionElement* CategorySelectionController::CheckChildrenState(
    int numParent, bool& isAllSameState, bool checkOn,
    const PositionElementVector& elements)
{
    CPositionElement* parentElt = nullptr;
    isAllSameState = true;

    for (CPositionElement* v : elements)
    {
        if (!v || v->GetType() != ELEMENT_CHECKBOX) continue;
        auto* td = static_cast<CTreeDataCategory*>(v->GetTreeData());

        if (td->GetNumElement() == numParent)
            parentElt = v;
        else if (td->GetNumParent() == numParent)
        {
            auto* cb = dynamic_cast<CTreeElementCheckBox*>(v->GetTreeElement());
            if (cb->GetCheckState() == !checkOn)
            {
                isAllSameState = false;
                return parentElt;
            }
        }
    }
    return parentElt;
}
