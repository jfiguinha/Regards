#include <header.h>
#include "PngOption.h"

#ifndef WX_PRECOMP
//(*InternalHeadersPCH(PngOption)
//*)
#endif
//(*InternalHeaders(PngOption)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(PngOption)
//*)

BEGIN_EVENT_TABLE(PngOption, wxDialog)
	//(*EventTable(PngOption)
	//*)
END_EVENT_TABLE()

PngOption::PngOption(wxWindow* parent)
{
	isOk = false;
	compressOption = 0;
	//(*Initialize(PngOption)
	wxXmlResource::Get()->LoadObject(this, parent,_T("PngOption"),_T("wxDialog"));
	rbInterlace = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBINTERLACE")));
	rbCompression = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBCOMPRESSION")));
	btnOK = static_cast<wxButton*>(FindWindow(XRCID("ID_OK")));
	btnCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_CANCEL")));

	Connect(XRCID("ID_OK"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&PngOption::OnButton1Click);
	Connect(XRCID("ID_CANCEL"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&PngOption::OnbtnCancelClick);
	//*)

	SetTitle("Png Saving Option");
}

PngOption::~PngOption()
{
	//(*Destroy(PngOption)
	//*)
}

bool PngOption::IsOk()
{
	return isOk;
}

int PngOption::CompressionOption()
{
	return compressOption;
}

void PngOption::OnInit(wxInitDialogEvent& event)
{
}

void PngOption::OnButton1Click(wxCommandEvent& event)
{
	isOk = true;
	int compressSelection = rbCompression->GetSelection();
	int interlaceSelection = rbInterlace->GetSelection();

	if (compressSelection == 0)
		compressSelection = 2;
	else if (compressSelection == 1)
		compressSelection = 4;
	else if (compressSelection == 2)
		compressSelection = 6;
	else if (compressSelection == 3)
		compressSelection = 8;

	compressOption = compressSelection + interlaceSelection;
	this->Close();
}

void PngOption::OnbtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}
