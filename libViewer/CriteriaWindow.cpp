#include <header.h>
#include "CriteriaWindow.h"
#include <CriteriaTreeWnd.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <window_id.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Control;


CCriteriaWindow::CCriteriaWindow(wxWindow* parent, wxWindowID id,
	const CThemeSplitter & theme, const bool &horizontal)
	: CSplitter(parent, id, theme)
{
    
    url = "http://www.google.fr";
    
	//CRegardsConfigParam * config = CParamInit::getInstance();
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();
    
    if (viewerTheme != nullptr)
    {
        CThemeScrollBar themeScroll;
        viewerTheme->GetScrollTheme(&themeScroll);
        
        CThemeTree themeTree;
        viewerTheme->GetTreeTheme(&themeTree);
        criteriaTreeWnd = new CCriteriaTreeWnd(this, wxID_ANY, MAINVIEWERWINDOWID, themeTree, themeScroll);
    }
    
    if(webBrowser == nullptr)
    {
        webBrowser = wxWebView::New(this, wxID_ANY);
        webBrowser->Show(true);   
 
    }
    
	SetHorizontal(horizontal);

	this->SetWindow(criteriaTreeWnd, webBrowser);
    
    Connect(wxEVENT_UPDATEURL, wxCommandEventHandler(CCriteriaWindow::UpdateURL));

}

wxString CCriteriaWindow::MapsUpdate(const wxString &filename)
{
	wxString urlServer;
	CRegardsConfigParam * param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetUrlServer();
	}
	
	CFileGeolocation * fileGeolocalisation = new CFileGeolocation(urlServer);
	fileGeolocalisation->SetFile(filename);
	wxString url = L"http://www.openstreetmap.org/?mlat=";
	url.append(fileGeolocalisation->GetLatitude());
	url.append(L"&mlon=");
	url.append(fileGeolocalisation->GetLongitude());
	url.append(L"#map=15/");
	url.append(fileGeolocalisation->GetLatitude());
	url.append(L"/");
	url.append(fileGeolocalisation->GetLongitude());


	delete fileGeolocalisation;
	
    return url;
}

void CCriteriaWindow::UpdateURL(wxCommandEvent& event)
{
    wxString newUrl = MapsUpdate(filename);
    if(url != newUrl)
        webBrowser->LoadURL(newUrl);    
    url = newUrl;
}

void CCriteriaWindow::SetFile(const wxString &filename)
{
    if(criteriaTreeWnd != nullptr)
        criteriaTreeWnd->SetFile(filename);
        
    this->filename = filename;
    wxString newUrl = MapsUpdate(filename);
    if(url != newUrl)
        webBrowser->LoadURL(newUrl);
    url = newUrl;
    
    
}

CCriteriaWindow::~CCriteriaWindow()
{
 	if (criteriaTreeWnd != nullptr)
		delete(criteriaTreeWnd);

    if(webBrowser != nullptr)
        delete(webBrowser);   
}


void CCriteriaWindow::UpdateScreenRatio()
{
 	if (criteriaTreeWnd != nullptr)
		criteriaTreeWnd->UpdateScreenRatio();
 
}