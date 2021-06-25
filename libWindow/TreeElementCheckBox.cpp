#include "header.h"
#include "TreeElementCheckBox.h"
#include <wxSVG/SVGDocument.h>
#include <LibResource.h>
using namespace Regards::Window;

CTreeElementCheckBox::CTreeElementCheckBox()
{
	checked = false;
}

CTreeElementCheckBox& CTreeElementCheckBox::operator=(const CTreeElementCheckBox& other)
{
	visible = other.visible;
	xPos = other.xPos;
	yPos = other.yPos;
	numRow = other.numRow;
	numColumn = other.numColumn;
	checked = other.checked;
	themeTreeCheckBox = other.themeTreeCheckBox;
	return *this;
}

CTreeElementCheckBox::~CTreeElementCheckBox()
{
}

void CTreeElementCheckBox::SetTheme(CThemeTreeCheckBox* theme)
{
	themeTreeCheckBox = *theme;
}

void CTreeElementCheckBox::ClickElement(wxWindow* window, const int& x, const int& y)
{
	checked = !checked;
}

bool CTreeElementCheckBox::GetCheckState()
{
	return checked;
}

void CTreeElementCheckBox::DrawElement(wxDC* deviceContext, const int& x, const int& y)
{
	DrawBitmap(deviceContext, x, y);
}

void CTreeElementCheckBox::SetCheckState(const bool& check)
{
	checked = check;
}

void CTreeElementCheckBox::DrawBitmap(wxDC* deviceContext, const int& xPos, const int& yPos)
{
	int checkWidth = themeTreeCheckBox.GetCheckBoxWidth();
	int checkHeight = themeTreeCheckBox.GetCheckBoxHeight();
	wxImage imageScale;

	if (checked)
	{
		imageScale = CLibResource::CreatePictureFromSVG("IDB_CHECKBOX_ON", checkWidth, checkHeight);
		int y = yPos + (themeTreeCheckBox.GetHeight() - imageScale.GetHeight()) / 2;
		int x = xPos + (themeTreeCheckBox.GetWidth() - imageScale.GetWidth()) / 2;
		deviceContext->DrawBitmap(imageScale, x, y);
	}
	else
	{
		imageScale = CLibResource::CreatePictureFromSVG("IDB_CHECKBOX_OFF", checkWidth, checkHeight);
		int y = yPos + (themeTreeCheckBox.GetHeight() - imageScale.GetHeight()) / 2;
		int x = xPos + (themeTreeCheckBox.GetWidth() - imageScale.GetWidth()) / 2;
		deviceContext->DrawBitmap(imageScale, x, y);
	}
}
