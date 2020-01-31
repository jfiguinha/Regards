#include <header.h>
#include "ToolbarPhoto.h"
#include <LibResource.h>
using namespace Regards::Window;
using namespace Regards::Viewer;


CToolbarPhoto::CToolbarPhoto(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	folder = nullptr;
    criteria = nullptr;
	this->toolbarInterface = toolbarInterface;
    saveLastPush = true;
	wxString folder_label = CLibResource::LoadStringFromResource(L"LBLFOLDER",1);// L"Folder";
	wxString criteria_label = CLibResource::LoadStringFromResource(L"LBLCRITERIA", 1);//L"Criteria";

	folder = new CToolbarTexte(themeToolbar.texte);
	folder->SetCommandId(WM_FOLDER);
	folder->SetLibelle(folder_label);
	navElement.push_back(folder);

	criteria = new CToolbarTexte(themeToolbar.texte);
	criteria->SetCommandId(WM_CRITERIA);
	criteria->SetLibelle(criteria_label);
	navElement.push_back(criteria);

}

CToolbarPhoto::~CToolbarPhoto()
{
}

void CToolbarPhoto::SetFolderPush()
{
    folder->SetPush(true);
}

void CToolbarPhoto::SetCriteriaPush()
{
    criteria->SetPush(true);
}

void CToolbarPhoto::SetFaceListPush()
{
    facelist->SetPush(true);
}

void CToolbarPhoto::Resize()
{
	int nbElement = (int)navElement.size();
	themeToolbar.texte.SetTailleX(GetWindowWidth() / nbElement);

	for (CToolbarElement * nav : navElement)
	{
		nav->Resize(themeToolbar.texte.GetTailleX(), themeToolbar.texte.GetTailleY());
	}
	Refresh();
	Update();
}

void CToolbarPhoto::EventManager(const int &id)
{
	if (toolbarInterface != nullptr)
	{
		toolbarInterface->ClickShowButton(id);
	}
}




