#include <header.h>
#include "WindowUtility.h"
using namespace Regards::Window;

void CWindowUtility::FillRect(wxDC* dc, const wxRect& rc, const wxColour& color)
{
	wxBrush brush(color, wxBRUSHSTYLE_SOLID);
	dc->SetBrush(brush);
	dc->SetPen(wxPen(color, 1)); // 10-pixels-thick pink outline
	dc->DrawRectangle(rc);
	dc->SetPen(wxNullPen);
	dc->SetBrush(wxNullBrush);
}

void CWindowUtility::DrawTexte(wxDC* dc, const wxString& libelle, const int& xPos, const int& yPos, CThemeFont font)
{
	int fontSize = font.GetFontSize();
	wxColour color = font.GetColorFont();
	wxFont _font(fontSize, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	dc->SetFont(_font);
	dc->SetTextForeground(color);
	dc->DrawText(libelle, xPos, yPos);
	dc->SetFont(wxNullFont);
}

wxSize CWindowUtility::GetSizeTexte(wxDC* dc, const wxString& libelle, CThemeFont font)
{
	wxSize size;
	wxMemoryDC temp_dc(dc);
 	wxFont _font(font.GetFontSize(), wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	temp_dc.SetFont(_font);
	size = temp_dc.GetTextExtent(libelle);
	temp_dc.SetFont(wxNullFont);
	return size;
};
