#pragma once
#include <theme.h>
#include "ToolbarElement.h"
using namespace Regards::Window;

class CRegardsBitmap;

namespace Regards
{
	namespace Window
	{
		class CToolbarButton : public CToolbarElement
		{
		public:
			CToolbarButton(const CThemeToolbarButton& theme);
			~CToolbarButton() override;

			void SetLibelle(const wxString& libelle);
			void SetButtonResourceId(const wxString& resourceId);
			void DrawButton(wxDC* dc, const int& x, const int& y) override;
			void Resize(const int& tailleX, const int& tailleY) override;
			void ReplaceColor(const wxColor& colorToReplace, const wxColor& colorActifReplacement,
				const wxColor& colorInactifReplacement);
			int GetWidth() override;
			int GetHeight() override;
			void SetBorder(const int& size);
			int GetBitmapWidth();
			int GetBitmapHeight();

		protected:
			virtual void DrawElement(wxDC* dc, const int& x, const int& y, const bool& inactif = false);

		private:
			void CreateInactifButton(wxDC* dc, const int& x, const int& y);
			void CreateActifButton(wxDC* dc, const int& x, const int& y);
			void CreatePushButton(wxDC* dc, const int& x, const int& y);
			void DrawShapeElement(wxDC* dc, const wxRect& rc);


			int width;
			int height;
			wxString resourceId;

			wxString resourceFolder;
			string resourceExtension;

			wxString libelle;
			int buttonWidth;
			int buttonHeight;

			wxString vector;
			wxImage button;
			CThemeToolbarButton themeButton;
			wxImage wxBackground;

			wxColor colorToReplace;
			wxColor colorActifReplacement;
			wxColor colorInactifReplacement;
		};
	}
}
