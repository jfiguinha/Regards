#include "header.h"
#include "ToolbarSlide.h"
#include <LibResource.h>
#include <ClosedHandCursor.h>
#include "LoadingResource.h"
#include "WindowMain.h"
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
using namespace Regards::Window;

CToolbarSlide::CToolbarSlide(const CThemeSlider & themeSlider, CSliderInterface * eventInterface)
{
	y = 0;
	colorBackground = true;
	position = 0;
	this->eventInterface = eventInterface;
	this->themeSlider = themeSlider;
	positionButton = wxRect( 0, 0, 0, 0 );
	hCursorHand = CResourceCursor::GetClosedHand();
	mouseBlock = false;
	captureBall = false;
}

void CToolbarSlide::Resize(const int &tailleX, const int &tailleY)
{
	themeSlider.SetWidth(tailleX);
	themeSlider.SetHeight(tailleY);
}

CToolbarSlide::~CToolbarSlide()
{
}

int CToolbarSlide::GetWidth()
{
	return themeSlider.GetWidth();

}

int CToolbarSlide::GetHeight()
{
	return themeSlider.GetHeight();
}

void CToolbarSlide::ZoomPos(const int &position)
{
	eventInterface->ZoomPos(position);
}

void CToolbarSlide::SetPosition(const int &iPos)
{
	this->position = iPos;
}

int CToolbarSlide::GetNbValue()
{
	return tabValue.size();
}

int CToolbarSlide::GetPosition()
{
	return position;
}

void CToolbarSlide::SetTabValue(vector<int> value)
{
	tabValue = value;
}

void CToolbarSlide::CalculZoomPosition(const int &x)
{
	float posX = x - positionSlider.x;
	float total = positionSlider.width;
	position = (posX / total) * tabValue.size();
}

void CToolbarSlide::ClickLeftPage(const int &x)
{
	//Click Top Triangle
	CalculZoomPosition(x);
	if (position >= tabValue.size())
		position = int(tabValue.size()) - 1;

	if (position < 0)
		position = 0;

	eventInterface->SlidePosChange(position, L"");
}

void CToolbarSlide::ClickRightPage(const int &x)
{

	//Click Top Triangle
	CalculZoomPosition(x);

	if (position >= tabValue.size())
		position = int(tabValue.size()) - 1;

	if (position < 0)
		position = 0;

	eventInterface->SlidePosChange(position, L"");

}

bool CToolbarSlide::FindCirclePos(wxWindow * window, const int &y, const int &x)
{
	//wxWindowDC dc(window);
	//wxSize renderLast = CWindowMain::GetSizeTexte(&dc, to_string(GetLastValue()), themeSlider.font);

	int posXButtonBegin = this->x + posRectangle.x + positionButton.x;
	int posXButtonEnd = this->x + posRectangle.x + positionButton.x + positionButton.width;
	
	if ((x >= posXButtonBegin && x <= posXButtonEnd) && (y >= positionButton.y && y <= (positionButton.y + positionButton.height)))
	{
		return true;
	}
	return false;
}

void CToolbarSlide::ClickElement(wxWindow * window, const int &x, const int &y)
{
	int xSlide = x;
	//wxWindowDC dc(window);
	//wxSize renderLast = CWindowMain::GetSizeTexte(&dc, to_string(GetLastValue()), themeSlider.font);

	int posXButtonBegin = this->x + posRectangle.x + positionButton.x;
	int posXButtonEnd = this->x + posRectangle.x + positionButton.x + positionButton.width;

	if (x > (this->x + posRectangle.x) && x < (posRectangle.width + posRectangle.x + this->x))
	{
		if (FindCirclePos(window, y, xSlide))
		{
			mouseBlock = true;
			window->CaptureMouse();
			wxSetCursor(hCursorHand);
			captureBall = true;
		}
		else if (xSlide > posXButtonEnd)
		{
			ClickRightPage(xSlide - (this->x + posRectangle.x));
		}
		else if (xSlide < posXButtonBegin)
		{
			ClickLeftPage(xSlide - (this->x + posRectangle.x));
		}
	}
}

void CToolbarSlide::UnclickElement(wxWindow * window, const int &x, const int &y)
{
	if (captureBall)
	{
		mouseBlock = false;
		if (window->GetCapture())
			window->ReleaseMouse();
	}
}

bool CToolbarSlide::MouseOver(wxDC * deviceContext, const int &x, const int &y)
{
	if (mouseBlock)
	{
		wxSize renderLast = CWindowMain::GetSizeTexte(deviceContext, to_string(GetLastValue()), themeSlider.font);
		int xSlide = x - this->x - renderLast.x;
		if ((xSlide >= positionSlider.x && xSlide <= (positionSlider.x + positionSlider.width)))
		{
			::wxSetCursor(hCursorHand);
			CalculZoomPosition(xSlide);

			if (position >= tabValue.size())
				position = int(tabValue.size()) - 1;

			if (position < 0)
				position = 0;

			eventInterface->SlidePosChange(position, L"");

			return true;
		}
	}
	return false;
}

int CToolbarSlide::GetPositionValue()
{
	if (tabValue.size() > position)
		return tabValue.at(position);

	return -1;
}


