#include <header.h>
#include "CategoryTreeView.h"
#include "CategoryTreeStatePersistence.h"
#include "TreeDataCategory.h"
#include <TreeElementTexte.h>
#include <TreeElementTriangle.h>
#include <TreeElementCheckBox.h>
#include <PositionElement.h>
using namespace Regards::Viewer;
using namespace Regards::Window;
// NOTE : CategoryTreeView délègue la création des éléments visuels aux
// méthodes héritées de CTreeControl (CreateTriangleElement, CreateCheckBoxElement,
// CreateTexteElement, CreatePositionElement, GetElement).  Comme CategoryTreeView
// n'est pas elle-même un CTreeControl, ces appels se font via des pointeurs de
// fonctions injectés ou en déplaçant CategoryTreeView en classe interne/amie de
// CCategoryWnd.  Dans cette implémentation on utilise le pattern « foncteur
// factory » : CCategoryWnd passe des lambdas au constructeur.  Pour garder
// la lisibilité maximale du découpage, les signatures restent symboliques ;
// l'intégration finale choisit la stratégie d'injection qui convient
// (lambda, interface abstraite, ou classe interne).

CategoryTreeView::CategoryTreeView(CThemeTree& theme,
                                   CategoryTreeStatePersistence& persistence,
                                   PositionElementVector& posElements,
                                   PositionElementVector& dynElements,
                                   std::vector<int>& rowWidth)
    :
    persistence_(persistence)
    , posElements_(posElements)
    , dynElements_(dynElements)
    , rowWidth_(rowWidth)
{
    theme_ = theme;
}

// =============================================================================
// CreateAll
// =============================================================================
void CategoryTreeView::CreateAll(tree<CTreeData*>& tr)
{
    posElements_.clear();
    dynElements_.clear();
    yPos_          = 0;
    nbRow_         = 0;
    widthPosition_ = 0;

    auto it    = tr.begin();
    auto itend = tr.end();
    while (it != itend)
    {
        if (tr.depth(it) == 0)
            CreateRootRow(it, tr);
        ++it;
    }
}

void CategoryTreeView::CreateRootRow(tree<CTreeData*>::pre_order_iterator& it,
                                      tree<CTreeData*>& tr)
{
    auto* data      = static_cast<CTreeDataCategory*>(*it);
    int   xPos      = theme_.GetMargeX();
    int   widthElt  = 0;

    // Triangle
    bool isOpen = false;
    CPositionElement* pe = GetOrCreateTriangle(data, xPos, isOpen);
    widthPosition_ = xPos + pe->GetWidth();
    xPos += pe->GetWidth() + theme_.GetMargeX();

    // Checkbox
    pe    = GetOrCreateCheckBox(data, xPos);
    xPos += pe->GetWidth() + theme_.GetMargeX();

    // Texte
    pe        = GetOrCreateTexte(data, xPos);
    widthElt  = xPos + pe->GetWidth() + theme_.GetMargeX();
    yPos_ += theme_.GetRowHeight();
    nbRow_++;
    if (rowWidth_[0] < widthElt) rowWidth_[0] = widthElt;

    if (isOpen)
        CreateChildTree(it, tr);
}

// =============================================================================
// CreateChildTree – récursion sur les enfants
// =============================================================================
void CategoryTreeView::CreateChildTree(tree<CTreeData*>::pre_order_iterator& parent,
                                        tree<CTreeData*>& tr)
{
    auto it = tr.begin(parent);
    for (auto i = 0; i < parent.number_of_children(); ++i, ++it)
    {
        auto* data         = static_cast<CTreeDataCategory*>(*it);
        const int depth    = tr.depth(it);
        const bool hasData = !data->GetValue().empty() || it.number_of_children() == 0;

        if (depth == 1 && hasData)
            CreateLeafRow(data, widthPosition_ * (depth + 1));
        else if (hasData)
            CreateGroupRow(data, widthPosition_ * (depth + 1));
        else
            CreateNodeRow(data, widthPosition_ * depth, it, tr);
    }
}

