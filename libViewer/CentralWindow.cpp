// ReSharper disable All
#include <header.h>
#include "CentralWindow.h"
#include "ViewerParamInit.h"
#include <LibResource.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <ImageLoadingFormat.h>
#include <libPicture.h>
#include <picture_id.h>
#include <ThumbnailMessage.h>
#include "PictureElement.h"
#include <wx/display.h>
#include "AnimationToolbar.h"
#include "PanelPhotoWnd.h"
#include <ConvertUtility.h>
#include "ListPicture.h"
#include "WindowManager.h"
#include "ThumbnailViewerVideo.h"
#include <SqlPhotos.h>
#include <ffplaycore.h>
#include "ThumbnailViewerPicture.h"
#ifndef __NOFACE_DETECTION__
#include "ListFace.h"
#endif
#include "ViewerParam.h"
#include "PanelInfosWnd.h"
#include <PanelWithClickToolbar.h>
#include "PreviewWnd.h"
#include <ParamInit.h>
#include <ShowElement.h>
#include <RegardsConfigParam.h>
#include <ImageVideoThumbnail.h>
#include <picture_utility.h>
#include <wx/busyinfo.h>
#include <OpenCVVideoPlayer.h>
#include "FolderProcess.h"
#include <MediaInfo.h>
#include <ThumbnailFolder.h>
#include <ThreadLoadingBitmap.h>
#include <ThumbnailFace.h>
using namespace Regards::Video;
using namespace Regards::Picture;
using namespace Regards::Window;
using namespace Regards::Sqlite;
using namespace Regards::Viewer;
using namespace Regards::FiltreEffet;

#define DELAY_ANIMATION 20

#define wxEVENT_ENDLOADPICTURE 1001

#include "window_mode_id.h"

extern bool firstElementToShow;
extern int numElementToLoad;

extern wxImage defaultPicture;

