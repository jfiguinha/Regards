#include "ToolbarInfos.h"
#include <LibResource.h>
#include <effect.h>
using namespace Regards::Window;
using namespace Regards::Viewer;

CToolbarInfos::CToolbarInfos(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface)
	: CToolbarWindow(parent, id, theme)
{
	this->toolbarInterface = toolbarInterface;
    saveLastPush = true;
	wxString infos_label = CLibResource::LoadStringFromResource(L"LBLINFOS",1);// L"Infos";
	wxString history_label = CLibResource::LoadStringFromResource(L"LBLHISTORY", 1);//L"History";
	wxString effect_label = CLibResource::LoadStringFromResource(L"LBLEFFECT", 1);//L"Effect";
	wxString maps_label = CLibResource::LoadStringFromResource(L"LBLMAPS", 1);//L"Maps";
	wxString effectParameter_label = CLibResource::LoadStringFromResource(L"LBLEFFECTPARAMETER", 1);//L"Effect Parameter";

	infos = new CToolbarTexte(themeToolbar.texte);
	infos->SetCommandId(WM_INFOS);
	infos->SetLibelle(infos_label);
	navElement.push_back(infos);

	history = new CToolbarTexte(themeToolbar.texte);
	history->SetCommandId(WM_HISTORY);
	history->SetLibelle(history_label);
	navElement.push_back(history);

	effect = new CToolbarTexte(themeToolbar.texte);
	effect->SetCommandId(WM_EFFECT);
	effect->SetLibelle(effect_label);
	navElement.push_back(effect);

	map = new CToolbarTexte(themeToolbar.texte);
	map->SetCommandId(WM_MAPS);
	map->SetLibelle(maps_label);
	navElement.push_back(map);
    
#ifdef VIEWER
    wxString libelleCriteria = CLibResource::LoadStringFromResource(L"LBLCRITERIA", 1);
    criteria = new CToolbarTexte(themeToolbar.texte);
    criteria->SetCommandId(WM_CRITERIA);
    criteria->SetLibelle(libelleCriteria);
    navElement.push_back(criteria);
#endif
	effectParameter = new CToolbarTexte(themeToolbar.texte);
	effectParameter->SetCommandId(WM_EFFECTPARAMETER);
	effectParameter->SetLibelle(effectParameter_label);
	navElement.push_back(effectParameter);

}

CToolbarInfos::~CToolbarInfos()
{
}

void CToolbarInfos::SetInfosPush()
{
    infos->SetPush(true);
}

void CToolbarInfos::SetMapPush()
{
    map->SetPush(true);
}

void CToolbarInfos::SetEffectPush()
{
    effect->SetPush(true);
}

void CToolbarInfos::SetHistoryPush()
{
    history->SetPush(true);
}

void CToolbarInfos::SetEffectParameterPush()
{
    effectParameter->SetPush(true);
}


void CToolbarInfos::SetInfosActif()
{
	//infos->SetActif();
    infos->SetVisible(true);
	toolbarInterface->ClickShowButton(WM_INFOS);
}

void CToolbarInfos::SetMapActif()
{
	map->SetVisible(true);
	this->Refresh();
}

void CToolbarInfos::SetMapInactif()
{
	map->SetVisible(false);
	this->Refresh();
}

void CToolbarInfos::SetEffectActif()
{
	effect->SetVisible(true);
	this->Refresh();
}
#ifdef VIEWER
void CToolbarInfos::SetCriteriaPush()
{
    criteria->SetPush(true);
}
#endif

void CToolbarInfos::SetEffectInactif()
{
	effect->SetVisible(false);
	this->Refresh();
}

void CToolbarInfos::SetEffectParameterInactif()
{
	effectParameter->SetVisible(false);
	this->Refresh();
}

void CToolbarInfos::SetEffectParameterActif(const wxString &libelle)
{
	
	effectParameter->SetVisible(true);
	effectParameter->SetLibelle(libelle);
	effectParameter->SetLibelleTooltip(libelle);
	if (navPush != nullptr)
		navPush->SetInactif();
	effectParameter->SetActif();
	this->Refresh();
}

void CToolbarInfos::SetVideoToolbar()
{
	history->SetVisible(false);
	map->SetVisible(false);
	effectParameter->SetVisible(false);
    effect->SetVisible(false);
	this->Refresh();
}

void CToolbarInfos::SetPictureThumbnailToolbar()
{
    effect->SetVisible(false);
    history->SetVisible(false);
    effectParameter->SetVisible(false);
    this->Refresh();
}


void CToolbarInfos::SetPictureToolbar()
{
    effect->SetVisible(true);
	history->SetVisible(true);
	effectParameter->SetVisible(false);
	this->Refresh();
}

void CToolbarInfos::OnSize(wxSizeEvent& event)
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

void CToolbarInfos::EventManager(const int &id)
{
	if (toolbarInterface != nullptr)
	{
		toolbarInterface->ClickShowButton(id);
	}
}




