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
: CWindowMain(parent, id)
{
    InfosFileScroll = new CScrollbarWnd(this, wxID_ANY);
    treeWindow = new CTreeWindow(InfosFileScroll, wxID_ANY, theme);
    InfosFileScroll->SetCentralWindow(treeWindow, themeScroll);
    Connect(EVENT_UPDATEINFOSTHREAD, wxCommandEventHandler(CInfosFileWnd::UpdateTreeInfosEvent));
}

CInfosFileWnd::~CInfosFileWnd(void)
{
    if(oldInfosFileControl != nullptr)
        delete(oldInfosFileControl);
    
    if(treeWindow != nullptr)
        delete(treeWindow);
    
    if(InfosFileScroll != nullptr)
        delete(InfosFileScroll);
}

void CInfosFileWnd::UpdateTreeInfosEvent(wxCommandEvent &event)
{
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
    
}

void CInfosFileWnd::InfosUpdate(const wxString &filename)
{
    this->filename = filename;
    CInfosFile * infosFileWnd = new CInfosFile(treeWindow->GetTheme(), treeWindow);
    CThreadLoadInfos * threadInfos = new CThreadLoadInfos();
    threadInfos->infosFileWnd = infosFileWnd;
    threadInfos->panelInfos = this;
    threadInfos->filename = filename;
    threadInfos->threadLoadInfos = new std::thread(GenerateTreeInfos, threadInfos);
    
    wxCommandEvent * event = new wxCommandEvent(wxEVT_STARTANIMATION);
    event->SetClientData(this);
    wxQueueEvent(this->GetParent(), event);
}

void CInfosFileWnd::GenerateTreeInfos(CThreadLoadInfos * threadInfos)
{
    CInfosFile * infosFileWnd = threadInfos->infosFileWnd;
    infosFileWnd->SetFile(threadInfos->filename);
    
    wxCommandEvent * event = new wxCommandEvent(EVENT_UPDATEINFOSTHREAD);
    event->SetClientData(threadInfos);
    wxQueueEvent(threadInfos->panelInfos, event);
}

void CInfosFileWnd::UpdateScreenRatio()
{
    InfosFileScroll->UpdateScreenRatio();
    
    if(oldInfosFileControl != nullptr)
    {
        oldInfosFileControl->UpdateScreenRatio();
        treeWindow->UpdateScreenRatio();
    }
}

void CInfosFileWnd::Resize()
{
    if(InfosFileScroll != nullptr)
        if (InfosFileScroll->IsShown())
            InfosFileScroll->SetSize(0, 0, width, height);
}
