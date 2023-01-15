#ifndef INDEXGENERATOR_H
#define INDEXGENERATOR_H

//(*Headers(IndexGenerator)
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statbmp.h>
#include <wx/dialog.h>
//*)


class IndexGenerator : public wxDialog
{
public:
	IndexGenerator(wxWindow* parent);
	~IndexGenerator() override;

	//(*Declarations(IndexGenerator)
	wxButton* btOK;
	wxStaticText* StaticText2;
	wxStaticBitmap* StaticBitmap1;
	wxSpinCtrl* spPictureLine;
	wxStaticText* StaticText1;
	wxStaticText* StaticText3;
	wxStaticBox* StaticBox1;
	wxTextCtrl* txtWidth;
	wxButton* btCancel;
	wxButton* btColor;
	wxTextCtrl* txtHeight;
	wxStaticText* StaticText4;
	wxTextCtrl* txtTitle;

	void SetThumbnailSize(const int& width, const int& height);
	bool IsOk();
	wxColour GetColor();
	int GetThumbnailSize(int& width, int& height);
	int GetNbPictureByLine();
	wxString GetIndexTitle();
	//*)

protected:
	//(*Identifiers(IndexGenerator)
	//*)

private:
	//(*Handlers(IndexGenerator)
	//*)
	bool isOk;
	void OnbtOkClick(wxCommandEvent& event);
	void OnbtCancelClick(wxCommandEvent& event);
	void OnbtColorClick(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()

	wxColour colour;
};

#endif
