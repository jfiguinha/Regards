//
//  CalendarDialogBox.cpp
//  Regards.libExplorer
//
//  Created by figuinha jacques on 28/09/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "MapDialogBox.h"
using namespace Regards::Window;

CMapDialogBox::CMapDialogBox(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxDialog(parent, id, title, pos, size, style)
{
	webBrowser = nullptr;
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainSizer);
    
    webBrowser = wxWebView::New(this, wxID_ANY, "http://www.google.fr");
    webBrowser->Show(true);
    webBrowser->SetSize(0,0,500,240);
    
    //mainSizer->Add(webBrowser, 0, wxLEFT|wxRIGHT|wxTOP|wxBOTTOM|wxEXPAND, 0);
    
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
    

    
    SetSize(wxSize(500,300));
    Centre();
    // Connect events
}

void CMapDialogBox::SetUrl(const wxString &url)
{
    webBrowser->LoadURL(url);
}

wxString CMapDialogBox::GetUrl()
{
    return webBrowser->GetCurrentURL();
}


CMapDialogBox::~CMapDialogBox()
{
}

void CMapDialogBox::OnWebBrowserRClick(wxMouseEvent& event)
{
    const wxPoint pt = event.GetPosition();
    wxString msg = wxString::Format("Point (%d, %d) is ", pt.x, pt.y);
}
