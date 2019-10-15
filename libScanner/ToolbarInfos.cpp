#include <header.h>
#include "ToolbarInfos.h"
#include <LibResource.h>
using namespace Regards::Window;
using namespace Regards::Scanner;


CToolbarInfos::CToolbarInfos(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	infos = nullptr;
	this->toolbarInterface = toolbarInterface;
    saveLastPush = true;
	wxString infos_label = CLibResource::LoadStringFromResource(L"LBLINFOS",1);// L"Infos";
	wxString ocr_label = "OCR";
	infos = new CToolbarTexte(themeToolbar.texte);
	infos->SetCommandId(WM_INFOS);
	infos->SetLibelle(infos_label);
	navElement.push_back(infos);

	ocrText = new CToolbarTexte(themeToolbar.texte);
	ocrText->SetCommandId(WM_OCR);
	ocrText->SetLibelle(ocr_label);
	navElement.push_back(ocrText);
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
	Refresh();
}

void CToolbarInfos::EventManager(const int &id)
{
	if (toolbarInterface != nullptr)
	{
		toolbarInterface->ClickShowButton(id);
	}
}




