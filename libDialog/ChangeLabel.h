#pragma once

#ifndef WX_PRECOMP
//(*Headers(ChangeLabel)
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
#endif

class ChangeLabel: public wxDialog
{
	public:

		ChangeLabel(wxWindow* parent);
		virtual ~ChangeLabel();

		//(*Declarations(ChangeLabel)
		wxButton* btOK;
		wxTextCtrl* txtCtrlFaceName;
		wxButton* btCancel;
		wxStaticText* stFaceName;
		//*)
		bool IsOk();
		void SetLabel(const wxString &labelFaceName);
		wxString GetNewLabel();

	protected:

		//(*Identifiers(ChangeLabel)
		//*)

	private:

		//(*Handlers(ChangeLabel)
		//*)
		void OnbtnOkClick(wxCommandEvent& event);
		void OnBtnCancelClick(wxCommandEvent& event);

		bool isOk;
		wxString labelFaceName;
		DECLARE_EVENT_TABLE()
};


