#include <header.h>
#include "ShowBitmap.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <libPicture.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <ThemeInit.h>
#include <FileUtility.h>
#include <array>
#include <ImageLoadingFormat.h>
using namespace Regards::Window;
using namespace Regards::Control;



void CShowBitmap::SetDiaporamaMode()
{
	isDiaporama = true;
	bitmapWindow->SetDiaporamaMode();
	this->Resize();
}

void CShowBitmap::SetNormalMode()
{
	isDiaporama = false;
	bitmapWindow->SetNormalMode();
	this->Resize();
}

void CShowBitmap::SetFullscreen(const bool &fullscreen)
{
	bitmapWindow->SetFullscreen(fullscreen);
}

void CShowBitmap::UpdateScreenRatio()
{
    scrollbar->UpdateScreenRatio();
    pictureToolbar->UpdateScreenRatio();
    bitmapWindow->UpdateScreenRatio();
    this->Resize();
}

CShowBitmap::CShowBitmap(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId,
	wxWindowID mainViewerId, CBitmapInterface * bitmapInterface, CThemeParam * config)
	: wxWindow(parent, id, wxPoint(0,0), wxSize(0,0), 0)
{
	transitionEnd = false;
	tempImage = nullptr;
	scrollbar = nullptr;
	pictureToolbar = nullptr;
	bitmapWindow = nullptr;
	configRegards = nullptr;
	defaultToolbar = true;
	defaultViewer = true;
	isDiaporama = false;
	width = 0;
	height = 0;
	CThemeBitmapWindow themeBitmap;
	configRegards = CParamInit::getInstance();
	CThemeScrollBar themeScroll;
	CThemeToolbar themeToolbar;
	std::vector<int> value = { 1, 2, 3, 4, 5, 6, 8, 12, 16, 25, 33, 50, 66, 75, 100, 133, 150, 166, 200, 300, 400, 500, 600, 700, 800, 1200, 1600 };

	this->bitmapInterface = bitmapInterface;

	if (config != nullptr)
		config->GetScrollTheme(&themeScroll);

	scrollbar = new CScrollbarWnd(this, wxID_ANY, "BitmapScroll", false);

	if (config != nullptr)
	{
		config->GetBitmapToolbarTheme(&themeToolbar);
	}

	pictureToolbar = new CBitmapToolbar(this, wxID_ANY, themeToolbar, false);
	pictureToolbar->SetTabValue(value);

	if (config != nullptr)
		config->GetBitmapWindowTheme(&themeBitmap);

	bitmapWindow = new CBitmapWndViewer(scrollbar, BITMAPWINDOWVIEWERID, pictureToolbar, mainViewerId, themeBitmap, bitmapInterface);
	
    loadingTimer = new wxTimer(this, wxTIMER_REFRESH);
	//pictureToolbar->SetBitmapDisplayPt(bitmapWindow);
	scrollbar->SetCentralWindow(bitmapWindow, themeScroll);
	progressBar = new wxGauge(this, wxID_ANY, 200, wxPoint(1000, 0), wxSize(200, 10), wxGA_HORIZONTAL);
	progressBar->SetRange(100);
	progressBar->SetValue(0);    
    
    Connect(wxTIMER_REFRESH, wxEVT_TIMER, wxTimerEventHandler(CShowBitmap::OnTimerRefresh), nullptr, this);
	Connect(wxEVT_IDLE, wxIdleEventHandler(CShowBitmap::OnIdle));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CShowBitmap::OnSize));
    Connect(wxEVT_BITMAPDBLCLICK, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowBitmap::OnViewerDblClick));
    Connect(wxEVT_BITMAPZOOMIN, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowBitmap::OnViewerZoomIn));
    Connect(wxEVT_BITMAPZOOMOUT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowBitmap::OnViewerZoomOut));
	Connect(wxEVENT_ONSTARTLOADINGPICTURE, wxCommandEventHandler(CShowBitmap::StartLoadingPicture));
	Connect(wxEVENT_ONSTOPLOADINGPICTURE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowBitmap::StopLoadingPicture));
    progressValue = 0;
}

void CShowBitmap::OnTimerRefresh(wxTimerEvent& event)
{
    progressValue += 5;
    progressBar->SetValue(progressValue);
}

void CShowBitmap::OnViewerDblClick(wxCommandEvent& event)
{
    if (pictureToolbar != nullptr)
    {
        if (pictureToolbar->IsShown())
        {
            HideToolbar();
        }
        else
        {
            ShowToolbar();
        }
    }
}

CShowBitmap::~CShowBitmap()
{
    delete(progressBar);
	delete(pictureToolbar);
	delete(bitmapWindow);
	delete(scrollbar);
}

void CShowBitmap::StopTransition()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->StopTransition();
}

void CShowBitmap::HideToolbar()
{
	if (pictureToolbar != nullptr)
	{
		if (pictureToolbar->IsShown())
		{
			if (!pictureToolbar->IsMouseOver())
			{
				if (bitmapWindow != nullptr)
					bitmapWindow->FixArrowNavigation(false);

				pictureToolbar->Show(false);
				this->Resize();
			}
		}
	}
}

bool CShowBitmap::IsToolbarMouseOver()
{
	if (pictureToolbar != nullptr)
		return pictureToolbar->IsMouseOver();

	return false;
}

