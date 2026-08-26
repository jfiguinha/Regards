#include "header.h"
#include "TreeElementTexte.h"
#include <WindowUtility.h>
using namespace Regards::Window;



CTreeElementTexte::CTreeElementTexte()
{
	canUpdate = false;
	isClick = false;
	position = RENDERFONT_LEFT;
}

CTreeElementTexte& CTreeElementTexte::operator=(const CTreeElementTexte& other)
{
	visible = other.visible;
	xPos = other.xPos;
	yPos = other.yPos;
	numRow = other.numRow;
	numColumn = other.numColumn;
	themeTexte = other.themeTexte;
	canUpdate = other.canUpdate;
	isClick = other.isClick;
	libelle = other.libelle;
	position = other.position;
	return *this;
}

void CTreeElementTexte::SetTheme(CThemeTreeTexte* theme)
{
	themeTexte = *theme;
	textSize = GetSizeText();
}

void CTreeElementTexte::MouseOver(wxDC* deviceContext, const int& x, const int& y, bool& update)
{
	if (canUpdate)
		wxSetCursor(wxCursor(wxCURSOR_IBEAM));
	else
		wxSetCursor(wxCursor(wxCURSOR_HAND));

	update = false;
}

void CTreeElementTexte::SetLibelle(const wxString& libelle)
{
	this->libelle = libelle;

	textSize = GetSizeText();
	if (themeTexte.GetWidth() < textSize.x)
		themeTexte.SetWidth(textSize.x);

	if (themeTexte.GetHeight() < textSize.y)
		themeTexte.SetHeight(textSize.y);
}

wxSize CTreeElementTexte::GetSizeText()
{
	wxSize size;
	wxBitmap bitmap(250, 250);
	wxMemoryDC dc(bitmap);
	size = CWindowUtility::GetSizeTexte(&dc, libelle, themeTexte.font);
	dc.SelectObject(wxNullBitmap);
	return size;
}


void CTreeElementTexte::SetPosition(const int& position)
{
	this->position = position;
}

void CTreeElementTexte::DrawElement(wxDC* deviceContext, const int& x, const int& y)
{
	//wxSize size = GetSizeText();
	if(textSize.IsEmpty())
		textSize = GetSizeText();

	int xPos = 0;
	int yPos = y + (themeTexte.GetHeight() - textSize.y) / 2;
	switch (position)
	{
	case RENDERFONT_LEFT:
		xPos = x;
		break;

	case RENDERFONT_CENTER:
		xPos = x + (themeTexte.GetWidth() - textSize.x) / 2;
		break;

	case RENDERFONT_RIGHT:
		xPos = x + themeTexte.GetWidth() - textSize.x;
		break;
	default: ;
	}


	CWindowUtility::DrawTexte(deviceContext, libelle, xPos, yPos, themeTexte.font);
}

void CTreeElementTexte::SetClick(const bool& value)
{
	isClick = value;
}
