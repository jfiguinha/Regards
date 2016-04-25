#include "wx_pch.h"
#include "ConfigRegards.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#if defined(__WXMSW__)
#include "../include/config_id.h"
#else
#include <config_id.h>
#endif
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
	rbFullscreen = (wxRadioBox*)FindWindow(XRCID("ID_RBFULLSCREEN"));
	rbDiaporamaTransitionEffect = (wxRadioBox*)FindWindow(XRCID("ID_RBDIAPORAMATRANSITIONEFFECT"));
	btCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));
	sbThumbnail = (wxStaticBox*)FindWindow(XRCID("ID_STATICBOX2"));
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

    /*
	//Preview
	int previewLib = regardsParam->GetPreviewLibrary();
	switch (previewLib)
	{
	case LIBOPENGL:
		rdPreviewRender->SetSelection(1);
		break;
    case LIBOPENCL:
	case LIBCPU:
		rdPreviewRender->SetSelection(0);
		break;
	}

	//filtre
	int filtre = regardsParam->GetEffectLibrary();
	switch (filtre)
	{
	case LIBOPENCL:
		rbEffectLibrary->SetSelection(1);
		break;
	case LIBCPU:
		rbEffectLibrary->SetSelection(0);
		break;
	}
	//Video
    
#if defined(__APPLE__) || defined(WIN32)
    
    rbRenderVideo->SetSelection(0);
    
#else
	int video = regardsParam->GetVideoLibrary();
	switch (video)
	{
	case LIBOPENGL:
		rbRenderVideo->SetSelection(1);
		break;
	case LIBCPU:
		rbRenderVideo->SetSelection(0);
		break;
	}
#endif
    */
	int transition = regardsParam->GetEffect();
	if (transition == 0)
		rbTransitionEffect->SetSelection(1);
	else
		rbTransitionEffect->SetSelection(0);

	int timeDiaporama = regardsParam->GetDiaporamaTime();
	scTime->SetValue(timeDiaporama);

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

    /*
	int thumbnailQuality = regardsParam->GetThumbnailQuality();
	if (thumbnailQuality == 0)
		rbThumbnailQuality->SetSelection(1);
	else
		rbThumbnailQuality->SetSelection(0);
	
	int thumbnailCache = regardsParam->GetThumbnailIconeCache();
	if (thumbnailCache == 0)
		rbThumbnailCache->SetSelection(1);
	else
		rbThumbnailCache->SetSelection(0);
     */
}

void ConfigRegards::OnbtnOkClick(wxCommandEvent& event)
{
	isOk = true;
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
	//Preview
    /*
	int previewLib = rdPreviewRender->GetSelection();
	switch (previewLib)
	{
	case 1:
		regardsParam->SetPreviewLibrary(LIBOPENGL);
		break;
	case 0:
		regardsParam->SetPreviewLibrary(LIBCPU);
		break;
	}

	//filtre
	int filtre = rbEffectLibrary->GetSelection();
	switch (filtre)
	{
	case 1:
		regardsParam->SetEffectLibrary(LIBOPENCL);
		break;
	case 0:
		regardsParam->SetEffectLibrary(LIBCPU);
		break;
	}
    
#if defined(__APPLE__) || defined(WIN32)
    
    regardsParam->SetVideoLibrary(LIBCPU);
    
#else
	//Video
	int video = rbRenderVideo->GetSelection();
	switch (video)
	{
	case 1:
		regardsParam->SetVideoLibrary(LIBOPENGL);
		break;
	case 0:
		regardsParam->SetVideoLibrary(LIBCPU);
		break;
	}
#endif
    */
	int transition = rbTransitionEffect->GetSelection();
	if (transition == 0)
		regardsParam->SetEffect(1);
	else
		regardsParam->SetEffect(0);

	int timeDiaporama = scTime->GetValue();
	regardsParam->SetDiaporamaTime(timeDiaporama);

	int effectDiaporama = rbDiaporamaTransitionEffect->GetSelection();
	if (effectDiaporama == 0)
		regardsParam->SetDiaporamaTransitionEffect(1);
	else
		regardsParam->SetDiaporamaTransitionEffect(0);

	int fullscreenDiaporama = rbFullscreen->GetSelection();
	if (fullscreenDiaporama == 0)
		regardsParam->SetDiaporamaFullscreen(1);
	else
		regardsParam->SetDiaporamaFullscreen(0);

    /*
	int thumbnailQuality = rbThumbnailQuality->GetSelection();
	if (thumbnailQuality == 0)
		regardsParam->SetThumbnailQuality(1);
	else
		regardsParam->SetThumbnailQuality(0);

	int thumbnailCache = rbThumbnailCache->GetSelection();
	if (thumbnailCache == 0)
		regardsParam->SetThumbnailIconeCache(1);
	else
		regardsParam->SetThumbnailIconeCache(0);
     */
	this->Close();
}

void ConfigRegards::OnBtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}
