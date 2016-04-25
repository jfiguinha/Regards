#include "CriteriaTreeWnd.h"
#include "CalendarDialogBox.h"
#include "MapDialogBox.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <ConvertUtility.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <SqlCriteria.h>
#include <SqlPhotoCriteria.h>
#include <Gps.h>
#include <SqlGps.h>
#include <PhotoCriteria.h>
#include <SqlPhotos.h>
#include <TreeDataLink.h>
//#include <MainWindow.h>
#ifdef __APPLE__
#include <AppleReadExif.h>
#endif
#include <MapSelect.h>
#include <CalendarSelect.h>
using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Internet;
using namespace Regards::Control;
#define TAILLEMAX 4096

wxDEFINE_EVENT(EVENT_UPDATEINFOSTHREAD, wxCommandEvent);

CCriteriaTreeWnd::CCriteriaTreeWnd(wxWindow* parent, wxWindowID id, const int &mainWindowID, CFileGeolocation * fileGeolocalisation, const CThemeTree & theme,const CThemeScrollBar & themeScroll)
: CWindowMain(parent, id)
{
    this->mainWindowID = mainWindowID;
    //CThemeScrollBar themeScroll;
    //CExplorerTheme * explorerTheme = CExplorerThemeInit::getInstance();
    this->fileGeolocalisation = fileGeolocalisation;
    
    //if (explorerTheme != nullptr)
    //{
    //    explorerTheme->GetScrollInfosFileTheme(themeScroll);
        InfosFileScroll = new CScrollbarWnd(this, wxID_ANY);
    //}
    
    //if (explorerTheme != nullptr)
    //{
     //   CThemeTree theme;
     //   explorerTheme->GetTreeInfosFileTheme(theme);
        treeWindow = new CTreeWindow(InfosFileScroll, wxID_ANY, theme);
        InfosFileScroll->SetCentralWindow(treeWindow, themeScroll);
    //}

    Connect(wxEVT_SHOWCALENDAR, wxCommandEventHandler(CCriteriaTreeWnd::ShowCalendar));
    Connect(wxEVT_SHOWMAP, wxCommandEventHandler(CCriteriaTreeWnd::ShowMap));
}

wxString CCriteriaTreeWnd::GenerateUrl()
{
    if(fileGeolocalisation->HasGps())
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
    return "http://www.openstreetmap.org";
}

void CCriteriaTreeWnd::ShowCalendar(wxCommandEvent &event)
{
    CPhotoCriteria * photoCriteria = (CPhotoCriteria *)event.GetClientData();
    CCalendarSelect calendarSelect;
    if(calendarSelect.SelectNewDate(this, fileGeolocalisation->GetDateTimeInfos()))
    {
        bool isNew = false;
        CSqlCriteria sqlCriteria;
        CSqlPhotoCriteria sqlPhotoCriteria;
        int numCriteriaId = sqlCriteria.GetOrInsertCriteriaId(1, 3, calendarSelect.GetSelectStringDate(), isNew);
        sqlPhotoCriteria.InsertPhotoCriteria(numPhotoId, numCriteriaId);
        if(photoCriteria != nullptr && numCriteriaId != photoCriteria->GetCriteriaId())
            sqlPhotoCriteria.DeletePhotoCriteria(numPhotoId, photoCriteria->GetCriteriaId());
#ifdef __APPLE__ 
        CAppleReadExif appleReadExif;
        appleReadExif.WriteDateTime(filename, calendarSelect.GetSelectDate());
#endif
        fileGeolocalisation->RefreshData();
        
        wxWindow * mainWnd = this->FindWindowById(mainWindowID);
        wxCommandEvent * eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
        wxQueueEvent(mainWnd, eventChange);
    }
    
    if(photoCriteria != nullptr)
        delete photoCriteria;
    
    this->Refresh();
}

void CCriteriaTreeWnd::ShowMap(wxCommandEvent &event)
{
    CMapSelect mapSelect;
    wxString url = GenerateUrl();
    CPhotoCriteria * photoCriteria = (CPhotoCriteria *)event.GetClientData();
    wxString infoGpsLocalisation = mapSelect.SelectNewMapLocalisation(this, url);
    
    if(infoGpsLocalisation != "")
    {
        bool isNew = false;
        CSqlCriteria sqlCriteria;
        CSqlGps sqlGps;
        CSqlPhotoCriteria sqlPhotoCriteria;
        int numCriteriaId = sqlCriteria.GetOrInsertCriteriaId(1, 1, infoGpsLocalisation, isNew);
        sqlPhotoCriteria.InsertPhotoCriteria(numPhotoId, numCriteriaId);
        if(photoCriteria != nullptr && numCriteriaId != photoCriteria->GetCriteriaId())
            sqlPhotoCriteria.DeletePhotoCriteria(numPhotoId, photoCriteria->GetCriteriaId());
        
        
        wxString latitude = mapSelect.GetLatitude();
        wxString longitude = mapSelect.GetLongitude();
        
        sqlGps.DeleteGps(filename);
        sqlGps.InsertGps(filename, latitude, longitude);
        
        
        double dlat = 0.0;
        double dlong = 0.0;
        latitude.ToDouble(&dlat);
        longitude.ToDouble(&dlong);
        
#ifdef __APPLE__
        CAppleReadExif appleReadExif;
        appleReadExif.WriteGps(filename, dlat, dlong);
#endif
        fileGeolocalisation->RefreshData();
        
        //Update Infos
        wxWindow * mainWnd = this->FindWindowById(mainWindowID);
        wxCommandEvent * eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
        wxQueueEvent(mainWnd, eventChange);
    }
    
    if(photoCriteria != nullptr)
        delete photoCriteria;
    
    this->Refresh();
}



CCriteriaTreeWnd::~CCriteriaTreeWnd(void)
{
    if(oldCriteriaTree != nullptr)
        delete(oldCriteriaTree);
    
    if(treeWindow != nullptr)
        delete(treeWindow);
    
    if(InfosFileScroll != nullptr)
        delete(InfosFileScroll);
}

void CCriteriaTreeWnd::UpdateTreeData()
{
    CCriteriaTree * criteriaTree = new CCriteriaTree(treeWindow->GetTheme(), treeWindow);
    criteriaTree->SetFile(filename, numPhotoId);
    criteriaTree->CreateElement();
    treeWindow->SetTreeControl(criteriaTree);
    if(oldCriteriaTree != nullptr)
        delete(oldCriteriaTree);
    oldCriteriaTree = criteriaTree;
}

void CCriteriaTreeWnd::SetFile(const wxString &filename)
{
    this->filename = filename;
    
    CSqlPhotos sqlPhotos;
    numPhotoId = sqlPhotos.GetPhotoId(filename);
    
    UpdateTreeData();
    
    this->Refresh();
}


void CCriteriaTreeWnd::UpdateScreenRatio()
{
    InfosFileScroll->UpdateScreenRatio();
    
    if(oldCriteriaTree != nullptr)
    {
        oldCriteriaTree->UpdateScreenRatio();
        treeWindow->UpdateScreenRatio();
    }
}

void CCriteriaTreeWnd::Resize()
{
    if(InfosFileScroll != nullptr)
        if (InfosFileScroll->IsShown())
            InfosFileScroll->SetSize(0, 0, width, height);
}
