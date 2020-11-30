#pragma once

#ifndef WX_PRECOMP
	//(*HeadersPCH(ConfigRegards)
	#include <wx/stattext.h>
	#include <wx/radiobox.h>
	#include <wx/statbox.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(ConfigRegards)
#include <wx/spinctrl.h>
//*)

class ExternalProgram: public wxDialog
{
	public:

		ExternalProgram(wxWindow* parent);
		virtual ~ExternalProgram();

		//(*Declarations(ConfigRegards)
		wxButton* btOk;
		wxTextCtrl  * txtPicturePath;
		wxButton* btPicturePath;
		wxButton* btCancel;
		wxTextCtrl  * txtVideoPath;
		wxButton* btVideoPath;
		//*)

		bool IsOk();

	protected:

		//(*Identifiers(ConfigRegards)
		//*)

	private:

		//(*Handlers(ConfigRegards)
		void Init();
		void OnbtnOkClick(wxCommandEvent& event);
		void OnBtnCancelClick(wxCommandEvent& event);
		void OnbtnPathVideoClick(wxCommandEvent& event);
		void OnBtnPathPictureClick(wxCommandEvent& event);
		//*)
		bool isOk;
		DECLARE_EVENT_TABLE()
};

