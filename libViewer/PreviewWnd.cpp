#include <header.h>
#include "PreviewWnd.h"
#include "MainWindow.h"
#ifdef FFMPEG
#include <ShowVideo.h>
#endif
#include <libPicture.h>
#include <window_id.h>
#include "ViewerParam.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <ImageLoadingFormat.h>
using namespace Regards::Control;
using namespace Regards::Viewer;

#define PANE_INFOS 2
#define PANE_PREVIEW 3


CPreviewWnd::CPreviewWnd(wxWindow* parent, wxWindowID id, CFileGeolocation * fileGeolocalisation, const bool &horizontal)
	: CWindowMain("CPreviewWnd",parent, id)
{
	showBitmapWindow = nullptr;
	showVideoWindow = nullptr;
	previewToolbar = nullptr;
	bitmapInfos = nullptr;
	filtreToolbar = nullptr;
	isBitmap = false;
	isVideo = false;
	isEffect = false;
	isAnimation = false;
    showToolbar = true;
	fullscreen = false;
	isDiaporama = false;
	animationToolbar = nullptr;
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();
	
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
		previewToolbar = new CPreviewToolbar(this, wxID_ANY, theme, this, false);
		animationToolbar = new CAnimationToolbar(this, ANIMATIONTOOLBARWINDOWID, theme, this, false);
	}

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetFiltreToolbarTheme(&theme);
		filtreToolbar = new CFiltreToolbar(this, wxID_ANY, theme, false);
	}
    
    if (viewerTheme != nullptr)
        viewerTheme->GetBitmapWindowTheme(&themeBitmap);




	CMainWindow * mainWnd = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	showBitmapWindow = new CShowBitmap(this, SHOWBITMAPVIEWERID, BITMAPWINDOWVIEWERID, MAINVIEWERWINDOWID, this, viewerTheme);
	showVideoWindow = new CShowVideo(this, SHOWVIDEOVIEWERID, mainWnd, viewerTheme);
    Connect(wxEVT_PAINT, wxPaintEventHandler(CPreviewWnd::OnPaint));
    
    
	isVideo = false;
	animationToolbar->Show(false);
	showVideoWindow->Show(false);
	filtreToolbar->Show(false);
	bitmapInfos->Show(false);
	previewToolbar->Show(false);

	Connect(wxEVENT_HIDETOOLBAR, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPreviewWnd::HideToolbar));
	Connect(wxEVENT_SHOWTOOLBAR, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPreviewWnd::ShowToolbar));
}


CPreviewWnd::~CPreviewWnd()
{
	delete(showBitmapWindow);
	delete(showVideoWindow);
	delete(previewToolbar);
	delete(animationToolbar);
	delete(bitmapInfos);
	delete(filtreToolbar);

}


void CPreviewWnd::OnPaint(wxPaintEvent& event)
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();
    if(width == 0 || height == 0)
        return;
    
    wxPaintDC dc(this);
    wxRect rc = GetRect();
    if(fullscreen)
    {
        this->FillRect(&dc, rc, themeBitmap.colorFullscreen);
    }
    else
    {
        this->FillRect(&dc, rc, themeBitmap.colorScreen);
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
	//int widthScreen = GetWindowWidth();
	//int width = this->GetSize().x;
	//int heightScreen = GetWindowHeight();
	//int height = this->GetSize().y;


	if (isDiaporama)
	{ 
		wxRect rcAffichageBitmap;
		int bitmapInfosHeight = 0;
		rcAffichageBitmap.x = 0;
		rcAffichageBitmap.y = 0;
		rcAffichageBitmap.width = GetWindowWidth();
		rcAffichageBitmap.height = GetWindowHeight();

        if(bitmapInfos != nullptr)
            bitmapInfos->SetSize(0, 0, 0, 0);
        if(filtreToolbar != nullptr)
            filtreToolbar->SetSize(0, 0, 0, 0);
        if(previewToolbar != nullptr)
            previewToolbar->SetSize(0, 0, 0, 0);
		if(animationToolbar != nullptr)
			animationToolbar->SetSize(0, 0, 0, 0);

        if(showBitmapWindow != nullptr)
        {
            if (showBitmapWindow->IsShown())
			{
                showBitmapWindow->SetSize(rcAffichageBitmap.x, bitmapInfosHeight, rcAffichageBitmap.width, rcAffichageBitmap.height - bitmapInfosHeight);
				//showBitmapWindow->SendSizeEvent();
			}
            else if(showVideoWindow->IsShown())
			{
                showVideoWindow->SetSize(rcAffichageBitmap.x, bitmapInfosHeight, rcAffichageBitmap.width, rcAffichageBitmap.height - bitmapInfosHeight);
				//showVideoWindow->SendSizeEvent();
			}
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
			int toolbarAnimationHeightSize = animationToolbar->GetHeight();
            //if (filtreToolbar->IsShown())
            //    toolbarHeightSize = filtreToolbar->GetHeight();
            
            if (!showBitmapWindow->IsShown() && !showVideoWindow->IsShown() && !animationToolbar->IsShown())
                bitmapInfosHeight = 0;

            rcAffichageBitmap.x = 0;
            rcAffichageBitmap.y = 0;
            rcAffichageBitmap.width = GetWindowWidth();
            rcAffichageBitmap.height = GetWindowHeight() - toolbarHeightSize;

            if (showBitmapWindow->IsShown())
            {
				if (animationToolbar->IsShown())
				{
					rcAffichageBitmap.height = GetWindowHeight() - toolbarHeightSize - toolbarAnimationHeightSize;
				}

                if (bitmapInfos->IsShown())
                    bitmapInfos->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, bitmapInfosHeight);
                else
                    bitmapInfosHeight = 0;

                showBitmapWindow->SetSize(rcAffichageBitmap.x, bitmapInfosHeight, rcAffichageBitmap.width, rcAffichageBitmap.height - bitmapInfosHeight);
				//showBitmapWindow->SendSizeEvent();
            }
            else
            {
                if (bitmapInfos->IsShown())
                    bitmapInfos->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, bitmapInfosHeight);
                else
                    bitmapInfosHeight = 0;

                showVideoWindow->SetSize(rcAffichageBitmap.x, bitmapInfosHeight, rcAffichageBitmap.width, rcAffichageBitmap.height - bitmapInfosHeight);
				//showVideoWindow->SendSizeEvent();
            }

            if (filtreToolbar->IsShown())
                filtreToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height, rcAffichageBitmap.width, toolbarHeightSize);
			else
			{
				if (animationToolbar->IsShown())
				{
					animationToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height, rcAffichageBitmap.width, toolbarAnimationHeightSize);
					previewToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height + toolbarAnimationHeightSize, rcAffichageBitmap.width, toolbarHeightSize);
				}
				else
					previewToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height, rcAffichageBitmap.width, toolbarHeightSize);
			}
            
            return;
        }
	}

		
    if (showBitmapWindow->IsShown())
	{
        showBitmapWindow->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
		//showBitmapWindow->SendSizeEvent();
	}
    else
        showVideoWindow->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());

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
	if (animationToolbar->IsShown())
		animationToolbar->Show(false);
	if (!filtreToolbar->IsShown())
		filtreToolbar->Show(true);
	Resize();

	
}

