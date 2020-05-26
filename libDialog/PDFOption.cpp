#include <header.h>
#include "PDFOption.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(PDFOption)
	//*)
#endif
//(*InternalHeaders(PDFOption)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(PDFOption)
//*)

BEGIN_EVENT_TABLE(PDFOption,wxDialog)
	//(*EventTable(PDFOption)
	//*)
END_EVENT_TABLE()

PDFOption::PDFOption(wxWindow* parent)
{
	compressOption = 0;
    isOk = false;
	//(*Initialize(PDFOption)
	wxXmlResource::Get()->LoadObject(this,parent,_T("PDFOption"),_T("wxDialog"));
	rbCompression = (wxRadioBox*)FindWindow(XRCID("ID_RBCOMPRESSION"));
	btnOK = (wxButton*)FindWindow(XRCID("ID_OK"));
	btnCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));

	Connect(XRCID("ID_OK"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PDFOption::OnbtnOKClick);
	Connect(XRCID("ID_CANCEL"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PDFOption::OnbtnCancelClick);

	//*)
    
    SetTitle("Tiff Saving Option");
}

PDFOption::~PDFOption()
{
	//(*Destroy(PDFOption)
	//*)
}

bool PDFOption::IsOk()
{
    return isOk;
}
int PDFOption::CompressionOption()
{
    return compressOption;
}


void PDFOption::OnslCompressLevelCmdScroll(wxScrollEvent& event)
{
}

void PDFOption::OnbtnOKClick(wxCommandEvent& event)
{
    isOk = true;
    compressOption = rbCompression->GetSelection();   
    //compressLevel = slCompressLevel->GetValue();
    this->Close();
}

void PDFOption::OnbtnCancelClick(wxCommandEvent& event)
{
    isOk = false;
    this->Close();
}
