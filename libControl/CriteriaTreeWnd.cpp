#include <header.h>
#include "CriteriaTreeWnd.h"
#include "CalendarDialogBox.h"
#include "MapDialogBox.h"
#include <RegardsConfigParam.h>
#include <TreeWindow.h>
#include "CriteriaTree.h"
#include "KeywordDialogBox.h"
#if defined(LIBAPPLE)
#include <AppleReadExif.h>
#endif
#if defined(EXIV2)
#include <MetadataExiv2.h>
#endif
#include <LibResource.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <SqlCriteria.h>
#include <SqlPhotoCriteria.h>
#include <SqlGps.h>
#include <PhotoCriteria.h>
#include <SqlPhotos.h>
#include <MapSelect.h>
#include <CalendarSelect.h>
#include <ParamInit.h>
#include <GpsEngine.h>
using namespace Regards::exiv2;
using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Internet;
using namespace Regards::Control;
#define TAILLEMAX 4096

wxDEFINE_EVENT(EVENT_UPDATEINFOSTHREAD, wxCommandEvent);

CCriteriaTreeWnd::CCriteriaTreeWnd(wxWindow* parent, wxWindowID id, const int& mainWindowID,
                                   const CThemeTree& theme, const CThemeScrollBar& themeScroll)
	: CTreeWithScrollbar("CCriteriaTreeWnd", parent, id, themeScroll, theme)
{
	wxString urlServer = "";
	//Géolocalisation
	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetUrlServer();
	}

	criteriaTree = nullptr;
	oldCriteriaTree = nullptr;
	numPhotoId = 0;
	this->mainWindowID = mainWindowID;
	fileGeolocalisation = new CFileGeolocation(urlServer);


	Connect(wxEVT_SHOWCALENDAR, wxCommandEventHandler(CCriteriaTreeWnd::ShowCalendar));
	Connect(wxEVT_SHOWMAP, wxCommandEventHandler(CCriteriaTreeWnd::ShowMap));
	Connect(wxEVT_SHOWKEYWORD, wxCommandEventHandler(CCriteriaTreeWnd::ShowKeyWord));
}

wxString CCriteriaTreeWnd::GenerateUrl()
{
	if (fileGeolocalisation->HasGps())
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

void CCriteriaTreeWnd::ShowCalendar(wxCommandEvent& event)
{
	auto photoCriteria = static_cast<CPhotoCriteria*>(event.GetClientData());
	CCalendarSelect calendarSelect;

	if (calendarSelect.SelectNewDate(this, fileGeolocalisation->GetDateTimeInfos()))
	{
		bool isNew = false;
		CSqlCriteria sqlCriteria;
		CSqlPhotoCriteria sqlPhotoCriteria;
		int numCriteriaId = sqlCriteria.GetOrInsertCriteriaId(1, 3, calendarSelect.GetSelectStringDate(), isNew);
		sqlPhotoCriteria.InsertPhotoCriteria(numPhotoId, numCriteriaId);
		if (photoCriteria != nullptr && numCriteriaId != photoCriteria->GetCriteriaId())
			sqlPhotoCriteria.DeletePhotoCriteria(numPhotoId, photoCriteria->GetCriteriaId());
#if defined(LIBAPPLE)
        CAppleReadExif appleReadExif;
        appleReadExif.WriteDateTime(filename, calendarSelect.GetSelectDate());
#else
		CMetadataExiv2 metadata(filename);
		metadata.SetDateTime(calendarSelect.GetSelectStringDate());

#endif

		wxWindow* mainWnd = this->FindWindowById(mainWindowID);
		auto eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
		wxQueueEvent(mainWnd, eventChange);
	}

	CListOfWindow* fileGeolocalisation = CGpsEngine::getInstance();
	fileGeolocalisation->SendMessageToWindow(filename, 2);

	if (photoCriteria != nullptr)
		delete photoCriteria;

	Refresh();
}

void CCriteriaTreeWnd::ShowMap(wxCommandEvent& event)
{
	CMapSelect mapSelect;
	wxString url = GenerateUrl();
	auto photoCriteria = static_cast<CPhotoCriteria*>(event.GetClientData());
	wxString infoGpsLocalisation = mapSelect.SelectNewMapLocalisation(this, url);

	if (infoGpsLocalisation != "")
	{
		bool isNew = false;
		CSqlCriteria sqlCriteria;
		CSqlGps sqlGps;
		CSqlPhotoCriteria sqlPhotoCriteria;
		int numCriteriaId = sqlCriteria.GetOrInsertCriteriaId(1, 1, infoGpsLocalisation, isNew);
		sqlPhotoCriteria.InsertPhotoCriteria(numPhotoId, numCriteriaId);
		if (photoCriteria != nullptr && numCriteriaId != photoCriteria->GetCriteriaId())
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
        {
            CAppleReadExif appleReadExif;
            appleReadExif.WriteGps(filename, dlat, dlong);
        }
#else
		{
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
		}
#endif

		wxCommandEvent evt(wxEVENT_UPDATEURL);
		this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
	}

	if (photoCriteria != nullptr)
		delete photoCriteria;

	//Update infos
	CListOfWindow* fileGeolocalisation = CGpsEngine::getInstance();
	fileGeolocalisation->SendMessageToWindow(filename, 1);

	Refresh();
}


void CCriteriaTreeWnd::ShowKeyWord(wxCommandEvent& event)
{
	auto photoCriteria = static_cast<CPhotoCriteria*>(event.GetClientData());
	KeywordDialogBox keywordDialog(this);
	keywordDialog.ShowModal();
	bool isOk = false;
	if (isOk)
	{
		wxWindow* mainWnd = this->FindWindowById(mainWindowID);
		auto eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
		wxQueueEvent(mainWnd, eventChange);
	}

	if (photoCriteria != nullptr)
		delete photoCriteria;

	Refresh();
}

CCriteriaTreeWnd::~CCriteriaTreeWnd(void)
{
	if (oldCriteriaTree != nullptr)
		delete(oldCriteriaTree);
}

void CCriteriaTreeWnd::UpdateTreeData()
{
	auto criteriaTree = new CCriteriaTree(treeWindow->GetTheme(), treeWindow);
	criteriaTree->SetFile(filename, numPhotoId);
	//criteriaTree->CreateElement();
	treeWindow->SetTreeControl(criteriaTree);
	if (oldCriteriaTree != nullptr)
		delete(oldCriteriaTree);
	oldCriteriaTree = criteriaTree;
}

void CCriteriaTreeWnd::SetFile(const wxString& filename)
{
	if (this->filename != filename)
	{
		wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
		fileGeolocalisation->SetFile(this->filename, notGeo);
		CSqlPhotos sqlPhotos;
		numPhotoId = sqlPhotos.GetPhotoId(filename);
		this->filename = filename;
		UpdateTreeData();

		Refresh();
	}
}