// =============================================================================
// CreateChildTree – récursion sur les enfants
// =============================================================================
void CategoryTreeView::CreateChildTree(tree<CTreeData*>::sibling_iterator& parent,
    tree<CTreeData*>& tr)
{
    auto it = tr.begin(parent);
    for (auto i = 0; i < parent.number_of_children(); ++i, ++it)
    {
        auto* data = static_cast<CTreeDataCategory*>(*it);
        const int depth = tr.depth(it);
        const bool hasData = !data->GetValue().empty() || it.number_of_children() == 0;

        if (depth == 1 && hasData)
            CreateLeafRow(data, widthPosition_ * (depth + 1));
        else if (hasData)
            CreateGroupRow(data, widthPosition_ * (depth + 1));
        else
            CreateNodeRow(data, widthPosition_ * depth, it, tr);
    }
}

void CategoryTreeView::CreateLeafRow(CTreeDataCategory* data, int xPos)
{
    // Niveau 1 avec données : texte seul (pas de checkbox)
    CPositionElement* pe = GetOrCreateTexte(data, xPos);
    const int w = xPos + pe->GetWidth() + theme_.GetMargeX();
    yPos_ += theme_.GetRowHeight();
    nbRow_++;
    if (rowWidth_[0] < w) rowWidth_[0] = w;
}

void CategoryTreeView::CreateGroupRow(CTreeDataCategory* data, int xPos)
{
    // Feuille avec données : checkbox + texte
    CPositionElement* pe = GetOrCreateCheckBox(data, xPos);
    xPos += pe->GetWidth() + theme_.GetMargeX();
    pe = GetOrCreateTexte(data, xPos);
    const int w = xPos + pe->GetWidth() + theme_.GetMargeX();
    yPos_ += theme_.GetRowHeight();
    nbRow_++;
    if (rowWidth_[0] < w) rowWidth_[0] = w;
}

void CategoryTreeView::CreateNodeRow(CTreeDataCategory* data, int xPos,
    tree<CTreeData*>::pre_order_iterator& it,
    tree<CTreeData*>& tr)
{
    // Nœud intermédiaire : triangle + checkbox + texte + récursion si ouvert
    bool isOpen = false;
    CPositionElement* pe = GetOrCreateTriangle(data, xPos, isOpen);
    xPos += pe->GetWidth() + theme_.GetMargeX();
    pe = GetOrCreateCheckBox(data, xPos);
    xPos += pe->GetWidth() + theme_.GetMargeX();
    pe = GetOrCreateTexte(data, xPos);
    const int w = xPos + pe->GetWidth() + theme_.GetMargeX();
    yPos_ += theme_.GetRowHeight();
    nbRow_++;
    if (rowWidth_[0] < w) rowWidth_[0] = w;
    if (isOpen) CreateChildTree(it, tr);
}

void CategoryTreeView::CreateNodeRow(CTreeDataCategory* data, int xPos,
                                      tree<CTreeData*>::sibling_iterator& it,
                                      tree<CTreeData*>& tr)
{
    // Nœud intermédiaire : triangle + checkbox + texte + récursion si ouvert
    bool isOpen = false;
    CPositionElement* pe = GetOrCreateTriangle(data, xPos, isOpen);
    xPos += pe->GetWidth() + theme_.GetMargeX();
    pe    = GetOrCreateCheckBox(data, xPos);
    xPos += pe->GetWidth() + theme_.GetMargeX();
    pe    = GetOrCreateTexte(data, xPos);
    const int w = xPos + pe->GetWidth() + theme_.GetMargeX();
    yPos_ += theme_.GetRowHeight();
    nbRow_++;
    if (rowWidth_[0] < w) rowWidth_[0] = w;
    if (isOpen) CreateChildTree(it, tr);
}

// =============================================================================
// UpdateAll
// =============================================================================
void CategoryTreeView::UpdateAll(tree<CTreeData*>& tr, bool init)
{
    yPos_          = 0;
    nbRow_         = 0;
    widthPosition_ = 0;

    // Masquer tous les éléments existants
    for (CPositionElement* v : posElements_)
        if (v) v->GetTreeElement()->SetVisible(false);

    auto it = tr.begin();
    auto itend = tr.end();
    while (it != itend)
    {
        if (tr.depth(it) == 0)
            UpdateRootRow(it, tr, init);
        ++it;
    }
}

