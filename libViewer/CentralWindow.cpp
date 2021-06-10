#include <header.h>
#include "CentralWindow.h"
#include <ConfigParam.h>
#include "ViewerParamInit.h"
#include <LibResource.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <ImageLoadingFormat.h>
#include <libPicture.h>
#include <picture_id.h>
#include <ThumbnailMessage.h>
#include "PictureElement.h"
#include <ShowVideo.h>
#include <wx/display.h>
#include "AnimationToolbar.h"
#include "PanelPhotoWnd.h"
#include "ListPicture.h"
#include "WindowManager.h"
#include "ThumbnailViewerVideo.h"
#include <SqlPhotos.h>
#include "ThumbnailViewerPicture.h"
#ifndef __NOFACE_DETECTION__
#include "ListFace.h"
#endif
#include "ImageList.h"
#include "ViewerParam.h"
#include "PanelInfosWnd.h"
#include <PanelWithClickToolbar.h>
#include "PreviewWnd.h"
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <libPicture.h>
#include <FiltreEffet.h>
using namespace Regards::Picture;
using namespace Regards::Window;
using namespace Regards::Sqlite;
using namespace Regards::Viewer;
using namespace Regards::FiltreEffet;

#define DELAY_ANIMATION 20

#define WINDOW_FACE 2
#define WINDOW_EXPLORER 3
#define WINDOW_VIEWER 1
#define WINDOW_PICTURE 4


CCentralWindow::CCentralWindow(wxWindow* parent, wxWindowID id,
	const CThemeSplitter & theme, const bool &horizontal)
	: CWindowMain("CentralWindow", parent, id)
{

	oldWindowMode = 1;
	panelPhotoWnd = nullptr;
	viewerconfig = nullptr;
	isFullscreen = false;
	isDiaporama = false;
	showToolbar = true;
	videoStart = false;
	

	wxRect rect;
	wxRect left;
	wxRect right;
    int widthInfosWindow = 0;//wxDisplay().GetGeometry().GetWidth() / 4;

	CMainParam* config = CMainParamInit::getInstance();
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();

	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		faceDetection = regardsParam->GetFaceDetection();
	}

	/*
	if (imageList != nullptr)
	{
		photoVector = imageList->GetCopy();
	}
	*/
	windowManager = new CWindowManager(this, wxID_ANY, theme);

	if (config != nullptr)
	{
		windowMode = config->GetViewerMode();
		left = config->GetPositionLeftPanel();
		right = config->GetPositionRightPanel();
	}
	else
	{
		windowMode = 1;
	}

	//Verify left position panel is correct
	if (left.width < 50)
		left = { 0,0,0,0 };

	if (right.width < 50)
		right = { 0,0,0,0 };
	//Add Criteria Window
	if (viewerTheme != nullptr)
	{
		bool isPanelVisible = true;
		if(viewerconfig != nullptr)
			viewerconfig->GetShowFilter(isPanelVisible);

		wxString libelle = CLibResource::LoadStringFromResource(L"LBLFOLDERCATEGORY", 1);
		CThemePane theme;
		viewerTheme->GetPaneTheme(&theme);

		CThemeToolbar themetoolbar;
		viewerTheme->GetClickToolbarTheme(&themetoolbar);
		
		panelPhotoWnd = new CPanelPhotoWnd(windowManager, CRITERIAFOLDERWINDOWID);
		windowManager->AddPanel(panelPhotoWnd, Pos::wxLEFT, false, widthInfosWindow, left, libelle, "PanelPhotoSearch", true, PHOTOSEEARCHPANEL, true, true);

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
		{
			thumbnailPicture->SetListeFile();
		}

		windowManager->AddPanel(scrollPictureWindow, Pos::wxBOTTOM, true, themeThumbnail.themeIcone.GetHeight() + theme.GetHeight() * 2, rect, libelle, "ThumbnailPicturePanel", true, THUMBNAILPICTUREPANEL, true, true);

	}

	wxString urlServer;
	CRegardsConfigParam * param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetUrlServer();
	}


	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);
		CThemePane theme;
		viewerTheme->GetPaneTheme(&theme);
		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickToolbarTheme(&themeClickInfosToolbar);
		//paneInfos = new CPanelWithClickToolbar(this, "CPictureInfosPanel", PANELCLICKINFOSWNDID, theme, themeClickInfosToolbar, libelle, showInfos, false, true);
		panelInfosWindow = new CPanelInfosWnd(windowManager, PANELINFOSWNDID);

		panelInfosClick = windowManager->AddPanel(panelInfosWindow, Pos::wxRIGHT, false, widthInfosWindow, right, libelle, "PictureInfosPanel", true, PANELCLICKINFOSWNDID, false);

	}

	previewWindow = new CPreviewWnd(windowManager, PREVIEWVIEWERID);
	windowManager->AddWindow(previewWindow, Pos::wxCENTRAL, false, 0, rect, PREVIEWVIEWERID, false);


	if (viewerTheme != nullptr)
	{
		listPicture = new CListPicture(windowManager, LISTPICTUREID);
		listPicture->Show(false);
		listPicture->SetListeFile();
	}

	if (faceDetection)
	{
		if (viewerTheme != nullptr)
		{
			listFace = new CListFace(windowManager, LISTFACEID);
			listFace->Show(false);
		}
	}

	Connect(VIDEO_END_ID, wxCommandEventHandler(CCentralWindow::OnVideoEnd));
	Connect(wxEVENT_CHANGETYPEAFFICHAGE, wxCommandEventHandler(CCentralWindow::ChangeTypeAffichage));
	Connect(wxEVENT_SETMODEVIEWER, wxCommandEventHandler(CCentralWindow::SetMode));
	Connect(VIDEO_UPDATE_ID, wxCommandEventHandler(CCentralWindow::SetVideoPos));
	Connect(wxTIMER_ANIMATION, wxEVT_TIMER, wxTimerEventHandler(CCentralWindow::OnTimerAnimation), nullptr, this);
	Connect(wxEVENT_LOADPICTURE, wxCommandEventHandler(CCentralWindow::OnLoadPicture));
	Connect(EVENT_SHOWPICTURE, wxCommandEventHandler(CCentralWindow::OnShowPicture));
	Connect(EVENT_ENDNEWPICTURETHREAD, wxCommandEventHandler(CCentralWindow::EndPictureThread));
    Connect(VIDEO_START, wxCommandEventHandler(CCentralWindow::OnVideoStart));
	Connect(wxEVENT_ANIMATIONSTART, wxCommandEventHandler(CCentralWindow::OnAnimationStart));
	Connect(wxEVENT_ANIMATIONSTOP, wxCommandEventHandler(CCentralWindow::OnAnimationStop));
	animationTimer = new wxTimer(this, wxTIMER_ANIMATION);
	processLoadPicture = false;
    windowManager->HideWindow(Pos::wxTOP, false);

    stopVideo = false;

	wxCommandEvent event(wxEVENT_SETMODEVIEWER);
	event.SetInt(windowMode);
	wxPostEvent(this, event);

	if (listPicture != nullptr)
		listPicture->SetListeFile();

	if (thumbnailPicture != nullptr)
		thumbnailPicture->SetListeFile();

	wxString local = "";
	if (config != nullptr)
		local = config->GetLastShowPicture();

	LoadPicture(local);
}

