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
#include <TreeWindow.h>
#include <ScrollbarWnd.h>
#include "PhotoCriteriaUtility.h"
using namespace Regards::exiv2;
using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Internet;
using namespace Regards::Control;

wxDEFINE_EVENT(EVENT_UPDATEINFOSTHREAD, wxCommandEvent);

CCriteriaTreeWnd::CCriteriaTreeWnd(wxWindow* parent,
	wxWindowID id,
	wxWindowID mainWindowID,
	const CThemeTree& theme,
	const CThemeScrollBar& themeScroll)
	: CTreeWithScrollbar("CCriteriaTreeWnd", parent, id, themeScroll, theme),
	mainWindowID(mainWindowID)
{
	wxString urlServer;
	wxString apiKey;

	// Géolocalisation
	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetGeoLocUrlServer();
		apiKey = param->GetApiKey();
	}

	fileGeolocalisation = std::make_unique<CFileGeolocation>(urlServer, apiKey);

	Connect(wxEVT_SHOWCALENDAR, wxCommandEventHandler(CCriteriaTreeWnd::ShowCalendar));
	Connect(wxEVT_SHOWMAP, wxCommandEventHandler(CCriteriaTreeWnd::ShowMap));
	Connect(wxEVT_SHOWKEYWORD, wxCommandEventHandler(CCriteriaTreeWnd::ShowKeyWord));
}

wxString CCriteriaTreeWnd::GenerateUrl() const
{
	if (fileGeolocalisation == nullptr || !fileGeolocalisation->HasGps())
		return CLibResource::LoadStringFromResource("openstreetmapurl", 1);

	const wxString latitude = fileGeolocalisation->GetLatitude();
	const wxString longitude = fileGeolocalisation->GetLongitude();

	return wxString::Format(
		"https://www.openstreetmap.org/?mlat=%s&mlon=%s#map=15/%s/%s",
		latitude,
		longitude,
		latitude,
		longitude);
}

void CCriteriaTreeWnd::NotifyCriteriaChanged() const
{
	wxWindow* mainWnd = FindWindowById(mainWindowID);
	if (mainWnd == nullptr)
		return;

	wxQueueEvent(mainWnd, new wxCommandEvent(wxEVT_CRITERIACHANGE));
}

void CCriteriaTreeWnd::NotifyUrlChanged() const
{
	wxWindow* parent = GetParent();
	if (parent == nullptr)
		return;

	wxQueueEvent(parent, new wxCommandEvent(wxEVENT_UPDATEURL));
}

void CCriteriaTreeWnd::UpdateGpsWindow(const int messageId) const
{
	CListOfWindow* gpsEngine = CGpsEngine::getInstance();
	if (gpsEngine != nullptr && !filename.IsEmpty())
		gpsEngine->SendMessageToWindow(filename, messageId);
}

void CCriteriaTreeWnd::ReleasePhotoCriteria(wxCommandEvent& event) const
{
	auto* photoCriteria = static_cast<CPhotoCriteria*>(event.GetClientData());
	if (photoCriteria != nullptr)
	{
		delete photoCriteria;
		event.SetClientData(nullptr);
	}
}

void CCriteriaTreeWnd::ShowCalendar(wxCommandEvent& event)
{
	auto* photoCriteria = static_cast<CPhotoCriteria*>(event.GetClientData());
	CCalendarSelect calendarSelect;

	bool modified = false;

	if (calendarSelect.SelectNewDate(this,
		fileGeolocalisation != nullptr
		? fileGeolocalisation->GetDateTimeInfos()
		: wxDateTime().Now().Format(wxT("%d-%m-%y__%H-%M-%S"), wxDateTime::CET)))
	{

		CPhotoCriteriaUtility::ChangeDateFile(filename, calendarSelect.GetSelectDate(), calendarSelect.GetSelectStringDate(), photoCriteria->GetCriteriaId());

		NotifyCriteriaChanged();
		modified = true;
	}

	ReleasePhotoCriteria(event);

	if (modified)
	{
		UpdateGpsWindow(2);
		needToRefresh = true;
	}
}

void CCriteriaTreeWnd::ShowMap(wxCommandEvent& event)
{
	CMapSelect mapSelect;
	const wxString url = GenerateUrl();
	auto* photoCriteria = static_cast<CPhotoCriteria*>(event.GetClientData());

	const wxString infoGpsLocalisation = mapSelect.SelectNewMapLocalisation(this, url);
	bool modified = false;

	if (!infoGpsLocalisation.IsEmpty())
	{
		const wxString latitude = mapSelect.GetLatitude();
		const wxString longitude = mapSelect.GetLongitude();

		CPhotoCriteriaUtility::GeolocalizeFile(filename, latitude, longitude, infoGpsLocalisation, photoCriteria->GetCriteriaId());

		NotifyUrlChanged();
		modified = true;
	}

	ReleasePhotoCriteria(event);

	if (modified)
	{
		UpdateGpsWindow(1);
		needToRefresh = true;
	}
}

void CCriteriaTreeWnd::ShowKeyWord(wxCommandEvent& event)
{
	KeywordDialogBox keywordDialog(this);
	const bool isOk = keywordDialog.ShowModal() == wxID_OK;

	ReleasePhotoCriteria(event);

	if (isOk)
	{
		NotifyCriteriaChanged();
		needToRefresh = true;
	}
}

void CCriteriaTreeWnd::UpdateTreeData()
{
	if (treeWindow == nullptr)
		return;

	auto criteriaTree = std::make_unique<CCriteriaTree>(
		treeWindow->GetTheme(),
		treeWindow);

	criteriaTree->SetFile(filename, numPhotoId);

	// treeWindow ne devient pas propriétaire de criteriaTree.
	// oldCriteriaTree conserve donc la propriété de l'objet.
	treeWindow->SetTreeControl(criteriaTree.get());
	oldCriteriaTree = std::move(criteriaTree);
}

void CCriteriaTreeWnd::SetFile(const wxString& newFilename)
{
	if (filename == newFilename)
		return;

	filename = newFilename;

	if (fileGeolocalisation != nullptr)
	{
		const wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
		fileGeolocalisation->SetFile(filename, notGeo);
	}

	CSqlPhotos sqlPhotos;
	numPhotoId = sqlPhotos.GetPhotoId(filename);

	UpdateTreeData();
	needToRefresh = true;
}