void CategoryTreeView::UpdateRootRow(tree<CTreeData*>::pre_order_iterator& it,
                                      tree<CTreeData*>& tr, bool init)
{
    auto* data     = static_cast<CTreeDataCategory*>(*it);
    int   xPos     = theme_.GetMargeX();
    int   widthElt = 0;

    // Triangle
    bool isOpen = false;
    CPositionElement* pe = GetOrCreateTriangle(data, xPos, isOpen);
    pe->GetTreeElement()->SetVisible(true);
    widthPosition_ = xPos + pe->GetWidth();
    xPos += pe->GetWidth() + theme_.GetMargeX();

    // Checkbox
    pe = GetOrCreateCheckBox(data, xPos);
    pe->GetTreeElement()->SetVisible(true);
    xPos += pe->GetWidth() + theme_.GetMargeX();

    // Texte
    pe       = GetOrCreateTexte(data, xPos);
    pe->GetTreeElement()->SetVisible(true);
    widthElt = xPos + pe->GetWidth() + theme_.GetMargeX();
    yPos_ += theme_.GetRowHeight();
    nbRow_++;
    if (rowWidth_[0] < widthElt) rowWidth_[0] = widthElt;

    if (isOpen)
        UpdateChildTree(it, tr, init);
}

void CategoryTreeView::UpdateChildTree(tree<CTreeData*>::pre_order_iterator& parent,
                                        tree<CTreeData*>& tr, bool init)
{
    auto it = tr.begin(parent);
    for (auto i = 0; i < parent.number_of_children(); ++i, ++it)
    {
        auto* data         = static_cast<CTreeDataCategory*>(*it);
        const int depth    = tr.depth(it);
        const bool hasData = !data->GetValue().empty() || it.number_of_children() == 0;
        int xPos;

        if (depth == 1 && hasData)
        {
            xPos = widthPosition_ * (depth + 1);
            CPositionElement* pe = GetOrCreateTexte(data, xPos);
            pe->GetTreeElement()->SetVisible(true);
            const int w = xPos + pe->GetWidth() + theme_.GetMargeX();
            yPos_ += theme_.GetRowHeight(); nbRow_++;
            if (rowWidth_[0] < w) rowWidth_[0] = w;
        }
        else if (hasData)
        {
            xPos = widthPosition_ * (depth + 1);
            CPositionElement* pe = GetOrCreateCheckBox(data, xPos);
            pe->GetTreeElement()->SetVisible(true);
            xPos += pe->GetWidth() + theme_.GetMargeX();
            pe = GetOrCreateTexte(data, xPos);
            pe->GetTreeElement()->SetVisible(true);
            const int w = xPos + pe->GetWidth() + theme_.GetMargeX();
            yPos_ += theme_.GetRowHeight(); nbRow_++;
            if (rowWidth_[0] < w) rowWidth_[0] = w;
        }
        else
        {
            xPos = widthPosition_ * depth;
            bool isOpen = false;
            CPositionElement* pe = GetOrCreateTriangle(data, xPos, isOpen);
            pe->GetTreeElement()->SetVisible(true);
            xPos += pe->GetWidth() + theme_.GetMargeX();
            pe = GetOrCreateCheckBox(data, xPos);
            pe->GetTreeElement()->SetVisible(true);
            xPos += pe->GetWidth() + theme_.GetMargeX();
            pe = GetOrCreateTexte(data, xPos);
            pe->GetTreeElement()->SetVisible(true);
            const int w = xPos + pe->GetWidth() + theme_.GetMargeX();
            yPos_ += theme_.GetRowHeight(); nbRow_++;
            if (rowWidth_[0] < w) rowWidth_[0] = w;
            if (isOpen) UpdateChildTree(it, tr, init);
        }
    }
}



