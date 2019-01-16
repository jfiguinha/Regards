#include "TreeElementTriangle.h"

CTreeElementTriangle::CTreeElementTriangle()
{
	open = false;
}


CTreeElementTriangle::~CTreeElementTriangle()
{
}

void CTreeElementTriangle::SetTheme(CThemeTreeTriangle * theme)
{
	themeTriangle = *theme;
}

void CTreeElementTriangle::SetOpen(const bool &open)
{
	this->open = open;
}

bool CTreeElementTriangle::GetOpen()
{
	return open;
}

void CTreeElementTriangle::DrawElement(wxDC * deviceContext, const int &x, const int &y)
{
	if (open)
	{
		DrawTriangleOpen(deviceContext, x, y);
	}
	else
	{
		DrawTriangleClose(deviceContext, x, y);
	}
}

void CTreeElementTriangle::DrawTriangleClose(wxDC * dc, const int &x, const int &y)
{
	int marge = (themeTriangle.GetHeight() - themeTriangle.GetElementHeight()) / 2;
	wxBrush brushHatch(themeTriangle.color);
	dc->SetBrush(brushHatch);
	wxPoint star[3];
	star[0] = wxPoint(x + themeTriangle.GetMarge(), y + marge);
	star[1] = wxPoint(x + themeTriangle.GetMarge(), y + marge + themeTriangle.GetElementHeight());
	star[2] = wxPoint(x + themeTriangle.GetElementWidth() + themeTriangle.GetMarge(), y + marge + (themeTriangle.GetElementHeight() / 2));
	dc->DrawPolygon(WXSIZEOF(star), star, 0, 0);
	dc->SetBrush(wxNullBrush);
}

void CTreeElementTriangle::ClickElement(wxWindow * window, const int &x, const int &y)
{
	open = !open;
}

void CTreeElementTriangle::DrawTriangleOpen(wxDC * dc, const int &x, const int &y)
{
	int marge = (themeTriangle.GetHeight() - themeTriangle.GetElementHeight()) / 2;
	wxBrush brushHatch(themeTriangle.color);
	dc->SetBrush(brushHatch);
	wxPoint star[3];
	star[0] = wxPoint(x + themeTriangle.GetMarge(), y + marge + themeTriangle.GetElementHeight());
	star[1] = wxPoint(x + themeTriangle.GetMarge() + themeTriangle.GetElementWidth(), y + marge);
	star[2] = wxPoint(x + themeTriangle.GetElementWidth() + themeTriangle.GetMarge(), y + marge + themeTriangle.GetElementHeight());
	dc->DrawPolygon(WXSIZEOF(star), star, 0, 0);
	dc->SetBrush(wxNullBrush);

}