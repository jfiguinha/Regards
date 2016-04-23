#include "PreviewWnd.h"
#include "MainWindow.h"
#include <effect.h>
#include <libPicture.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include "ViewerParam.h"
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
using namespace Regards::Control;
using namespace Regards::Viewer;

#define PANE_INFOS 2
#define PANE_PREVIEW 3


CPreviewWnd::CPreviewWnd(wxWindow* parent, wxWindowID id, CVideoEffectParameter * videoEffectParameter, IStatusBarInterface * statusBarInterface, CFileGeolocation * fileGeolocalisation, const bool &horizontal)
	: CWindowMain(parent, id)
{
	
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();
	
	if (viewerTheme != nullptr)
	{
		CThemeBitmapInfos theme;
		viewerTheme->GetBitmapInfosTheme(&theme);
		bitmapInfos = new CBitmapInfos(this, wxID_ANY, theme, fileGeolocalisation);
	}

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetPreviewToolbarTheme(&theme);
		previewToolbar = new CPreviewToolbar(this, wxID_ANY, theme, this);
	}

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetFiltreToolbarTheme(&theme);
		filtreToolbar = new CFiltreToolbar(this, wxID_ANY, theme);
	}
    
    if (viewerTheme != nullptr)
        viewerTheme->GetBitmapWindowTheme(&themeBitmap);

	CMainWindow * mainWnd = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	showBitmapWindow = new CShowBitmap(this, SHOWBITMAPVIEWERID, BITMAPWINDOWVIEWERID, mainWnd, this, statusBarInterface, viewerTheme);
	showVideoWindow = new CShowVideo(this, SHOWVIDEOVIEWERID, mainWnd, videoEffectParameter, viewerTheme);
	m_animationCtrl = new wxAnimationCtrl(this, wxID_ANY);
    Connect(wxEVT_PAINT, wxPaintEventHandler(CPreviewWnd::OnPaint));
    
    
	isVideo = false;
	this->videoEffectParameter = videoEffectParameter;

    m_animationCtrl->Show(false);
	showVideoWindow->Show(false);
	filtreToolbar->Show(false);
	bitmapInfos->Show(false);
	previewToolbar->Show(false);
}


CPreviewWnd::~CPreviewWnd()
{
    delete(m_animationCtrl);
	delete(showBitmapWindow);
	delete(showVideoWindow);
	delete(previewToolbar);
	delete(bitmapInfos);
	delete(filtreToolbar);
}

void CPreviewWnd::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    wxRect rc = GetRect();
    if(fullscreen)
    {
        this->FillRect(&dc, rc, themeBitmap.colorFullscreen);
		if (m_animationCtrl->IsShown())
			m_animationCtrl->SetBackgroundColour(themeBitmap.colorFullscreen);
    }
    else
    {
        this->FillRect(&dc, rc, themeBitmap.colorScreen);
		if (m_animationCtrl->IsShown())
			m_animationCtrl->SetBackgroundColour(themeBitmap.colorScreen);
    }
}

void CPreviewWnd::ClickShowButton(const int &id)
{
	switch (id)
	{
		case WM_ROTATE90:
		{
			if (isVideo)
				showVideoWindow->Rotate90();
			else
				showBitmapWindow->Rotate90();
		}
		break;

		case WM_ROTATE270:
		{
			if (isVideo)
				showVideoWindow->Rotate270();
			else
				showBitmapWindow->Rotate270();
		}
		break;

		case WM_FLIPVERTICAL:
		{
			if (isVideo)
				showVideoWindow->FlipVertical();
			else
				showBitmapWindow->FlipVertical();
		}
		break;

		case WM_FLIPHORIZONTAL:
		{
			if (isVideo)
				showVideoWindow->FlipHorizontal();
			else
				showBitmapWindow->FlipHorizontal();
		}
		break;

	}
}

