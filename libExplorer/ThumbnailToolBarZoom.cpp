#include "ThumbnailToolBarZoom.h"
#include <LibResource.h>
#include "ListPicture.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Explorer;

CThumbnailToolBarZoom::CThumbnailToolBarZoom(wxWindow* parent, wxWindowID id, const CThemeToolBarZoom &theme)
	: CWindowMain(parent, id)
{
	positionPrevious = { 0, 0, 0, 0 };
	positionNext = { 0, 0, 0, 0 };
	themeToolbarZoom = theme;
	criteriaPrevious = CLibResource::LoadStringFromResource(L"LBLCriteriaPrevious", 1);
	criteriaNext = CLibResource::LoadStringFromResource(L"LBLCriteriaNext", 1);
	allPhoto = CLibResource::LoadStringFromResource(L"LBLAllPhoto", 1);
	byYear = CLibResource::LoadStringFromResource(L"LBLByYear", 1);
	byMonth = CLibResource::LoadStringFromResource(L"LBLByMonth", 1);
	byDay = CLibResource::LoadStringFromResource(L"LBLByDay", 1);
	byLocalisation = CLibResource::LoadStringFromResource(L"LBLByLocalisation", 1);

	Connect(wxEVT_PAINT, wxPaintEventHandler(CThumbnailToolBarZoom::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CThumbnailToolBarZoom::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CThumbnailToolBarZoom::OnLButtonDown));
}

int CThumbnailToolBarZoom::GetHeight()
{
	return themeToolbarZoom.GetHeight();
}


CThumbnailToolBarZoom::~CThumbnailToolBarZoom()
{
}


void CThumbnailToolBarZoom::Resize()
{
	this->Refresh();
}

void CThumbnailToolBarZoom::OnLButtonDown(wxMouseEvent& event)
{
	bool update = false;
	int xPos = event.GetX();
	int yPos = event.GetY();

	if ((positionPrevious.x < xPos && xPos < (positionPrevious.x + positionPrevious.width)) && (positionPrevious.y < yPos && yPos < (positionPrevious.y + positionPrevious.height)))
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
	else if ((positionNext.x < xPos && xPos < (positionNext.x + positionNext.width)) && (positionNext.y < yPos && yPos < (positionNext.y + positionNext.height)))
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

	if (update)
	{
		wxWindowDC winDC(this);
		CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
		if (listPicture != nullptr)
			listPicture->SetTypeOrderPhoto(typeAffichage);

		Draw(&winDC);

	}
}

void CThumbnailToolBarZoom::OnMouseMove(wxMouseEvent& event)
{
	wxWindowDC winDC(this);
	int xPos = event.GetX();
	int yPos = event.GetY();

	if ((positionPrevious.x < xPos && xPos < (positionPrevious.x + positionPrevious.width)) && (positionPrevious.y < yPos && yPos < (positionPrevious.y + positionPrevious.height)))
	{
		//Draw Actif Previous
		switch (typeAffichage)
		{
		case 1: //By Year
			DrawPreviousElement(&winDC, allPhoto + criteriaPrevious, themeToolbarZoom.fontActif);
			break;

		case 2: //By Month
			DrawPreviousElement(&winDC, byYear + criteriaPrevious, themeToolbarZoom.fontActif);
			break;

		case 3: //By Day
			DrawPreviousElement(&winDC, byMonth + criteriaPrevious, themeToolbarZoom.fontActif);
			break;

		case 4: //By Localisation
			DrawPreviousElement(&winDC, byDay + criteriaPrevious, themeToolbarZoom.fontActif);
			break;
		}
	}
	else if ((positionNext.x < xPos && xPos < (positionNext.width + positionNext.x)) && (positionNext.y < yPos && yPos < (positionNext.y + positionNext.height)))
	{
		//Draw Actif Previous
		switch (typeAffichage)
		{
		case 0: //All photo
			DrawNextElement(&winDC, byYear + criteriaNext, themeToolbarZoom.fontActif);
			break;

		case 1: //By Year
			DrawNextElement(&winDC, byMonth + criteriaNext, themeToolbarZoom.fontActif);
			break;

		case 2: //By Month
			DrawNextElement(&winDC, byDay + criteriaNext, themeToolbarZoom.fontActif);
			break;

		case 3: //By Day
			DrawNextElement(&winDC, byLocalisation + criteriaNext, themeToolbarZoom.fontActif);
			break;
		}
	}
	else
	{
		switch (typeAffichage)
		{
		case 0: //All photo
			DrawElement(&winDC, allPhoto, themeToolbarZoom.font);
			DrawNextElement(&winDC, byYear + criteriaNext, themeToolbarZoom.font);
			break;

		case 1: //By Year
			DrawPreviousElement(&winDC, allPhoto + criteriaPrevious, themeToolbarZoom.font);
			DrawElement(&winDC, byYear, themeToolbarZoom.font);
			DrawNextElement(&winDC, byMonth + criteriaNext, themeToolbarZoom.font);
			break;

		case 2: //By Month
			DrawPreviousElement(&winDC, byYear + criteriaPrevious, themeToolbarZoom.font);
			DrawElement(&winDC, byMonth, themeToolbarZoom.font);
			DrawNextElement(&winDC, byDay + criteriaNext, themeToolbarZoom.font);
			break;

		case 3: //By Day
			DrawPreviousElement(&winDC, byMonth + criteriaPrevious, themeToolbarZoom.font);
			DrawElement(&winDC, byDay, themeToolbarZoom.font);
			DrawNextElement(&winDC, byLocalisation + criteriaNext, themeToolbarZoom.font);
			break;

		case 4: //By Localisation
			DrawPreviousElement(&winDC, byDay + criteriaPrevious, themeToolbarZoom.font);
			DrawElement(&winDC, byLocalisation, themeToolbarZoom.font);
			break;
		}
	}
}

void CThumbnailToolBarZoom::DrawPreviousElement(wxDC * dc, const wxString &libelle, const CThemeFont &themeFont)
{
	wxSize element = GetSizeTexte(dc, libelle, themeFont);

	int posY = (height - element.y) / 2;

	DrawTexte(dc, libelle, 0, posY, themeFont);

	positionPrevious.x = 0;
	positionPrevious.width = element.x;
	positionPrevious.y = 0;
	positionPrevious.height = height;
}

void CThumbnailToolBarZoom::DrawNextElement(wxDC * dc, const wxString &libelle, const CThemeFont &themeFont)
{
	wxSize element = GetSizeTexte(dc, libelle, themeFont);
	int posY = (height - element.y) / 2;
	int posX = (width - element.x);

	DrawTexte(dc, libelle, posX, posY, themeFont);

	positionNext.x = width - element.x;
	positionNext.width = width;
	positionNext.y = 0;
	positionNext.height = height;
}

void CThumbnailToolBarZoom::DrawElement(wxDC * dc, const wxString &libelle, const CThemeFont &themeFont)
{
	wxSize element = GetSizeTexte(dc, libelle, themeFont);
	int posY = (height - element.y) / 2;
	int posX_center = (width - element.x) / 2;
	DrawTexte(dc, libelle, posX_center, posY, themeFont);
}

void CThumbnailToolBarZoom::UpdateScreenRatio()
{
    this->Resize();
}


void CThumbnailToolBarZoom::Draw(wxDC * dc)
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


void CThumbnailToolBarZoom::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	Draw(&dc);
}