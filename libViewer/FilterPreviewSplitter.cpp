#include <header.h>
#include "FilterPreviewSplitter.h"
#include <ConfigParam.h>
#include "ViewerParamInit.h"
#include <LibResource.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#include <ImageLoadingFormat.h>
#include <window_id.h>
#include <Gps.h>
using namespace Regards::Window;
using namespace Regards::Viewer;

#define PANE_FOLDER 2
wxDEFINE_EVENT(EVENT_HIDDENPANE, wxCommandEvent);

CFilterPreviewSplitter::CFilterPreviewSplitter(wxWindow* parent, wxWindowID id, 
	IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, const bool &horizontal)
	: CSplitter(parent, id, theme)
{
	posBarInfos = 0;
	panePhoto = nullptr;
	clickInfoToolbar = nullptr;
	panelPhotoWnd = nullptr;
	previewInfosWnd = nullptr;
	viewerconfig = nullptr;

	fullscreen = false;
	paneFilterShow = true;
	clickToobarShow = false;

	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

    
	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLSELECTFILE",1);
		CThemePane theme;
		viewerTheme->GetInfosPaneTheme(&theme);
		panePhoto = new CPane(this, wxID_ANY, this, PANE_FOLDER, theme);
		panePhoto->SetTitle(libelle);
	}

	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetPreviewInfosSplitterTheme(&theme);

		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickInfosToolbarTheme(&themeClickInfosToolbar);
		//previewInfosWnd = new CPreviewThumbnailFolderSplitter(this, wxID_ANY, statusBarInterface, theme, themeClickInfosToolbar, videoEffectParameter, false);
		previewInfosWnd = new CPreviewInfosWnd(this, PREVIEWINFOWND, statusBarInterface, theme, false);
	}

	if (viewerTheme != nullptr)
	{
		panelPhotoWnd = new CPanelPhotoWnd(panePhoto, CRITERIAFOLDERWINDOWID, statusBarInterface);
        panePhoto->SetOtherWindow(panelPhotoWnd);
	}


	if (viewerTheme != nullptr)
	{
		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickInfosToolbarTheme(&themeClickInfosToolbar);
		clickInfoToolbar = new CClickToolbar(this, wxID_ANY, themeClickInfosToolbar, this, CATALOG_FILTER);
		clickInfoToolbar->SetVertical(true);
	}

	SetHorizontal(horizontal);

	this->SetWindow(panePhoto, previewInfosWnd);

	bool showFolder = true;
	bool showFilter = true;
	CConfigParam * configParam = CViewerParamInit::getInstance();
	if (configParam != nullptr)
	{
		viewerconfig = (CViewerParam *)configParam;
		viewerconfig->GetShowFilter(showFilter);
		viewerconfig->GetShowFolder(showFolder);

	}

	if (showFolder || showFilter)
		ShowWindow(showFilter);
	else
		ClosePane(CATALOG_FILTER);

	//Connect(EVENT_HIDDENPANE, wxCommandEventHandler(CFilterPreviewSplitter::OnHidePane));
	Connect(EVENT_HIDDENPANE, wxCommandEventHandler(CFilterPreviewSplitter::OnHidePane));
	Connect(wxEVENT_ALLPANECLOSED, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CFilterPreviewSplitter::CloseWindow));
	Connect(wxEVENT_PANELFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CFilterPreviewSplitter::ShowPanelPhotoList));

	if (configParam != nullptr)
	{
		viewerconfig = (CViewerParam *)configParam;
		this->posBar = viewerconfig->GetPositionCriteriaPreview();
        this->FastRefresh(this);
    }
}

CFilterPreviewSplitter::~CFilterPreviewSplitter()
{
	if (viewerconfig != nullptr)
		viewerconfig->SetPositionCriteriaPreview(this->posBar);

	if (previewInfosWnd != nullptr)
		delete(previewInfosWnd);

    if(panePhoto != nullptr)
        delete(panePhoto);

	if (panelPhotoWnd != nullptr)
		delete(panelPhotoWnd);
}

wxString CFilterPreviewSplitter::GetSqlRequest()
{
	if (panelPhotoWnd != nullptr)
		return panelPhotoWnd->GetSqlRequest();
	return "";
}

void CFilterPreviewSplitter::CloseWindow(wxCommandEvent& event)
{
	ClosePane(CATALOG_FILTER);
}

void CFilterPreviewSplitter::OnHidePane(wxCommandEvent& event)
{
	this->Resize(this);
}

