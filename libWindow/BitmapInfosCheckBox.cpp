#include "BitmapInfosCheckBox.h"
#include <FileUtility.h>
#include <LibResource.h>
#include <ConvertUtility.h>
#include <wx/dcbuffer.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <wx/sstream.h>
#include <LoadingResource.h>
#include <wxSVG/SVGDocument.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Window;
using namespace Regards::Internet;

CBitmapInfosCheckBox::CBitmapInfosCheckBox(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos & theme)
	: CWindowMain(parent, id)
{
	bitmapInfosTheme = theme;
    Connect(wxEVT_PAINT, wxPaintEventHandler(CBitmapInfosCheckBox::OnPaint));
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CBitmapInfosCheckBox::OnLButtonDown));
    
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

wxImage CBitmapInfosCheckBox::CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vector)
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



void CBitmapInfosCheckBox::SetFilename(const wxString &libelle)
{
	if (filename != libelle)
	{
        filename = libelle;
	}
}


CBitmapInfosCheckBox::~CBitmapInfosCheckBox()
{
	//delete fileGeolocalisation;
}

int CBitmapInfosCheckBox::GetHeight()
{
	return bitmapInfosTheme.GetHeight();
}

void CBitmapInfosCheckBox::UpdateScreenRatio()
{
    Resize();
}

void CBitmapInfosCheckBox::Resize()
{
	this->Refresh();
}

void CBitmapInfosCheckBox::Redraw()
{
	wxWindowDC dc(this);
    DrawInformations(&dc);
}

void CBitmapInfosCheckBox::SetCheckboxVisible(const bool &checkboxVisibility)
{
    this->checkboxVisibility = checkboxVisibility;
    this->Redraw();
}

void CBitmapInfosCheckBox::OnLButtonDown(wxMouseEvent& event)
{
    int x = event.GetX();
    int y = event.GetY();
    
    if((x > xPos && x < xPos + bitmapCheckOn.GetWidth()) && (y > yPos && y < yPos + bitmapCheckOn.GetHeight()))
    {
        isSelected = !isSelected;
        this->Refresh();
        
        wxCommandEvent * event = new wxCommandEvent(wxEVENT_CHANGECHECKBOX);
        wxQueueEvent(this->GetParent(), event);
    }
    
}

bool CBitmapInfosCheckBox::GetCheckState()
{
    return isSelected;
}

void CBitmapInfosCheckBox::DrawInformations(wxDC * dc)
{
    
    wxRect rc;
    rc.x = 0;
    rc.y = 0;
    rc.width = width;
    rc.height = height;
    FillRect(dc, rc, bitmapInfosTheme.colorBack);
    wxString message = L"";
    wxString libelle = CFileUtility::GetFileName(filename);
    
    wxSize size = GetSizeTexte(dc, libelle, bitmapInfosTheme.themeFont);

    if(!checkboxVisibility)
        DrawTexte(dc, libelle, (width - size.x) / 2, (height - size.y) / 2, bitmapInfosTheme.themeFont);
    else
        DrawTexte(dc, libelle, (width - size.x) / 2, 0, bitmapInfosTheme.themeFont);

    if(checkboxVisibility)
    {
        wxString libelleSelectAll = CLibResource::LoadStringFromResource(L"LBLSelectAll", 1);
        
        if(isVector)
        {
            if (!bitmapCheckOn.IsOk() || (bitmapCheckOn.GetHeight() != bitmapInfosTheme.GetCheckboxHeight() || bitmapCheckOn.GetWidth() != bitmapInfosTheme.GetCheckboxWidth()))
            {
                bitmapCheckOn = CreateFromSVG(bitmapInfosTheme.GetCheckboxWidth(), bitmapInfosTheme.GetCheckboxHeight(), checkOnVector);
                bitmapCheckOn = bitmapCheckOn.ConvertToDisabled();
            }
            
            if (!bitmapCheckOff.IsOk() ||  (bitmapCheckOff.GetHeight() != bitmapInfosTheme.GetCheckboxHeight() || bitmapCheckOff.GetWidth() != bitmapInfosTheme.GetCheckboxWidth()))
            {
                bitmapCheckOff = CreateFromSVG(bitmapInfosTheme.GetCheckboxWidth(), bitmapInfosTheme.GetCheckboxHeight(), checkOffVector);
                bitmapCheckOff = bitmapCheckOff.ConvertToDisabled();
            }
        }
        
        xPos = 0;
        yPos = height - bitmapCheckOn.GetHeight();
        
        
        if (isSelected)
            dc->DrawBitmap(bitmapCheckOn, xPos, yPos);
        else
            dc->DrawBitmap(bitmapCheckOff, xPos, yPos);
        
        size = GetSizeTexte(dc, libelleSelectAll, bitmapInfosTheme.themeFont);
        
        int x = xPos + 5 + bitmapCheckOn.GetWidth();
        int y = (height - size.y) - (bitmapCheckOn.GetHeight() - size.y) / 2;
        
        DrawTexte(dc, libelleSelectAll, x, y, bitmapInfosTheme.themeFont);
    }

}

void CBitmapInfosCheckBox::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
    DrawInformations(&dc);
}