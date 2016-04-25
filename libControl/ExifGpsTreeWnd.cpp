#include "ExifGpsTreeWnd.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <ConvertUtility.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <Gps.h>
#include <TreeDataLink.h>
#ifdef __APPLE__
#include <AppleReadExif.h>
#endif
using namespace Regards::Window;
using namespace Regards::Internet;
using namespace Regards::Control;
#define TAILLEMAX 4096

wxDEFINE_EVENT(EVENT_UPDATEINFOSTHREAD, wxCommandEvent);

CExifGpsTreeWnd::CExifGpsTreeWnd(wxWindow* parent, wxWindowID id, const CThemeTree & theme,const CThemeScrollBar & themeScroll)
: CWindowMain(parent, id)
{
    InfosFileScroll = new CScrollbarWnd(this, wxID_ANY);
    treeWindow = new CTreeWindow(InfosFileScroll, wxID_ANY, theme);
    InfosFileScroll->SetCentralWindow(treeWindow, themeScroll);

}

int CExifGpsTreeWnd::GetHeight()
{
    if(treeWindow != nullptr)
        return treeWindow->GetHeight();
    
    return 0;
}

int CExifGpsTreeWnd::GetWidth()
{
    if(treeWindow != nullptr)
        return treeWindow->GetWidth();
    
    return 0;
}

CExifGpsTreeWnd::~CExifGpsTreeWnd(void)
{
    if(oldCriteriaTree != nullptr)
        delete(oldCriteriaTree);
    
    if(treeWindow != nullptr)
        delete(treeWindow);
    
    if(InfosFileScroll != nullptr)
        delete(InfosFileScroll);
}

void CExifGpsTreeWnd::SetFile(const wxString &filename)
{
    this->filename = filename;
    
    CExifGpsTree * criteriaTree = new CExifGpsTree(treeWindow->GetTheme(), treeWindow);
    criteriaTree->SetFile(filename);
    criteriaTree->CreateElement();
    treeWindow->SetTreeControl(criteriaTree);
    if(oldCriteriaTree != nullptr)
        delete(oldCriteriaTree);
    oldCriteriaTree = criteriaTree;
    
    this->Refresh();
}

void CExifGpsTreeWnd::SaveChange()
{
#ifdef __APPLE__
    CAppleReadExif appleReadExif;
    appleReadExif.WriteGps(filename, latitude, longitude);
#endif
}

void CExifGpsTreeWnd::SetFile(const wxString & filename, const float &latitude, const float &longitude)
{
    this->filename = filename;
    this->latitude = latitude;
    this->longitude = longitude;
    CExifGpsTree * criteriaTree = new CExifGpsTree(treeWindow->GetTheme(), treeWindow);
    criteriaTree->SetFile(filename, latitude, longitude);
    criteriaTree->CreateElement();
    treeWindow->SetTreeControl(criteriaTree);
    if(oldCriteriaTree != nullptr)
        delete(oldCriteriaTree);
    oldCriteriaTree = criteriaTree;
    
    this->Refresh();
}


void CExifGpsTreeWnd::UpdateScreenRatio()
{
    InfosFileScroll->UpdateScreenRatio();
    
    if(oldCriteriaTree != nullptr)
    {
        oldCriteriaTree->UpdateScreenRatio();
        treeWindow->UpdateScreenRatio();
    }
}

void CExifGpsTreeWnd::Resize()
{
    if(InfosFileScroll != nullptr)
        if (InfosFileScroll->IsShown())
            InfosFileScroll->SetSize(0, 0, width, height);
}
