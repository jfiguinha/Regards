#include <header.h>
#include "InfosFileWnd.h"

#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif

using namespace Regards::Control;
using namespace Regards::Window;

#define TAILLEMAX 4096

wxDEFINE_EVENT(EVENT_UPDATEINFOSTHREAD, wxCommandEvent);

CInfosFileWnd::CInfosFileWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & theme)
: CWindowMain("CInfosFileWnd",parent, id)
{
	InfosFileScroll = nullptr;
	treeWindow = nullptr;
	infosFile = nullptr;
	oldInfosFileControl = nullptr;
    InfosFileScroll = new CScrollbarWnd(this, wxID_ANY);
    treeWindow = new CTreeWindow(InfosFileScroll, INFOSFILEWINDOWID, theme);
    InfosFileScroll->SetCentralWindow(treeWindow, themeScroll);
    Connect(EVENT_UPDATEINFOSTHREAD, wxCommandEventHandler(CInfosFileWnd::UpdateTreeInfosEvent));
}

CInfosFileWnd::~CInfosFileWnd(void)
{
    TRACE();
    
    if(oldInfosFileControl != nullptr)
        delete(oldInfosFileControl);
    
    if(treeWindow != nullptr)
        delete(treeWindow);
    
    if(InfosFileScroll != nullptr)
        delete(InfosFileScroll);
}

void CInfosFileWnd::UpdateTreeInfosEvent(wxCommandEvent &event)
{
    TRACE();
    
    CThreadLoadInfos * threadInfos = (CThreadLoadInfos *)event.GetClientData();
    if(threadInfos->filename == filename)
    {
        threadInfos->infosFileWnd->CreateElement();
        //StopLoadingPicture(InfosFileScroll);
        wxCommandEvent * event = new wxCommandEvent(wxEVT_STOPANIMATION);
        event->SetClientData(this);
        wxQueueEvent(this->GetParent(), event);
        
        treeWindow->SetTreeControl(threadInfos->infosFileWnd);

		delete(oldInfosFileControl);
        oldInfosFileControl = threadInfos->infosFileWnd;
    }
    else
        delete(threadInfos->infosFileWnd);
    
    threadInfos->threadLoadInfos->join();
    delete threadInfos->threadLoadInfos;
    delete threadInfos;
    

	treeWindow->Refresh();
}

void CInfosFileWnd::InfosUpdate(const wxString &filename)
{
    TRACE();
    
	if(filename != this->filename)
	{
		this->filename = filename;
		CInfosFile * infosFileWnd = new CInfosFile(treeWindow->GetTheme(), treeWindow);
		CThreadLoadInfos * threadInfos = new CThreadLoadInfos();
		threadInfos->infosFileWnd = infosFileWnd;
		threadInfos->panelInfos = this;
		threadInfos->filename = filename;
		threadInfos->threadLoadInfos = new thread(GenerateTreeInfos, threadInfos);
    
		wxCommandEvent * event = new wxCommandEvent(wxEVT_STARTANIMATION);
		event->SetClientData(this);
		wxQueueEvent(this->GetParent(), event);
	}
	/*
	int x = InfosFileScroll->GetSize().x;
	int y = InfosFileScroll->GetSize().y;

	int width = GetWindowWidth();
	int height = GetWindowHeight();

	Resize();
	*/
}

void CInfosFileWnd::GenerateTreeInfos(CThreadLoadInfos * threadInfos)
{
    TRACE();
    
    CInfosFile * infosFileWnd = threadInfos->infosFileWnd;
    infosFileWnd->SetFile(threadInfos->filename);
    
    wxCommandEvent * event = new wxCommandEvent(EVENT_UPDATEINFOSTHREAD);
    event->SetClientData(threadInfos);
    wxQueueEvent(threadInfos->panelInfos, event);
}

void CInfosFileWnd::UpdateScreenRatio()
{
    TRACE();
    
    InfosFileScroll->UpdateScreenRatio();
    
    if(oldInfosFileControl != nullptr)
    {
        oldInfosFileControl->UpdateScreenRatio();
        treeWindow->UpdateScreenRatio();
    }
}

void CInfosFileWnd::Resize()
{
    TRACE();
    
	if(InfosFileScroll != nullptr)
	{
		InfosFileScroll->SetSize(GetWindowWidth(), GetWindowHeight());
		//InfosFileScroll->SendSizeEvent();
		treeWindow->SetSize(GetWindowWidth(),GetWindowHeight());
		//treeWindow->SendSizeEvent();
	}
}
