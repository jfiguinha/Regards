#include <header.h>
#include "CentralWindow.h"
#include <ConfigParam.h>
#include "ViewerParamInit.h"
#include <LibResource.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <ImageLoadingFormat.h>
#include <window_id.h>
#include <Gps.h>
#include <libPicture.h>
#include <picture_id.h>
#include <ThumbnailMessage.h>
#include "PictureElement.h"
#include <ShowVideo.h>
#include <wx/display.h>
using namespace Regards::Window;
using namespace Regards::Viewer;


#define DELAY_ANIMATION 20


CCentralWindow::CCentralWindow(wxWindow* parent, wxWindowID id,
	const CThemeSplitter & theme, CImageList * imageList, const bool &horizontal)
	: CWindowMain("CentralWindow", parent, id)
{
	panelPhotoWnd = nullptr;
	viewerconfig = nullptr;
	isFullscreen = false;
	isDiaporama = false;
	showToolbar = true;
	videoStart = false;
	wxRect rect;
    int widthInfosWindow = 0;//wxDisplay().GetGeometry().GetWidth() / 4;

	CMainParam* config = CMainParamInit::getInstance();
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();

	if (imageList != nullptr)
	{
		photoVector = imageList->GetCopy();
	}

	windowManager = new CWindowManager(this, wxID_ANY, theme);


	//Add Criteria Window
	if (viewerTheme != nullptr)
	{
		bool isPanelVisible = true;
		if(viewerconfig != nullptr)
			viewerconfig->GetShowFilter(isPanelVisible);

		wxString libelle = CLibResource::LoadStringFromResource(L"LBLSELECTFILE", 1);
		CThemePane theme;
		viewerTheme->GetPaneTheme(&theme);

		CThemeToolbar themetoolbar;
		viewerTheme->GetClickToolbarTheme(&themetoolbar);
		
		panelPhotoWnd = new CPanelPhotoWnd(windowManager, CRITERIAFOLDERWINDOWID);
		windowManager->AddPanel(panelPhotoWnd, Pos::wxLEFT, false, widthInfosWindow, rect, libelle, "PanelPhotoSearch", true, PHOTOSEEARCHPANEL, true, true);

	}


	//----------------------------------------------------------------------------------------
	//Panel Thumbnail Video
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		bool checkValidity = true;
		bool isPanelVisible = true;
		CMainTheme* viewerTheme = CMainThemeInit::getInstance();
		CMainParam* config = CMainParamInit::getInstance();
		if (config != nullptr)
		{
			config->GetShowVideoThumbnail(isPanelVisible);
			checkValidity = config->GetCheckThumbnailValidity();
		}

		wxString libelle = CLibResource::LoadStringFromResource(L"LBLTHUMBNAILVIDEO", 1);
		CThemePane theme;
		CThemeToolbar themetoolbar;
		CThemeScrollBar themeScroll;
		CThemeThumbnail themeVideo;
		viewerTheme->GetPaneTheme(&theme);
		viewerTheme->GetClickToolbarTheme(&themetoolbar);
		viewerTheme->GetScrollTheme(&themeScroll);
		viewerTheme->GetThumbnailTheme(&themeVideo);

		thumbnailVideo = new CThumbnailViewerVideo(windowManager, THUMBNAILVIDEOWINDOW, themeVideo, checkValidity);
		scrollVideoWindow = new CScrollbarWnd(windowManager, thumbnailVideo, wxID_ANY);
		scrollVideoWindow->HideVerticalScroll();
		scrollVideoWindow->SetPageSize(200);
		scrollVideoWindow->SetLineSize(200);

		windowManager->AddPanel(scrollVideoWindow, Pos::wxTOP, true, themeVideo.themeIcone.GetHeight() + theme.GetHeight() * 2, rect, libelle, "ThumbnailVideoPanel", true, THUMBNAILVIDEOPANEL, false, true);
	}

	//----------------------------------------------------------------------------------------
	//Panel Thumbnail Picture
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		bool checkValidity = false;
		bool isPanelVisible = true;
		CMainTheme* viewerTheme = CMainThemeInit::getInstance();
		CMainParam* config = CMainParamInit::getInstance();
		if (config != nullptr)
		{
			config->GetShowThumbnail(isPanelVisible);
			checkValidity = config->GetCheckThumbnailValidity();
		}

		wxString libelle = CLibResource::LoadStringFromResource(L"LBLTHUMBNAIL", 1);
		CThemePane theme;
		CThemeToolbar themetoolbar;
		CThemeScrollBar themeScroll;
		CThemeThumbnail themeThumbnail;
		viewerTheme->GetPaneTheme(&theme);
		viewerTheme->GetScrollTheme(&themeScroll);
		viewerTheme->GetThumbnailTheme(&themeThumbnail);
		viewerTheme->GetClickToolbarTheme(&themetoolbar);

		thumbnailPicture = new CThumbnailViewerPicture(windowManager, THUMBNAILVIEWERPICTURE, themeThumbnail, checkValidity);
		scrollPictureWindow = new CScrollbarWnd(windowManager, thumbnailPicture, wxID_ANY);
		scrollPictureWindow->HideVerticalScroll();
		scrollPictureWindow->SetPageSize(200);
		scrollPictureWindow->SetLineSize(200);
		thumbnailPicture->SetNoVScroll(true);

		if (thumbnailPicture != nullptr)
			thumbnailPicture->SetListeFile(&photoVector);

		windowManager->AddPanel(scrollPictureWindow, Pos::wxBOTTOM, true, themeThumbnail.themeIcone.GetHeight() + theme.GetHeight() * 2, rect, libelle, "ThumbnailPicturePanel", true, THUMBNAILPICTUREPANEL, false, true);

	}

	bool showInfos = true;
	wxString urlServer;
	CRegardsConfigParam * param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetUrlServer();
	}

	CMainParam * viewerParam = CMainParamInit::getInstance();
	if (viewerParam != nullptr)
	{
		viewerParam->GetShowInfos(showInfos);
	}

	fileGeolocalisation = new CFileGeolocation(urlServer);

	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);
		CThemePane theme;
		viewerTheme->GetPaneTheme(&theme);
		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickToolbarTheme(&themeClickInfosToolbar);
		//paneInfos = new CPanelWithClickToolbar(this, "CPictureInfosPanel", PANELCLICKINFOSWNDID, theme, themeClickInfosToolbar, libelle, showInfos, false, true);
		panelInfosWindow = new CPanelInfosWnd(windowManager, PANELINFOSWNDID, fileGeolocalisation);

		windowManager->AddPanel(panelInfosWindow, Pos::wxRIGHT, false, widthInfosWindow, rect, libelle, "PictureInfosPanel", true, PANELCLICKINFOSWNDID, false);

	}

	previewWindow = new CPreviewWnd(windowManager, PREVIEWVIEWERID, fileGeolocalisation);
	windowManager->AddWindow(previewWindow, Pos::wxCENTRAL, false, 0, rect, PREVIEWVIEWERID, false);


	if (viewerTheme != nullptr)
	{
		listPicture = new CListPicture(windowManager, LISTPICTUREID);
		listPicture->Show(false);
		listPicture->SetListeFile(&photoVector);
	}
