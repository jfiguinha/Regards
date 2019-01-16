#include "FilterPreviewSplitter.h"
#include <ConfigParam.h>
#include "ViewerParamInit.h"
#include <LibResource.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#include <ImageLoadingFormat.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <Gps.h>
using namespace Regards::Window;
using namespace Regards::Viewer;


wxDEFINE_EVENT(EVENT_HIDDENPANE, wxCommandEvent);

CFilterPreviewSplitter::CFilterPreviewSplitter(wxWindow* parent, wxWindowID id, 
	IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, const bool &horizontal)
	: CSplitter(parent, id, theme)
{
	posBarInfos = 0;
	paneFilter = nullptr;
	clickInfoToolbar = nullptr;
	criteriaFolderWnd = nullptr;
	previewInfosWnd = nullptr;
	viewerconfig = nullptr;

	fullscreen = false;
	paneFilterShow = true;
	clickToobarShow = false;

	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

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
		CThemeSplitter theme;
		viewerTheme->GetPreviewInfosSplitterTheme(&theme);

		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickInfosToolbarTheme(&themeClickInfosToolbar);

		criteriaFolderWnd = new CCriteriaFolderSplitter(this, CRITERIAFOLDERWINDOWID, statusBarInterface, theme, themeClickInfosToolbar, false);
	}


	if (viewerTheme != nullptr)
	{
		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickInfosToolbarTheme(&themeClickInfosToolbar);
		clickInfoToolbar = new CClickToolbar(this, wxID_ANY, themeClickInfosToolbar, this, CATALOG_FILTER);
		clickInfoToolbar->SetVertical(true);
	}

	SetHorizontal(horizontal);

	this->SetWindow(criteriaFolderWnd, previewInfosWnd);

	bool showFolder = true;
	bool showFilter = true;
	CConfigParam * configParam = CViewerParamInit::getInstance();
	if (configParam != nullptr)
	{
		viewerconfig = (CViewerParam *)configParam;
		this->posBar = viewerconfig->GetPositionCriteriaPreview();
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
	Connect(wxEVENT_PANELCRITERIA, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CFilterPreviewSplitter::ShowPanelCriteria));
	Connect(wxEVENT_PANELFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CFilterPreviewSplitter::ShowPanelFolder));

}

CFilterPreviewSplitter::~CFilterPreviewSplitter()
{
	if (viewerconfig != nullptr)
		viewerconfig->SetPositionCriteriaPreview(this->posBar);

	if (previewInfosWnd != nullptr)
		delete(previewInfosWnd);

	if (criteriaFolderWnd != nullptr)
		delete(criteriaFolderWnd);
}

wxString CFilterPreviewSplitter::GetSqlRequest()
{
	if (criteriaFolderWnd != nullptr)
		return criteriaFolderWnd->GetSqlRequest();
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

void CFilterPreviewSplitter::ShowPanelCriteria(wxCommandEvent& aEvent)
{
	if (criteriaFolderWnd != nullptr)
	{
		if (!criteriaFolderWnd->IsShown())
			ClickShowButton(CATALOG_FILTER);

		wxWindow * window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
		if (window)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_PANELCRITERIA);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

	}
}

void CFilterPreviewSplitter::ShowPanelFolder(wxCommandEvent& aEvent)
{
	if (criteriaFolderWnd != nullptr)
	{
		if (!criteriaFolderWnd->IsShown())
			ClickShowButton(CATALOG_FILTER);

		//criteriaFolderWnd->ShowPanelFolder();
		wxWindow * window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
		if (window)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_PANELFOLDER);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
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

	if (criteriaFolderWnd != nullptr)
		criteriaFolderWnd->UpdateScreenRatio();
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
	paneFilterShow = criteriaFolderWnd->IsShown();
	criteriaFolderWnd->Show(false);
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
	
	if(criteriaFolderWnd != nullptr)
	{
		criteriaFolderWnd->Show(paneFilterShow);
		criteriaFolderWnd->ScreenMode();
	}

	if (!paneFilterShow)
		ClosePane(CATALOG_FILTER);
	else
	{
		this->SetWindow(criteriaFolderWnd, previewInfosWnd);
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
    criteriaFolderWnd->HidePanel();
}

void CFilterPreviewSplitter::ShowWindow(const bool & showInfos)
{
	if (showInfos)
	{
		criteriaFolderWnd->Show(true);
		criteriaFolderWnd->ShowWindow();
		clickInfoToolbar->Show(false);
		this->SetWindow(criteriaFolderWnd, previewInfosWnd);
		SetWindow1FixPosition(false, posBarInfos);
		this->SetSeparationBarVisible(true);
		posBar = posBarInfos;
	}
	else
	{
		criteriaFolderWnd->Show(false);
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

