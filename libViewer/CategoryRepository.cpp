#include <header.h>
#include "CategoryRepository.h"
#include <SqlFindCatalog.h>
#include <SqlCatalog.h>
#include <SqlPhotoCategorie.h>
#include <SqlFindCriteria.h>
#include <SqlFindFolderCatalog.h>
#include <SqlFindFacePhoto.h>

using namespace Regards::Viewer;
using namespace Regards::Sqlite;

// -----------------------------------------------------------------------------
void CategoryRepository::GetCatalogs(CatalogVector& out) const
{
    CSqlFindCatalog sql;
    sql.GetCatalog(&out);
}

wxString CategoryRepository::GetCatalogLibelle(int numCatalog) const
{
    CSqlCatalog sql;
    return sql.GetCatalogLibelle(numCatalog);
}

// -----------------------------------------------------------------------------
void CategoryRepository::GetRootCategories(PhotoCategorieVector& out, int level) const
{
    CSqlPhotoCategorie sql;
    sql.LoadPhotoCategorie(&out, level);
}

// -----------------------------------------------------------------------------
void CategoryRepository::GetCriteria(CriteriaVector& out, int numCategorie, int numCatalog) const
{
    CSqlFindCriteria sql;
    sql.SearchCriteria(&out, numCategorie, numCatalog);
}

// -----------------------------------------------------------------------------
void CategoryRepository::GetFolders(FolderCatalogVector& out, int numCatalog) const
{
    CSqlFindFolderCatalog sql;
    sql.GetFolderCatalog(&out, numCatalog);
}

void CategoryRepository::GetFoldersForQuery(FolderCatalogVector& out, int numCatalog) const
{
    GetFolders(out, numCatalog);
}

// -----------------------------------------------------------------------------
std::vector<CFaceName> CategoryRepository::GetFaces() const
{
    CSqlFindFacePhoto sql;
    return sql.GetListFaceNameSelectable();
}
