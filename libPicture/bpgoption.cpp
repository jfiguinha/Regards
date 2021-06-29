#include <header.h>
#include "bpgoption.h"

#ifndef WX_PRECOMP
//(*InternalHeadersPCH(CBpgOption)
//*)
#endif
//(*InternalHeaders(CBpgOption)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(CBpgOption)
//*)

BEGIN_EVENT_TABLE(CBpgOption, wxDialog)
	//(*EventTable(CBpgOption)
	//*)
END_EVENT_TABLE()

CBpgOption::CBpgOption(wxWindow* parent)
{
	isOk = false;
	compressOption = 0;
	compressLevel = 0;
	//(*Initialize(CBpgOption)
	wxXmlResource::Get()->LoadObject(this, parent,_T("bpgoption"),_T("wxDialog"));
	btnOK = static_cast<wxButton*>(FindWindow(XRCID("ID_OK")));
	btnCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_CANCEL")));
	sbCompression = static_cast<wxStaticBox*>(FindWindow(XRCID("ID_SBCOMPRESSION")));
	slCompression = static_cast<wxSlider*>(FindWindow(XRCID("ID_SLCOMPRESSION")));
	rbCompression = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBCOMPRESSION")));
	Connect(XRCID("ID_OK"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CBpgOption::OnbtnOKClick);
	Connect(XRCID("ID_CANCEL"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CBpgOption::OnbtnCancelClick);
	Connect(
		XRCID("ID_SLCOMPRESSION"),
		wxEVT_SCROLL_TOP | wxEVT_SCROLL_BOTTOM | wxEVT_SCROLL_LINEUP | wxEVT_SCROLL_LINEDOWN | wxEVT_SCROLL_PAGEUP |
		wxEVT_SCROLL_PAGEDOWN | wxEVT_SCROLL_THUMBTRACK | wxEVT_SCROLL_THUMBRELEASE | wxEVT_SCROLL_CHANGED,
		(wxObjectEventFunction)&CBpgOption::OnslCompressionCmdScroll);
	//*)

	slCompression->SetRange(0, 10);
	slCompression->SetTick(1);
	slCompression->SetValue(8);
	SetTitle("bpg Saving Option");
}

CBpgOption::~CBpgOption()
{
	//(*Destroy(CBpgOption)
	//*)
}

bool CBpgOption::IsOk()
{
	return isOk;
}

int CBpgOption::CompressionOption()
{
	return compressOption;
}

int CBpgOption::CompressionLevel()
{
	return compressLevel;
}

void CBpgOption::OnbtnOKClick(wxCommandEvent& event)
{
	isOk = true;
	compressOption = rbCompression->GetSelection();
	compressLevel = slCompression->GetValue();
	this->Close();
}

void CBpgOption::OnbtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}

void CBpgOption::OnslCompressionCmdScroll(wxScrollEvent& event)
{
}
