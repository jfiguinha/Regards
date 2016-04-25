#include "PreviewWnd.h"
#include "CentralWnd.h"
#include "PanelInfosWnd.h"
#include "PanelPreview.h"
#include <RegardsBitmap.h>
#include "ExplorerParam.h"
#include <libResource.h>
#include "ExplorerParamInit.h"
#include "ExplorerThemeInit.h"
#include "ExplorerTheme.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Window;
using namespace Regards::Explorer;

#define INFOS_PANE 1
#define PREVIEW_PANE 2
#define CLICK_INFOS_PANEL 3

CPreviewWnd::CPreviewWnd(wxWindow* parent, wxWindowID id, const CThemeSplitter & theme, CVideoEffectParameter * videoEffectParameter, IStatusBarInterface * statusBarInterface, const bool &horizontal)
	: CSplitter(parent, id, theme, horizontal)
{
	
	explorerconfig = CExplorerParamInit::getInstance();

	CExplorerTheme * explorerTheme = CExplorerThemeInit::getInstance();
    
    if (explorerTheme != nullptr)
    {
        CThemeToolbar theme;
        explorerTheme->GetClickPanelInfosToolbarTheme(theme);
        clickPanelInfos = new CClickToolbar(this, wxID_ANY, theme, this, CLICK_INFOS_PANEL);
    }

	if (explorerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLPREVIEW", 1);
		CThemePane themePane;
		explorerTheme->GetPanePreviewTheme(themePane);
		panePreview = new CPane(this, wxID_ANY, this, PREVIEW_PANE, themePane);
		panePreview->SetTitle(libelle);
		panePreview->SetTitleBarVisibility(false);
	}
    
    if (explorerTheme != nullptr)
    {
        wxString libelle = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);
        CThemePane themePane;
        explorerTheme->GetPaneInfosTheme(themePane);
        paneInfos = new CPane(this, wxID_ANY, this, INFOS_PANE, themePane);
        paneInfos->SetTitle(libelle);
    }
    
    wxString urlServer;
    CRegardsConfigParam * param = CParamInit::getInstance();
    if (param != nullptr)
    {
        urlServer = param->GetUrlServer();
    }
  
    fileGeolocalisation = new CFileGeolocation(urlServer);
	
	panelInfos = new CPanelInfosWnd(paneInfos, wxID_ANY,fileGeolocalisation);
	panelPreview = new CPanelPreview(panePreview, wxID_ANY, videoEffectParameter, statusBarInterface, fileGeolocalisation);
	SetHorizontal(true);

	paneInfos->SetOtherWindow(panelInfos);
	panePreview->SetOtherWindow(panelPreview);
    
    //paneInfos->Move(0,0);

	this->SetWindow(panePreview, paneInfos);

	if (explorerconfig != nullptr)
	{
		this->posBar = explorerconfig->GetPositionPreviewInfos();
		bool showInfos = explorerconfig->GetShowInfos();
		if (!showInfos)
			ClosePane(INFOS_PANE);
	}
}


CPreviewWnd::~CPreviewWnd()
{
	if (explorerconfig != nullptr)
		explorerconfig->SetPositionPreviewInfos(this->posBar);

	delete(clickPanelInfos);
	delete(panelInfos);
	delete(panelPreview);
	delete(paneInfos);
	delete(panePreview);
    delete(fileGeolocalisation);
}

void CPreviewWnd::UpdateCriteria()
{
    if(fileGeolocalisation != nullptr)
        fileGeolocalisation->RefreshData();
    
    if(panelInfos != nullptr)
        panelInfos->UpdateData();
    
    if(panelPreview != nullptr)
        panelPreview->UpdateData();
    
    
    panelPreview->Refresh();
}

void CPreviewWnd::UpdateScreenRatio()
{
    paneInfos->UpdateScreenRatio();
    panePreview->UpdateScreenRatio();
    clickPanelInfos->UpdateScreenRatio();
    panelInfos->UpdateScreenRatio();
    panelPreview->UpdateScreenRatio();
    this->Resize();
}

void CPreviewWnd::ShowInfos()
{
	if (!paneInfos->IsShown())
		ClickShowButton(CLICK_INFOS_PANEL);
}

void CPreviewWnd::ClickShowButton(const int &id)
{
	switch (id)
	{
	case CLICK_INFOS_PANEL:
        clickPanelInfos->Hide();
        if (explorerconfig != nullptr)
            explorerconfig->SetShowInfos(true);
		paneInfos->Show(true);
		this->SetFixPosition(false);
		this->SetSeparationBarVisible(true);
		this->SetWindow(panePreview, paneInfos);
		if (oldPos != 0)
			this->SetPosition(oldPos);
		
		break;
	}
}


void CPreviewWnd::HideToolbar()
{
	if (this->filename != L"")
		if (panelPreview != nullptr)
			panelPreview->HideToolbar();
}

void CPreviewWnd::ShowToolbar()
{
	if (this->filename != L"")
		if (panelPreview != nullptr)
			panelPreview->ShowToolbar();
}

void CPreviewWnd::TransitionEnd()
{

}

void CPreviewWnd::ImageSuivante()
{
	CCentralWnd * centralWnd = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (centralWnd != nullptr)
		centralWnd->ImageSuivante();
}

void CPreviewWnd::ImagePrecedente()
{
	CCentralWnd * centralWnd = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (centralWnd != nullptr)
		centralWnd->ImagePrecedente();
}

void CPreviewWnd::ClosePane(const int &id)
{
	switch (id)
	{
		case INFOS_PANE:
		{
			clickPanelInfos->Show(true);
			paneInfos->Show(false);
			oldPos = this->GetPosition();
			//this->SetPosition(20);
			this->SetSeparationBarVisible(false);
			this->SetFixPosition(true);
			this->SetWindow2FixPosition(true, clickPanelInfos->GetNavigatorHeight());

			if (explorerconfig != nullptr)
				explorerconfig->SetShowInfos(false);

			this->SetWindow(panePreview, clickPanelInfos);
			this->Resize(this);
			panePreview->Refresh();
			break;
		}
	}
}

void CPreviewWnd::SetPhoto(const wxString &filename)
{
    if (fileGeolocalisation->GetFilename() != filename)
    {
        fileGeolocalisation->SetFile(filename);
    }
    
	this->filename = filename;
	panelPreview->SetPhoto(filename);
	panelInfos->SetFile(filename);
	
}

void CPreviewWnd::SetAnimation(const wxString &filename)
{
    if (fileGeolocalisation->GetFilename() != filename)
    {
        fileGeolocalisation->SetFile(filename);
    }
    
    this->filename = filename;
    panelPreview->SetAnimation(filename);
    panelInfos->SetFile(filename);
}

void CPreviewWnd::SetVideo(const wxString &filename)
{
    if (fileGeolocalisation->GetFilename() != filename)
    {
        fileGeolocalisation->SetFile(filename);
    }
    
	this->filename = filename;
	panelInfos->SetFile(filename);
	panelPreview->SetVideo(filename);
}

