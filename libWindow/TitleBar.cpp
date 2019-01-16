#include "TitleBar.h"
#include <wx/colour.h>
#include <wx/dc.h>
#include <wx/bitmap.h>
#include <LibResource.h>
#include <wx/dcbuffer.h>
using namespace Regards::Window;

CTitleBar::CTitleBar(wxWindow* parent, wxWindowID id, CTitleBarInterface * titleBarInterface) :
CWindowMain("CTitleBar",parent, id)
{
	mouseCapture = false;
	isClosable = true;
	this->titleBarInterface = titleBarInterface;
	tooltip = CLibResource::LoadStringFromResource("LBLClose",1);
	CreateBitmapCrossOff();
	CreateBitmapCrossOn();
	SetWindowHeight(this->themeTitle.GetHeight());
	Connect(wxEVT_PAINT, wxPaintEventHandler(CTitleBar::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CTitleBar::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CTitleBar::OnLButtonDown));

}

void CTitleBar::Redraw()
{
	wxRect rect = GetWindowRect();

	//int middleYPos = (height - themeTitle.GetCroixHeigth()) / 2;

	wxWindowDC dc(this);
	FillRect(&dc, rect, themeTitle.colorBack);
	dc.DrawBitmap(m_croixOff, rcFermer.x, rcFermer.y, false);
	wxSize size = GetSizeTexte(&dc, libelle, themeTitle.font);
	int yPos = (this->GetWindowHeight() - size.y) / 2;
	DrawTexte(&dc, libelle, themeTitle.GetMarge(), yPos, themeTitle.font);
}

void CTitleBar::SetTheme(CThemeTitleBar * themeTitle)
{
	this->themeTitle = *themeTitle;
	SetWindowHeight(this->themeTitle.GetHeight());
}

CTitleBar::~CTitleBar()
{
}

int CTitleBar::SetTooltipText(const wxString & tooltip)
{
	this->tooltip = tooltip;
	return 0;
}

void CTitleBar::SetTitle(const wxString & title)
{
	libelle = title;
}

void CTitleBar::OnMouseMove(wxMouseEvent& event)
{
    if(!isClosable)
        return;

	int xPos = event.GetX();
	int yPos = event.GetY();
	//int bmpWidth = m_croixOff.GetWidth();
	//int bmpHeight = m_croixOff.GetHeight();

	if ((xPos > rcFermer.x && rcFermer.width > xPos) &&
		(yPos > rcFermer.y && rcFermer.height > yPos))
	{
		mouseCapture = true;

		wxSetCursor(wxCursor(wxCURSOR_HAND));

		wxClientDC dc(this);
		dc.DrawBitmap(m_croixOn, rcFermer.x, rcFermer.y, false);

		this->SetToolTip(tooltip.c_str());
	}
	else if (mouseCapture)
	{
		wxClientDC dc(this);
		dc.DrawBitmap(m_croixOff, rcFermer.x, rcFermer.y, false);

		wxSetCursor(wxNullCursor);

		mouseCapture = false;
	}
}


void CTitleBar::UpdateScreenRatio()
{
    SetWindowHeight(themeTitle.GetHeight());
    CreateBitmapCrossOff();
    CreateBitmapCrossOn();
    Resize();
}

void CTitleBar::SetClosable(const bool &value)
{
	isClosable = value;
}

void CTitleBar::Resize()
{
	int middleYPos = (GetWindowHeight() - themeTitle.GetCroixHeight()) / 2;
	rcFermer.x = GetWindowWidth() - themeTitle.GetCroixWidth() - themeTitle.GetMarge();
	rcFermer.y = middleYPos;
	rcFermer.width = rcFermer.x + themeTitle.GetCroixWidth();
	rcFermer.height = rcFermer.y + themeTitle.GetCroixHeight();
	this->FastRefresh(this);
}

