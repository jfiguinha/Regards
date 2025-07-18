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
#include <wx/filename.h>
#include <MetadataExiv2.h>
using namespace Regards::Window;
using namespace Regards::Sqlite;
using namespace Regards::Internet;
using namespace Regards::exiv2;
using namespace std;

#define GPS_TIME 1000


struct InfosGps
{
	CWindowMain* window;
	wxString filename;
	wxString gpsInfos;
};

wxDEFINE_EVENT(wxEVENT_GPSINFOSLOCAL, wxCommandEvent);

CBitmapInfos::CBitmapInfos(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos& theme)
	: CWindowMain("CBitmapInfos", parent, id)
{
	CListOfWindow* fileGeolocalisation = CGpsEngine::getInstance();
	gpsInfosUpdate = false;
	bitmapInfosTheme = theme;
	threadGps = nullptr;
	fileGeolocalisation->AddWindow(this);
	listProcessWindow.push_back(this);
    gpsTimer = new wxTimer(this, wxTIMER_EXIT);
	Connect(wxEVT_PAINT, wxPaintEventHandler(CBitmapInfos::on_paint));
	Connect(wxTIMER_EXIT, wxEVT_TIMER, wxTimerEventHandler(CBitmapInfos::OnStartGps), nullptr, this);
	Connect(wxEVENT_UPDATEGPSINFOS, wxCommandEventHandler(CBitmapInfos::OnUpdateGpsInfos));
    Connect(wxEVENT_GPSINFOSLOCAL, wxCommandEventHandler(CBitmapInfos::UpdateGpsInfosLocal));
}

bool CBitmapInfos::GetProcessEnd()
{
	if (gpsTimer->IsRunning())
		gpsTimer->Stop();

	if (threadGps != nullptr)
		return false;

	return true;
}

void CBitmapInfos::OnStartGps(wxTimerEvent& event)
{
	if (threadGps == nullptr)
	{
		InfosGps* infos = new InfosGps();
		infos->window = this;
		infos->filename = filename;
		threadGps = new thread(GetGpsInfos, infos);
	}
	else
		gpsTimer->StartOnce(GPS_TIME);
}


void CBitmapInfos::GetGpsInfos(void* data)
{
	wxString urlServer = "";
	//Géolocalisation
	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetUrlServer();
	}

	InfosGps * infosGps = static_cast<InfosGps*>(data);

	if (infosGps != nullptr)
	{
		if (wxFileName::FileExists(infosGps->filename))
		{
			wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
			CFileGeolocation fileGeolocalisation(urlServer);
			fileGeolocalisation.SetFile(infosGps->filename, notGeo);
			infosGps->gpsInfos = fileGeolocalisation.Geolocalize();
			//infosGps->gpsInfos = fileGeolocalisation.GetGpsInformation();
		}
	}

	wxCommandEvent event(wxEVENT_GPSINFOSLOCAL);
	event.SetClientData(infosGps);
	wxPostEvent(infosGps->window, event);

}

void CBitmapInfos::UpdateGpsInfosLocal(wxCommandEvent& event)
{
	InfosGps* infosGps = (InfosGps *)event.GetClientData();
	threadGps->join();
	delete threadGps;
	threadGps = nullptr;

	if (infosGps->filename == filename)
	{
		gpsInfos = infosGps->gpsInfos;
		

		if(gpsInfos == "")
			gpsTimer->StartOnce(GPS_TIME);
		else
			needToRefresh = true;
	}

	delete infosGps;
}

