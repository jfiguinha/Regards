#pragma once
#include <TreeData.h>


namespace Regards::Window
{
    class CTreeDataCategory;
}

using namespace Regards::Window;

namespace Regards::Viewer
{
    class CategoryRepository;
    class CategoryTreeStatePersistence;

    // -------------------------------------------------------------------------
    // CategoryHierarchyIndex
    //
    // Responsabilité unique : construire et maintenir la structure
    // tree<CTreeData*> à partir des données du repository.
    // Fournit également les recherches de nœuds (FindExifKey, FindFolderKey,
    // FindChild) utilisées lors de la construction et du rendu.
    // -------------------------------------------------------------------------
    class CategoryHierarchyIndex
    {
    public:
        CategoryHierarchyIndex(CategoryRepository& repo,
                               const std::vector<wxString>& monthNames);

        // Construction complète de l'arbre (appelée par CCategoryWnd::Init)
        // faceDetection : 0 = désactivé, ≠0 = actif
        void Build(int faceDetection);

        // Accès à l'arbre (lecture seule pour les autres classes)
        const tree<CTreeData*>& Tree() const { return tr_; }
        tree<CTreeData*>&       Tree()       { return tr_; }

        // Recherches
        tree<CTreeData*>::iterator FindExifKey(const wxString& exifKey);
        tree<CTreeData*>::iterator FindFolderKey(const wxString& path);
        tree<CTreeData*>::iterator FindChild(tree<CTreeData*>::iterator parent,
                                             const wxString& catlibelle);
        tree<CTreeData*>::iterator FindKey(const wxString& key, tree<CTreeData*>::iterator& parent);

    private:

        void InitCatalogCategorie(tree<CTreeData*>::iterator parent, int numParent,
                                  int numCatalog, int faceDetection);
        void LoadCategorie(int numCategorie, tree<CTreeData*>::iterator parent,
                           int numParent, int numCatalog);
        void LoadFolder(int numCatalog, tree<CTreeData*>::iterator parent, int numParent);
        void LoadFace(int numCatalog, tree<CTreeData*>::iterator parent, int numParent);

        // Insère un nœud hiérarchique à partir d'un libellé "A.B.C"
        // Retourne l'itérateur vers la feuille insérée/réutilisée.
        tree<CTreeData*>::iterator AddCategorie(int numCatalog, int numCategorie,
                                                tree<CTreeData*>::iterator parent,
                                                int numParent,
                                                const wxString& catlibelle, int id);

        CategoryRepository&         repo_;
        const std::vector<wxString>& monthNames_;
        tree<CTreeData*>            tr_;
        int                         idElement_ = 0;
    };
}
