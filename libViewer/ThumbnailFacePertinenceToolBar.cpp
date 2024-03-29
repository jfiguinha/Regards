#include <header.h>
#ifndef __NOFACE_DETECTION__
#include "ThumbnailFacePertinenceToolBar.h"
#include <ToolbarSlide.h>
#include <ToolbarTexte.h>
#include <LibResource.h>
#include "ListFace.h"
#include <window_id.h>
#include "ViewerParamInit.h"
#include "ViewerParam.h"
using namespace Regards::Viewer;

#define WM_REFRESHTHUMBNAIL 1023
#define WM_EXPORT 1024
#define WM_CALENDAR 1025
#define WM_GEOLOCALISE 1026

CThumbnailFacePertinenceToolBar::CThumbnailFacePertinenceToolBar(wxWindow* parent, wxWindowID id,
                                                                 const CThemeToolbar& theme, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	themeToolbar = theme;
	wxString pertinence = CLibResource::LoadStringFromResource(L"LBLPERTINENCELEVEL", 1); //L"History";
	wxString zoomon = CLibResource::LoadStringFromResource(L"LBLZOOMON", 1);
	wxString zoomoff = CLibResource::LoadStringFromResource(L"LBLZOOMOFF", 1);


	auto toolbarText = new CToolbarTexte(themeToolbar.texte);
	toolbarText->SetLibelle(pertinence);
	toolbarText->SetInactif();
	toolbarText->SetLibelleTooltip(pertinence);
	navElement.push_back(toolbarText);

	auto moins = new CToolbarButton(themeToolbar.button);
	moins->SetButtonResourceId(L"IDB_MINUS");
	moins->SetCommandId(WM_ZOOMOUT);
	moins->SetLibelleTooltip(zoomoff);
	navElement.push_back(moins);

	//themeToolbar.slider.colorBack.Set(51, 54, 62);
	slide = new CToolbarSlide(themeToolbar.slider, this);
	navElement.push_back(slide);

	auto plus = new CToolbarButton(themeToolbar.button);
	plus->SetButtonResourceId(L"IDB_PLUS");
	plus->SetCommandId(WM_ZOOMON);
	plus->SetLibelleTooltip(zoomon);
	navElement.push_back(plus);
}

CThumbnailFacePertinenceToolBar::~CThumbnailFacePertinenceToolBar()
{
}

void CThumbnailFacePertinenceToolBar::ZoomOn()
{
	//OnChangeValue();
	if (slide != nullptr)
	{
		int dwPos = slide->GetPosition();
		dwPos++;
		if (dwPos >= slide->GetNbValue())
			dwPos = slide->GetNbValue() - 1;
		else
		{
			SetTrackBarPosition(dwPos);
			needToRefresh = true;
			OnChangeValue();
		}
	}
}

void CThumbnailFacePertinenceToolBar::ZoomOff()
{
	//OnChangeValue();
	if (slide != nullptr)
	{
		int dwPos = slide->GetPosition();
		dwPos--;
		if (dwPos < 0)
			dwPos = 0;
		else
		{
			SetTrackBarPosition(dwPos);
			needToRefresh = true;
			OnChangeValue();
		}
	}
}

void CThumbnailFacePertinenceToolBar::SetTabValue(vector<int> value)
{
	if (slide != nullptr)
		slide->SetTabValue(value);
}

void CThumbnailFacePertinenceToolBar::SetTrackBarPosition(const int& iPos)
{
	int positionTrackBar = iPos;
	if (slide != nullptr)
	{
		slide->SetPosition(positionTrackBar);
		needToRefresh = true;
	}
}

void CThumbnailFacePertinenceToolBar::SlidePosChange(const int& position, const wxString& key)
{
	OnChangeValue();
}

void CThumbnailFacePertinenceToolBar::OnChangeValue()
{
	CMainParam* viewerParam = CMainParamInit::getInstance();
	if (viewerParam != nullptr)
	{
		viewerParam->SetPertinenceValue(slide->GetPositionValue());
	}

	wxWindow* mainWindow = this->FindWindowById(LISTFACEID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnailFacePertinenceToolBar::ZoomPos(const int& position)
{
	OnChangeValue();
}

void CThumbnailFacePertinenceToolBar::EventManager(const int& id)
{
	switch (id)
	{
	case WM_ZOOMON:
		ZoomOn();
		break;

	case WM_ZOOMOUT:
		ZoomOff();
		break;
	}
}
#endif
