#include <header.h>
#include "PreviewInfosWnd.h"
#include <RegardsBitmap.h>
#include <ConfigParam.h>
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <LibResource.h>
#include "ParamInit.h"
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#include <ImageLoadingFormat.h>
#include <window_id.h>
#include <Gps.h>
using namespace Regards::Window;
using namespace Regards::Viewer;

#define PANE_INFOS 2
#define PANE_PREVIEW 3

CPreviewInfosWnd::CPreviewInfosWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, 
	const CThemeSplitter & theme, const bool &horizontal)
	: CSplitter(parent, id, theme)
{
	paneInfos = nullptr;
	panelInfosWindow = nullptr;
	previewWindow = nullptr;
	isVideo = false;
	posBarInfos = 0;
	isDiapoarama = false;
    clickToobarShow = false;
    paneInfosShow = false;
    isThumbnail = false;

	bool showInfos = true;
	int positionBar = 0;

	CViewerParam * viewerParam = CViewerParamInit::getInstance();
	if (viewerParam != nullptr)
	{
		viewerParam->GetShowInfos(showInfos);
		viewerParam->GetPositionSplitter(positionBar);
	}

	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();
    
    wxString urlServer;
    CRegardsConfigParam * param = CParamInit::getInstance();
    if (param != nullptr)
    {
        urlServer = param->GetUrlServer();
    }
    
    fileGeolocalisation = new CFileGeolocation(urlServer);
    
	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS",1);
		CThemePane theme;
		viewerTheme->GetInfosPaneTheme(&theme);
		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickInfosToolbarTheme(&themeClickInfosToolbar);
		paneInfos = new CPanelWithClickToolbar(this, "CPictureInfosPanel", wxID_ANY, theme, themeClickInfosToolbar, libelle, showInfos);
		panelInfosWindow = new CPanelInfosWnd(paneInfos->GetPaneWindow(), PANELINFOSWNDID, statusBarInterface, fileGeolocalisation);
		paneInfos->SetWindow(panelInfosWindow);
	}

	SetHorizontal(horizontal);
	previewWindow = new CPreviewWnd(this, PREVIEWVIEWERID, statusBarInterface, fileGeolocalisation);
	

	
	this->posBar = positionBar;
	this->SetWindow(previewWindow, paneInfos);

	Connect(wxEVENT_RESIZE, wxCommandEventHandler(CPreviewInfosWnd::OnResize));

	RedrawBarPos();
}

void CPreviewInfosWnd::StopLoadingPicture()
{

}

void CPreviewInfosWnd::OnResize(wxCommandEvent& event)
{
	RedrawBarPos();
}

bool CPreviewInfosWnd::IsToolbarVisible()
{
	if (previewWindow != nullptr)
		return previewWindow->IsToolbarVisible();

	return false;
}

void CPreviewInfosWnd::UpdateScreenRatio()
{
    paneInfos->UpdateScreenRatio();
    panelInfosWindow->UpdateScreenRatio();
    previewWindow->UpdateScreenRatio();
    
    this->ResizeWindow();
}

CPreviewInfosWnd::~CPreviewInfosWnd()
{
	CViewerParam * config = CViewerParamInit::getInstance();
	if (config != nullptr)
	{
		bool showInfos = true;
		CViewerParam * viewerParam = (CViewerParam *)config;
		viewerParam->GetShowInfos(showInfos);
		if (showInfos)
			viewerParam->SetPositionSplitter(posBar);
	}


	if (panelInfosWindow != nullptr)
		delete(panelInfosWindow);
	

	if (previewWindow != nullptr)
		delete(previewWindow);

	if (paneInfos != nullptr)
		delete(paneInfos);
    
    if(fileGeolocalisation != nullptr)
        delete(fileGeolocalisation);


}

bool CPreviewInfosWnd::IsPanelInfosVisible()
{
	return paneInfos->IsPanelVisible();
}

void CPreviewInfosWnd::ResizeWindow()
{
	posBar = posBar - 1;
	this->Resize(this);
}

void CPreviewInfosWnd::ShowInfos()
{
	if (paneInfos->IsPanelVisible())
	{
		this->posBar = posBarInfos;
		this->SetSeparationBarVisible(true);
		SetWindow2FixPosition(false, posBarInfos);
	}
}


