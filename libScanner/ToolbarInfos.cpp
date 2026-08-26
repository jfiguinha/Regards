#include <header.h>
#include "ToolbarInfos.h"
#include <LibResource.h>
using namespace Regards::Window;
using namespace Regards::Scanner;


CToolbarInfos::CToolbarInfos(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme,
                             CToolbarInterface* toolbarInterface, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical), editorParam(nullptr)
{
	this->toolbarInterface = toolbarInterface;
	saveLastPush = true;

	infos = CreateTexte("LBLINFOS", WM_INFOS);
	ocrText = CreateTexte("LBLOCR", WM_OCR);
	history = CreateTexte("LBLHISTORY", WM_HISTORY);
	effect = CreateTexte("LBLEFFECT", WM_EFFECT);
	effectParameter = CreateTexte("LBLEFFECTPARAMETER", WM_EFFECTPARAMETER);
}

void CToolbarInfos::SetInfosPush()
{
	infos->SetPush(true);
}

void CToolbarInfos::SetOcrPush()
{
	ocrText->SetPush(true);
}

void CToolbarInfos::SetEffectPush()
{
	effect->SetPush(true);
}

void CToolbarInfos::SetHistoryPush()
{
	history->SetPush(true);
}

void CToolbarInfos::SetEditorPush()
{
	editorParam->SetPush(true);
}

void CToolbarInfos::SetEffectParameterPush()
{
	effectParameter->SetPush(true);
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
		if (navPush != nullptr)
			navPush->SetInactif();
		effectParameter->SetActif();
		needToRefresh = true;
	}
}

void CToolbarInfos::SetOcrActif()
{
	ocrText->SetVisible(true);
	toolbarInterface->ClickShowButton(WM_OCR);
}

void CToolbarInfos::SetInfosActif()
{
	//infos->SetActif();
	infos->SetVisible(true);
	toolbarInterface->ClickShowButton(WM_INFOS);
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
