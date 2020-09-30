#include <header.h>
#include "JpegOption.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(JpegOption)
	//*)
#endif
//(*InternalHeaders(JpegOption)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(JpegOption)
//*)

BEGIN_EVENT_TABLE(JpegOption,wxDialog)
	//(*EventTable(JpegOption)
	//*)
END_EVENT_TABLE()

JpegOption::JpegOption(wxWindow* parent)
{
	isOk = false;
    compressOption = 0;
    compressLevel = 0;
	//(*Initialize(JpegOption)
	wxXmlResource::Get()->LoadObject(this,parent,_T("JpegOption"),_T("wxDialog"));
	rbCodec = (wxRadioBox*)FindWindow(XRCID("ID_RBCODEC"));
	btnOK = (wxButton*)FindWindow(XRCID("ID_OK"));
	btnCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));
	sbCompression = (wxStaticBox*)FindWindow(XRCID("ID_SBCOMPRESSION"));
	slCompression = (wxSlider*)FindWindow(XRCID("ID_SLCOMPRESSION"));

	Connect(XRCID("ID_OK"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&JpegOption::OnbtnOKClick);
	Connect(XRCID("ID_CANCEL"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&JpegOption::OnbtnCancelClick);
	Connect(XRCID("ID_SLCOMPRESSION"),wxEVT_SCROLL_TOP|wxEVT_SCROLL_BOTTOM|wxEVT_SCROLL_LINEUP|wxEVT_SCROLL_LINEDOWN|wxEVT_SCROLL_PAGEUP|wxEVT_SCROLL_PAGEDOWN|wxEVT_SCROLL_THUMBTRACK|wxEVT_SCROLL_THUMBRELEASE|wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&JpegOption::OnslCompressionCmdScroll);
	//*)
    
    SetTitle("Jpeg Saving Option");
}

JpegOption::~JpegOption()
{
	//(*Destroy(JpegOption)
	//*)
}

bool JpegOption::IsOk()
{
    return isOk;
}
int JpegOption::CompressionOption()
{
    return compressOption;
}
int JpegOption::CompressionLevel()
{
    return compressLevel;
}

void JpegOption::OnbtnOKClick(wxCommandEvent& event)
{
    isOk = true;
    compressOption = rbCodec->GetSelection();
    if(compressOption == 0)
        compressOption = 0x1;
    else if(compressOption == 1)
        compressOption = 0x2;
    else if(compressOption == 2)
        compressOption = 0x4;
    else if(compressOption == 3)
        compressOption = 0x8;
    else if(compressOption == 4)
        compressOption = 0x10;
    else if(compressOption == 5)
        compressOption = 0x20;
    else if(compressOption == 6)
        compressOption = 0x40;
    else if(compressOption == 7)
        compressOption = 0x1000;
    else if(compressOption == 8)
        compressOption = 0x2000;
    
    
    compressLevel = slCompression->GetValue();
    this->Close();
}

void JpegOption::OnbtnCancelClick(wxCommandEvent& event)
{
    isOk = false;
    this->Close();
}

void JpegOption::OnslCompressionCmdScroll(wxScrollEvent& event)
{
}