wxString CCentralWindow::GetFilename()
{
	return filename;
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
		pictureElement->reloadResource = true;
        wxCommandEvent evt(wxEVENT_LOADPICTURE);
        evt.SetClientData(pictureElement);
        this->GetEventHandler()->AddPendingEvent(evt); 
          
    }
	else if (isDiaporama)
	{
		ImageSuivante();
	}

	loadPicture = false;

	
}

void CCentralWindow::StopDiaporama()
{
	isDiaporama = false;
}


void CCentralWindow::StartDiaporama()
{
	isDiaporama = true;
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

int CCentralWindow::RefreshPicture(const wxString & filename)
{

	int numLocalItem = 0;

	if (windowMode == WINDOW_EXPLORER)
	{
		if (listPicture != nullptr)
		{
			numLocalItem = listPicture->GetNumItem();
		}
	}
	else if (windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE)
	{
		if (thumbnailPicture != nullptr)
		{
			numLocalItem = thumbnailPicture->GetNumItem();
		}
	}

    printf("CCentralWindow::RefreshPicture \n");
	if (filename != this->filename)
	{
		this->filename = filename;

		if (stopVideo)
			return 0;

		if (videoStart)
		{
			loadPicture = true;

			if (previewWindow != nullptr && !stopVideo)
			{
				stopVideo = true;
				previewWindow->StopVideo();
			}
			return 1;
		}
		else
		{
			previewWindow->StopVideo();
			loadPicture = false;
			CPictureElement * pictureElement = new CPictureElement();
			pictureElement->filename = filename;
			wxCommandEvent evt(wxEVENT_LOADPICTURE);
			evt.SetClientData(pictureElement);
			this->GetEventHandler()->AddPendingEvent(evt);
		}
	}


	if(thumbnailPicture != nullptr)
		thumbnailPicture->SetActifItem(GetPhotoId(filename), true);
	if (listPicture != nullptr)
		listPicture->SetActifItem(GetPhotoId(filename), true);

	int outItem = 0;

	if (windowMode == WINDOW_EXPLORER)
	{
		if (listPicture != nullptr)
		{
			outItem = listPicture->GetNumItem();
		}
	}
	else if (windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE)
	{
		if (thumbnailPicture != nullptr)
		{
			outItem = thumbnailPicture->GetNumItem();
		}
	}

	if (outItem != numLocalItem)
	{
		if (outItem > numLocalItem)
			isNext = true;
		else
			isNext = false;
	}

	return 0;
}

wxString CCentralWindow::ImageSuivante(const bool &loadPicture)
{
	wxString localFilename = "";
	int numItem = 0;
	if (windowMode)
	{
		if (windowMode == WINDOW_EXPLORER)
		{
			if (listPicture != nullptr)
			{
				numItem = listPicture->ImageSuivante();
				localFilename = listPicture->GetFilename(numItem);
			}
		}
		else if (windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE)
		{
			if (thumbnailPicture != nullptr)
			{
				numItem = thumbnailPicture->ImageSuivante();
				localFilename = thumbnailPicture->GetFilename(numItem);
			}
		}
	}

	if (localFilename != "" && loadPicture)
	{
		isNext = true;
		LoadPicture(localFilename);
	}
	return localFilename;
}

wxString CCentralWindow::ImageFin(const bool& loadPicture)
{
	wxString localFilename = "";
	int numItem = 0;
	if (windowMode)
	{
		if (windowMode == WINDOW_EXPLORER)
		{
			if (listPicture != nullptr)
			{
				numItem = listPicture->ImageFin();
				localFilename = listPicture->GetFilename(numItem);
			}
		}
		else if (windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE)
		{
			if (thumbnailPicture != nullptr)
			{
				numItem = thumbnailPicture->ImageFin();
				localFilename = thumbnailPicture->GetFilename(numItem);
			}
		}
	}


	if (localFilename != "" && loadPicture)
	{
		LoadPicture(localFilename);
	}
	return localFilename;
}

int CCentralWindow::GetNbElement()
{
	if (thumbnailPicture != nullptr)
		return thumbnailPicture->GetNbElement();
	return 0;
}

wxString CCentralWindow::ImageDebut(const bool& loadPicture)
{
	wxString localFilename = "";
	int numItem = 0;
	if (windowMode)
	{
		if (windowMode == WINDOW_EXPLORER)
		{
			if (listPicture != nullptr)
			{
				numItem = listPicture->ImageDebut();
				localFilename = listPicture->GetFilename(numItem);
			}
		}
		else if (windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE)
		{
			if (thumbnailPicture != nullptr)
			{
				numItem = thumbnailPicture->ImageDebut();
				localFilename = thumbnailPicture->GetFilename(numItem);
			}
		}
	}

	if (localFilename != "" && loadPicture)
	{
		LoadPicture(localFilename);
	}
	return localFilename;
}

wxString CCentralWindow::ImagePrecedente(const bool& loadPicture)
{
	wxString localFilename = "";
	int numItem = 0;
	if (windowMode)
	{
		if (windowMode == WINDOW_EXPLORER)
		{
			if (listPicture != nullptr)
			{
				numItem = listPicture->ImagePrecedente();
				localFilename = listPicture->GetFilename(numItem);
			}	
		}
		else if (windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE)
		{
			if (thumbnailPicture != nullptr)
			{
				numItem = thumbnailPicture->ImagePrecedente();
				localFilename = thumbnailPicture->GetFilename(numItem);
			}
		}
	}


	if (localFilename != "" && loadPicture)
	{
		isNext = false;
		LoadPicture(localFilename);
	}
	return localFilename;
}

int CCentralWindow::LoadPicture(const wxString &filename)
{
	return RefreshPicture(filename);
}

void CCentralWindow::HideToolbar()
{
    printf("CCentralWindow::HideToolbar \n");
	showToolbar = false;
	if (isFullscreen)
	{
		windowManager->HideWindow(Pos::wxTOP);
		windowManager->HideWindow(Pos::wxBOTTOM);
		windowManager->HideWindow(Pos::wxRIGHT);
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

	if(windowMode == WINDOW_VIEWER)
		windowManager->ShowWindow(Pos::wxBOTTOM);

	if (isFullscreen)
	{
		windowManager->ShowWindow(Pos::wxRIGHT);
		windowManager->HideWindow(Pos::wxCENTRAL);
		windowManager->ShowWindow(Pos::wxCENTRAL);
	}
		

	if (isFullscreen && !isPicture)
	{
		if (windowMode == WINDOW_VIEWER)
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
	//bool needToReload = false;
	bool isThumbnail = false;
#if defined(WIN32) && defined(_DEBUG)
	OutputDebugString(L"CCentralWindow::OnShowPicture : ");
	OutputDebugString(L"\n");
#endif
	
	processLoadPicture = false;
	bool isPictureToShow = false;
	CBitmapReturn * pictureData = (CBitmapReturn *)event.GetClientData();
    int redraw = event.GetInt();
	if (pictureData->bitmap != nullptr)
	{
		if (filename == pictureData->bitmap->GetFilename())
			isPictureToShow = true;
	}

	if (isPictureToShow)
	{
		
		if (pictureData != nullptr)
		{
			SetPicture(pictureData->bitmap, pictureData->isThumbnail);

			if (!pictureData->isThumbnail)
			{
				StopLoadingPicture();
			}
			else
				isThumbnail = true;

			delete pictureData;
		}
	}
	else
	{
		if (pictureData != nullptr)
		{
			if (!pictureData->isThumbnail)
			{
				StopLoadingPicture();
			}

			if (pictureData->bitmap != nullptr)
				delete pictureData->bitmap;

			delete pictureData;
		}

	}
    
    if (redraw)
    {
        CThreadPictureData * pictureData = new CThreadPictureData();
		pictureData->mainWindow = this;
		pictureData->picture = this->filename;
		pictureData->isVisible = true;
		thread * threadloadPicture = new thread(LoadingNewPicture, pictureData);
		pictureData->myThread = threadloadPicture;
		processLoadPicture = true;  
    }

	if (!isThumbnail)
	{
		wxString* _filename = new wxString(this->filename);
		wxWindow * mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
		wxCommandEvent evt(EVENT_ENDNEWPICTURETHREAD);
		evt.SetClientData(_filename);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
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
#if defined(WIN32) && defined(_DEBUG)
		OutputDebugString(L"CCentralWindow::SetPicture");
		OutputDebugString(L"\n");
		OutputDebugString(bitmap->GetFilename());
		OutputDebugString(L"\n");
#endif

		SetBitmap(bitmap, isThumbnail);
		CPictureInfosMessage * pictureInfos = new CPictureInfosMessage();
		pictureInfos->filename = filename;
		pictureInfos->infos = to_string(bitmap->GetWidth()) + "x" + to_string(bitmap->GetHeight());

		wxWindow * mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
		wxCommandEvent evt(wxEVENT_INFOS);
		evt.SetClientData(pictureInfos);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);

		wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_SHOWSAVEBUTTON);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

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
	//bool refresh = isPicture ? false : true;
	bool result = false;
	isAnimation = false;
	isPicture = true;
	isVideo = false;

	if (bitmap != nullptr && bitmap->IsOk())
	{
#if defined(WIN32) && defined(_DEBUG)
		OutputDebugString(L"CCentralWindow::SetBitmap");
		OutputDebugString(L"\n");
		OutputDebugString(bitmap->GetFilename());
		OutputDebugString(L"\n");
#endif

		windowManager->HideWindow(Pos::wxTOP);
		//windowManager->Resize();
		result = SetBitmap(bitmap, isThumbnail, false);
	}
	return result;
}


bool CCentralWindow::SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation)
{
#if defined(WIN32) && defined(_DEBUG)
	OutputDebugString(L"CCentralWindow::SetBitmap");
	OutputDebugString(L"\n");
#endif

	previewWindow->IsNextPicture(isNext);

    printf("CCentralWindow::SetBitmap \n");
	TRACE();
	if (bitmap != nullptr && bitmap->IsOk())
	{
		isVideo = false;

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

		panelInfosWindow->Refresh();
		panelInfosWindow->Update();
	}
	else
	{
		if (!isThumbnail && this->isThumbnail)
		{
			panelInfosWindow->SetBitmapFile(filename, isThumbnail);
			panelInfosWindow->Refresh();
			panelInfosWindow->Update();
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
		//CImageLoadingFormat * image = nullptr;
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
		listPicture->ChangeTypeAffichage(typeAffichage);
	}
		
}

void CCentralWindow::SetListeFile(const wxString& filename)
{
	if (listPicture != nullptr)
		listPicture->SetListeFile();

	if (thumbnailPicture != nullptr)
		thumbnailPicture->SetListeFile();

	LoadPicture(filename);

}

CCentralWindow::~CCentralWindow()
{
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		config->SetLastShowPicture(filename);

	if (windowManager != nullptr)
		delete windowManager;
}

void CCentralWindow::SaveParameter()
{
	int showInfos;
	int showThumbnail;
	int showFolder;
	int showVideoThumbnail;
	//Save Window Mode
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
		wxRect left = windowManager->GetWindowSize(Pos::wxLEFT);
		wxRect right = windowManager->GetWindowSize(Pos::wxRIGHT);
		config->SetViewerMode(windowMode);
		
		

		showFolder = windowManager->GetPaneState(Pos::wxLEFT);
		if(showFolder)
			config->SetPositionLeftPanel(left);

		showVideoThumbnail = windowManager->GetPaneState(Pos::wxTOP);
		showThumbnail = windowManager->GetPaneState(Pos::wxBOTTOM);
		showInfos = windowManager->GetPaneState(Pos::wxRIGHT);
		if(showInfos)
			config->SetPositionRightPanel(right);

		config->SetShowInfos(showInfos);
		config->SetShowThumbnail(showThumbnail);
		config->SetShowVideoThumbnail(showVideoThumbnail);
		config->SetShowFolder(showFolder);

	}
}

bool CCentralWindow::IsVideo()
{
    return isVideo;
}

void CCentralWindow::SetMode(wxCommandEvent& event)
{
	bool showInfos = true;
	bool showThumbnail = true;
	bool showFolder = true;
	bool showVideoThumbnail = true;
	windowMode = event.GetInt();
	if (windowMode == oldWindowMode)
		return;

	CMainParam* config = CMainParamInit::getInstance();

	if (!windowInit && oldWindowMode == WINDOW_VIEWER)
	{
		showVideoThumbnail = windowManager->GetPaneState(Pos::wxTOP);
		CMainParam* config = CMainParamInit::getInstance();
		if (config != nullptr)
			config->SetShowVideoThumbnail(showVideoThumbnail);
	}

	if (windowInit)
	{
		if (config != nullptr)
		{
			config->GetShowInfos(showInfos);
			config->GetShowThumbnail(showThumbnail);
			config->GetShowFolder(showFolder);
		}
	}

	if (config != nullptr)
	{
		config->GetShowVideoThumbnail(showVideoThumbnail);
	}

	previewWindow->SetNormalMode();
	panelInfosWindow->Show(false);
	panelInfosClick->Show(false);
	if(faceDetection)
		listFace->Show(false);
	listPicture->Show(false);

	if (windowInit)
	{
		if (!showFolder)
			windowManager->HidePaneWindow(Pos::wxLEFT);
		else
			windowManager->ShowPaneWindow(Pos::wxLEFT);

		if (!showInfos)
			windowManager->HidePaneWindow(Pos::wxRIGHT);
		else
			windowManager->ShowPaneWindow(Pos::wxRIGHT);

		if (!showThumbnail)
			windowManager->HidePaneWindow(Pos::wxBOTTOM);
		else
			windowManager->ShowPaneWindow(Pos::wxBOTTOM);
	}

	if (!showVideoThumbnail)
		windowManager->HidePaneWindow(Pos::wxTOP);
	else
		windowManager->ShowPaneWindow(Pos::wxTOP);

	switch (windowMode)
	{

	case WINDOW_VIEWER:
		{
			wxWindow* window = this->FindWindowById(PREVIEWVIEWERID);
			if (window != nullptr)
			{
				wxCommandEvent evt(wxEVENT_SHOWSCREENBUTTON);
				window->GetEventHandler()->AddPendingEvent(evt);
			}

			if (!windowManager->GetWindowIsShow(Pos::wxLEFT))
				windowManager->ShowWindow(Pos::wxLEFT);
			if (!windowManager->GetWindowIsShow(Pos::wxRIGHT))
				windowManager->ShowWindow(Pos::wxRIGHT);
			if (!windowManager->GetWindowIsShow(Pos::wxBOTTOM))
				windowManager->ShowWindow(Pos::wxBOTTOM);
			if (!windowManager->GetWindowIsShow(Pos::wxTOP))
				windowManager->ShowWindow(Pos::wxTOP);

			if (isVideo || isAnimation)
			{
				if (showVideoThumbnail)
					windowManager->ShowPaneWindow(Pos::wxTOP);
				else
					windowManager->HidePaneWindow(Pos::wxTOP);
			}
			else
			{
				windowManager->HideWindow(Pos::wxTOP);
			}

			panelInfosWindow->Show(true);
			panelInfosClick->SetWindow(panelInfosWindow);
			panelInfosClick->Show(true);
			panelInfosClick->SetTitle("Informations");

			if (windowInit)
				if (!showInfos)
					windowManager->HidePaneWindow(Pos::wxRIGHT);
		}
		break;


	case WINDOW_FACE:
	{
		wxWindow* window = this->FindWindowById(PREVIEWVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_HIDESCREENBUTTON);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		if (faceDetection)
		{
			panelInfosClick->Show(true);
			if (!windowManager->GetWindowIsShow(Pos::wxLEFT))
				windowManager->ShowWindow(Pos::wxLEFT);
			if (!windowManager->GetWindowIsShow(Pos::wxRIGHT))
				windowManager->ShowWindow(Pos::wxRIGHT);
			if (windowManager->GetWindowIsShow(Pos::wxBOTTOM))
				windowManager->HideWindow(Pos::wxBOTTOM);
			if (windowManager->GetWindowIsShow(Pos::wxTOP))
				windowManager->HideWindow(Pos::wxTOP);

			listFace->Show(true);
			panelInfosClick->SetWindow(listFace);
			panelInfosClick->Show(true);
			panelInfosClick->SetTitle("Face List");

			if (windowInit)
				if (!showInfos)
					windowManager->HidePaneWindow(Pos::wxRIGHT);
		}
		break;
	}
	case WINDOW_EXPLORER:
		{
			wxWindow* window = this->FindWindowById(PREVIEWVIEWERID);
			if (window != nullptr)
			{
				wxCommandEvent evt(wxEVENT_HIDESCREENBUTTON);
				window->GetEventHandler()->AddPendingEvent(evt);
			}

			panelInfosClick->Show(true);
			if (!windowManager->GetWindowIsShow(Pos::wxLEFT))
				windowManager->ShowWindow(Pos::wxLEFT);
			if (!windowManager->GetWindowIsShow(Pos::wxRIGHT))
				windowManager->ShowWindow(Pos::wxRIGHT);

			if (windowManager->GetWindowIsShow(Pos::wxBOTTOM))
				windowManager->HideWindow(Pos::wxBOTTOM);
			if (windowManager->GetWindowIsShow(Pos::wxTOP))
				windowManager->HideWindow(Pos::wxTOP);

			listPicture->Show(true);
			panelInfosClick->SetWindow(listPicture);
			panelInfosClick->Show(true);
			panelInfosClick->SetTitle("Picture List");

			if (windowInit)
				if (!showInfos)
					windowManager->HidePaneWindow(Pos::wxRIGHT);
		}

		break;
	case WINDOW_PICTURE:
		{
			wxWindow* window = this->FindWindowById(PREVIEWVIEWERID);
			if (window != nullptr)
			{
				wxCommandEvent evt(wxEVENT_SHOWSCREENBUTTON);
				window->GetEventHandler()->AddPendingEvent(evt);
			}

			if (windowManager->GetWindowIsShow(Pos::wxLEFT))
				windowManager->HideWindow(Pos::wxLEFT);
			if (windowManager->GetWindowIsShow(Pos::wxBOTTOM))
				windowManager->HideWindow(Pos::wxBOTTOM);
			if (windowManager->GetWindowIsShow(Pos::wxTOP))
				windowManager->HideWindow(Pos::wxTOP);
			if (windowManager->GetWindowIsShow(Pos::wxRIGHT))
				windowManager->HideWindow(Pos::wxRIGHT);
		}
		break;

	}
	windowInit = false;
	
	oldWindowMode = windowMode;

	windowManager->Resize();
}

void CCentralWindow::UpdateScreenRatio()
{
	if (windowManager != nullptr)
		windowManager->UpdateScreenRatio();
}

void CCentralWindow::OnAnimationStop(wxCommandEvent& event)
{
	StopAnimation();
}

void CCentralWindow::OnAnimationStart(wxCommandEvent& event)
{
	StartAnimation();
}

void CCentralWindow::StartAnimation()
{
	animationPosition = 0;
	LoadAnimationBitmap(filename, 0);
	animationTimer->Start(DELAY_ANIMATION, wxTIMER_ONE_SHOT);
}

bool CCentralWindow::FullscreenMode()
{
	if (!(windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE))
		return false;

	bool showVideoThumbnail = windowManager->GetPaneState(Pos::wxTOP);
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		config->SetShowVideoThumbnail(showVideoThumbnail);

	previewWindow->SetFullscreen(true);
	if (!isFullscreen)
	{
		isFullscreen = true;
		windowManager->HideWindow(Pos::wxLEFT);
		//windowManager->HideWindow(Pos::wxRIGHT);
		windowManager->Resize();
	}

	return true;
}

bool CCentralWindow::IsCompatibleFullscreen()
{
	if (windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE)
		return true;
	return false;;
}

bool CCentralWindow::ScreenMode()
{
	if (!(windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE))
		return false;

	bool showVideoThumbnail = windowManager->GetPaneState(Pos::wxTOP);
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		config->SetShowVideoThumbnail(showVideoThumbnail);

	previewWindow->SetFullscreen(false);
	if (isFullscreen)
	{
		oldWindowMode = -1;
		isFullscreen = false;
		wxCommandEvent event(wxEVENT_SETMODEVIEWER);
		event.SetInt(windowMode);
		wxPostEvent(this, event);
	}

	return true;
}


void CCentralWindow::OnLoadPicture(wxCommandEvent& event)
{
	TRACE();
	CPictureElement * pictureElement = (CPictureElement *)event.GetClientData();
	if (pictureElement != nullptr)
	{
#if defined(WIN32) && defined(_DEBUG)
		OutputDebugString(L"CCentralWindow::OnLoadPicture\n");
		OutputDebugString(pictureElement->filename);
		OutputDebugString(L"\n");
#endif

		LoadPictureInThread(pictureElement);

		if (listPicture != nullptr)
		{
			listPicture->SetActifItem(GetPhotoId(pictureElement->filename), true);
			// thumbnailPicture->Refresh();
		}

		if (thumbnailPicture != nullptr)
		{
			thumbnailPicture->SetActifItem(GetPhotoId(pictureElement->filename), true);
			// thumbnailPicture->Refresh();
		}
		delete pictureElement;
	}
}


bool CCentralWindow::GetProcessEnd()
{
	TRACE();
	StopAnimation();
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
#if defined(WIN32) && defined(_DEBUG)
	OutputDebugString(L"CCentralWindow::LoadPictureInThread\n");
	OutputDebugString(pictureElement->filename);
	OutputDebugString(L"\n");
#endif
	wxString localFile = pictureElement->filename;

	if (!wxFileExists(localFile))
		localFile = CLibResource::GetPhotoCancel();
	//else
	//	filename = pictureElement->filename;

	CLibPicture libPicture;
	bool isVideoValid = true;

	if (libPicture.TestIsVideo(localFile))
	{
		isVideoValid = libPicture.TestIsVideoValid(localFile);
	}

	if (libPicture.TestIsVideo(localFile) && isVideoValid)
	{
		SetVideo(localFile);
		
	}
	else if (libPicture.TestIsVideo(localFile) && !isVideoValid)
	{
		CImageLoadingFormat * bitmap = libPicture.GetCancelPhoto(localFile);
		SetBitmap(bitmap, false, false);
	}
	else if (libPicture.TestIsAnimation(localFile))
	{

		SetAnimation(localFile);

		if (isDiaporama)
		{
			wxWindow* mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
			wxCommandEvent evt(wxTIMER_DIAPORAMATIMERSTART);
			mainWindow->GetEventHandler()->AddPendingEvent(evt);
		}
	}
	else
	{
		StartLoadingPicture();
		LoadingPicture(localFile);
	}
	//windowManager->Resize();
}

int CCentralWindow::GetPhotoId(const wxString & filename)
{
	CSqlPhotos photo;
	return photo.GetPhotoId(filename);

}

void CCentralWindow::StartLoadingPicture()
{
	//showBitmapWindow->StartLoadingPicture();
	wxWindow* bitmapWindow = this->FindWindowById(THUMBNAILVIEWERPICTURE);
	if (bitmapWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_ONSTARTLOADINGPICTURE);
		evt.SetExtraLong(GetPhotoId(filename));
		//showBitmapWindow->GetEventHandler()->AddPendingEvent(evt);
		bitmapWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

bool CCentralWindow::SetAnimation(const wxString &filename)
{
	if(windowMode == WINDOW_VIEWER)
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


	SetPanelInfos(false);

	if (previewWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_SHOWSAVEBUTTON);
		previewWindow->GetEventHandler()->AddPendingEvent(evt);

	}

	if (previewWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_ANIMATIONSTART);
		previewWindow->GetEventHandler()->AddPendingEvent(evt);
	}
	//previewWindow->Resize();

	return result;
}


