//
//  CalendarDialogBox.hpp
//  Regards.libExplorer
//
//  Created by figuinha jacques on 28/09/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#pragma once
#include <wx/dialog.h>
#include <wx/statline.h>
#include <wx/calctrl.h>

#if wxUSE_DATEPICKCTRL
#if wxUSE_DATEPICKCTRL_GENERIC
#include "wx/generic/datectrl.h"
#endif // wxUSE_DATEPICKCTRL_GENERIC
#endif // wxUSE_DATEPICKCTRL

#if wxUSE_TIMEPICKCTRL
#include "wx/timectrl.h"
#if wxUSE_TIMEPICKCTRL_GENERIC
#include "wx/generic/timectrl.h"
#endif // wxUSE_TIMEPICKCTRL_GENERIC
#endif // wxUSE_TIMEPICKCTRL

#ifdef wxHAS_NATIVE_CALENDARCTRL
#endif

namespace Regards
{
	namespace Window
	{
		class CCalendarDialogBox : public wxDialog
		{
		protected:
			wxStaticLine* m_staticLine15;
			wxButton* m_buttonOK;
			wxButton* m_buttonCancel;
			wxCalendarCtrlBase* m_calendar;
		protected:
			wxCalendarCtrlBase* DoCreateCalendar(const wxDateTime& dt, long style);

		public:
			CCalendarDialogBox(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Select new date"),
				const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(300, 200),
				long style = wxDEFAULT_DIALOG_STYLE);
			~CCalendarDialogBox() override;

			void OnCalRClick(wxMouseEvent& event);
			wxDateTime GetDate() const { return m_calendar->GetDate(); }
			void SetDate(const wxDateTime& dt) { m_calendar->SetDate(dt); }
		};
	}
}
