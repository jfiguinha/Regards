#include <header.h>
#include "ConfigRegards.h"
#include <RegardsConfigParam.h>
#include <LibResource.h>
#include <ParamInit.h>
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <OpenCLEngine.h>
using namespace Regards::Viewer;
using namespace Regards::OpenCL;
#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(ConfigRegards)
	//*)
#endif
//(*InternalHeaders(ConfigRegards)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(ConfigRegards)
//*)

BEGIN_EVENT_TABLE(ConfigRegards,wxDialog)
	//(*EventTable(ConfigRegards)
	//*)
END_EVENT_TABLE()

ConfigRegards::ConfigRegards(wxWindow* parent)
{
	isOk = false;
	//(*Initialize(ConfigRegards)
	SetTitle("Regards Configuration");
	wxXmlResource::Get()->LoadObject(this,parent,_T("ConfigRegards"),_T("wxDialog"));
	rbTransitionEffect = (wxComboBox*)FindWindow(XRCID("ID_RBTRANSITIONEFFECT"));
	rbTransitionDiaporamaEffect = (wxComboBox*)FindWindow(XRCID("ID_RBDIAPORAMATRANSITIONEFFECT"));
	sbDiaporama = (wxStaticBox*)FindWindow(XRCID("ID_SBDIAPORAMA"));
	btOk = (wxButton*)FindWindow(XRCID("ID_OK"));
	scTime = (wxSpinCtrl*)FindWindow(XRCID("ID_SCTIME"));
	stTime = (wxStaticText*)FindWindow(XRCID("ID_STTIME"));
    scProcessExif = (wxSpinCtrl*)FindWindow(XRCID("ID_SCEXIF"));
	scProcessFace = (wxSpinCtrl*)FindWindow(XRCID("ID_SCFACE"));
    scProcessThumbnail= (wxSpinCtrl*)FindWindow(XRCID("ID_SCTHUMBNAIL"));
	btCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));
	sbThumbnail = (wxStaticBox*)FindWindow(XRCID("ID_STATICBOX2"));
	//ID_RBVIDEOFACEDETECTION
	rbVideoFaceDetection = (wxRadioBox*)FindWindow(XRCID("ID_RBVIDEOFACEDETECTION"));
	rbFaceDetection = (wxRadioBox*)FindWindow(XRCID("ID_RBFACEDETECTION"));
	txtPicturePath = (wxTextCtrl *)FindWindow(XRCID("ID_TXTPICTUREPATH"));
	btPicturePath = (wxButton*)FindWindow(XRCID("ID_PICTUREPATH"));
	txtVideoPath = (wxTextCtrl *)FindWindow(XRCID("ID_TXTVIDEOPATH"));
	btVideoPath = (wxButton*)FindWindow(XRCID("ID_VIDEOPATH"));
	cbOpenCLDevice = (wxComboBox*)FindWindow(XRCID("ID_CBOPENCLDEVICE"));
	cbOpenCLPlatform = (wxComboBox*)FindWindow(XRCID("ID_CBOPENCLPLATFORM"));
	rbKernelInMemory = (wxRadioBox*)FindWindow(XRCID("ID_RBKERNELINMEMORY"));

	rbDatabaseInMemory = (wxRadioBox*)FindWindow(XRCID("ID_RBDATAINMEMORY"));
	rbAutoRotate = (wxRadioBox*)FindWindow(XRCID("ID_RBROTATEAUTO"));
	rbInterpolation = (wxComboBox*)FindWindow(XRCID("ID_CBINTERPOLATIONFILTER"));
	rbContrastCorrection = (wxRadioBox*)FindWindow(XRCID("ID_RBAUTOCONTRAST"));

	Connect(XRCID("ID_OK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ConfigRegards::OnbtnOkClick);
	Connect(XRCID("ID_CANCEL"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ConfigRegards::OnBtnCancelClick);
	//*)
	Connect(XRCID("ID_CBOPENCLPLATFORM"), wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction)&ConfigRegards::OnPlatformSelected);
	//Connect(wxID_ANY, wxEVT_INIT_DIALOG, (wxObjectEventFunction)&ConfigRegards::OnInit);
	Connect(XRCID("ID_VIDEOPATH"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ConfigRegards::OnbtnPathVideoClick);
	Connect(XRCID("ID_PICTUREPATH"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ConfigRegards::OnBtnPathPictureClick);


    Init();

	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		txtVideoPath->SetValue(config->GetPathForVideoEdit());
	if (config != nullptr)
		txtPicturePath->SetValue(config->GetPathForPictureEdit());
    
    SetAutoLayout( TRUE );
}

void ConfigRegards::OnbtnPathVideoClick(wxCommandEvent& event)
{
	wxString label = CLibResource::LoadStringFromResource(L"LBLSELECTVIDEOEDITOR", 1);
	wxString allfiles = CLibResource::LoadStringFromResource(L"LBLALLFILES", 1);

	wxFileDialog openFileDialog(nullptr, label, "", "",
		allfiles, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_OK)
		txtVideoPath->SetValue(openFileDialog.GetPath());
}

void ConfigRegards::OnBtnPathPictureClick(wxCommandEvent& event)
{
	wxString label = CLibResource::LoadStringFromResource(L"LBLSELECTPICTUREEDITOR", 1);
	wxString allfiles = CLibResource::LoadStringFromResource(L"LBLALLFILES", 1);

	wxFileDialog openFileDialog(nullptr, label, "", "",
		allfiles, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_OK)
		txtPicturePath->SetValue(openFileDialog.GetPath());
}

ConfigRegards::~ConfigRegards()
{
	//(*Destroy(ConfigRegards)
	//*)
}

int ConfigRegards::GetDeviceIndex()
{
	wxString selectItem = cbOpenCLDevice->GetStringSelection();
	OpenCLDevice * openCLDevice = COpenCLDeviceList::SelectDevice(selectItem);
	if (openCLDevice != nullptr)
		return openCLDevice->deviceIndex;
	return 0;
}

wxString ConfigRegards::GetPlatformName()
{
	wxString selectItem = cbOpenCLPlatform->GetStringSelection();
	return selectItem;
}

void ConfigRegards::OnPlatformSelected(wxCommandEvent& event)
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

void ConfigRegards::Init()
{
	this->SetTitle("Configuration");
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();

	int transition = max((regardsParam->GetEffect() - 300),0);
	if (transition == 0)
		rbTransitionEffect->SetSelection(0);
	else
		rbTransitionEffect->SetSelection(transition);

	int transitionDiaporama = max((regardsParam->GetDiaporamaTransitionEffect() - 400), 0);
	if (transitionDiaporama == 0)
		rbTransitionDiaporamaEffect->SetSelection(0);
	else
		rbTransitionDiaporamaEffect->SetSelection(transitionDiaporama);

	int autoRotate = regardsParam->GetDetectOrientation();
	if (autoRotate == 0)
		rbAutoRotate->SetSelection(1);
	else
		rbAutoRotate->SetSelection(0);

	int autoContrast = regardsParam->GetAutoConstrast();
	if (autoContrast == 0)
		rbContrastCorrection->SetSelection(1);
	else
		rbContrastCorrection->SetSelection(0);

	int videoFaceDetection = regardsParam->GetFaceVideoDetection();
	if (videoFaceDetection == 0)
		rbVideoFaceDetection->SetSelection(1);
	else
		rbVideoFaceDetection->SetSelection(0);

	int faceDetection = regardsParam->GetFaceDetection();
	if (faceDetection == 0)
		rbFaceDetection->SetSelection(1);
	else
		rbFaceDetection->SetSelection(0);

	int timeDiaporama = regardsParam->GetDiaporamaTime();
	scTime->SetValue(timeDiaporama);
    
    int thumbnailProcess = regardsParam->GetThumbnailProcess();
    scProcessThumbnail->SetValue(thumbnailProcess);
       
    int exifProcess = regardsParam->GetExifProcess();
    scProcessExif->SetValue(exifProcess);

	int faceProcess = regardsParam->GetFaceProcess();
	scProcessFace->SetValue(faceProcess);

	int dataInMemory = regardsParam->GetDatabaseInMemory();
	if (dataInMemory == 0)
		rbDatabaseInMemory->SetSelection(1);
	else
		rbDatabaseInMemory->SetSelection(0);

	int interpolation = regardsParam->GetInterpolationType();
	rbInterpolation->SetSelection(interpolation);

	bool kernelInMemory = false;
	int supportOpenCL = 0;
	wxString platformName = "";
	wxString deviceName = "";
	int indexDevice = -1;
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

	if (openCLPlatformSelected != nullptr)
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

void ConfigRegards::OnbtnOkClick(wxCommandEvent& event)
{
	isOk = true;
	bool showInfosRestart = false;
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
	CMainParam* mainparam = CMainParamInit::getInstance();

	//Get old value to compare with new value
	int _transition = max((regardsParam->GetEffect() - 300), 0);
	int _autoRotate = regardsParam->GetDetectOrientation();
	int _autoContrast = regardsParam->GetAutoConstrast();
	int _videoFaceDetection = regardsParam->GetFaceVideoDetection();
	int _faceDetection = regardsParam->GetFaceDetection();
	int _timeDiaporama = regardsParam->GetDiaporamaTime();
	int _thumbnailProcess = regardsParam->GetThumbnailProcess();
	int _exifProcess = regardsParam->GetExifProcess();
	int _faceProcess = regardsParam->GetFaceProcess();
	int _dataInMemory = regardsParam->GetDatabaseInMemory();
	int _interpolation = regardsParam->GetInterpolationType();

	int _supportOpenCL = 0;
	wxString _platformName = "";
	wxString _deviceName = "";
	int _indexDevice = -1;
	
	if (regardsParam != nullptr)
	{
		_platformName = regardsParam->GetOpenCLPlatformName();
		_indexDevice = regardsParam->GetOpenCLPlatformIndex();
		_supportOpenCL = regardsParam->GetIsOpenCLSupport();
	}

    
    int nbProcesseur = thread::hardware_concurrency();

	int kernelInMemory = rbKernelInMemory->GetSelection();

	if (regardsParam != nullptr)
	{
		regardsParam->SetOpenCLLoadFromBinaries(kernelInMemory);
		regardsParam->SetOpenCLPlatformIndex(GetDeviceIndex());
		regardsParam->SetOpenCLPlatformName(GetPlatformName());

		if (_platformName != GetPlatformName())
			showInfosRestart = true;

		if (_indexDevice != GetDeviceIndex())
			showInfosRestart = true;

		if (regardsParam->GetOpenCLLoadFromBinaries() != kernelInMemory)
			showInfosRestart = true;

	}


	if (mainparam != nullptr)
	{
		mainparam->SetPathForVideoEdit(txtVideoPath->GetValue());
		mainparam->SetPathForPictureEdit(txtPicturePath->GetValue());
	}

	int transition = rbTransitionEffect->GetSelection();
	if (transition == 0)
		regardsParam->SetEffect(0);
	else
		regardsParam->SetEffect(transition + 300);

	int transitionDiaporama = rbTransitionDiaporamaEffect->GetSelection();
	if (transitionDiaporama == 0)
		regardsParam->SetDiaporamaTransitionEffect(0);
	else
		regardsParam->SetDiaporamaTransitionEffect(transitionDiaporama + 400);

	int autoRotate = rbAutoRotate->GetSelection();
	if (autoRotate == 0)
		regardsParam->SetDectectOrientation(1);
	else
		regardsParam->SetDectectOrientation(0);

	int videoFaceDetection = rbVideoFaceDetection->GetSelection();
	if (videoFaceDetection == 0)
		regardsParam->SetFaceVideoDetection(1);
	else
		regardsParam->SetFaceVideoDetection(0);

	int faceDetection = rbFaceDetection->GetSelection();
	if (faceDetection == 0)
		regardsParam->SetFaceDetection(1);
	else
		regardsParam->SetFaceDetection(0);

	if(_faceDetection == faceDetection)
		showInfosRestart = true;

	int autoContrast = rbContrastCorrection->GetSelection();
	if (autoContrast == 0)
		regardsParam->SetAutoConstrast(1);
	else
		regardsParam->SetAutoConstrast(0);

	int interpolation = rbInterpolation->GetSelection();
	regardsParam->SetInterpolationType(interpolation);

	int timeDiaporama = scTime->GetValue();
	regardsParam->SetDiaporamaTime(timeDiaporama);
 
    int thumbnailProcess = scProcessThumbnail->GetValue();
    int faceProcess = scProcessFace->GetValue();
    int exifProcess = scProcessExif->GetValue();
    
    if(thumbnailProcess == 0 || faceProcess == 0 || exifProcess == 0)
    {
        wxString errorProcessNumberMin = CLibResource::LoadStringFromResource(L"ErrorProcessNumberMin",1);
        wxString errorInfo = CLibResource::LoadStringFromResource(L"informationserror",1);
        wxMessageBox(errorProcessNumberMin, errorInfo);
    }
    else if((thumbnailProcess + exifProcess) > nbProcesseur && faceProcess > nbProcesseur && (thumbnailProcess > 1 || faceProcess > 1 || exifProcess > 1))
    {
        wxString errorProcessNumberMax = CLibResource::LoadStringFromResource(L"ErrorProcessNumberMax",1);
        wxString errorInfo = CLibResource::LoadStringFromResource(L"informationserror",1);
        wxMessageBox(errorProcessNumberMax, errorInfo);
    }
    else
    {
        regardsParam->SetFaceProcess(faceProcess);
        regardsParam->SetExifProcess(exifProcess);
        regardsParam->SetThumbnailProcess(thumbnailProcess);

        int dataInMemory = rbDatabaseInMemory->GetSelection();
        if (dataInMemory == 0)
            regardsParam->SetDatabaseInMemory(1);
        else
            regardsParam->SetDatabaseInMemory(0);

		if (showInfosRestart)
		{
			wxString labelRestart = CLibResource::LoadStringFromResource(L"labelRestart", 1);
			wxString lblInfos = CLibResource::LoadStringFromResource(L"labelInformations", 1);
			wxMessageBox(labelRestart, lblInfos);
		}


        this->Close();        
    }

}

void ConfigRegards::OnBtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}

bool ConfigRegards::IsOk()
{
	return isOk;
}