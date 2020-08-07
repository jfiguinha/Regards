#include "header.h"
#include "BitmapInfosCheckBox.h"
#include <FileUtility.h>
#include <LibResource.h>
#include <wx/dcbuffer.h>
#include <RegardsConfigParam.h>
#include <wx/sstream.h>
#include <wxSVG/SVGDocument.h>
#include <window_id.h>

using namespace Regards::Window;
//using namespace Regards::Internet;

CBitmapInfosCheckBox::CBitmapInfosCheckBox(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos & theme)
	: CWindowMain("CBitmapInfosCheckBox",parent, id)
{
	bitmapInfosTheme = theme;
    Connect(wxEVT_PAINT, wxPaintEventHandler(CBitmapInfosCheckBox::OnPaint));
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CBitmapInfosCheckBox::OnLButtonDown));
    
    isSelected = false;
	checkboxVisibility = true;
	xPos = 0;
	yPos = 0;

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


void CBitmapInfosCheckBox::Redraw()
{
	wxClientDC dc(this);
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
        Refresh();
        
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
    
    wxRect rc = GetWindowRect();
    FillRect(dc, rc, bitmapInfosTheme.colorBack);
    wxString message = L"";
    wxString libelle = CFileUtility::GetFileName(filename);
    
    wxSize size = GetSizeTexte(dc, libelle, bitmapInfosTheme.themeFont);

    if(!checkboxVisibility)
        DrawTexte(dc, libelle, (GetWindowWidth() - size.x) / 2, (GetWindowHeight() - size.y) / 2, bitmapInfosTheme.themeFont);
    else
        DrawTexte(dc, libelle, (GetWindowWidth() - size.x) / 2, 0, bitmapInfosTheme.themeFont);

    if(checkboxVisibility)
    {
        wxString libelleSelectAll = CLibResource::LoadStringFromResource(L"LBLSelectAll", 1);
        

            if (!bitmapCheckOn.IsOk() || (bitmapCheckOn.GetHeight() != bitmapInfosTheme.GetCheckboxHeight() || bitmapCheckOn.GetWidth() != bitmapInfosTheme.GetCheckboxWidth()))
            {
                bitmapCheckOn = CLibResource::CreatePictureFromSVG("IDB_CHECKBOX_ON", bitmapInfosTheme.GetCheckboxWidth(), bitmapInfosTheme.GetCheckboxHeight());
                bitmapCheckOn = bitmapCheckOn.ConvertToDisabled();
            }
            
            if (!bitmapCheckOff.IsOk() ||  (bitmapCheckOff.GetHeight() != bitmapInfosTheme.GetCheckboxHeight() || bitmapCheckOff.GetWidth() != bitmapInfosTheme.GetCheckboxWidth()))
            {
                bitmapCheckOff = CLibResource::CreatePictureFromSVG("IDB_CHECKBOX_OFF", bitmapInfosTheme.GetCheckboxWidth(), bitmapInfosTheme.GetCheckboxHeight());
                bitmapCheckOff = bitmapCheckOff.ConvertToDisabled();
            }

        
        xPos = 0;
        yPos = GetWindowHeight() - bitmapCheckOn.GetHeight();
        
        
        if (isSelected && bitmapCheckOn.IsOk())
            dc->DrawBitmap(bitmapCheckOn, xPos, yPos);
        else if(bitmapCheckOff.IsOk())
            dc->DrawBitmap(bitmapCheckOff, xPos, yPos);
        
        size = GetSizeTexte(dc, libelleSelectAll, bitmapInfosTheme.themeFont);
        
        int x = xPos + 5 + bitmapCheckOn.GetWidth();
        int y = (GetWindowHeight() - size.y) - (bitmapCheckOn.GetHeight() - size.y) / 2;
        
        DrawTexte(dc, libelleSelectAll, x, y, bitmapInfosTheme.themeFont);
    }

}

void CBitmapInfosCheckBox::OnPaint(wxPaintEvent& event)
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;

    
	wxBufferedPaintDC dc(this);
    DrawInformations(&dc);
}