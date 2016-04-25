#include "FilterPreviewSplitter.h"
#include <ConfigParam.h>
#include "ViewerParamInit.h"
#include <LibResource.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <Gps.h>
using namespace Regards::Window;
using namespace Regards::Viewer;


wxDEFINE_EVENT(EVENT_HIDDENPANE, wxCommandEvent);

CFilterPreviewSplitter::CFilterPreviewSplitter(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, CVideoEffectParameter * videoEffectParameter, const bool &horizontal)
	: CSplitter(parent, id, theme)
{
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetPreviewInfosSplitterTheme(&theme);
		previewInfosWnd = new CPreviewInfosWnd(this, wxID_ANY, statusBarInterface, theme, videoEffectParameter, false);
	}

	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLCRITERIA", 1);
		CThemePane themePane;
		CThemeScrollBar themeScroll;
		CThemeTree themeTree;

		viewerTheme->GetPaneCategory(themePane);
		paneFilter = new CPane(this, wxID_ANY, this, CATALOG_FILTER, themePane);
		paneFilter->SetTitle(libelle);
		paneFilter->SetClosable(true);

		categoryFolderWnd = new CCategoryFolderWindow(paneFilter, CATEGORYFOLDERWINDOWID, statusBarInterface);
		paneFilter->SetOtherWindow(categoryFolderWnd);
	}


	if (viewerTheme != nullptr)
	{
		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickInfosToolbarTheme(&themeClickInfosToolbar);
		clickInfoToolbar = new CClickToolbar(this, wxID_ANY, themeClickInfosToolbar, this, CATALOG_FILTER);
		clickInfoToolbar->SetVertical(true);
	}

	SetHorizontal(horizontal);

	this->SetWindow(paneFilter, previewInfosWnd);

	bool showFilter = true;
	CConfigParam * configParam = CViewerParamInit::getInstance();
	if (configParam != nullptr)
	{
		viewerconfig = (CViewerParam *)configParam;
		this->posBar = viewerconfig->GetPositionCriteriaPreview();
		viewerconfig->GetShowFilter(showFilter);
	}

	ShowWindow(showFilter);

	Connect(EVENT_HIDDENPANE, wxCommandEventHandler(CFilterPreviewSplitter::OnHidePane));
}

CFilterPreviewSplitter::~CFilterPreviewSplitter()
{
	if (viewerconfig != nullptr)
		viewerconfig->SetPositionCriteriaPreview(this->posBar);

	if (previewInfosWnd != nullptr)
		delete(previewInfosWnd);

	if (categoryFolderWnd != nullptr)
		delete(categoryFolderWnd);
}

wxString CFilterPreviewSplitter::GetSqlRequest()
{
    if (categoryFolderWnd != nullptr)
        return categoryFolderWnd->GetSqlRequest();
    return "";
}

void CFilterPreviewSplitter::InitSaveParameter()
{
    if (categoryFolderWnd != nullptr)
        categoryFolderWnd->InitSaveParameter();
}

void CFilterPreviewSplitter::OnHidePane(wxCommandEvent& event)
{
	this->Resize(this);
}

void CFilterPreviewSplitter::UpdateInfos()
{
    if (previewInfosWnd != nullptr)
        previewInfosWnd->UpdateInfos();
}

void CFilterPreviewSplitter::RefreshFilter()
{
    if (categoryFolderWnd != nullptr)
        categoryFolderWnd->RefreshFilter();
}

void CFilterPreviewSplitter::ShowToolbar()
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->ShowToolbar();
}

void CFilterPreviewSplitter::UpdateCriteria(const int64_t & idFolder)
{
	if (categoryFolderWnd != nullptr)
		categoryFolderWnd->Init(idFolder);
}

void CFilterPreviewSplitter::HideToolbar()
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->HideToolbar();
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

	if (categoryFolderWnd != nullptr)
		categoryFolderWnd->UpdateScreenRatio();
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

void CFilterPreviewSplitter::HidePanelInfos()
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->HidePanelInfos();
}

void CFilterPreviewSplitter::ShowPanelInfos()
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->ShowPanelInfos();
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

bool CFilterPreviewSplitter::SetAnimation(const wxString &filename)
{
	if (previewInfosWnd != nullptr)
		return previewInfosWnd->SetAnimation(filename);
	return false;
}

bool CFilterPreviewSplitter::SetBitmap(CRegardsBitmap * bitmap, const bool &isThumbnail)
{
	if (previewInfosWnd != nullptr)
		return previewInfosWnd->SetBitmap(bitmap, isThumbnail);
	return false;
}

void CFilterPreviewSplitter::FullscreenMode()
{
	if (previewInfosWnd != nullptr)
		previewInfosWnd->FullscreenMode();

	fullscreen = true;
	this->posBarInfos = this->posBar;
	paneFilterShow = paneFilter->IsShown();
	paneFilter->Show(false);
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
	paneFilter->Show(paneFilterShow);

	if (!paneFilterShow)
		ClosePane(CATALOG_FILTER);
	else
	{
		this->SetWindow(paneFilter, previewInfosWnd);
		ShowPanelInfos();
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

void CFilterPreviewSplitter::ShowWindow(const bool & showInfos)
{
	if (showInfos)
	{
		paneFilter->Show(true);
		clickInfoToolbar->Show(false);
		this->SetWindow(paneFilter, previewInfosWnd);
		SetWindow1FixPosition(false, posBarInfos);
		this->SetSeparationBarVisible(true);
		posBar = posBarInfos;
	}
	else
	{
		paneFilter->Show(false);
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


