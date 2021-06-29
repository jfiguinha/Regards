#include <header.h>
#include "TiffOption.h"

#ifndef WX_PRECOMP
//(*InternalHeadersPCH(TiffOption)
//*)
#endif
//(*InternalHeaders(TiffOption)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(TiffOption)
//*)

BEGIN_EVENT_TABLE(TiffOption, wxDialog)
	//(*EventTable(TiffOption)
	//*)
END_EVENT_TABLE()

TiffOption::TiffOption(wxWindow* parent)
{
	compressOption = 0;
	isOk = false;
	//(*Initialize(TiffOption)
	wxXmlResource::Get()->LoadObject(this, parent,_T("TiffOption"),_T("wxDialog"));
	rbCompression = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBCOMPRESSION")));
	btnOK = static_cast<wxButton*>(FindWindow(XRCID("ID_OK")));
	btnCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_CANCEL")));

	Connect(XRCID("ID_OK"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&TiffOption::OnbtnOKClick);
	Connect(XRCID("ID_CANCEL"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&TiffOption::OnbtnCancelClick);

	//*)

	SetTitle("Tiff Saving Option");
}

TiffOption::~TiffOption()
{
	//(*Destroy(TiffOption)
	//*)
}

bool TiffOption::IsOk()
{
	return isOk;
}

int TiffOption::CompressionOption()
{
	return compressOption;
}


void TiffOption::OnslCompressLevelCmdScroll(wxScrollEvent& event)
{
}

void TiffOption::OnbtnOKClick(wxCommandEvent& event)
{
	isOk = true;
	compressOption = rbCompression->GetSelection();
	if (compressOption == 0)
		compressOption = 1;
	else if (compressOption == 1)
		compressOption = 2;
	else if (compressOption == 2)
		compressOption = 5;

	//compressLevel = slCompressLevel->GetValue();
	this->Close();
}

void TiffOption::OnbtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}
