// ReSharper disable All
#include <header.h>
#include "PreviewWnd.h"
#include "MainWindow.h"
#include <libPicture.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <ImageLoadingFormat.h>
#include "FiltreToolbar.h"
#include "PreviewToolbar.h"
#include "AnimationToolbar.h"
#include <BitmapInfos.h>
#include <wx/animate.h>
#include "PanelInfosWnd.h"
#include <ShowElement.h>
using namespace Regards::Video;
using namespace Regards::Control;
using namespace Regards::Internet;
using namespace Regards::Viewer;
using namespace Regards::Picture;

//constexpr auto PANE_INFOS = 2;
//constexpr auto PANE_PREVIEW = 3;

CPreviewWnd::CPreviewWnd(wxWindow* parent, wxWindowID id, const bool& horizontal)
	: CWindowMain("CPreviewWnd", parent, id), id_(id), horizontal_(horizontal), parent_(parent)
{
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
	CMainTheme* viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeBitmapInfos theme;
		viewerTheme->GetBitmapInfosTheme(&theme);
		bitmapInfos = new CBitmapInfos(this, wxID_ANY, theme);
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
		viewerTheme->GetBitmapWindowTheme(&themeBitmap);
	}

	showElement = new CShowElement(this, SHOWBITMAPVIEWERID, BITMAPWINDOWVIEWERID, MAINVIEWERWINDOWID, this, viewerTheme, true);


	isVideo = false;
	animationToolbar->Show(true);
	showElement->Show(true);
	filtreToolbar->Show(false);
	bitmapInfos->Show(true);
	previewToolbar->Show(true);

	Connect(wxEVENT_SHOWSCREENBUTTON, wxCommandEventHandler(CPreviewWnd::ShowScreenButton));
	Connect(wxEVENT_HIDESCREENBUTTON, wxCommandEventHandler(CPreviewWnd::DisableScreenButton));
	Connect(wxEVENT_SHOWSAVEBUTTON, wxCommandEventHandler(CPreviewWnd::ShowSaveButton));
	Connect(wxEVENT_HIDESAVEBUTTON, wxCommandEventHandler(CPreviewWnd::HideSaveButton));
	Connect(wxEVENT_HIDETOOLBAR, wxCommandEventHandler(CPreviewWnd::HideToolbar));
	Connect(wxEVENT_SHOWTOOLBAR, wxCommandEventHandler(CPreviewWnd::ShowToolbar));
	Connect(wxEVENT_FILTREOK, wxCommandEventHandler(CPreviewWnd::OnFiltreOK));
	Connect(wxEVENT_FILTRECANCEL, wxCommandEventHandler(CPreviewWnd::OnFiltreCancel));
	Connect(wxEVENT_SHOWTOOLBARFILTRE, wxCommandEventHandler(CPreviewWnd::OnShowToolbar));
	Connect(wxEVENT_ANIMATIONSTART, wxCommandEventHandler(CPreviewWnd::StartAnimation));
}


CPreviewWnd::~CPreviewWnd()
{
	delete(showElement);
	delete(previewToolbar);
	delete(animationToolbar);
	delete(bitmapInfos);
	delete(filtreToolbar);
}

void CPreviewWnd::StartAnimation(wxCommandEvent& event)
{
	if (animationToolbar != nullptr)
		animationToolbar->AnimationStart();
	this->Resize();
}

void CPreviewWnd::HideSaveButton(wxCommandEvent& event)
{
	if (previewToolbar != nullptr)
		previewToolbar->DisableSaveButton();
}

void CPreviewWnd::ShowSaveButton(wxCommandEvent& event)
{
	if (previewToolbar != nullptr)
		previewToolbar->EnableSaveButton();
}

void CPreviewWnd::ShowScreenButton(wxCommandEvent& event)
{
	if (previewToolbar != nullptr)
		previewToolbar->EnableScreenButton();
}

void CPreviewWnd::DisableScreenButton(wxCommandEvent& event)
{
	if (previewToolbar != nullptr)
		previewToolbar->DisableScreenButton();
}

void CPreviewWnd::OnShowToolbar(wxCommandEvent& event)
{
	int numItem = event.GetInt();
	ShowValidationToolbar(true, numItem);
}

void CPreviewWnd::OnFiltreOK(wxCommandEvent& event)
{
	auto panelInfos = static_cast<CPanelInfosWnd*>(this->FindWindowById(PANELINFOSWNDID));
	int numFiltre = event.GetInt();
	if (panelInfos != nullptr)
		panelInfos->OnFiltreOk(numFiltre);
	this->HideValidationToolbar();
}

