#include <header.h>
#include "CentralWnd.h"
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

using namespace Regards::Window;
using namespace Regards::Viewer;
#define PANE_PICTURETHUMBNAIL 1
#define PANE_VIDEOTHUMBNAIL 2
#define DELAY_ANIMATION 20

CCentralWnd::CCentralWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, CImageList * imageList)
	: wxWindow(parent,id)
{
	previewThumbnailSplitter = nullptr;
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

	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetPreviewInfosSplitterTheme(&theme);
		CThemeToolbar themeToolbar;
		viewerTheme->GetClickThumbnailVideoToolbarTheme(&themeToolbar);
		previewThumbnailSplitter = new CPreviewThumbnailSplitter(this, FILTERPREVIEWTHUMBNAILSPLITTER, statusBarInterface, theme, themeToolbar, imageList, false);
	}

	thumbnailVideoPanel = new CThumbnailVideoPanel(this, THUMBNAILVIDEOPANEL, statusBarInterface);
	Connect(wxTIMER_ANIMATION, wxEVT_TIMER, wxTimerEventHandler(CCentralWnd::OnTimerAnimation), nullptr, this);
    Connect(wxEVT_ANIMATIONTIMERSTART, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CCentralWnd::StartTimerAnimation));
    Connect(wxEVT_ANIMATIONPOSITION, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CCentralWnd::AnimationSetPosition));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CCentralWnd::OnSize));

	animationTimer = new wxTimer(this, wxTIMER_ANIMATION);
}

void CCentralWnd::OnTimerAnimation(wxTimerEvent& event)
{
    printf("CCentralWnd::OnTimerAnimation %d \n", animationPosition);
	SetVideoPosition(animationPosition);
	LoadAnimationBitmap(animationPosition);
	animationPosition++;
	if (animationPosition < listThumbnail.size())
	{
		CImageVideoThumbnail * thumbnail = listThumbnail[animationPosition];
		if (thumbnail != nullptr)
		{
			//if (thumbnail->delay <= 0)
			thumbnail->delay = DELAY_ANIMATION;
                
            printf("CCentralWnd::OnTimerAnimation Start delay %d \n", thumbnail->delay);
			animationTimer->Start(thumbnail->delay, wxTIMER_ONE_SHOT);
		}
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

void CCentralWnd::AnimationPictureNext()
{
	animationPosition++;
	if (animationPosition >= listThumbnail.size())
		animationPosition = listThumbnail.size() - 1;
	SetVideoPosition(animationPosition);
	LoadAnimationBitmap(animationPosition);
}

void CCentralWnd::AnimationPicturePrevious()
{
	animationPosition--;
	if (animationPosition < 0)
		animationPosition = 0;
	SetVideoPosition(animationPosition);
	LoadAnimationBitmap(animationPosition);
}

void CCentralWnd::ShowFilter()
{
	if (previewThumbnailSplitter != nullptr)
		previewThumbnailSplitter->ClickShowButton(CATALOG_FILTER);
}


CCentralWnd::~CCentralWnd()
{
	if (listThumbnail.size() > 0)
	{
		for (int j = 0; j < listThumbnail.size(); j++)
		{
			CImageVideoThumbnail * thumbnail = listThumbnail[j];
			if (thumbnail != nullptr)
				delete thumbnail;
		}
	}

	if (animationTimer->IsRunning())
		animationTimer->Stop();

	delete(animationTimer);

	delete(thumbnailVideoPanel);
	delete(previewThumbnailSplitter);
}


void CCentralWnd::ShowListFace()
{
	if (previewThumbnailSplitter != nullptr)
		previewThumbnailSplitter->ShowListFace();

	this->RedrawBarPos();
}

void CCentralWnd::SetDiaporamaMode()
{
	isDiaporama = true;
	if (previewThumbnailSplitter != nullptr)
		previewThumbnailSplitter->SetDiaporamaMode();

	this->RedrawBarPos();
}

void CCentralWnd::SetNormalMode()
{
	isDiaporama = false;
	if (previewThumbnailSplitter != nullptr)
		previewThumbnailSplitter->SetNormalMode();
	this->RedrawBarPos();
}

wxString CCentralWnd::GetSqlRequest()
{
    if (previewThumbnailSplitter != nullptr)
        return previewThumbnailSplitter->GetSqlRequest();
    return "";
}


void CCentralWnd::HideToolbar()
{
    showToolbar = false;
	if (previewThumbnailSplitter != nullptr)
	{
		if (isFullscreen)
		{
			thumbnailVideoPanel->HideVideoThumbnail(false);
		}
		previewThumbnailSplitter->HideToolbar();
		this->RedrawBarPos();
	}
}

void CCentralWnd::ShowToolbar()
{
    showToolbar = true;
	if (previewThumbnailSplitter != nullptr)
	{
		if (isFullscreen && !isPicture)
		{
			thumbnailVideoPanel->ShowVideoThumbnail();
		}



		previewThumbnailSplitter->ShowToolbar();
		this->RedrawBarPos();
	}
}

 void CCentralWnd::HidePanel()
 {
    if (thumbnailVideoPanel != nullptr)
        thumbnailVideoPanel->HideVideoThumbnail();

	if (previewThumbnailSplitter != nullptr)
		previewThumbnailSplitter->HidePanel();

	this->RedrawBarPos();    
 }

void CCentralWnd::ShowPanelVideoThumbnail()
{
	if(isVideo)
	{
		if (thumbnailVideoPanel != nullptr)
		{
			thumbnailVideoPanel->ShowVideoThumbnail();
		}
	}


	if (previewThumbnailSplitter != nullptr)
		previewThumbnailSplitter->ShowThumbnail();

	this->RedrawBarPos();
}

void CCentralWnd::SetVideoPosition(const int64_t &videoTime)
{
    int64_t * data = new int64_t();
    *data = videoTime;
    wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVT_ANIMATIONPOSITION);
    evt.SetClientData(data);
    this->GetEventHandler()->AddPendingEvent(evt);       
    /*
	if (thumbnailVideoPanel != nullptr)
		thumbnailVideoPanel->SetVideoPosition(videoTime);
    */

}

void CCentralWnd::SetPosition(const long &timePosition)
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
		LoadAnimationBitmap(timePosition);
	}
}

