#include <header.h>
#include "WindowUtility.h"
#include <membitmap.h>
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
	try
	{
		int fontSize = font.GetFontSize();
		wxColour color = font.GetColorFont();
		wxFont _font(fontSize, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		dc->SetFont(_font);
		//dc->SetBackgroundMode(wxSOLID);
		dc->SetTextForeground(color);

		dc->DrawText(libelle, xPos, yPos);
		dc->SetFont(wxNullFont);
	}
	catch (...)
	{
	}
}

wxSize CWindowUtility::GetSizeTexte(const wxString& libelle, CThemeFont font)
{
	// Create a memory DC
	static mutex muBitmap;
	static CMemBitmap membitmap(500, 100);
	wxSize size;
	try
	{
		//membitmap.SetWindowSize(500, 100);
		//wxMemoryDC temp_dc(dc);
 		wxFont _font(font.GetFontSize(), wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		muBitmap.lock();
		membitmap.sourceDCContext.SetFont(_font);
		size = membitmap.sourceDCContext.GetTextExtent(libelle);
		membitmap.sourceDCContext.SetFont(wxNullFont);
		muBitmap.unlock();
	}
	catch (...)
	{

	}

	return size;
};