void CCentralWindow::SetVideo(const wxString &path)
{
    printf("CCentralWindow::SetVideo \n");
	StopAnimation();
	//bool refresh = isVideo ? false : true;
	filename = path;
	isVideo = true;
	isAnimation = false;
	isPicture = false;
	
	if(windowMode == WINDOW_VIEWER)
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
		previewWindow->SetVideo(path);

	SetPanelInfos(false);

}

void CCentralWindow::LoadingPicture(const wxString &filenameToShow)
{
   // int processPicture = 0;
#if defined(WIN32) && defined(_DEBUG)
	OutputDebugString(L"CCentralWindow::LoadingPicture");
	OutputDebugString(L"\n");
	OutputDebugString(filenameToShow);
	OutputDebugString(L"\n");
#endif
	filename = filenameToShow;
    printf("CCentralWindow::LoadingPicture \n");
	TRACE();


	//--------------------------------------------------------------------------------
	//Load Thumbnail
	//--------------------------------------------------------------------------------
	if (!isDiaporama)
	{
		CSqlThumbnail sqlThumbnail;
		//CImageLoadingFormat * _loadingPicture = new CImageLoadingFormat();
		CImageLoadingFormat* _loadingPicture = sqlThumbnail.GetPictureThumbnail(filenameToShow);
		if (_loadingPicture != nullptr && _loadingPicture->IsOk())
		{
			_loadingPicture->SetFilename(filenameToShow);
			//_loadingPicture->SetPicture(bitmapThumbnail);
			CBitmapReturn* bitmapReturn = new CBitmapReturn();
			bitmapReturn->myThread = nullptr;
			bitmapReturn->isThumbnail = true;
			bitmapReturn->bitmap = _loadingPicture;
			wxCommandEvent* event = new wxCommandEvent(EVENT_SHOWPICTURE);
			event->SetClientData(bitmapReturn);
			event->SetInt(processLoadPicture ? 1 : 0);
			wxQueueEvent(this, event);
		}
	}


    
    if (!processLoadPicture)
	{
#if defined(WIN32) && defined(_DEBUG)
		OutputDebugString(L"CCentralWindow::LoadingPicture OK Loading : ");
		OutputDebugString(L"\n");
		OutputDebugString(filenameToShow);
		OutputDebugString(L"\n");
#endif

		CThreadPictureData * pictureData = new CThreadPictureData();
		pictureData->mainWindow = this;
		pictureData->picture = filenameToShow;
		pictureData->isVisible = true;
		thread * threadloadPicture = new thread(LoadingNewPicture, pictureData);
		pictureData->myThread = threadloadPicture;
		processLoadPicture = true;
	}

    if (listPicture != nullptr)
    {
        listPicture->SetActifItem(GetPhotoId(filename), true);
       // listPicture->ForceRefresh();
    }

    if (thumbnailPicture != nullptr)
    {
        thumbnailPicture->SetActifItem(GetPhotoId(filename), true);
       // thumbnailPicture->ForceRefresh();
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
#if defined(WIN32) && defined(_DEBUG)
		OutputDebugString(L"CCentralWindow::EndPictureThread");
		OutputDebugString(L"\n");
		OutputDebugString(pictureData->picture);
		OutputDebugString(L"\n");
#endif

		if (pictureData->myThread != nullptr)
		{
			pictureData->myThread->join();
			delete pictureData->myThread;
		}
        localPicture = pictureData->picture;


		delete pictureData;
	}
}