void CFilterPreviewSplitter::ShowPanelPhotoList(wxCommandEvent& aEvent)
{
	if (panePhoto != nullptr)
	{
		if (!panePhoto->IsShown())
			ClickShowButton(CATALOG_FILTER);
	}
}

void CFilterPreviewSplitter::SetDiaporamaMode()
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->SetDiaporamaMode();
}

void CFilterPreviewSplitter::SetNormalMode()
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->SetNormalMode();

}

void CFilterPreviewSplitter::UpdateScreenRatio()
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->UpdateScreenRatio();

	if (panePhoto != nullptr)
		panePhoto->UpdateScreenRatio();
}

void CFilterPreviewSplitter::SetEffect(const bool &effect)
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->SetEffect(effect);
}

bool CFilterPreviewSplitter::IsPanelInfosVisible()
{
	if (previewInfosWnd != nullptr)
		return previewInfosWnd->IsPanelInfosVisible();
	return false;
}

void CFilterPreviewSplitter::SetVideo(const wxString &path)
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->SetVideo(path);
}

void CFilterPreviewSplitter::StartLoadingPicture()
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->StartLoadingPicture();
}

bool CFilterPreviewSplitter::SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation)
{
    TRACE();
	if(bitmap != nullptr && bitmap->IsOk())
	{
		if (previewInfosWnd != nullptr)
			return previewInfosWnd->SetBitmap(bitmap, isThumbnail, isAnimation);
	}
	return false;
}

void CFilterPreviewSplitter::FullscreenMode()
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->FullscreenMode();

	fullscreen = true;
	this->posBarInfos = this->posBar;
	paneFilterShow = panePhoto->IsShown();
	panePhoto->Show(false);
	clickToobarShow = clickInfoToolbar->IsShown();

	clickInfoToolbar->Show(false);
	this->SetWindow(nullptr, previewInfosWnd);
	this->Resize(this);
}

void CFilterPreviewSplitter::ScreenMode()
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->ScreenMode();

	fullscreen = false;
	
	if (!paneFilterShow)
    {
        clickInfoToolbar->Show(true);
        ClosePane(CATALOG_FILTER);
    }
	else
	{
        panePhoto->Show(true);
		this->SetWindow(panePhoto, previewInfosWnd);
		//ShowPanelInfos();
		wxWindow * window = this->FindWindowById(PREVIEWINFOWND);
		if (window)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SHOWPANELINFO);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
		this->SetSeparationBarVisible(true);
		this->Resize(this);
	}
}

void CFilterPreviewSplitter::ClickShowButton(const int &id)
{
	switch (id)
	{
	case CATALOG_FILTER:
	{
		ShowWindow(true);


		CViewerParam * config = CViewerParamInit::getInstance();
		if (config != nullptr)
		{
			CViewerParam * viewerParam = (CViewerParam *)config;
			viewerParam->SetShowFilter(true);
		}


	}
	break;
	}
	this->Resize(this);
}

void CFilterPreviewSplitter::ClosePane(const int &id)
{
	switch (id)
	{
	case CATALOG_FILTER:
	{
		ShowWindow(false);

		CViewerParam * config = CViewerParamInit::getInstance();
		if (config != nullptr)
		{
			CViewerParam * viewerParam = (CViewerParam *)config;
			viewerParam->SetShowFilter(false);
		}
	}
	break;
	}
	this->Resize(this);
}

void CFilterPreviewSplitter::HidePanel()
{
    ClosePane(CATALOG_FILTER);
    previewInfosWnd->HidePanel();
}

void CFilterPreviewSplitter::ShowWindow(const bool & showInfos)
{
	if (showInfos)
	{
		panePhoto->Show(true);
		clickInfoToolbar->Show(false);
		this->SetWindow(panePhoto, previewInfosWnd);
		SetWindow1FixPosition(false, posBarInfos);
		this->SetSeparationBarVisible(true);
		posBar = posBarInfos;
	}
	else
	{
		panePhoto->Show(false);
		clickInfoToolbar->Show(true);
		this->SetWindow(clickInfoToolbar, previewInfosWnd);
		posBarInfos = posBar;
		int taille = clickInfoToolbar->GetNavigatorWidth();
		posBar = taille;
		SetWindow1FixPosition(true, clickInfoToolbar->GetNavigatorWidth());
		this->SetSeparationBarVisible(false);
	}
	viewerconfig->SetShowFilter(showInfos);
}

