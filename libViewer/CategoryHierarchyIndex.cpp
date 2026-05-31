#include <header.h>
#include <PhotoCatalog.h>
#include "CategoryHierarchyIndex.h"
#include "CategoryRepository.h"
#include <ThumbnailData.h>
#include <TreeDataCategory.h>
using namespace Regards::Window;
using namespace Regards::Viewer;

// -----------------------------------------------------------------------------
CategoryHierarchyIndex::CategoryHierarchyIndex(CategoryRepository& repo,
                                               const std::vector<wxString>& monthNames)
    : repo_(repo), monthNames_(monthNames)
{
}

// -----------------------------------------------------------------------------
void CategoryHierarchyIndex::Build(int faceDetection)
{
    tr_.clear();
    idElement_ = 0;

    CatalogVector catalogs;
    repo_.GetCatalogs(catalogs);

    auto top = tr_.begin();
    for (CPhotoCatalog& catalog : catalogs)
    {
        auto treeData = new CTreeDataCategory();
        const int numCatalog = catalog.GetNumCatalog();

        treeData->SetNumElement(idElement_);
        treeData->SetIdElement(-1);
        treeData->SetNumParent(0);
        treeData->SetIsParent(true);
        treeData->SetNumLevel(0);
        treeData->SetNumCatalog(numCatalog);
        treeData->SetKey(catalog.GetLibelle());
        treeData->SetExifKey(catalog.GetLibelle());
        treeData->SetTypeElement(TYPECATALOG);

        auto child = tr_.insert(top, treeData);
        treeData->child = child;

        InitCatalogCategorie(child, idElement_, numCatalog, faceDetection);
    }
}

// -----------------------------------------------------------------------------
void CategoryHierarchyIndex::InitCatalogCategorie(tree<CTreeData*>::iterator parent,
                                                   int numParent,
                                                   int numCatalog,
                                                   int faceDetection)
{
    PhotoCategorieVector categories;
    repo_.GetRootCategories(categories, 1);

    for (auto& cat : categories)
    {
        if (cat.GetId() == 4 && !faceDetection) continue;

        idElement_++;
        auto treeData = new CTreeDataCategory();
        treeData->SetNumElement(idElement_);
        treeData->SetNumCatalog(numCatalog);
        treeData->SetIdElement(-1);
        treeData->SetNumCategorie(cat.GetId());
        treeData->SetNumParent(numParent);
        treeData->SetIsParent(false);
        treeData->SetNumLevel(1);
        treeData->SetKey(cat.GetLibelle());
        treeData->SetTypeElement(TYPECATEGORIE);

        auto child = tr_.append_child(parent, treeData);
        treeData->child = child;

        switch (cat.GetId())
        {
        case 2: LoadFolder(numCatalog, child, idElement_); break;
        case 4: LoadFace(numCatalog, child, idElement_);   break;
        default: LoadCategorie(cat.GetId(), child, idElement_, numCatalog); break;
        }
    }
}

// -----------------------------------------------------------------------------
void CategoryHierarchyIndex::LoadFolder(int numCatalog,
                                         tree<CTreeData*>::iterator parent,
                                         int numParent)
{
    FolderCatalogVector folders;
    repo_.GetFolders(folders, numCatalog);

    for (auto& folder : folders)
    {
        idElement_++;
        auto treeData = new CTreeDataCategory();
        treeData->SetNumElement(idElement_);
        treeData->SetIdElement(folder.GetNumFolder());
        treeData->SetNumCatalog(numCatalog);
        treeData->SetNumCategorie(2);
        treeData->SetNumLevel(2);
        treeData->SetNumParent(numParent);
        treeData->SetIsParent(false);
        treeData->SetKey(folder.GetFolderPath());
        treeData->SetTypeElement(TYPEFOLDER);
        auto child = tr_.append_child(parent, treeData);
        treeData->child = child;
    }
}

// -----------------------------------------------------------------------------
void CategoryHierarchyIndex::LoadFace(int numCatalog,
                                       tree<CTreeData*>::iterator parent,
                                       int numParent)
{
    auto faces = repo_.GetFaces();
    for (auto& face : faces)
    {
        idElement_++;
        auto treeData = new CTreeDataCategory();
        treeData->SetNumElement(idElement_);
        treeData->SetIdElement(face.numFace);
        treeData->SetNumCatalog(numCatalog);
        treeData->SetNumCategorie(4);
        treeData->SetNumLevel(2);
        treeData->SetNumParent(numParent);
        treeData->SetIsParent(false);
        treeData->SetKey(face.faceName);
        treeData->SetTypeElement(TYPEFACE);
        auto child = tr_.append_child(parent, treeData);
        treeData->child = child;
    }
}

