#include "PanelPreview.h"
#include <libPicture.h>
#include <RegardsBitmap.h>
#include <EffectVideoParameter.h>
#include "ExplorerThemeInit.h"
#include "ExplorerTheme.h"
#include <effect.h>
#include "CentralWnd.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include "eventid.h"
#include <SqlThumbnail.h>
using namespace Regards::Window;
using namespace Regards::Explorer;
using namespace Regards::Sqlite;

#define PANE_INFOS 2
#define PANE_PREVIEW 3
#define WM_STOPPLAYVIDEO 5
#define WM_SHOWPICTURE 6
wxDEFINE_EVENT(EVENT_POSTPICTURE, wxCommandEvent);

CPanelPreview::CPanelPreview(wxWindow* parent, wxWindowID id, CVideoEffectParameter * videoEffectParameter, IStatusBarInterface * statusBarInterface, CFileGeolocation * fileGeolocalisation, const bool &horizontal)
	: CWindowMain(parent, id)
{
	config = CParamInit::getInstance();

	CExplorerTheme * explorerTheme = CExplorerThemeInit::getInstance();

	if (explorerTheme != nullptr)
	{
		CThemeBitmapInfos theme;
		explorerTheme->GetBitmapInfosTheme(theme);
		bitmapInfos = new CBitmapInfos(this, wxID_ANY, theme, fileGeolocalisation);
	}

	if (explorerTheme != nullptr)
	{
		CThemeToolbar theme;
		explorerTheme->GetViewerToolbarTheme(theme);
		previewToolbar = new CPreviewToolbar(this, wxID_ANY, theme, this);
	}
    
    if (explorerTheme != nullptr)
        explorerTheme->GetBitmapWindowTheme(&themeBitmap);

    m_animationCtrl = new wxAnimationCtrl(this, wxID_ANY);
    
	Connect(wxEVT_PAINT, wxPaintEventHandler(CPanelPreview::OnPaint));
    Connect(EVENT_POSTPICTURE, wxCommandEventHandler(CPanelPreview::DisplayPicture));
    
	CMainWindow * mainWnd = (CMainWindow *)this->FindWindowById(MAINEXPLORERWINDOWID);
	showBitmapWindow = new CShowBitmap(this, SHOWBITMAPEXPLORERID, BITMAPWINDOWEXPLORERID, mainWnd, this, statusBarInterface, explorerTheme);
	showVideoWindow = new CShowVideo(this, SHOWVIDEOEXPLORERID, mainWnd, videoEffectParameter, explorerTheme);
	
	isVideo = false;
	this->videoEffectParameter = videoEffectParameter;

    m_animationCtrl->Show(false);
	showVideoWindow->Show(false);
	bitmapInfos->Show(false);
}

void CPanelPreview::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    wxRect rc = GetRect();
    this->FillRect(&dc, rc, themeBitmap.colorFullscreen);

	if (m_animationCtrl->IsShown())
		m_animationCtrl->SetBackgroundColour(themeBitmap.colorFullscreen);

}

void CPanelPreview::DisplayPicture(wxCommandEvent& event)
{
    CThreadPictureData * pictureData = (CThreadPictureData *)event.GetClientData();
    if(pictureData->picture == filenameToShow)
    {
        showBitmapWindow->SetBitmap(pictureData->bitmap);
    }
    else
    {
        delete pictureData->bitmap;
    }
    pictureData->threadPicture->join();
    delete pictureData->threadPicture;
    delete pictureData;
    
    CCentralWnd * mainWnd = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
    if (mainWnd != nullptr)
        mainWnd->StopLoadingPicture();
}

//Ajout d'un thread pour le chargement des photos

void CPanelPreview::LoadPicture(CThreadPictureData * pictureData)
{
    CLibPicture libPicture;
    pictureData->bitmap = libPicture.LoadPicture(pictureData->picture);
    
    if (pictureData->bitmap != nullptr)
    {
        pictureData->bitmap->SetFilename(pictureData->picture);
    }
    
    wxCommandEvent event(EVENT_POSTPICTURE);
    event.SetClientData(pictureData);
    wxPostEvent(pictureData->panelPreview, event);
    
}


CPanelPreview::~CPanelPreview()
{
    delete(m_animationCtrl);
	delete(showBitmapWindow);
	delete(showVideoWindow);
	delete(previewToolbar);
	delete(bitmapInfos);
}

void CPanelPreview::ClickShowButton(const int &id)
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

	case WM_IMAGES_FIRST:
		ImageDebut();
		break;

	case WM_IMAGES_PRCDENTE:
		ImagePrecedente();
		break;

	case WM_IMAGES_SUIVANTE:
		ImageSuivante();
		break;

	case WM_IMAGES_END:
		ImageFin();
		break;
	}
}

void CPanelPreview::Resize()
{
	if (previewToolbar->IsShown())
	{
		wxRect rcAffichageBitmap;
		int bitmapInfosHeight = bitmapInfos->GetHeight();
		//if (!showBitmapWindow->IsShown())
		//	bitmapInfosHeight = 0;

		rcAffichageBitmap.x = 0;
		rcAffichageBitmap.y = 0;
		rcAffichageBitmap.width = width;
		rcAffichageBitmap.height = height - previewToolbar->GetHeight();

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
            int yPos = bitmapInfosHeight + ((height - bitmapInfosHeight) - animationSize.GetHeight()) / 2;
            
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

		previewToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height, rcAffichageBitmap.width, previewToolbar->GetHeight());

	}
	else
	{
		if (showBitmapWindow->IsShown())
			showBitmapWindow->SetSize(0, 0, width, height);
        else if(m_animationCtrl->IsShown())
            m_animationCtrl->SetSize(0, 0, width, height);
		else
			showVideoWindow->SetSize(0, 0, width, height);
	}

	//needToRedraw = true;
}


