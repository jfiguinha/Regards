#include "PanelInfosWnd.h"
#include <Gps.h>
#include "ExplorerTheme.h"
#include "ExplorerThemeInit.h"
#include "ExplorerParam.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <FileUtility.h>
#include "PreviewWnd.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <effect.h>
using namespace Regards::Window;
using namespace Regards::Explorer;

wxDEFINE_EVENT(EVENT_UPDATEINFOSTHREAD, wxCommandEvent);

CPanelInfosWnd::CPanelInfosWnd(wxWindow* parent, wxWindowID id, CFileGeolocation * fileGeolocalisation)
	: CWindowMain(parent, id)
{
	CExplorerTheme * explorerTheme = CExplorerThemeInit::getInstance();
    windowVisible = WM_INFOS;
    this->fileGeolocalisation = fileGeolocalisation;

    
    if (explorerTheme != nullptr)
        explorerTheme->GetBitmapWindowTheme(&themeBitmap);

	webBrowser = wxWebView::New(this, wxID_ANY, url);
    webBrowser->Show(false);
    
    if (explorerTheme != nullptr)
    {
        CThemeToolbar theme;
        explorerTheme->GetInfosToolbarTheme(theme);
        infosToolbar = new CInfosToolbar(this, wxID_ANY, theme, this);
    }
    
    if (explorerTheme != nullptr)
    {
        CThemeScrollBar themeScroll;
        explorerTheme->GetScrollInfosFileTheme(themeScroll);

        CThemeTree theme;
        explorerTheme->GetTreeInfosFileTheme(theme);
        
        infosFileWnd = new CInfosFileWnd(this, wxID_ANY, themeScroll, theme);
    }

    
    if (explorerTheme != nullptr)
    {
        CThemeScrollBar themeScroll;
        explorerTheme->GetScrollInfosFileTheme(themeScroll);
        
        CThemeTree themeTree;
        explorerTheme->GetTreeInfosFileTheme(themeTree);
        
        criteriaTreeWnd = new CCriteriaTreeWnd(this, wxID_ANY, MAINEXPLORERWINDOWID, fileGeolocalisation, themeTree, themeScroll);
    }

    
    Connect(wxEVT_PAINT, wxPaintEventHandler(CPanelInfosWnd::OnPaint));
    Connect(wxEVT_STOPANIMATION, wxCommandEventHandler(CPanelInfosWnd::StopLoadingPicture));
    Connect(wxEVT_STARTANIMATION, wxCommandEventHandler(CPanelInfosWnd::StartLoadingPicture));
    
    //Connect(EVENT_UPDATEINFOSTHREAD, wxCommandEventHandler(CPanelInfosWnd::UpdateTreeInfosEvent));
    
    m_animationCtrl = new wxAnimationCtrl(this, wxID_ANY);
    m_animationCtrl->Show(false);
    
    wxString resourcePath = CFileUtility::GetResourcesFolderPath();
    m_animationCtrl->LoadFile(resourcePath + "/loading.gif");
}


CPanelInfosWnd::~CPanelInfosWnd()
{
    delete(m_animationCtrl);
    delete(infosFileWnd);
    delete(criteriaTreeWnd);
	delete(infosToolbar);
	delete(webBrowser);
}

void CPanelInfosWnd::UpdateData()
{
    if (!fileGeolocalisation->HasGps())
    {
        if (webBrowser->IsShown())
            if (!fileGeolocalisation->HasGps())
                infosToolbar->SetInfosActif();
        
        infosToolbar->SetMapInactif();
    }
    else
        infosToolbar->SetMapActif();
    
    LoadInfo();
}

void CPanelInfosWnd::SetFile(const wxString &filename)
{
    this->filename = filename;

    if (!fileGeolocalisation->HasGps())
    {
        if (webBrowser->IsShown())
            if (!fileGeolocalisation->HasGps())
                infosToolbar->SetInfosActif();

        infosToolbar->SetMapInactif();
    }
    else
        infosToolbar->SetMapActif();

    
    LoadInfo();

}

void CPanelInfosWnd::DisplayURL(const wxString &url)
{
	webBrowser->LoadURL(url);
	infosToolbar->SetMapActif();
}

void CPanelInfosWnd::StartLoadingPicture(wxCommandEvent &event)
{
    wxWindow * window = (wxWindow *)event.GetClientData();
    if(window != nullptr)
        window->Show(false);
    m_animationCtrl->Show(true);
    m_animationCtrl->Play();
	m_animationCtrl->SetBackgroundColour(themeBitmap.colorScreen);
    this->Resize();
    
}

void CPanelInfosWnd::StopLoadingPicture(wxCommandEvent &event)
{
    wxWindow * window = (wxWindow *)event.GetClientData();
    m_animationCtrl->Stop();
    m_animationCtrl->Show(false);
    if(window != nullptr)
        window->Show(true);
    this->Resize();
    
}

