#pragma once
#include "TreeElement.h"

namespace Regards
{
	namespace Window
	{
		class CTreeElementTexte : public CTreeElement
		{
		public:
			CTreeElementTexte();
			~CTreeElementTexte();

			CTreeElementTexte& operator=(const CTreeElementTexte &other);

			void IsUpdatable(const bool &value)
			{
				canUpdate = value;
			};

			void SetZoneSize(const int &width, const int &height)
			{
				themeTexte.SetWidth(width);
				themeTexte.SetHeight(height);
			}

			void MouseOver(wxDC * deviceContext, const int &x, const int &y, bool & update);
			void DrawElement(wxDC * deviceContext, const int &x, const int &y);
			void SetLibelle(const wxString &libelle);
			void SetClick(const bool &value);
			void SetTheme(CThemeTreeTexte * theme);
			void SetPosition(const int &position);

			int GetWidth()
			{
				return themeTexte.GetWidth();
			}

			int GetHeight()
			{
				return themeTexte.GetHeight();
			}


		protected:

			void DrawText(wxDC * dc, const int &xPos, const int &yPos);
			wxSize GetSizeText();

			static mutex muTexteSize;
			bool canUpdate;
			bool isClick;
			wxString libelle;
			int position;
			CThemeTreeTexte themeTexte;
		};
	}
}