/*
void CPreviewWnd::ReloadResourceVideo()
{
	showElement->ReloadResource();
}

void CPreviewWnd::ReloadResourceBitmap()
{
	showElement->ReloadResource();
}
*/
void CPreviewWnd::OnFiltreCancel(wxCommandEvent& event)
{
	auto panelInfos = static_cast<CPanelInfosWnd*>(this->FindWindowById(PANELINFOSWNDID));
	//int numFiltre = event.GetInt();
	if (panelInfos != nullptr)
		panelInfos->OnFiltreCancel();
	this->HideValidationToolbar();
}

void CPreviewWnd::ClickShowButton(const int& id, const int& refresh)
{
	switch (id)
	{
	case WM_ROTATE90:
		showElement->Rotate90();
		break;

	case WM_ROTATE270:
		showElement->Rotate270();
		break;

	case WM_FLIPVERTICAL:
		showElement->FlipVertical();
		break;

	case WM_FLIPHORIZONTAL:
		showElement->FlipHorizontal();
		break;

	case WM_SAVE:
		showElement->SavePicture();
		break;
	default: ;
	}
}

void CPreviewWnd::Resize()
{
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

			if (!showElement->IsShown() && !animationToolbar->IsShown())
				bitmapInfosHeight = 0;

			wxSize size = this->GetSize();
			//rcAffichageBitmap = this->GetRect();

			rcAffichageBitmap.x = 0;
			//rcAffichageBitmap.width = GetWindowWidth();
			//rcAffichageBitmap.height = GetWindowHeight() - toolbarHeightSize;
			rcAffichageBitmap.width = size.x;
			rcAffichageBitmap.height = size.y - toolbarHeightSize;

			if (!isVideo)
			{
				if (animationToolbar->IsShown())
				{
					rcAffichageBitmap.height = GetWindowHeight() - toolbarHeightSize - toolbarAnimationHeightSize;
				}

				if (bitmapInfos->IsShown())
				{
					bitmapInfos->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, bitmapInfosHeight);
				}
				else
					bitmapInfosHeight = 0;

				showElement->SetSize(rcAffichageBitmap.x, bitmapInfosHeight, rcAffichageBitmap.width,
					rcAffichageBitmap.height - bitmapInfosHeight);
			}
			else
			{
				if (bitmapInfos->IsShown())
				{
					bitmapInfos->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, bitmapInfosHeight);
				}
				else
					bitmapInfosHeight = 0;

				showElement->SetSize(rcAffichageBitmap.x, bitmapInfosHeight, rcAffichageBitmap.width,
					rcAffichageBitmap.height - bitmapInfosHeight);
			}

			if (filtreToolbar->IsShown())
			{
				filtreToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height, rcAffichageBitmap.width,
					toolbarHeightSize);
			}
			else
			{
				if (animationToolbar->IsShown())
				{
					animationToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height, rcAffichageBitmap.width,
						toolbarAnimationHeightSize);
					animationToolbar->Refresh();
					previewToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height + toolbarAnimationHeightSize,
						rcAffichageBitmap.width, toolbarHeightSize);
				}
				else
				{
					previewToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height, rcAffichageBitmap.width,
						toolbarHeightSize);
				}
			}

			return;
		}
	}


	showElement->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());

}

void CPreviewWnd::SetDiaporamaMode()
{
	previewToolbar->EnableNavigationButton();
	isDiaporama = true;
	showElement->SetDiaporamaMode();
	this->Resize();
}

void CPreviewWnd::StopDiaporamaMode()
{
	previewToolbar->EnableNavigationButton();
	isDiaporama = false;
	showElement->SetNormalMode();
	this->Resize();
}

void CPreviewWnd::SetNormalMode()
{
	previewToolbar->EnableNavigationButton();
	this->Resize();
}

void CPreviewWnd::SetFaceMode()
{
	previewToolbar->DisableNavigationButton();
}

void CPreviewWnd::SetEffect(const bool& effect)
{
	//showElement->SetEffect(effect);
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
			bool previewToolbarMouseOver;
			previewToolbarMouseOver = showElement->IsToolbarMouseOver();

			if (!previewToolbarMouseOver)
			{
				if (previewToolbar->IsShown())
					previewToolbar->Show(false);

				if (animationToolbar->IsShown())
					animationToolbar->Show(false);

				if (isVideo)
					bitmapInfos->Show(false);
				else if (isBitmap)
					bitmapInfos->Show(false);
				else
					bitmapInfos->Show(false);

				showElement->HideToolbar();
				Resize();
			}
		}
	}
}

