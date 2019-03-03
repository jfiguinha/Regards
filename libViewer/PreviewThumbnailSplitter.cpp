#include <header.h>
#include "PreviewThumbnailSplitter.h"
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

#define POSITION_RIGHT 1
#define POSITION_BOTTOM 2

//CThemeToolbar themeClickInfosToolbar;
//viewerTheme->GetClickInfosToolbarTheme(&themeClickInfosToolbar);

CPreviewThumbnailSplitter::CPreviewThumbnailSplitter(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, 
	const CThemeSplitter & theme, const CThemeToolbar & themeInfosToolbar, CImageList * imageList, const bool &horizontal)
	: CSplitterWithPanel(parent, id, theme, themeInfosToolbar, horizontal)
{
	posBarInfos = 0;
	previewFaceSplitter = nullptr;
	listPicture = nullptr;
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
		paneWindow1->SetClosable(false);
		paneWindow1->SetTitleBarVisibility(false);
		CThemeSplitter theme;
		viewerTheme->GetPreviewInfosSplitterTheme(&theme);
		previewFaceSplitter = new CPreviewFaceSplitter(paneWindow1, PREVIEWFACESPLITTER, statusBarInterface, theme, themeInfosToolbar);
		paneWindow1->SetOtherWindow(previewFaceSplitter);
	}

	//Criteria
	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLTHUMBNAIL", 1);
		CThemePane themePane;

		viewerTheme->GetPaneCategory(themePane);
		paneWindow2 = new CPane(this, wxID_ANY, this, PANEL2_FILTER, themePane);
		paneWindow2->SetTitle(libelle);
		paneWindow2->SetClosable(true);
		listPicture = new CListPicture(paneWindow2, LISTPICTUREID, statusBarInterface, imageList);
		paneWindow2->SetOtherWindow(listPicture);

	}

	this->SetWindow(paneWindow1, paneWindow2);

	Connect(wxEVENT_SETTHUMBNAILBOTTOM, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPreviewThumbnailSplitter::SetThumbnailBottom));
	Connect(wxEVENT_SETTHUMBNAILRIGHT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPreviewThumbnailSplitter::SetThumbnailRight));
	Connect(wxEVENT_SHOWPANELTHUMBNAIL, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPreviewThumbnailSplitter::ShowPanelThumbnail));
	Connect(wxEVENT_HIDEPANELTHUMBNAIL, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPreviewThumbnailSplitter::HidePanelThumbnail));

    isThumbnailVisible = true;
	//bool showFilter = true;
	CConfigParam * configParam = CViewerParamInit::getInstance();
	if (configParam != nullptr)
	{
		bool isThumbnailVisible = false;
		viewerconfig = (CViewerParam *)configParam;
		this->posBar = viewerconfig->GetPositionPreviewThumbnail();
		isThumbnailBottom = viewerconfig->IsThumbnailBottom();
		if(isThumbnailBottom)
			thumbnailPosition = 2;
		else
			thumbnailPosition = 1;

		viewerconfig->GetShowThumbnail(isThumbnailVisible);
		wxCommandEvent aEvent;
		if (isThumbnailBottom)
			SetThumbnailBottom(aEvent);
		else
			SetThumbnailRight(aEvent);

		if (!isThumbnailVisible)
			this->CloseThumbnailPane();
	}
    
    
}

CPreviewThumbnailSplitter::~CPreviewThumbnailSplitter()
{
	CViewerParam * config = CViewerParamInit::getInstance();
	if (config != nullptr)
	{
		//bool showInfos = true;
		CViewerParam * viewerParam = (CViewerParam *)config;
		viewerParam->SetPositionPreviewThumbnail(posBar);
		if(thumbnailPosition == POSITION_BOTTOM)
			viewerconfig->SetThumbnailBottom(true);
		else
			viewerconfig->SetThumbnailBottom(false);
		viewerconfig->SetShowThumbnail(this->paneWindow2->IsShown());
	}


	if (previewFaceSplitter != nullptr)
		delete(previewFaceSplitter);

	if (listPicture != nullptr)
		delete(listPicture);

}

