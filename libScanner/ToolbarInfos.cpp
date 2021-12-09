#include <header.h>
#include "ToolbarInfos.h"
#include <LibResource.h>
using namespace Regards::Window;
using namespace Regards::Scanner;


CToolbarInfos::CToolbarInfos(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical), editorParam(nullptr)
{
	infos = nullptr;
	history = nullptr;
	effect = nullptr;
	effectParameter = nullptr;
	this->toolbarInterface = toolbarInterface;
	saveLastPush = true;
	wxString infos_label = CLibResource::LoadStringFromResource(L"LBLINFOS", 1); // L"Infos";
	wxString ocr_label = "OCR";
	wxString history_label = CLibResource::LoadStringFromResource(L"LBLHISTORY", 1); //L"History";
	wxString effect_label = CLibResource::LoadStringFromResource(L"LBLEFFECT", 1); //L"Effect";
	wxString effectParameter_label = CLibResource::LoadStringFromResource(L"LBLEFFECTPARAMETER", 1);
	//L"Effect Parameter";
	wxString editor_label = "EDITOR";

	infos = new CToolbarTexte(themeToolbar.texte);
	infos->SetCommandId(WM_INFOS);
	infos->SetLibelle(infos_label);
	navElement.push_back(infos);

	ocrText = new CToolbarTexte(themeToolbar.texte);
	ocrText->SetCommandId(WM_OCR);
	ocrText->SetLibelle(ocr_label);
	navElement.push_back(ocrText);

	history = new CToolbarTexte(themeToolbar.texte);
	history->SetCommandId(WM_HISTORY);
	history->SetLibelle(history_label);
	navElement.push_back(history);

	effect = new CToolbarTexte(themeToolbar.texte);
	effect->SetCommandId(WM_EFFECT);
	effect->SetLibelle(effect_label);
	navElement.push_back(effect);

	effectParameter = new CToolbarTexte(themeToolbar.texte);
	effectParameter->SetCommandId(WM_EFFECTPARAMETER);
	effectParameter->SetLibelle(effectParameter_label);
	navElement.push_back(effectParameter);

	/*
	editorParam = new CToolbarTexte(themeToolbar.texte);
	editorParam->SetCommandId(WM_HTMLEDITOR);
	editorParam->SetLibelle(editor_label);
	navElement.push_back(editorParam);
	*/
}

CToolbarInfos::~CToolbarInfos()
{
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

void CToolbarInfos::SetEffectParameterActif(const wxString &libelle)
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
	int nbElement = (int)navElement.size();
	themeToolbar.texte.SetTailleX(GetWindowWidth() / nbElement);

	for (CToolbarElement * nav : navElement)
	{
		nav->Resize(themeToolbar.texte.GetTailleX(), themeToolbar.texte.GetTailleY());
	}
	needToRefresh = true;
}

void CToolbarInfos::EventManager(const int &id)
{
	if (toolbarInterface != nullptr)
	{
		toolbarInterface->ClickShowButton(id);
	}
}




