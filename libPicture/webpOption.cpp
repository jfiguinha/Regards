#include <header.h>
#include "webpOption.h"

#ifndef WX_PRECOMP
//(*InternalHeadersPCH(WebpOption)
//*)
#endif
//(*InternalHeaders(WebpOption)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(WebpOption)
//*)

BEGIN_EVENT_TABLE(WebpOption, wxDialog)
	//(*EventTable(WebpOption)
	//*)
END_EVENT_TABLE()

WebpOption::WebpOption(wxWindow* parent)
{
	isOk = false;
	compressOption = 0;
	compressLevel = 0;
	//(*Initialize(WebpOption)
	wxXmlResource::Get()->LoadObject(this, parent, _T("WebpOption"), _T("wxDialog"));
	rbCodec = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBCODEC")));
	btnOK = static_cast<wxButton*>(FindWindow(XRCID("ID_OK")));
	btnCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_CANCEL")));
	sbCompression = static_cast<wxStaticBox*>(FindWindow(XRCID("ID_SBCOMPRESSION")));
	slCompression = static_cast<wxSlider*>(FindWindow(XRCID("ID_SLCOMPRESSION")));

	Connect(XRCID("ID_OK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&WebpOption::OnbtnOKClick);
	Connect(XRCID("ID_CANCEL"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&WebpOption::OnbtnCancelClick);
	Connect(
		XRCID("ID_SLCOMPRESSION"),
		wxEVT_SCROLL_TOP | wxEVT_SCROLL_BOTTOM | wxEVT_SCROLL_LINEUP | wxEVT_SCROLL_LINEDOWN | wxEVT_SCROLL_PAGEUP |
		wxEVT_SCROLL_PAGEDOWN | wxEVT_SCROLL_THUMBTRACK | wxEVT_SCROLL_THUMBRELEASE | wxEVT_SCROLL_CHANGED,
		(wxObjectEventFunction)&WebpOption::OnslCompressionCmdScroll);
	//*)
	slCompression->SetValue(70);
	SetTitle("Webp Saving Option");
}

WebpOption::~WebpOption()
{
	//(*Destroy(WebpOption)
	//*)
}

bool WebpOption::IsOk()
{
	return isOk;
}

int WebpOption::CompressionOption()
{
	return compressOption;
}

int WebpOption::CompressionLevel()
{
	return compressLevel;
}

void WebpOption::OnbtnOKClick(wxCommandEvent& event)
{
	isOk = true;
	compressOption = rbCodec->GetSelection();
	compressLevel = slCompression->GetValue();
	this->Close();
}

void WebpOption::OnbtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}

void WebpOption::OnslCompressionCmdScroll(wxScrollEvent& event)
{
}