void CPanelInfosWnd::LoadInfo()
{
	if (this->IsShown())
	{
		switch (windowVisible)
		{
		case WM_INFOS:
			InfosUpdate();
            infosToolbar->SetInfosPush();
			break;
        case WM_CRITERIA:
            criteriaTreeWnd->SetFile(filename);
            infosToolbar->SetCriteriaPush();
            break;
		case WM_MAPS:
            {
                infosToolbar->SetMapPush();
                wxString newUrl = MapsUpdate();
                if(newUrl != url)
                    DisplayURL(newUrl);
                
                url = newUrl;
            }
			break;
		}
	}
}

void CPanelInfosWnd::InfosUpdate()
{
    infosFileWnd->InfosUpdate(filename);
}

wxString CPanelInfosWnd::MapsUpdate()
{
	wxString url = L"http://www.openstreetmap.org/?mlat=";
	url.append(fileGeolocalisation->GetLatitude());
	url.append(L"&mlon=");
	url.append(fileGeolocalisation->GetLongitude());
	url.append(L"#map=15/");
	url.append(fileGeolocalisation->GetLatitude());
	url.append(L"/");
	url.append(fileGeolocalisation->GetLongitude());
    return url;
}

void CPanelInfosWnd::UpdateScreenRatio()
{
    infosFileWnd->UpdateScreenRatio();
    infosToolbar->UpdateScreenRatio();
    criteriaTreeWnd->UpdateScreenRatio();
    this->Resize();

}

void CPanelInfosWnd::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    wxRect rc = GetRect();
    this->FillRect(&dc, rc, themeBitmap.colorScreen);
    //if(m_animationCtrl->IsShown())
     //   m_animationCtrl->SetBackgroundColour(themeBitmap.colorScreen);
}



void CPanelInfosWnd::ClickShowButton(const int &id)
{
	switch (id)
	{
	case WM_INFOS:
		ShowInfos();
        infosToolbar->SetInfosPush();
		break;
    case WM_CRITERIA:
        ShowCriteria();
        infosToolbar->SetCriteriaPush();
        break;
	case WM_MAPS:
		ShowMap();
        infosToolbar->SetMapPush();
		break;
	}
}

void CPanelInfosWnd::ShowCriteria()
{
    if (!infosToolbar->IsShown())
        infosToolbar->Show(true);
    
    if (webBrowser->IsShown())
        webBrowser->Show(false);
    
    if (infosFileWnd->IsShown())
        infosFileWnd->Show(false);
    
    if (!criteriaTreeWnd->IsShown())
        criteriaTreeWnd->Show(true);
    
    criteriaTreeWnd->SetFile(filename);
    
    windowVisible = WM_CRITERIA;
    
    Resize();
}

void CPanelInfosWnd::ShowInfos()
{
	if (!infosToolbar->IsShown())
		infosToolbar->Show(true);

	if (webBrowser->IsShown())
		webBrowser->Show(false);

	if (!infosFileWnd->IsShown())
		infosFileWnd->Show(true);
    
    if (criteriaTreeWnd->IsShown())
        criteriaTreeWnd->Show(false);
    
     windowVisible = WM_INFOS;
    
    InfosUpdate();
	Resize();
}

void CPanelInfosWnd::ShowMap()
{
	if (!infosToolbar->IsShown())
		infosToolbar->Show(true);

	if (!webBrowser->IsShown())
		webBrowser->Show(true);

	if (infosFileWnd->IsShown())
		infosFileWnd->Show(false);
    
    if (criteriaTreeWnd->IsShown())
        criteriaTreeWnd->Show(false);
    
    wxString newUrl = MapsUpdate();
    if(newUrl != url)
        DisplayURL(newUrl);
    
    url = newUrl;
    
     windowVisible = WM_MAPS;
    
	Resize();
}

void CPanelInfosWnd::Resize()
{
	wxRect rcAffichageBitmap;

	rcAffichageBitmap.x = 0;
	rcAffichageBitmap.y = 0;
	rcAffichageBitmap.width = width;

	if (infosToolbar->IsShown())
	{
		rcAffichageBitmap.height = height - infosToolbar->GetHeight();
		infosToolbar->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, infosToolbar->GetHeight());
	}
	else
	{
		rcAffichageBitmap.height = height;
	}

    if(m_animationCtrl->IsShown())
    {
        wxAnimation animation = m_animationCtrl->GetAnimation();
        wxSize animationSize = animation.GetSize();
        
        int xPos = rcAffichageBitmap.x + (width - animationSize.GetWidth()) / 2;
        int yPos = infosToolbar->GetHeight() + ((height - infosToolbar->GetHeight()) - animationSize.GetHeight()) / 2;
        
        m_animationCtrl->SetSize(xPos, yPos, animationSize.GetWidth(), animationSize.GetHeight());
    }
	else if (infosFileWnd->IsShown())
		infosFileWnd->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
	else if (webBrowser->IsShown())
		webBrowser->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
    else if (criteriaTreeWnd->IsShown())
        criteriaTreeWnd->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
}
