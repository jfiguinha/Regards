
#include "TreeElementSlide.h"
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
#include <libResource.h>
#include <ClosedHandCursor.h>
#include "LoadingResource.h"
#include "WindowMain.h"
using namespace Regards::Window;

CTreeElementSlide::CTreeElementSlide(CTreeElementSlideInterface * eventInterface)
{
	CLoadingResource loadingResource;
	this->eventInterface = eventInterface;
	positionButton = { 0, 0, 0, 0 };
	hCursorHand = CResourceCursor::GetClosedHand();
	mouseBlock = false;
	captureBall = false;
    if(isVector)
    {
        button.Create(0,0);
        buttonVector = CLibResource::GetVector("IDB_BOULESLIDER");
    }
    else
    {
        button = loadingResource.LoadImageResource("IDB_BOULESLIDER");
    }

}

wxImage CTreeElementSlide::CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vectorCode)
{
    wxStringInputStream memBuffer(vectorCode);
    wxSVGDocument svgDoc;
    svgDoc.Load(memBuffer);
    return svgDoc.Render(buttonWidth,buttonHeight,NULL,true,true);
}

CTreeElementSlide& CTreeElementSlide::operator=(const CTreeElementSlide &other)
{
	visible = other.visible;
	xPos = other.xPos;
	yPos = other.yPos;
	numRow = other.numRow;
	numColumn = other.numColumn;
	themeSlide = other.themeSlide;
	positionButton = other.positionButton;
	positionSlider = other.positionSlider;
	position = other.position;
	tabValue = other.tabValue;
	exifKey = other.exifKey;
	eventInterface = other.eventInterface;
	return *this;
}

CTreeElementSlide::~CTreeElementSlide()
{

}

void CTreeElementSlide::SetPosition(const int &position)
{
	this->position = position;
}

void CTreeElementSlide::SetTheme(CThemeTreeSlide * theme)
{
	themeSlide = *theme;
}

void CTreeElementSlide::SetExifKey(const wxString &exifKey)
{
	this->exifKey = exifKey;
}

void CTreeElementSlide::SetTabValue(vector<int> value)
{
	tabValue = value;
}

void CTreeElementSlide::SetVisible(const bool &visible)
{
	this->visible = visible;
}


void CTreeElementSlide::DrawShapeElement(wxDC * dc, const wxRect &rc)
{
	float pourcentage = 0.0;
	wxRect rcPast;
	rcPast.x = rc.x;
	rcPast.y = rc.y;

	if (position > 0)
	{
		pourcentage = (float)position / (float)tabValue.size();
		rcPast.width = (rc.width * pourcentage);
		rcPast.y = rc.y;
		rcPast.height = rc.height;
		CWindowMain::FillRect(dc, rcPast, themeSlide.rectanglePast);
	}

	wxRect rcNext;
	rcNext.x = rcPast.x + rcPast.width;
	rcNext.width = rc.width - rcPast.width;
	rcNext.y = rc.y;
	rcNext.height = rc.height;
	CWindowMain::FillRect(dc, rcNext, themeSlide.rectangleNext);
}

void CTreeElementSlide::CalculZoomPosition(const int &x)
{
	float posX = x - positionSlider.x;
	float total = positionSlider.width;
	position = (posX / total) * tabValue.size();
}

void CTreeElementSlide::MouseOver(wxDC * deviceContext, const int &x, const int &y)
{
	if (mouseBlock)
	{
		wxSize renderLast = CWindowMain::GetSizeTexte(deviceContext, to_string(GetLastValue()), themeSlide.font);
		int xSlide = x - renderLast.x;
		if ((xSlide >= positionSlider.x && xSlide <= (positionSlider.x + positionSlider.width)))
		{
			::wxSetCursor(hCursorHand);
			CalculZoomPosition(xSlide);

			if (position >= tabValue.size())
				position = int(tabValue.size()) - 1;

			if (position < 0)
				position = 0;

			eventInterface->SlidePosChange(this, position, tabValue[position], exifKey);
		}
	}
}

void CTreeElementSlide::ClickLeftPage(const int &x)
{
	//Click Top Triangle
	CalculZoomPosition(x);
	if (position >= tabValue.size())
		position = int(tabValue.size()) - 1;

	if (position < 0)
		position = 0;

	eventInterface->SlidePosChange(this, position, tabValue[position], exifKey);
}

void CTreeElementSlide::ClickRightPage(const int &x)
{
	if (position >= tabValue.size())
		position = int(tabValue.size()) - 1;

	if (position < 0)
		position = 0;

	eventInterface->SlidePosChange(this, position, tabValue[position], exifKey);

	//Click Top Triangle
	CalculZoomPosition(x);
}

bool CTreeElementSlide::FindCirclePos(wxWindow * window, const int &y, const int &x)
{
	wxWindowDC dc(window);
	wxSize renderLast = CWindowMain::GetSizeTexte(&dc, to_string(GetLastValue()), themeSlide.font);

	if ((x >= (positionButton.x + renderLast.x) && x <= ((positionButton.x + positionButton.width) + renderLast.x)) && (y >= positionButton.y && y <= (positionButton.y + positionButton.height)))
	{
		return true;
	}
	return false;
}

void CTreeElementSlide::ClickElement(wxWindow * window, const int &x, const int &y)
{
	int xSlide = x;

	if (FindCirclePos(window, y, xSlide))
	{
		mouseBlock = true;
		window->CaptureMouse();
		wxSetCursor(hCursorHand);
		captureBall = true;
	}
	else if (xSlide > positionButton.width)
	{
		ClickRightPage(xSlide);
	}
	else if (xSlide < positionButton.x)
	{
		ClickLeftPage(xSlide);
	}
}

