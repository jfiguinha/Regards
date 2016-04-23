#include "wx_pch.h"
#include "OpenCLDialog.h"
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(OpenCLDialog)
	//*)
#endif
//(*InternalHeaders(OpenCLDialog)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(OpenCLDialog)
//*)

BEGIN_EVENT_TABLE(OpenCLDialog,wxDialog)
	//(*EventTable(OpenCLDialog)
	//*)
END_EVENT_TABLE()

OpenCLDialog::OpenCLDialog(wxWindow* parent)
{
	//(*Initialize(OpenCLDialog)
	wxXmlResource::Get()->LoadObject(this,parent,_T("OpenCLDialog"),_T("wxDialog"));
	cbOpenCLDevice = (wxComboBox*)FindWindow(XRCID("ID_COMBOBOX2"));
	deviceLabel = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	btnOk = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	BtnCancel = (wxButton*)FindWindow(XRCID("ID_BUTTON2"));

	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OpenCLDialog::OnbtnOkClick);
	Connect(XRCID("ID_BUTTON2"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OpenCLDialog::OnBtnCancelClick);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&OpenCLDialog::OnInit);
	//*)
}

OpenCLDialog::~OpenCLDialog()
{
	//(*Destroy(OpenCLDialog)
	//*)
}


void OpenCLDialog::OnInit(wxInitDialogEvent& event)
{
	this->SetTitle("OpenCL Device Selection");
	vector<COpenCLDeviceList> listDevice = COpenCLEngine::GetListOfContext();
	for (COpenCLDeviceList openCLDevice : listDevice)
		cbOpenCLDevice->Append(openCLDevice.platformName);

	cbOpenCLDevice->SetStringSelection(listDevice[0].platformName);
}

bool OpenCLDialog::IsOk()
{
	return isOk;
}

wxString OpenCLDialog::GetSelectItem()
{
	return cbOpenCLDevice->GetStringSelection();
}

int OpenCLDialog::GetIndexItem()
{
	wxString selectItem = cbOpenCLDevice->GetStringSelection();
	vector<COpenCLDeviceList> listDevice = COpenCLEngine::GetListOfContext();
	for (COpenCLDeviceList openCLDevice : listDevice)
	{
		if (selectItem == openCLDevice.platformName)
			return openCLDevice.platformIndex;
	}
    
    return 0;
}

void OpenCLDialog::OnbtnOkClick(wxCommandEvent& event)
{
	isOk = true;
	this->Close();
}

void OpenCLDialog::OnBtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}
