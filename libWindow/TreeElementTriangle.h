#pragma once
#include "TreeElement.h"
#include "TreeElementDelete.h"

namespace Regards::Window
{
	class CTreeElementTriangle : public CTreeElement
	{
	public:
		CTreeElementTriangle();
		~CTreeElementTriangle() override;

		void SetOpen(const bool& open);
		bool GetOpen();
		void DrawElement(wxDC* deviceContextc, const int& x, const int& y) override;
		void ClickElement(wxWindow* window, const int& x, const int& y) override;
		void SetTheme(CThemeTreeTriangle* theme);

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
		void DrawTriangleClose(wxDC* deviceContext, const int& x, const int& y);
		void DrawTriangleOpen(wxDC* deviceContext, const int& x, const int& y);

		bool open;
		CThemeTreeTriangle themeTriangle;
	};
}
