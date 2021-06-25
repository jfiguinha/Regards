#pragma once

#ifndef WX_PRECOMP
//(*HeadersPCH(J2kOption)
#include <wx/radiobox.h>
#include <wx/statbox.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
#endif
//(*Headers(J2kOption)
//*)

class J2kOption : public wxDialog
{
public:
	J2kOption(wxWindow* parent);
	~J2kOption() override;

	//(*Declarations(J2kOption)
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
	//(*Identifiers(J2kOption)
	//*)

private:
	//(*Handlers(J2kOption)
	void OnbtnOKClick(wxCommandEvent& event);
	void OnbtnCancelClick(wxCommandEvent& event);
	void OnslCompressionCmdScroll(wxScrollEvent& event);
	//*)

	bool isOk;
	int compressOption;
	int compressLevel;
DECLARE_EVENT_TABLE()
};
