#pragma once
#include "TreeElement.h"
#include "TreeElementListBox.h"

namespace Regards
{
	namespace Window
	{
		class CTreeElementDelete : public CTreeElement
		{
		public:
			CTreeElementDelete();
			~CTreeElementDelete() override;

			void DrawElement(wxDC* deviceContext, const int& x, const int& y) override;
			void ClickElement(wxWindow* window, const int& x, const int& y) override;
			void SetTheme(CThemeTreeDelete* theme);

			CTreeElementDelete& operator=(const CTreeElementDelete& other);

			void SetZoneSize(const int& width, const int& height) override
			{
				themeTreeDelete.SetWidth(width);
				themeTreeDelete.SetHeight(height);
			}


			void SetBackgroundColor(const wxColour& color) override
			{
				themeTreeDelete.color = color;
			}

			int GetWidth() override
			{
				return themeTreeDelete.GetWidth();
			}

			int GetHeight() override
			{
				return themeTreeDelete.GetHeight();
			}


		private:
			void DrawBitmap(wxDC* deviceContext, const int& xPos, const int& yPos);
			CThemeTreeDelete themeTreeDelete;
		};
	}
}