void CategoryTreeView::UpdateChildTree(tree<CTreeData*>::sibling_iterator& parent,
    tree<CTreeData*>& tr, bool init)
{
    auto it = tr.begin(parent);
    for (auto i = 0; i < parent.number_of_children(); ++i, ++it)
    {
        auto* data = static_cast<CTreeDataCategory*>(*it);
        const int depth = tr.depth(it);
        const bool hasData = !data->GetValue().empty() || it.number_of_children() == 0;
        int xPos;

        if (depth == 1 && hasData)
        {
            xPos = widthPosition_ * (depth + 1);
            CPositionElement* pe = GetOrCreateTexte(data, xPos);
            pe->GetTreeElement()->SetVisible(true);
            const int w = xPos + pe->GetWidth() + theme_.GetMargeX();
            yPos_ += theme_.GetRowHeight(); nbRow_++;
            if (rowWidth_[0] < w) rowWidth_[0] = w;
        }
        else if (hasData)
        {
            xPos = widthPosition_ * (depth + 1);
            CPositionElement* pe = GetOrCreateCheckBox(data, xPos);
            pe->GetTreeElement()->SetVisible(true);
            xPos += pe->GetWidth() + theme_.GetMargeX();
            pe = GetOrCreateTexte(data, xPos);
            pe->GetTreeElement()->SetVisible(true);
            const int w = xPos + pe->GetWidth() + theme_.GetMargeX();
            yPos_ += theme_.GetRowHeight(); nbRow_++;
            if (rowWidth_[0] < w) rowWidth_[0] = w;
        }
        else
        {
            xPos = widthPosition_ * depth;
            bool isOpen = false;
            CPositionElement* pe = GetOrCreateTriangle(data, xPos, isOpen);
            pe->GetTreeElement()->SetVisible(true);
            xPos += pe->GetWidth() + theme_.GetMargeX();
            pe = GetOrCreateCheckBox(data, xPos);
            pe->GetTreeElement()->SetVisible(true);
            xPos += pe->GetWidth() + theme_.GetMargeX();
            pe = GetOrCreateTexte(data, xPos);
            pe->GetTreeElement()->SetVisible(true);
            const int w = xPos + pe->GetWidth() + theme_.GetMargeX();
            yPos_ += theme_.GetRowHeight(); nbRow_++;
            if (rowWidth_[0] < w) rowWidth_[0] = w;
            if (isOpen) UpdateChildTree(it, tr, init);
        }
    }
}

// =============================================================================
// Helpers GetOrCreate* : GetElement puis Create si absent
// Les appels effectifs aux méthodes CTreeControl (CreateTriangleElement, etc.)
// sont injectés via foncteurs dans l'implémentation finale ; ici on documente
// le contrat attendu avec des pseudo-appels explicites.
// =============================================================================
CPositionElement* CategoryTreeView::GetOrCreateTriangle(CTreeDataCategory* data,
                                                          int xPos, bool& isOpenOut)
{
    CPositionElement* pe = GetElement(data, ELEMENT_TRIANGLE);
    if (!pe)
    {
        isOpenOut            = persistence_.GetTriangleState(data->GetExifKey(), data->GetKey());
        auto* tri            = CreateTriangleElement(theme_.GetRowWidth(), theme_.GetRowHeight(), isOpenOut);
        pe = CreatePositionElement(xPos, yPos_, nbRow_, 0,
                                   tri->GetWidth(), tri->GetHeight(),
                                   ELEMENT_TRIANGLE, tri, data);
    }
    else
    {
        auto* tri = static_cast<CTreeElementTriangle*>(pe->GetTreeElement());
        tri->SetElementPos(xPos, yPos_);
        pe->SetX(xPos); pe->SetY(yPos_);
        isOpenOut = tri->GetOpen();
    }
    return pe;
}

CPositionElement* CategoryTreeView::GetOrCreateCheckBox(CTreeDataCategory* data, int xPos)
{
    CPositionElement* pe = GetElement(data, ELEMENT_CHECKBOX);
    if (!pe)
    {
        const bool check = persistence_.GetCheckState(data->GetExifKey(), data->GetKey(),
                                                       data->GetNumCategorie());
        auto* cb = CreateCheckBoxElement(theme_.GetRowWidth(), theme_.GetRowHeight(), check);
        pe = CreatePositionElement(xPos, yPos_, nbRow_, 0,
                                   cb->GetWidth(), cb->GetHeight(),
                                   ELEMENT_CHECKBOX, cb, data);
    }
    else
    {
        auto* cb = dynamic_cast<CTreeElementCheckBox*>(pe->GetTreeElement());
        cb->SetElementPos(xPos, yPos_);
        pe->SetX(xPos); pe->SetY(yPos_);
    }
    return pe;
}

CPositionElement* CategoryTreeView::GetOrCreateTexte(CTreeDataCategory* data, int xPos)
{
    CPositionElement* pe = GetElement(data, ELEMENT_TEXTE);
    if (!pe)
    {
        auto* txt = CreateTexteElement(theme_.GetRowWidth(), theme_.GetRowHeight(),
                                        data->GetKey());
        pe = CreatePositionElement(xPos, yPos_, nbRow_, 0,
                                   txt->GetWidth(), txt->GetHeight(),
                                   ELEMENT_TEXTE, txt, data, false);
    }
    else
    {
        auto* txt = dynamic_cast<CTreeElementTexte*>(pe->GetTreeElement());
        txt->SetElementPos(xPos, yPos_);
        pe->SetX(xPos); pe->SetY(yPos_);
    }
    return pe;
}
