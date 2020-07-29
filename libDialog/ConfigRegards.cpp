#include <header.h>
#include "ConfigRegards.h"
#include <RegardsConfigParam.h>
#include <LibResource.h>
#include <ParamInit.h>
#include <config_id.h>

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
	//rdPreviewRender = (wxRadioBox*)FindWindow(XRCID("ID_RDPREVIEWRENDER"));
	//rbRenderVideo = (wxRadioBox*)FindWindow(XRCID("ID_RBRENDERVIDEO"));
	//rbEffectLibrary = (wxRadioBox*)FindWindow(XRCID("ID_RBEFFECTLIBRARY"));
	rbTransitionEffect = (wxRadioBox*)FindWindow(XRCID("ID_RBTRANSITIONEFFECT"));
	sbDiaporama = (wxStaticBox*)FindWindow(XRCID("ID_SBDIAPORAMA"));
	btOk = (wxButton*)FindWindow(XRCID("ID_OK"));
	scTime = (wxSpinCtrl*)FindWindow(XRCID("ID_SCTIME"));
	stTime = (wxStaticText*)FindWindow(XRCID("ID_STTIME"));
    scProcessFace = (wxSpinCtrl*)FindWindow(XRCID("ID_SCFACE"));
    scProcessExif = (wxSpinCtrl*)FindWindow(XRCID("ID_SCEXIF"));
    scProcessThumbnail= (wxSpinCtrl*)FindWindow(XRCID("ID_SCTHUMBNAIL"));
	//rbFullscreen = (wxRadioBox*)FindWindow(XRCID("ID_RBFULLSCREEN"));
	//rbDiaporamaTransitionEffect = (wxRadioBox*)FindWindow(XRCID("ID_RBDIAPORAMATRANSITIONEFFECT"));
	btCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));
	sbThumbnail = (wxStaticBox*)FindWindow(XRCID("ID_STATICBOX2"));
	rdDxva2Render = (wxRadioBox*)FindWindow(XRCID("ID_RBDXVA2"));
	rbDatabaseInMemory = (wxRadioBox*)FindWindow(XRCID("ID_RBDATAINMEMORY"));
	rbPictureFaceDetectionSize = (wxRadioBox*)FindWindow(XRCID("ID_PICTUREFACEDETECTION"));
	rbAutoRotate = (wxRadioBox*)FindWindow(XRCID("ID_RBROTATEAUTO"));
	//rbThumbnailQuality = (wxRadioBox*)FindWindow(XRCID("ID_RDTHUMBNAILQUALITY"));
	//rbThumbnailCache = (wxRadioBox*)FindWindow(XRCID("ID_RDTHUMBNAILCACHE"));

	Connect(XRCID("ID_OK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ConfigRegards::OnbtnOkClick);
	Connect(XRCID("ID_CANCEL"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ConfigRegards::OnBtnCancelClick);
	//*)
    
    Init();
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

	int transition = regardsParam->GetEffect();
	if (transition == 0)
		rbTransitionEffect->SetSelection(1);
	else
		rbTransitionEffect->SetSelection(0);

	int autoRotate = regardsParam->GetDetectOrientation();
	if (autoRotate == 0)
		rbAutoRotate->SetSelection(1);
	else
		rbAutoRotate->SetSelection(0);

	int timeDiaporama = regardsParam->GetDiaporamaTime();
	scTime->SetValue(timeDiaporama);
    
    int thumbnailProcess = regardsParam->GetThumbnailProcess();
    scProcessThumbnail->SetValue(thumbnailProcess);
    
    int faceProcess = regardsParam->GetFaceProcess();
    scProcessFace->SetValue(faceProcess);
    
    int exifProcess = regardsParam->GetExifProcess();
    scProcessExif->SetValue(exifProcess);

	bool dxva2Use = regardsParam->GetDxva2Actif();
	if (dxva2Use == 0)
		rdDxva2Render->SetSelection(1);
	else
		rdDxva2Render->SetSelection(0);
/*
	int effectDiaporama = regardsParam->GetDiaporamaTransitionEffect();
	if (effectDiaporama == 0)
		rbDiaporamaTransitionEffect->SetSelection(1);
	else
		rbDiaporamaTransitionEffect->SetSelection(0);

	int fullscreenDiaporama = regardsParam->GetDiaporamaFullscreen();
	if (fullscreenDiaporama == 0)
		rbFullscreen->SetSelection(1);
	else
		rbFullscreen->SetSelection(0);
*/
	int dataInMemory = regardsParam->GetDatabaseInMemory();
	if (dataInMemory == 0)
		rbDatabaseInMemory->SetSelection(1);
	else
		rbDatabaseInMemory->SetSelection(0);

	int pictureSize = regardsParam->GetFaceDetectionPictureSize();
	rbPictureFaceDetectionSize->SetSelection(pictureSize);
}

void ConfigRegards::OnbtnOkClick(wxCommandEvent& event)
{
	isOk = true;
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
    
    int nbProcesseur = thread::hardware_concurrency();

	int transition = rbTransitionEffect->GetSelection();
	if (transition == 0)
		regardsParam->SetEffect(1);
	else
		regardsParam->SetEffect(0);

	int autoRotate = rbAutoRotate->GetSelection();
	if (autoRotate == 0)
		regardsParam->SetDectectOrientation(0);
	else
		regardsParam->SetDectectOrientation(1);

	int dxva2Use = rdDxva2Render->GetSelection();
	if (dxva2Use == 0)
		regardsParam->SetDxva2Actif(1);
	else
		regardsParam->SetDxva2Actif(0);

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

        int pictureSize = rbPictureFaceDetectionSize->GetSelection();
        regardsParam->SetFaceDetectionPictureSize(pictureSize);

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