#include "header.h"
#include "TreeElementCheckBox.h"
#include "LoadingResource.h"
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
#include <LibResource.h>
CTreeElementCheckBox::CTreeElementCheckBox()
{
	checked = false;
    checkOnVector = CLibResource::GetVector(L"IDB_CHECKBOX_ON");;
    checkOffVector = CLibResource::GetVector(L"IDB_CHECKBOX_OFF");;

}

CTreeElementCheckBox& CTreeElementCheckBox::operator=(const CTreeElementCheckBox &other)
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

void CTreeElementCheckBox::SetTheme(CThemeTreeCheckBox * theme)
{
	themeTreeCheckBox = *theme;

}

void CTreeElementCheckBox::ClickElement(wxWindow * window, const int &x, const int &y)
{
	checked = !checked;
}

bool CTreeElementCheckBox::GetCheckState()
{
	return checked;
}

void CTreeElementCheckBox::DrawElement(wxDC * deviceContext, const int &x, const int &y)
{
	DrawBitmap(deviceContext, x, y);
}

void CTreeElementCheckBox::SetCheckState(const bool &check)
{
	checked = check;
}

wxImage CTreeElementCheckBox::CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vector)
{
    wxImage img;
    if(vector.size() > 0)
    {
        wxStringInputStream memBuffer(vector);
        wxSVGDocument svgDoc;
        svgDoc.Load(memBuffer);
        img = svgDoc.Render(buttonWidth,buttonHeight,NULL,true,true);
        
    }
    else
    {
        img.Create(buttonWidth, buttonHeight);
    }
    return img;
}

void CTreeElementCheckBox::DrawBitmap(wxDC * deviceContext, const int &xPos, const int &yPos)
{
    int checkWidth = themeTreeCheckBox.GetCheckBoxWidth();
    int checkHeight = themeTreeCheckBox.GetCheckBoxHeight();
    wxImage imageScale;
    
	if (checked)
	{

            imageScale = CreateFromSVG(checkWidth, checkHeight, checkOnVector);


		int y = yPos + (themeTreeCheckBox.GetHeight() - imageScale.GetHeight()) / 2;
		int x = xPos + (themeTreeCheckBox.GetWidth() - imageScale.GetWidth()) / 2;
		deviceContext->DrawBitmap(imageScale, x, y);
	}
	else
	{

            imageScale = CreateFromSVG(checkWidth, checkHeight, checkOffVector);


		int y = yPos + (themeTreeCheckBox.GetHeight() - imageScale.GetHeight()) / 2;
		int x = xPos + (themeTreeCheckBox.GetWidth() - imageScale.GetWidth()) / 2;
		deviceContext->DrawBitmap(imageScale, x, y);
	}
}

