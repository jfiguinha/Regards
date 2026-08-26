#include <header.h>
#include "ThumbnailToolBar.h"
#include <ToolbarSlide.h>
#include <LibResource.h>
#include "ListPicture.h"
#include <window_id.h>
using namespace Regards::Viewer;

#define WM_REFRESHTHUMBNAIL 1023
#define WM_EXPORT 1024
#define WM_CALENDAR 1025
#define WM_GEOLOCALISE 1026
#define WM_INDEX 1027

CThumbnailToolBar::CThumbnailToolBar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	themeToolbar = theme;


	indexButton = CreateButton("IDB_PHOTOINDEX", "LBLINDEXPHOTO", WM_INDEX, false);
	exportButton = CreateButton("IDB_EXPORT", "LBLEXPORT", WM_EXPORT, false);
	dateButton = CreateButton("IDB_CALENDAR", "LBLCALENDAR", WM_CALENDAR, false);
	geoButton = CreateButton("IDB_MAPLOCATION", "LBLGEOLOCALISE", WM_GEOLOCALISE, false);
	deleteButton = CreateButton("IDB_DELETE", "LBLDELETE", WM_CLEAR, false);
	copy = CreateButton("IDB_MULTIPLESELECT", "LBLCOPY", WM_COPY, false);
	moins= CreateButton("IDB_ZOOMMOINS", "LBLZOOMOFF", WM_ZOOMOUT, false);
	plus = CreateButton("IDB_ZOOMPLUS", "LBLZOOMON", WM_ZOOMON, false);


	navElement.push_back(indexButton.get());
	navElement.push_back(exportButton.get());
	navElement.push_back(dateButton.get());
	navElement.push_back(geoButton.get());
	navElement.push_back(deleteButton.get());
	navElement.push_back(copy.get());
	navElement.push_back(moins.get());

	slide = std::make_unique<CToolbarSlide>(themeToolbar.slider, this);
	navElement.push_back(slide.get());

	navElement.push_back(plus.get());
}

void CThumbnailToolBar::ZoomOn()
{
	Zoom(+1);
}

void CThumbnailToolBar::ZoomOff()
{
	Zoom(-1);
}

void CThumbnailToolBar::SetTabValue(vector<int> value)
{
	if (slide != nullptr)
		slide->SetTabValue(value);
}

void CThumbnailToolBar::SetTrackBarPosition(const int& iPos)
{
	int positionTrackBar = iPos;
	if (slide != nullptr)
	{
		slide->SetPosition(positionTrackBar);
		needToRefresh = true;
	}
}

void CThumbnailToolBar::SendZoomPosition(int position)
{
	if (auto* list = GetListPicture())
	{
		wxCommandEvent evt(wxEVENT_THUMBNAILZOOMPOSITION);
		evt.SetExtraLong(position);
		list->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnailToolBar::SlidePosChange(const int& position, const wxString& key)
{
	SendZoomPosition(position);
}

void CThumbnailToolBar::ZoomPos(const int& position)
{
	SendZoomPosition(position);
}

CListPicture* CThumbnailToolBar::GetListPicture()
{
	return static_cast<CListPicture*>(FindWindowById(LISTPICTUREID));
}

void CThumbnailToolBar::PostEvent(wxEventType type)
{
	if (auto* list = GetListPicture())
	{
		wxCommandEvent evt(type);
		list->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnailToolBar::Zoom(int delta)
{
	if (!slide)
		return;

	const int position = std::clamp(
		slide->GetPosition() + delta,
		0,
		slide->GetNbValue() - 1);

	if (position != slide->GetPosition())
	{
		SetTrackBarPosition(position);
		needToRefresh = true;
	}
}

void CThumbnailToolBar::EventManager(const int& id)
{
		switch (id)
		{
		case WM_ZOOMON:
			Zoom(+1);
			PostEvent(wxEVENT_THUMBNAILZOOMON);
			break;

		case WM_ZOOMOUT:
			Zoom(-1);
			PostEvent(wxEVENT_THUMBNAILZOOMOFF);
			break;

		case WM_EXPORT:
			PostEvent(wxEVENT_EXPORTFILE);
			break;

		case WM_INDEX:
			PostEvent(wxEVENT_GENERATEINDEXFILE);
			break;

		case WM_CLEAR:
			PostEvent(wxEVENT_DELETEFILE);
			break;

		case WM_COPY:
			PostEvent(wxEVENT_COPYFILE);
			break;

		case WM_CALENDAR:
			PostEvent(wxEVENT_CHANGEDATEFILE);
			break;

		case WM_GEOLOCALISE:
			PostEvent(wxEVENT_GEOLOCALIZEFILE);
			break;
		}
}
