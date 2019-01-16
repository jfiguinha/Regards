#include "ThumbnailFaceToolBar.h"
#include <ToolbarSlide.h>
#include <LibResource.h>
#include "ListFace.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Viewer;

#define WM_REFRESHTHUMBNAIL 1023
#define WM_EXPORT 1024
#define WM_CALENDAR 1025
#define WM_GEOLOCALISE 1026

CThumbnailFaceToolBar::CThumbnailFaceToolBar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme)
	: CToolbarWindow(parent, id, theme)
{
	themeToolbar = theme;
    wxString refresh = CLibResource::LoadStringFromResource(L"LBLREFRESHTHUMBNAIL",1);//L"History";
    wxString zoomon = CLibResource::LoadStringFromResource(L"LBLZOOMON",1);
    wxString zoomoff = CLibResource::LoadStringFromResource(L"LBLZOOMOFF",1);
    wxString copy_label = CLibResource::LoadStringFromResource(L"LBLCOPY",1);

	CToolbarButton * copy = new CToolbarButton(themeToolbar.button);
	copy->SetButtonResourceId(L"IDB_MULTIPLESELECT");
	copy->SetCommandId(WM_COPY);
	copy->SetLibelleTooltip(copy_label);
	navElement.push_back(copy);

	CToolbarButton * moins = new CToolbarButton(themeToolbar.button);
	moins->SetButtonResourceId(L"IDB_ZOOMMOINS");
	moins->SetCommandId(WM_ZOOMOUT);
    moins->SetLibelleTooltip(zoomoff);
	navElement.push_back(moins);

	slide = new CToolbarSlide(themeToolbar.slider, this);
	navElement.push_back(slide);

	CToolbarButton * plus = new CToolbarButton(themeToolbar.button);
	plus->SetButtonResourceId(L"IDB_ZOOMPLUS");
	plus->SetCommandId(WM_ZOOMON);
    plus->SetLibelleTooltip(zoomon);
	navElement.push_back(plus);

}

CThumbnailFaceToolBar::~CThumbnailFaceToolBar()
{
}

void CThumbnailFaceToolBar::ZoomOn()
{
	if (slide != nullptr)
	{
		int dwPos = slide->GetPosition();
		dwPos++;
		if (dwPos >= slide->GetNbValue())
			dwPos = slide->GetNbValue() - 1;
		SetTrackBarPosition(dwPos);
#ifdef __APPLE__
    this->CallRefresh(this);
#else
	this->FastRefresh(this);
#endif
	}
}

void CThumbnailFaceToolBar::ZoomOff()
{
	if (slide != nullptr)
	{
		int dwPos = slide->GetPosition();
		dwPos--;
		if (dwPos < 0)
			dwPos = 0;
		SetTrackBarPosition(dwPos);
#ifdef __APPLE__
    this->CallRefresh(this);
#else
	this->FastRefresh(this);
#endif
	}
}

void CThumbnailFaceToolBar::SetTabValue(vector<int> value)
{
	if (slide != nullptr)
		slide->SetTabValue(value);
}

void CThumbnailFaceToolBar::SetTrackBarPosition(const int &iPos)
{
	int positionTrackBar = iPos;
	if (slide != nullptr)
	{
		slide->SetPosition(positionTrackBar);
#ifdef __APPLE__
    this->CallRefresh(this);
#else
	this->FastRefresh(this);
#endif
	}
}

void CThumbnailFaceToolBar::SlidePosChange(const int &position, const wxString &key)
{
	CListFace * listFace = (CListFace *)this->FindWindowById(LISTFACEID);
	if (listFace != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_THUMBNAILZOOMPOSITION);
		evt.SetExtraLong(position);
		listFace->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnailFaceToolBar::ZoomPos(const int &position)
{
	CListFace * listFace = (CListFace *)this->FindWindowById(LISTFACEID);
	if (listFace != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_THUMBNAILZOOMPOSITION);
		evt.SetExtraLong(position);
		listFace->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnailFaceToolBar::EventManager(const int &id)
{
	CListFace * listFace = (CListFace *)this->FindWindowById(LISTFACEID);
	if (listFace != nullptr)
	{
		switch (id)
		{
		case WM_COPY:
			{
				if (listFace != nullptr)
				{
					wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_THUMBNAILMOVE);
					listFace->GetEventHandler()->AddPendingEvent(evt);
				}
			}
			break;
               
		case WM_ZOOMON:
			ZoomOn();
			if (listFace != nullptr)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_THUMBNAILZOOMON);
				listFace->GetEventHandler()->AddPendingEvent(evt);
			}
			break;

		case WM_ZOOMOUT:
			ZoomOff();
			if (listFace != nullptr)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_THUMBNAILZOOMOFF);
				listFace->GetEventHandler()->AddPendingEvent(evt);
			}
			break;

		}
	}
}
