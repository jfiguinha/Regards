#include <header.h>
#include "CategoryQueryService.h"
#include "CategoryRepository.h"
#include "TreeDataCategory.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <TreeElementCheckBox.h>
#include <SqlFindPhotos.h>
#include <LibResource.h>
#include <window_id.h>
#include <WindowMain.h>
using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::Window;
// -----------------------------------------------------------------------------
CategoryQueryService::CategoryQueryService(CategoryRepository& repo,
                                            CWindowMain* windowMain)
    : repo_(repo), windowMain_(windowMain)
{
}

// -----------------------------------------------------------------------------
wxString CategoryQueryService::BuildSqlRequest(
    PositionElementVector& elements) const
{
    vector<int> listFolder;
    vector<int> listFace;
    vector<int> listFolderNotSelected;
    vector<int> listFaceNotSelected;
    vector<int> listFaceSelected;
    vector<int> listCriteriaNotIn;
    vector<int> listStarSelected;
    vector<int> listStarNotSelected;
    vector<int> listKeywordSelected;
    vector<int> listKeywordNotSelected;

    // --- Lecture de l'état de chaque checkbox ---
    for (CPositionElement* value : elements)
    {
        if (!value || value->GetType() != ELEMENT_CHECKBOX) continue;
        auto* checkBox  = static_cast<CTreeElementCheckBox*>(value->GetTreeElement());
        auto* treeData  = static_cast<CTreeDataCategory*>(value->GetTreeData());
        const int cat   = treeData->GetNumCategorie();
        const int eltId = treeData->GetIdElement();

        if (!checkBox->GetCheckState())
        {
            if (cat != 2 && cat != 4 && cat != 6 && cat != 7 && eltId != -1)
                listCriteriaNotIn.push_back(eltId);
            else if (cat == 2) listFolderNotSelected.push_back(eltId);
            else if (cat == 4) listFaceNotSelected.push_back(eltId);
            else if (cat == 6) listStarNotSelected.push_back(eltId);
            else if (cat == 7) listKeywordNotSelected.push_back(eltId);
        }
        else if (eltId != -1)
        {
            if (cat == 4) listFaceSelected.push_back(eltId);
            else if (cat == 6) listStarSelected.push_back(eltId);
            else if (cat == 7) listKeywordSelected.push_back(eltId);
        }
    }

    // --- Dossiers actifs = tous les dossiers moins ceux décochés ---
    FolderCatalogVector folderList;
    repo_.GetFoldersForQuery(folderList, NUMCATALOGID);
    for (CFolderCatalog& f : folderList)
    {
        if (std::find(listFolderNotSelected.begin(),
                      listFolderNotSelected.end(),
                      f.GetNumFolder()) == listFolderNotSelected.end())
            listFolder.push_back(f.GetNumFolder());
    }

    // --- Pertinence ---
    double pertinence = 0.0;
    auto* viewerParam = CMainParamInit::getInstance();
    if (viewerParam) pertinence = viewerParam->GetPertinenceValue();

    const wxString libelleNotGeo = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);

    CSqlFindPhotos sqlFindPhotos;
    const wxString sql = sqlFindPhotos.GenerateSqlRequest(
        NUMCATALOGID, listFolder, listCriteriaNotIn,
        listFaceNotSelected, listFaceSelected,
        listStarSelected, listStarNotSelected,
        listKeywordSelected, listKeywordNotSelected,
        libelleNotGeo, pertinence);

    if (viewerParam) viewerParam->SetLastSqlRequest(sql);
    return sql;
}

// -----------------------------------------------------------------------------
void CategoryQueryService::UpdateAndNotify(PositionElementVector& elements,
                                            wxString& inOutOldRequest)
{
    const wxString newSql = BuildSqlRequest(elements);
    if (inOutOldRequest == newSql) return;

    CSqlFindPhotos sqlFindPhotos;
    sqlFindPhotos.SearchPhotos(newSql);

    auto* viewerParam = CMainParamInit::getInstance();
    if (viewerParam) viewerParam->SetLastSqlRequest(newSql);

    if (windowMain_)
    {
        const wxCommandEvent evt(wxEVENT_REFRESHFOLDERLIST);
        windowMain_->GetEventHandler()->AddPendingEvent(evt);
    }

    inOutOldRequest = newSql;
}