#ifndef __NOFACE_DETECTION__
	if (viewerTheme != nullptr)
	{
		listFace = new CListFace(windowManager, LISTFACEID);
		listFace->Show(false);
	}
#endif


	Connect(VIDEO_END_ID, wxCommandEventHandler(CCentralWindow::OnVideoEnd));
	Connect(wxEVENT_SETLISTPICTURE, wxCommandEventHandler(CCentralWindow::SetListeFile));
	Connect(wxEVENT_CHANGETYPEAFFICHAGE, wxCommandEventHandler(CCentralWindow::ChangeTypeAffichage));
	Connect(wxEVENT_SETMODEVIEWER, wxCommandEventHandler(CCentralWindow::SetMode));
	Connect(VIDEO_UPDATE_ID, wxCommandEventHandler(CCentralWindow::SetVideoPos));
	Connect(wxTIMER_ANIMATION, wxEVT_TIMER, wxTimerEventHandler(CCentralWindow::OnTimerAnimation), nullptr, this);
	Connect(wxEVENT_LOADPICTURE, wxCommandEventHandler(CCentralWindow::OnLoadPicture));
	Connect(EVENT_SHOWPICTURE, wxCommandEventHandler(CCentralWindow::OnShowPicture));
	Connect(EVENT_ENDNEWPICTURETHREAD, wxCommandEventHandler(CCentralWindow::EndPictureThread));
    Connect(VIDEO_START, wxCommandEventHandler(CCentralWindow::OnVideoStart));
	animationTimer = new wxTimer(this, wxTIMER_ANIMATION);
	processLoadPicture = false;
    windowManager->HideWindow(Pos::wxTOP, false);
	windowMode = 1;
    stopVideo = false;
}


