#include <header.h>
#include "CategoryTreeStatePersistence.h"
#include <TreeElementCheckBox.h>
#include <TreeElementTriangle.h>
#include "TreeDataCategory.h"
#include "ViewerParam.h"
using namespace Regards::Viewer;
using namespace Regards::Window;

CategoryTreeStatePersistence::CategoryTreeStatePersistence(CMainParam* config)
    : config_(config)
{
}

// -----------------------------------------------------------------------------
void CategoryTreeStatePersistence::Load()
{
    if (!config_) return;
    stateValue_         = config_->GetCatalogCriteria();
    stateTriangleValue_ = config_->GetCatalogOpenTriangle();
}

// -----------------------------------------------------------------------------
void CategoryTreeStatePersistence::SaveCheckState(const PositionElementVector& elements)
{
    stateValue_.clear();
    for (CPositionElement* value : elements)
    {
        if (!value || value->GetType() != ELEMENT_CHECKBOX) continue;
        auto* treeData = value->GetTreeData();
        auto* checkBox = dynamic_cast<CTreeElementCheckBox*>(value->GetTreeElement());
        if (!checkBox || !treeData) continue;

        stateValue_ << treeData->GetExifKey()
                    << L":" << treeData->GetKey()
                    << (checkBox->GetCheckState() ? L":1" : L":0")
                    << L";";
    }
    if (config_) config_->SetCatalogCriteria(stateValue_);
}

// -----------------------------------------------------------------------------
void CategoryTreeStatePersistence::SaveTriangleState(const PositionElementVector& elements)
{
    stateTriangleValue_.clear();
    for (CPositionElement* value : elements)
    {
        if (!value || value->GetType() != ELEMENT_TRIANGLE) continue;
        auto* treeData = value->GetTreeData();
        auto* triangle = static_cast<CTreeElementTriangle*>(value->GetTreeElement());
        if (!triangle || !treeData) continue;

        stateTriangleValue_ << treeData->GetExifKey()
                            << L":" << treeData->GetKey()
                            << (triangle->GetOpen() ? L":1" : L":0")
                            << L";";
    }
    if (config_) config_->SetCatalogOpenTriangle(stateTriangleValue_);
}

// -----------------------------------------------------------------------------
bool CategoryTreeStatePersistence::GetCheckState(const wxString& exifKey,
                                                  const wxString& key,
                                                  int numCategorie) const
{
    // Catégories 6 et 7 (étoiles, mots-clés) : non cochées par défaut
    const bool defaultVal = (numCategorie > 5) ? false : true;
    return LookupState(stateValue_, exifKey, key, defaultVal);
}

bool CategoryTreeStatePersistence::GetTriangleState(const wxString& exifKey,
                                                     const wxString& key) const
{
    return LookupState(stateTriangleValue_, exifKey, key, /*defaultOpen=*/true);
}

// -----------------------------------------------------------------------------
/*static*/
bool CategoryTreeStatePersistence::LookupState(const wxString& stateString,
                                                const wxString& exifKey,
                                                const wxString& key,
                                                bool defaultValue)
{
    const wxString localkey = exifKey + L":" + key + L":";
    const size_t pos = stateString.find(localkey);
    if (pos >= stateString.size()) return defaultValue;
    return stateString[pos + localkey.size()] == L'1';
}
