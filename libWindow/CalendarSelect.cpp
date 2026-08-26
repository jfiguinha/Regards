#include "header.h"
//
//  CalendarSelect.cpp
//  Regards.libExplorer
//
//  Created by figuinha jacques on 01/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "CalendarSelect.h"
#include "CalendarDialogBox.h"
#include <ConvertUtility.h>
using namespace Regards::Window;


bool CCalendarSelect::SelectNewDate(wxWindow* window, const wxString& dataInfos)
{
	CCalendarDialogBox calendarBox(window);
	wxDateTime dt;
	if (dataInfos.Length() == 10)
	{
		vector<wxString> vDateTime = CConvertUtility::split(dataInfos, dataInfos[4]);
		int month = atoi(vDateTime[1]) - 1;
		dt.SetYear(atoi(vDateTime[0]));
		dt.SetMonth(static_cast<wxDateTime::Month>(month));
		dt.SetDay(atoi(vDateTime[2]));
	}

	calendarBox.SetDate(dt);

	if (calendarBox.ShowModal() == wxID_OK)
	{
		SetSelectDate(calendarBox.GetDate());
		return true;
	}

	return false;
}

void CCalendarSelect::SetSelectDate(const wxDateTime& dateSelect)
{
	dt = dateSelect;
	updateDate = dt.Format("%Y.%m.%d");
}

bool CCalendarSelect::SelectNewDate(wxWindow* window, const wxDateTime& dateSelect)
{
	CCalendarDialogBox calendarBox(window);
	calendarBox.SetDate(dateSelect);

	if (calendarBox.ShowModal() == wxID_OK)
	{
		SetSelectDate(calendarBox.GetDate());
		return true;
	}


	return false;
}

wxDateTime CCalendarSelect::GetSelectDate()
{
	return dt;
}

wxString CCalendarSelect::GetSelectStringDate()
{
	return updateDate;
}
