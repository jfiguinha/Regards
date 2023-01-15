#ifndef COMPRESSIONOPTION_H
#define COMPRESSIONOPTION_H

#ifndef WX_PRECOMP
//(*HeadersPCH(CompressionOption)
#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
#endif
//(*Headers(CompressionOption)
//*)

class CompressionOption : public wxDialog
{
public:
	CompressionOption(wxWindow* parent);
	~CompressionOption() override;

	bool IsOk();
	int CompressionLevel();

	//(*Declarations(CompressionOption)
	wxButton* btOK;
	wxStaticText* StaticText2;
	wxStaticText* StaticText1;
	wxSlider* slCompression;
	wxStaticBox* StaticBox1;
	wxButton* btCancel;
	//*)

protected:
	//(*Identifiers(CompressionOption)
	//*)

private:
	bool isOk;
	int compressLevel;

	//(*Handlers(CompressionOption)
	void OnButton1Click(wxCommandEvent& event);
	void OnButton2Click(wxCommandEvent& event);
	//*)

	DECLARE_EVENT_TABLE()
};

#endif
