#include <header.h>
#include "JxrOption.h"

#ifndef WX_PRECOMP
//(*InternalHeadersPCH(JxrOption)
//*)
#endif
//(*InternalHeaders(JxrOption)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(JxrOption)
//*)

BEGIN_EVENT_TABLE(JxrOption, wxDialog)
	//(*EventTable(JxrOption)
	//*)
END_EVENT_TABLE()

JxrOption::JxrOption(wxWindow* parent)
{
	isOk = false;
	compressOption = 0;
	compressLevel = 0;
	//(*Initialize(JxrOption)
	wxXmlResource::Get()->LoadObject(this, parent, _T("JxrOption"), _T("wxDialog"));
	rbCodec = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBCODEC")));
	btnOK = static_cast<wxButton*>(FindWindow(XRCID("ID_OK")));
	btnCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_CANCEL")));
	sbCompression = static_cast<wxStaticBox*>(FindWindow(XRCID("ID_SBCOMPRESSION")));
	slCompression = static_cast<wxSlider*>(FindWindow(XRCID("ID_SLCOMPRESSION")));

	Connect(XRCID("ID_OK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&JxrOption::OnbtnOKClick);
	Connect(XRCID("ID_CANCEL"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&JxrOption::OnbtnCancelClick);
	Connect(
		XRCID("ID_SLCOMPRESSION"),
		wxEVT_SCROLL_TOP | wxEVT_SCROLL_BOTTOM | wxEVT_SCROLL_LINEUP | wxEVT_SCROLL_LINEDOWN | wxEVT_SCROLL_PAGEUP |
		wxEVT_SCROLL_PAGEDOWN | wxEVT_SCROLL_THUMBTRACK | wxEVT_SCROLL_THUMBRELEASE | wxEVT_SCROLL_CHANGED,
		(wxObjectEventFunction)&JxrOption::OnslCompressionCmdScroll);
	//*)

	wxTopLevelWindowMSW::SetTitle("Jxr Saving Option");
}

JxrOption::~JxrOption()
{
	//(*Destroy(JxrOption)
	//*)
}

bool JxrOption::IsOk()
{
	return isOk;
}

int JxrOption::CompressionOption()
{
	return compressOption;
}

int JxrOption::CompressionLevel()
{
	return compressLevel;
}

void JxrOption::OnbtnOKClick(wxCommandEvent& event)
{
	isOk = true;
	compressOption = rbCodec->GetSelection();
	compressLevel = slCompression->GetValue();
	this->Close();
}

void JxrOption::OnbtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}

void JxrOption::OnslCompressionCmdScroll(wxScrollEvent& event)
{
}
