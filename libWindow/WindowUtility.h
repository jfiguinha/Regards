#pragma once
#include <theme.h>

namespace Regards::Window
{
	class CWindowUtility
	{
	public:
		static void FillRect(wxDC* dc, const wxRect& rc, const wxColour& color);
		static void DrawTexte(wxDC* dc, const wxString& libelle, const int& xPos, const int& yPos, CThemeFont font);
		static wxSize GetSizeTexte(const wxString& libelle, CThemeFont font);
	};
}
