#pragma once
#include "TreeElement.h"
#include "TreeElementDelete.h"

namespace Regards
{
	namespace Window
	{
		class CTreeElementStar : public CTreeElement
		{
		public:
			CTreeElementStar();
			~CTreeElementStar() override;

			void SetValue(const int& value);
			int GetValue();
			void DrawElement(wxDC* deviceContextc, const int& x, const int& y) override;
			void ClickElement(wxWindow* window, const int& x, const int& y) override;
			void SetTheme(CThemeTreeTriangle* theme);
			void SetNumPhoto(const int& numPhotoId);

			void SetZoneSize(const int& width, const int& height) override
			{
				themeTriangle.SetWidth(width);
				themeTriangle.SetHeight(height);
			}

			int GetWidth() override
			{
				return themeTriangle.GetWidth();
			}

			int GetHeight() override
			{
				return themeTriangle.GetHeight();
			}


		private:
			wxImage CreateFromSVG(const int& buttonWidth, const int& buttonHeight, const wxString& vector);
			void CreateStar();
			void DrawStar(wxDC* deviceContext, const int& x, const int& y);

			int value;
			CThemeTreeTriangle themeTriangle;
			wxImage starEmpty;
			wxImage starYellow;
			int localx;
			int localy;
			int numPhotoId;
		};
	}
}
