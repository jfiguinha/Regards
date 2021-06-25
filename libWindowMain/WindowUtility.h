#pragma once
#include <theme.h>

namespace Regards
{
	namespace Window
	{
		class CWindowUtility
		{
		public:
			void FillRect(wxDC* dc, const wxRect& rc, const wxColour& color);
			void DrawTexte(wxDC* dc, const wxString& libelle, const int& xPos, const int& yPos, CThemeFont font);
			wxSize GetSizeTexte(wxDC* dc, const wxString& libelle, CThemeFont font);
		};
	}
}
