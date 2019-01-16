#include "ThumbnailVideoPanel.h"
#include "ThumbnailViewerVideo.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include "CentralWnd.h"
#include <FileUtility.h>
#include <ShowVideo.h>
#include <LibResource.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Window;
using namespace Regards::Viewer;
#define PANE_PICTURETHUMBNAIL 1
#define PANE_VIDEOTHUMBNAIL 2

CThumbnailVideoPanel::CThumbnailVideoPanel(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface)
	: CWindowMain("CThumbnailVideoPanel",parent,id)
{
	paneThumbnailVideo = nullptr;
	scrollThumbnailVideo = nullptr;
	thumbnailVideo = nullptr;
	clickThumbnailVideo = nullptr;

	isPanelVisible = true;

	Connect(wxEVENT_ONSTOPLOADINGPICTURE, wxCommandEventHandler(CThumbnailVideoPanel::StopLoadingPicture));

	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();
	CViewerParam * config = CViewerParamInit::getInstance();
	if (config != nullptr)
	{
		config->GetShowVideoThumbnail(isPanelVisible);
		checkValidity = config->GetCheckThumbnailValidity();
	}


	//----------------------------------------------------------------------------------------
	//Panel Thumbnail Video
	//----------------------------------------------------------------------------------------

	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLTHUMBNAILVIDEO", 1);
		CThemePane theme;
		viewerTheme->GetThumbnailVideoPaneTheme(&theme);
		paneThumbnailVideo = new CPane(this, wxID_ANY, this, PANE_VIDEOTHUMBNAIL, theme);
		paneThumbnailVideo->SetTitle(libelle);
	}
		
	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetClickThumbnailVideoToolbarTheme(&theme);
		clickThumbnailVideo = new CClickToolbar(this, wxID_ANY, theme, this, PANE_VIDEOTHUMBNAIL);
	}
	
	
	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollThumbnailVideoTheme(&themeScroll);
		scrollThumbnailVideo = new CScrollbarWnd(paneThumbnailVideo, wxID_ANY);

		CThemeThumbnail theme;
		viewerTheme->GetThumbnailVideoTheme(&theme);
		
		thumbnailVideo = new CThumbnailViewerVideo(scrollThumbnailVideo, wxID_ANY, statusBarInterface, theme, checkValidity);

		scrollThumbnailVideo->SetCentralWindow(thumbnailVideo, themeScroll);
		scrollThumbnailVideo->HideVerticalScroll();
		scrollThumbnailVideo->SetPageSize(1000);
		scrollThumbnailVideo->SetLineSize(200);

		paneThumbnailVideo->SetOtherWindow(scrollThumbnailVideo);

		
	}

	if(isPanelVisible)
	{
		clickThumbnailVideo->Show(false);
		paneThumbnailVideo->Show(true);
	}
	else
	{
		clickThumbnailVideo->Show(true);
		paneThumbnailVideo->Show(false);
	}

    m_animationCtrl = new wxAnimationCtrl(this, wxID_ANY);
    m_animationCtrl->Show(false);

	wxString resourcePath = CFileUtility::GetResourcesFolderPath();
#ifdef WIN32
	m_animationCtrl->LoadFile(resourcePath + "\\loading.gif");
#else
	m_animationCtrl->LoadFile(resourcePath + "//loading.gif");
#endif

	CThemeThumbnail theme;
	m_animationCtrl->SetBackgroundColour(theme.colorBack);
}


void CThumbnailVideoPanel::StartLoadingPicture()
{
	if(paneThumbnailVideo->IsShown())
	{
		scrollThumbnailVideo->Show(false);
		paneThumbnailVideo->SetOtherWindow(m_animationCtrl);
		m_animationCtrl->Show(true);
		m_animationCtrl->Play();
		wxSize size = m_animationCtrl->GetSize();
		wxPoint pt;
		pt.x = (this->GetWindowWidth() - size.GetWidth()) / 2;
		pt.y = (this->GetWindowHeight() - size.GetHeight()) / 2;
		m_animationCtrl->SetPosition(pt);
		this->Resize();
	}
    
}

void CThumbnailVideoPanel::StopLoadingPicture(wxCommandEvent& event)
{
	if(paneThumbnailVideo->IsShown())
	{
		m_animationCtrl->Stop();
		m_animationCtrl->Show(false);
		scrollThumbnailVideo->Show(true);
		paneThumbnailVideo->SetOtherWindow(scrollThumbnailVideo);
		this->Resize();
	}

}

CThumbnailVideoPanel::~CThumbnailVideoPanel()
{
	if (m_animationCtrl != nullptr)
		delete m_animationCtrl;

	delete(thumbnailVideo);
	delete(scrollThumbnailVideo);
	delete(paneThumbnailVideo);
}

