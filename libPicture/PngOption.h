#ifndef PNGOPTION_H
#define PNGOPTION_H

#ifndef WX_PRECOMP
//(*HeadersPCH(PngOption)
#include <wx/radiobox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
#endif
//(*Headers(PngOption)
//*)

class PngOption : public wxDialog
{
public:
	PngOption(wxWindow* parent);
	~PngOption() override;

	//(*Declarations(PngOption)
	wxRadioBox* rbInterlace;
	wxButton* btnCancel;
	wxButton* btnOK;
	wxRadioBox* rbCompression;
	//*)

	bool IsOk();
	int CompressionOption();

protected:
	//(*Identifiers(PngOption)
	//*)

private:
	//(*Handlers(PngOption)
	void OnInit(wxInitDialogEvent& event);
	void OnButton1Click(wxCommandEvent& event);
	void OnbtnCancelClick(wxCommandEvent& event);
	//*)
	bool isOk;
	int compressOption;
	DECLARE_EVENT_TABLE()
};

#endif
