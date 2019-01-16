#include "PreviewFaceSplitter.h"
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

#define POSITION_RIGHT 1
#define POSITION_BOTTOM 2

//CThemeToolbar themeClickInfosToolbar;
//viewerTheme->GetClickInfosToolbarTheme(&themeClickInfosToolbar);

CPreviewFaceSplitter::CPreviewFaceSplitter(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, 
	const CThemeSplitter & theme, const CThemeToolbar & themeInfosToolbar)
	: CSplitterWithPanel(parent, id, theme, themeInfosToolbar, false)
{
	posBarInfos = 0;
	filterPreviewSplitter = nullptr;
	viewerconfig = nullptr;
	fullscreen = false;
	clickToolbarShow = false;
	panel2Show = false;
	lastWindow = ALL_WINDOW;
	isThumbnailBottom = false;
	thumbnailPosition = 2;
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

	//Folder
	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLPREVIEW", 1);
		CThemePane themePane;

		viewerTheme->GetPaneCategory(themePane);
		paneWindow1 = new CPane(this, wxID_ANY, this, PANEL1_FILTER, themePane);
		paneWindow1->SetTitle(libelle);
		paneWindow1->SetClosable(false);
		paneWindow1->SetTitleBarVisibility(false);
		CThemeSplitter theme;
		viewerTheme->GetPreviewInfosSplitterTheme(&theme);
		filterPreviewSplitter = new CFilterPreviewSplitter(paneWindow1, FILTERPREVIEWSPLITTER, statusBarInterface, theme, false);

		paneWindow1->SetOtherWindow(filterPreviewSplitter);
	}

	//Criteria
	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLFACELIST", 1);
		CThemePane themePane;

		viewerTheme->GetPaneCategory(themePane);
		paneWindow2 = new CPane(this, wxID_ANY, this, PANEL2_FILTER, themePane);
		paneWindow2->SetTitle(libelle);
		paneWindow2->SetClosable(true);

		listFace = new CListFace(paneWindow2, LISTFACEID, statusBarInterface);
		paneWindow2->SetOtherWindow(listFace);

	}

	this->SetWindow(paneWindow1, paneWindow2);

	//bool showFilter = true;
	CConfigParam * configParam = CViewerParamInit::getInstance();
	if (configParam != nullptr)
	{
		bool isFaceVisible = false;
		viewerconfig = (CViewerParam *)configParam;
		this->posBar = viewerconfig->GetPositionPreviewFace();
		viewerconfig->GetShowFace(isFaceVisible);
		if (!isFaceVisible)
			this->ClosePane(PANEL2_FILTER);

		this->FastRefresh(this);
	}
}

CPreviewFaceSplitter::~CPreviewFaceSplitter()
{
	CViewerParam * config = CViewerParamInit::getInstance();
	if (config != nullptr)
	{
		//bool showInfos = true;
		CViewerParam * viewerParam = (CViewerParam *)config;
		viewerParam->SetPositionPreviewFace(posBar);
		viewerconfig->SetShowFace(this->paneWindow2->IsShown());
	}

	if (filterPreviewSplitter != nullptr)
		delete(filterPreviewSplitter);

	if (listFace != nullptr)
		delete(listFace);
}


void CPreviewFaceSplitter::ShowListFace()
{
	ClickShowButton(PANEL2_FILTER);
	this->Resize();
}

void CPreviewFaceSplitter::SetEffect(const bool &effect)
{
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->SetEffect(effect);
}

bool CPreviewFaceSplitter::IsPanelInfosVisible()
{
	if (filterPreviewSplitter != nullptr)
		return filterPreviewSplitter->IsPanelInfosVisible();
	return false;
}

void CPreviewFaceSplitter::SetVideo(const wxString &path)
{
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->SetVideo(path);
}


wxString CPreviewFaceSplitter::GetSqlRequest()
{
	if (filterPreviewSplitter != nullptr)
		return filterPreviewSplitter->GetSqlRequest();
	return "";
}

bool CPreviewFaceSplitter::IsPanelFaceVisible()
{
	return paneWindow2->IsShown();
}

void CPreviewFaceSplitter::HidePanel()
{
    filterPreviewSplitter->HidePanel();
    this->ClosePane(PANEL2_FILTER);
    this->Resize();
}

void CPreviewFaceSplitter::StartLoadingPicture(const int &numElement)
{
	if (filterPreviewSplitter != nullptr)
	{
		//filterPreviewSplitter->StartLoadingPicture();
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ONSTARTLOADINGPICTURE);
		filterPreviewSplitter->GetEventHandler()->AddPendingEvent(evt);
	}
}


bool CPreviewFaceSplitter::SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation)
{
    TRACE();
	if(bitmap != nullptr && bitmap->IsOk())
	{
		if (filterPreviewSplitter != nullptr)
			return filterPreviewSplitter->SetBitmap(bitmap, isThumbnail, isAnimation);
	}
	return false;
}

void CPreviewFaceSplitter::FullscreenMode()
{
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->FullscreenMode();
	
	clickToolbarShow = this->clickWindow2Toolbar->IsShown();
	panel2Show = this->paneWindow2->IsShown();
	fullscreen = true;
	showclickWindow2Toolbar = false;

	clickWindow2Toolbar->Hide();
	ClosePane(PANEL2_FILTER);
}

void CPreviewFaceSplitter::ScreenMode()
{
	showclickWindow2Toolbar = true;

	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->ScreenMode();

	fullscreen = false;


	if (panel2Show)
		ClickShowButton(PANEL2_FILTER);
    else
        ClosePane(PANEL2_FILTER);

	if(clickToolbarShow)
		this->clickWindow2Toolbar->Show();

	this->Resize();
}


void CPreviewFaceSplitter::SetDiaporamaMode()
{
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->SetDiaporamaMode();
}

void CPreviewFaceSplitter::SetNormalMode()
{
	if (filterPreviewSplitter != nullptr)
		filterPreviewSplitter->SetNormalMode();

}