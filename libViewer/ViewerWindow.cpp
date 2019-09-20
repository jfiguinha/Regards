#include <header.h>
#include "ViewerWindow.h"
#include "ThumbnailViewerVideo.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <ImageLoadingFormat.h>
#include <ImageVideoThumbnail.h>
#include <FileUtility.h>
#include <LibResource.h>
#include "ViewerTheme.h"
#include <libPicture.h>
#include <ShowVideo.h>
#include "ViewerThemeInit.h"
#include <window_id.h>
#include <ParamInit.h>
#include <StatusText.h>
#include <picture_id.h>
#include "PictureElement.h"
#include "ThumbnailMessage.h"
#include <SqlThumbnailVideo.h>

using namespace Regards::Window;
using namespace Regards::Viewer;
#define PANE_PICTURETHUMBNAIL 1
#define PANE_VIDEOTHUMBNAIL 2
#define DELAY_ANIMATION 20

CViewerWindow::CViewerWindow(wxWindow* parent, wxWindowID id)
	: CWindowMain("CentralWindow", parent, id)
{
	previewInfosWnd = nullptr;
	viewerParam = nullptr;
	isFullscreen = false;
	filename = L"";
	width = 0;
	height = 0;

	isDiaporama = false;
    showToolbar = true;
	checkValidity = false;
	isAnimation = false;
	isPicture = false;
	isVideo = false;

	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();
	CViewerParam * config = CViewerParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

	//----------------------------------------------------------------------------------------
	//Panel Thumbnail Video
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		bool checkValidity = true;
		bool isPanelVisible = true;
		CViewerTheme* viewerTheme = CViewerThemeInit::getInstance();
		CViewerParam* config = CViewerParamInit::getInstance();
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

		panelVideo = new CPanelWithClickToolbar(this, "CThumbnailVideoPanel", THUMBNAILVIDEOPANEL, theme, themetoolbar, libelle, isPanelVisible, false, false);
		scrollVideoWindow = new CScrollbarWnd(panelVideo->GetPaneWindow(), wxID_ANY);
		thumbnailVideo = new CThumbnailViewerVideo(scrollVideoWindow, wxID_ANY, themeVideo, checkValidity);
		scrollVideoWindow->SetCentralWindow(thumbnailVideo, themeScroll);
		scrollVideoWindow->HideVerticalScroll();
		scrollVideoWindow->SetPageSize(1000);
		scrollVideoWindow->SetLineSize(200);
		panelVideo->SetWindow(scrollVideoWindow);
	}

	//----------------------------------------------------------------------------------------
	//Panel Picture Infos
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetSplitterTheme(&theme);

		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickToolbarTheme(&themeClickInfosToolbar);
		previewInfosWnd = new CPreviewInfosWnd(this, PREVIEWINFOWND, theme, false);
	}

	//----------------------------------------------------------------------------------------
	//Panel Thumbnail Picture
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		bool checkValidity = false;
		bool isPanelVisible = true;
		CViewerTheme* viewerTheme = CViewerThemeInit::getInstance();
		CViewerParam* config = CViewerParamInit::getInstance();
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
		panelPicture = new CPanelWithClickToolbar(this, "CThumbnailPicturePanel", THUMBNAILPICTUREPANEL, theme, themetoolbar, libelle, isPanelVisible, true, false);
		scrollPictureWindow = new CScrollbarWnd(panelPicture->GetPaneWindow(), wxID_ANY);
		thumbnailPicture = new CThumbnailViewerPicture(scrollPictureWindow, THUMBNAILVIEWERPICTURE, themeThumbnail, checkValidity);
		scrollPictureWindow->SetCentralWindow(thumbnailPicture, themeScroll);
		scrollPictureWindow->HideVerticalScroll();
		scrollPictureWindow->SetPageSize(1000);
		scrollPictureWindow->SetLineSize(200);
		thumbnailPicture->SetNoVScroll(true);
		panelPicture->SetWindow(scrollPictureWindow);
	}
	
	Connect(VIDEO_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CViewerWindow::SetVideoPos));
	Connect(wxTIMER_ANIMATION, wxEVT_TIMER, wxTimerEventHandler(CViewerWindow::OnTimerAnimation), nullptr, this);
    Connect(wxEVT_ANIMATIONPOSITION, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CViewerWindow::AnimationSetPosition));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CViewerWindow::OnSize));
	Connect(wxEVENT_RESIZE, wxCommandEventHandler(CViewerWindow::OnResize));
    Connect(wxEVENT_REFRESHDATA, wxCommandEventHandler(CViewerWindow::OnRefreshData));
	Connect(wxEVENT_LOADPICTURE, wxCommandEventHandler(CViewerWindow::OnLoadPicture));
	Connect(EVENT_SHOWPICTURE, wxCommandEventHandler(CViewerWindow::OnShowPicture));
	Connect(EVENT_ENDNEWPICTURETHREAD, wxCommandEventHandler(CViewerWindow::EndPictureThread));

	animationTimer = new wxTimer(this, wxTIMER_ANIMATION);
	processLoadPicture = false;
}

