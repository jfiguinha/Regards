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
	clickInfoToolbar = nullptr;
	isVideo = false;
	posBarInfos = 0;
	isDiapoarama = false;
    clickToobarShow = false;
    paneInfosShow = false;
    isThumbnail = false;

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
		paneInfos = new CPane(this, wxID_ANY, this, PANE_INFOS, theme);
		paneInfos->SetTitle(libelle);
	}

	SetHorizontal(horizontal);

	if (viewerTheme != nullptr)
	{
		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickInfosToolbarTheme(&themeClickInfosToolbar);
		clickInfoToolbar = new CClickToolbar(this, wxID_ANY, themeClickInfosToolbar, this, PANE_INFOS);
		clickInfoToolbar->SetVertical(true);
	}
	
	previewWindow = new CPreviewWnd(this, PREVIEWVIEWERID, statusBarInterface, fileGeolocalisation);
	panelInfosWindow = new CPanelInfosWnd(paneInfos, PANELINFOSWNDID, statusBarInterface, fileGeolocalisation);

	bool showInfos = true;
	int positionBar = 0;

	CViewerParam * viewerParam = CViewerParamInit::getInstance();
	if (viewerParam != nullptr)
	{
		viewerParam->GetShowInfos(showInfos);
		viewerParam->GetPositionSplitter(positionBar);
	}

	paneInfos->SetOtherWindow(panelInfosWindow);
	//panePreview->SetOtherWindow(previewWindow);

	if (showInfos)
	{
		this->posBar = positionBar;
		this->SetWindow(previewWindow, paneInfos);
	}
	else
	{
		posBarInfos = positionBar;
		paneInfos->Show(false);
		clickInfoToolbar->Show(true);
		this->SetWindow(previewWindow, clickInfoToolbar);
		RedrawBarPos();
	}

	Connect(wxEVENT_SHOWPANELINFO, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPreviewInfosWnd::ShowPanelInfos));
	Connect(wxEVENT_HIDEPANELINFO, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPreviewInfosWnd::HidePanelInfos));

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
    //panePreview->UpdateScreenRatio();
    
    panelInfosWindow->UpdateScreenRatio();
    previewWindow->UpdateScreenRatio();
    
    clickInfoToolbar->UpdateScreenRatio();
    
    this->ResizeWindow();
}

void CPreviewInfosWnd::ShowPanelInfos(wxCommandEvent& event)
{
	if (paneInfos != nullptr)
	{
		if (!paneInfos->IsShown())
		{
			ClickShowButton(PANE_INFOS);
		}
	}
}

void CPreviewInfosWnd::HidePanelInfos(wxCommandEvent& event)
{
	if (paneInfos != nullptr)
	{
		if (paneInfos->IsShown())
			ClosePane(PANE_INFOS);
	}
}

bool CPreviewInfosWnd::IsPanelInfosVisible()
{
	if (paneInfos != nullptr)
		return paneInfos->IsShown();
	return false;
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

	//if (panePreview != nullptr)
	//	delete(panePreview);

	if (clickInfoToolbar != nullptr)
		delete(clickInfoToolbar);
    
    if(fileGeolocalisation != nullptr)
        delete(fileGeolocalisation);


}

void CPreviewInfosWnd::ClosePane(const int &id)
{
	switch (id)
	{
		case PANE_INFOS:
		{
			posBarInfos = posBar;

			if (paneInfos != nullptr)
				if (paneInfos->IsShown())
					paneInfos->Show(false);

			if (clickInfoToolbar != nullptr)
				clickInfoToolbar->Show(true);

			CViewerParam * config = CViewerParamInit::getInstance();
			if (config != nullptr)
			{
				CViewerParam * viewerParam = (CViewerParam *)config;
				viewerParam->SetShowInfos(false);
			}

			if (clickInfoToolbar != nullptr && previewWindow != nullptr)
				this->SetWindow(previewWindow, clickInfoToolbar);
			RedrawBarPos();
		}
		break;
	}

	this->Resize(this);
}

void CPreviewInfosWnd::ResizeWindow()
{
	posBar = posBar - 1;
	this->Resize(this);
}


void CPreviewInfosWnd::RedrawBarPos()
{
    if(paneInfos != nullptr)
    {
        bool showInfos = paneInfos->IsShown();

        if (!showInfos && clickInfoToolbar != nullptr)
        {
            wxRect rc = this->GetWindowRect();
		
            int taille = rc.width - clickInfoToolbar->GetNavigatorWidth();
            if (taille > 0)
                posBar = taille;

            SetWindow2FixPosition(true, clickInfoToolbar->GetNavigatorWidth());
        }
    }

	this->SetSeparationBarVisible(false);
	this->Resize(this);

}

void CPreviewInfosWnd::SetEffect(const bool &effect)
{
	if (previewWindow != nullptr)
		previewWindow->SetEffect(effect);
}

void CPreviewInfosWnd::ClickShowButton(const int &id)
{
	switch (id)
	{
		case PANE_INFOS:
		{
			if (clickInfoToolbar->IsShown())
				clickInfoToolbar->Show(false);

			if (!paneInfos->IsShown())
				paneInfos->Show(true);

			this->SetSeparationBarVisible(true);

			SetWindow2FixPosition(false, posBarInfos);

			posBar = posBarInfos;

			CViewerParam * config = CViewerParamInit::getInstance();
			if (config != nullptr)
			{
				CViewerParam * viewerParam = (CViewerParam *)config;
				viewerParam->SetPositionSplitter(posBar);
				viewerParam->SetShowInfos(true);
			}

			this->SetWindow(previewWindow, paneInfos);

			SetPanelInfos(false);

			this->RedrawBarPos();
		}
		break;
	}
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

void CPreviewInfosWnd::HidePanel()
{
    ClosePane(PANE_INFOS);
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
    clickToobarShow = clickInfoToolbar->IsShown();
    
    clickInfoToolbar->Show(false);
	previewWindow->SetFullscreen(true);
	this->SetWindow(previewWindow, nullptr);
	this->Resize(this);

}

void CPreviewInfosWnd::ScreenMode()
{
	fullscreen = false;
	paneInfos->Show(paneInfosShow);
	//panePreview->SetTitleBarVisibility(true);
    previewWindow->SetFullscreen(false);
    
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
}

