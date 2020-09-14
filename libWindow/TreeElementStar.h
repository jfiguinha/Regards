#pragma once
#include "TreeElement.h"

namespace Regards
{
	namespace Window
	{

		class CTreeElementStar : public CTreeElement
		{
		public:
			CTreeElementStar();
			~CTreeElementStar();

			void SetValue(const int &value);
			int GetValue();
			void DrawElement(wxDC * deviceContextc, const int &x, const int &y);
			void ClickElement(wxWindow * window, const int &x, const int &y);
			void SetTheme(CThemeTreeTriangle * theme);

			void SetZoneSize(const int &width, const int &height)
			{
				themeTriangle.SetWidth(width);
				themeTriangle.SetHeight(height);
			}

			int GetWidth()
			{
				return themeTriangle.GetWidth();
			}

			int GetHeight()
			{
				return themeTriangle.GetHeight();
			}


		private:

			wxImage CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vector);
			void CreateStar();
			void DrawStar(wxDC * deviceContext, const int &x, const int &y);

			int value;
			CThemeTreeTriangle themeTriangle;
			wxImage starEmpty;
			wxImage starYellow;
			int localx;
			int localy;
		};
	}
}
