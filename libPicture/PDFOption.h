#ifndef PDFOption_H
#define PDFOption_H


#ifndef WX_PRECOMP
	//(*HeadersPCH(PDFOption)
	#include <wx/radiobox.h>
	#include <wx/statbox.h>
	#include <wx/slider.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(PDFOption)
//*)

class PDFOption: public wxDialog
{
	public:

		PDFOption(wxWindow* parent);
		virtual ~PDFOption();

		//(*Declarations(PDFOption)
		wxStaticBox* sbCompressionLevel;
		wxButton* btnCancel;
		wxButton* btnOK;
		wxRadioBox* rbCompression;
		wxSlider* slCompressLevel;
		//*)
        bool IsOk();
        int CompressionOption();
        //int CompressionLevel();
    
	protected:

		//(*Identifiers(PDFOption)
		//*)

	private:

		//(*Handlers(PDFOption)
		void OnslCompressLevelCmdScroll(wxScrollEvent& event);
		void OnbtnOKClick(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		//*)

        int compressOption;
        bool isOk;
		DECLARE_EVENT_TABLE()
};

#endif
