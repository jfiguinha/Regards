#include <header.h>
#include "ToolbarInfos.h"
#include <LibResource.h>
using namespace Regards::Window;
using namespace Regards::Viewer;


CToolbarInfos::CToolbarInfos(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	infos = nullptr;
	this->toolbarInterface = toolbarInterface;
    saveLastPush = true;
	wxString infos_label = CLibResource::LoadStringFromResource(L"LBLINFOS",1);// L"Infos";

	infos = new CToolbarTexte(themeToolbar.texte);
	infos->SetCommandId(WM_INFOS);
	infos->SetLibelle(infos_label);
	navElement.push_back(infos);


}

CToolbarInfos::~CToolbarInfos()
{
}

void CToolbarInfos::SetInfosPush()
{
    infos->SetPush(true);
}

void CToolbarInfos::SetInfosActif()
{
	//infos->SetActif();
    infos->SetVisible(true);
	toolbarInterface->ClickShowButton(WM_INFOS);
}

void CToolbarInfos::Resize()
{
	int nbElement = (int)navElement.size();
	themeToolbar.texte.SetTailleX(GetWindowWidth() / nbElement);

	for (CToolbarElement * nav : navElement)
	{
		nav->Resize(themeToolbar.texte.GetTailleX(), themeToolbar.texte.GetTailleY());
	}
	Refresh();
}

void CToolbarInfos::EventManager(const int &id)
{
	if (toolbarInterface != nullptr)
	{
		toolbarInterface->ClickShowButton(id);
	}
}




