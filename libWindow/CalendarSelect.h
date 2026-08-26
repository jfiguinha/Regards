//
//  CalendarSelect.hpp
//  Regards.libExplorer
//
//  Created by figuinha jacques on 01/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#pragma once

namespace Regards::Window
{
	class CCalendarSelect
	{
	public:
		CCalendarSelect() = default;
		~CCalendarSelect() = default;

		bool SelectNewDate(wxWindow* window, const wxDateTime& dateSelect);
		bool SelectNewDate(wxWindow* window, const wxString& dataInfos);
		wxDateTime GetSelectDate();
		wxString GetSelectStringDate();

	private:

		void SetSelectDate(const wxDateTime& dateSelect);
		wxDateTime dt;
		wxString updateDate;
	};
}