void CPreviewWnd::Resize()
{
	if (isDiaporama)
	{ 
		wxRect rcAffichageBitmap;
		int bitmapInfosHeight = 0;
		rcAffichageBitmap.x = 0;
		rcAffichageBitmap.y = 0;
		rcAffichageBitmap.width = width;
		rcAffichageBitmap.height = height;

        if(bitmapInfos != nullptr)
            bitmapInfos->SetSize(0, 0, 0, 0);
        if(filtreToolbar != nullptr)
            filtreToolbar->SetSize(0, 0, 0, 0);
        if(previewToolbar != nullptr)
            previewToolbar->SetSize(0, 0, 0, 0);

        if(showBitmapWindow != nullptr)
        {
            if (showBitmapWindow->IsShown())
                showBitmapWindow->SetSize(rcAffichageBitmap.x, bitmapInfosHeight, rcAffichageBitmap.width, rcAffichageBitmap.height - bitmapInfosHeight);
            else if(showVideoWindow->IsShown())
                showVideoWindow->SetSize(rcAffichageBitmap.x, bitmapInfosHeight, rcAffichageBitmap.width, rcAffichageBitmap.height - bitmapInfosHeight);
            else if(m_animationCtrl->IsShown())
                m_animationCtrl->SetSize(rcAffichageBitmap.x, bitmapInfosHeight, rcAffichageBitmap.width, rcAffichageBitmap.height - bitmapInfosHeight);
        }
        
        return;
	}
    
    if (previewToolbar != nullptr && filtreToolbar != nullptr)
    {
        if (previewToolbar->IsShown() || filtreToolbar->IsShown())
        {
            wxRect rcAffichageBitmap;
            int bitmapInfosHeight = bitmapInfos->GetHeight();
            int toolbarHeightSize = previewToolbar->GetHeight();
            //if (filtreToolbar->IsShown())
            //    toolbarHeightSize = filtreToolbar->GetHeight();
            
            if (!showBitmapWindow->IsShown() && !showVideoWindow->IsShown() && !m_animationCtrl->IsShown())
                bitmapInfosHeight = 0;

            rcAffichageBitmap.x = 0;
            rcAffichageBitmap.y = 0;
            rcAffichageBitmap.width = width;
            rcAffichageBitmap.height = height - toolbarHeightSize;

            if (showBitmapWindow->IsShown())
            {
                if (bitmapInfos->IsShown())
                    bitmapInfos->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, bitmapInfosHeight);
                else
                    bitmapInfosHeight = 0;

                showBitmapWindow->SetSize(rcAffichageBitmap.x, bitmapInfosHeight, rcAffichageBitmap.width, rcAffichageBitmap.height - bitmapInfosHeight);
            }
            else if(m_animationCtrl->IsShown())
            {
                if (bitmapInfos->IsShown())
                    bitmapInfos->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, bitmapInfosHeight);
                else
                    bitmapInfosHeight = 0;
                
                wxAnimation animation = m_animationCtrl->GetAnimation();
                wxSize animationSize = animation.GetSize();
                
                int xPos = rcAffichageBitmap.x + (width - animationSize.GetWidth()) / 2;
                int yPos = bitmapInfosHeight + ((height - bitmapInfosHeight - toolbarHeightSize) - animationSize.GetHeight()) / 2;
                
                m_animationCtrl->SetSize(xPos, yPos, animationSize.GetWidth(), animationSize.GetHeight());
            }
            else
            {
                if (bitmapInfos->IsShown())
                    bitmapInfos->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, bitmapInfosHeight);
                else
                    bitmapInfosHeight = 0;

                showVideoWindow->SetSize(rcAffichageBitmap.x, bitmapInfosHeight, rcAffichageBitmap.width, rcAffichageBitmap.height - bitmapInfosHeight);
            }

            if (filtreToolbar->IsShown())
                filtreToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height, rcAffichageBitmap.width, toolbarHeightSize);
            else
                previewToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height, rcAffichageBitmap.width, toolbarHeightSize);
            
            return;
        }
	}

		
    if (showBitmapWindow->IsShown())
        showBitmapWindow->SetSize(0, 0, width, height);
    else if(m_animationCtrl->IsShown())
        m_animationCtrl->SetSize(0, 0, width, height);
    else
        showVideoWindow->SetSize(0, 0, width, height);

}

