#include <header.h>
#include "ExrOption.h"

#ifndef WX_PRECOMP
//(*InternalHeadersPCH(ExrOption)
//*)
#endif
//(*InternalHeaders(ExrOption)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(ExrOption)
//*)

BEGIN_EVENT_TABLE(ExrOption, wxDialog)
	//(*EventTable(ExrOption)
	//*)
END_EVENT_TABLE()

ExrOption::ExrOption(wxWindow* parent)
{
	compressOption = 0;
	isOk = false;
	//(*Initialize(ExrOption)
	wxXmlResource::Get()->LoadObject(this, parent,_T("ExrOption"),_T("wxDialog"));
	rbCompression = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBCODEC")));
	btnOK = static_cast<wxButton*>(FindWindow(XRCID("ID_OK")));
	btnCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_CANCEL")));

	Connect(XRCID("ID_OK"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ExrOption::OnbtnOKClick);
	Connect(XRCID("ID_CANCEL"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ExrOption::OnbtnCancelClick);

	//*)

	wxTopLevelWindowMSW::SetTitle("Exr Saving Option");
}

ExrOption::~ExrOption()
{
	//(*Destroy(ExrOption)
	//*)
}

bool ExrOption::IsOk()
{
	return isOk;
}

int ExrOption::CompressionOption()
{
	return compressOption;
}

void ExrOption::OnbtnOKClick(wxCommandEvent& event)
{
	isOk = true;
	compressOption = rbCompression->GetSelection();
	this->Close();
}

void ExrOption::OnbtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}