void CTitleBar::OnPaint(wxPaintEvent& event)
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();
    if(width == 0 || height == 0)
        return;

    
	wxRect rect = GetWindowRect();
	
	//int middleYPos = (height - themeTitle.GetCroixHeigth()) / 2;

	wxPaintDC dc(this);
    FillRect(&dc, rect, themeTitle.colorBack);
    
    if(isClosable)
    {
        dc.DrawBitmap(m_croixOff, rcFermer.x, rcFermer.y, false);
    }
	
	
	wxSize size = GetSizeTexte(&dc, libelle, themeTitle.font);
	int yPos = (this->GetWindowHeight() - size.y) / 2;
	DrawTexte(&dc, libelle, themeTitle.GetMarge(), yPos, themeTitle.font);

}

void CTitleBar::OnLButtonDown(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();

	if ((xPos > rcFermer.x && rcFermer.width > xPos) &&
		(yPos > rcFermer.y && rcFermer.height > yPos))
	{
		if (titleBarInterface != nullptr)
			titleBarInterface->ClosePane();
	}
}

void CTitleBar::CreateBitmapCrossOn()
{
	wxRect rcBitmap;
	rcBitmap.x = 0;
	rcBitmap.y = 0;
	rcBitmap.width = themeTitle.GetCroixWidth();
	rcBitmap.height = themeTitle.GetCroixHeight();
	m_croixOn = wxBitmap(themeTitle.GetCroixWidth(), themeTitle.GetCroixHeight(), 32);
	wxMemoryDC memorydc(m_croixOn);

	wxPen pen(themeTitle.colorCross, 2);

    wxColour colorLight;
    colorLight.Set(themeTitle.colorBack.Red() + 20, themeTitle.colorBack.Green() + 20, themeTitle.colorBack.Blue() + 20);
    wxColour colorDark;
    colorDark.Set(themeTitle.colorBack.Red() - 20, themeTitle.colorBack.Green() - 20, themeTitle.colorBack.Blue() - 20);

	wxPen penLight(colorLight, 2);
	wxPen penDark(colorDark, 2);

	FillRect(&memorydc, rcBitmap, themeTitle.colorBack);

	memorydc.SetPen(pen);
    memorydc.DrawLine(3, 3, themeTitle.GetCroixWidth() - 4, themeTitle.GetCroixHeight() - 4);
    memorydc.DrawLine(themeTitle.GetCroixWidth() - 4, 3, 3, themeTitle.GetCroixHeight() - 4);
	//memorydc.DrawLine(3, 3, 11, 11);
	//memorydc.DrawLine(11, 3, 3, 11);
	memorydc.SetPen(wxNullPen);

	memorydc.SetPen(penLight);
	memorydc.DrawLine(0, 0, themeTitle.GetCroixWidth(), 0);
	memorydc.DrawLine(0, 0, 0, themeTitle.GetCroixHeight());
	memorydc.SetPen(wxNullPen);

	memorydc.SetPen(penDark);
	memorydc.DrawLine(themeTitle.GetCroixWidth(), themeTitle.GetCroixHeight(), themeTitle.GetCroixWidth(), 0);
	memorydc.DrawLine(0, themeTitle.GetCroixHeight(), themeTitle.GetCroixWidth(), themeTitle.GetCroixHeight());
	memorydc.SetPen(wxNullPen);

	memorydc.SelectObject(wxNullBitmap);
}

void CTitleBar::CreateBitmapCrossOff()
{
	wxRect rcBitmap;
	rcBitmap.x = 0;
	rcBitmap.y = 0;
	rcBitmap.width = themeTitle.GetCroixWidth();
	rcBitmap.height = themeTitle.GetCroixHeight();

	m_croixOff = wxBitmap(themeTitle.GetCroixWidth(), themeTitle.GetCroixHeight(), 32);
	wxMemoryDC memorydc(m_croixOff);
	wxPen pen(themeTitle.colorCross, 2);
	FillRect(&memorydc, rcBitmap, themeTitle.colorBack);
	memorydc.SetPen(pen);
    /*
	memorydc.DrawLine(3, 3, 11, 11);
	memorydc.DrawLine(11, 3, 3, 11);
    */
    memorydc.DrawLine(3, 3, themeTitle.GetCroixWidth() - 4, themeTitle.GetCroixHeight() - 4);
    memorydc.DrawLine(themeTitle.GetCroixWidth() - 4, 3, 3, themeTitle.GetCroixHeight() - 4);
	memorydc.SelectObject(wxNullBitmap);
}
