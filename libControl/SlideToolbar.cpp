#include <header.h>
#include "SlideToolbar.h"
#include <LibResource.h>
using namespace Regards::Video;

#define WM_EMAIL 4
#define WM_IMPRIMER 5
#define WM_CROP 6
#define IDM_SETSHRINK 7
#define WM_EXPORT 8
#define IDM_NONE 9
#ifndef WIN32
#define WM_CLOSE 10
#endif

CSlideToolbar::CSlideToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme)
	: CToolbarWindow(parent, id, theme, false)
{
	themeToolbar = theme;
	slide = nullptr;

	speaker = CreateButton("IDB_VOLUME_UP_VIDEO", "LBLSPEAKER", IDM_NONE, false);
	zoom = CreateButton("IDB_ZOOMPLUS", "LBLZOOMON", IDM_NONE, false);
	shrink = CreateButton("IDB_SHRINK", "LBLSHRINK", IDM_SETSHRINK, false);
	moins = CreateButton("IDB_MINUS", "LBLZOOMOFF", WM_ZOOMOUT, false);
	
	slide = std::make_unique<CToolbarSlide>(themeToolbar.slider, this);
	navElement.push_back(slide.get());

	plus = CreateButton("IDB_PLUS", "LBLZOOMON", WM_ZOOMON, false);
	cross = CreateButton("IDB_CANCEL", "LBLClose", WM_CLOSE, false);
}

void CSlideToolbar::SetSpeakerMode()
{
	id = SPEAKER_MODE;
	speaker->SetVisible(true);
	shrink->SetVisible(false);
	zoom->SetVisible(false);
}

void CSlideToolbar::SetZoomMode()
{
	id = ZOOM_MODE;
	speaker->SetVisible(false);
	shrink->SetVisible(true);
	zoom->SetVisible(true);
}

int CSlideToolbar::GetMode()
{
	return id;
}

void CSlideToolbar::ZoomPos(const int& position)
{
	wxCommandEvent evt(wxEVENT_ZOOMPOS);
	evt.SetInt(position);
	this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
}

void CSlideToolbar::ChangeZoomInPos(const int& position)
{
	positionTrackBar++;
	if (positionTrackBar >= slide->GetNbValue())
		positionTrackBar--;
	SetTrackBarPosition(positionTrackBar);
	ZoomPos(positionTrackBar);
}

void CSlideToolbar::ChangeZoomOutPos(const int& position)
{
	positionTrackBar--;
	if (positionTrackBar < 0)
		positionTrackBar = 0;
	SetTrackBarPosition(positionTrackBar);
	ZoomPos(positionTrackBar);
}


void CSlideToolbar::ZoomOn()
{
	ChangeZoomInPos(slide->GetPositionValue());
}


void CSlideToolbar::ZoomOut()
{
	ChangeZoomOutPos(slide->GetPositionValue());
}

void CSlideToolbar::SetTabValue(vector<int> value)
{
	if (slide != nullptr)
		slide->SetTabValue(value);
}


void CSlideToolbar::SetTrackBarPosition(const int& iPos)
{
	positionTrackBar = iPos;
	if (slide != nullptr)
	{
		slide->SetPosition(positionTrackBar);
		needToRefresh = true;
	}
}

void CSlideToolbar::SetComposantId(const int& id)
{
	this->id = id;
}

void CSlideToolbar::SlidePosChange(const int& position, const wxString& key)
{
	positionTrackBar = position;
	wxCommandEvent event(wxEVENT_ZOOMPOS);
	event.SetInt(position);
	this->GetParent()->GetEventHandler()->AddPendingEvent(event);
}

void CSlideToolbar::ClickButton(const int& id)
{
}

void CSlideToolbar::EventManager(const int& id)
{
	switch (id)
	{
	case WM_ZOOMOUT:
		ZoomOut();
		break;
	case WM_ZOOMON:
		ZoomOn();
		break;
	case IDM_SETSHRINK:
		{
			wxCommandEvent evt(wxEVENT_SHRINK);
			evt.SetInt(0);
			this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
			break;
		}

	case WM_CLOSE:
		{
			wxCommandEvent evt(wxEVENT_CLOSE);
			this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
			break;
		}
	default: ;
	}
}