int CToolbarSlide::GetLastValue()
{
	if (tabValue.size() > 0)
		return tabValue.at(tabValue.size() - 1);

	return -1;
}

int CToolbarSlide::GetFirstValue()
{
	if (tabValue.size() > 0)
		return tabValue.at(0);

	return -1;
}

void CToolbarSlide::SetBackgroundBitmap(const wxBitmap & background)
{
	colorBackground = false;
	this->background = background;
}


void CToolbarSlide::CalculPositionButton()
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

void CToolbarSlide::CalculPositionButton(const int &x)
{

    int buttonWidth = themeSlider.GetButtonWidth();
	int buttonHeight = themeSlider.GetButtonHeight();
	int xPos = x - (buttonWidth / 2);
	int yPos = positionSlider.y + (positionSlider.height - buttonHeight) / 2;

	positionButton.x = xPos;
	positionButton.width = buttonWidth;
	positionButton.y = yPos;
	positionButton.height = buttonHeight;
}



void CToolbarSlide::DrawShapeElement(wxDC * dc, const wxRect &rc)
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
		rcPast.height = themeSlider.GetRectangleHeight();
		CWindowMain::FillRect(dc, rcPast, themeSlider.rectanglePast);
	}

	wxRect rcNext;
	rcNext.x = rcPast.x + rcPast.width;
	rcNext.width = rc.width - rcPast.width;
	rcNext.y = rc.y;
	rcNext.height = themeSlider.GetRectangleHeight();
	CWindowMain::FillRect(dc, rcNext, themeSlider.rectangleNext);
}

void CToolbarSlide::RenderSlide(wxDC * dc, const int &width, const int &height, const int &x, const int &y)
{
	wxBitmap bitmapBuffer = wxBitmap(width, height);
	wxMemoryDC memDC(bitmapBuffer);
		wxRect rc;
		rc.x = 0;
		rc.width = width;
		rc.y = 0;
		rc.height = height;
		CWindowMain::FillRect(&memDC, rc, themeSlider.colorBack);
    
	if (colorBackground)
	{
		wxRect rc;
		rc.x = 0;
		rc.width = width;
		rc.y = 0;
		rc.height = height;
		CWindowMain::FillRect(&memDC, rc, themeSlider.colorBack);
	}
	else
	{
		memDC.DrawBitmap(background, 0, 0);
	}
    
	positionSlider.x = themeSlider.GetButtonWidth();
	positionSlider.width = width - (themeSlider.GetButtonWidth() * 2);
	positionSlider.y = (height - themeSlider.GetRectangleHeight()) / 2;
	positionSlider.height = themeSlider.GetRectangleHeight();

	DrawShapeElement(&memDC, positionSlider);



	memDC.SelectObject(wxNullBitmap);

	dc->DrawBitmap(bitmapBuffer, x, y);
}

void CToolbarSlide::DrawButton(wxDC * dc, const int &x, const int &y)
{
	//bool oldRender = true;

	wxBitmap bitmapBuffer = wxBitmap(themeSlider.GetWidth(), themeSlider.GetHeight());
	wxMemoryDC memDC(bitmapBuffer);


	wxRect rc;
	rc.x = 0;
	rc.width = themeSlider.GetWidth();
	rc.y = 0;
	rc.height = themeSlider.GetHeight();
	CWindowMain::FillRect(&memDC, rc, themeSlider.colorBack);

	//int first = GetFirstValue();
	//int last = GetLastValue();

	wxSize renderFirst = CWindowMain::GetSizeTexte(dc, to_string(GetPositionValue()), themeSlider.font);
	wxSize renderLast = CWindowMain::GetSizeTexte(dc, to_string(GetLastValue()), themeSlider.font);

	posRectangle.x = renderLast.x;
	posRectangle.width = themeSlider.GetWidth() - (posRectangle.x*2);
	posRectangle.y = 0;
	posRectangle.height = themeSlider.GetHeight();

	RenderSlide(&memDC, posRectangle.width, posRectangle.height, posRectangle.x, posRectangle.y);

	int yMedium = (themeSlider.GetHeight() - renderFirst.y) / 2;
	CWindowMain::DrawTexte(&memDC, to_string(GetPositionValue()), 0, yMedium, themeSlider.font);

	yMedium = (themeSlider.GetHeight() - renderLast.y) / 2;
	CWindowMain::DrawTexte(&memDC, to_string(GetLastValue()), themeSlider.GetWidth() - renderLast.x, yMedium, themeSlider.font);

	memDC.SelectObject(wxNullBitmap);

	if (!colorBackground)
	{
		bitmapBuffer.SetMask(new wxMask(bitmapBuffer, themeSlider.colorBack));
		dc->DrawBitmap(bitmapBuffer, x, y, true);
	}
	else
	{
		dc->DrawBitmap(bitmapBuffer, x, y, false);
	}

	CalculPositionButton();
    

    if(!button.IsOk() || (button.GetWidth() != themeSlider.GetButtonWidth() || button.GetHeight() != themeSlider.GetButtonHeight()))
        button = CLibResource::CreatePictureFromSVG("IDB_BOULESLIDER", themeSlider.GetButtonWidth(), themeSlider.GetButtonHeight());
    dc->DrawBitmap(button, x + renderLast.x + positionButton.x, y + positionButton.y);

}

