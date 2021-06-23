#include "header.h"
#include "BitmapInfos.h"
#include <FileUtility.h>
#include <LibResource.h>
#include <ConvertUtility.h>
#include <wx/dcbuffer.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <SqlPhotos.h>
#include <FileGeolocation.h>
#include <GpsEngine.h>
using namespace Regards::Window;
using namespace Regards::Sqlite;
using namespace Regards::Internet;
using namespace std;


CBitmapInfos::CBitmapInfos(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos & theme)
	: CWindowMain("CBitmapInfos", parent, id)
{
    TRACE();
	CListOfWindow * fileGeolocalisation = CGpsEngine::getInstance();
	gpsInfosUpdate = false;
	bitmapInfosTheme = theme;
	fileGeolocalisation->AddWindow(this);
    //gpsTimer = new wxTimer(this, wxTIMER_GPSINFOS);
	Connect(wxEVT_PAINT, wxPaintEventHandler(CBitmapInfos::OnPaint));
    Connect(wxEVENT_UPDATEGPSINFOS, wxCommandEventHandler(CBitmapInfos::OnUpdateGpsInfos));
}

void CBitmapInfos::OnUpdateGpsInfos(wxCommandEvent& event)
{
	wxString urlServer = "";
	//Géolocalisation
	CRegardsConfigParam * param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetUrlServer();
	}
	
	wxString * filename = (wxString *)event.GetClientData();
	int typeData = event.GetInt();
	if(filename != nullptr)
	{
		if (*filename == this->filename)
		{
            wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO",1);
			CFileGeolocation fileGeolocalisation(urlServer);
			fileGeolocalisation.SetFile(*filename, notGeo);
			if (typeData == 1)
			{
				printf("CBitmapInfos OnTimerGPSUpdate \n");
				gpsInfos = fileGeolocalisation.GetGpsInformation();
			}
			else if (typeData == 2)
			{
				dateInfos = fileGeolocalisation.GetDateTimeInfos();
				SetDateInfos(dateInfos, '.');
			}
			delete filename;
		}
	}


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
        dateInfos = L"";
        gpsInfos = "";
    }
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
    double scale_factor = 1.0f;
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