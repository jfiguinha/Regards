#include "CriteriaTreeWnd.h"
#include "CalendarDialogBox.h"
#include "MapDialogBox.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <ConvertUtility.h>

#if defined(LIBAPPLE)
#include <AppleReadExif.h>
#endif
#if defined(EXIV2)
#include <MetadataExiv2.h>
using namespace Regards::exiv2;
#endif

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
#include <MapSelect.h>
#include <CalendarSelect.h>
using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Internet;
using namespace Regards::Control;
#define TAILLEMAX 4096

wxDEFINE_EVENT(EVENT_UPDATEINFOSTHREAD, wxCommandEvent);

CCriteriaTreeWnd::CCriteriaTreeWnd(wxWindow* parent, wxWindowID id, const int &mainWindowID, 
	CFileGeolocation * fileGeolocalisation, const CThemeTree & theme,const CThemeScrollBar & themeScroll)
: CWindowMain("CCriteriaTreeWnd",parent, id)
{
	InfosFileScroll = nullptr;
	treeWindow = nullptr;
	criteriaTree = nullptr;
	oldCriteriaTree = nullptr;
	numPhotoId = 0;
	this->mainWindowID = mainWindowID;
    this->fileGeolocalisation = fileGeolocalisation;
    InfosFileScroll = new CScrollbarWnd(this, wxID_ANY);
    treeWindow = new CTreeWindow(InfosFileScroll, wxID_ANY, theme);
    InfosFileScroll->SetCentralWindow(treeWindow, themeScroll);
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
#if defined(LIBAPPLE)
        CAppleReadExif appleReadExif;
        appleReadExif.WriteDateTime(filename, calendarSelect.GetSelectDate());
#else
        CMetadataExiv2 metadata(filename);
        metadata.SetDateTime(calendarSelect.GetSelectStringDate());
        
#endif
        fileGeolocalisation->RefreshData();
        
        wxWindow * mainWnd = this->FindWindowById(mainWindowID);
        wxCommandEvent * eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
        wxQueueEvent(mainWnd, eventChange);
    }
    
    if(photoCriteria != nullptr)
        delete photoCriteria;
    
    this->FastRefresh(this);
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
        
#if defined(LIBAPPLE)
        CAppleReadExif appleReadExif;
        appleReadExif.WriteGps(filename, dlat, dlong);
#else
        wxString wlatitudeRef = "";
        wxString wlongitudeRef = "";
        wxString wlongitude = to_string(abs(dlong));
        wxString wlatitude = to_string(abs(dlat));
        
        if (dlat < 0)
            wlatitudeRef = "S";
        else
            wlatitudeRef = "N";
        
        if (dlong < 0)
            wlongitudeRef = "W";
        else
            wlongitudeRef = "E";
        
        CMetadataExiv2 metadataExiv2(filename);
        metadataExiv2.SetGpsInfos(wlatitudeRef, wlongitudeRef, wlatitude, wlongitude);
        
#endif
        fileGeolocalisation->RefreshData();
        
        //Update Infos
        wxWindow * mainWnd = this->FindWindowById(mainWindowID);
        wxCommandEvent * eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
        wxQueueEvent(mainWnd, eventChange);
    }
    
    if(photoCriteria != nullptr)
        delete photoCriteria;
    
    this->FastRefresh(this);
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
    
    this->FastRefresh(this);
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
	{
		InfosFileScroll->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
		//InfosFileScroll->SendSizeEvent();
		treeWindow->SetSize(GetWindowWidth(),GetWindowHeight());
		//treeWindow->SendSizeEvent();
	}
}
