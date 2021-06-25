#pragma once


#ifndef WX_PRECOMP
	//(*HeadersPCH(TiffOption)
	#include <wx/radiobox.h>
	#include <wx/statbox.h>
	#include <wx/slider.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(TiffOption)
//*)

class BmpOption : public wxDialog
{
public:
	BmpOption(wxWindow* parent);
	~BmpOption() override;

	wxButton* btnCancel;
	wxButton* btnOK;
	wxStaticBitmap* bitmap;
	wxRadioBox* rbCompression;
	//*)
	bool IsOk();
	int CompressionOption();
	//int CompressionLevel();

protected:
	//(*Identifiers(TiffOption)
	//*)

private:
	//(*Handlers(TiffOption)
	void OnbtnOKClick(wxCommandEvent& event);
	void OnbtnCancelClick(wxCommandEvent& event);
	//*)

	int compressOption;
	bool isOk;
DECLARE_EVENT_TABLE()
};
