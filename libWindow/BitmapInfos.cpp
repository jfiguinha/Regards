#include "header.h"
#include "BitmapInfos.h"
#include <FileUtility.h>
#include <LibResource.h>
#include <ConvertUtility.h>
#include <wx/dcbuffer.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <Gps.h>
#include <SqlPhotos.h>
#include <FileGeolocation.h>

using namespace Regards::Window;
using namespace Regards::Sqlite;
using namespace Regards::Internet;
wxDEFINE_EVENT(EVENT_ENDINFOSUPDATE, wxCommandEvent);
#include <thread>
using namespace std;

#define wxTIMER_GPSINFOS 2000

CBitmapInfos::CBitmapInfos(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos & theme, CFileGeolocation * fileGeolocalisation)
	: CWindowMain("CBitmapInfos", parent, id)
{
    TRACE();
	gpsInfosUpdate = false;
	bitmapInfosTheme = theme;
	this->fileGeolocalisation = fileGeolocalisation;
    gpsTimer = new wxTimer(this, wxTIMER_GPSINFOS);
	Connect(wxEVT_PAINT, wxPaintEventHandler(CBitmapInfos::OnPaint));
    Connect(wxTIMER_GPSINFOS, wxEVT_TIMER, wxTimerEventHandler(CBitmapInfos::OnTimerGPSUpdate), nullptr, this);
}

void CBitmapInfos::OnTimerGPSUpdate(wxTimerEvent& event)
{
    
    printf("CBitmapInfos OnTimerGPSUpdate \n");
    mufileGeoloc.lock();
    CSqlPhotos sqlPhotos;
    int insertValue = sqlPhotos.GetCriteriaInsert(filename);
    if (insertValue > 0 && fileGeolocalisation->HasGps() && fileGeolocalisation->GetFilename() == filename && gpsInfos == "")
    {
        CriteriaVector criteriaList;
        sqlPhotos.GetPhotoCriteria(&criteriaList, filename);

        for (CCriteria criteria : criteriaList)
        {
            if (criteria.GetCategorieId() == 1)
            {
                gpsInfos = criteria.GetLibelle();
                break;
            }
        }   
    
        if(gpsInfos == "")
        {
            gpsInfos = "";
            wxString urlServer = "";
            CRegardsConfigParam * param = CParamInit::getInstance();
            if (param != nullptr)
                urlServer = param->GetUrlServer();

            CGps * gps = new CGps(urlServer);
            //Execution de la requête de géolocalisation
            if (gps->GeolocalisationGPS(fileGeolocalisation->GetLatitude(), fileGeolocalisation->GetLongitude()))
            {
                GeoPluginVector * geoPluginVector = gps->GetGpsList();
                for (CGeoPluginValue geoValue : *geoPluginVector)
                {
                    gpsInfos.append(geoValue.GetCountryCode());
                    gpsInfos.append(L".");
                    gpsInfos.append(geoValue.GetRegion());
                    gpsInfos.append(L".");
                    gpsInfos.append(geoValue.GetPlace());
                    break;
                }
            }
            delete gps;
        }
    }
    
    mufileGeoloc.unlock();
    
    if(gpsInfos == "" && fileGeolocalisation->GetFilename() == filename)
        gpsTimer->Start(500, wxTIMER_ONE_SHOT);
    else
        Refresh();
}

void CBitmapInfos::SetFilename(const wxString &libelle)
{
    printf("SetFilename \n");
	if (filename != libelle)
	{
        gpsInfos = "";
        filename = libelle;
		gpsInfosUpdate = false;            
        processIdle = true;
        UpdateData();
	}
}

