#include "ToolbarButton.h"
#include <theme.h>
#include <LibResource.h>
#include "LoadingResource.h"
#include "WindowMain.h"
#include <RegardsBitmap.h>
#include <InterpolationBicubic.h>
#include <FiltreEffet.h>
#if defined(__WXMSW__)
#include "../include/config_id.h"
#else
#include <config_id.h>
#endif
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>

CToolbarButton::CToolbarButton(const CThemeToolbarButton & theme)
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	isActif = false;
	isPush = false;
	resourceId = L"";
	resourceFolder = L"";
	resourceExtension = "";
	buttonWidth = 0;
	buttonHeight = 0;
	themeButton = theme;
    colorToReplace = wxColor(0,0,0);
    colorActifReplacement = themeButton.colorActifReplacement;
    colorInactifReplacement = themeButton.replaceColor;
}

void CToolbarButton::Resize(const int &tailleX, const int &tailleY)
{
	themeButton.SetTailleX(tailleX);
	themeButton.SetTailleY(tailleY);
}


CToolbarButton::~CToolbarButton()
{
}

void CToolbarButton::SetLibelle(const wxString &libelle)
{
	libelleTooltip = libelle;
	this->libelle = libelle;
}

void CToolbarButton::SetBorder(const int &size)
{
    themeButton.SetRectangleSize(size);
}

void CToolbarButton::ReplaceColor(const wxColor & colorToReplace, const wxColor & colorActifReplacement, const wxColor & colorInactifReplacement)
{
    this->colorToReplace = colorToReplace;
    this->colorActifReplacement = colorActifReplacement;
    this->colorInactifReplacement = colorInactifReplacement;
}

wxImage CToolbarButton::CreateFromSVG(const int & buttonWidth, const int & buttonHeight)
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

int CToolbarButton::GetBitmapWidth()
{
	int width = 0;
	if (button.IsOk())
		width += buttonWidth;
	return width;
}

int CToolbarButton::GetBitmapHeight()
{
	int height = 0;
	if (button.IsOk())
		height += buttonHeight;
	return height;
}

void CToolbarButton::SetButtonResourceId(const wxString &resourceId)
{

    this->resourceId = resourceId;
    vector = CLibResource::GetVector(resourceId);

}

int CToolbarButton::GetWidth()
{
	//if (button.GetWidth() > themeButton.GetTailleX())
	//	return button.GetWidth();
		
	return themeButton.GetTailleX();
}

int CToolbarButton::GetHeight()
{
	//if (button.GetHeight() > themeButton.GetTailleY())
	//	return button.GetHeight();

	return themeButton.GetTailleY();
}


void CToolbarButton::DrawShapeElement(wxDC * dc, const wxRect &rc)
{
	if (themeButton.GetRectangleSize() > 0)
	{
		wxPen penTop(themeButton.lineColorTop, themeButton.GetRectangleSize(), wxPENSTYLE_SOLID);
		dc->SetPen(penTop);
		dc->DrawLine(rc.x, rc.height, rc.width, rc.height);
		dc->DrawLine(rc.x, rc.height, rc.x, rc.y);
		dc->SetPen(wxNullPen);

		wxPen penBottom(themeButton.lineColorBottom, themeButton.GetRectangleSize(),  wxPENSTYLE_SOLID);
		dc->SetPen(penBottom);
		dc->DrawLine(rc.x, rc.y, rc.width, rc.y);
		dc->DrawLine(rc.width, rc.y, rc.width, rc.height);
		dc->SetPen(wxNullPen);
	}
}