CCentralWindow::CCentralWindow(wxWindow* parent, wxWindowID id,
                               const CThemeSplitter& theme, const bool& horizontal)
	: CWindowMain("CentralWindow", parent, id)
{
	oldWindowMode = 0;
	panelPhotoWnd = nullptr;
	viewerconfig = nullptr;
	isFullscreen = false;
	isDiaporama = false;
	showToolbar = true;
	videoStart = false;


	wxRect rect;
	wxRect left;
	wxRect right;
	int widthInfosWindow = 0; //wxDisplay().GetGeometry().GetWidth() / 4;

	CMainParam* config = CMainParamInit::getInstance();
	CMainTheme* viewerTheme = CMainThemeInit::getInstance();

	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		faceDetection = regardsParam->GetFaceDetection();
	}

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
		left = {0, 0, 0, 0};

	if (right.width < 50)
		right = {0, 0, 0, 0};
	//Add Criteria Window
	if (viewerTheme != nullptr)
	{
		bool isPanelVisible = true;
		if (viewerconfig != nullptr)
			viewerconfig->GetShowFilter(isPanelVisible);

		wxString libelle = CLibResource::LoadStringFromResource(L"LBLFOLDERCATEGORY", 1);
		CThemePane theme_pane;
		viewerTheme->GetPaneTheme(&theme_pane);

		CThemeToolbar themetoolbar;
		viewerTheme->GetClickToolbarTheme(&themetoolbar);

		panelPhotoWnd = new CPanelPhotoWnd(windowManager, CRITERIAFOLDERWINDOWID);
		windowManager->AddPanel(panelPhotoWnd, Pos::wxLEFT, false, widthInfosWindow, left, libelle, "PanelPhotoSearch",
		                        true, PHOTOSEEARCHPANEL, true, true);
	}


	//----------------------------------------------------------------------------------------
	//Panel Thumbnail Video
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		bool checkValidity = true;
		bool isPanelVisible = true;

		if (config != nullptr)
		{
			config->GetShowVideoThumbnail(isPanelVisible);
			checkValidity = config->GetCheckThumbnailValidity();
		}

		wxString libelle = CLibResource::LoadStringFromResource(L"LBLTHUMBNAILVIDEO", 1);
		CThemePane theme_pane;
		CThemeToolbar themetoolbar;
		CThemeScrollBar themeScroll;
		CThemeThumbnail themeVideo;
		viewerTheme->GetPaneTheme(&theme_pane);
		viewerTheme->GetClickToolbarTheme(&themetoolbar);
		viewerTheme->GetScrollTheme(&themeScroll);
		viewerTheme->GetThumbnailTheme(&themeVideo);

		thumbnailVideo = new CThumbnailViewerVideo(windowManager, THUMBNAILVIDEOWINDOW, themeVideo, checkValidity);
		scrollVideoWindow = new CScrollbarWnd(windowManager, thumbnailVideo, wxID_ANY);
		scrollVideoWindow->HideVerticalScroll();
		scrollVideoWindow->SetPageSize(200);
		scrollVideoWindow->SetLineSize(200);

		windowManager->AddPanel(scrollVideoWindow, Pos::wxTOP, true,
		                        themeVideo.themeIcone.GetHeight() + theme_pane.GetHeight() * 2, rect, libelle,
		                        "ThumbnailVideoPanel", true, THUMBNAILVIDEOPANEL, true, true);
	}

	//----------------------------------------------------------------------------------------
	//Panel Thumbnail Picture
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		bool checkValidity = false;
		bool isPanelVisible = true;
		CMainTheme* viewer_theme = CMainThemeInit::getInstance();

		if (config != nullptr)
		{
			config->GetShowThumbnail(isPanelVisible);
			checkValidity = config->GetCheckThumbnailValidity();
		}

		wxString libelle = CLibResource::LoadStringFromResource(L"LBLTHUMBNAIL", 1);
		CThemePane theme_pane;
		CThemeToolbar themetoolbar;
		CThemeScrollBar themeScroll;
		CThemeThumbnail themeThumbnail;
		viewer_theme->GetPaneTheme(&theme_pane);
		viewer_theme->GetScrollTheme(&themeScroll);
		viewer_theme->GetThumbnailTheme(&themeThumbnail);
		viewer_theme->GetClickToolbarTheme(&themetoolbar);

		thumbnailPicture = new CThumbnailViewerPicture(windowManager, THUMBNAILVIEWERPICTURE, themeThumbnail,
		                                               checkValidity);
		scrollPictureWindow = new CScrollbarWnd(windowManager, thumbnailPicture, wxID_ANY);
		scrollPictureWindow->HideVerticalScroll();
		scrollPictureWindow->SetPageSize(200);
		scrollPictureWindow->SetLineSize(200);
		thumbnailPicture->SetNoVScroll(true);
		windowManager->AddPanel(scrollPictureWindow, Pos::wxBOTTOM, true,
		                        themeThumbnail.themeIcone.GetHeight() + theme_pane.GetHeight() * 2, rect, libelle,
		                        "ThumbnailPicturePanel", true, THUMBNAILPICTUREPANEL, true, true);
	}

	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);
		CThemePane theme_pane;
		viewerTheme->GetPaneTheme(&theme_pane);
		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickToolbarTheme(&themeClickInfosToolbar);
		//paneInfos = new CPanelWithClickToolbar(this, "CPictureInfosPanel", PANELCLICKINFOSWNDID, theme, themeClickInfosToolbar, libelle, showInfos, false, true);
		panelInfosWindow = new CPanelInfosWnd(windowManager, PANELINFOSWNDID);

		panelInfosClick = windowManager->AddPanel(panelInfosWindow, Pos::wxRIGHT, false, widthInfosWindow, right,
		                                          libelle, "PictureInfosPanel", true, PANELCLICKINFOSWNDID, false);
	}

	previewWindow = new CPreviewWnd(windowManager, PREVIEWVIEWERID);
	windowManager->AddWindow(previewWindow, Pos::wxCENTRAL, false, 0, rect, PREVIEWVIEWERID, false);


	if (viewerTheme != nullptr)
	{
		listPicture = new CListPicture(windowManager, LISTPICTUREID);
		listPicture->Show(false);
	}

	if (viewerTheme != nullptr)
	{
		listFace = new CListFace(windowManager, LISTFACEID);
		listFace->Show(false);
	}

	Connect(wxEVT_ANIMATIONTIMERSTOP, wxCommandEventHandler(CCentralWindow::StopAnimationEvent));
	Connect(wxTIMER_DIAPORAMATIMERSTART, wxCommandEventHandler(CCentralWindow::StartDiaporamaMessage));

	Connect(VIDEO_END_ID, wxCommandEventHandler(CCentralWindow::OnVideoEnd));
	Connect(wxEVENT_CHANGETYPEAFFICHAGE, wxCommandEventHandler(CCentralWindow::ChangeTypeAffichage));
	Connect(wxEVENT_SETMODEVIEWER, wxCommandEventHandler(CCentralWindow::SetMode));
	Connect(VIDEO_UPDATE_ID, wxCommandEventHandler(CCentralWindow::SetVideoPos));
	Connect(wxTIMER_ANIMATION, wxEVT_TIMER, wxTimerEventHandler(CCentralWindow::OnTimerAnimation), nullptr, this);
	//Connect(wxEVENT_LOADPICTURE, wxCommandEventHandler(CCentralWindow::OnLoadPicture));
	Connect(EVENT_SHOWPICTURE, wxCommandEventHandler(CCentralWindow::OnShowPicture));


	Connect(VIDEO_START, wxCommandEventHandler(CCentralWindow::OnVideoStart));
	Connect(wxVIDEO_STOP, wxCommandEventHandler(CCentralWindow::OnVideoStop));
	Connect(wxEVENT_ANIMATIONSTART, wxCommandEventHandler(CCentralWindow::OnAnimationStart));
	Connect(wxEVENT_ANIMATIONSTOP, wxCommandEventHandler(CCentralWindow::OnAnimationStop));

	Connect(wxEVENT_STOPVIDEO, wxCommandEventHandler(CCentralWindow::OnStopAudio));
	Connect(wxEVENT_ENDVIDEOTHREAD, wxCommandEventHandler(CCentralWindow::OnQuitAudio));

	Connect(wxEVENT_PICTUREPREVIOUS, wxCommandEventHandler(CCentralWindow::OnPicturePrevious));
	Connect(wxEVENT_PICTURENEXT, wxCommandEventHandler(CCentralWindow::OnPictureNext));
	Connect(wxEVENT_PICTUREFIRST, wxCommandEventHandler(CCentralWindow::OnPictureFirst));
	Connect(wxEVENT_PICTURELAST, wxCommandEventHandler(CCentralWindow::OnPictureLast));

	Connect(wxEVENT_STOPDIAPORAMA, wxCommandEventHandler(CCentralWindow::StopDiaporama));
	Connect(wxEVENT_STARTDIAPORAMA, wxCommandEventHandler(CCentralWindow::StartDiaporama));

	Connect(wxEVENT_THUMBNAILREFRESH, wxCommandEventHandler(CCentralWindow::OnRefreshThumbnail));
	Connect(wxEVENT_ICONEUPDATE, wxCommandEventHandler(CCentralWindow::UpdateThumbnailIcone));

	animationTimer = new wxTimer(this, wxTIMER_ANIMATION);
	processLoadPicture = false;
	windowManager->HideWindow(Pos::wxTOP, false);

	stopVideo = false;

	wxCommandEvent event(wxEVENT_SETMODEVIEWER);
	event.SetInt(windowMode);
	wxPostEvent(this, event);

	diaporamaTimer = new wxTimer(this, wxTIMER_DIAPORAMA);
	Connect(wxTIMER_DIAPORAMA, wxEVT_TIMER, wxTimerEventHandler(CCentralWindow::OnTimerDiaporama), nullptr, this);
}

