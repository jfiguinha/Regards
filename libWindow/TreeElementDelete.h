#pragma once
#include "TreeElement.h"

class CTreeElementDelete : public CTreeElement
{
public:
	CTreeElementDelete();
	~CTreeElementDelete();

	void DrawElement(wxDC * deviceContext, const int &x, const int &y);
	void ClickElement(wxWindow * window, const int &x, const int &y);
	void SetTheme(CThemeTreeDelete * theme);

	CTreeElementDelete& operator=(const CTreeElementDelete &other);

	void SetZoneSize(const int &width, const int &height)
	{
		themeTreeDelete.SetWidth(width);
		themeTreeDelete.SetHeight(height);
	}


	void SetBackgroundColor(const wxColour &color)
	{
		themeTreeDelete.color = color;
	}

	int GetWidth()
	{
		return themeTreeDelete.GetWidth();
	}

	int GetHeight()
	{
		return themeTreeDelete.GetHeight();
	}


private:

	void DrawBitmap(wxDC * deviceContext, const int &xPos, const int &yPos);
	CThemeTreeDelete themeTreeDelete;
};

