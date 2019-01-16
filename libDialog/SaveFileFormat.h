#ifndef SAVEFILEFORMAT_H
#define SAVEFILEFORMAT_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(SaveFileFormat)
	#include <wx/radiobox.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(SaveFileFormat)
//*)

class SaveFileFormat: public wxDialog
{
	public:

		SaveFileFormat(wxWindow* parent);
		virtual ~SaveFileFormat();
        int GetSaveFormat();
        bool IsOk();

		//(*Declarations(SaveFileFormat)
		wxRadioBox* rbSelectFormat;
		wxButton* btnCancel;
		wxButton* btnOK;
		//*)

	protected:

		//(*Identifiers(SaveFileFormat)
		//*)

	private:

		//(*Handlers(SaveFileFormat)
		void OnbtnOKClick(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		//*)

        bool isOk;
        int fileFormat;
    
		DECLARE_EVENT_TABLE()
};

#endif
