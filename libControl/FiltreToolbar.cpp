#include <header.h>
#include "FiltreToolbar.h"
#include <LibResource.h>
#include <window_id.h>
using namespace Regards::Control;

#define WM_OK 1
#define WM_CANCEL 2

CFiltreToolbar::CFiltreToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	numFiltre = 0;

	ok = CreateButton("IDB_OK", "IDS_LBLOK", WM_OK);
	cancel = CreateButton("IDB_CANCEL", "IDS_LBLCANCEL", WM_CANCEL);
}


void CFiltreToolbar::SetNumFiltre(const int& numFiltre)
{
	this->numFiltre = numFiltre;
}

void CFiltreToolbar::EventManager(const int& id)
{
	switch (id)
	{
	case WM_OK:
		{
			wxCommandEvent evt(wxEVENT_FILTREOK);
			evt.SetInt(numFiltre);
			GetParent()->GetEventHandler()->AddPendingEvent(evt);
		}
		break;

	case WM_CANCEL:
		{
			wxCommandEvent evt(wxEVENT_FILTRECANCEL);
			evt.SetInt(numFiltre);
			GetParent()->GetEventHandler()->AddPendingEvent(evt);
		}
		break;

	default:
		break;
	}
}