void CBitmapInfos::OnUpdateGpsInfos(wxCommandEvent& event)
{
	wxString urlServer = "";
	//Géolocalisation
	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetUrlServer();
	}

	auto filename = static_cast<wxString*>(event.GetClientData());
	int typeData = event.GetInt();
	if (filename != nullptr)
	{
		if (*filename == this->filename)
		{
			wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
			CFileGeolocation fileGeolocalisation(urlServer);
			fileGeolocalisation.SetFile(*filename, notGeo);
			if (typeData == 1)
			{
				//printf("CBitmapInfos OnTimerGPSUpdate \n");
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

	needToRefresh = true;
}

void CBitmapInfos::SetFilename(const wxString& libelle)
{
	//printf("SetFilename \n");
	if (filename != libelle)
	{
		if (gpsTimer->IsRunning())
			gpsTimer->Stop();

		gpsInfos = "";
		filename = libelle;
		gpsInfosUpdate = false;
		processIdle = true;
		UpdateData();
	}
}


wxString CBitmapInfos::GenerateDefaultTimeStamp()
{
	wxFileName file = wxFileName(filename);
	wxDateTime dt = file.GetModificationTime();
	wxString str = dt.Format(wxT("%Y-%m-%d"), wxDateTime::CET);

	return str;
}

void CBitmapInfos::UpdateData()
{
	bool hasGps = false;
	bool hasDataTime = false;
	wxString latitudeGps;
	wxString longitudeGps;
	wxString dateTimeInfos;
    
	//printf("UpdateData \n");
	gpsInfos = "";

	CMetadataExiv2 metadata(filename);
	if(metadata.HasExif())
    {
		dateInfos = metadata.GetCreationDate();
        metadata.ReadPicture(hasGps, hasDataTime, dateTimeInfos, latitudeGps, longitudeGps);
    }
    
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

		if (dateInfos == "")
		{
			wxString str = GenerateDefaultTimeStamp();
			SetDateInfos(str, '-');
		}
		else
			SetDateInfos(dateInfos, '.');
	}
	else
	{
		if (dateInfos == "")
		{
			wxString str = GenerateDefaultTimeStamp();
			SetDateInfos(str, '-');
		}
		else
			SetDateInfos(dateInfos, '.');

		gpsInfos = "";

		
	}
    
    if(hasGps && gpsInfos == "")
    {
        gpsTimer->StartOnce(100);
    }
    
	needToRefresh = true;
}

int CBitmapInfos::Dayofweek(int d, int m, int y)
{
	static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	y -= m < 3;
	return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

void CBitmapInfos::SetDateInfos(const wxString& dataInfos, char seperator)
{
	wxString listMonth = CLibResource::LoadStringFromResource("LBLMONTHNAME", 1);
	wxString listDay = CLibResource::LoadStringFromResource("LBLDAYNAME", 1);
	vector<wxString> MonthName = CConvertUtility::split(listMonth, ',');
	vector<wxString> DayName = CConvertUtility::split(listDay, ',');
	vector<wxString> vDateTime = CConvertUtility::split(dataInfos, seperator);
	if (vDateTime.size() >= 3)
	{
		int year = atoi(vDateTime[0]);
		int month = atoi(vDateTime[1]);
		int day = atoi(vDateTime[2]);

		if (year == 0 || month == 0 || day == 0)
			dateInfos = "Invalid Date Infos";
		else
		{
			int ijour = Dayofweek(day, month, year);
			dateInfos = DayName.at(ijour) + L" " + to_string(day) + L" " + MonthName.at(month - 1) + L", " + to_string(year)
				+ L" ";
		}
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

void CBitmapInfos::DrawInformations(wxDC* dc)
{
	double scale_factor = 1.0f;
	wxRect rc = GetWindowRect();
	FillRect(dc, rc, bitmapInfosTheme.colorBack);
	wxString message;
	wxString libelle = CFileUtility::GetFileName(filename);
	CThemeFont font = bitmapInfosTheme.themeFont;
	font.SetFontSize(bitmapInfosTheme.themeFont.GetFontSize() / scale_factor);
	wxSize size = GetSizeTexte(dc, libelle, font);

	DrawTexte(dc, libelle, (GetWindowWidth() / scale_factor - size.x) / 2, 0, font);

	if (gpsInfos != L"")
	{
		wchar_t seperator = '.';
		vector<wxString> listGeo = CConvertUtility::split(gpsInfos, seperator);
		message = dateInfos;
		for (int i = listGeo.size() - 1;i >= 0 ; i--)
			message += L"," + listGeo.at(i);
	}
	else
	{
		message = dateInfos;
	}

	size = GetSizeTexte(dc, message, font);
	DrawTexte(dc, message, (GetWindowWidth() / scale_factor - size.x) / 2, (GetWindowHeight() / scale_factor) / 2,
	          font);
}

void CBitmapInfos::on_paint(wxPaintEvent& event)
{
	int width = GetWindowWidth();
	int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;

	wxBufferedPaintDC dc(this);
	DrawInformations(&dc);
}
