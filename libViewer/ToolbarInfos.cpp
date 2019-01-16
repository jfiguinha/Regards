#include "ToolbarInfos.h"
#include <LibResource.h>
using namespace Regards::Window;
using namespace Regards::Viewer;


CToolbarInfos::CToolbarInfos(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface)
	: CToolbarWindow(parent, id, theme)
{
	isVideo = false;
	infos = nullptr;
	history = nullptr;
	effect = nullptr;
	map = nullptr;
	effectParameter = nullptr;
    criteria = nullptr;
	this->toolbarInterface = toolbarInterface;
    saveLastPush = true;
	wxString infos_label = CLibResource::LoadStringFromResource(L"LBLINFOS",1);// L"Infos";
	wxString history_label = CLibResource::LoadStringFromResource(L"LBLHISTORY", 1);//L"History";
	wxString effect_label = CLibResource::LoadStringFromResource(L"LBLEFFECT", 1);//L"Effect";
	wxString maps_label = CLibResource::LoadStringFromResource(L"LBLMAPS", 1);//L"Maps";
	wxString audiovideo_label = CLibResource::LoadStringFromResource(L"LBLAUDIOVIDEO", 1);//L"Effect Parameter";
	wxString videoeffect_label = CLibResource::LoadStringFromResource(L"LBLVIDEOEFFECT", 1);//L"Effect Parameter";
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

	audiovideo = new CToolbarTexte(themeToolbar.texte);
	audiovideo->SetCommandId(WM_AUDIOVIDEO);
	audiovideo->SetLibelle(audiovideo_label);
	navElement.push_back(audiovideo);

	videoeffect = new CToolbarTexte(themeToolbar.texte);
	videoeffect->SetCommandId(WM_VIDEOEFFECT);
	videoeffect->SetLibelle(videoeffect_label);
	navElement.push_back(videoeffect);

    wxString libelleCriteria = CLibResource::LoadStringFromResource(L"LBLCRITERIA", 1);
    criteria = new CToolbarTexte(themeToolbar.texte);
    criteria->SetCommandId(WM_CRITERIA);
    criteria->SetLibelle(libelleCriteria);
    navElement.push_back(criteria);

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

void CToolbarInfos::SetAudioVideoPush()
{
	audiovideo->SetPush(true);
}
void CToolbarInfos::SetVideoEffectPush()
{
	videoeffect->SetPush(true);
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
	this->FastRefresh(this);
}

void CToolbarInfos::SetMapInactif()
{
	map->SetVisible(false);
	this->FastRefresh(this);
}

void CToolbarInfos::SetEffectActif()
{
	effect->SetVisible(true);
	this->FastRefresh(this);
}

void CToolbarInfos::SetCriteriaPush()
{
    criteria->SetPush(true);
}


void CToolbarInfos::SetEffectInactif()
{
	effect->SetVisible(false);
	this->FastRefresh(this);
}

void CToolbarInfos::SetEffectParameterInactif()
{
	effectParameter->SetVisible(false);
	this->FastRefresh(this);
}

void CToolbarInfos::SetEffectParameterActif(const wxString &libelle)
{
	if(libelle != "")
	{
		effectParameter->SetVisible(true);
		effectParameter->SetLibelle(libelle);
		effectParameter->SetLibelleTooltip(libelle);
		if (navPush != nullptr)
			navPush->SetInactif();
		effectParameter->SetActif();
		this->FastRefresh(this);
	}
}

void CToolbarInfos::SetVideoToolbar()
{
	history->SetVisible(false);
	map->SetVisible(false);
	effectParameter->SetVisible(false);
    effect->SetVisible(false);
	videoeffect->SetVisible(true);
	audiovideo->SetVisible(true);
	this->FastRefresh(this);
}

void CToolbarInfos::SetPictureThumbnailToolbar()
{
    effect->SetVisible(false);
	audiovideo->SetVisible(false);
	videoeffect->SetVisible(false);
    history->SetVisible(false);
    effectParameter->SetVisible(false);
    this->FastRefresh(this);
}


void CToolbarInfos::SetPictureToolbar()
{
    effect->SetVisible(true);
	videoeffect->SetVisible(false);
	history->SetVisible(true);
	audiovideo->SetVisible(false);
	effectParameter->SetVisible(false);
	this->FastRefresh(this);
}

void CToolbarInfos::Resize()
{
	int nbElement = (int)navElement.size();
	themeToolbar.texte.SetTailleX(GetWindowWidth() / nbElement);

	for (CToolbarElement * nav : navElement)
	{
		nav->Resize(themeToolbar.texte.GetTailleX(), themeToolbar.texte.GetTailleY());
	}
	this->FastRefresh(this);
}

void CToolbarInfos::EventManager(const int &id)
{
	if (toolbarInterface != nullptr)
	{
		toolbarInterface->ClickShowButton(id);
	}
}