void CPreviewWnd::HideToolbar(wxCommandEvent& event)
{
	showToolbar = false;

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

				if (animationToolbar->IsShown())
					animationToolbar->Show(false);

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

	if(animationToolbar != nullptr)
		if (animationToolbar->IsShown())
			animationToolbar->Show(false);

	if (filtreToolbar != nullptr)
	{
		if (!filtreToolbar->IsShown())
			filtreToolbar->Show(true);
	}
	Resize();
}

void CPreviewWnd::HideValidationToolbar()
{
	isEffect = false;
	if (filtreToolbar != nullptr)
	{
		if (filtreToolbar->IsShown())
			filtreToolbar->Show(false);
	}
	showBitmapWindow->SetBitmapPreviewEffect(0);
	if (showToolbar)
	{
		wxCommandEvent event;
		ShowToolbar(event);
	}
}

void CPreviewWnd::ShowToolbar(wxCommandEvent& event)
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
			else if (isBitmap)
			{
				if (isAnimation)
					if (!animationToolbar->IsShown())
						animationToolbar->Show(true);
				showBitmapWindow->ShowToolbar();
			}

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
	animationToolbar->UpdateScreenRatio();
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

void CPreviewWnd::UpdateInfos()
{
    if(bitmapInfos != nullptr)
        bitmapInfos->UpdateData();
}

bool CPreviewWnd::SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation )
{
    TRACE();

	if(bitmap != nullptr && bitmap->IsOk())
	{
		wxString filename = bitmap->GetFilename();
		if(oldfilename != filename)
		{
        
            
            
			this->isAnimation = isAnimation;
			filtreToolbar->Show(false);

			showBitmapWindow->StopTransition();
        
			if (!isAnimation)
				animationToolbar->Show(false);
			else
				animationToolbar->Show(true);
        
			if(!showBitmapWindow->IsShown())
			   showBitmapWindow->Show(true);
        
			if(showVideoWindow->IsShown())
				showVideoWindow->Show(false);

			if(showToolbar && !bitmapInfos->IsShown())
				bitmapInfos->Show(true);
        
			bitmapInfos->SetFilename(filename);
			//bitmapInfos->Refresh();

			isVideo = false;
			isBitmap = true;
            
			showBitmapWindow->SetBitmap(bitmap, isThumbnail);
			oldfilename = filename;
        
			this->Resize();
		}
		else if(!isThumbnail)
		{
			showBitmapWindow->SetBitmap(bitmap, isThumbnail);
			this->Resize();
		}
	}
	return 1;
}


bool CPreviewWnd::SetVideo(const wxString &filename)
{
    if(oldfilename != filename)
    {
        CLibPicture libPicture;
        filtreToolbar->Show(false);

        if(isBitmap)
        {
            showBitmapWindow->StopTransition();
        }
        
        if(animationToolbar->IsShown())
			animationToolbar->Show(false);
        
        if(showBitmapWindow->IsShown())
            showBitmapWindow->Show(false);
        
        if(!showVideoWindow->IsShown())
            showVideoWindow->Show(true);
        
        isVideo = true;
        isBitmap = false;
		isAnimation = false;
		int width = 0, height = 0;
        //Initialisation du player Video
        if(showToolbar && !bitmapInfos->IsShown())
            bitmapInfos->Show(true);
        
        bitmapInfos->SetFilename(filename);
        bitmapInfos->Refresh();
        
		int rotation = 0;
        libPicture.GetPictureDimensions(filename, width, height, rotation);
        showVideoWindow->SetVideo(filename, rotation);
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