// -----------------------------------------------------------------------------
void CategoryHierarchyIndex::LoadCategorie(int numCategorie,
                                            tree<CTreeData*>::iterator parent,
                                            int numParent,
                                            int numCatalog)
{
    CriteriaVector criteria;
    repo_.GetCriteria(criteria, numCategorie, numCatalog);

    for (CCriteria& c : criteria)
    {
        if (c.GetLibelle().empty()) continue;
        AddCategorie(numCatalog, numCategorie, parent, numParent, c.GetLibelle(), c.GetId());
    }
}

// -----------------------------------------------------------------------------
tree<CTreeData*>::iterator
CategoryHierarchyIndex::AddCategorie(int numCatalog, int numCategorie,
                                      tree<CTreeData*>::iterator parent,
                                      int numParent,
                                      const wxString& catlibelle, int id)
{
    wchar_t seps[]        = L".";
    wxString exifKey;
    wchar_t  libelle[1024];
    wchar_t* next_token   = nullptr;
    auto     child        = parent;
    int      numParentElt = numParent;
    int      level        = 2;

    wcscpy(libelle, catlibelle.c_str());
#if defined(WIN32) && _MSC_VER < 1900
    wchar_t* token = wcstok(libelle, seps);
#else
    wchar_t* token = wcstok(libelle, seps, &next_token);
#endif

    while (token)
    {
        idElement_++;

        if (!exifKey.empty()) exifKey += L".";
        exifKey += token;

        auto treeData = new CTreeDataCategory();
        treeData->SetIsParent(false);

        // Mois (catégorie 3, niveau 3) : affiche le nom du mois
        if (numCategorie == 3 && level == 3)
        {
            wchar_t* pEnd;
            const long numMonth = wcstol(token, &pEnd, 10);
            if (numMonth >= 1 && numMonth <= 12)
                treeData->SetKey(monthNames_.at(static_cast<size_t>(numMonth - 1)));
            else
                treeData->SetKey(token);
        }
        else
            treeData->SetKey(token);

        treeData->SetNumElement(idElement_);
        treeData->SetTypeElement(TYPECRITERIA);
        treeData->SetNumCatalog(numCatalog);
        treeData->SetNumCategorie(numCategorie);
        treeData->SetNumParent(numParentElt);
        treeData->SetNumLevel(level++);
        treeData->SetIdElement(catlibelle == exifKey ? id : -1);

        auto existing = FindKey(treeData->GetKey(), child);
        if (existing != nullptr)
        {
            child = existing;
            auto* existData = static_cast<CTreeDataCategory*>(*existing);
            numParentElt = existData->GetNumElement();
            delete treeData;
        }
        else
        {
            numParentElt = idElement_;
            treeData->SetExifKey(exifKey);
            auto insertBefore = FindChild(child, exifKey);
            child = (insertBefore == nullptr)
                ? tr_.append_child(child, treeData)
                : tr_.insert(insertBefore, treeData);
            treeData->child = child;
        }

#if defined(WIN32) && _MSC_VER < 1900
        token = wcstok(nullptr, seps);
#else
        token = wcstok(nullptr, seps, &next_token);
#endif
    }
    return child;
}

tree<CTreeData*>::iterator CategoryHierarchyIndex::FindKey(const wxString& key, tree<CTreeData*>::iterator& parent)
{
    auto p = std::find_if(
        tr_.begin(parent), tr_.end(parent),
        [&](const auto& data)
        {
            if (data != nullptr)
                return data->GetKey() == key;
        }
    );

    if (p == tr_.end(parent))
        return nullptr;

    return p;
}

// -----------------------------------------------------------------------------
tree<CTreeData*>::iterator
CategoryHierarchyIndex::FindExifKey(const wxString& exifKey)
{
    for (auto it = tr_.begin(); it != tr_.end(); ++it)
        if ((*it)->GetExifKey() == exifKey) return it;
    return tr_.end();
}

tree<CTreeData*>::iterator
CategoryHierarchyIndex::FindFolderKey(const wxString& path)
{
    for (auto it = tr_.begin(); it != tr_.end(); ++it)
        if ((*it)->GetKey() == path) return it;
    return tr_.end();
}

tree<CTreeData*>::iterator
CategoryHierarchyIndex::FindChild(tree<CTreeData*>::iterator parent,
                                   const wxString& catlibelle)
{
    auto it    = tr_.begin(parent);
    auto itOld = it;
    for (auto i = 0; i < parent.number_of_children(); ++i, ++it)
    {
        if ((*it)->GetExifKey() > catlibelle) return itOld;
        itOld = it;
    }
    return nullptr;
}
