#include <header.h>
#include "KeywordDialogBox.h"
#include <RegardsConfigParam.h>
#include <LibResource.h>
#include <ParamInit.h>

#ifndef WX_PRECOMP
//(*InternalHeadersPCH(ConfigRegards)
//*)
#endif
//(*InternalHeaders(ConfigRegards)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(ConfigRegards)
//*)

BEGIN_EVENT_TABLE(KeywordDialogBox, wxDialog)
//(*EventTable(ConfigRegards)
//*)
END_EVENT_TABLE()


#include <SqlFindCriteria.h>
#include <SqlCriteria.h>
#include <SqlPhotoCriteria.h>
using namespace Regards::Sqlite;


KeywordDialogBox::KeywordDialogBox(wxWindow* parent)
{
	//(*Initialize(ConfigRegards)
	SetTitle("Keyword List");
	wxXmlResource::Get()->LoadObject(this, parent, _T("KeywordDialogBox"), _T("wxDialog"));

	CSqlFindCriteria sqlCriteria;
	CriteriaVector criteriaVector;
	sqlCriteria.SearchCriteria(&criteriaVector, 7, 1);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainSizer);
	//wxString choices[8] = { "0", "1", "2", "3", "4", "5", "6", "7" };
	
	for (CCriteria criteria : criteriaVector)
	{
		choices.push_back(criteria.GetLibelle());
	}
	wxCArrayString helper(choices);

	m_listKeyword = (wxCheckListBox*)FindWindow(XRCID("ID_CHECKLIST"));
	if(criteriaVector.size() > 0)
		m_listKeyword->InsertItems(choices, 0);
	btCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));
	btOk = (wxButton*)FindWindow(XRCID("ID_OK"));
    // Connect events
	Connect(XRCID("ID_OK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&KeywordDialogBox::OnbtnOkClick);
	Connect(XRCID("ID_CANCEL"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&KeywordDialogBox::OnBtnCancelClick);
}

void KeywordDialogBox::DeleteItemChecked()
{
	if (isOk && choices.size() > 0)
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

		sqlCriteria.DeleteCriteriaAlone();
	}
}

bool KeywordDialogBox::IsOk()
{
	return isOk;
}

void KeywordDialogBox::OnbtnOkClick(wxCommandEvent& event)
{
	isOk = true;
	DeleteItemChecked();
	this->Close();
	//this->EndDialog(0);
}

void KeywordDialogBox::OnBtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
	//this->EndDialog(0);
}

KeywordDialogBox::~KeywordDialogBox()
{
}
