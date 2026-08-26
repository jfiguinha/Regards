#include <header.h>
#include "ToolbarInfos.h"
#include <LibResource.h>
using namespace Regards::Window;
using namespace Regards::Viewer;


CToolbarInfos::CToolbarInfos(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme,
                             CToolbarInterface* toolbarInterface, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	this->toolbarInterface = toolbarInterface;
	saveLastPush = true;

	infos = CreateTexte("LBLINFOS", WM_INFOS);
	history = CreateTexte("LBLHISTORY", WM_HISTORY);
	effect = CreateTexte("LBLEFFECT", WM_EFFECT);
	map = CreateTexte("LBLMAPS", WM_MAPS);
	audiovideo = CreateTexte("LBLAUDIOVIDEO", WM_AUDIOVIDEO);
	videoeffect = CreateTexte("LBLVIDEOEFFECT", WM_VIDEOEFFECT);
	criteria = CreateTexte("LBLCRITERIA", WM_CRITERIA);
	effectParameter = CreateTexte("LBLEFFECTPARAMETER", WM_EFFECTPARAMETER);
	histogramParameter = CreateTexte("LBLHISTOGRAM", WM_HISTOGRAM);
}

void CToolbarInfos::SetHistogramPush()
{
	histogramParameter->SetPush(true);
}

void CToolbarInfos::SetInfosPush()
{
	SetAllDisable();
	infos->SetPush(true);
	this->Refresh(false);
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
	SetAllDisable();
	infos->SetVisible(true);
	toolbarInterface->ClickShowButton(WM_INFOS);
}

void CToolbarInfos::SetMapActif()
{
	SetAllDisable();
	map->SetVisible(true);
	needToRefresh = true;
}

void CToolbarInfos::SetMapInactif()
{
	map->SetVisible(false);
	needToRefresh = true;
}

void CToolbarInfos::SetEffectActif()
{
	SetAllDisable();
	effect->SetVisible(true);
	needToRefresh = true;
}

void CToolbarInfos::SetCriteriaPush()
{
	criteria->SetPush(true);
}


void CToolbarInfos::SetEffectInactif()
{
	effect->SetVisible(false);
	needToRefresh = true;
}

void CToolbarInfos::SetEffectParameterInactif()
{
	effectParameter->SetVisible(false);
	needToRefresh = true;
}

void CToolbarInfos::SetEffectParameterActif(const wxString& libelle)
{

	if (libelle != "")
	{
		effectParameter->SetVisible(true);
		effectParameter->SetLibelle(libelle);
		effectParameter->SetLibelleTooltip(libelle);
		effectParameter->SetActif();
		needToRefresh = true;
	}
}

void CToolbarInfos::SetVideoToolbar()
{
	histogramParameter->SetVisible(false);
	history->SetVisible(false);
	map->SetVisible(false);
	effectParameter->SetVisible(false);
	effect->SetVisible(false);
	videoeffect->SetVisible(true);
	audiovideo->SetVisible(true);
	needToRefresh = true;
}

void CToolbarInfos::SetPictureThumbnailToolbar()
{
	effect->SetVisible(false);
	audiovideo->SetVisible(false);
	videoeffect->SetVisible(false);
	history->SetVisible(false);
	effectParameter->SetVisible(false);
	histogramParameter->SetVisible(false);
	needToRefresh = true;
}


void CToolbarInfos::SetPictureToolbar()
{
	effect->SetVisible(true);
	videoeffect->SetVisible(false);
	history->SetVisible(true);
	audiovideo->SetVisible(false);
	effectParameter->SetVisible(false);
	histogramParameter->SetVisible(true);
	needToRefresh = true;
}

void CToolbarInfos::Resize()
{
	int nbElement = static_cast<int>(navElement.size());
	themeToolbar.texte.SetTailleX(GetWindowWidth() / nbElement);

	for (auto& nav : navElement)
	{
		nav->Resize(themeToolbar.texte.GetTailleX(), themeToolbar.texte.GetTailleY());
	}
	needToRefresh = true;
}

void CToolbarInfos::EventManager(const int& id)
{
	if (toolbarInterface != nullptr)
	{
		toolbarInterface->ClickShowButton(id);
	}
}
