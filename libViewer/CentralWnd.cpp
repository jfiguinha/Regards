#include "CentralWnd.h"
#include "ThumbnailBitmap.h"
#include "ThumbnailViewerVideo.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <FileUtility.h>
#include <ShowVideo.h>
#include <libResource.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
using namespace Regards::Window;
using namespace Regards::Viewer;
#define PANE_PICTURETHUMBNAIL 1
#define PANE_VIDEOTHUMBNAIL 2

CCentralWnd::CCentralWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, CVideoEffectParameter * videoEffectParameter)
	: wxWindow(parent,id)
{

	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();
	
	//----------------------------------------------------------------------------------------
	//Panel Thumbnail Picture
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLTHUMBNAIL",1);
		CThemePane theme;
		viewerTheme->GetThumbnailPaneTheme(&theme);
		paneThumbnail = new CPane(this, wxID_ANY, this, PANE_PICTURETHUMBNAIL, theme);
		paneThumbnail->SetTitle(libelle);
	}

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetClickThumbnailToolbarTheme(&theme);
		clickThumbnail = new CClickToolbar(this, wxID_ANY, theme, this, PANE_PICTURETHUMBNAIL);
	}
	
	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollThumbnailTheme(&themeScroll);
		scrollThumbnail = new CScrollbarWnd(paneThumbnail, wxID_ANY);

		CThemeThumbnail theme;
		viewerTheme->GetThumbnailTheme(&theme);
		thumbnail = new CThumbnailBitmap(scrollThumbnail, wxID_ANY, statusBarInterface, theme);

		scrollThumbnail->SetCentralWindow(thumbnail, themeScroll);
		scrollThumbnail->HideVerticalScroll();
		scrollThumbnail->SetPageSize(1000);
		scrollThumbnail->SetLineSize(200);

		paneThumbnail->SetOtherWindow(scrollThumbnail);
	}

	//----------------------------------------------------------------------------------------
	//Panel Thumbnail Video
	//----------------------------------------------------------------------------------------

	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLTHUMBNAILVIDEO", 1);
		CThemePane theme;
		viewerTheme->GetThumbnailVideoPaneTheme(&theme);
		paneThumbnailVideo = new CPane(this, wxID_ANY, this, PANE_VIDEOTHUMBNAIL, theme);
		paneThumbnailVideo->SetTitle(libelle);
	}
		
	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetClickThumbnailVideoToolbarTheme(&theme);
		clickThumbnailVideo = new CClickToolbar(this, wxID_ANY, theme, this, PANE_VIDEOTHUMBNAIL);
	}
	
	
	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollThumbnailVideoTheme(&themeScroll);
		scrollThumbnailVideo = new CScrollbarWnd(paneThumbnailVideo, wxID_ANY);

		CThemeThumbnail theme;
		viewerTheme->GetThumbnailVideoTheme(&theme);
		thumbnailVideo = new CThumbnailViewerVideo(scrollThumbnailVideo, wxID_ANY, statusBarInterface, theme);

		scrollThumbnailVideo->SetCentralWindow(thumbnailVideo, themeScroll);
		scrollThumbnailVideo->HideVerticalScroll();
		scrollThumbnailVideo->SetPageSize(1000);
		scrollThumbnailVideo->SetLineSize(200);

		paneThumbnailVideo->SetOtherWindow(scrollThumbnailVideo);

		
	}

#ifdef VIEWER
	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetPreviewInfosSplitterTheme(&theme);
		filterPreviewSplitter = new CFilterPreviewSplitter(this, wxID_ANY, statusBarInterface, theme, videoEffectParameter, false);
	}
#else
	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetPreviewInfosSplitterTheme(&theme);
		previewInfosWnd = new CPreviewInfosWnd(this, wxID_ANY, statusBarInterface, theme, videoEffectParameter, false);
	}
#endif

	Connect(wxEVT_SIZE, wxSizeEventHandler(CCentralWnd::OnSize));

	clickThumbnail->Show(false);
	paneThumbnail->Show(true);

	clickThumbnailVideo->Show(true);
	paneThumbnailVideo->Show(false);
}