void CShowBitmap::ShowToolbar()
{
	if (pictureToolbar != nullptr)
	{
		if (!pictureToolbar->IsShown())
		{
			if (bitmapWindow != nullptr)
				bitmapWindow->FixArrowNavigation(true);

			pictureToolbar->Show(true);
			this->Resize();
		}
	}
}

void CShowBitmap::Resize()
{
	if (isDiaporama)
	{
		pictureToolbar->SetSize(0, 0, 0, 0);
		scrollbar->SetSize(0, 0, width, height);
	}
	else if (pictureToolbar != nullptr && scrollbar != nullptr)
	{
		if (pictureToolbar->IsShown())
		{
			//CDeferPos deferpos;
			if (showLoadBar)
			{
				int pictureWidth = width;
				int pictureHeight = height - pictureToolbar->GetHeight() - 10;

				scrollbar->SetSize(0, 0, pictureWidth, pictureHeight);
				progressBar->SetSize(0, height - pictureToolbar->GetHeight() - 10, width, 10);
				pictureToolbar->SetSize(0, height - pictureToolbar->GetHeight(), width, pictureToolbar->GetHeight());
			}
			else
			{
				int pictureWidth = width;
				int pictureHeight = height - pictureToolbar->GetHeight();

				scrollbar->SetSize(0, 0, pictureWidth, pictureHeight);
				pictureToolbar->SetSize(0, height - pictureToolbar->GetHeight(), width, pictureToolbar->GetHeight());
			}
		}
		else
		{
			scrollbar->SetSize(0, 0, width, height);

		}
	}
}

void CShowBitmap::SetBitmapPreviewEffect(const int &effect)
{
	if (bitmapWindow != nullptr)
	{
		bitmapWindow->SetBitmapPreviewEffect(effect);
	}
}

void CShowBitmap::StartLoadingPicture(wxCommandEvent& event)
{
    if (bitmapWindow != nullptr)
    {
		showLoadBar = true;
        bitmapWindow->StartLoadingBitmap();
		loadingTimer->Start(100);
		progressBar->SetValue(0);
		progressValue = 0;
		progressBar->Show();
		this->Resize();
    }
}

void CShowBitmap::StopLoadingPicture(wxCommandEvent& event)
{
    if (bitmapWindow != nullptr)
    {
		showLoadBar = false;
        bitmapWindow->StopLoadingBitmap();
        loadingTimer->Stop();
		progressBar->Hide();
		this->Resize();
    }
}

void CShowBitmap::TransitionEnd()
{
	transitionEnd = true;
	if(tempImage != nullptr)
	{
		bitmapWindow->SetBitmap(tempImage);
		tempImage = nullptr;
		transitionEnd = false;
		if (pictureToolbar != nullptr)
			pictureToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());
		bitmapWindow->Refresh();
	}
}

void CShowBitmap::OnIdle(wxIdleEvent& evt)
{
    //TRACE();
	int numEffect = 0;
		
	if (configRegards != nullptr)
		numEffect = configRegards->GetEffect();

	if (numEffect != 0 && transitionEnd)
	{
		if(tempImage != nullptr)
		{
			bitmapWindow->SetBitmap(tempImage);
			tempImage = nullptr;
			transitionEnd = false;
			if (pictureToolbar != nullptr)
				pictureToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());
			bitmapWindow->Refresh();
		}
	}
}

bool CShowBitmap::SetBitmap(CImageLoadingFormat * bitmap, const bool & isThumbnail)
{
	TRACE();
	if (bitmapWindow != nullptr)
	{
   
        
		//bitmapWindow->FixArrowNavigation(true);
        bitmapWindow->SetIsBitmapThumbnail(isThumbnail);
		int numEffect = 0;
		
		if (configRegards != nullptr)
			numEffect = configRegards->GetEffect();

		if (numEffect != 0)
		{
			if (isThumbnail)
			{
				transitionEnd = false;
				bitmapWindow->ShrinkImage();
				bitmapWindow->SetTransitionBitmap(bitmap);
				tempImage = nullptr;
			}
			else
			{
				//if(transitionEnd)
				bitmapWindow->StopTransition();
				bitmapWindow->SetBitmap(bitmap, false);
				//else
				//	tempImage = bitmap;
			}
		}
		else
			bitmapWindow->SetBitmap(bitmap, false);

		if (pictureToolbar != nullptr)
			pictureToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());
		//bitmapWindow->Refresh();
       
		return true;
	}
	return false;
}


CRegardsBitmap * CShowBitmap::GetBitmap(const bool &source)
{
	if (bitmapWindow != nullptr)
		return bitmapWindow->GetBitmap(source);

	return nullptr;
}

void CShowBitmap::OnSize(wxSizeEvent& event)
{
    
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	Resize();
}

void CShowBitmap::OnViewerZoomIn(wxCommandEvent& event)
{
    if(pictureToolbar != nullptr)
        pictureToolbar->ChangeZoomInPos();
}

void CShowBitmap::OnViewerZoomOut(wxCommandEvent& event)
{
    if(pictureToolbar != nullptr)
        pictureToolbar->ChangeZoomOutPos();
}

void CShowBitmap::FlipVertical()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->FlipVertical();
}

void CShowBitmap::Rotate90()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->Rotate90();
}

void CShowBitmap::Rotate270()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->Rotate270();
}

void CShowBitmap::FlipHorizontal()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->FlipHorizontal();
}
