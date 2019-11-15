#include <header.h>
#include "FiltreToolbar.h"
#include "ViewerPDF.h"
#include "PanelInfosWnd.h"
#include <LibResource.h>
#include <window_id.h>
using namespace Regards::Scanner;

#define WM_OK 1
#define WM_CANCEL 2

CFiltreToolbar::CFiltreToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	numFiltre = 0;
	wxString libelleOk = CLibResource::LoadStringFromResource(L"IDS_LBLOK",1);
	wxString libelleCancel = CLibResource::LoadStringFromResource(L"IDS_LBLCANCEL",1);

	CToolbarButton * ok = new CToolbarButton(themeToolbar.button);
	ok->SetButtonResourceId(L"IDB_OK");
	ok->SetCommandId(WM_OK);
	ok->SetLibelle(libelleOk);
	navElement.push_back(ok);

	CToolbarButton * cancel = new CToolbarButton(themeToolbar.button);
	cancel->SetButtonResourceId(L"IDB_CANCEL");
	cancel->SetCommandId(WM_CANCEL);
	cancel->SetLibelle(libelleCancel);
	navElement.push_back(cancel);
}


CFiltreToolbar::~CFiltreToolbar()
{
}


void CFiltreToolbar::SetNumFiltre(const int &numFiltre)
{
	this->numFiltre = numFiltre;
}

void CFiltreToolbar::EventManager(const int &id)
{
	CPanelInfosWnd * panelInfos = (CPanelInfosWnd *)this->FindWindowById(PANELINFOSWNDSCANNERID);
	CViewerPDF * previewWindow = (CViewerPDF *)this->FindWindowById(PREVIEWVIEWERID);
	switch (id)
	{

	case WM_OK:
		if (panelInfos != nullptr)
			panelInfos->OnFiltreOk(numFiltre);
		if (previewWindow != nullptr)
			previewWindow->HideValidationToolbar();
		break;

	case WM_CANCEL:
		if (panelInfos != nullptr)
			panelInfos->OnFiltreCancel();
		if (previewWindow != nullptr)
			previewWindow->HideValidationToolbar();
		break;

	default:
		break;
	}
}

