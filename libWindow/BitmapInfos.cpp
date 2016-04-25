#include "BitmapInfos.h"
#include <FileUtility.h>
#include <LibResource.h>
#include <ConvertUtility.h>
#include <wx/dcbuffer.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <Gps.h>
#include <SqlPhotos.h>
using namespace Regards::Window;
using namespace Regards::Sqlite;
using namespace Regards::Internet;
wxDEFINE_EVENT(EVENT_ENDINFOSUPDATE, wxCommandEvent);

CBitmapInfos::CBitmapInfos(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos & theme, CFileGeolocation * fileGeolocalisation)
	: CWindowMain(parent, id)
{
	bitmapInfosTheme = theme;
	this->fileGeolocalisation = fileGeolocalisation;
	Connect(wxEVT_PAINT, wxPaintEventHandler(CBitmapInfos::OnPaint));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CBitmapInfos::OnIdle));
}


void CBitmapInfos::OnIdle(wxIdleEvent& evt)
{
	if (fileGeolocalisation->HasGps() && gpsInfos == "" && gpsInfosUpdate)
	{
        wxString urlServer = L"";
        CRegardsConfigParam * param = CParamInit::getInstance();
        if (param != nullptr)
        {
            urlServer = param->GetUrlServer();
        }
        
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
        
		gpsInfosUpdate = false;
        
        this->Refresh();
	}
}


void CBitmapInfos::SetFilename(const wxString &libelle)
{
	if (filename != libelle)
	{
        filename = libelle;
        UpdateData();
	}
}

void CBitmapInfos::UpdateData()
{
    gpsInfos = "";
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
    else
    {
        if(fileGeolocalisation->HasGps())
        {
            gpsInfosUpdate = true;
        }
        
        dateInfos = L"";
        gpsInfos = fileGeolocalisation->GetGpsInformation();
        filename = filename;
        wxString dataInfos = fileGeolocalisation->GetDateTimeInfos();
        if(dataInfos.Length() > 10)
            SetDateInfos(dataInfos, dataInfos[4]);
    }
    this->Refresh();
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
		dateInfos = DayName.at(ijour) + L" " + std::to_string(day) + L" " + MonthName.at(month - 1) + L", " + std::to_string(year) + L" ";
	}
}



CBitmapInfos::~CBitmapInfos()
{
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

void CBitmapInfos::Resize()
{
	this->Refresh();
}

void CBitmapInfos::Redraw()
{
	wxWindowDC dc(this);
    DrawInformations(&dc);
}

void CBitmapInfos::DrawInformations(wxDC * dc)
{
    
    wxRect rc;
    rc.x = 0;
    rc.y = 0;
    rc.width = width;
    rc.height = height;
    FillRect(dc, rc, bitmapInfosTheme.colorBack);
    wxString message = L"";
    wxString libelle = CFileUtility::GetFileName(filename);
    
    wxSize size = GetSizeTexte(dc, libelle, bitmapInfosTheme.themeFont);

    DrawTexte(dc, libelle, (width - size.x) / 2, 0, bitmapInfosTheme.themeFont);
    
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
    
    size = GetSizeTexte(dc, message, bitmapInfosTheme.themeFont);
    DrawTexte(dc, message, (width - size.x) / 2, height / 2, bitmapInfosTheme.themeFont);
}

void CBitmapInfos::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
    DrawInformations(&dc);
}