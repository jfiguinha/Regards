#include "header.h"
//
//  CalendarDialogBox.cpp
//  Regards.libExplorer
//
//  Created by figuinha jacques on 28/09/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "KeywordDialogBox.h"
#include <SqlFindCriteria.h>
#include <SqlCriteria.h>
#include <SqlPhotoCriteria.h>
using namespace Regards::Sqlite;
using namespace Regards::Window;

CKeywordDialogBox::CKeywordDialogBox(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxDialog(parent, id, title, pos, size, style)
{
	CSqlFindCriteria sqlCriteria;
	CriteriaVector criteriaVector;
	sqlCriteria.SearchCriteria(&criteriaVector, 7, 1);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainSizer);
	//wxString choices[8] = { "0", "1", "2", "3", "4", "5", "6", "7" };
	wxArrayString choices;
	for (CCriteria criteria : criteriaVector)
	{
		choices.push_back(criteria.GetLibelle());
	}
	wxCArrayString helper(choices);

	m_listKeyword = new wxCheckListBox(this, wxID_ANY, wxPoint(40, 250), wxSize(260, 150), criteriaVector.size(), helper.GetStrings(), wxLB_SINGLE | wxLB_HSCROLL, wxDefaultValidator);
    
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
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CKeywordDialogBox::OnOk));
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CKeywordDialogBox::OnCancel));
}

void CKeywordDialogBox::DeleteItemChecked()
{
	if (isOk)
	{
		CSqlPhotoCriteria photoCriteria;
		CSqlFindCriteria sqlFindCriteria;
		CSqlCriteria sqlCriteria;
		CSqlPhotoCriteria sqlPhotoCriteria;
		CriteriaVector criteriaVector;
		wxArrayInt listItem;
		m_listKeyword->GetCheckedItems(listItem);
		sqlFindCriteria.SearchCriteria(&criteriaVector, 7, 1);

		for (int i : listItem)
		{
			if (i < criteriaVector.size())
			{
				int numCriteria = criteriaVector[i].GetId();
				sqlCriteria.DeletePhotoCriteria(1, numCriteria);
				sqlPhotoCriteria.DeleteCriteria(numCriteria);
			}
		}


	}
}

bool CKeywordDialogBox::IsOk()
{
	return isOk;
}

void CKeywordDialogBox::OnOk(wxCommandEvent& event)
{
	isOk = true;
	DeleteItemChecked();
	this->Close();
	this->EndDialog(0);
}

void CKeywordDialogBox::OnCancel(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
	this->EndDialog(0);
}

CKeywordDialogBox::~CKeywordDialogBox()
{
}