void CThumbnailVideoPanel::SetVideoPosition(const int64_t &videoTime)
{
	if (thumbnailVideo != nullptr)
		thumbnailVideo->SetVideoPosition(videoTime);
}

void CThumbnailVideoPanel::UpdateScreenRatio()
{
    paneThumbnailVideo->UpdateScreenRatio();
    scrollThumbnailVideo->UpdateScreenRatio();
    thumbnailVideo->UpdateScreenRatio();
    clickThumbnailVideo->UpdateScreenRatio();   
    this->Resize();
}

void CThumbnailVideoPanel::ClickShowButton(const int &id)
{
	switch (id)
	{
	case PANE_VIDEOTHUMBNAIL:
		{
			CViewerParam * config = CViewerParamInit::getInstance();
			if (config != nullptr)
			{
				config->SetShowVideoThumbnail(true);
			}

			isPanelVisible = true;
			clickThumbnailVideo->Show(false);
			paneThumbnailVideo->Show(true);

			CCentralWnd * centralWnd = (CCentralWnd *)this->GetParent();
			centralWnd->Resize();
		}
		break;
	}

}


void CThumbnailVideoPanel::ClosePane(const int &id)
{
	switch (id)
	{
	case PANE_VIDEOTHUMBNAIL:
		{
			CViewerParam * config = CViewerParamInit::getInstance();
			if (config != nullptr)
				config->SetShowVideoThumbnail(false);

			isPanelVisible = false;
			paneThumbnailVideo->Show(false);
			clickThumbnailVideo->Show(true);

			m_animationCtrl->Stop();
			m_animationCtrl->Show(false);
			scrollThumbnailVideo->Show(true);
			paneThumbnailVideo->SetOtherWindow(scrollThumbnailVideo);

			CCentralWnd * centralWnd = (CCentralWnd *)this->GetParent();
			centralWnd->Resize();
		}
		break;
	}

	Resize();
}

void CThumbnailVideoPanel::Resize()
{
	if(clickThumbnailVideo->IsShown())
	{
		clickThumbnailVideo->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
	}
	else if(paneThumbnailVideo->IsShown())
	{
		paneThumbnailVideo->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
	}
}

int CThumbnailVideoPanel::GetHeight()
{
	if(!isPanelVisible)
		return clickThumbnailVideo->GetHeight();
	return thumbnailVideo->GetIconeHeight() + scrollThumbnailVideo->GetBarHeight() + paneThumbnailVideo->GetTitleHeight();
}

void CThumbnailVideoPanel::SetAnimation(const wxString &path, vector<CImageVideoThumbnail *> * pictureThumbnail)
{
	filename = path;
	ShowVideoThumbnail(pictureThumbnail);
	Resize();
    
    
    CWindowMain * windowMain = (CWindowMain *)this->FindWindowById(MAINVIEWERWINDOWID);
    if(windowMain != nullptr)
    {
        wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVT_ANIMATIONTIMERSTART);
        windowMain->GetEventHandler()->AddPendingEvent(evt);       
    }
}

void CThumbnailVideoPanel::SetVideo(const wxString &path)
{
	filename = path;
	ShowVideoThumbnail();
	Resize();
}

void CThumbnailVideoPanel::HideVideoThumbnail(const bool &refresh)
{
	this->Show(false);
	clickThumbnailVideo->Show(false);
	paneThumbnailVideo->Show(false);

	if(refresh)
	{
		CCentralWnd * centralWnd = (CCentralWnd *)this->GetParent();
		centralWnd->Resize();
	}
}

void CThumbnailVideoPanel::ShowVideoThumbnail(vector<CImageVideoThumbnail *> * pictureThumbnail)
{
	this->Show(true);

	if(isPanelVisible)
	{
		clickThumbnailVideo->Show(false);
		paneThumbnailVideo->Show(true);
	}
	else
	{
		clickThumbnailVideo->Show(true);
		paneThumbnailVideo->Show(false);
	}

	if (thumbnailVideo->GetFilename() != filename)
	{
		thumbnailVideo->SetFilename(filename);
		if (pictureThumbnail != nullptr)
			thumbnailVideo->SetVideoThumbnail(filename, pictureThumbnail);
		else
			thumbnailVideo->SetVideoFile(filename);
	}
	/*
	if(refresh)
	{
		CCentralWnd * centralWnd = (CCentralWnd *)this->GetParent();
		centralWnd->Resize();
	}*/
	//StartLoadingPicture();
}

bool CThumbnailVideoPanel::IsPanelVideoThumbnailVisible()
{
	return isPanelVisible;
}