void CPreviewWnd::ShowValidationToolbar(const bool& visible, const int& filtre)
{
	isEffect = visible;
	filtreToolbar->SetNumFiltre(filtre);
	if (previewToolbar != nullptr)
	{
		if (previewToolbar->IsShown())
			previewToolbar->Show(false);
	}

	if (animationToolbar != nullptr)
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
	showElement->SetBitmapPreviewEffect(0);
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
				showElement->ShowToolbar();
			else if (isBitmap)
			{
				if (isAnimation)
					if (!animationToolbar->IsShown())
						animationToolbar->Show(true);
			}

			showElement->ShowToolbar();
			showElement->ShowToolbar();
			Resize();
		}
	}
}

void CPreviewWnd::UpdateScreenRatio()
{
	showElement->UpdateScreenRatio();
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

void CPreviewWnd::SetFullscreen(const bool& fullscreen)
{
	this->fullscreen = fullscreen;
	showElement->SetFullscreen(fullscreen);

	if (fullscreen)
		previewToolbar->SetFullscreen();
	else
		previewToolbar->SetScreen();

	Resize();
}

void CPreviewWnd::UpdateInfos()
{
	if (bitmapInfos != nullptr)
		bitmapInfos->UpdateData();
}

void CPreviewWnd::IsNextPicture(const bool& value)
{
	showElement->IsNextPicture(value);
}

bool CPreviewWnd::SetBitmap(CImageLoadingFormat* bitmap, const bool& isThumbnail, const bool& isAnimation)
{
#if defined(WIN32) && defined(_DEBUG)
	OutputDebugString(L"CPreviewWnd::SetBitmap");
	OutputDebugString(L"\n");
	OutputDebugString(bitmap->GetFilename());
	OutputDebugString(L"\n");
#endif

	if (isThumbnail && isDiaporama)
	{
		return true;
	}

	if (bitmap != nullptr && bitmap->IsOk())
	{
		wxString filename = bitmap->GetFilename();
		if (oldfilename != filename)
		{
			this->isAnimation = isAnimation;
			filtreToolbar->Show(false);

			showElement->StopTransition();

			if (!isAnimation)
				animationToolbar->Show(false);
			else if (showToolbar)
				animationToolbar->Show(true);

			if (!showElement->IsShown())
			{
				showElement->ReloadResource();
				showElement->Show(true);
			}

			if (showToolbar && !bitmapInfos->IsShown())
				bitmapInfos->Show(true);

			bitmapInfos->SetFilename(filename);
			//bitmapInfos->Refresh();

			isVideo = false;
			isBitmap = true;

			showElement->SetBitmap(bitmap, isThumbnail);
			oldfilename = filename;

			if (isOldState != 1)
			{
				this->Resize();
			}
			isOldState = 1;
		}
		else if (!isThumbnail)
		{
			showElement->SetBitmap(bitmap, isThumbnail);
		}
	}
	return true;
}


void CPreviewWnd::StopVideo()
{
	showElement->StopVideo("");
}

bool CPreviewWnd::SetVideo(const wxString& filename, const bool& play)
{
	if (oldfilename != filename)
	{
		CLibPicture libPicture;

		//Test if video is valid
		bool iValid = libPicture.TestIsVideoValid(filename);
		if (iValid)
		{
			filtreToolbar->Show(false);

			if (isBitmap)
				showElement->StopTransition();

			if (animationToolbar->IsShown())
				animationToolbar->Show(false);

			if (!showElement->IsShown())
			{
				showElement->ReloadResource();
				showElement->Show(true);
			}


			isVideo = true;
			isBitmap = false;
			isAnimation = false;
			int width = 0, height = 0;
			//Initialisation du player Video
			if (showToolbar && !bitmapInfos->IsShown())
				bitmapInfos->Show(true);

			bitmapInfos->SetFilename(filename);
			bitmapInfos->Refresh();

			int rotation = 0;
			libPicture.GetPictureDimensions(filename, width, height, rotation);
			showElement->SetVideo(filename, rotation, play);
			oldfilename = filename;
			// 
			if (isOldState != 2)
			{
				this->Resize();
				//this->ForceRefresh();
			}
			isOldState = 2;

			//wxMicroSleep(500);
		}
		else
		{
			CImageLoadingFormat* image = libPicture.GetCancelPhoto(filename);
			SetBitmap(image, false);
		}
	}
	return true;
}

void CPreviewWnd::TransitionEnd()
{
	auto mainWindow = static_cast<CMainWindow*>(this->FindWindowById(MAINVIEWERWINDOWID));
	if (mainWindow != nullptr)
		mainWindow->TransitionEnd();
}

void CPreviewWnd::ImageSuivante()
{
	wxWindow* mainWindow = this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_PICTURENEXT);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CPreviewWnd::ImagePrecedente()
{
	wxWindow* mainWindow = this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_PICTUREPREVIOUS);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}
