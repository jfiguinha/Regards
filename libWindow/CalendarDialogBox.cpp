//
//  CalendarDialogBox.cpp
//  Regards.libExplorer
//
//  Created by figuinha jacques on 28/09/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "CalendarDialogBox.h"
using namespace Regards::Window;

CCalendarDialogBox::CCalendarDialogBox(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxDialog(parent, id, title, pos, size, style)
{
  
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainSizer);
    
    m_calendar = DoCreateCalendar(wxDefaultDateTime, style);
    
    mainSizer->Add(m_calendar, 0, wxLEFT|wxRIGHT|wxTOP|wxBOTTOM|wxEXPAND, 0);
    
    mainSizer->AddStretchSpacer(1);
    
    m_staticLine15 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxLI_HORIZONTAL);
    
    mainSizer->Add(m_staticLine15, 0, wxLEFT|wxRIGHT|wxTOP|wxBOTTOM|wxEXPAND, 0);
    
    wxBoxSizer* boxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    
    mainSizer->Add(boxSizer12, 0, wxLEFT|wxRIGHT|wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL, 0);
    
    m_buttonOK = new wxButton(this, wxID_OK, _("OK"), wxDefaultPosition, wxSize(-1,-1), 0);
    m_buttonOK->SetDefault();
    boxSizer12->Add(m_buttonOK, 0, wxLEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);
    
    m_buttonCancel = new wxButton(this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    boxSizer12->Add(m_buttonCancel, 0, wxLEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);
    

    
    SetSize(wxSize(300,230));
    Centre();
    // Connect events
}

wxCalendarCtrlBase * CCalendarDialogBox::DoCreateCalendar(const wxDateTime& dt, long style)
{
    wxCalendarCtrlBase *calendar;

	calendar = new wxCalendarCtrl(this, wxID_ANY,
                                      dt,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      style);
    
    calendar->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CCalendarDialogBox::OnCalRClick));
    
    return calendar;
}

void CCalendarDialogBox::OnCalRClick(wxMouseEvent& event)
{
    wxDateTime dt;
    //wxDateTime::WeekDay wd;
    
    const wxPoint pt = event.GetPosition();
    wxString msg = wxString::Format("Point (%d, %d) is ", pt.x, pt.y);
    /*
    switch ( m_panel->GetCal()->HitTest(pt, &dt, &wd) )
    {
        default:
            wxFAIL_MSG( "unexpected" );
            // fall through
            
        case wxCAL_HITTEST_NOWHERE:
            msg += "nowhere";
            break;
            
        case wxCAL_HITTEST_HEADER:
            msg += wxString::Format("over %s", wxDateTime::GetWeekDayName(wd));
            break;
            
        case wxCAL_HITTEST_DAY:
            msg += wxString::Format("over %s", dt.FormatISODate());
            break;
            
        case wxCAL_HITTEST_INCMONTH:
            msg += "over next month button";
            break;
            
        case wxCAL_HITTEST_DECMONTH:
            msg += "over previous month button";
            break;
            
        case wxCAL_HITTEST_SURROUNDING_WEEK:
            msg += "over a day from another month";
            break;
    }
    */
    wxLogMessage("%s", msg);
}

CCalendarDialogBox::~CCalendarDialogBox()
{
}
