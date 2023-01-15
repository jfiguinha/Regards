#pragma once

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

class CBpgOption : public wxDialog
{
public:
	CBpgOption(wxWindow* parent);
	~CBpgOption() override;

	//(*Declarations(JpegOption)
	wxRadioBox* rbCompression;
	wxStaticBox* sbCompression;
	wxButton* btnCancel;
	wxSlider* slCompression;
	wxButton* btnOK;
	//*)

	bool IsOk();
	int CompressionOption();
	int CompressionLevel();

protected:
	//(*Identifiers(JpegOption)
	//*)

private:
	//(*Handlers(JpegOption)
	void OnbtnOKClick(wxCommandEvent& event);
	void OnbtnCancelClick(wxCommandEvent& event);
	void OnslCompressionCmdScroll(wxScrollEvent& event);
	//*)

	bool isOk;
	int compressOption;
	int compressLevel;
	DECLARE_EVENT_TABLE()
};
