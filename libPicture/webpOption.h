#pragma once

#ifndef WX_PRECOMP
//(*HeadersPCH(WebpOption)
#include <wx/radiobox.h>
#include <wx/statbox.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
#endif
//(*Headers(WebpOption)
//*)

class WebpOption : public wxDialog
{
public:

	WebpOption(wxWindow* parent);
	virtual ~WebpOption();

	//(*Declarations(WebpOption)
	wxStaticBox* sbCompression;
	wxButton* btnCancel;
	wxRadioBox* rbCodec;
	wxSlider* slCompression;
	wxButton* btnOK;
	//*)

	bool IsOk();
	int CompressionOption();
	int CompressionLevel();

protected:

	//(*Identifiers(WebpOption)
	//*)

private:

	//(*Handlers(WebpOption)
	void OnbtnOKClick(wxCommandEvent& event);
	void OnbtnCancelClick(wxCommandEvent& event);
	void OnslCompressionCmdScroll(wxScrollEvent& event);
	//*)

	bool isOk;
	int compressOption;
	int compressLevel;
	DECLARE_EVENT_TABLE()
};
