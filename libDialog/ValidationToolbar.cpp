#include <header.h>
#include "ValidationToolbar.h"
#include <LibResource.h>
using namespace Regards::Scanner;

#define WM_OK 1
#define WM_CANCEL 2

CValidationToolbar::CValidationToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme,
                                       const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	this->parent = parent;
	ok = CreateButton("IDB_OK", "IDS_LBLOK", WM_OK);
	cancel = CreateButton("IDB_CANCEL", "IDS_LBLCANCEL", WM_CANCEL);
}

void CValidationToolbar::EventManager(const int& id)
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