void CCentralWindow::RefreshThumbnail(int type)
{
	if (listPicture != nullptr)
	{
		CThumbnailFolder* ptFolder = listPicture->GetPtThumbnailFolder();
		if (ptFolder->IsShown())
			ptFolder->Refresh();
	}
	if (listFace != nullptr)
	{
		CThumbnailFace * ptListFace = listFace->GetThumbnailFace();
		if (ptListFace->IsShown())
			ptListFace->Refresh();
	}
	if (thumbnailPicture != nullptr)
	{
		if (thumbnailPicture->IsShown())
			thumbnailPicture->Refresh();
	}
	if (thumbnailVideo != nullptr)
	{
		if (type == 1)
		{
			auto event = new wxCommandEvent(wxEVENT_REFRESHVIDEOTHUMBNAIL);
			wxQueueEvent(thumbnailVideo, event);
		}

		if (thumbnailVideo->IsShown())
			thumbnailVideo->Refresh();
	}
}

void CCentralWindow::OnRefreshThumbnail(wxCommandEvent& event)
{
	RefreshThumbnail(event.GetInt());
}

void CCentralWindow::UpdateThumbnailIcone(wxCommandEvent& event)
{
	CThreadLoadingBitmap* threadLoadingBitmap = (CThreadLoadingBitmap*)event.GetClientData();
	if (listPicture != nullptr)
	{
		CThumbnailFolder* ptFolder = listPicture->GetPtThumbnailFolder();
		ptFolder->UpdateRenderIcone(threadLoadingBitmap);
	}
	if (listFace != nullptr)
	{
		CThumbnailFace * thumbFace = listFace->GetThumbnailFace();
		if(thumbFace != nullptr)
			thumbFace->UpdateRenderIcone(threadLoadingBitmap);
	}
	if (thumbnailPicture != nullptr)
	{
		thumbnailPicture->UpdateRenderIcone(threadLoadingBitmap);
	}
	if (thumbnailVideo != nullptr)
	{
		thumbnailVideo->UpdateRenderIcone(threadLoadingBitmap);
		thumbnailVideo->UpdateVideoThumbnail(threadLoadingBitmap->filename);
	}

	RefreshThumbnail(threadLoadingBitmap->type);

	delete threadLoadingBitmap;
}


void CCentralWindow::OnPicturePrevious(wxCommandEvent& event)
{
	ImagePrecedente();

	/*
	wxWindow* mainWindow = this->FindWindowById(FRAMEVIEWER_ID);
	wxCommandEvent evt(wxEVENT_PICTUREENDLOADING);
	mainWindow->GetEventHandler()->AddPendingEvent(evt);
	*/
}

