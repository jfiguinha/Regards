#include <header.h>
#include "J2kOption.h"

#ifndef WX_PRECOMP
//(*InternalHeadersPCH(J2kOption)
//*)
#endif
//(*InternalHeaders(J2kOption)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(J2kOption)
//*)

BEGIN_EVENT_TABLE(J2kOption, wxDialog)
//(*EventTable(J2kOption)
//*)
END_EVENT_TABLE()

J2kOption::J2kOption(wxWindow* parent)
{
	isOk = false;
	compressOption = 0;
	compressLevel = 0;
	//(*Initialize(J2kOption)
	wxXmlResource::Get()->LoadObject(this, parent, _T("J2kOption"), _T("wxDialog"));
	rbCodec = (wxRadioBox*)FindWindow(XRCID("ID_RBCODEC"));
	btnOK = (wxButton*)FindWindow(XRCID("ID_OK"));
	btnCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));
	sbCompression = (wxStaticBox*)FindWindow(XRCID("ID_SBCOMPRESSION"));
	slCompression = (wxSlider*)FindWindow(XRCID("ID_SLCOMPRESSION"));

	Connect(XRCID("ID_OK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&J2kOption::OnbtnOKClick);
	Connect(XRCID("ID_CANCEL"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&J2kOption::OnbtnCancelClick);
	Connect(XRCID("ID_SLCOMPRESSION"), wxEVT_SCROLL_TOP | wxEVT_SCROLL_BOTTOM | wxEVT_SCROLL_LINEUP | wxEVT_SCROLL_LINEDOWN | wxEVT_SCROLL_PAGEUP | wxEVT_SCROLL_PAGEDOWN | wxEVT_SCROLL_THUMBTRACK | wxEVT_SCROLL_THUMBRELEASE | wxEVT_SCROLL_CHANGED, (wxObjectEventFunction)&J2kOption::OnslCompressionCmdScroll);
	//*)
	slCompression->SetValue(70);
	SetTitle("JPEG2000 Saving Option");
}

J2kOption::~J2kOption()
{
	//(*Destroy(J2kOption)
	//*)
}

bool J2kOption::IsOk()
{
	return isOk;
}
int J2kOption::CompressionOption()
{
	return compressOption;
}
int J2kOption::CompressionLevel()
{
	return compressLevel;
}

void J2kOption::OnbtnOKClick(wxCommandEvent& event)
{
	isOk = true;
	compressOption = rbCodec->GetSelection();
	compressLevel = slCompression->GetValue();
	this->Close();
}

void J2kOption::OnbtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}

void J2kOption::OnslCompressionCmdScroll(wxScrollEvent& event)
{
}