#ifdef VIEWER
void CCentralWnd::ShowFilter()
{
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->ClickShowButton(CATALOG_FILTER);
}
#endif

CCentralWnd::~CCentralWnd()
{
	delete(thumbnailVideo);
	delete(thumbnail);
	delete(scrollThumbnail);
	delete(paneThumbnail);
	delete(clickThumbnail);
	delete(scrollThumbnailVideo);
	delete(paneThumbnailVideo);
#ifdef VIEWER
	delete(filterPreviewSplitter);
#else
	delete(previewInfosWnd);
#endif
}

void CCentralWnd::SetDiaporamaMode()
{
	isDiaporama = true;
#ifdef VIEWER
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->SetDiaporamaMode();
#else
	previewInfosWnd->SetDiaporamaMode();
#endif
	this->RedrawBarPos();
}

void CCentralWnd::SetNormalMode()
{
	isDiaporama = false;
#ifdef VIEWER
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->SetNormalMode();
#else
	previewInfosWnd->SetNormalMode();
#endif
	this->RedrawBarPos();
}

#ifdef VIEWER
void CCentralWnd::UpdateCriteria(const int64_t & idFolder)
{
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->UpdateCriteria(idFolder);
}

wxString CCentralWnd::GetSqlRequest()
{
    if (filterPreviewSplitter != nullptr)
        return filterPreviewSplitter->GetSqlRequest();
    return "";
}

void CCentralWnd::InitSaveParameter()
{
    if (filterPreviewSplitter != nullptr)
        filterPreviewSplitter->InitSaveParameter();
}

void CCentralWnd::UpdateCriteria()
{
    if (filterPreviewSplitter != nullptr)
        filterPreviewSplitter->UpdateInfos();
}

void CCentralWnd::RefreshFilter()
{
    if (filterPreviewSplitter != nullptr)
        filterPreviewSplitter->RefreshFilter();
}

#endif

void CCentralWnd::HideToolbar()
{
    showToolbar = false;
#ifdef VIEWER
	if (filterPreviewSplitter != nullptr)
#else
	if (previewInfosWnd != nullptr)
#endif
	{
		if (isFullscreen)
		{
			if (clickThumbnail->IsShown())
				clickThumbnail->Show(false);
			if (clickThumbnailVideo->IsShown())
				clickThumbnailVideo->Show(false);
		}
#ifdef VIEWER
		filterPreviewSplitter->HideToolbar();
#else
		previewInfosWnd->HideToolbar();
#endif
		this->RedrawBarPos();
	}
}

void CCentralWnd::ShowToolbar()
{
    showToolbar = true;
#ifdef VIEWER
	if (filterPreviewSplitter != nullptr)
#else
	if (previewInfosWnd != nullptr)
#endif
	{
		if (isFullscreen)
		{
			if (!paneThumbnail->IsShown())
			{
				if (!clickThumbnail->IsShown())
					clickThumbnail->Show(true);
				if (!clickThumbnailVideo->IsShown())
					clickThumbnailVideo->Show(true);
			}
		}
#ifdef VIEWER
		filterPreviewSplitter->ShowToolbar();
#else
		previewInfosWnd->ShowToolbar();
#endif
		this->RedrawBarPos();
	}
}

void CCentralWnd::RefreshThumbnail()
{
    if(thumbnail != nullptr)
    {
        thumbnail->EraseThumbnail();
    }
}

void CCentralWnd::SetVideoPosition(const int64_t &videoTime)
{
	if (thumbnailVideo != nullptr)
		thumbnailVideo->SetVideoPosition(videoTime);
}

void CCentralWnd::UpdateScreenRatio()
{
    paneThumbnail->UpdateScreenRatio();
    scrollThumbnail->UpdateScreenRatio();
    thumbnail->UpdateScreenRatio();
    clickThumbnail->UpdateScreenRatio();
    
    paneThumbnailVideo->UpdateScreenRatio();
    scrollThumbnailVideo->UpdateScreenRatio();
    thumbnailVideo->UpdateScreenRatio();
    clickThumbnailVideo->UpdateScreenRatio();
    
#ifdef VIEWER
	filterPreviewSplitter->UpdateScreenRatio();
#else
	previewInfosWnd->UpdateScreenRatio();
#endif
    
    this->Resize();
}

