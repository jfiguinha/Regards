#pragma once
#include <wx/string.h>
#include <vector>
#include <SqlFindFacePhoto.h>   // CFaceName
#include <SqlFindFolderCatalog.h> // CFolderCatalog, FolderCatalogVector
#include <SqlPhotoCategorie.h>  // CPhotoCategorie, PhotoCategorieVector
#include <SqlCatalog.h>         // CPhotoCatalog, CatalogVector
#include <Criteria.h>           // CCriteria, CriteriaVector
#include <PhotoCatalog.h>

namespace Regards::Viewer
{
    // -------------------------------------------------------------------------
    // CategoryRepository
    //
    // Responsabilité unique : encapsuler tous les accès base de données
    // nécessaires à la construction de l'arbre de catégories.
    // Aucune logique d'arbre ici, uniquement du chargement.
    // -------------------------------------------------------------------------
    class CategoryRepository
    {
    public:
        // Catalogues
        void GetCatalogs(CatalogVector& out) const;
        wxString GetCatalogLibelle(int numCatalog) const;

        // Catégories principales (niveau 1)
        void GetRootCategories(PhotoCategorieVector& out, int level = 1) const;

        // Critères d'une catégorie (ex : dates, lieux, mots-clés)
        void GetCriteria(CriteriaVector& out, int numCategorie, int numCatalog) const;

        // Dossiers d'un catalogue
        void GetFolders(FolderCatalogVector& out, int numCatalog) const;

        // Visages reconnus sélectionnables
        std::vector<CFaceName> GetFaces() const;

        // Dossiers pour la requête SQL finale (par catalogue id)
        void GetFoldersForQuery(FolderCatalogVector& out, int numCatalog) const;
    };
}