void CPreviewWnd::SetDiaporamaMode()
{
	isDiaporama = true;
	showBitmapWindow->SetDiaporamaMode();
	this->Resize();
}

void CPreviewWnd::SetNormalMode()
{
	isDiaporama = false;
	showBitmapWindow->SetNormalMode();
	this->Resize();
}

void CPreviewWnd::SetEffect(const bool &effect)
{
	//showBitmapWindow->SetEffect(effect);
	if (previewToolbar->IsShown())
		previewToolbar->Show(false);
	if (!filtreToolbar->IsShown())
		filtreToolbar->Show(true);
	Resize();
}

void CPreviewWnd::HideToolbar()
{
	if (!isEffect)
	{
		if (previewToolbar != nullptr)
		{
			bool previewToolbarMouseOver = previewToolbar->IsMouseOver();
			if (isVideo)
				previewToolbarMouseOver = showVideoWindow->IsToolbarMouseOver();
			else
				previewToolbarMouseOver = showBitmapWindow->IsToolbarMouseOver();

			if (!previewToolbarMouseOver && !previewToolbarMouseOver)
			{
				if (previewToolbar->IsShown())
					previewToolbar->Show(false);

				if (isVideo)
                {
                    bitmapInfos->Show(false);
                    showVideoWindow->HideToolbar();
                }
				else if(isBitmap)
				{
					bitmapInfos->Show(false);
					showBitmapWindow->HideToolbar();
				}
                else
                {
                    bitmapInfos->Show(false);
                }
				Resize();
			}
		}
	}
}

void CPreviewWnd::ShowValidationToolbar(const bool &visible, const int &filtre)
{
	isEffect = visible;
	filtreToolbar->SetNumFiltre(filtre);
	if (previewToolbar != nullptr)
	{
		if (previewToolbar->IsShown())
			previewToolbar->Show(false);
	}

	if (filtreToolbar != nullptr)
	{
		if (!filtreToolbar->IsShown())
			filtreToolbar->Show(true);
	}
	Resize();
}

void CPreviewWnd::HideValidationToolbar()
{
    showToolbar = false;
	isEffect = false;
	if (filtreToolbar != nullptr)
	{
		if (filtreToolbar->IsShown())
			filtreToolbar->Show(false);
	}
	showBitmapWindow->SetBitmapPreviewEffect(0);
	ShowToolbar();
}

void CPreviewWnd::ShowToolbar()
{
    showToolbar = true;
	if (!isEffect)
	{
		if (previewToolbar != nullptr)
		{
			if (!previewToolbar->IsShown())
				previewToolbar->Show(true);

			bitmapInfos->Show(true);

			if (isVideo)
                showVideoWindow->ShowToolbar();
			else if(isBitmap)
				showBitmapWindow->ShowToolbar();

			Resize();
		}
	}
}

void CPreviewWnd::UpdateScreenRatio()
{
    showBitmapWindow->UpdateScreenRatio();
    showVideoWindow->UpdateScreenRatio();
    previewToolbar->UpdateScreenRatio();
    bitmapInfos->UpdateScreenRatio();
    filtreToolbar->UpdateScreenRatio();
    this->Resize();
}

bool CPreviewWnd::IsToolbarVisible()
{
	if (previewToolbar != nullptr)
	{
		return previewToolbar->IsShown();
	}
	return false;
}

void CPreviewWnd::SetFullscreen(const bool &fullscreen)
{
	this->fullscreen = fullscreen;
	showBitmapWindow->SetFullscreen(fullscreen);
    
    if(fullscreen)
        previewToolbar->SetFullscreen();
    else
        previewToolbar->SetScreen();
    
	Resize();
}

