#include "ThumbnailToolBar.h"
#include <ToolbarSlide.h>
#include <libResource.h>
#include <effect.h>
#include "ListPicture.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Explorer;

#define WM_REFRESHTHUMBNAIL 1023
#define WM_EXPORT 1024

CThumbnailToolBar::CThumbnailToolBar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme)
	: CToolbarWindow(parent, id, theme)
{
	themeToolbar = theme;
    wxString refresh = CLibResource::LoadStringFromResource(L"LBLREFRESHTHUMBNAIL",1);//L"History";
	wxString delete_label = CLibResource::LoadStringFromResource(L"LBLDELETE",1);//L"History";
	wxString copy_label = CLibResource::LoadStringFromResource(L"LBLCOPY",1);//L"Effect";
	wxString export_label = CLibResource::LoadStringFromResource(L"LBLEXPORT", 1);//L"Effect";
    wxString zoomon = CLibResource::LoadStringFromResource(L"LBLZOOMON",1);
    wxString zoomoff = CLibResource::LoadStringFromResource(L"LBLZOOMOFF",1);
    
    CToolbarButton * refreshButton = new CToolbarButton(themeToolbar.button);
    refreshButton->SetButtonResourceId(L"IDB_FOLDER_REFRESH");
    refreshButton->SetCommandId(WM_REFRESHTHUMBNAIL);
    refreshButton->SetLibelleTooltip(refresh);
    navElement.push_back(refreshButton);
    

	CToolbarButton * exportButton = new CToolbarButton(themeToolbar.button);
	exportButton->SetButtonResourceId(L"IDB_EXPORT");
	exportButton->SetCommandId(WM_EXPORT);
	exportButton->SetLibelleTooltip(export_label);
	navElement.push_back(exportButton);


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
		this->Refresh();
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
		this->Refresh();
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
		this->Refresh();
	}
}

void CThumbnailToolBar::SlidePosChange(const int &position, const wxString &key)
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
		listPicture->ThumbnailZoomPosition(position);
}

void CThumbnailToolBar::ZoomPos(const int &position)
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
		listPicture->ThumbnailZoomPosition(position);
}

void CThumbnailToolBar::EventManager(const int &id)
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
	{
		switch (id)
		{
        case WM_REFRESHTHUMBNAIL:
            if (listPicture != nullptr)
                listPicture->RefreshThumbnail();
            break;
                
		case WM_ZOOMON:
			ZoomOn();
			if (listPicture != nullptr)
				listPicture->ThumbnailZoomOn();
			break;

		case WM_ZOOMOUT:
			ZoomOff();
			if (listPicture != nullptr)
				listPicture->ThumbnailZoomOff();
			break;

		case WM_EXPORT:
			if (listPicture != nullptr)
				listPicture->ExportFile();
			break;

		case WM_CLEAR:
			if (listPicture != nullptr)
				listPicture->DeleteFile();
			break;

		case WM_COPY:
			if (listPicture != nullptr)
				listPicture->CopyFile();
			break;

		}
	}
}
