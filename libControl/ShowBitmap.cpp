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
	wxWindowID mainViewerId, CBitmapInterface * bitmapInterface, IStatusBarInterface * statusBarInterface, CThemeParam * config)
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
		config->GetBitmapScrollbarTheme(&themeScroll);

	scrollbar = new CScrollbarWnd(this, wxID_ANY, false);

	if (config != nullptr)
	{
		config->GetBitmapToolbarTheme(&themeToolbar);
	}

	pictureToolbar = new CBitmapToolbar(this, wxID_ANY, themeToolbar);
	pictureToolbar->SetTabValue(value);

	if (config != nullptr)
		config->GetBitmapWindowTheme(&themeBitmap);

	bitmapWindow = new CBitmapWndViewer(scrollbar, BITMAPWINDOWVIEWERID, pictureToolbar, mainViewerId, themeBitmap, bitmapInterface, statusBarInterface);
	
	pictureToolbar->SetBitmapDisplayPt(bitmapWindow);

	scrollbar->SetCentralWindow(bitmapWindow, themeScroll);

	Connect(wxEVT_IDLE, wxIdleEventHandler(CShowBitmap::OnIdle));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CShowBitmap::OnSize));
    Connect(wxEVT_BITMAPDBLCLICK, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowBitmap::OnViewerDblClick));
    Connect(wxEVT_BITMAPZOOMIN, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowBitmap::OnViewerZoomIn));
    Connect(wxEVT_BITMAPZOOMOUT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowBitmap::OnViewerZoomOut));
	Connect(wxEVENT_ONSTARTLOADINGPICTURE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowBitmap::StartLoadingPicture));
	Connect(wxEVENT_ONSTOPLOADINGPICTURE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowBitmap::StopLoadingPicture));
    
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

CBitmapWndViewer * CShowBitmap::GetBitmapViewer()
{
	return bitmapWindow;
}

/*
void CShowBitmap::ShowCropButton()
{
	if (pictureToolbar != nullptr)
		pictureToolbar->ShowCropButton();
}

void CShowBitmap::HideCropButton()
{
	if (pictureToolbar != nullptr)
		pictureToolbar->HideCropButton();

}
*/

CShowBitmap::~CShowBitmap()
{
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
			int pictureWidth = width;
			int pictureHeight = height - pictureToolbar->GetHeight();

			scrollbar->SetSize(0, 0, pictureWidth, pictureHeight);
			pictureToolbar->SetSize(0, height - pictureToolbar->GetHeight(), width, pictureToolbar->GetHeight());
		}
		else
		{
			scrollbar->SetSize(0, 0, width, height);

		}
	}

	if(pictureToolbar != nullptr)
	{
		if (pictureToolbar->IsShown())
			pictureToolbar->PostSizeEvent();
	}


	if(scrollbar != nullptr)
		scrollbar->PostSizeEvent();
}

void CShowBitmap::SetBitmapPreviewEffect(const int &effect)
{
	if (bitmapWindow != nullptr)
	{
		bitmapWindow->SetBitmapPreviewEffect(effect);
	}
}

void CShowBitmap::SendEmail()
{
	if (bitmapWindow != nullptr)
	{
		bitmapWindow->SendEmail();
	}
}


bool CShowBitmap::GetShrinkImage()
{
	if (bitmapWindow != nullptr)
		return bitmapWindow->GetShrinkImage();

	return false;
}

void CShowBitmap::PrintPicture()
{
	if (bitmapWindow != nullptr)
	{
		bitmapWindow->PrintPicture();
	}
}

void CShowBitmap::SetShrinkImage(const bool &value)
{
	if (bitmapWindow != nullptr)
		bitmapWindow->SetShrinkImage(value);
}

void CShowBitmap::ShrinkImage()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->ShrinkImage();
}

bool CShowBitmap::SetPhoto(const wxString &filename)
{
	CLibPicture libPicture;
	bool returnValue = false;
	CImageLoadingFormat * pBitmap = libPicture.LoadPicture(filename.c_str());
	if (pBitmap != nullptr)
    {
		returnValue = SetBitmap(pBitmap, false);
    }
	if (!pBitmap->IsOk())
	{
		if (pBitmap != nullptr)
			delete pBitmap;
#ifdef WIN32
		wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "\\photo_cancel.png";
#else
		wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "/photo_cancel.png";
#endif
		pBitmap = libPicture.LoadPicture(photoCancel);
	}

	return returnValue;
}

void CShowBitmap::StartLoadingPicture(wxCommandEvent& event)
{
    if (bitmapWindow != nullptr)
    {
        bitmapWindow->StartLoadingBitmap();
    }
}

void CShowBitmap::StopLoadingPicture(wxCommandEvent& event)
{
    if (bitmapWindow != nullptr)
    {
        bitmapWindow->StopLoadingBitmap();
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

int CShowBitmap::GetBitmapWidth()
{
	if (bitmapWindow != nullptr)
		return bitmapWindow->GetBitmapWidth(); 

	return -1;
}

int CShowBitmap::GetBitmapHeight()
{
	if (bitmapWindow != nullptr)
		return bitmapWindow->GetBitmapHeight();
	
	return -1;
}

void CShowBitmap::OnSize(wxSizeEvent& event)
{
    
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	Resize();
}

void CShowBitmap::DeplacementDroite()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->MoveRight();
}

void CShowBitmap::DeplacementGauche()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->MoveLeft();
}

void CShowBitmap::DeplacementHaut()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->MoveTop();
}

void CShowBitmap::DeplacementBas()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->MoveBottom();
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

void CShowBitmap::ZoomOn()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->ZoomOn();
}

void CShowBitmap::ZoomOut()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->ZoomOut();
}

