#include <header.h>
#include "OpenCLDialog.h"
#include <OpenCLEngine.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
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
	isOk = false;
	//(*Initialize(OpenCLDialog)
	wxXmlResource::Get()->LoadObject(this,parent,_T("OpenCLDialog"),_T("wxDialog"));
	cbOpenCLPlatform = (wxComboBox*)FindWindow(XRCID("ID_COMBOBOX2"));
	cbOpenCLDevice = (wxComboBox*)FindWindow(XRCID("ID_COMBOBOX3"));
	deviceLabel = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
    rbKernelInMemory =  (wxRadioBox*)FindWindow(XRCID("ID_RBKERNELINMEMORY"));
	btnOk = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	BtnCancel = (wxButton*)FindWindow(XRCID("ID_BUTTON2"));

	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OpenCLDialog::OnbtnOkClick);
	Connect(XRCID("ID_BUTTON2"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OpenCLDialog::OnBtnCancelClick);
	Connect(XRCID("ID_COMBOBOX2"), wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction)&OpenCLDialog::OnPlatformSelected);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&OpenCLDialog::OnInit);
	//*)
}

OpenCLDialog::~OpenCLDialog()
{
	//(*Destroy(OpenCLDialog)
	//*)
}

void OpenCLDialog::OnPlatformSelected(wxCommandEvent& event)
{
	wxString device = "";
	OpenCLPlatform * openCLPlatformSelected = nullptr;
	wxString selectItem = cbOpenCLPlatform->GetStringSelection();
	vector<OpenCLPlatform *> listPlatform = COpenCLPlatformList::GetPlatform();

	for (OpenCLPlatform * openCLPlatform : listPlatform)
	{
		if (selectItem == openCLPlatform->platformName)
		{
			openCLPlatformSelected = openCLPlatform;
			break;
		}
	}

	int i = 0;
	cbOpenCLDevice->Clear();
		
	vector<OpenCLDevice *> listDevice = COpenCLDeviceList::GetPlatformDevice(openCLPlatformSelected);

	for (OpenCLDevice * openCLDevice : listDevice)
	{
		cbOpenCLDevice->Append(openCLDevice->deviceName);
		if (i == 0)
		{
			device = openCLDevice->deviceName;
			i++;
		}
	}

	cbOpenCLDevice->SetStringSelection(device);
}

void OpenCLDialog::OnInit(wxInitDialogEvent& event)
{
    bool kernelInMemory = false;
	int supportOpenCL = 0;
    wxString platformName = "";
    wxString deviceName = "";
    int indexDevice  = -1;
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
	{ 
        platformName = config->GetOpenCLPlatformName();
        indexDevice = config->GetOpenCLPlatformIndex();
        kernelInMemory = config->GetOpenCLLoadFromBinaries();
		supportOpenCL = config->GetIsOpenCLSupport();
	}
    
    rbKernelInMemory->SetSelection(kernelInMemory);

	OpenCLPlatform * openCLPlatformSelected = nullptr;
	this->SetTitle("OpenCL Device Selection");
	vector<OpenCLPlatform *> listPlatform = COpenCLPlatformList::GetPlatform();

;
	for (OpenCLPlatform * openCLPlatform : listPlatform)
	{

		cbOpenCLPlatform->Append(openCLPlatform->platformName);
		if (platformName == openCLPlatform->platformName)
		{
			openCLPlatformSelected = openCLPlatform;
		}
	}

	cbOpenCLPlatform->SetStringSelection(platformName);
    
    if(openCLPlatformSelected != nullptr)
    {
        vector<OpenCLDevice *> listDevice = COpenCLDeviceList::GetPlatformDevice(openCLPlatformSelected);

        printf("Select Device Index : %d \n", indexDevice);
        for (OpenCLDevice * openCLDevice : listDevice)
        {
            cbOpenCLDevice->Append(openCLDevice->deviceName);
            if (openCLDevice->deviceIndex == indexDevice)
            {
                deviceName = openCLDevice->deviceName;
            }
        }

        cbOpenCLDevice->SetStringSelection(deviceName);        
    }
    

}

bool OpenCLDialog::IsOk()
{
	return isOk;
}

int OpenCLDialog::GetDeviceIndex()
{
	wxString selectItem = cbOpenCLDevice->GetStringSelection();
	OpenCLDevice * openCLDevice = COpenCLDeviceList::SelectDevice(selectItem);
    if (openCLDevice != nullptr)
        return openCLDevice->deviceIndex;
	return 0;
}

wxString OpenCLDialog::GetPlatformName()
{
	wxString selectItem = cbOpenCLPlatform->GetStringSelection();    
    return selectItem;
}

void OpenCLDialog::OnbtnOkClick(wxCommandEvent& event)
{
	isOk = true;
    int kernelInMemory = rbKernelInMemory->GetSelection();
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
	{ 
         config->SetOpenCLLoadFromBinaries(kernelInMemory);
	}
	this->Close();
}

void OpenCLDialog::OnBtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}
