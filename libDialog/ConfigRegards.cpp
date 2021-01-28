#include <header.h>
#include "ConfigRegards.h"
#include <RegardsConfigParam.h>
#include <LibResource.h>
#include <ParamInit.h>

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
	sbDiaporama = (wxStaticBox*)FindWindow(XRCID("ID_SBDIAPORAMA"));
	btOk = (wxButton*)FindWindow(XRCID("ID_OK"));
	scTime = (wxSpinCtrl*)FindWindow(XRCID("ID_SCTIME"));
	stTime = (wxStaticText*)FindWindow(XRCID("ID_STTIME"));
    scProcessExif = (wxSpinCtrl*)FindWindow(XRCID("ID_SCEXIF"));
	scProcessFace = (wxSpinCtrl*)FindWindow(XRCID("ID_SCFACE"));
    scProcessThumbnail= (wxSpinCtrl*)FindWindow(XRCID("ID_SCTHUMBNAIL"));
	btCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));
	sbThumbnail = (wxStaticBox*)FindWindow(XRCID("ID_STATICBOX2"));
	//rdDxva2Render = (wxRadioBox*)FindWindow(XRCID("ID_RBDXVA2"));
#ifdef WIN32
	ckDxva2Opengl = (wxCheckBox*)FindWindow(XRCID("ID_CKDXVA2OPENGL"));
	ckDxva2acc = (wxCheckBox*)FindWindow(XRCID("ID_CKDXVA2ACCELERATOR"));
#endif
	rbDatabaseInMemory = (wxRadioBox*)FindWindow(XRCID("ID_RBDATAINMEMORY"));
	rbAutoRotate = (wxRadioBox*)FindWindow(XRCID("ID_RBROTATEAUTO"));
	rbInterpolation = (wxComboBox*)FindWindow(XRCID("ID_CBINTERPOLATIONFILTER"));
	rbContrastCorrection = (wxRadioBox*)FindWindow(XRCID("ID_RBAUTOCONTRAST"));
	Connect(XRCID("ID_OK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ConfigRegards::OnbtnOkClick);
	Connect(XRCID("ID_CANCEL"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ConfigRegards::OnBtnCancelClick);
	//*)
    
    Init();
    
    SetAutoLayout( TRUE );
}

ConfigRegards::~ConfigRegards()
{
	//(*Destroy(ConfigRegards)
	//*)
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

	int timeDiaporama = regardsParam->GetDiaporamaTime();
	scTime->SetValue(timeDiaporama);
    
    int thumbnailProcess = regardsParam->GetThumbnailProcess();
    scProcessThumbnail->SetValue(thumbnailProcess);
       
    int exifProcess = regardsParam->GetExifProcess();
    scProcessExif->SetValue(exifProcess);

#ifdef WIN32
	bool dxva2Use = regardsParam->GetDxva2Actif();
	ckDxva2Opengl->SetValue(dxva2Use);
	wxString decoder = regardsParam->GetVideoDecoderHardware();
	if(decoder == "dxva2")
		ckDxva2acc->SetValue(1);
	else
		ckDxva2acc->SetValue(0);
#endif
	int dataInMemory = regardsParam->GetDatabaseInMemory();
	if (dataInMemory == 0)
		rbDatabaseInMemory->SetSelection(1);
	else
		rbDatabaseInMemory->SetSelection(0);

	int interpolation = regardsParam->GetInterpolationType();
	rbInterpolation->SetSelection(interpolation);
}

void ConfigRegards::OnbtnOkClick(wxCommandEvent& event)
{
	isOk = true;
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
    
    int nbProcesseur = thread::hardware_concurrency();

	int transition = rbTransitionEffect->GetSelection();
	if (transition == 0)
		regardsParam->SetEffect(0);
	else
		regardsParam->SetEffect(transition + 300);

	int autoRotate = rbAutoRotate->GetSelection();
	if (autoRotate == 0)
		regardsParam->SetDectectOrientation(1);
	else
		regardsParam->SetDectectOrientation(0);

	int autoContrast = rbContrastCorrection->GetSelection();
	if (autoContrast == 0)
		regardsParam->SetAutoConstrast(1);
	else
		regardsParam->SetAutoConstrast(0);

#ifdef WIN32
	regardsParam->SetDxva2Actif(ckDxva2Opengl->IsChecked());
	bool ckAcc = ckDxva2acc->IsChecked();
	if(ckAcc)
		regardsParam->SetVideoDecoderHardware("dxva2");
	else
		regardsParam->SetVideoDecoderHardware("");
#endif

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