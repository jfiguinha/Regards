#ifndef JPEGOPTION_H
#define JPEGOPTION_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(JpegOption)
	#include <wx/radiobox.h>
	#include <wx/statbox.h>
	#include <wx/slider.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(JpegOption)
//*)

class CInterpolationFilterSelect: public wxDialog
{
	public:

		CInterpolationFilterSelect(wxWindow* parent);
		virtual ~CInterpolationFilterSelect();

		wxButton* btnCancel;
		wxRadioBox* rbCodec;
		wxButton* btnOK;
		//*)
    
        bool IsOk();
        void SetFilterIndex(const int &numFilter);
        int GetFilterIndex();

	protected:

		//(*Identifiers(JpegOption)
		//*)

	private:

		//(*Handlers(JpegOption)
		void OnbtnOKClick(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);
		//*)

        bool isOk;
        int numFilter;
		DECLARE_EVENT_TABLE()
};

#endif