void CTreeElementSlide::UnclickElement(wxWindow * window, const int &x, const int &y)
{
	if (captureBall)
	{
		mouseBlock = false;
		if (window->GetCapture())
			window->ReleaseMouse();
	}
}

bool CTreeElementSlide::MouseBlock()
{
	return mouseBlock;
}


void CTreeElementSlide::RenderSlide(wxDC * dc, const int &width, const int &height, const int &x, const int &y)
{
	wxBitmap bitmapBuffer = wxBitmap(width, height);
	wxMemoryDC memDC(bitmapBuffer);

    this->height = height;
    
	wxRect rc;
	rc.x = 0;
	rc.width = width;
	rc.y = 0;
	rc.height = height;
	CWindowMain::FillRect(&memDC, rc, themeSlide.color);
	
	positionSlider.x = themeSlide.GetButtonWidth();
	positionSlider.width = width - (themeSlide.GetButtonWidth() * 2);
    positionSlider.y = themeSlide.GetMarge();
	positionSlider.height = height - (themeSlide.GetMarge() * 2);

	DrawShapeElement(&memDC, positionSlider);

	CalculPositionButton();
    
    if(isVector)
    {
        if(!button.IsOk() || (button.GetWidth() != themeSlide.GetButtonWidth() || button.GetHeight() != themeSlide.GetButtonHeight()))
            button = CreateFromSVG(themeSlide.GetButtonWidth(), themeSlide.GetButtonHeight(), buttonVector);
        memDC.DrawBitmap(button, positionButton.x, positionButton.y);
    }
    else
    {
        memDC.DrawBitmap(button.Rescale(themeSlide.GetButtonWidth(), themeSlide.GetButtonHeight()), positionButton.x, positionButton.y);
    }

    /*
    wxImage imageScale = button.ConvertToImage();
    imageScale = imageScale.Rescale(themeSlide.GetButtonWidth(), themeSlide.GetButtonHeight());
    
    memDC.DrawBitmap(imageScale, positionButton.x, positionButton.y);
    */
	//memDC.DrawBitmap(button, positionButton.x, positionButton.y);

	memDC.SelectObject(wxNullBitmap);

	dc->DrawBitmap(bitmapBuffer, x, y);
}


void CTreeElementSlide::CalculPositionButton()
{
	if (position > 0)
	{
		float pourcentage = (float)position / (float)tabValue.size();
		CalculPositionButton(positionSlider.x + int((float)(positionSlider.width) * pourcentage));
	}
	else
	{
		//int buttonWidth = button.GetWidth();
		CalculPositionButton(positionSlider.x);
	}
}

void CTreeElementSlide::CalculPositionButton(const int &x)
{
	int buttonWidth = themeSlide.GetButtonWidth();
	int buttonHeight = themeSlide.GetButtonHeight();
	int xPos = x - (buttonWidth / 2);
	int yPos = positionSlider.y + (positionSlider.height - buttonHeight) / 2;

	positionButton.x = xPos;
	positionButton.width = buttonWidth;
	positionButton.y = yPos;
	positionButton.height = buttonHeight;
}

int CTreeElementSlide::GetFirstValue()
{
	if (tabValue.size() > 0)
		return tabValue.at(0);

	return -1;
}

void  CTreeElementSlide::SetZoneSize(const int &width, const int &height)
{
	themeSlide.SetWidth(width);
	themeSlide.SetHeight(height);
}

void CTreeElementSlide::SetBackgroundColor(const wxColour &color)
{
	themeSlide.color = color;
}

int CTreeElementSlide::GetLastValue()
{
	if (tabValue.size() > 0)
		return tabValue.at(tabValue.size() - 1);

	return -1;
}

void CTreeElementSlide::SetElementPos(const int &x, const int &y)
{
	xPos = x;
	yPos = y;
}

int CTreeElementSlide::GetPositionValue()
{
	if (tabValue.size() > position)
		return tabValue.at(position);

	return -1;
}

void CTreeElementSlide::DrawElement(wxDC * deviceContext, const int &x, const int &y)
{
	//bool oldRender = true;

	wxBitmap bitmapBuffer = wxBitmap(themeSlide.GetWidth(), themeSlide.GetHeight());
	wxMemoryDC memDC(bitmapBuffer);

	wxRect rc;
	rc.x = 0;
	rc.width = themeSlide.GetWidth();
	rc.y = 0;
	rc.height = themeSlide.GetHeight();
	CWindowMain::FillRect(&memDC, rc, themeSlide.color);

	//int first = GetFirstValue();
	//int last = GetLastValue();

	wxSize renderFirst = CWindowMain::GetSizeTexte(deviceContext, to_string(GetPositionValue()), themeSlide.font);
	wxSize renderLast = CWindowMain::GetSizeTexte(deviceContext, to_string(GetLastValue()), themeSlide.font);

	RenderSlide(&memDC, themeSlide.GetWidth() - (renderLast.x + renderLast.x), themeSlide.GetHeight(), renderLast.x, 0);

	int yMedium = (themeSlide.GetHeight() - renderFirst.y) / 2;
	CWindowMain::DrawTexte(&memDC, to_string(GetPositionValue()), 0, yMedium, themeSlide.font);
		
	yMedium = (themeSlide.GetHeight() - renderLast.y) / 2;
	CWindowMain::DrawTexte(&memDC, to_string(GetLastValue()), themeSlide.GetWidth() - renderLast.x, yMedium, themeSlide.font);

	memDC.SelectObject(wxNullBitmap);

	deviceContext->DrawBitmap(bitmapBuffer, x, y);
}
