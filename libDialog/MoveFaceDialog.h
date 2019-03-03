#ifndef OPENCLDIALOG_H
#define OPENCLDIALOG_H


#ifndef WX_PRECOMP
	//(*HeadersPCH(OpenCLDialog)
	#include <wx/stattext.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	#include <wx/combobox.h>
	//*)
#endif
//(*Headers(OpenCLDialog)
//*)

class MoveFaceDialog: public wxDialog
{
	public:

		MoveFaceDialog(wxWindow* parent);
		virtual ~MoveFaceDialog();

		//(*Declarations(OpenCLDialog)
		wxButton* BtnCancel;
		wxButton* btnOk;
		wxStaticText* deviceLabel;
		wxComboBox* cbFaceLabel;
		//*)

		bool IsOk();
		wxString GetFaceNameSelected();

	protected:

		//(*Identifiers(OpenCLDialog)
		//*)

	private:

		//(*Handlers(OpenCLDialog)
		void OnInit(wxInitDialogEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnbtnOkClick(wxCommandEvent& event);
		void OnBtnCancelClick(wxCommandEvent& event);
		void OnPlatformSelected(wxCommandEvent& event);
		//*)


		bool isOk;
		wxString selectItem;
		DECLARE_EVENT_TABLE()
};

#endif