void CToolbarButton::DrawElement(wxDC * dc, const int &x, const int &y, const bool &inactif)
{
    if(resourceId == L"")
        return;
    
    
    //Calcul des éléments suivant la présence ou non d'un libelle
    wxImage imageScale;
    
    if (libelle != L"")
    {
        int width = themeButton.GetTailleX();
        int height = themeButton.GetTailleY() - ((float)themeButton.GetTailleY() * 0.25);
        int diffHeight = themeButton.GetTailleY() -height;
        
        if(width == 0)
            return;
        
        if(height == 0)
            return;
        
        int buttonWidth = width - (2*themeButton.GetMarge());
        int buttonHeight = height - (2*themeButton.GetMarge());
        
        //float ratio = 1.0;

            if(!button.IsOk() || (button.GetWidth() != buttonWidth || button.GetHeight() != buttonHeight))
                button = CreateFromSVG(buttonWidth, buttonHeight);

            imageScale = button;


        int xPos = x + (themeButton.GetTailleX() - imageScale.GetWidth()) / 2;
        int yPos = y + (height - imageScale.GetHeight()) / 2;
        
        if (!inactif)
        {
            imageScale.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                               colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());
            dc->DrawBitmap(imageScale, xPos, yPos);
        }
        else
        {
            imageScale = imageScale.ConvertToGreyscale();
            
            imageScale.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                               colorInactifReplacement.Red(), colorInactifReplacement.Green(), colorInactifReplacement.Blue());
            dc->DrawBitmap(imageScale, xPos, yPos);
        }
        
        if (libelle != L"")
        {
            wxSize size = CWindowMain::GetSizeTexte(dc, libelle, themeButton.font);
            int xPos = x + (width - size.x) / 2;
            int yPos = y + height + (diffHeight - size.y) / 2;
            if (!inactif)
            {
                CThemeFont font = themeButton.font;
                font.SetColorFont(wxColor(0,0,0));
                CWindowMain::DrawTexte(dc, libelle, xPos, yPos, font);
            }
            else
                CWindowMain::DrawTexte(dc, libelle, xPos, yPos, themeButton.font);
        }
    }
    else
    {
        int width = themeButton.GetTailleX();
        int height = themeButton.GetTailleY();
        
        if(width == 0)
            return;
        
        if(height == 0)
            return;
        

        
        int buttonWidth = width - (2*themeButton.GetMarge());
        int buttonHeight = height - (2*themeButton.GetMarge());
        
        if(buttonWidth == 0)
        {
            buttonWidth = width;
        }
        
        if(buttonHeight == 0)
        {
            buttonHeight = height;
        }

        

            if(!button.IsOk() || (button.GetWidth() != buttonWidth || button.GetHeight() != buttonHeight))
                button = CreateFromSVG(buttonWidth, buttonHeight);
            
            imageScale = button;

        
        int xPos = x + (themeButton.GetTailleX() - imageScale.GetWidth()) / 2;
        int yPos = y + (themeButton.GetTailleY() - imageScale.GetHeight()) / 2;
        
        if (!inactif)
        {
            imageScale.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                               colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());
            dc->DrawBitmap(imageScale, xPos, yPos);
        }
        else
        {
            imageScale = imageScale.ConvertToGreyscale();
            
            imageScale.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                               colorInactifReplacement.Red(), colorInactifReplacement.Green(), colorInactifReplacement.Blue());
            dc->DrawBitmap(imageScale, xPos, yPos);
        }
    }

}

void CToolbarButton::DrawButton(wxDC * dc, const int &x, const int &y)
{
    //wxSVGFileDC
    
    
	if (this->isVisible)
	{
		if (isPush)
		{
			CreatePushButton(dc, x, y);
		}
		else if (isActif)
		{
			CreateActifButton(dc, x, y);
		}
		else
		{
			CreateInactifButton(dc, x, y);
		}
	}
}


void CToolbarButton::CreatePushButton(wxDC * dc, const int &x, const int &y)
{
	if (!themeButton.showButtonOnly)
	{
        wxRect rc;
        rc.x = 0;
        rc.y = 0;
        rc.width = GetWidth();
        rc.height = GetHeight();
        
        wxBitmap imageBackground(GetWidth(), GetHeight());
        wxMemoryDC memDC(imageBackground);
        memDC.GradientFillLinear(rc, themeButton.actifTop, themeButton.actifBottom);
        memDC.SelectObject(wxNullBitmap);
        
        wxBackground = imageBackground.ConvertToImage();
        
        /*
		wxRect rc;
		rc.x = x;
		rc.y = y;
		rc.width = GetWidth();
		rc.height = GetHeight();
		dc->GradientFillLinear(rc, themeButton.actifTop, themeButton.actifBottom);
        */
        
        dc->DrawBitmap(imageBackground, x, y);
        
		int size = themeButton.GetRectangleSize() / 2;

		rc.x = x + size;
		rc.width = x + GetWidth() - size;
		rc.y = y + size;
		rc.height = y + GetHeight() - size;
		DrawShapeElement(dc, rc);
	}
	DrawElement(dc,x, y);
}


void CToolbarButton::CreateInactifButton(wxDC * dc, const int &x, const int &y)
{
	DrawElement(dc, x, y, true);
}

void CToolbarButton::CreateActifButton(wxDC * dc, const int &x, const int &y)
{
	if (!themeButton.showButtonOnly)
	{
        
        wxRect rc;
        rc.x = 0;
        rc.y = 0;
        rc.width = GetWidth();
        rc.height = GetHeight();
        
        wxBitmap imageBackground(GetWidth(), GetHeight());
        wxMemoryDC memDC(imageBackground);
        memDC.GradientFillLinear(rc, themeButton.actifTop, themeButton.actifBottom);
        memDC.SelectObject(wxNullBitmap);
        
        wxBackground = imageBackground.ConvertToImage();
        
        /*
         wxRect rc;
         rc.x = x;
         rc.y = y;
         rc.width = GetWidth();
         rc.height = GetHeight();
         dc->GradientFillLinear(rc, themeButton.actifTop, themeButton.actifBottom);
         */
        
        dc->DrawBitmap(imageBackground, x, y);
        


		int size = themeButton.GetRectangleSize() / 2;

		rc.x = x + size;
		rc.width = x + GetWidth() - size;
		rc.y = y + size;
		rc.height = y + GetHeight() - size;
		DrawShapeElement(dc, rc);
	}
	DrawElement(dc, x, y);

}
