#include <header.h>
#include "BmpOption.h"

#ifndef WX_PRECOMP
//(*InternalHeadersPCH(BmpOption)
//*)
#endif
//(*InternalHeaders(BmpOption)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(BmpOption)
//*)

BEGIN_EVENT_TABLE(BmpOption, wxDialog)
		//(*EventTable(BmpOption)
		//*)
END_EVENT_TABLE()

BmpOption::BmpOption(wxWindow* parent)
{
	compressOption = 0;
	isOk = false;
	//(*Initialize(BmpOption)
	wxXmlResource::Get()->LoadObject(this, parent,_T("BmpOption"),_T("wxDialog"));
	rbCompression = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBCODEC")));
	btnOK = static_cast<wxButton*>(FindWindow(XRCID("ID_OK")));
	btnCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_CANCEL")));

	Connect(XRCID("ID_OK"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&BmpOption::OnbtnOKClick);
	Connect(XRCID("ID_CANCEL"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&BmpOption::OnbtnCancelClick);

	//*)

	SetTitle("Bmp Saving Option");
}

BmpOption::~BmpOption()
{
	//(*Destroy(BmpOption)
	//*)
}

bool BmpOption::IsOk()
{
	return isOk;
}

int BmpOption::CompressionOption()
{
	return compressOption;
}

void BmpOption::OnbtnOKClick(wxCommandEvent& event)
{
	isOk = true;
	compressOption = rbCompression->GetSelection();
	this->Close();
}

void BmpOption::OnbtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}
