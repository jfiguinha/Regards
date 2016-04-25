#include "InfosSeparationBarExplorer.h"
#include <LoadingResource.h>
#include <WindowMain.h>
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
#include <LibResource.h>
using namespace Regards::Window;
using namespace Regards::Explorer;

CInfosSeparationBarExplorer::~CInfosSeparationBarExplorer(void)
{
}

bool CInfosSeparationBarExplorer::GetSelected()
{
	return isSelected;
}

CInfosSeparationBarExplorer::CInfosSeparationBarExplorer(const CThemeInfosSeparationBar &theme)
	: CInfosSeparationBar(theme)
{
	isSelected = false;
	CLoadingResource loadingResource;
    if(isVector)
    {
        checkOnVector = CLibResource::GetVector(L"IDB_CHECKBOX_ON");
        checkOffVector = CLibResource::GetVector(L"IDB_CHECKBOX_OFF");
    }
    else
    {
        bitmapCheckOn = loadingResource.LoadImageResource(L"IDB_CHECKBOX_ON");
        bitmapCheckOff = loadingResource.LoadImageResource(L"IDB_CHECKBOX_OFF");
    }
}

wxImage CInfosSeparationBarExplorer::CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vector)
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

void CInfosSeparationBarExplorer::OnClick(const int &x, const int &y)
{
	wxRect rc;
	rc.x = 0;
	rc.width = bitmapCheckOn.GetWidth();
	rc.y = this->y + (theme.GetHeight() - bitmapCheckOn.GetHeight());
	rc.height = theme.GetHeight();

	if ((rc.x < x && x < (rc.x + rc.width)) && (rc.y < y && y < (rc.y + rc.height)))
	{
		isSelected = !isSelected;
	}
}


void CInfosSeparationBarExplorer::RenderIcone(wxDC * deviceContext)
{
	RenderTitle(deviceContext);
	wxString libelleSelectAll = CLibResource::LoadStringFromResource(L"LBLSelectAll", 1);
    
    if(isVector)
    {
		if (!bitmapCheckOn.IsOk() || (bitmapCheckOn.GetHeight() != theme.GetCheckboxHeight() || bitmapCheckOn.GetWidth() != theme.GetCheckboxWidth()))
        {
            bitmapCheckOn = CreateFromSVG(theme.GetCheckboxWidth(), theme.GetCheckboxHeight(), checkOnVector);
            bitmapCheckOn = bitmapCheckOn.ConvertToDisabled();
        }
        
		if (!bitmapCheckOff.IsOk() ||  (bitmapCheckOff.GetHeight() != theme.GetCheckboxHeight() || bitmapCheckOff.GetWidth() != theme.GetCheckboxWidth()))
        {
            bitmapCheckOff = CreateFromSVG(theme.GetCheckboxWidth(), theme.GetCheckboxHeight(), checkOffVector);
            bitmapCheckOff = bitmapCheckOff.ConvertToDisabled();
        }
        
        
    }
    

    
    int xPos = x;
    int yPos = y + (theme.GetHeight() - bitmapCheckOn.GetHeight());
    


    if (isSelected)
        deviceContext->DrawBitmap(bitmapCheckOn, xPos, yPos);
    else
        deviceContext->DrawBitmap(bitmapCheckOff, xPos, yPos);
    
    wxSize size = CWindowMain::GetSizeTexte(deviceContext, libelleSelectAll, theme.themeFont);
    
    xPos = xPos + 5 + bitmapCheckOn.GetWidth();
    yPos = y + (theme.GetHeight() - size.y) - (bitmapCheckOn.GetHeight() - size.y) / 2;
    
    CWindowMain::DrawTexte(deviceContext, libelleSelectAll, xPos, yPos, theme.themeFont);


}