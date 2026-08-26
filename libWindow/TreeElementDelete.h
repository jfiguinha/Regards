#pragma once
#include "TreeElement.h"
#include "TreeElementListBox.h"

namespace Regards::Window
{
	class CTreeElementDelete : public CTreeElement
	{
	public:
		CTreeElementDelete();
		~CTreeElementDelete() = default;

		void DrawElement(wxDC* deviceContext, const int& x, const int& y) override;

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
		void GenerateCrossBitmap();
		CThemeTreeDelete themeTreeDelete;
		wxBitmap m_croixOff;
	};
}