void CCentralWindow::OnPictureNext(wxCommandEvent& event)
{
	ImageSuivante();

	/*
	wxWindow* mainWindow = this->FindWindowById(FRAMEVIEWER_ID);
	wxCommandEvent evt(wxEVENT_PICTUREENDLOADING);
	mainWindow->GetEventHandler()->AddPendingEvent(evt);
	*/
}

void CCentralWindow::OnPictureFirst(wxCommandEvent& event)
{
	ImageDebut();
}

void CCentralWindow::OnPictureLast(wxCommandEvent& event)
{
	ImageFin();
}

void CCentralWindow::OnTimerDiaporama(wxTimerEvent& event)
{
	printf("OnTimerDiaporama \n");

	ImageSuivante();
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

	if (loadPicture)
	{
		//auto pictureElement = new CPictureElement();
		//pictureElement->filename = filename;
		//pictureElement->reloadResource = true;
		//wxCommandEvent evt(wxEVENT_LOADPICTURE);
		//evt.SetClientData(pictureElement);
		//this->GetEventHandler()->AddPendingEvent(evt);
		LoadPicture(filename, true);
	}
	else if (isDiaporama)
	{
		ImageSuivante();
	}

	loadPicture = false;
}

void CCentralWindow::StopDiaporama(wxCommandEvent& event)
{
	if (isDiaporama)
	{
		auto previewWindow = static_cast<CPreviewWnd*>(this->FindWindowById(PREVIEWVIEWERID));
		if (previewWindow != nullptr)
			previewWindow->StopDiaporamaMode();

		isDiaporama = false;
		if (diaporamaTimer->IsRunning())
			diaporamaTimer->Stop();
	}

	if (ffmfc != nullptr)
	{
		StopMusic();
		musicPause = true;
	}

	isDiaporama = false;
}

bool CCentralWindow::IsDiaporamaStart()
{
	return isDiaporama;
}

void CCentralWindow::TransitionEnd()
{
	if (isDiaporama)
	{
		//CMainParam* viewerParam = CMainParamInit::getInstance();
		//const int timeDelai = viewerParam->GetDelaiDiaporamaOption();
		//diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
		diaporamaTimer->Start(1000, wxTIMER_ONE_SHOT);
	}
	if (musicPause && isDiaporama)
	{
		musicPause = false;
		StartMusic();
	}
}

void CCentralWindow::StartDiaporama(wxCommandEvent& event)
{
	CMainParam* viewerParam = CMainParamInit::getInstance();
	if (viewerParam != nullptr)
	{
		auto previewWindow = static_cast<CPreviewWnd*>(this->FindWindowById(PREVIEWVIEWERID));
		if (previewWindow != nullptr)
			previewWindow->SetDiaporamaMode();

		const int timeDelai = viewerParam->GetDelaiDiaporamaOption();
		diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
	}

	StartMusic();
	isDiaporama = true;
}

void CCentralWindow::OnVideoStop(wxCommandEvent& event)
{
	if (videoStart && isDiaporama)
	{
		stopVideo = true;
		previewWindow->StopVideo();
	}
}

void CCentralWindow::OnVideoStart(wxCommandEvent& event)
{
	printf("CCentralWindow::OnVideoStart \n");

	if (ffmfc != nullptr)
	{
		StopMusic();
		musicPause = true;
	}

	videoStart = true;
	if (thumbnailVideo != nullptr)
	{
		thumbnailVideo->SetVideoPosition(0);
	}
}

