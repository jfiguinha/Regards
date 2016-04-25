#include "InfosToolbar.h"
#include "ExplorerParam.h"
#include <ConfigParam.h>
#include <libResource.h>
#include <effect.h>
using namespace Regards::Explorer;

CInfosToolbar::CInfosToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface)
	: CToolbarWindow(parent, id ,theme)
{
    saveLastPush = true;
	wxString libelleinfos = CLibResource::LoadStringFromResource(L"LBLINFOS",1);
	wxString libellemap = CLibResource::LoadStringFromResource(L"LBLMAPS",1);
	wxString libelleCriteria = CLibResource::LoadStringFromResource(L"LBLCRITERIA", 1);

	infos = new CToolbarTexte(themeToolbar.texte);
	infos->SetCommandId(WM_INFOS);
	infos->SetLibelle(libelleinfos);
	navElement.push_back(infos);

	map = new CToolbarTexte(themeToolbar.texte);
	map->SetCommandId(WM_MAPS);
	map->SetLibelle(libellemap);
	navElement.push_back(map);

	criteria = new CToolbarTexte(themeToolbar.texte);
	criteria->SetCommandId(WM_CRITERIA);
	criteria->SetLibelle(libelleCriteria);
	navElement.push_back(criteria);

	this->toolbarInterface = toolbarInterface;
}

CInfosToolbar::~CInfosToolbar()
{
}

void CInfosToolbar::SetInfosPush()
{
    infos->SetPush(true);
}


void CInfosToolbar::SetCriteriaPush()
{
    criteria->SetPush(true);
}

void CInfosToolbar::SetMapPush()
{
    map->SetPush(true);
}

void CInfosToolbar::SetInfosActif()
{
	if (width > 0 && height > 0)
	{
		infos->SetActif();
	}
	toolbarInterface->ClickShowButton(WM_INFOS);
}

void CInfosToolbar::SetMapActif()
{
	map->SetVisible(true);
	this->Refresh();
}

void CInfosToolbar::SetMapInactif()
{
	map->SetVisible(false);
	this->Refresh();
}


void CInfosToolbar::OnSize(wxSizeEvent& event)
{
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	int nbElement = (int)navElement.size();
	themeToolbar.texte.SetTailleX(width / nbElement);
	for (CToolbarElement * nav : navElement)
	{
		nav->Resize(themeToolbar.texte.GetTailleX(), themeToolbar.texte.GetTailleY());
	}
	this->Refresh();
}

void CInfosToolbar::EventManager(const int &id)
{
	if (toolbarInterface != nullptr)
	{
		toolbarInterface->ClickShowButton(id);
	}
}




