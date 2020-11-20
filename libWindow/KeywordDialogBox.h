#pragma once

#ifndef WX_PRECOMP
//(*HeadersPCH(KeywordDialogBox)
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
#endif
//(*Headers(KeywordDialogBox)
#include <wx/spinctrl.h>
//*)

class KeywordDialogBox : public wxDialog
{
public:

	KeywordDialogBox(wxWindow* parent);
	virtual ~KeywordDialogBox();

	//(*Declarations(ConfigRegards)
	wxButton* btOk;
	wxButton* btCancel;
	wxCheckListBox * m_listKeyword;
	//*)

	bool IsOk();

protected:

	//(*Identifiers(ConfigRegards)
	//*)
	void DeleteItemChecked();

private:

	//(*Handlers(ConfigRegards)
	//void Init();
	void OnbtnOkClick(wxCommandEvent& event);
	void OnBtnCancelClick(wxCommandEvent& event);
	//*)
	bool isOk;
	wxArrayString choices;
	DECLARE_EVENT_TABLE()
};