void CViewerWindow::SetListeFile(PhotosVector * photoVector)
{
	if (thumbnailPicture != nullptr)
		thumbnailPicture->SetListeFile(photoVector);
}

void CViewerWindow::SetVideoPos(wxCommandEvent& event)
{
	int64_t pos = event.GetExtraLong();
	SetVideoPosition(pos);
}

void CViewerWindow::OnTimerAnimation(wxTimerEvent& event)
{
    printf("CViewerWindow::OnTimerAnimation %d \n", animationPosition);
	SetVideoPosition(animationPosition);
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
        printf("CViewerWindow::OnTimerAnimation Start delay %d \n", delay);
		animationTimer->Start(delay, wxTIMER_ONE_SHOT);
	}
	else
	{
		//Stop
		CAnimationToolbar * animationToolbar = (CAnimationToolbar *)this->FindWindowById(ANIMATIONTOOLBARWINDOWID);
		if (animationToolbar != nullptr)
			animationToolbar->AnimationStop();
            
        CWindowMain * windowMain = (CWindowMain *)this->FindWindowById(MAINVIEWERWINDOWID);
        if(windowMain != nullptr)
        {
            wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVT_ANIMATIONTIMERSTOP);
            windowMain->GetEventHandler()->AddPendingEvent(evt);       
        }            
	}
}

void CViewerWindow::OnRefreshData(wxCommandEvent& event)
{
	wxWindow* window = this->FindWindowById(THUMBNAILFOLDER);
	if (window)
	{
		wxCommandEvent evt(wxEVENT_REFRESHTHUMBNAIL);
		evt.SetExtraLong(1);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CViewerWindow::AnimationPictureNext()
{
	animationPosition++;
	if (animationPosition >= nbThumbnail)
		animationPosition = nbThumbnail - 1;
	SetVideoPosition(animationPosition);
	LoadAnimationBitmap(filename, animationPosition);
}

void CViewerWindow::AnimationPicturePrevious()
{
	animationPosition--;
	if (animationPosition < 0)
		animationPosition = 0;
	SetVideoPosition(animationPosition);
	LoadAnimationBitmap(filename, animationPosition);
}

CViewerWindow::~CViewerWindow()
{
	if (videoThumbnail.size() > 0)
	{
		for (int i = 0; i < videoThumbnail.size(); i++)
		{
			CImageVideoThumbnail * imageVideo = videoThumbnail.at(i);
			delete imageVideo;
		}
	}

	videoThumbnail.clear();

	if (animationTimer->IsRunning())
		animationTimer->Stop();

	delete(animationTimer);

	delete(thumbnailVideo);
	delete(scrollVideoWindow);
	delete(panelVideo);
	delete(previewInfosWnd);
}

void CViewerWindow::SetDiaporamaMode()
{
	isDiaporama = true;
	if (previewInfosWnd != nullptr)
		previewInfosWnd->SetDiaporamaMode();

	this->RedrawBarPos();
}

void CViewerWindow::SetNormalMode()
{
	isDiaporama = false;
	if (previewInfosWnd != nullptr)
		previewInfosWnd->SetNormalMode();
	this->RedrawBarPos();
}

void CViewerWindow::HideToolbar()
{
    showToolbar = false;
	if (previewInfosWnd != nullptr)
	{
		if (isFullscreen)
		{
			panelVideo->HidePanel(false);
		}
		//previewInfosWnd->HideToolbar();

		wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_HIDETOOLBAR);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		this->RedrawBarPos();
	}
}

void CViewerWindow::ShowToolbar()
{
    showToolbar = true;
	if (previewInfosWnd != nullptr)
	{
		if (isFullscreen && !isPicture)
		{
			panelVideo->ShowPanel();
		}

		//previewThumbnailSplitter->ShowToolbar();

		wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SHOWTOOLBAR);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		this->RedrawBarPos();
	}
}

 void CViewerWindow::HidePanel()
 {
    if (panelVideo != nullptr)
		panelVideo->HidePanel();

	if (panelPicture != nullptr)
		panelPicture->HidePanel();

	this->RedrawBarPos();    
 }

void CViewerWindow::ShowPanelVideoThumbnail()
{
	if(isVideo)
	{
		if (panelVideo != nullptr)
		{
			panelVideo->ShowPanel();
		}
	}

	panelPicture->ShowPanel();

	this->RedrawBarPos();
}