void CCentralWnd::UpdateScreenRatio()
{
    printf("CCentralWnd::UpdateScreenRatio() \n");
    thumbnailVideoPanel->UpdateScreenRatio();
	previewThumbnailSplitter->UpdateScreenRatio();  
    this->Resize();
}

void CCentralWnd::SetEffect(const bool &effect)
{

	if (previewThumbnailSplitter != nullptr)
		previewThumbnailSplitter->SetEffect(effect);
}


void CCentralWnd::Resize()
{
	RedrawBarPos();
}

wxRect CCentralWnd::GetWindowRect()
{
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = width;
	rc.height = height;
	return rc;
}


void CCentralWnd::RedrawBarPos()
{
    if(isPicture)
    {
        thumbnailVideoPanel->HideVideoThumbnail(false);
    }
    
    /*
	if (isDiaporama)
	{
		thumbnailVideoPanel->Show(false);

		if (previewThumbnailSplitter != nullptr)
			previewThumbnailSplitter->SetSize(0, 0, width, height);
	}
	else
	{
     */
		//wxRect rc = GetWindowRect();
		int bottomHeight = 0;
		int topHeight = 0;
		if(!isPicture && !isFullscreen && !thumbnailVideoPanel->IsShown())
		{
			thumbnailVideoPanel->Show();
			//if(!thumbnailVideoPanel->IsPanelVideoThumbnailVisible())
			thumbnailVideoPanel->ShowVideoThumbnail();
		}

		if (!isPicture && thumbnailVideoPanel->IsShown())
		{
			int iconeHeight = thumbnailVideoPanel->GetHeight();
			thumbnailVideoPanel->SetSize(0, 0, width, iconeHeight);
   			topHeight += iconeHeight;
		}
        
        if(!showToolbar && isFullscreen)
        {
            thumbnailVideoPanel->HideVideoThumbnail(false);
            bottomHeight = 0;
            topHeight = 0;             
        }
             
		if (previewThumbnailSplitter != nullptr)
		{
			previewThumbnailSplitter->SetSize(0, topHeight, width, height - (topHeight + bottomHeight));
			previewThumbnailSplitter->SendSizeEvent();
		}
        
        //if(thumbnailVideoPanel->IsShown())
         //   thumbnailVideoPanel->Refresh();
	//}
    
    printf("CCentralWnd::RedrawBarPos() \n");
}