void CPanelPreview::HideToolbar()
{
    showToolbar = false;
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

void CPanelPreview::ShowToolbar()
{
    showToolbar = true;
	if (previewToolbar != nullptr)
	{
        bitmapInfos->Show(true);
        
		if (!previewToolbar->IsShown())
			previewToolbar->Show(true);
        
        if (isVideo)
            showVideoWindow->ShowToolbar();
        else if(isBitmap)
            showBitmapWindow->ShowToolbar();


		Resize();
	}
}

bool CPanelPreview::IsToolbarVisible()
{
	if (previewToolbar != nullptr)
	{
		return previewToolbar->IsShown();
	}
	return false;
}

void CPanelPreview::UpdateScreenRatio()
{
    showBitmapWindow->UpdateScreenRatio();
    showVideoWindow->UpdateScreenRatio();
    previewToolbar->UpdateScreenRatio();
    bitmapInfos->UpdateScreenRatio();
    this->Resize();
}


bool CPanelPreview::SetPhoto(const wxString &filename)
{
	if (filename != filenameToShow)
	{
		filenameToShow = filename;
		if (isVideo)
			showVideoWindow->StopVideo();
        else if(isAnimation)
            m_animationCtrl->Stop();
		else
			showBitmapWindow->StopTransition();
        
        //loading thumbnail
        CSqlThumbnail sqlThumbnail;
        CRegardsBitmap * bitmap = sqlThumbnail.GetPictureThumbnail(filename);
        if(bitmap == nullptr)
        {
            //Chargement du thumbnail de l'image
            CLibPicture libPicture;
            bitmap = libPicture.LoadThumbnail(filename, true);
        }
        
        
        int numEffect = 0;
        
        if (config != nullptr)
            numEffect = config->GetEffect();
        
        if(bitmap != nullptr)
            showBitmapWindow->SetBitmap(bitmap);
        
        m_animationCtrl->Show(false);
        showVideoWindow->Show(false);
        
        if(!showBitmapWindow->IsShown())
            showBitmapWindow->Show(true);
        
        isVideo = false;
        isAnimation = false;
        isBitmap = true;

        if(showToolbar && !bitmapInfos->IsShown())
            bitmapInfos->Show(true);
        
		bitmapInfos->SetFilename(filename);
		bitmapInfos->Refresh();
        
        CThreadPictureData * pictureData = new CThreadPictureData();
        pictureData->panelPreview = this;
        pictureData->picture = filenameToShow;
        pictureData->threadPicture = new thread(LoadPicture, pictureData);
        
		Resize();
	}
	return true;
}

bool CPanelPreview::SetAnimation(const wxString &filename)
{
    if (filename != filenameToShow)
    {
        if (isVideo)
            showVideoWindow->StopVideo();
        else if(isBitmap)
            showBitmapWindow->StopTransition();
        
        showVideoWindow->Show(false);
        showBitmapWindow->Show(false);
        
        if(showToolbar && !bitmapInfos->IsShown())
            bitmapInfos->Show(true);
        
        bitmapInfos->SetFilename(filename);
        bitmapInfos->Refresh();
        
        isVideo = false;
        isBitmap = false;
        
        if(isAnimation)
            m_animationCtrl->Stop();
        
        m_animationCtrl->LoadFile(filename);
        m_animationCtrl->Play();
        
        
        isAnimation = true;
        filenameToShow = filename;
        
        this->Resize();
    }
    return true;
}


bool CPanelPreview::SetVideo(const wxString &filename)
{
	if (filename != filenameToShow)
	{
		CLibPicture libPicture;

        if(isBitmap)
            showBitmapWindow->StopTransition();
        else if(isAnimation)
            m_animationCtrl->Stop();

        showBitmapWindow->Show(false);
        m_animationCtrl->Show(false);
        
        if (!showVideoWindow->IsShown())
            showVideoWindow->Show(true);

        if(showToolbar && !bitmapInfos->IsShown())
            bitmapInfos->Show(true);
        
		bitmapInfos->SetFilename(filename);
		//
		int width = 0, height = 0, rotation = 0;

		//Initialisation du player Video
		libPicture.GetDimensions(filename, width, height, rotation);
		videoEffectParameter->rotation = 360 - rotation;

		if (isVideo)
            showVideoWindow->StopVideo();
			
        showVideoWindow->SetVideo(filename);

        filenameToShow = filename;
		isVideo = true;
        isBitmap = false;
        isAnimation = false;
		Resize();
        bitmapInfos->Refresh();
	}
	return true;
}

void CPanelPreview::TransitionEnd()
{
	CCentralWnd * mainWnd = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWnd != nullptr)
		mainWnd->TransitionEnd();
}

void CPanelPreview::ImageSuivante()
{
	CCentralWnd * mainWnd = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWnd != nullptr)
		mainWnd->ImageSuivante();
}

void CPanelPreview::ImagePrecedente()
{
	CCentralWnd * mainWnd = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWnd != nullptr)
		mainWnd->ImagePrecedente();
}


void CPanelPreview::ImageDebut()
{
	CCentralWnd * mainWnd = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWnd != nullptr)
		mainWnd->ImageDebut();
}

void CPanelPreview::ImageFin()
{
	CCentralWnd * mainWnd = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWnd != nullptr)
		mainWnd->ImageFin();
}