//////////////////////////////////////////////////////////////////////////
//Picture Loading Function
//////////////////////////////////////////////////////////////////////////
void CCentralWindow::LoadingNewPicture(CThreadPictureData * pictureData)
{

#if defined(WIN32) && defined(_DEBUG)
	OutputDebugString(L"CCentralWindow::LoadingNewPicture : ");
	OutputDebugString(L"\n");
#endif

    printf("CCentralWindow::LoadingNewPicture \n");
	TRACE();
	CLibPicture libPicture;
	CImageLoadingFormat * bitmap = libPicture.LoadPicture(pictureData->picture);

	if (bitmap == nullptr || (bitmap->GetWidth() == 0 || bitmap->GetHeight() == 0))
	{
		if (bitmap != nullptr)
			delete bitmap;

		bitmap = libPicture.LoadPicture(CLibResource::GetPhotoCancel());
		bitmap->SetFilename(pictureData->picture);	
	}

	if (bitmap != nullptr)
	{
		CBitmapReturn * bitmapReturn = new CBitmapReturn();
		bitmapReturn->myThread = nullptr;
		bitmapReturn->isThumbnail = false;
		bitmapReturn->bitmap = bitmap;
		wxCommandEvent * event = new wxCommandEvent(EVENT_SHOWPICTURE);
		event->SetClientData(bitmapReturn);
		event->SetInt(0);
		wxQueueEvent(pictureData->mainWindow, event);
	}

}