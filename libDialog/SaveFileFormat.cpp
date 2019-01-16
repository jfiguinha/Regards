#include "wx_pch.h"
#include "SaveFileFormat.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(SaveFileFormat)
	//*)
#endif
//(*InternalHeaders(SaveFileFormat)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(SaveFileFormat)
//*)

BEGIN_EVENT_TABLE(SaveFileFormat,wxDialog)
	//(*EventTable(SaveFileFormat)
	//*)
END_EVENT_TABLE()

SaveFileFormat::SaveFileFormat(wxWindow* parent)
{
	isOk = false;
	//(*Initialize(SaveFileFormat)
	wxXmlResource::Get()->LoadObject(this,parent,_T("SaveFileFormat"),_T("wxDialog"));
	rbSelectFormat = (wxRadioBox*)FindWindow(XRCID("ID_RBSELECTFORMAT"));
	btnOK = (wxButton*)FindWindow(XRCID("ID_BTNOK"));
	btnCancel = (wxButton*)FindWindow(XRCID("ID_BTNCANCEL"));

	Connect(XRCID("ID_BTNOK"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SaveFileFormat::OnbtnOKClick);
	Connect(XRCID("ID_BTNCANCEL"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SaveFileFormat::OnbtnCancelClick);
	//*)
}

SaveFileFormat::~SaveFileFormat()
{
	//(*Destroy(SaveFileFormat)
	//*)
}

bool SaveFileFormat::IsOk()
{
    return isOk;
}

int SaveFileFormat::GetSaveFormat()
{
    return fileFormat;
}

void SaveFileFormat::OnbtnOKClick(wxCommandEvent& event)
{
    isOk = true;
    fileFormat = rbSelectFormat->GetSelection();
    this->Close();
}

void SaveFileFormat::OnbtnCancelClick(wxCommandEvent& event)
{
    isOk = false;
    this->Close();
}