void CViewerWindow::SetVideoPosition(const int64_t &videoTime)
{
    int64_t * data = new int64_t();
    *data = videoTime;
    wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVT_ANIMATIONPOSITION);
    evt.SetClientData(data);
    this->GetEventHandler()->AddPendingEvent(evt);       
}

void CViewerWindow::SetPosition(const long &timePosition)
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

void CViewerWindow::UpdateScreenRatio()
{
    printf("CViewerWindow::UpdateScreenRatio() \n");
	panelVideo->UpdateScreenRatio();
	previewInfosWnd->UpdateScreenRatio();
	panelPicture->UpdateScreenRatio();
    this->Resize();
}

void CViewerWindow::SetEffect(const bool &effect)
{

	if (previewInfosWnd != nullptr)
		previewInfosWnd->SetEffect(effect);
}

void CViewerWindow::OnResize(wxCommandEvent& event)
{
	RedrawBarPos();
}

void CViewerWindow::Resize()
{
	RedrawBarPos();
}

wxRect CViewerWindow::GetWindowRect()
{
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = width;
	rc.height = height;
	return rc;
}


void CViewerWindow::RedrawBarPos()
{
    if(isPicture)
    {
		panelVideo->HidePanel(false);
    }
	
    
	if (isDiaporama)
	{
		panelVideo->Show(false);

		if (previewInfosWnd != nullptr)
			previewInfosWnd->SetSize(0, 0, width, height);
	}
	else
	{
		//wxRect rc = GetWindowRect();
		int bottomHeight = 0;
		int topHeight = 0;
		if(!isPicture && !isFullscreen && !panelVideo->IsShown())
		{
			panelVideo->Show();
			//if(!thumbnailVideoPanel->IsPanelVideoThumbnailVisible())
			panelVideo->ShowPanel();
		}

		if (!isPicture && panelVideo->IsShown())
		{
			int iconeHeight = panelVideo->GetHeight();
			panelVideo->SetSize(0, 0, width, iconeHeight);
			panelVideo->Refresh();
   			topHeight += iconeHeight;
		}


        
        if(!showToolbar && isFullscreen)
        {
			panelPicture->HidePanel(false);
			panelVideo->HidePanel(false);
            bottomHeight = 0;
            topHeight = 0;             
        }
		else
		{
			panelPicture->ShowPanel();
			bottomHeight = panelPicture->GetHeight();
			panelPicture->SetSize(0, height - bottomHeight, width, bottomHeight);
		}
             
		if (previewInfosWnd != nullptr)
		{
			previewInfosWnd->SetSize(0, topHeight, width, height - (topHeight + bottomHeight));
		}
	}
    
    printf("CViewerWindow::RedrawBarPos() \n");
}


void CViewerWindow::OnSize(wxSizeEvent& event)
{
    
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	RedrawBarPos();
}

void CViewerWindow::SetVideo(const wxString &path)
{
	StopAnimation();
	bool refresh = isVideo ? false : true;
	filename = path;
	isVideo = true;
	isAnimation = false;
	isPicture = false;
    //thumbnailVideoPanel->SetVideo(path);
    //thumbnailVideoPanel->ShowVideoThumbnail();
	if (thumbnailVideo->GetFilename() != filename)
	{
		thumbnailVideo->SetFilename(filename);
		thumbnailVideo->SetFile(filename);
	}

    if (previewInfosWnd != nullptr)
		previewInfosWnd->SetVideo(path);
	if(refresh)
		this->RedrawBarPos();
}

void CViewerWindow::StartLoadingPicture(const int &numElement)
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

void CViewerWindow::AnimationSetPosition(wxCommandEvent& event)
{
    int64_t * videoTime = (int64_t *)event.GetClientData();
	if (thumbnailVideo != nullptr && videoTime != nullptr)
		thumbnailVideo->SetVideoPosition(*videoTime);
        
    if(videoTime != nullptr)
        delete videoTime;
}


void CViewerWindow::StartAnimation()
{
	animationPosition = 0;
    LoadAnimationBitmap(filename, 0);
	animationTimer->Start(DELAY_ANIMATION, wxTIMER_ONE_SHOT);


}

void CViewerWindow::StopAnimation()
{
	animationPosition = 0;
	if (animationTimer->IsRunning())
		animationTimer->Stop();
}

