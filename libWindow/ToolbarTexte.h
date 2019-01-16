#pragma once
#include "ToolbarElement.h"
#include <theme.h>

namespace Regards
{
	namespace Window
	{

		class CToolbarTexte : public CToolbarElement
		{
		public:
			CToolbarTexte(const CThemeToolbarTexte & theme);
			virtual ~CToolbarTexte();
			int GetWidth();
			int GetHeight();
			void SetLibelle(const wxString &libelle);
			void DrawButton(wxDC * dc, const int &x, const int &y);
			void Resize(const int &tailleX, const int &tailleY);

		protected:

			virtual void DrawElement(wxDC * dc, const int &x, const int &y, const wxColor &color);

		private:

			void CreateInactifButton(wxDC * dc, const int &x, const int &y);
			void CreateActifButton(wxDC * dc, const int &x, const int &y);
			void CreatePushButton(wxDC * dc, const int &x, const int &y);
			void DrawShapeElement(wxDC * deviceContext, const wxRect &rc);	

			bool drawShape;
			int width;
			int height;
			wxBitmap buttonLibelle;
			wxString libelle;
			CThemeToolbarTexte themeTexte;
		};

		
	}
}