void CCentralWnd::SetNumElement(const int &numElement, const bool &move)
{
	if (previewThumbnailSplitter != nullptr)
		previewThumbnailSplitter->SetActifItem(numElement, move);
}

void CCentralWnd::OnSize(wxSizeEvent& event)
{
    
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	RedrawBarPos();
}

void CCentralWnd::SetVideo(const wxString &path)
{
	StopAnimation();
	bool refresh = isVideo ? false : true;
	filename = path;
	isVideo = true;
	isAnimation = false;
	isPicture = false;
    thumbnailVideoPanel->SetVideo(path);
    //thumbnailVideoPanel->ShowVideoThumbnail();
    

    if (previewThumbnailSplitter != nullptr)
        previewThumbnailSplitter->SetVideo(path);
	if(refresh)
		this->RedrawBarPos();
}

void CCentralWnd::StartLoadingPicture(const int &numElement)
{
	previewThumbnailSplitter->StartLoadingPicture(numElement);

}

void CCentralWnd::AnimationSetPosition(wxCommandEvent& event)
{
    int64_t * videoTime = (int64_t *)event.GetClientData();
	if (thumbnailVideoPanel != nullptr && videoTime != nullptr)
		thumbnailVideoPanel->SetVideoPosition(*videoTime);
        
    if(videoTime != nullptr)
        delete videoTime;
}


void CCentralWnd::StartAnimation()
{
	animationPosition = 0;
    LoadAnimationBitmap(0);
	animationTimer->Start(DELAY_ANIMATION, wxTIMER_ONE_SHOT);
}

void CCentralWnd::StopAnimation()
{
	animationPosition = 0;
	if (animationTimer->IsRunning())
		animationTimer->Stop();
}

void CCentralWnd::StopLoadingPicture()
{
	previewThumbnailSplitter->StopLoadingPicture();
}

void CCentralWnd::LoadAnimationBitmap(const int &numFrame)
{
	printf("CCentralWnd::LoadAnimationBitmap %d \n", numFrame);
	if (numFrame < listThumbnail.size() && numFrame >= 0)
	{
		CImageVideoThumbnail * thumbnail = listThumbnail.at(numFrame);
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
				previewThumbnailSplitter->SetBitmap(bitmap, false, true);
			}

		}
	}
   // RedrawBarPos();
}

bool CCentralWnd::SetAnimation(const wxString &filename)
{
	StopAnimation();
	bool refresh = isAnimation ? false : true;
    bool result = false;      
    isVideo = false;
	isAnimation = true;
	isPicture = false;

	if (listThumbnail.size() > 0)
	{
		for (int j = 0; j < listThumbnail.size(); j++)
		{
			CImageVideoThumbnail * thumbnail = listThumbnail[j];
			if (thumbnail != nullptr)
				delete thumbnail;
		}
	}


	CLibPicture libPicture;
	listThumbnail = libPicture.LoadAllVideoThumbnail(filename);
	thumbnailVideoPanel->SetAnimation(filename, &listThumbnail);
    
	if (previewThumbnailSplitter != nullptr)
	{
		animationPosition = 0;
        
	}  
    LoadAnimationBitmap(0);
	if(refresh)
		RedrawBarPos();

    //animationTimer->Start(2000, wxTIMER_ONE_SHOT);
    return result;
}


void CCentralWnd::StartTimerAnimation(wxCommandEvent& event)
{
     StartAnimation();
}

bool CCentralWnd::SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail)
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
		thumbnailVideoPanel->HideVideoThumbnail(false);

		if (previewThumbnailSplitter != nullptr)
			result = previewThumbnailSplitter->SetBitmap(bitmap, isThumbnail, false);

		if(refresh)
			RedrawBarPos();
	}
	return result;
}

void CCentralWnd::FullscreenMode()
{
	isFullscreen = true;
	previewThumbnailSplitter->FullscreenMode();
	RedrawBarPos();
}

void CCentralWnd::ScreenMode()
{
	isFullscreen = false;
	previewThumbnailSplitter->ScreenMode();
	RedrawBarPos();
}


bool CCentralWnd::IsPanelThumbnailVisible()
{

	return previewThumbnailSplitter->IsPanelThumbnailVisible();

}

bool CCentralWnd::IsPanelInfosVisible()
{

	if (previewThumbnailSplitter != nullptr)
		return previewThumbnailSplitter->IsPanelInfosVisible();

	return false;
}