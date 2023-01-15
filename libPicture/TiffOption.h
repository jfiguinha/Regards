#ifndef TIFFOPTION_H
#define TIFFOPTION_H


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

class TiffOption : public wxDialog
{
public:
	TiffOption(wxWindow* parent);
	~TiffOption() override;

	//(*Declarations(TiffOption)
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
	//(*Identifiers(TiffOption)
	//*)

private:
	//(*Handlers(TiffOption)
	void OnslCompressLevelCmdScroll(wxScrollEvent& event);
	void OnbtnOKClick(wxCommandEvent& event);
	void OnbtnCancelClick(wxCommandEvent& event);
	//*)

	int compressOption;
	bool isOk;
	DECLARE_EVENT_TABLE()
};

#endif
