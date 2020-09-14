#include "header.h"
//
//  CalendarDialogBox.cpp
//  Regards.libExplorer
//
//  Created by figuinha jacques on 28/09/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "KeywordDialogBox.h"
using namespace Regards::Window;

CKeywordDialogBox::CKeywordDialogBox(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxDialog(parent, id, title, pos, size, style)
{
  
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainSizer);
	wxString choices[8] = { "0", "1", "2", "3", "4", "5", "6", "7" };
	//wxArrayString choices;

	m_listKeyword = new wxCheckListBox(this, wxID_ANY, wxPoint(40, 250), wxSize(260, 150), 8, choices, wxLB_SINGLE | wxLB_HSCROLL, wxDefaultValidator);
    
    mainSizer->Add(m_listKeyword, 0, wxLEFT|wxRIGHT|wxTOP|wxBOTTOM|wxEXPAND, 0);
    
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
	Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CKeywordDialogBox::OnOk));
	Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CKeywordDialogBox::OnCancel));
}

bool CKeywordDialogBox::IsOk()
{
	return false;
}


void CKeywordDialogBox::OnOk(wxCommandEvent& event)
{
	this->Close();
	this->EndDialog(0);
}

void CKeywordDialogBox::OnCancel(wxCommandEvent& event)
{
	this->Close();
	this->EndDialog(0);
}

CKeywordDialogBox::~CKeywordDialogBox()
{
}
