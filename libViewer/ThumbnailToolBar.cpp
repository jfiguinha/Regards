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

CThumbnailToolBar::CThumbnailToolBar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	themeToolbar = theme;
	wxString index = CLibResource::LoadStringFromResource(L"LBLINDEXPHOTO", 1);
    wxString refresh = CLibResource::LoadStringFromResource(L"LBLREFRESHTHUMBNAIL",1);//L"History";
	wxString delete_label = CLibResource::LoadStringFromResource(L"LBLDELETE",1);//L"History";
	wxString copy_label = CLibResource::LoadStringFromResource(L"LBLCOPY",1);//L"Effect";
	wxString export_label = CLibResource::LoadStringFromResource(L"LBLEXPORT", 1);//L"Effect";
    wxString geo_label = CLibResource::LoadStringFromResource(L"LBLGEOLOCALISE", 1);
    wxString date_label = CLibResource::LoadStringFromResource(L"LBLCALENDAR", 1);
    wxString zoomon = CLibResource::LoadStringFromResource(L"LBLZOOMON",1);
    wxString zoomoff = CLibResource::LoadStringFromResource(L"LBLZOOMOFF",1);

	CToolbarButton * indexButton = new CToolbarButton(themeToolbar.button);
	indexButton->SetButtonResourceId(L"IDB_PHOTOINDEX");
	indexButton->SetCommandId(WM_INDEX);
	indexButton->SetLibelleTooltip(index);
	navElement.push_back(indexButton);
    
	CToolbarButton * exportButton = new CToolbarButton(themeToolbar.button);
	exportButton->SetButtonResourceId(L"IDB_EXPORT");
	exportButton->SetCommandId(WM_EXPORT);
	exportButton->SetLibelleTooltip(export_label);
	navElement.push_back(exportButton);

    CToolbarButton * dateButton = new CToolbarButton(themeToolbar.button);
    dateButton->SetButtonResourceId(L"IDB_CALENDAR");
    dateButton->SetCommandId(WM_CALENDAR);
    dateButton->SetLibelleTooltip(date_label);
    navElement.push_back(dateButton);

    CToolbarButton * geoButton = new CToolbarButton(themeToolbar.button);
    geoButton->SetButtonResourceId(L"IDB_MAPLOCATION");
    geoButton->SetCommandId(WM_GEOLOCALISE);
    geoButton->SetLibelleTooltip(geo_label);
    navElement.push_back(geoButton);

	CToolbarButton * deleteButton = new CToolbarButton(themeToolbar.button);
	deleteButton->SetButtonResourceId(L"IDB_DELETE");
	deleteButton->SetCommandId(WM_CLEAR);
	deleteButton->SetLibelleTooltip(delete_label);
	navElement.push_back(deleteButton);
    

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

CThumbnailToolBar::~CThumbnailToolBar()
{
}

void CThumbnailToolBar::ZoomOn()
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
	ForceRefresh();
#endif
	}
}

void CThumbnailToolBar::ZoomOff()
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
		ForceRefresh();
#endif
	}
}

void CThumbnailToolBar::SetTabValue(vector<int> value)
{
	if (slide != nullptr)
		slide->SetTabValue(value);
}

void CThumbnailToolBar::SetTrackBarPosition(const int &iPos)
{
	int positionTrackBar = iPos;
	if (slide != nullptr)
	{
		slide->SetPosition(positionTrackBar);
#ifdef __APPLE__
    this->CallRefresh(this);
#else
		ForceRefresh();
#endif
	}
}

void CThumbnailToolBar::SlidePosChange(const int &position, const wxString &key)
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
	{
		wxCommandEvent evt(wxEVENT_THUMBNAILZOOMPOSITION);
		evt.SetExtraLong(position);
		listPicture->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnailToolBar::ZoomPos(const int &position)
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
	{
		wxCommandEvent evt(wxEVENT_THUMBNAILZOOMPOSITION);
		evt.SetExtraLong(position);
		listPicture->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnailToolBar::EventManager(const int &id)
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
	{
		switch (id)
		{
               
		case WM_ZOOMON:
			ZoomOn();
			if (listPicture != nullptr)
			{
				wxCommandEvent evt(wxEVENT_THUMBNAILZOOMON);
				listPicture->GetEventHandler()->AddPendingEvent(evt);
			}
			break;

		case WM_ZOOMOUT:
			ZoomOff();
			if (listPicture != nullptr)
			{
				wxCommandEvent evt(wxEVENT_THUMBNAILZOOMOFF);
				listPicture->GetEventHandler()->AddPendingEvent(evt);
			}
			break;

		case WM_EXPORT:
			if (listPicture != nullptr)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_EXPORTFILE);
				listPicture->GetEventHandler()->AddPendingEvent(evt);
			}
			break;

		case WM_INDEX:
			if (listPicture != nullptr)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_GENERATEINDEXFILE);
				listPicture->GetEventHandler()->AddPendingEvent(evt);
			}
			break;

		case WM_CLEAR:
			if (listPicture != nullptr)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_DELETEFILE);
				listPicture->GetEventHandler()->AddPendingEvent(evt);
			}
			break;

		case WM_COPY:
			if (listPicture != nullptr)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_COPYFILE);
				listPicture->GetEventHandler()->AddPendingEvent(evt);
			}
			break;
        
        case WM_CALENDAR:
			if (listPicture != nullptr)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_CHANGEDATEFILE);
				listPicture->GetEventHandler()->AddPendingEvent(evt);
			}
            break;
                
        case WM_GEOLOCALISE:
			if (listPicture != nullptr)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_GEOLOCALIZEFILE);
				listPicture->GetEventHandler()->AddPendingEvent(evt);
			}
            break;

		}
	}
}