void CViewerWindow::StopLoadingPicture()
{
	//showBitmapWindow->StopLoadingPicture();
	wxWindow* bitmapWindow = this->FindWindowById(THUMBNAILVIEWERPICTURE);
	if (bitmapWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_ONSTOPLOADINGPICTURE);
		//showBitmapWindow->GetEventHandler()->AddPendingEvent(evt);
		bitmapWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CViewerWindow::LoadAnimationBitmap(const wxString &filename, const int &numFrame)
{
    if(numFrame == oldAnimationPosition && filename == oldFilename)
        return;
        
    oldFilename = filename;
    oldAnimationPosition = numFrame;
    
	printf("CViewerWindow::LoadAnimationBitmap %d \n", numFrame);
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
					previewInfosWnd->SetBitmap(bitmap, false, true);
				}
			}
		}
		else
		{
			image = libPicture.LoadPicture(filename, false, numFrame);
		}
		previewInfosWnd->SetBitmap(image, false, true);
	}
}


bool CViewerWindow::SetAnimation(const wxString &filename)
{
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

	if(iFormat != TIFF && iFormat != PDF)
		libPicture.LoadAllVideoThumbnail(filename, &videoThumbnail, false);
    
	if (previewInfosWnd != nullptr)
	{
		animationPosition = 0;
        
	}  
    LoadAnimationBitmap(filename, 0);
	if(refresh)
		RedrawBarPos();

	CAnimationToolbar* animationToolbar = (CAnimationToolbar*)this->FindWindowById(ANIMATIONTOOLBARWINDOWID);
	if (animationToolbar != nullptr)
		animationToolbar->AnimationStart();
    return result;
}

bool CViewerWindow::SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail)
{
    TRACE();
	StopAnimation();
	bool refresh = isPicture ? false : true;
	bool result = false;
	isAnimation = false;
	isPicture = true;
	isVideo = false;

	if(bitmap != nullptr && bitmap->IsOk())
	{
		panelVideo->HidePanel(false);

		if (previewInfosWnd != nullptr)
			result = previewInfosWnd->SetBitmap(bitmap, isThumbnail, false);

		if(refresh)
			RedrawBarPos();
	}
	return result;
}

void CViewerWindow::FullscreenMode()
{
	isFullscreen = true;
	previewInfosWnd->FullscreenMode();
	RedrawBarPos();
}

void CViewerWindow::ScreenMode()
{
	isFullscreen = false;
	previewInfosWnd->ScreenMode();
	RedrawBarPos();
}



bool CViewerWindow::IsPanelThumbnailVisible()
{
	return panelPicture->IsPanelVisible();
}

bool CViewerWindow::IsPanelInfosVisible()
{
	if (previewInfosWnd != nullptr)
		return previewInfosWnd->IsPanelInfosVisible();

	return false;
}

//////////////////////////////////////////////////////////////////////////
//Picture Loading Function
//////////////////////////////////////////////////////////////////////////
void CViewerWindow::LoadingNewPicture(CThreadPictureData * pictureData)
{
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

void CViewerWindow::OnShowPicture(wxCommandEvent& event)
{
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

////////////////////////////////////////////////////////////////////////////////////////////////
///Mise à jour des informations sur les fichiers
////////////////////////////////////////////////////////////////////////////////////////////////
void CViewerWindow::SetPicture(CImageLoadingFormat * bitmap, const bool &isThumbnail)
{
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


void CViewerWindow::OnLoadPicture(wxCommandEvent& event)
{
	TRACE();
	CPictureElement * pictureElement = (CPictureElement *)event.GetClientData();
	LoadPictureInThread(pictureElement->filename, pictureElement->numElement);
	if (thumbnailPicture != nullptr)
		thumbnailPicture->SetActifItem(pictureElement->numElement, true);
	delete pictureElement;
}

bool CViewerWindow::GetProcessEnd()
{
	TRACE();
	if (processLoadPicture)
		return false;

	return true;
}

void CViewerWindow::LoadPictureInThread(const wxString &filename, const int &numElement, const bool &load)
{
	TRACE();
	if (!wxFileExists(filename))
		this->filename = CLibResource::GetPhotoCancel();
	else
		this->filename = filename;

	CLibPicture libPicture;

	if (libPicture.TestIsVideo(filename))
	{
		//StartLoadingPicture(numElement);
		SetVideo(filename);
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
		else
		{

		}
	}
	else
	{
		StartLoadingPicture(numElement);
		LoadingPicture(filename);
	}
}

void CViewerWindow::LoadingPicture(const wxString &filenameToShow)
{
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

void CViewerWindow::EndPictureThread(wxCommandEvent& event)
{
	TRACE();
	processLoadPicture = false;

	CThreadPictureData * pictureData = (CThreadPictureData *)event.GetClientData();
	if (pictureData != nullptr)
	{
		if (pictureData->myThread != nullptr)
		{
			pictureData->myThread->join();
			delete pictureData->myThread;
		}

		wxString * filename = new wxString();
		*filename = pictureData->picture;
		wxWindow * mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
		wxCommandEvent evt(EVENT_ENDNEWPICTURETHREAD);
		evt.SetClientData(filename);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);

		delete pictureData;
	}
}