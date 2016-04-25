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

CCalendarSelect::CCalendarSelect()
{
    
}

CCalendarSelect::~CCalendarSelect()
{
    
}

bool CCalendarSelect::SelectNewDate(wxWindow * window,const wxString & dataInfos)
{
    CCalendarDialogBox calendarBox(window);
    wxDateTime dt;
    if(dataInfos.Length() == 10)
    {
        printf("Date infos : %s \n", dataInfos.ToStdString().c_str());
        vector<wxString> vDateTime = CConvertUtility::split(dataInfos, dataInfos[4]);
        int month = atoi(vDateTime[1].c_str()) - 1;
        dt.SetYear(atoi(vDateTime[0].c_str()));
        dt.SetMonth((wxDateTime::Month)month);
        dt.SetDay(atoi(vDateTime[2].c_str()));
    }
    
    calendarBox.SetDate(dt);
    
    if(calendarBox.ShowModal()== wxID_OK)
    {
        dt = calendarBox.GetDate();
        int month = dt.GetMonth() + 1;
        int day = dt.GetDay();
        updateDate = to_string(dt.GetYear()) + ".";
        if(month < 10)
            updateDate.Append("0");
        updateDate.Append(to_string(month) + ".");
        if(day < 10)
            updateDate.Append("0");
        updateDate.Append(to_string(day));
       
        return true;
    }
    
    return false;
}

bool CCalendarSelect::SelectNewDate(wxWindow * window,const wxDateTime & dateSelect)
{
    CCalendarDialogBox calendarBox(window);
    
    calendarBox.SetDate(dateSelect);
    
    if(calendarBox.ShowModal()== wxID_OK)
    {
        dt = calendarBox.GetDate();
        int month = dt.GetMonth() + 1;
        int day = dt.GetDay();
        updateDate = to_string(dt.GetYear()) + ".";
        if(month < 10)
            updateDate.Append("0");
        updateDate.Append(to_string(month) + ".");
        if(day < 10)
            updateDate.Append("0");
        updateDate.Append(to_string(day));
        
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