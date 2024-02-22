#include <header.h>
#include "ThumbnailToolBarZoom.h"
#include <LibResource.h>
#include <window_id.h>
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include "MainWindow.h"
using namespace Regards::Viewer;

CThumbnailToolBarZoom::CThumbnailToolBarZoom(wxWindow* parent, wxWindowID id, const CThemeToolBarZoom& theme)
	: CWindowMain("CThumbnailToolBarZoom", parent, id)
{
	typeAffichage = 0;

	positionPrevious = wxRect(0, 0, 0, 0);
	positionNext = wxRect(0, 0, 0, 0);
	themeToolbarZoom = theme;
	criteriaPrevious = CLibResource::LoadStringFromResource(L"LBLCriteriaPrevious", 1);
	criteriaNext = CLibResource::LoadStringFromResource(L"LBLCriteriaNext", 1);
	allPhoto = CLibResource::LoadStringFromResource(L"LBLAllPhoto", 1);
	byYear = CLibResource::LoadStringFromResource(L"LBLByYear", 1);
	byMonth = CLibResource::LoadStringFromResource(L"LBLByMonth", 1);
	byDay = CLibResource::LoadStringFromResource(L"LBLByDay", 1);
	byLocalisation = CLibResource::LoadStringFromResource(L"LBLByLocalisation", 1);

	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
		typeAffichage = config->GetTypeAffichage();
	}

	Connect(wxEVT_PAINT, wxPaintEventHandler(CThumbnailToolBarZoom::on_paint));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CThumbnailToolBarZoom::OnLButtonDown));
}

int CThumbnailToolBarZoom::GetHeight()
{
	return themeToolbarZoom.GetHeight();
}


CThumbnailToolBarZoom::~CThumbnailToolBarZoom()
{
}

void CThumbnailToolBarZoom::OnLButtonDown(wxMouseEvent& event)
{
	bool update = false;
	int xPos = event.GetX();
	int yPos = event.GetY();

	if ((positionPrevious.x < xPos && xPos < (positionPrevious.x + positionPrevious.width)) && (positionPrevious.y <
		yPos && yPos < (positionPrevious.y + positionPrevious.height)))
	{
		//Draw Actif Previous
		switch (typeAffichage)
		{
		case 1: //By Year
		case 2: //By Month
		case 3: //By Day
		case 4: //By Localisation
			typeAffichage--;
			update = true;
			break;
		}
	}
	else if ((positionNext.x < xPos && xPos < (positionNext.x + positionNext.width)) && (positionNext.y < yPos && yPos <
		(positionNext.y + positionNext.height)))
	{
		//Draw Actif Previous
		switch (typeAffichage)
		{
		case 0: //All photo
		case 1: //By Year
		case 2: //By Month
		case 3: //By Day
			typeAffichage++;
			update = true;
			break;
		}
	}

	wxWindow* mainWindow = static_cast<CMainWindow*>(this->FindWindowById(CENTRALVIEWERWINDOWID));
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_CHANGETYPEAFFICHAGE);
		evt.SetExtraLong(typeAffichage);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
    
	needToRefresh = true;
}

void CThumbnailToolBarZoom::DrawPreviousElement(wxDC* dc, const wxString& libelle, const CThemeFont& themeFont)
{
	wxSize element = GetSizeTexte(dc, libelle, themeFont);

	int posY = (GetWindowHeight() - element.y) / 2;

	DrawTexte(dc, libelle, 0, posY, themeFont);

	positionPrevious.x = 0;
	positionPrevious.width = element.x;
	positionPrevious.y = 0;
	positionPrevious.height = GetWindowHeight();
}

void CThumbnailToolBarZoom::DrawNextElement(wxDC* dc, const wxString& libelle, const CThemeFont& themeFont)
{
	wxSize element = GetSizeTexte(dc, libelle, themeFont);
	int posY = (GetWindowHeight() - element.y) / 2;
	int posX = (GetWindowWidth() - element.x);

	DrawTexte(dc, libelle, posX, posY, themeFont);

	positionNext.x = GetWindowWidth() - element.x;
	positionNext.width = GetWindowWidth();
	positionNext.y = 0;
	positionNext.height = GetWindowHeight();
}

void CThumbnailToolBarZoom::DrawElement(wxDC* dc, const wxString& libelle, const CThemeFont& themeFont)
{
	wxSize element = GetSizeTexte(dc, libelle, themeFont);
	int posY = (GetWindowHeight() - element.y) / 2;
	int posX_center = (GetWindowWidth() - element.x) / 2;
	DrawTexte(dc, libelle, posX_center, posY, themeFont);
}

void CThumbnailToolBarZoom::UpdateScreenRatio()
{
	this->Resize();
}


void CThumbnailToolBarZoom::Draw(wxDC* dc)
{
	FillRect(dc, GetWindowRect(), themeToolbarZoom.colorBack);

	switch (typeAffichage)
	{
	case 0: //All photo
		DrawElement(dc, allPhoto, themeToolbarZoom.font);
		DrawNextElement(dc, byYear + criteriaNext, themeToolbarZoom.font);
		break;

	case 1: //By Year
		DrawPreviousElement(dc, allPhoto + criteriaPrevious, themeToolbarZoom.font);
		DrawElement(dc, byYear, themeToolbarZoom.font);
		DrawNextElement(dc, byMonth + criteriaNext, themeToolbarZoom.font);
		break;

	case 2: //By Month
		DrawPreviousElement(dc, byYear + criteriaPrevious, themeToolbarZoom.font);
		DrawElement(dc, byMonth, themeToolbarZoom.font);
		DrawNextElement(dc, byDay + criteriaNext, themeToolbarZoom.font);
		break;

	case 3: //By Day
		DrawPreviousElement(dc, byMonth + criteriaPrevious, themeToolbarZoom.font);
		DrawElement(dc, byDay, themeToolbarZoom.font);
		DrawNextElement(dc, byLocalisation + criteriaNext, themeToolbarZoom.font);
		break;

	case 4: //By Localisation
		DrawPreviousElement(dc, byDay + criteriaPrevious, themeToolbarZoom.font);
		DrawElement(dc, byLocalisation, themeToolbarZoom.font);
		break;
	}
}


void CThumbnailToolBarZoom::on_paint(wxPaintEvent& event)
{
	int width = GetWindowWidth();
	int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;

	wxPaintDC dc(this);
	Draw(&dc);
}
