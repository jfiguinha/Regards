#include "header.h"
#include "ToolbarTexte.h"
#include "WindowMain.h"
using namespace Regards::Window;

CToolbarTexte::CToolbarTexte(const CThemeToolbarTexte& theme)
{
	drawShape = false;
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	isActif = false;
	isPush = false;
	themeTexte.SetTailleX(DEFAULT_SIZEX);
	themeTexte.SetTailleY(DEFAULT_SIZEY);
	themeTexte = theme;
}

void CToolbarTexte::Resize(const int& tailleX, const int& tailleY)
{
	themeTexte.SetTailleX(tailleX);
	themeTexte.SetTailleY(tailleY);
}

CToolbarTexte::~CToolbarTexte()
{
}

void CToolbarTexte::SetLibelle(const wxString& libelle)
{
	libelleTooltip = libelle;
	this->libelle = libelle;
}

int CToolbarTexte::GetWidth()
{
	wxBitmap bitmap(250, 250);
	wxMemoryDC dc(bitmap);
	wxSize size = CWindowMain::GetSizeTexte(&dc, libelle, themeTexte.font);
	dc.SelectObject(wxNullBitmap);
	if (size.x > themeTexte.GetTailleX())
		return (size.x + 10);

	return themeTexte.GetTailleX();
}

int CToolbarTexte::GetHeight()
{
	return themeTexte.GetTailleY();
}

void CToolbarTexte::DrawShapeElement(wxDC* dc, const wxRect& rc)
{
	if (themeTexte.GetRectangleSize() > 0)
	{
        /*
        wxColor color;
        wxSystemAppearance systemApp = wxSystemSettings::GetAppearance();
        bool isDarkTheme =  systemApp.IsDark();
        if(isDarkTheme)
           color = wxColor(255, 255, 255);
        else
           color = wxColor(0, 0, 0);
        */
        CThemeFont font = themeTexte.font;
		wxColor color = font.GetColorFont();
		wxPen penTop(themeTexte.rectTop, themeTexte.GetRectangleSize(), wxPENSTYLE_SOLID);
		penTop.SetColour(color);
		dc->SetPen(penTop);
		dc->DrawLine(rc.x, rc.height, rc.width, rc.height);
		//dc->DrawLine(rc.x, rc.height, rc.x, rc.y);
		dc->SetPen(wxNullPen);
	}
}

void CToolbarTexte::DrawElement(wxDC* dc, const int& x, const int& y, const wxColor& color)
{
	if (libelle != L"")
	{
		CThemeFont font = themeTexte.font;
		font.SetColorFont(color);
		wxSize size = CWindowMain::GetSizeTexte(dc, libelle, font);
		int xPos = x + (GetWidth() - size.x) / 2;
		int yPos = y + (GetHeight() - size.y) / 2;
		CWindowMain::DrawTexte(dc, libelle, xPos, yPos, font);
	}
}

void CToolbarTexte::DrawButton(wxDC* dc, const int& x, const int& y)
{
	if (this->isVisible)
	{
		if (isPush && activePush)
		{
            printf("CToolbarTexte::DrawButton CreatePushButton\n");
			CreatePushButton(dc, x, y);
		}
		else if (isActif)
		{
            printf("CToolbarTexte::DrawButton CreateActifButton\n");
			CreateActifButton(dc, x, y);
		}
		else
		{
            printf("CToolbarTexte::DrawButton CreateInactifButton\n");
			CreateInactifButton(dc, x, y);
		}
	}
}

void CToolbarTexte::CreatePushButton(wxDC* dc, const int& x, const int& y)
{
	int size = themeTexte.GetRectangleSize() / 2;
	wxRect rc;
	rc.x = x + size;
	rc.width = x + GetWidth() - size;
	rc.y = y + size;
	rc.height = y + GetHeight() - size;
	DrawShapeElement(dc, rc);
	DrawElement(dc, x, y, themeTexte.font.GetColorFont());
}

void CToolbarTexte::CreateInactifButton(wxDC* dc, const int& x, const int& y)
{
    DrawElement(dc, x, y, themeTexte.colorBack);

    /*
    wxSystemAppearance systemApp = wxSystemSettings::GetAppearance();
    bool isDarkTheme =  systemApp.IsDark();
    if(isDarkTheme)
        DrawElement(dc, x, y, wxColor(0, 0, 0));
    else
        DrawElement(dc, x, y, wxColor(255, 255, 255));
    */
}

void CToolbarTexte::CreateActifButton(wxDC* dc, const int& x, const int& y)
{
	int size = themeTexte.GetRectangleSize() / 2;
	wxRect rc;
	rc.x = x + size;
	rc.width = x + GetWidth() - size;
	rc.y = y + size;
	rc.height = y + GetHeight() - size;
	DrawShapeElement(dc, rc);
	DrawElement(dc, x, y, themeTexte.font.GetColorFont());
}