void CPreviewInfosWnd::RedrawBarPos()
{
    if(paneInfos != nullptr)
    {
		if (!paneInfos->IsPanelVisible())
		{
			posBarInfos = this->posBar;
			SetWindow2FixPosition(true, paneInfos->GetWidth());
			this->SetSeparationBarVisible(false);
		}
		else
		{
			this->posBar = posBarInfos;
			this->SetSeparationBarVisible(true);
			SetWindow2FixPosition(false, posBarInfos);
		}
    }

	
	this->Resize(this);

}

void CPreviewInfosWnd::SetEffect(const bool &effect)
{
	if (previewWindow != nullptr)
		previewWindow->SetEffect(effect);
}
void CPreviewInfosWnd::SetDiaporamaMode()
{
	isDiapoarama = true;
	previewWindow->SetDiaporamaMode();
}

void CPreviewInfosWnd::SetNormalMode()
{
	isDiapoarama = false;
	previewWindow->SetNormalMode();
}

bool CPreviewInfosWnd::SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation)
{
    TRACE();
	if(bitmap != nullptr && bitmap->IsOk())
	{
		isVideo = false;
		bool loadInfo = true;
		if (fileGeolocalisation->GetFilename() != bitmap->GetFilename())
			fileGeolocalisation->SetFile(bitmap->GetFilename());
		else
			loadInfo = false;
    
		if (isDiapoarama)
		{
			if (previewWindow != nullptr)
			{
				if (previewWindow->SetBitmap(bitmap, isThumbnail, isAnimation))
				{
					isVideo = false;
					filename = bitmap->GetFilename();
				}
			}
		}
		else
		{

			if (previewWindow != nullptr)
			{
				previewWindow->HideValidationToolbar();
				if (previewWindow->SetBitmap(bitmap, isThumbnail, isAnimation))
				{
					isVideo = false;
					filename = bitmap->GetFilename();
					//if (loadInfo && panelInfosWindow->IsShown())
						SetPanelInfos(isThumbnail);
				}
			}
		}
	}
	return 0;
}




void CPreviewInfosWnd::StartLoadingPicture()
{
    previewWindow->StartLoadingPicture();
}

void CPreviewInfosWnd::SetPanelInfos(const bool &isThumbnail)
{
    
	if (panelInfosWindow->GetFilename() != filename)
	{
		if (isVideo)
			panelInfosWindow->SetVideoFile(filename);
		else
			panelInfosWindow->SetBitmapFile(filename, isThumbnail);
        
        this->Resize(this);
	}
    else
    {
        if(!isThumbnail && this->isThumbnail)
        {
            panelInfosWindow->SetBitmapFile(filename, isThumbnail);
            this->Resize(this);
        }
    }
    
    this->isThumbnail = isThumbnail;
	
}

void CPreviewInfosWnd::SetVideo(const wxString &filename)
{

    if (fileGeolocalisation->GetFilename() != filename)
        fileGeolocalisation->SetFile(filename); 
    
	if (isDiapoarama)
	{
		this->filename = filename;
		if (previewWindow->SetVideo(filename))
			isVideo = true;
	}
	else
	{
		this->filename = filename;
		previewWindow->HideValidationToolbar();
		if (previewWindow->SetVideo(filename))
		{
			isVideo = true;
			SetPanelInfos(false);
		}
	}
}

void CPreviewInfosWnd::FullscreenMode()
{
	fullscreen = true;
    this->posBarInfos = this->posBar;
    paneInfosShow = paneInfos->IsShown();
	paneInfos->Show(false);
	//panePreview->SetTitleBarVisibility(false);
    //clickToobarShow = clickInfoToolbar->IsShown();  
    //clickInfoToolbar->Show(false);
	previewWindow->SetFullscreen(true);
	this->SetWindow(previewWindow, nullptr);
	this->Resize(this);

}

void CPreviewInfosWnd::ScreenMode()
{
	fullscreen = false;
	
	//panePreview->SetTitleBarVisibility(true);
    previewWindow->SetFullscreen(false);
	this->SetWindow(previewWindow, paneInfos);
	if (!paneInfosShow)
		paneInfos->Show();
	else
	{
		paneInfos->Show(paneInfosShow);
		this->SetSeparationBarVisible(true);
		this->Resize(this);
	}
	
	/*
	if (!paneInfosShow)
        ClosePane(PANE_INFOS);
    else
    {
		this->SetWindow(previewWindow, paneInfos);
		wxCommandEvent event;
		ShowPanelInfos(event);
        this->SetSeparationBarVisible(true);
        this->Resize(this);
    }
	*/
}

