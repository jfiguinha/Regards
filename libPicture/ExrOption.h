#pragma once
#ifndef WX_PRECOMP
	//(*HeadersPCH(ExrOption)
	#include <wx/radiobox.h>
	#include <wx/statbox.h>
	#include <wx/slider.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(ExrOption)
//*)

class ExrOption: public wxDialog
{
	public:

		ExrOption(wxWindow* parent);
		virtual ~ExrOption();
		wxButton* btnCancel;
		wxButton* btnOK;
		wxRadioBox* rbCompression;
        bool IsOk();
        int CompressionOption();
        //int CompressionLevel();
    
	protected:

		//(*Identifiers(ExrOption)
		//*)

	private:

		//(*Handlers(ExrOption)
		void OnslCompressLevelCmdScroll(wxScrollEvent& event);
		void OnbtnOKClick(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		//*)

        int compressOption;
        bool isOk;
		DECLARE_EVENT_TABLE()
};