void CCentralWindow::OnVideoEnd(wxCommandEvent& event)
{
    printf("CCentralWindow::OnVideoEnd \n");
    stopVideo = false;
	videoStart = false;
    if(loadPicture)
    {
        CPictureElement * pictureElement = new CPictureElement();
        pictureElement->filename = filename;
        pictureElement->numElement = numElement;  
        pictureElement->first = false;     
        wxCommandEvent evt(wxEVENT_LOADPICTURE);
        evt.SetClientData(pictureElement);
        this->GetEventHandler()->AddPendingEvent(evt); 
        loadPicture = false;   
    }
}

void CCentralWindow::OnEndThumbnail()
{
	if (isVideo || isAnimation)
	{
		wxCommandEvent evt(wxEVENT_ENDVIDEOTHUMBNAIL);
		thumbnailVideo->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CCentralWindow::OnVideoStart(wxCommandEvent& event)
{
     printf("CCentralWindow::OnVideoStart \n");
	videoStart = true;
	if (thumbnailVideo != nullptr)
    {
        thumbnailVideo->SetVideoPosition(0);
    }
}

void CCentralWindow::RefreshPicture(const wxString &filename, const int &numElement, const bool &first)
{
    printf("CCentralWindow::RefreshPicture \n");
	this->filename = filename;
	this->numElement = numElement;

	if (videoStart)
	{
		loadPicture = true;

		if (previewWindow != nullptr && !stopVideo)
        {
            stopVideo = true;
            previewWindow->StopVideo();
        }
			
	}
	else
	{
		loadPicture = false;
		CPictureElement * pictureElement = new CPictureElement();
		pictureElement->filename = filename;
		pictureElement->numElement = numElement;
		pictureElement->first = first;
		wxCommandEvent evt(wxEVENT_LOADPICTURE);
		evt.SetClientData(pictureElement);
		this->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CCentralWindow::LoadPicture(const wxString &filename, const int &numElement, const bool &first)
{
	TRACE();
    printf("CMainWindow::LoadPicture %s \n", filename.ToStdString().c_str());
	if (this->filename != filename)
	{
		RefreshPicture(filename, numElement, first);
	}
	/*
	LoadPictureInThread(filename, numElement);
	if (thumbnailPicture != nullptr)
		thumbnailPicture->SetActifItem(numElement, true);
	*/
}

void CCentralWindow::HideToolbar()
{
    printf("CCentralWindow::HideToolbar \n");
	showToolbar = false;
	if (isFullscreen)
	{
		windowManager->HideWindow(Pos::wxTOP);
		windowManager->HideWindow(Pos::wxBOTTOM);
		windowManager->Resize();
	}

	wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
	if (window != nullptr)
	{
		wxCommandEvent evt(wxEVENT_HIDETOOLBAR);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CCentralWindow::ShowToolbar()
{
     printf("CCentralWindow::ShowToolbar \n");
     
	showToolbar = true;

	windowManager->ShowWindow(Pos::wxBOTTOM);

	if (isFullscreen && !isPicture)
	{
		windowManager->ShowWindow(Pos::wxTOP);
		
	}

	windowManager->Resize();

	if (previewWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_SHOWTOOLBAR);
		previewWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CCentralWindow::OnShowPicture(wxCommandEvent& event)
{
    printf("CCentralWindow::OnShowPicture \n");
	TRACE();

	CBitmapReturn * pictureData = (CBitmapReturn *)event.GetClientData();
	if (pictureData != nullptr)
	{
		if (pictureData->bitmap->GetFilename() == filename)
		{
			SetPicture(pictureData->bitmap, pictureData->isThumbnail);

			if (!pictureData->isThumbnail)
			{
				StopLoadingPicture();
			}

		}
		else
			delete pictureData->bitmap;

		delete pictureData;
	}
}

void CCentralWindow::StopLoadingPicture()
{
    printf("CCentralWindow::StopLoadingPicture \n");
	//showBitmapWindow->StopLoadingPicture();
	wxWindow* bitmapWindow = this->FindWindowById(THUMBNAILVIEWERPICTURE);
	if (bitmapWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_ONSTOPLOADINGPICTURE);
		//showBitmapWindow->GetEventHandler()->AddPendingEvent(evt);
		bitmapWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
///Mise à jour des informations sur les fichiers
////////////////////////////////////////////////////////////////////////////////////////////////
void CCentralWindow::SetPicture(CImageLoadingFormat * bitmap, const bool &isThumbnail)
{
    printf("CCentralWindow::SetPicture \n");
	TRACE();
	if (bitmap != nullptr && bitmap->IsOk())
	{
		filename = bitmap->GetFilename();
		SetBitmap(bitmap, isThumbnail);
		//UpdateInfos(bitmap);

		CPictureInfosMessage * pictureInfos = new CPictureInfosMessage();
		pictureInfos->filename = filename;
		pictureInfos->infos = to_string(bitmap->GetWidth()) + "x" + to_string(bitmap->GetHeight());

		wxWindow * mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
		wxCommandEvent evt(wxEVENT_INFOS);
		evt.SetClientData(pictureInfos);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);

	}
}

void CCentralWindow::StopAnimation()
{
     printf("CCentralWindow::StopAnimation \n");
	animationPosition = 0;
	if (animationTimer->IsRunning())
		animationTimer->Stop();
}


bool CCentralWindow::SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail)
{
    printf("CCentralWindow::SetBitmap \n");
	TRACE();
	StopAnimation();
	bool refresh = isPicture ? false : true;
	bool result = false;
	isAnimation = false;
	isPicture = true;
	isVideo = false;

	if (bitmap != nullptr && bitmap->IsOk())
	{
		windowManager->HideWindow(Pos::wxTOP);
		windowManager->Resize();
		result = SetBitmap(bitmap, isThumbnail, false);
	}
	return result;
}


bool CCentralWindow::SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation)
{
    printf("CCentralWindow::SetBitmap \n");
	TRACE();
	if (bitmap != nullptr && bitmap->IsOk())
	{
		isVideo = false;
		bool loadInfo = true;
		if (fileGeolocalisation->GetFilename() != bitmap->GetFilename())
			fileGeolocalisation->SetFile(bitmap->GetFilename());
		else
			loadInfo = false;

		if (isDiaporama)
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

	if (isThumbnail && isDiaporama)
	{
		if (bitmap != nullptr && bitmap->IsOk())
			delete bitmap;
	}
	return 0;
}


void CCentralWindow::SetPanelInfos(const bool &isThumbnail)
{
    printf("CCentralWindow::SetPanelInfos \n");
	if (panelInfosWindow->GetFilename() != filename)
	{
		if (isVideo)
			panelInfosWindow->SetVideoFile(filename);
		else
			panelInfosWindow->SetBitmapFile(filename, isThumbnail);

		//panelInfosWindow->Refresh();
	}
	else
	{
		if (!isThumbnail && this->isThumbnail)
		{
			panelInfosWindow->SetBitmapFile(filename, isThumbnail);
			//panelInfosWindow->Refresh();
		}
	}

	this->isThumbnail = isThumbnail;

}


void CCentralWindow::SetVideoPos(wxCommandEvent& event)
{
    printf("CCentralWindow::SetVideoPos \n");
	int64_t pos = event.GetExtraLong();
	if (thumbnailVideo != nullptr)
		thumbnailVideo->SetVideoPosition(pos);

}

void CCentralWindow::OnTimerAnimation(wxTimerEvent& event)
{
	printf("CViewerWindow::OnTimerAnimation %d \n", animationPosition);

	if (thumbnailVideo != nullptr)
		thumbnailVideo->SetVideoPosition(animationPosition);

	LoadAnimationBitmap(filename, animationPosition);
	uint32_t delay = 0;
	animationPosition++;
	if (animationPosition < nbThumbnail)
	{
		CLibPicture libPicture;
		CImageLoadingFormat * image = nullptr;
		int iFormat = libPicture.TestImageFormat(filename);
		if (iFormat != TIFF && iFormat != PDF)
		{
			CImageVideoThumbnail * video = videoThumbnail.at(animationPosition);
			delay = video->delay;
			//image = video->image;
		}
		else
		{
			//image = libPicture.LoadPicture(filename, false, animationPosition);
			delay = libPicture.GetFrameDelay(filename);
		}
		//printf("CViewerWindow::OnTimerAnimation Start delay %d \n", delay);
		animationTimer->Start(delay, wxTIMER_ONE_SHOT);
	}
	else
	{
		//Stop
		CAnimationToolbar * animationToolbar = (CAnimationToolbar *)this->FindWindowById(ANIMATIONTOOLBARWINDOWID);
		if (animationToolbar != nullptr)
			animationToolbar->AnimationStop();

		CWindowMain * windowMain = (CWindowMain *)this->FindWindowById(MAINVIEWERWINDOWID);
		if (windowMain != nullptr)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVT_ANIMATIONTIMERSTOP);
			windowMain->GetEventHandler()->AddPendingEvent(evt);
		}
	}
}

void CCentralWindow::AnimationSetPosition(wxCommandEvent& event)
{
    printf("CCentralWindow::AnimationSetPosition \n");
    
	int64_t * videoTime = (int64_t *)event.GetClientData();
	if (thumbnailVideo != nullptr && videoTime != nullptr)
		thumbnailVideo->SetVideoPosition(*videoTime);

	if (videoTime != nullptr)
		delete videoTime;
}


void CCentralWindow::AnimationPictureNext()
{
	animationPosition++;
	if (animationPosition >= nbThumbnail)
		animationPosition = nbThumbnail - 1;
	if (thumbnailVideo != nullptr)
		thumbnailVideo->SetVideoPosition(animationPosition);
	LoadAnimationBitmap(filename, animationPosition);
}

void CCentralWindow::AnimationPicturePrevious()
{
	animationPosition--;
	if (animationPosition < 0)
		animationPosition = 0;
	if (thumbnailVideo != nullptr)
		thumbnailVideo->SetVideoPosition(animationPosition);
	LoadAnimationBitmap(filename, animationPosition);
}


void CCentralWindow::Resize()
{
    printf("CCentralWindow::Resize() width : %d height : %d  width : %d \n", GetWindowWidth(), GetWindowHeight(), wxDisplay().GetGeometry().GetWidth());
    if(!init)
    {
        if(wxDisplay().GetGeometry().GetWidth() == GetWindowWidth())
        {
            printf("CCentralWindow::Resize() \n");
            init = true;
            //windowManager->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
            //windowManager->ResetPosition();
        }
        else
        {
            if (windowManager != nullptr)	
                windowManager->SetSize(0, 0, wxDisplay().GetGeometry().GetWidth(), GetWindowHeight());   
        }        
    }


    if (windowManager != nullptr)	
        windowManager->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
}

void CCentralWindow::LoadAnimationBitmap(const wxString &filename, const int &numFrame)
{
	if (numFrame == oldAnimationPosition && filename == oldFilename)
		return;

	oldFilename = filename;
	oldAnimationPosition = numFrame;

	//printf("CViewerWindow::LoadAnimationBitmap %d \n", numFrame);
	if (numFrame < nbThumbnail && numFrame >= 0)
	{
		CImageLoadingFormat * image = nullptr;
		CLibPicture libPicture;
		int iFormat = libPicture.TestImageFormat(filename);
		if (iFormat != TIFF && iFormat != PDF)
		{
			CImageVideoThumbnail * thumbnail = videoThumbnail.at(numFrame);
			if (thumbnail != nullptr)
			{
				CImageLoadingFormat * image = thumbnail->image;
				if (image != nullptr)
				{
					CImageLoadingFormat * bitmap = new CImageLoadingFormat();
					switch (thumbnail->image->GetFormat())
					{
					case TYPE_IMAGE_CXIMAGE:
						bitmap->SetPicture(image->GetCxImage());
						break;
					case TYPE_IMAGE_WXIMAGE:
						bitmap->SetPicture(image->GetwxImage());
						break;
					case TYPE_IMAGE_REGARDSIMAGE:
						bitmap->SetPicture(image->GetRegardsBitmap());
						break;
					case TYPE_IMAGE_REGARDSJPEGIMAGE:
						bitmap->SetPicture(image->GetwxImage());
						break;
					}
					bitmap->SetFilename(thumbnail->image->GetFilename());
					SetBitmap(bitmap, false, true);
				}
			}
		}
		else
		{
			image = libPicture.LoadPicture(filename, false, numFrame);
		}
		SetBitmap(image, false, true);
	}
}


void CCentralWindow::ChangeTypeAffichage(wxCommandEvent& event)
{
	if (listPicture != nullptr)
	{
		long typeAffichage = event.GetExtraLong();
		listPicture->ChangeTypeAffichage(&photoVector, typeAffichage);
	}
		
}

void CCentralWindow::SetListeFile(wxCommandEvent& event)
{
	CImageList * picture = (CImageList *)event.GetClientData();
	if (picture != nullptr)
	{
		this->photoVector.clear();
		photoVector = picture->GetCopy();
	}

	if (listPicture != nullptr)
		listPicture->SetListeFile(&photoVector);

	if (thumbnailPicture != nullptr)
		thumbnailPicture->SetListeFile(&photoVector);
}

CCentralWindow::~CCentralWindow()
{
	if (windowManager != nullptr)
		delete windowManager;

}

bool CCentralWindow::IsVideo()
{
    return isVideo;
}

void CCentralWindow::SetMode(wxCommandEvent& event)
{
	windowMode = event.GetInt();

#ifndef __NOFACE_DETECTION__
	listFace->Show(false);
#endif
	listPicture->Show(false);

	switch (windowMode)
	{
	case 1:
		previewWindow->Show(true);
		windowManager->ChangeWindow(previewWindow, Pos::wxCENTRAL);
		windowManager->ShowWindow(Pos::wxRIGHT,false);
		windowManager->ShowWindow(Pos::wxBOTTOM,false);
		windowManager->ShowPaneWindow(Pos::wxRIGHT,0);
		windowManager->ShowPaneWindow(Pos::wxLEFT,0);
		windowManager->ShowPaneWindow(Pos::wxBOTTOM);
		windowManager->Update();
		//windowManager->Resize();
		break;
#ifndef __NOFACE_DETECTION__
	case 2:
		previewWindow->Show(false);
		listFace->Show(true);
		windowManager->ChangeWindow(listFace, Pos::wxCENTRAL);
		windowManager->HideWindow(Pos::wxRIGHT,false);
		windowManager->HideWindow(Pos::wxBOTTOM, false);
		windowManager->HideWindow(Pos::wxTOP, false);
		windowManager->ShowPaneWindow(Pos::wxLEFT);
		windowManager->Update();
		break;
#endif
	case 3:
		previewWindow->Show(false);
		listPicture->Show(true);
		windowManager->ChangeWindow(listPicture, Pos::wxCENTRAL);
		windowManager->HideWindow(Pos::wxRIGHT, false);
		windowManager->HideWindow(Pos::wxBOTTOM, false);
		windowManager->HideWindow(Pos::wxTOP, false);
		windowManager->ShowPaneWindow(Pos::wxLEFT);
		windowManager->Update();
		break;
	case 4:
		previewWindow->Show(true);
		windowManager->ChangeWindow(previewWindow, Pos::wxCENTRAL);
		windowManager->ShowWindow(Pos::wxRIGHT,0);
		windowManager->HidePaneWindow(Pos::wxRIGHT,0);
		windowManager->HidePaneWindow(Pos::wxLEFT);
		windowManager->HidePaneWindow(Pos::wxTOP,0);
		windowManager->ShowWindow(Pos::wxBOTTOM,false);
		windowManager->HidePaneWindow(Pos::wxBOTTOM);
		windowManager->Update();
		//viewerWindow->Show(true);
		//this->SetWindow(panelSearch, viewerWindow);
		//panelSearch->ClosePane(1);
		////viewerWindow->HidePanelInfos();
		//viewerWindow->HideThumbnailPanel();
		break;

	}
	//windowManager->Resize();
}

void CCentralWindow::UpdateScreenRatio()
{
	if (windowManager != nullptr)
		windowManager->UpdateScreenRatio();
}

void CCentralWindow::StartAnimation()
{
	animationPosition = 0;
	LoadAnimationBitmap(filename, 0);
	animationTimer->Start(DELAY_ANIMATION, wxTIMER_ONE_SHOT);
}

void CCentralWindow::FullscreenMode()
{
	previewWindow->SetFullscreen(true);
	if (!isFullscreen)
	{
		isFullscreen = true;
		windowManager->HideWindow(Pos::wxLEFT);
		windowManager->HideWindow(Pos::wxRIGHT);
		windowManager->Resize();
	}
}

void CCentralWindow::ScreenMode()
{
	previewWindow->SetFullscreen(false);
	if (isFullscreen)
	{
		isFullscreen = false;
		windowManager->ShowWindow(Pos::wxLEFT);
		windowManager->ShowWindow(Pos::wxRIGHT);
		if (!showToolbar)
			windowManager->ShowWindow(Pos::wxBOTTOM);
		if(!isPicture)
			windowManager->ShowWindow(Pos::wxTOP);

		windowManager->Resize();
	}
}


void CCentralWindow::OnLoadPicture(wxCommandEvent& event)
{
	TRACE();
	CPictureElement * pictureElement = (CPictureElement *)event.GetClientData();
	LoadPictureInThread(pictureElement);
	if (thumbnailPicture != nullptr)
    {
		thumbnailPicture->SetActifItem(pictureElement->numElement, true);
       // thumbnailPicture->Refresh();
    }
	delete pictureElement;
}


bool CCentralWindow::GetProcessEnd()
{
	TRACE();
	if (processLoadPicture)
		return false;

	return true;
}


void CCentralWindow::SetPosition(const long &timePosition)
{
	if (isVideo)
	{
		CShowVideo * showVideoWindow = (CShowVideo *)this->FindWindowById(SHOWVIDEOVIEWERID);
#ifdef FFMPEG
		if (showVideoWindow != nullptr)
			showVideoWindow->SetPosition(timePosition);
#else
		if (showVideoWindow != nullptr)
			showVideoWindow->SetPosition(timePosition * ONE_MSEC);
#endif
	}
	else
	{
		animationPosition = timePosition;
		LoadAnimationBitmap(filename, timePosition);
	}
}

void CCentralWindow::LoadPictureInThread(CPictureElement * pictureElement)
{
	TRACE();
	if (!wxFileExists(filename))
		filename = CLibResource::GetPhotoCancel();
	else
		filename = pictureElement->filename;

	CLibPicture libPicture;

	if (libPicture.TestIsVideo(filename))
	{
		//StartLoadingPicture(numElement);
		SetVideo(filename, pictureElement->first);
		
	}
	else if (libPicture.TestIsAnimation(filename))
	{
		SetAnimation(filename);

		if (isDiaporama)
		{
			wxWindow* mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
			wxCommandEvent evt(wxTIMER_DIAPORAMATIMERSTART);
			mainWindow->GetEventHandler()->AddPendingEvent(evt);
		}
	}
	else
	{
		StartLoadingPicture(numElement);
		LoadingPicture(filename);
	}
	windowManager->Resize();
}

void CCentralWindow::StartLoadingPicture(const int &numElement)
{
	//showBitmapWindow->StartLoadingPicture();
	wxWindow* bitmapWindow = this->FindWindowById(THUMBNAILVIEWERPICTURE);
	if (bitmapWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_ONSTARTLOADINGPICTURE);
		evt.SetExtraLong(numElement);
		//showBitmapWindow->GetEventHandler()->AddPendingEvent(evt);
		bitmapWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

bool CCentralWindow::SetAnimation(const wxString &filename)
{
	windowManager->ShowWindow(Pos::wxTOP);
	StopAnimation();
	CLibPicture libPicture;
	bool refresh = isAnimation ? false : true;
	bool result = false;
	isVideo = false;
	isAnimation = true;
	isPicture = false;
	oldAnimationPosition = -1;
	oldFilename = L"";
	int iFormat = libPicture.TestImageFormat(filename);
	nbThumbnail = libPicture.GetNbImage(filename);
	if (thumbnailVideo->GetFilename() != filename)
	{
		thumbnailVideo->SetFilename(filename);
		if (nbThumbnail > 0)
			thumbnailVideo->SetFile(filename, nbThumbnail);
	}

	if (videoThumbnail.size() > 0)
	{
		for (int i = 0; i < videoThumbnail.size(); i++)
		{
			CImageVideoThumbnail * imageVideo = videoThumbnail.at(i);
			delete imageVideo;
		}
	}

	videoThumbnail.clear();

	if (iFormat != TIFF && iFormat != PDF)
		libPicture.LoadAllVideoThumbnail(filename, &videoThumbnail, false, false);

	animationPosition = 0;

	LoadAnimationBitmap(filename, 0);
	if (refresh)
		Refresh();

	CAnimationToolbar* animationToolbar = (CAnimationToolbar*)this->FindWindowById(ANIMATIONTOOLBARWINDOWID);
	if (animationToolbar != nullptr)
		animationToolbar->AnimationStart();


	SetPanelInfos(false);
	return result;
}


void CCentralWindow::SetVideo(const wxString &path, const bool &first)
{
    printf("CCentralWindow::SetVideo \n");
	StopAnimation();
	bool refresh = isVideo ? false : true;
	filename = path;
	isVideo = true;
	isAnimation = false;
	isPicture = false;
	
	windowManager->ShowWindow(Pos::wxTOP);
	//thumbnailVideoPanel->SetVideo(path);
	//thumbnailVideoPanel->ShowVideoThumbnail();
	if (thumbnailVideo->GetFilename() != filename)
	{
		thumbnailVideo->SetFilename(filename);
		thumbnailVideo->SetFile(filename);
       // thumbnailVideo->Refresh();
	}

	if (previewWindow != nullptr)
		previewWindow->SetVideo(path, !first);

	SetPanelInfos(false);

}

void CCentralWindow::LoadingPicture(const wxString &filenameToShow)
{
    printf("CCentralWindow::LoadingPicture \n");
	TRACE();
	if (!processLoadPicture)
	{
		CThreadPictureData * pictureData = new CThreadPictureData();
		pictureData->mainWindow = this;
		pictureData->picture = filenameToShow;
		pictureData->isVisible = true;
		thread * threadloadPicture = new thread(LoadingNewPicture, pictureData);
		pictureData->myThread = threadloadPicture;
		processLoadPicture = true;
	}
}

void CCentralWindow::EndPictureThread(wxCommandEvent& event)
{
    printf("CCentralWindow::EndPictureThread \n");
	TRACE();
	processLoadPicture = false;
    wxString localPicture = "";
	CThreadPictureData * pictureData = (CThreadPictureData *)event.GetClientData();
	if (pictureData != nullptr)
	{
		if (pictureData->myThread != nullptr)
		{
			pictureData->myThread->join();
			delete pictureData->myThread;
		}
        localPicture = pictureData->picture;
		wxString * filename = new wxString();
		*filename = pictureData->picture;
		wxWindow * mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
		wxCommandEvent evt(EVENT_ENDNEWPICTURETHREAD);
		evt.SetClientData(filename);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);

		delete pictureData;
	}
    
    if(localPicture != "" && localPicture != this->filename)
    {
        CPictureElement * pictureElement = new CPictureElement();
		pictureElement->filename = filename;
		pictureElement->numElement = numElement;
		pictureElement->first = false;
		wxCommandEvent evt(wxEVENT_LOADPICTURE);
		evt.SetClientData(pictureElement);
		this->GetEventHandler()->AddPendingEvent(evt);
    }
}


//////////////////////////////////////////////////////////////////////////
//Picture Loading Function
//////////////////////////////////////////////////////////////////////////
void CCentralWindow::LoadingNewPicture(CThreadPictureData * pictureData)
{
    printf("CCentralWindow::LoadingNewPicture \n");
	TRACE();
	CLibPicture libPicture;
	CImageLoadingFormat * bitmap = nullptr;

	if (pictureData != nullptr)
	{
		CSqlThumbnail sqlThumbnail;
		CImageLoadingFormat * _loadingPicture = new CImageLoadingFormat();
		CRegardsBitmap * bitmapThumbnail = sqlThumbnail.GetPictureThumbnail(pictureData->picture);
		if (bitmapThumbnail != nullptr && bitmapThumbnail->IsValid())
		{
			_loadingPicture->SetPicture(bitmapThumbnail);
			CBitmapReturn * bitmapReturn = new CBitmapReturn();
			bitmapReturn->myThread = nullptr;
			bitmapReturn->isThumbnail = true;
			bitmapReturn->bitmap = _loadingPicture;
			wxCommandEvent * event = new wxCommandEvent(EVENT_SHOWPICTURE);
			event->SetClientData(bitmapReturn);
			wxQueueEvent(pictureData->mainWindow, event);
		}

		clock_t tStart = clock();
		bitmap = libPicture.LoadPicture(pictureData->picture);
		int timeToLoad = (int)((clock() - tStart) / (CLOCKS_PER_SEC / 1000));

		if (timeToLoad < 300)
		{
			CRegardsConfigParam * config;
			config = CParamInit::getInstance();
			//Test si transition
			if (config != nullptr)
			{
				int numEffect = config->GetEffect();
				if (numEffect > 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(300 - timeToLoad));
				}

			}
		}

		if (bitmap == nullptr || (bitmap->GetWidth() == 0 || bitmap->GetHeight() == 0))
		{
			if (bitmap != nullptr)
				delete bitmap;

			bitmap = libPicture.LoadPicture(CLibResource::GetPhotoCancel());
		}
	}

	if (bitmap != nullptr)
	{
		CBitmapReturn * bitmapReturn = new CBitmapReturn();
		bitmapReturn->myThread = nullptr;
		bitmapReturn->isThumbnail = false;
		bitmapReturn->bitmap = bitmap;
		wxCommandEvent * event = new wxCommandEvent(EVENT_SHOWPICTURE);
		event->SetClientData(bitmapReturn);
		wxQueueEvent(pictureData->mainWindow, event);
	}

	wxCommandEvent * event = new wxCommandEvent(EVENT_ENDNEWPICTURETHREAD);
	event->SetClientData(pictureData);
	wxQueueEvent(pictureData->mainWindow, event);
}