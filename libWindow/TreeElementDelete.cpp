#include "header.h"
#include "TreeElementDelete.h"
#include "WindowMain.h"
using namespace Regards::Window;

CTreeElementDelete::CTreeElementDelete()
{
	visible = true;
}

CTreeElementDelete& CTreeElementDelete::operator=(const CTreeElementDelete& other)
{
	visible = other.visible;
	xPos = other.xPos;
	yPos = other.yPos;
	numRow = other.numRow;
	numColumn = other.numColumn;
	themeTreeDelete = other.themeTreeDelete;
	return *this;
}

CTreeElementDelete::~CTreeElementDelete()
{
}

void CTreeElementDelete::SetTheme(CThemeTreeDelete* theme)
{
	themeTreeDelete = *theme;
}

void CTreeElementDelete::ClickElement(wxWindow* window, const int& x, const int& y)
{
	//checked = !checked;
}

void CTreeElementDelete::DrawElement(wxDC* deviceContext, const int& x, const int& y)
{
	DrawBitmap(deviceContext, x, y);
}

void CTreeElementDelete::DrawBitmap(wxDC* deviceContext, const int& xPos, const int& yPos)
{
	wxRect rcBitmap;
	rcBitmap.x = 0;
	rcBitmap.y = 0;
	rcBitmap.width = themeTreeDelete.GetCroixWidth();
	rcBitmap.height = themeTreeDelete.GetCroixHeight();

	auto m_croixOff = wxBitmap(themeTreeDelete.GetCroixWidth(), themeTreeDelete.GetCroixHeight(), 32);
	wxMemoryDC memorydc(m_croixOff);
	wxPen pen(themeTreeDelete.crossColor, 2);
	CWindowMain::FillRect(&memorydc, rcBitmap, themeTreeDelete.color);
	memorydc.SetPen(pen);
	memorydc.DrawLine(3, 3, themeTreeDelete.GetCroixWidth() - 4, themeTreeDelete.GetCroixHeight() - 4);
	memorydc.DrawLine(themeTreeDelete.GetCroixWidth() - 4, 3, 3, themeTreeDelete.GetCroixHeight() - 4);
	memorydc.SelectObject(wxNullBitmap);

	int y = yPos + (themeTreeDelete.GetHeight() - m_croixOff.GetHeight()) / 2;
	//int x = xPos + (themeTreeDelete.width - m_croixOff.GetWidth()) / 2;
	deviceContext->DrawBitmap(m_croixOff, xPos, y);
}