void CPreviewThumbnailSplitter::UpdateScreenRatio()
{
	if (paneWindow2 != nullptr)
		paneWindow2->UpdateScreenRatio();

	if (paneWindow1 != nullptr)
		paneWindow1->UpdateScreenRatio();
        
    clickToolbarShow = this->clickWindow2Toolbar->IsShown();
    
    if(!clickToolbarShow)
    {
        int iconeHeight = listPicture->GetThumbnailHeight() + paneWindow2->GetTitleHeight();
        this->SetWindow2FixPosition(true, iconeHeight);
        this->FixWindow(2, iconeHeight);        
        this->Resize();       
    }

     /*      
    int iconeHeight = listPicture->GetThumbnailHeight() + paneWindow2->GetTitleHeight();
    this->SetWindow2FixPosition(true, iconeHeight);
    this->FixWindow(2, iconeHeight);
    this->Resize();    */    
}

void CPreviewThumbnailSplitter::ShowThumbnail()
{
    isThumbnailVisible = true;
	wxString libelle = CLibResource::LoadStringFromResource(L"LBLTHUMBNAIL", 1);
	paneWindow2->SetTitle(libelle);

	if(thumbnailPosition == POSITION_BOTTOM)
	{
		if (listPicture != nullptr)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SETTHUMBNAILBOTTOM);
			listPicture->GetEventHandler()->AddPendingEvent(evt);
		}

		isThumbnailBottom = true;
		this->SetHorizontal(true);
		int iconeHeight = listPicture->GetThumbnailHeight() + paneWindow2->GetTitleHeight();
		this->SetWindow2FixPosition(true, iconeHeight);
		this->FixWindow(2, iconeHeight);
		this->Resize();
	}
	else
	{
		if (listPicture != nullptr)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SETTHUMBNAILRIGHT);
			listPicture->GetEventHandler()->AddPendingEvent(evt);
		}

		isThumbnailBottom = false;
		this->SetHorizontal(false);
		this->SetWindow2FixPosition(false, posBarInfos);
		this->FixWindow(0, 0);
		this->Resize();
	}

}

void CPreviewThumbnailSplitter::ShowListFace()
{
	previewFaceSplitter->ShowListFace();
}


void CPreviewThumbnailSplitter::SetThumbnailBottom(wxCommandEvent& aEvent)
{
	thumbnailPosition = 2;

	if (listPicture != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SETTHUMBNAILBOTTOM);
		listPicture->GetEventHandler()->AddPendingEvent(evt);
	}
	isThumbnailBottom = true;
	posBarInfos = this->posBar;
	this->SetHorizontal(true);
	int iconeHeight = listPicture->GetThumbnailHeight() + paneWindow2->GetTitleHeight();
	this->SetWindow2FixPosition(true, iconeHeight);
	this->FixWindow(2, iconeHeight);
	this->Resize();
}

void CPreviewThumbnailSplitter::SetThumbnailRight(wxCommandEvent& aEvent)
{
	thumbnailPosition = 1;

	//listPicture->SetRight();
	if (listPicture != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SETTHUMBNAILRIGHT);
		listPicture->GetEventHandler()->AddPendingEvent(evt);
	}
	isThumbnailBottom = false;
	this->SetHorizontal(false);
	this->SetWindow2FixPosition(false, posBarInfos);
	this->FixWindow(0, 0);
	this->Resize();
}

void CPreviewThumbnailSplitter::SetActifItem(const int &numItem, const bool &move)
{
	if (listPicture != nullptr)
		listPicture->SetActifItem(numItem, move);
}

void CPreviewThumbnailSplitter::SetEffect(const bool &effect)
{
	if (previewFaceSplitter != nullptr)
		previewFaceSplitter->SetEffect(effect);
}

bool CPreviewThumbnailSplitter::IsPanelInfosVisible()
{
	if (previewFaceSplitter != nullptr)
		return previewFaceSplitter->IsPanelInfosVisible();
	return false;
}

void CPreviewThumbnailSplitter::SetVideo(const wxString &path)
{
	if (previewFaceSplitter != nullptr)
		previewFaceSplitter->SetVideo(path);
}


wxString CPreviewThumbnailSplitter::GetSqlRequest()
{
	if (previewFaceSplitter != nullptr)
		return previewFaceSplitter->GetSqlRequest();
	return "";
}


void CPreviewThumbnailSplitter::ShowPanelThumbnail(wxCommandEvent& aEvent)
{
	ClickShowButton(PANEL2_FILTER);
    isThumbnailVisible = true;
	this->Resize();
}

