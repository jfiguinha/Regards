#include "GifOption.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(GifOption)
	//*)
#endif
//(*InternalHeaders(GifOption)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(GifOption)
//*)

BEGIN_EVENT_TABLE(GifOption,wxDialog)
	//(*EventTable(GifOption)
	//*)
END_EVENT_TABLE()

GifOption::GifOption(wxWindow* parent)
{
	isOk = false;
    compressOption = 0;
	//(*Initialize(GifOption)
	wxXmlResource::Get()->LoadObject(this,parent,_T("GifOption"),_T("wxDialog"));
	rbCompression = (wxRadioBox*)FindWindow(XRCID("ID_GIFCOMPRESSION"));
	btnOk = (wxButton*)FindWindow(XRCID("ID_OK"));
	btnCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));

	Connect(XRCID("ID_GIFCOMPRESSION"),wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&GifOption::OnRadioBox1Select);
	Connect(XRCID("ID_OK"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GifOption::OnbtnOkClick);
	Connect(XRCID("ID_CANCEL"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GifOption::OnbtnCancelClick);
	//*)
    
    SetTitle("Gif Saving Option");
}

GifOption::~GifOption()
{
	//(*Destroy(GifOption)
	//*)
}

bool GifOption::IsOk()
{
    return isOk;
}
int GifOption::CompressionOption()
{
    return compressOption;
}

void GifOption::OnRadioBox1Select(wxCommandEvent& event)
{
}

void GifOption::OnbtnOkClick(wxCommandEvent& event)
{
    isOk = true;
    compressOption = rbCompression->GetSelection();
    this->Close();
}

void GifOption::OnInit(wxInitDialogEvent& event)
{
}

void GifOption::OnbtnCancelClick(wxCommandEvent& event)
{
    isOk = false;
    this->Close();
}
