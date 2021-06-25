#ifndef GIFOPTION_H
#define GIFOPTION_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(GifOption)
	#include <wx/radiobox.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(GifOption)
//*)

class GifOption : public wxDialog
{
public:
	GifOption(wxWindow* parent);
	~GifOption() override;

	//(*Declarations(GifOption)
	wxButton* btnOk;
	wxButton* btnCancel;
	wxRadioBox* rbCompression;
	//*)
	bool IsOk();
	int CompressionOption();

protected:
	//(*Identifiers(GifOption)
	//*)

private:
	//(*Handlers(GifOption)
	void OnRadioBox1Select(wxCommandEvent& event);
	void OnbtnOkClick(wxCommandEvent& event);
	void OnInit(wxInitDialogEvent& event);
	void OnbtnCancelClick(wxCommandEvent& event);
	//*)

	bool isOk;
	int compressOption;
DECLARE_EVENT_TABLE()
};

#endif
