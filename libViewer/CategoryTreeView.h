#pragma once
#include <tree.h>
#include <TreeData.h>
#include <TreeControl.h>


class CThemeTree;

namespace Regards::Window
{
    class CTreeDataCategory;
    class CTreeElementCheckBox;
    class CTreeElementTriangle;
    class CTreeElementTexte;
    class CPositionElement;
}

using namespace Regards::Window;

namespace Regards::Viewer
{
    class CategoryTreeStatePersistence;

    // -------------------------------------------------------------------------
    // CategoryTreeView
    //
    // Responsabilité unique : à partir de tree<CTreeData*> et des thèmes,
    // construire (CreateElement/CreateChildTree) et mettre à jour
    // (UpdateElement/UpdateChildTree) la liste des CPositionElement rendus.
    //
    // Opère sur les vecteurs vectorPosElement et vectorPosElementDynamic
    // fournis par le CTreeControl parent via référence.
    // -------------------------------------------------------------------------
    class CategoryTreeView : public CTreeControl
    {
    public:
        CategoryTreeView(CThemeTree& theme,
                         CategoryTreeStatePersistence& persistence,
                         PositionElementVector& posElements,
                         PositionElementVector& dynElements,
                         std::vector<int>& rowWidth);

        // Création initiale de tous les éléments visuels
        void CreateAll(tree<CTreeData*>& tr);

        // Mise à jour des positions (après resize, scroll, triangle toggle)
        void UpdateAll(tree<CTreeData*>& tr, bool init = false);

        // Accès layout
        int GetWidthPosition() const { return widthPosition_; }

        void UpdateScreenRatio() override {}

        virtual void SlidePosChange(CTreeElement* treeElement, const int& position, CTreeElementValue* value,
            const wxString& key)
        {

        }

    private:
        // ---- Création ----
        void CreateRootRow(tree<CTreeData*>::pre_order_iterator& it,
                           tree<CTreeData*>& tr);
        void CreateChildTree(tree<CTreeData*>::pre_order_iterator& parent,
                             tree<CTreeData*>& tr);
        void CreateChildTree(tree<CTreeData*>::sibling_iterator& parent,
            tree<CTreeData*>& tr);
        void CreateNodeRow(CTreeDataCategory* data, int xPos,
            tree<CTreeData*>::pre_order_iterator& it,
            tree<CTreeData*>& tr);
        void CreateLeafRow(CTreeDataCategory* data, int xPos);
        void CreateGroupRow(CTreeDataCategory* data, int xPos);
        void CreateNodeRow(CTreeDataCategory* data, int xPos,
                           tree<CTreeData*>::sibling_iterator& it,
                           tree<CTreeData*>& tr);
        void UpdateChildTree(tree<CTreeData*>::sibling_iterator& parent,
            tree<CTreeData*>& tr, bool init);
        // ---- Mise à jour ----
        void UpdateRootRow(tree<CTreeData*>::pre_order_iterator& it,
                           tree<CTreeData*>& tr, bool init);
        void UpdateChildTree(tree<CTreeData*>::pre_order_iterator& parent,
                             tree<CTreeData*>& tr, bool init);

        // ---- Helpers visuels (délèguent vers CTreeControl) ----
        // Factorisé : retourne ou crée le CPositionElement pour un type donné
        CPositionElement* GetOrCreateTriangle(CTreeDataCategory* data, int xPos,
                                                               bool& isOpen);
        CPositionElement* GetOrCreateCheckBox(CTreeDataCategory* data, int xPos);
        CPositionElement* GetOrCreateTexte(CTreeDataCategory* data, int xPos);


        CThemeTree theme_;
        CategoryTreeStatePersistence&        persistence_;
        PositionElementVector&      posElements_;
        PositionElementVector&      dynElements_;
        std::vector<int>&                    rowWidth_;

        int yPos_          = 0;
        int nbRow_         = 0;
        int widthPosition_ = 0;
    };
}
