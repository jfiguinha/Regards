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
#define WM_CLOSE 10


CSlideToolbar::CSlideToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme)
	: CToolbarWindow(parent, id, theme, false)
{
	themeToolbar = theme;
	slide = nullptr;
	wxString shrinkLibelle = CLibResource::LoadStringFromResource("LBLSHRINK", 1);
	wxString speakerLibelle = CLibResource::LoadStringFromResource("LBLSPEAKER", 1);
	wxString zoomLibelle = CLibResource::LoadStringFromResource("LBLZOOMON", 1);
	wxString zoomOn = CLibResource::LoadStringFromResource("LBLZOOMON", 1); // "Zoom On";
	wxString zoomOff = CLibResource::LoadStringFromResource("LBLZOOMOFF", 1); // "Zoom Off";
	wxString close = CLibResource::LoadStringFromResource("LBLClose", 1); // "Zoom Off";

	speaker = new CToolbarButton(themeToolbar.button);
	speaker->SetButtonResourceId("IDB_VOLUME_UP_VIDEO");
	speaker->SetCommandId(IDM_NONE);
	speaker->SetLibelleTooltip(speakerLibelle);
	speaker->SetVisible(false);
	navElement.push_back(speaker);

	zoom = new CToolbarButton(themeToolbar.button);
	zoom->SetButtonResourceId("IDB_ZOOMPLUS");
	zoom->SetCommandId(IDM_NONE);
	zoom->SetLibelleTooltip(zoomLibelle);
	zoom->SetVisible(false);
	navElement.push_back(zoom);

	shrink = new CToolbarButton(themeToolbar.button);
	shrink->SetButtonResourceId("IDB_SHRINK");
	shrink->SetCommandId(IDM_SETSHRINK);
	shrink->SetLibelleTooltip(shrinkLibelle);
	shrink->SetVisible(false);
	navElement.push_back(shrink);

	auto moins = new CToolbarButton(themeToolbar.button);
	moins->SetButtonResourceId("IDB_MINUS");
	moins->SetCommandId(WM_ZOOMOUT);
	moins->SetLibelleTooltip(zoomOff);
	navElement.push_back(moins);

	slide = new CToolbarSlide(themeToolbar.slider, this);
	navElement.push_back(slide);

	auto plus = new CToolbarButton(themeToolbar.button);
	plus->SetButtonResourceId("IDB_PLUS");
	plus->SetCommandId(WM_ZOOMON);
	plus->SetLibelleTooltip(zoomOn);
	navElement.push_back(plus);

	auto cross = new CToolbarButton(themeToolbar.button);
	cross->SetButtonResourceId("IDB_CANCEL");
	cross->SetCommandId(WM_CLOSE);
	cross->SetLibelleTooltip(close);
	navElement.push_back(cross);
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

CSlideToolbar::~CSlideToolbar()
{
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
		ForceRefresh();
		//this->Update();
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