wxString CCentralWindow::ImageSuivante(const bool& loadPicture)
{
	wxString localFilename = "";
	if (windowMode)
	{
		int numItem = 0;
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
		else if (windowMode == WINDOW_FACE)
		{
			numItem = listFace->ImageSuivante();
			localFilename = listFace->GetFilename(numItem);
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
	if (windowMode)
	{
		int numItem = 0;
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
	if (windowMode)
	{
		int numItem = 0;
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
	if (windowMode)
	{
		int numItem = 0;
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
		else if (windowMode == WINDOW_FACE)
		{
			numItem = listFace->ImagePrecedente();
			localFilename = listFace->GetFilename(numItem);
		}
	}


	if (localFilename != "" && loadPicture)
	{
		isNext = false;
		LoadPicture(localFilename);
	}

	return localFilename;
}

int CCentralWindow::LoadPicture(const wxString& filename, const bool& refresh)
{
    if(numElementToLoad != 0)
        firstElementToShow = false;
    
	//return RefreshPicture(filename);
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
	else if (windowMode == WINDOW_FACE)
	{
		if (listFace != nullptr)
		{
			numLocalItem = listFace->GetNumItem();
		}
	}

	if (filename != this->filename || refresh)
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
		//previewWindow->StopVideo();
		loadPicture = false;

		bool errorPhoto = false;
		bool needToLoadPicture = false;
		wxString pictureToLoad = filename;

		if (wxFileExists(filename))
		{
			CLibPicture libPicture;
			bool isVideoValid = true;
			bool isVideo = false;

			if (libPicture.TestIsVideo(filename))
			{
				isVideoValid = libPicture.TestIsVideoValid(filename);
				isVideo = true;
			}

			if (isVideo && isVideoValid)
			{
				StopMusic();
				SetVideo(filename);
			}
			else if (isVideo && !isVideoValid)
			{
				//CImageLoadingFormat* bitmap = libPicture.GetCancelPhoto(filename);
				//SetBitmap(bitmap, false, false);
				errorPhoto = true;
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
				StartLoadingPicture();
				needToLoadPicture = true;
			}
		}
		else
		{
			errorPhoto = true;
		}

		if (errorPhoto)
		{
			pictureToLoad = CLibResource::GetPhotoCancel();
			needToLoadPicture = true;
			StartLoadingPicture();
		}

		if (needToLoadPicture)
		{
			isPicture = true;
			//--------------------------------------------------------------------------------
			//Load Thumbnail
			//--------------------------------------------------------------------------------
			if (!isDiaporama)
			{
				CSqlThumbnail sqlThumbnail;
				CImageLoadingFormat* _loadingPicture = sqlThumbnail.GetPictureThumbnail(pictureToLoad);
				if (_loadingPicture != nullptr && _loadingPicture->IsOk())
				{
					_loadingPicture->SetFilename(pictureToLoad);
					//_loadingPicture->Flip();
					CBitmapReturn* bitmapReturn = new CBitmapReturn();
					bitmapReturn->myThread = nullptr;
					bitmapReturn->isThumbnail = true;
					bitmapReturn->bitmap = _loadingPicture;
					auto event = new wxCommandEvent(EVENT_SHOWPICTURE);
					event->SetClientData(bitmapReturn);
					event->SetInt(processLoadPicture ? 1 : 0);
					wxQueueEvent(this, event);


					wxWindow* mainWindow = this->FindWindowById(FRAMEVIEWER_ID);
					wxCommandEvent evt(wxEVENT_PICTUREENDLOADING);
					mainWindow->GetEventHandler()->AddPendingEvent(evt);
				}
			}


			if (!processLoadPicture)
			{
				auto pictureData = new CThreadPictureData();
				pictureData->mainWindow = this;
				pictureData->picture = pictureToLoad;
				pictureData->isVisible = true;
				pictureData->isDiaporama = isDiaporama;
				const auto threadloadPicture = new thread(LoadingNewPicture, pictureData);
				pictureData->myThread = threadloadPicture;
				processLoadPicture = true;
			}
		}
	}

	if (thumbnailPicture != nullptr)
		thumbnailPicture->SetActifItem(GetPhotoId(filename), true);
	if (listPicture != nullptr)
		listPicture->SetActifItem(GetPhotoId(filename), true);
	if (listFace != nullptr)
		listFace->SetActifItem(GetPhotoId(filename), true);

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
	else if (windowMode == WINDOW_FACE)
	{
		if (listFace != nullptr)
		{
			outItem = listFace->GetNumItem();
		}
	}

	if (outItem != numLocalItem)
	{
		if (outItem > numLocalItem)
			isNext = true;
		else
			isNext = false;
	}

	numElementToLoad++;

	return 0;
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

	wxWindow* window = this->FindWindowById(PREVIEWVIEWERID);
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

	if (windowMode == WINDOW_VIEWER)
		windowManager->ShowWindow(Pos::wxBOTTOM);

	if (isFullscreen)
	{
		if (windowMode == WINDOW_VIEWER)
		{
			windowManager->ShowWindow(Pos::wxRIGHT);
		}

		//windowManager->HideWindow(Pos::wxCENTRAL);
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

void CCentralWindow::ShowPicture(CBitmapReturn* pictureData, const int& redraw)
{
	processLoadPicture = false;
	bool isPictureToShow = false;

	if (pictureData->bitmap != nullptr)
	{
		if (filename == pictureData->bitmap->GetFilename())
			isPictureToShow = true;
	}

	if (isPictureToShow)
	{
		if (pictureData != nullptr)
		{
			//SetPicture(pictureData->bitmap, pictureData->isThumbnail);
			if (pictureData->bitmap != nullptr && pictureData->bitmap->IsOk())
			{
				//Send Infos about Picture
				{
					auto pictureInfos = new CPictureInfosMessage();
					pictureInfos->filename = filename;
					pictureInfos->infos = to_string(pictureData->bitmap->GetWidth()) + "x" + to_string(
						pictureData->bitmap->GetHeight());

					wxWindow* mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
					wxCommandEvent evt(wxEVENT_INFOS);
					evt.SetClientData(pictureInfos);
					mainWindow->GetEventHandler()->AddPendingEvent(evt);
				}

				StopAnimation();
				bool isSetImage = false;
				isAnimation = false;
				isPicture = true;
				isVideo = false;
				windowManager->HideWindow(Pos::wxTOP);
				previewWindow->IsNextPicture(isNext);

				if (isDiaporama)
				{
					if (previewWindow != nullptr)
					{
						if (previewWindow->SetBitmap(pictureData->bitmap, pictureData->isThumbnail, isAnimation))
						{
							isSetImage = true;
							isVideo = false;
						}
					}
				}
				else
				{
					if (previewWindow != nullptr)
					{
						previewWindow->HideValidationToolbar();
						if (previewWindow->SetBitmap(pictureData->bitmap, pictureData->isThumbnail, isAnimation))
						{
							isSetImage = true;
							isVideo = false;
							SetPanelInfos(pictureData->isThumbnail);
						}
					}
				}

				if (!isSetImage)
				{
					if (pictureData->bitmap != nullptr)
						delete pictureData->bitmap;

					pictureData->bitmap = nullptr;
				}
			}


			if (!pictureData->isThumbnail)
				StopLoadingPicture();
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
		}
	}

	if (redraw)
	{
		LoadPicture(filename, true);
	}
}

void CCentralWindow::OnShowPicture(wxCommandEvent& event)
{
	CBitmapReturn* pictureData = static_cast<CBitmapReturn*>(event.GetClientData());
	if (pictureData != nullptr)
	{
		int redraw = event.GetInt();
		if (isPicture)
		{
			ShowPicture(pictureData, redraw);
		}
		else
		{
			delete pictureData->bitmap;
			pictureData->bitmap = nullptr;
		}

		if (pictureData->myThread != nullptr)
		{
			delete pictureData->myThread;
			pictureData->myThread = nullptr;
		}
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
		bitmapWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}


void CCentralWindow::StopAnimation()
{
	printf("CCentralWindow::StopAnimation \n");
	animationPosition = 0;
	if (animationTimer->IsRunning())
		animationTimer->Stop();
}


void CCentralWindow::SetPanelInfos(const bool& isThumbnail)
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
		if (!isThumbnail)
		{
			panelInfosWindow->SetBitmapFile(filename, isThumbnail);
			panelInfosWindow->Refresh();
			panelInfosWindow->Update();
		}
	}
}


void CCentralWindow::SetVideoPos(wxCommandEvent& event)
{
	//printf("CCentralWindow::SetVideoPos \n");
	int64_t pos = event.GetExtraLong();
	if (thumbnailVideo != nullptr)
		thumbnailVideo->SetVideoPosition(pos);
}

void CCentralWindow::OnTimerAnimation(wxTimerEvent& event)
{
	//printf("CViewerWindow::OnTimerAnimation %d \n", animationPosition);

	if (thumbnailVideo != nullptr)
		thumbnailVideo->SetVideoPosition(animationPosition);

	LoadAnimationBitmap(filename, animationPosition);
	animationPosition++;
	if (animationPosition < nbThumbnail)
	{
		uint32_t delay;
		CLibPicture libPicture;
		const int iFormat = libPicture.TestImageFormat(filename);
		if (iFormat != TIFF && iFormat != PDF)
		{
			if (videoThumbnail.size() > animationPosition)
			{
				CImageVideoThumbnail* video = videoThumbnail.at(animationPosition);
				delay = video->delay;
			}
		}
		else
		{
			delay = libPicture.GetFrameDelay(filename);
		}
		animationTimer->Start(delay, wxTIMER_ONE_SHOT);
	}
	else
	{
		//Stop
		auto animationToolbar = dynamic_cast<CAnimationToolbar*>(this->FindWindowById(ANIMATIONTOOLBARWINDOWID));
		if (animationToolbar != nullptr)
			animationToolbar->AnimationStop();

		wxCommandEvent evt(wxEVT_ANIMATIONTIMERSTOP);
		this->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CCentralWindow::StartDiaporamaMessage(wxCommandEvent& event)
{
	CMainParam* viewerParam = CMainParamInit::getInstance();
	const int timeDelai = viewerParam->GetDelaiDiaporamaOption();
	diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
}

void CCentralWindow::StopAnimationEvent(wxCommandEvent& event)
{
	if (isDiaporama)
	{
		CMainParam* viewerParam = CMainParamInit::getInstance();
		const int timeDelai = viewerParam->GetDelaiDiaporamaOption();
		diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
	}
}


void CCentralWindow::AnimationSetPosition(wxCommandEvent& event)
{
	printf("CCentralWindow::AnimationSetPosition \n");

	const auto videoTime = static_cast<int64_t*>(event.GetClientData());
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
	if (!init)
	{
		if (wxDisplay().GetGeometry().GetWidth() == GetWindowWidth())
		{
			init = true;
		}
		else
		{
			if (windowManager != nullptr)
				windowManager->SetSize(0, 0, wxDisplay().GetGeometry().GetWidth(), wxDisplay().GetGeometry().GetHeight());
		}
	}

    windowManager->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
}

void CCentralWindow::LoadAnimationBitmap(const wxString& filename, const int& numFrame)
{
	if (numFrame == oldAnimationPosition && filename == oldFilename)
		return;

	bool isSetImage = false;
	oldFilename = filename;
	oldAnimationPosition = numFrame;
	bool isThumbnail = false;
	bool isAnimation = true;
	CImageLoadingFormat* image = nullptr;

	if (numFrame < nbThumbnail && numFrame >= 0)
	{
		CLibPicture libPicture;
		const int iFormat = libPicture.TestImageFormat(filename);
		if (iFormat != TIFF && iFormat != PDF)
		{
			if (videoThumbnail.size() > numFrame)
			{
				CImageVideoThumbnail* thumbnail = videoThumbnail.at(numFrame);
				if (thumbnail != nullptr)
				{
					image = new CImageLoadingFormat();
					image->SetPicture(thumbnail->image);
					image->SetFilename(thumbnail->filename);
				}
			}
		}
		else
		{
			image = libPicture.LoadPicture(filename, false, numFrame);
		}
		//SetBitmap(image, false, true);
	}

	previewWindow->IsNextPicture(isNext);

	if (image != nullptr && image->IsOk())
	{
		isVideo = false;

		if (isDiaporama)
		{
			if (previewWindow != nullptr)
			{
				if (previewWindow->SetBitmap(image, isThumbnail, isAnimation))
				{
					isSetImage = true;
					isVideo = false;
				}
			}
		}
		else
		{
			if (previewWindow != nullptr)
			{
				previewWindow->HideValidationToolbar();
				if (previewWindow->SetBitmap(image, isThumbnail, isAnimation))
				{
					isSetImage = true;
					isVideo = false;
					SetPanelInfos(isThumbnail);
				}
			}
		}
	}

	if (!isSetImage)
	{
		if (image != nullptr && image->IsOk())
			delete image;
		image = nullptr;
	}
}


void CCentralWindow::ChangeTypeAffichage(wxCommandEvent& event)
{
	if (listPicture != nullptr)
	{
		const long typeAffichage = event.GetExtraLong();
		listPicture->ChangeTypeAffichage(typeAffichage);
	}
}



void CCentralWindow::SetListeFile(const wxString& filename, const int& typeAffichage)
{

	if (listPicture != nullptr)
		listPicture->SetListeFile(typeAffichage);

	if (thumbnailPicture != nullptr)
		thumbnailPicture->ApplyListeFile();

	LoadPicture(filename);
}

CCentralWindow::~CCentralWindow()
{
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		config->SetLastShowPicture(filename);

	if (diaporamaTimer->IsRunning())
		diaporamaTimer->Stop();

	delete(diaporamaTimer);
    delete(thumbnailVideo);
    delete(panelInfosWindow);
    delete(previewWindow);
    delete(scrollPictureWindow);
    delete(scrollVideoWindow);
    delete(windowManager);
    
	if (ffmfc != nullptr)
		delete ffmfc;
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
		if (showFolder)
			config->SetPositionLeftPanel(left);

		showVideoThumbnail = windowManager->GetPaneState(Pos::wxTOP);
		showThumbnail = windowManager->GetPaneState(Pos::wxBOTTOM);
		showInfos = windowManager->GetPaneState(Pos::wxRIGHT);
		if (showInfos)
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
		CMainParam* main_param = CMainParamInit::getInstance();
		if (main_param != nullptr)
			main_param->SetShowVideoThumbnail(showVideoThumbnail);
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
	if (faceDetection)
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
	default: ;
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
		windowManager->Resize();
	}

	return true;
}

bool CCentralWindow::IsCompatibleFullscreen()
{
	if (windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE)
		return true;
	return false;
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
	else
	{
		if (windowMode == WINDOW_PICTURE)
		{
			windowManager->HideWindow(Pos::wxRIGHT);
			windowManager->Resize();
		}
	}
	return true;
}

bool CCentralWindow::GetProcessEnd()
{
	StopAnimation();

	if (diaporamaTimer->IsRunning())
		diaporamaTimer->Stop();

	if (processLoadPicture)
		return false;

	if (isDiaporama)
	{
		if (ffmfc != nullptr && !ffmfcQuit)
		{
			isDiaporama = false;
			ffmfc->Quit();
			ffmfcQuit = true;
		}
		return musicStop;
	}

	return true;
}


void CCentralWindow::SetPosition(const long& timePosition)
{
	if (isVideo)
	{
		auto showVideoWindow = static_cast<CShowElement*>(this->FindWindowById(SHOWBITMAPVIEWERID));
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

int CCentralWindow::GetPhotoId(const wxString& filename)
{
	CSqlPhotos photo;
	return photo.GetPhotoId(filename);
}

void CCentralWindow::StartLoadingPicture()
{
	wxWindow* bitmapWindow = this->FindWindowById(THUMBNAILVIEWERPICTURE);
	if (bitmapWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_ONSTARTLOADINGPICTURE);
		evt.SetExtraLong(GetPhotoId(filename));
		bitmapWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

bool CCentralWindow::SetAnimation(const wxString& filename)
{
    
    printf("CCentralWindow::SetAnimation : %s \n", filename.ToStdString().c_str());
    
	if (windowMode == WINDOW_VIEWER)
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
	//if (thumbnailVideo->GetFilename() != filename)
	//{
		thumbnailVideo->SetFilename(filename);
		if (nbThumbnail > 0)
			thumbnailVideo->SetFile(filename, nbThumbnail);
	//}

	if (videoThumbnail.size() > 0)
	{
		for (int i = 0; i < videoThumbnail.size(); i++)
		{
			CImageVideoThumbnail* imageVideo = videoThumbnail.at(i);
			delete imageVideo;
		}
	}

	videoThumbnail.clear();

	if (iFormat != TIFF && iFormat != PDF)
		videoThumbnail = libPicture.LoadAllVideoThumbnail(filename, false, false);

	animationPosition = 0;

	LoadAnimationBitmap(filename, 0);
	if (refresh)
		needToRefresh = true;


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
	return result;
}


void CCentralWindow::SetVideo(const wxString& path)
{
	StopAnimation();
	filename = path;
	isVideo = true;
	isAnimation = false;
	isPicture = false;

	if (windowMode == WINDOW_VIEWER)
	{
		if((showToolbar && isFullscreen) || !isFullscreen)
			windowManager->ShowWindow(Pos::wxTOP);
	}
		

	if (thumbnailVideo->GetFilename() != filename)
	{
		int64_t duration = CMediaInfo::GetVideoDuration(filename); 

		if (duration > 20)
			thumbnailVideo->SetFile(filename, 20);
		else
			thumbnailVideo->SetFile(filename, duration);
	}
    
    
	if (previewWindow != nullptr)
		previewWindow->SetVideo(path);

	SetPanelInfos(false);
}


//////////////////////////////////////////////////////////////////////////
//Picture Loading Function
//////////////////////////////////////////////////////////////////////////
void CCentralWindow::LoadingNewPicture(CThreadPictureData* pictureData)
{
	CLibPicture libPicture;
	CImageLoadingFormat* bitmap = libPicture.LoadPicture(pictureData->picture);

	if (bitmap == nullptr || (bitmap->GetWidth() == 0 || bitmap->GetHeight() == 0))
	{
		bitmap->SetPicture(defaultPicture);
	}
	bitmap->SetFilename(pictureData->picture);


	if (bitmap != nullptr)
	{
		auto bitmapReturn = new CBitmapReturn();
		bitmapReturn->myThread = nullptr;
		bitmapReturn->isThumbnail = false;
		bitmapReturn->bitmap = bitmap;
		auto event = new wxCommandEvent(EVENT_SHOWPICTURE);
		event->SetClientData(bitmapReturn);
		event->SetInt(0);
		wxQueueEvent(pictureData->mainWindow, event);
	}
}


void CCentralWindow::OnQuitAudio(wxCommandEvent& event)
{
	musicStop = true;
	//wxSleep(1);
}

void CCentralWindow::OnStopAudio(wxCommandEvent& event)
{
	CRegardsConfigParam* config = CParamInit::getInstance();
	wxString musicDiaporama = "";
	musicStop = true;
	if (config != nullptr)
		musicDiaporama = config->GetMusicDiaporama();
	if (musicDiaporama != "" && !ffmfcQuit)
	{
		ffmfc->Quit();
		ffmfc->SetFile(nullptr, CConvertUtility::ConvertToStdString(musicDiaporama), "", false, 100);
		musicStop = false;
	}
}

void CCentralWindow::StartMusic()
{
	CRegardsConfigParam* config = CParamInit::getInstance();
	wxString musicDiaporama = "";

	if (config != nullptr)
		musicDiaporama = config->GetMusicDiaporama();
	if (ffmfc == nullptr && !musicDiaporama.empty() && wxFileExists(musicDiaporama))
	{
		ffmfc = new CFFmfc(this, wxNewId());
	}


	if (!musicDiaporama.empty() && wxFileExists(musicDiaporama))
	{
		if (!musicDiaporama.empty() && musicStop)
		{
			ffmfc->SetFile(nullptr, CConvertUtility::ConvertToStdString(musicDiaporama), "", false, 100);
			musicStop = false;
			ffmfc->SetTimePosition(musicPosition);
		}
	}
}

void CCentralWindow::StopMusic()
{
	if (!musicStop)
	{
		musicPosition = ffmfc->GetTimePosition();
		if (ffmfc != nullptr)
			ffmfc->Quit();
		ffmfcQuit = true;
		musicStop = true;
	}
}
