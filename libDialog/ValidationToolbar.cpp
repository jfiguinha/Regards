#include <header.h>
#include "ValidationToolbar.h"
#include <LibResource.h>
#include <window_id.h>
using namespace Regards::Scanner;

#define WM_OK 1
#define WM_CANCEL 2

CValidationToolbar::CValidationToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	this->parent = parent;
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


CValidationToolbar::~CValidationToolbar()
{
}

void CValidationToolbar::EventManager(const int &id)
{
	switch (id)
	{

		case WM_OK:
		{
			if (parent != nullptr)
			{
				wxCommandEvent evt(wxID_OK);
				parent->GetEventHandler()->AddPendingEvent(evt);
			}
		}
		break;

		case WM_CANCEL:
		{
			if (parent != nullptr)
			{
				wxCommandEvent evt(wxID_CANCEL);
				parent->GetEventHandler()->AddPendingEvent(evt);
				//parent->Close();
			}
		}
		break;

	default:
		break;
	}
}

