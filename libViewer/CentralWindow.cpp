#include <header.h>
#include "CentralWindow.h"
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

CCentralWindow::CCentralWindow(wxWindow* parent, wxWindowID id, 
	IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, CImageList * imageList, const bool &horizontal)
	: CSplitter(parent, id, theme)
{
	posBarInfos = 0;
	panelSearch = nullptr;
	panelPhotoWnd = nullptr;
	viewerWindow = nullptr;
	viewerconfig = nullptr;
	fullscreen = false;

	CViewerParam* config = CViewerParamInit::getInstance();
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

	if (imageList != nullptr)
	{
		photoVector = imageList->GetCopy();
	}

	if (viewerTheme != nullptr)
	{
		viewerWindow = new CViewerWindow(this, VIEWERPICTUREWND, statusBarInterface);
		viewerWindow->SetListeFile(&photoVector);
	}

	if (viewerTheme != nullptr)
	{
		listPicture = new CListPicture(this, LISTPICTUREID, statusBarInterface);
		listPicture->Show(false);
		listPicture->SetListeFile(&photoVector);
	}

	if (viewerTheme != nullptr)
	{
		listFace = new CListFace(this, LISTFACEID, statusBarInterface);
		listFace->Show(false);
	}

	if (viewerTheme != nullptr)
	{
		bool isPanelVisible = true;
		if(viewerconfig != nullptr)
			viewerconfig->GetShowFilter(isPanelVisible);

		wxString libelle = CLibResource::LoadStringFromResource(L"LBLSELECTFILE", 1);
		CThemePane theme;
		viewerTheme->GetInfosPaneTheme(&theme);

		CThemeToolbar themetoolbar;
		viewerTheme->GetClickInfosToolbarTheme(&themetoolbar);
		panelSearch = new CPanelWithClickToolbar(this, "PanelPhotoSearch", PHOTOSEEARCHPANEL, theme, themetoolbar, libelle, isPanelVisible);
		panelPhotoWnd = new CPanelPhotoWnd(panelSearch->GetPaneWindow(), CRITERIAFOLDERWINDOWID, statusBarInterface);
		panelSearch->SetWindow(panelPhotoWnd);
	}



	SetHorizontal(horizontal);

	this->SetWindow(panelSearch, viewerWindow);

	Connect(wxEVENT_SETLISTPICTURE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CCentralWindow::SetListeFile));
	Connect(wxEVENT_CHANGETYPEAFFICHAGE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CCentralWindow::ChangeTypeAffichage));
	Connect(wxEVENT_RESIZE, wxCommandEventHandler(CCentralWindow::OnResize));

	if (config != nullptr)
	{
		viewerconfig = (CViewerParam *)config;
		this->posBar = viewerconfig->GetPositionCriteriaPreview();
        this->FastRefresh(this);
    }

	windowMode = 1;
}


void CCentralWindow::OnResize(wxCommandEvent& event)
{
	RedrawBarPos();
}


void CCentralWindow::ChangeTypeAffichage(wxCommandEvent& event)
{
	if (listPicture != nullptr)
	{
		long typeAffichage = event.GetExtraLong();
		listPicture->ChangeTypeAffichage(&photoVector, typeAffichage);
	}
		
}

void CCentralWindow::ShowFile()
{
	if (!panelSearch->IsPanelVisible())
	{
		this->posBar = posBarInfos;
		this->SetSeparationBarVisible(true);
		SetWindow1FixPosition(false, posBarInfos);
	}
}

void CCentralWindow::RedrawBarPos()
{
	if (panelSearch != nullptr)
	{
		if (!panelSearch->IsPanelVisible())
		{
			posBarInfos = this->posBar;
			SetWindow1FixPosition(true, panelSearch->GetWidth());
			this->SetSeparationBarVisible(false);
		}
		else
		{
			this->posBar = posBarInfos;
			this->SetSeparationBarVisible(true);
			SetWindow1FixPosition(false, posBarInfos);
		}
	}

	this->Resize(this);

}

void CCentralWindow::SetListeFile(wxCommandEvent& event)
{
	CImageList * picture = (CImageList *)event.GetClientData();
	if (picture != nullptr)
	{
		this->photoVector.clear();
		photoVector = picture->GetCopy();
	}

	if (listPicture != nullptr)
		listPicture->SetListeFile(&photoVector);

	if(viewerWindow != nullptr)
		viewerWindow->SetListeFile(&photoVector);
}

CCentralWindow::~CCentralWindow()
{
	if (viewerconfig != nullptr)
		viewerconfig->SetPositionCriteriaPreview(this->posBar);

	if (viewerWindow != nullptr)
		delete(viewerWindow);

    if(panelPhotoWnd != nullptr)
        delete(panelPhotoWnd);

	if (panelSearch != nullptr)
		delete(panelSearch);

	if (listPicture != nullptr)
		delete(listPicture);

	if (listFace != nullptr)
		delete(listFace);
}

void CCentralWindow::SetMode(const int &mode)
{
	windowMode = mode;
	viewerWindow->Show(false);
	listFace->Show(false);
	listPicture->Show(false);

	switch (mode)
	{
	case 1:
		viewerWindow->Show(true);
		this->SetWindow(panelSearch, viewerWindow);
		break;
	case 2:
		listFace->Show(true);
		this->SetWindow(panelSearch, listFace);
		break;
	case 3:
		listPicture->Show(true);
		this->SetWindow(panelSearch, listPicture);
		break;
	}
	this->Resize(this);
}

wxString CCentralWindow::GetSqlRequest()
{
	if (panelPhotoWnd != nullptr)
		return panelPhotoWnd->GetSqlRequest();
	return "";
}

void CCentralWindow::UpdateScreenRatio()
{
	if (viewerWindow != nullptr)
		viewerWindow->UpdateScreenRatio();

	if (panelSearch != nullptr)
		panelSearch->UpdateScreenRatio();
}

void CCentralWindow::FullscreenMode()
{
	if (viewerWindow != nullptr)
		viewerWindow->FullscreenMode();

	fullscreen = true;
	this->posBarInfos = this->posBar;
	panelSearch->Show(false);
	this->SetWindow(nullptr, viewerWindow);
	this->Resize(this);
}

void CCentralWindow::ScreenMode()
{
	if (viewerWindow != nullptr)
		viewerWindow->ScreenMode();

	fullscreen = false;

    panelSearch->Show(true);
	this->SetWindow(panelSearch, viewerWindow);
	//ShowPanelInfos();
	wxWindow * window = this->FindWindowById(VIEWERPICTUREWND);
	if (window)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SHOWPANELINFO);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
	this->SetSeparationBarVisible(true);
	this->Resize(this);
}
