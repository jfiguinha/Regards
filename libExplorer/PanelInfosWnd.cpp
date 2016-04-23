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
	CThemeScrollBar themeScroll;
	CExplorerTheme * explorerTheme = CExplorerThemeInit::getInstance();
    windowVisible = WM_INFOS;
    this->fileGeolocalisation = fileGeolocalisation;

	if (explorerTheme != nullptr)
	{
		
		explorerTheme->GetScrollInfosFileTheme(themeScroll);
		InfosFileScroll = new CScrollbarWnd(this, wxID_ANY);
	}

	if (explorerTheme != nullptr)
	{
		CThemeTree theme;
		explorerTheme->GetTreeInfosFileTheme(theme);
		treeWindow = new CTreeWindow(InfosFileScroll, wxID_ANY, theme);
		InfosFileScroll->SetCentralWindow(treeWindow, themeScroll);
	}
    
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
    
    Connect(wxEVT_PAINT, wxPaintEventHandler(CPanelInfosWnd::OnPaint));
    Connect(EVENT_UPDATEINFOSTHREAD, wxCommandEventHandler(CPanelInfosWnd::UpdateTreeInfosEvent));
    
    m_animationCtrl = new wxAnimationCtrl(this, wxID_ANY);
    m_animationCtrl->Show(false);
    
    wxString resourcePath = CFileUtility::GetResourcesFolderPath();
    m_animationCtrl->LoadFile(resourcePath + "/loading.gif");
}


CPanelInfosWnd::~CPanelInfosWnd()
{
    delete(m_animationCtrl);
	delete(oldInfosFileControl);
	delete(treeWindow);
	delete(InfosFileScroll);
	//delete(fileGeolocalisation);
	delete(infosToolbar);
	delete(webBrowser);
}

void CPanelInfosWnd::UpdateTreeInfosEvent(wxCommandEvent &event)
{
    CThreadLoadInfos * threadInfos = (CThreadLoadInfos *)event.GetClientData();
    if(threadInfos->filename == filename)
    {
        threadInfos->infosFileWnd->CreateElement();
        StopLoadingPicture(InfosFileScroll);
        treeWindow->SetTreeControl(threadInfos->infosFileWnd);
        delete(oldInfosFileControl);
        oldInfosFileControl = threadInfos->infosFileWnd;
    }
    else
        delete(threadInfos->infosFileWnd);
    
    threadInfos->threadLoadInfos->join();
    delete threadInfos->threadLoadInfos;
    delete threadInfos;
    
}

void CPanelInfosWnd::GenerateTreeInfos(CThreadLoadInfos * threadInfos)
{
    CInfosFile * infosFileWnd = threadInfos->infosFileWnd;
    infosFileWnd->SetFile(threadInfos->filename);
    
    wxCommandEvent event(EVENT_UPDATEINFOSTHREAD);
    event.SetClientData(threadInfos);
    wxPostEvent(threadInfos->panelInfos, event);
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

void CPanelInfosWnd::StartLoadingPicture(wxWindow * window)
{
    window->Show(false);
    m_animationCtrl->Show(true);
    m_animationCtrl->Play();
    this->Resize();
    
}

void CPanelInfosWnd::StopLoadingPicture(wxWindow * window)
{
    m_animationCtrl->Stop();
    m_animationCtrl->Show(false);
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
    CInfosFile * infosFileWnd = new CInfosFile(treeWindow->GetTheme(), treeWindow);
    StartLoadingPicture(InfosFileScroll);
    CThreadLoadInfos * threadInfos = new CThreadLoadInfos();
    threadInfos->infosFileWnd = infosFileWnd;
    threadInfos->panelInfos = this;
    threadInfos->filename = filename;
    threadInfos->threadLoadInfos = new thread(GenerateTreeInfos, threadInfos);
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
    InfosFileScroll->UpdateScreenRatio();
    infosToolbar->UpdateScreenRatio();
    
    if(oldInfosFileControl != nullptr)
    {
        oldInfosFileControl->UpdateScreenRatio();
        treeWindow->UpdateScreenRatio();
    }


    this->Resize();

}

void CPanelInfosWnd::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    wxRect rc = GetRect();
    this->FillRect(&dc, rc, themeBitmap.colorScreen);
    if(m_animationCtrl->IsShown())
        m_animationCtrl->SetBackgroundColour(themeBitmap.colorScreen);
}



void CPanelInfosWnd::ClickShowButton(const int &id)
{
	switch (id)
	{
	case WM_INFOS:
		ShowInfos();
        infosToolbar->SetInfosPush();
		break;
	case WM_MAPS:
		ShowMap();
        infosToolbar->SetMapPush();
		break;
	}
}

void CPanelInfosWnd::ShowInfos()
{
	if (!infosToolbar->IsShown())
		infosToolbar->Show(true);

	if (webBrowser->IsShown())
		webBrowser->Show(false);

	if (!InfosFileScroll->IsShown())
		InfosFileScroll->Show(true);
    
    InfosUpdate();
	Resize();
}

void CPanelInfosWnd::ShowMap()
{
	if (!infosToolbar->IsShown())
		infosToolbar->Show(true);

	if (!webBrowser->IsShown())
		webBrowser->Show(true);

	if (InfosFileScroll->IsShown())
		InfosFileScroll->Show(false);
    
    wxString newUrl = MapsUpdate();
    if(newUrl != url)
        DisplayURL(newUrl);
    
    url = newUrl;
    
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
	else if (InfosFileScroll->IsShown())
		InfosFileScroll->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
	else if (webBrowser->IsShown())
		webBrowser->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
}