void CBitmapInfos::UpdateData()
{
    printf("UpdateData \n");
    gpsInfos = "";
	mufileGeoloc.lock();
    //Recherche dans la base de données des critères sur le fichier
    CSqlPhotos sqlPhotos;
    int insertValue = sqlPhotos.GetCriteriaInsert(filename);
    if (insertValue > 0)
    {
        CriteriaVector criteriaList;
        sqlPhotos.GetPhotoCriteria(&criteriaList, filename);

        for (CCriteria criteria : criteriaList)
        {
            if (criteria.GetCategorieId() == 1)
            {
                gpsInfos = criteria.GetLibelle();
            }
            else if (criteria.GetCategorieId() == 3)
            {
                dateInfos = criteria.GetLibelle();
            }
        }
        SetDateInfos(dateInfos,'.');
    }
    else if(fileGeolocalisation->GetFilename() == filename)
    {
        
        dateInfos = L"";
        gpsInfos = fileGeolocalisation->GetGpsInformation();
        wxString dataInfos = fileGeolocalisation->GetDateTimeInfos();
        if(dataInfos.Length() > 10)
            SetDateInfos(dataInfos, dataInfos[4]);
            
        if(fileGeolocalisation->HasGps() && gpsInfos == "")
            gpsTimer->Start(500, wxTIMER_ONE_SHOT);
    }
    else
    {
        dateInfos = L"";
        gpsInfos = "";
    }
    
    if(gpsInfos == "" && fileGeolocalisation->HasGps())
        gpsTimer->Start(500, wxTIMER_ONE_SHOT);
    
    processIdle = true;
	mufileGeoloc.unlock();
    Refresh();
}

int CBitmapInfos::Dayofweek(int d, int m, int y)
{
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	y -= m < 3;
	return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

void CBitmapInfos::SetDateInfos(const wxString &dataInfos, char seperator)
{
	wxString listMonth = CLibResource::LoadStringFromResource("LBLMONTHNAME", 1);
	wxString listDay = CLibResource::LoadStringFromResource("LBLDAYNAME", 1);
	vector<wxString> MonthName = CConvertUtility::split(listMonth, ',');
	vector<wxString> DayName = CConvertUtility::split(listDay, ',');
	vector<wxString> vDateTime = CConvertUtility::split(dataInfos, seperator);
	if (vDateTime.size() >= 3)
	{
		int year = atoi(vDateTime[0].c_str());
		int month = atoi(vDateTime[1].c_str());
		int day = atoi(vDateTime[2].c_str());
		int ijour = Dayofweek(day, month, year);
		dateInfos = DayName.at(ijour) + L" " + to_string(day) + L" " + MonthName.at(month - 1) + L", " + to_string(year) + L" ";
	}
}

CBitmapInfos::~CBitmapInfos()
{
    if(gpsTimer != nullptr)
    {
        if(gpsTimer->IsRunning())
            gpsTimer->Stop();
            
        delete gpsTimer;        
    }

	//delete fileGeolocalisation;
}

int CBitmapInfos::GetHeight()
{
	return bitmapInfosTheme.GetHeight();
}

void CBitmapInfos::UpdateScreenRatio()
{
    Resize();
}

void CBitmapInfos::Redraw()
{
	wxClientDC dc(this);
    DrawInformations(&dc);
}

void CBitmapInfos::DrawInformations(wxDC * dc)
{
#ifdef __WXGTK__
    double scale_factor = dc->GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
    wxRect rc = GetWindowRect();
    FillRect(dc, rc, bitmapInfosTheme.colorBack);
    wxString message = L"";
    wxString libelle = CFileUtility::GetFileName(filename);
    CThemeFont font = bitmapInfosTheme.themeFont;
    font.SetFontSize(bitmapInfosTheme.themeFont.GetFontSize() / scale_factor);
    wxSize size = GetSizeTexte(dc, libelle, font);

    DrawTexte(dc, libelle, (GetWindowWidth() / scale_factor - size.x) / 2, 0, font);
    
    if (gpsInfos != L"")
    {
        wchar_t seperator = '.';
        vector<wxString> listGeo = CConvertUtility::split(gpsInfos, seperator);
        message = dateInfos + L"," + listGeo.at(listGeo.size() - 1);
    }
    else
    {
        message = dateInfos;
    }
    
    size = GetSizeTexte(dc, message, font);
    DrawTexte(dc, message, (GetWindowWidth() / scale_factor - size.x) / 2, (GetWindowHeight() / scale_factor) / 2, font);
}

void CBitmapInfos::OnPaint(wxPaintEvent& event)
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;
    
	wxBufferedPaintDC dc(this);
    DrawInformations(&dc);
}