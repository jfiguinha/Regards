#include "CentralWnd.h"
#include "ThumbnailViewerVideo.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <ImageLoadingFormat.h>
#include <FileUtility.h>
#include <LibResource.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Window;
using namespace Regards::Viewer;
#define PANE_PICTURETHUMBNAIL 1
#define PANE_VIDEOTHUMBNAIL 2

CCentralWnd::CCentralWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, PhotosVector & photoList)
	: wxWindow(parent,id)
{
	previewThumbnailSplitter = nullptr;
	viewerParam = nullptr;
	isVideo = false;
	isFullscreen = false;
	filename = L"";
	width = 0;
	height = 0;
	isDiaporama = false;
    showToolbar = false;
	checkValidity = false;


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
		previewThumbnailSplitter = new CPreviewThumbnailSplitter(this, FILTERPREVIEWTHUMBNAILSPLITTER, statusBarInterface, theme, themeToolbar, photoList, false);
	}

	thumbnailVideoPanel = new CThumbnailVideoPanel(this, THUMBNAILVIDEOPANEL, statusBarInterface);
	
	Connect(wxEVT_SIZE, wxSizeEventHandler(CCentralWnd::OnSize));
}

void CCentralWnd::ShowFilter()
{
	if (previewThumbnailSplitter != nullptr)
		previewThumbnailSplitter->ClickShowButton(CATALOG_FILTER);
}

CCentralWnd::~CCentralWnd()
{
	delete(thumbnailVideoPanel);
	delete(previewThumbnailSplitter);
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
		if (isFullscreen)
		{
			thumbnailVideoPanel->ShowVideoThumbnail(false);
		}
		previewThumbnailSplitter->ShowToolbar();
		this->RedrawBarPos();
	}
}

void CCentralWnd::ShowPanelVideoThumbnail()
{
	if(isVideo)
	{
		if (thumbnailVideoPanel != nullptr)
		{
			thumbnailVideoPanel->ShowVideoThumbnail(false);
		}
	}
	this->RedrawBarPos();
}

void CCentralWnd::SetVideoPosition(const int64_t &videoTime)
{
	if (thumbnailVideoPanel != nullptr)
		thumbnailVideoPanel->SetVideoPosition(videoTime);
}

void CCentralWnd::UpdateScreenRatio()
{
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
	if (isDiaporama)
	{
		thumbnailVideoPanel->Show(false);

		if (previewThumbnailSplitter != nullptr)
			previewThumbnailSplitter->SetSize(0, 0, width, height);
	}
	else
	{
		//wxRect rc = GetWindowRect();
		int bottomHeight = 0;
		int topHeight = 0;

		if (thumbnailVideoPanel->IsShown())
		{
			int iconeHeight = thumbnailVideoPanel->GetHeight();
			thumbnailVideoPanel->SetSize(0, 0, width, iconeHeight);
			topHeight += iconeHeight;
		}
		if (previewThumbnailSplitter != nullptr)
			previewThumbnailSplitter->SetSize(0, topHeight, width, height - (topHeight + bottomHeight));
	}
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
	filename = path;
	isVideo = true;

	if(!isDiaporama && thumbnailVideoPanel != nullptr)
	{
		thumbnailVideoPanel->ShowVideoThumbnail(false);
		thumbnailVideoPanel->SetVideo(path);
	}
	else if(isDiaporama)
	{
		thumbnailVideoPanel->HideVideoThumbnail(false);
	}

	if (previewThumbnailSplitter != nullptr)
		previewThumbnailSplitter->SetVideo(path);

	this->RedrawBarPos();
}

void CCentralWnd::StartLoadingPicture(const int &numElement)
{
	previewThumbnailSplitter->StartLoadingPicture(numElement);

}

void CCentralWnd::StopLoadingPicture()
{
	previewThumbnailSplitter->StopLoadingPicture();
}

bool CCentralWnd::SetAnimation(const wxString &filename)
{
    bool result = false;      
    isVideo = false;
	thumbnailVideoPanel->HideVideoThumbnail(false);

	if (previewThumbnailSplitter != nullptr)
		result = previewThumbnailSplitter->SetAnimation(filename);

    RedrawBarPos();
    return result;
}

bool CCentralWnd::SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail)
{
	bool result = false;	
	isVideo = false;
	thumbnailVideoPanel->HideVideoThumbnail(false);

	if (previewThumbnailSplitter != nullptr)
		result = previewThumbnailSplitter->SetBitmap(bitmap, isThumbnail);
	RedrawBarPos();
	
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