void CPreviewThumbnailSplitter::HidePanel()
{
	CloseThumbnailPane();
    previewFaceSplitter->HidePanel();
	//paneWindow2->Hide();
	this->Resize();
}

void CPreviewThumbnailSplitter::HidePanelThumbnail(wxCommandEvent& aEvent)
{
	CloseThumbnailPane();
	//paneWindow2->Hide();
	this->Resize();
}

bool CPreviewThumbnailSplitter::IsPanelThumbnailVisible()
{
	return paneWindow2->IsShown();
}

void CPreviewThumbnailSplitter::StartLoadingPicture(const int &numElement)
{
	if (previewFaceSplitter != nullptr)
	{
		previewFaceSplitter->StartLoadingPicture(numElement);
	}
	
	if (listPicture != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ONSTARTLOADINGPICTURE);
		evt.SetExtraLong(numElement);
		listPicture->GetEventHandler()->AddPendingEvent(evt);
		//listPicture->StartLoadingPicture(numElement);
	}
	
}

void CPreviewThumbnailSplitter::StopLoadingPicture()
{
	
	if (listPicture != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ONSTOPLOADINGPICTURE);
		listPicture->GetEventHandler()->AddPendingEvent(evt);
		//listPicture->StopLoadingPicture();
	}
	
}

bool CPreviewThumbnailSplitter::SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation)
{
    TRACE();
	if(bitmap != nullptr && bitmap->IsOk())
	{
		if (previewFaceSplitter != nullptr)
			return previewFaceSplitter->SetBitmap(bitmap, isThumbnail, isAnimation);
	}
	return false;
}

void CPreviewThumbnailSplitter::FullscreenMode()
{
	if (previewFaceSplitter != nullptr)
		previewFaceSplitter->FullscreenMode();

	clickToolbarShow = this->clickWindow2Toolbar->IsShown();
	panel2Show = this->paneWindow2->IsShown();
	fullscreen = true;
    if(!isThumbnailBottom)
    {
        wxCommandEvent aEvent;
        SetThumbnailBottom(aEvent);        
    }

    if(!isThumbnailVisible)
        CloseThumbnailPane();
        
	if (!panel2Show)
		ClosePane(PANEL2_FILTER);
        
    this->Resize();
}

void CPreviewThumbnailSplitter::ScreenMode()
{
	this->paneWindow2->Show();

	if (previewFaceSplitter != nullptr)
		previewFaceSplitter->ScreenMode();

	fullscreen = false;

	if (clickToolbarShow)
		CloseThumbnailPane();

	if (panel2Show)
		ClickShowButton(PANEL2_FILTER);

	this->Resize();
}

void CPreviewThumbnailSplitter::CloseThumbnailPane()
{
    ClosePane(PANEL2_FILTER);
    isThumbnailVisible = false;
}

void CPreviewThumbnailSplitter::ShowToolbar()
{
	//if (filterPreviewSplitter != nullptr)
	//	filterPreviewSplitter->ShowToolbar();
	wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
	if (window != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SHOWTOOLBAR);
		window->GetEventHandler()->AddPendingEvent(evt);
	}

	if (fullscreen)
	{
		if(clickToolbarShow)
			this->clickWindow2Toolbar->Show();

		if(panel2Show)
			this->paneWindow2->Show();
	}
	this->Resize();
}

void CPreviewThumbnailSplitter::HideToolbar()
{
	//if (filterPreviewSplitter != nullptr)
	//	filterPreviewSplitter->HideToolbar();
	wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
	if (window != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_HIDETOOLBAR);
		window->GetEventHandler()->AddPendingEvent(evt);
	}

	if (fullscreen)
	{
		clickToolbarShow = this->clickWindow2Toolbar->IsShown();
		panel2Show = this->paneWindow2->IsShown();
		this->paneWindow2->Hide();
		this->clickWindow2Toolbar->Hide();
	}
	this->Resize();
}

void CPreviewThumbnailSplitter::SetDiaporamaMode()
{
	if (previewFaceSplitter != nullptr)
		previewFaceSplitter->SetDiaporamaMode();
}

void CPreviewThumbnailSplitter::SetNormalMode()
{
	if (previewFaceSplitter != nullptr)
		previewFaceSplitter->SetNormalMode();

}