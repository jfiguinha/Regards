#include "TreeElementCheckBox.h"
#include "LoadingResource.h"
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
#include <LibResource.h>
CTreeElementCheckBox::CTreeElementCheckBox()
{
	CLoadingResource loadingResource;
	checked = false;
    
    if(isVector)
    {
        checkOnVector = CLibResource::GetVector(L"IDB_CHECKBOX_ON");;
        checkOffVector = CLibResource::GetVector(L"IDB_CHECKBOX_OFF");;
    }
    else
    {
        checkOn = loadingResource.LoadBmpResource(L"IDB_CHECKBOX_ON");
        checkOff = loadingResource.LoadBmpResource(L"IDB_CHECKBOX_OFF");

#ifndef __WXGTK__
        if (checkOn.IsOk())
            checkOn.UseAlpha();

        if (checkOff.IsOk())
            checkOff.UseAlpha();
#endif
    }
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
        if(isVector)
        {
            imageScale = CreateFromSVG(checkWidth, checkHeight, checkOnVector);
        }
        else
        {
            imageScale = checkOn.ConvertToImage();
            imageScale = imageScale.Scale(checkWidth, checkHeight);
        }

		int y = yPos + (themeTreeCheckBox.GetHeight() - imageScale.GetHeight()) / 2;
		int x = xPos + (themeTreeCheckBox.GetWidth() - imageScale.GetWidth()) / 2;
		deviceContext->DrawBitmap(imageScale, x, y);
	}
	else
	{
        if(isVector)
        {
            imageScale = CreateFromSVG(checkWidth, checkHeight, checkOffVector);
        }
        else
        {
            imageScale = checkOff.ConvertToImage();
            imageScale = imageScale.Scale(checkWidth, checkHeight);
        }

		int y = yPos + (themeTreeCheckBox.GetHeight() - imageScale.GetHeight()) / 2;
		int x = xPos + (themeTreeCheckBox.GetWidth() - imageScale.GetWidth()) / 2;
		deviceContext->DrawBitmap(imageScale, x, y);
	}
}