void CCentralWnd::SetEffect(const bool &effect)
{
#ifdef VIEWER
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->SetEffect(effect);
#else
	previewInfosWnd->SetEffect(effect);
#endif
}

void CCentralWnd::ClickShowButton(const int &id)
{
	switch (id)
	{
	case PANE_PICTURETHUMBNAIL:
		{
			clickThumbnail->Show(false);
			paneThumbnail->Show(true);
			CViewerParam * config = CViewerParamInit::getInstance();
			if (config != nullptr)
			{
				CViewerParam * viewerParam = (CViewerParam *)config;
				viewerParam->SetShowThumbnail(true);
			}
		}
		break;

	case PANE_VIDEOTHUMBNAIL:
		{
			CViewerParam * config = CViewerParamInit::getInstance();
			if (config != nullptr)
			{
				CViewerParam * viewerParam = (CViewerParam *)config;
				viewerParam->SetShowVideoThumbnail(true);
			}

			clickThumbnailVideo->Show(false);
			paneThumbnailVideo->Show(true);
			ShowVideoThumbnail();
		}
		break;
	}

	
	RedrawBarPos();
}



void CCentralWnd::SetListeFile(const vector<wxString> listFichier)
{
	if (thumbnail != nullptr)
		thumbnail->SetListeFile(listFichier);
	RedrawBarPos();
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

void CCentralWnd::ShowVideoThumbnail()
{
	bool show = false;
	CViewerParam * config = CViewerParamInit::getInstance();
	if (config != nullptr)
	{
		//wxRect rc = GetWindowRect();
		CViewerParam * viewerParam = (CViewerParam *)config;
		viewerParam->GetShowThumbnail(show);
	}

	if (thumbnailVideo->GetFilename() != filename)
	{
		thumbnailVideo->SetFilename(filename);
		thumbnailVideo->SetVideoFile(filename);
	}
}

void CCentralWnd::RedrawBarPos()
{
	if (isDiaporama)
	{
		paneThumbnailVideo->SetSize(0, 0, 0, 0);
		clickThumbnailVideo->SetSize(0, 0, 0, 0);
		paneThumbnail->SetSize(0, 0, 0, 0);
		clickThumbnail->SetSize(0, 0, 0, 0);

#ifdef VIEWER
		if (filterPreviewSplitter != nullptr)
			filterPreviewSplitter->SetSize(0, 0, width, height);
#else
		if (previewInfosWnd != nullptr)
			previewInfosWnd->SetSize(0, 0, width, height);
#endif
	}
	else
	{
		//wxRect rc = GetWindowRect();
		int bottomHeight = 0;
		int topHeight = 0;

		if (paneThumbnailVideo != nullptr)
		{
			if (paneThumbnailVideo->IsShown())
			{
				int iconeHeight = thumbnailVideo->GetIconeHeight() + scrollThumbnailVideo->GetBarHeight() + paneThumbnailVideo->GetTitleHeight();
				paneThumbnailVideo->SetSize(0, 0, width, iconeHeight);
				topHeight += iconeHeight;
			}
			else if (isVideo)
			{
				if (isFullscreen && clickThumbnailVideo->IsShown())
				{
					//int iconeHeight = clickThumbnailVideo->GetHeight();
					clickThumbnailVideo->SetSize(0, 0, width, clickThumbnailVideo->GetHeight());
					topHeight += clickThumbnailVideo->GetHeight();
				}
				else if(showToolbar)
				{
					if (!clickThumbnailVideo->IsShown())
						clickThumbnailVideo->Show(true);

					//int iconeHeight = clickThumbnailVideo->GetHeight();
					clickThumbnailVideo->SetSize(0, 0, width, clickThumbnailVideo->GetHeight());
					topHeight += clickThumbnailVideo->GetHeight();
				}
                else if(clickThumbnailVideo->IsShown())
                {
                    //int iconeHeight = clickThumbnailVideo->GetHeight();
                    clickThumbnailVideo->SetSize(0, 0, width, clickThumbnailVideo->GetHeight());
                    topHeight += clickThumbnailVideo->GetHeight();
                }
                else
                {
                    paneThumbnailVideo->Show(true);
                    int iconeHeight = thumbnailVideo->GetIconeHeight() + scrollThumbnailVideo->GetBarHeight() + paneThumbnailVideo->GetTitleHeight();
                    paneThumbnailVideo->SetSize(0, 0, width, iconeHeight);
                    topHeight += iconeHeight;
                }
			}
		}

		if (paneThumbnail != nullptr)
		{
			if (paneThumbnail->IsShown())
			{
				int iconeHeight = thumbnail->GetIconeHeight() + scrollThumbnail->GetBarHeight() + paneThumbnail->GetTitleHeight();
				paneThumbnail->SetSize(0, height - iconeHeight, width, iconeHeight);
				bottomHeight += iconeHeight;
			}
			else
			{
				if (!isFullscreen)
				{
					if (!clickThumbnail->IsShown())
						clickThumbnail->Show(true);
					int iconeHeight = clickThumbnail->GetHeight();
					clickThumbnail->SetSize(0, height - iconeHeight, width, iconeHeight);
					bottomHeight += iconeHeight;
				}
				else if (showToolbar)
				{
					int iconeHeight = clickThumbnail->GetHeight();
					clickThumbnail->SetSize(0, height - iconeHeight, width, iconeHeight);
					bottomHeight += iconeHeight;
				}
			}
		}
#ifdef VIEWER
		if (filterPreviewSplitter != nullptr)
			filterPreviewSplitter->SetSize(0, topHeight, width, height - (topHeight + bottomHeight));
#else
		if (previewInfosWnd != nullptr)
			previewInfosWnd->SetSize(0, topHeight, width, height - (topHeight + bottomHeight));
#endif
	}
}

void CCentralWnd::SetNumElement(const int &numElement)
{
	if (!isDiaporama)
		thumbnail->SetActifItem(numElement);
}


void CCentralWnd::ShowPanelVideoThumbnail()
{
	if (isVideo)
	{
		if (!paneThumbnailVideo->IsShown())
		{
			paneThumbnailVideo->Show(true);
			ShowVideoThumbnail();
			RedrawBarPos();
		}
	}
}

void CCentralWnd::HidePanelVideoThumbnail()
{
	if (isVideo)
	{
		if (paneThumbnailVideo->IsShown())
		{
			paneThumbnailVideo->Show(false);
			RedrawBarPos();
		}
	}
}

bool CCentralWnd::IsPanelVideoThumbnailVisible()
{
	return paneThumbnailVideo->IsShown();
}

void CCentralWnd::ShowPanelThumbnail()
{
	if (!paneThumbnail->IsShown())
	{
		paneThumbnail->Show(true);
		scrollThumbnail->Show(true);
		RedrawBarPos();
	}
}

void CCentralWnd::HidePanelThumbnail()
{
	if (paneThumbnail->IsShown())
	{
		paneThumbnail->Show(false);
		RedrawBarPos();
	}
}

bool CCentralWnd::IsPanelThumbnailVisible()
{
	return paneThumbnail->IsShown();
}

bool CCentralWnd::IsPanelInfosVisible()
{
#ifdef VIEWER
	if (filterPreviewSplitter != nullptr)
		return filterPreviewSplitter->IsPanelInfosVisible();
#else
	if (previewInfosWnd != nullptr)
		return previewInfosWnd->IsPanelInfosVisible();
#endif
	return false;
}

void CCentralWnd::HidePanelInfos()
{
#ifdef VIEWER
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->HidePanelInfos();
#else
	if (previewInfosWnd != nullptr)
		previewInfosWnd->HidePanelInfos();
#endif
}

void CCentralWnd::ShowPanelInfos()
{
#ifdef VIEWER
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->ShowPanelInfos();
#else
	if (previewInfosWnd != nullptr)
		previewInfosWnd->ShowPanelInfos();
#endif
}


void CCentralWnd::OnSize(wxSizeEvent& event)
{
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	RedrawBarPos();
}

void CCentralWnd::ClosePane(const int &id)
{
	switch (id)
	{
	case PANE_PICTURETHUMBNAIL:
		{
			if (viewerParam != nullptr)
				viewerParam->SetShowThumbnail(false);

			paneThumbnail->Show(false);
			clickThumbnail->Show(true);
		}
		break;

	case PANE_VIDEOTHUMBNAIL:
		{
			if (viewerParam != nullptr)
				viewerParam->SetShowVideoThumbnail(false);

			paneThumbnailVideo->Show(false);
			clickThumbnailVideo->Show(true);
		}
		break;
	}

	RedrawBarPos();
}



void CCentralWnd::SetVideo(const wxString &path)
{
	filename = path;
	isVideo = true;
	if (!clickThumbnailVideo->IsShown())
	{
		if (!paneThumbnailVideo->IsShown())
		{
			paneThumbnailVideo->Show(true);
		}
	}

#ifdef VIEWER
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->SetVideo(path);
#else
	if (previewInfosWnd != nullptr)
		previewInfosWnd->SetVideo(path);
#endif
	ShowVideoThumbnail();
	this->RedrawBarPos();
}

void CCentralWnd::StartLoadingPicture(const int &numElement)
{
#ifdef VIEWER
	filterPreviewSplitter->StartLoadingPicture();
#else
	previewInfosWnd->StartLoadingPicture();
#endif
	thumbnail->StartLoadingPicture(numElement);
}

void CCentralWnd::StopLoadingPicture()
{
    thumbnail->StopLoadingPicture();

}

bool CCentralWnd::SetAnimation(const wxString &filename)
{
    bool result = false;
    if (isDiaporama)
    {
#ifdef VIEWER
		if (filterPreviewSplitter != nullptr)
			result = filterPreviewSplitter->SetAnimation(filename);
#else
		if (previewInfosWnd != nullptr)
			result = previewInfosWnd->SetAnimation(filename);
#endif
    }
    else
    {
        
        isVideo = false;
        if (paneThumbnailVideo != nullptr)
        {
            if (paneThumbnailVideo->IsShown())
                paneThumbnailVideo->Show(false);
        }
#ifdef VIEWER
		if (filterPreviewSplitter != nullptr)
			result = filterPreviewSplitter->SetAnimation(filename);
#else
		if (previewInfosWnd != nullptr)
			result = previewInfosWnd->SetAnimation(filename);
#endif
        RedrawBarPos();
    }
    return result;
}

bool CCentralWnd::SetBitmap(CRegardsBitmap * bitmap, const bool &isThumbnail)
{
	bool result = false;
	if (isDiaporama)
	{
#ifdef VIEWER
		if (filterPreviewSplitter != nullptr)
			result = filterPreviewSplitter->SetBitmap(bitmap, isThumbnail);
#else
		if (previewInfosWnd != nullptr)
			result = previewInfosWnd->SetBitmap(bitmap, isThumbnail);
#endif
	}
	else
	{
		
		isVideo = false;
		if (paneThumbnailVideo != nullptr)
		{
			if (paneThumbnailVideo->IsShown())
				paneThumbnailVideo->Show(false);
		}
#ifdef VIEWER
		if (filterPreviewSplitter != nullptr)
			result = filterPreviewSplitter->SetBitmap(bitmap, isThumbnail);
#else
		if (previewInfosWnd != nullptr)
			result = previewInfosWnd->SetBitmap(bitmap, isThumbnail);
#endif
		RedrawBarPos();
	}
	return result;
}

void CCentralWnd::FullscreenMode()
{
	isFullscreen = true;
#ifdef VIEWER
	filterPreviewSplitter->FullscreenMode();
#else
	previewInfosWnd->FullscreenMode();
#endif
	RedrawBarPos();
}

void CCentralWnd::ScreenMode()
{
	isFullscreen = false;
#ifdef VIEWER
	filterPreviewSplitter->ScreenMode();
#else
	previewInfosWnd->ScreenMode();
#endif
	RedrawBarPos();
}
