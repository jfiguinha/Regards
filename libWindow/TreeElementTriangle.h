#pragma once
#include "TreeElement.h"

class CTreeElementTriangle : public CTreeElement
{
public:
	CTreeElementTriangle();
	~CTreeElementTriangle();

	void SetOpen(const bool &open);
	bool GetOpen();
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

	void DrawTriangleClose(wxDC * deviceContext, const int &x, const int &y);
	void DrawTriangleOpen(wxDC * deviceContext, const int &x, const int &y);

	bool open;
	CThemeTreeTriangle themeTriangle;
};