bool CPreviewWnd::SetAnimation(const wxString &filename)
{
    if(oldfilename != filename)
    {
        filtreToolbar->Show(false);
        if (isVideo)
            showVideoWindow->StopVideo();
        else if(isBitmap)
            showBitmapWindow->StopTransition();
        
        if(!m_animationCtrl->IsShown())
            m_animationCtrl->Show(true);
        
        if(showBitmapWindow->IsShown())
            showBitmapWindow->Show(false);
        
        if(showVideoWindow->IsShown())
            showVideoWindow->Show(false);
        
        if(showToolbar && !bitmapInfos->IsShown())
            bitmapInfos->Show(true);
        
        bitmapInfos->SetFilename(filename);
        
        if(showToolbar)
            bitmapInfos->Refresh();
        
        isVideo = false;
        isBitmap = false;
        
        if(isAnimation)
            m_animationCtrl->Stop();

        m_animationCtrl->LoadFile(filename);
        m_animationCtrl->Play();
        
        
        isAnimation = true;
        oldfilename = filename;
        this->Resize();
    }
    return 1;
}

bool CPreviewWnd::SetBitmap(CRegardsBitmap * bitmap, const bool &isThumbnail)
{
    wxString filename = bitmap->GetFilename();
    if(oldfilename != filename)
    {
        
        filtreToolbar->Show(false);
        if (isVideo)
            showVideoWindow->StopVideo();
        else if(isAnimation)
            m_animationCtrl->Stop();
        else
            showBitmapWindow->StopTransition();
        
        if(m_animationCtrl->IsShown())
            m_animationCtrl->Show(false);
        
        if(!showBitmapWindow->IsShown())
            showBitmapWindow->Show(true);
        
        if(showVideoWindow->IsShown())
            showVideoWindow->Show(false);

        if(showToolbar && !bitmapInfos->IsShown())
            bitmapInfos->Show(true);
        
        bitmapInfos->SetFilename(filename);
        bitmapInfos->Refresh();

        isVideo = false;
        isAnimation = false;
        isBitmap = true;
            
        showBitmapWindow->SetBitmap(bitmap);
        oldfilename = filename;
        
        this->Resize();
    }
    else if(!isThumbnail)
    {
        showBitmapWindow->StopLoadingPicture();
        showBitmapWindow->SetBitmap(bitmap);
        this->Resize();
    }
	return 1;
}

void CPreviewWnd::StartLoadingPicture()
{
    showBitmapWindow->StartLoadingPicture();
}

bool CPreviewWnd::SetVideo(const wxString &filename)
{
    if(oldfilename != filename)
    {
        CLibPicture libPicture;
        filtreToolbar->Show(false);
        if (isVideo)
        {
            if(oldfilename != filename)
                showVideoWindow->StopVideo();
        }
        else if(isAnimation)
        {
            m_animationCtrl->Stop();
        }
        else
        {
            showBitmapWindow->StopTransition();
        }
        
        if(m_animationCtrl->IsShown())
            m_animationCtrl->Show(false);
        
        if(showBitmapWindow->IsShown())
            showBitmapWindow->Show(false);
        
        if(!showVideoWindow->IsShown())
            showVideoWindow->Show(true);
        
        isVideo = true;
        isAnimation = false;
        isBitmap = false;
        
        int width = 0, height = 0, rotation = 0;

        //Initialisation du player Video
        if(showToolbar && !bitmapInfos->IsShown())
            bitmapInfos->Show(true);
        
        bitmapInfos->SetFilename(filename);
        bitmapInfos->Refresh();
        
        libPicture.GetDimensions(filename, width, height, rotation);
        videoEffectParameter->rotation = 360 - rotation;
        showVideoWindow->SetVideo(filename);
        
        oldfilename = filename;
        this->Resize();
    }
	return 1;

}

void CPreviewWnd::TransitionEnd()
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
		mainWindow->TransitionEnd();
}

void CPreviewWnd::ImageSuivante()
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
		mainWindow->ImageSuivante();
}

void CPreviewWnd::ImagePrecedente()
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
		mainWindow->ImagePrecedente();
}

