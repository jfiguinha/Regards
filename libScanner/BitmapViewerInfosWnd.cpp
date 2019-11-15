#include <header.h>
#include "BitmapViewerInfosWnd.h"
#include <RegardsBitmap.h>
#include <ConfigParam.h>
#include "ScannerParamInit.h"
#include "ScannerParam.h"
#include <LibResource.h>
#include "ParamInit.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
#include "PanelInfosWnd.h"
#include "viewerPDF.h"
#include <ImageLoadingFormat.h>
#include <window_id.h>
#include <Gps.h>
using namespace Regards::Window;
using namespace Regards::Scanner;

#define PANE_INFOS 2
#define PANE_PREVIEW 3

CBitmapViewerInfosWnd::CBitmapViewerInfosWnd(wxWindow* parent, wxWindowID id, CScannerFrame * frame,
	const CThemeSplitter & theme, const bool &horizontal)
	: CSplitter(parent, id, theme)
{
	paneInfos = nullptr;
	panelInfosWindow = nullptr;
	previewWindow = nullptr;
	posBarInfos = 0;
    clickToobarShow = false;
    paneInfosShow = false;
	bool showInfos = true;
	int positionBar = 0;


	CMainTheme * viewerTheme = CMainThemeInit::getInstance();
	CMainParam * viewerParam = CMainParamInit::getInstance();
	if (viewerParam != nullptr)
	{
		viewerParam->GetShowInfos(showInfos);
		viewerParam->GetPositionSplitter(positionBar);
	}
    
	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS",1);
		CThemePane theme;
		viewerTheme->GetPaneTheme(&theme);
		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickToolbarTheme(&themeClickInfosToolbar);
		paneInfos = new CPanelWithClickToolbar(this, "CPictureInfosPanel", PANELCLICKINFOSWNDID, theme, themeClickInfosToolbar, libelle, showInfos, false, true);
		panelInfosWindow = new CPanelInfosWnd(paneInfos->GetPaneWindow(), PANELINFOSWNDSCANNERID);
		paneInfos->SetWindow(panelInfosWindow);
	}

	SetHorizontal(horizontal);
	previewWindow = new CViewerPDF(this, frame, PDFWINDOWID);
	

	
	this->posBar = positionBar;
	this->SetWindow(previewWindow, paneInfos);

	Connect(wxEVENT_RESIZE, wxCommandEventHandler(CBitmapViewerInfosWnd::OnResize));
	

	RedrawBarPos();
}


void CBitmapViewerInfosWnd::OnResize(wxCommandEvent& event)
{
	RedrawBarPos();
}

void CBitmapViewerInfosWnd::UpdateScreenRatio()
{
    paneInfos->UpdateScreenRatio();
    panelInfosWindow->UpdateScreenRatio();
    previewWindow->UpdateScreenRatio();
    
    this->ResizeWindow();
}

CBitmapViewerInfosWnd::~CBitmapViewerInfosWnd()
{
	CMainParam * config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
		bool showInfos = true;

		config->GetShowInfos(showInfos);
		if (showInfos)
			config->SetPositionSplitter(posBar);
	}


	if (panelInfosWindow != nullptr)
		delete(panelInfosWindow);
	

	if (previewWindow != nullptr)
		delete(previewWindow);

	if (paneInfos != nullptr)
		delete(paneInfos);
}

bool CBitmapViewerInfosWnd::IsPanelInfosVisible()
{
	return paneInfos->IsPanelVisible();
}

void CBitmapViewerInfosWnd::ResizeWindow()
{
	posBar = posBar - 1;
	this->Resize(this);
}

void CBitmapViewerInfosWnd::ShowInfos()
{
	if (paneInfos->IsPanelVisible())
	{
		paneInfos->ShowPanel();
	}
}


void CBitmapViewerInfosWnd::RedrawBarPos()
{
    if(paneInfos != nullptr)
    {
		if (!paneInfos->IsPanelVisible())
		{
			posBarInfos = this->posBar;
			SetWindow2FixPosition(true, paneInfos->GetWidth());
			this->SetSeparationBarVisible(false);
		}
		else
		{
			this->posBar = posBarInfos;
			this->SetSeparationBarVisible(true);
			SetWindow2FixPosition(false, posBarInfos);
		}
    }

	
	this->Resize(this);

}

int CBitmapViewerInfosWnd::GetAnimationPosition()
{
	if (previewWindow != nullptr)
		return previewWindow->GetAnimationPosition();

	return -1;
}

wxString CBitmapViewerInfosWnd::SetImage(wxImage imageFile)
{
	if (previewWindow != nullptr)
		return previewWindow->SetImage(imageFile);

	return "";
}

void CBitmapViewerInfosWnd::LoadFile(const wxString &filename)
{
	if (previewWindow != nullptr)
		previewWindow->LoadFile(filename);

	if (panelInfosWindow != nullptr)
		panelInfosWindow->SetFile(filename);
}

CImageLoadingFormat * CBitmapViewerInfosWnd::GetImage()
{
	if (previewWindow != nullptr)
		return previewWindow->GetImage();
	return nullptr;
}

wxString CBitmapViewerInfosWnd::GetFilename()
{
	if (previewWindow != nullptr)
		return previewWindow->GetFilename();
	return "";
}

