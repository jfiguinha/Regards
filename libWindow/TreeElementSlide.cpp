#include "header.h"
#include "TreeElementSlide.h"
#include <wxSVG/SVGDocument.h>
#include <LibResource.h>
#include <ClosedHandCursor.h>
#include "WindowMain.h"
using namespace Regards::Window;



CTreeElementSlide::CTreeElementSlide(CTreeElementSlideInterface* eventInterface)
{
	//eventInterface = nullptr;
	//CLoadingResource loadingResource;
	this->eventInterface = eventInterface;
	hCursorHand = CResourceCursor::GetClosedHand();
	mouseBlock = false;
	captureBall = false;
	exifKey = L"";
	height = 0;
	position = 0;
	button.Create(0, 0);
	buttonPlus.Create(0, 0);
	buttonMoins.Create(0, 0);
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

bool CTreeElementSlide::TestEgality(CTreeElementValue * value1, CTreeElementValue * value2, int type)
{
	switch(type)
	{
	case TYPE_ELEMENT_INT:
		{
			CTreeElementValueInt * valueInt1 = (CTreeElementValueInt *)value1;
			CTreeElementValueInt * valueInt2 = (CTreeElementValueInt *)value2;
			return (valueInt1->GetValue() == valueInt2->GetValue()) ? true : false;
		}
		break;

	case TYPE_ELEMENT_FLOAT:
				{
			CTreeElementValueFloat * valueInt1 = (CTreeElementValueFloat *)value1;
			CTreeElementValueFloat * valueInt2 = (CTreeElementValueFloat *)value2;
			return (valueInt1->GetValue() == valueInt2->GetValue()) ? true : false;
		}
		break;

	case TYPE_ELEMENT_BOOL:
				{
			CTreeElementValueBool * valueInt1 = (CTreeElementValueBool *)value1;
			CTreeElementValueBool * valueInt2 = (CTreeElementValueBool *)value2;
			return (valueInt1->GetValue() == valueInt2->GetValue()) ? true : false;
		}
		break;
	}
	return false;
}


void CTreeElementSlide::SetInitValue(CTreeElementValue * value)
{
	for(int i = 0;i < tabValue->size();i++)
	{
		if(TestEgality(tabValue->at(i),value, value->GetType()))
		{
			this->position = i;
			break;
		}
	}
}


void CTreeElementSlide::SetTheme(CThemeTreeSlide * theme)
{
	themeSlide = *theme;
}


void CTreeElementSlide::SetExifKey(const wxString &exifKey)
{
	this->exifKey = exifKey;
}


void CTreeElementSlide::SetTabValue(vector<CTreeElementValue *> * value)
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
		pourcentage = (float)position / (float)tabValue->size();
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
	float posX = x - (positionSlider.x + slidePos.x);
	float total = positionSlider.width;
	position = (posX / total) * tabValue->size();
}


void CTreeElementSlide::MouseOver(wxDC * deviceContext, const int &x, const int &y)
{
	if (mouseBlock)
	{
		if (x >= (positionSlider.x + slidePos.x) && x < (positionSlider.width + positionSlider.x + slidePos.x))
		{
			::wxSetCursor(hCursorHand);
			CalculZoomPosition(x);

			if (position >= tabValue->size())
				position = int(tabValue->size()) - 1;

			if (position < 0)
				position = 0;

			eventInterface->SlidePosChange(this, position, tabValue->at(position), exifKey);
		}
	}
}


void CTreeElementSlide::TestMaxMinValue()
{
	if (position >= tabValue->size())
		position = int(tabValue->size()) - 1;

	if (position < 0)
		position = 0;
}


void CTreeElementSlide::ClickLeftPage(const int &x)
{
	//Click Top Triangle
	CalculZoomPosition(x);

	TestMaxMinValue();

	eventInterface->SlidePosChange(this, position, tabValue->at(position), exifKey);
}


void CTreeElementSlide::ClickRightPage(const int &x)
{
	//Click Top Triangle
	CalculZoomPosition(x);

	TestMaxMinValue();

	eventInterface->SlidePosChange(this, position, tabValue->at(position), exifKey);
}


bool CTreeElementSlide::FindCirclePos(wxWindow * window, const int &y, const int &x)
{
	wxClientDC dc(window);
	//wxSize renderLast = CWindowMain::GetSizeTexte(&dc, to_string(GetLastValue()), themeSlide.font);

	if ((x >= (positionButton.x + slidePos.x) && x <= ((positionButton.x + positionButton.width) + slidePos.x)) && (y >= positionButton.y && y <= (positionButton.y + positionButton.height)))
	{
		return true;
	}
	return false;
}


void CTreeElementSlide::ClickElement(wxWindow * window, const int &x, const int &y)
{
	if (x >= (positionSlider.x + slidePos.x) && x < (positionSlider.width + positionSlider.x + slidePos.x))
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
	else if (x >= moinsPos.x && x < (moinsPos.width + moinsPos.x))
	{
		//Click button moins
		position--;
		TestMaxMinValue();
		eventInterface->SlidePosChange(this, position, tabValue->at(position), exifKey);
	}
	else if (x >= plusPos.x && x < (plusPos.width + plusPos.x))
	{
		//Click button plus
		position++;
		TestMaxMinValue();
		eventInterface->SlidePosChange(this, position, tabValue->at(position), exifKey);
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
    

        if(!button.IsOk() || (button.GetWidth() != themeSlide.GetButtonWidth() || button.GetHeight() != themeSlide.GetButtonHeight()))
            button = CLibResource::CreatePictureFromSVG("IDB_BOULESLIDER", themeSlide.GetButtonWidth(), themeSlide.GetButtonHeight());
        memDC.DrawBitmap(button, positionButton.x, positionButton.y);



	memDC.SelectObject(wxNullBitmap);

	dc->DrawBitmap(bitmapBuffer, x, y);
}


void CTreeElementSlide::CalculPositionButton()
{
	if (position > 0)
	{
		float pourcentage = (float)position / (float)tabValue->size();
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


CTreeElementValue * CTreeElementSlide::GetFirstValue()
{
	if (tabValue->size() > 0)
		return tabValue->at(0);

	return nullptr;
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


CTreeElementValue * CTreeElementSlide::GetLastValue()
{
	if (tabValue->size() > 0)
		return tabValue->at(tabValue->size() - 1);

	return nullptr;
}


void CTreeElementSlide::SetElementPos(const int &x, const int &y)
{
	xPos = x;
	yPos = y;
}


CTreeElementValue * CTreeElementSlide::GetPositionValue()
{
	if (tabValue->size() > position)
		return tabValue->at(position);

	return nullptr;
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

	wxSize renderFirst = CWindowMain::GetSizeTexte(deviceContext, GetPositionValue()->GetStringValue(), themeSlide.font);
	wxSize renderLast = CWindowMain::GetSizeTexte(deviceContext, GetLastValue()->GetStringValue(), themeSlide.font);
	
	slidePos.x = renderLast.x + themeSlide.GetButtonWidth();
	slidePos.y = 0;
	slidePos.width = themeSlide.GetWidth() - (renderLast.x + renderLast.x) - (themeSlide.GetButtonWidth() * 2);
	slidePos.height = themeSlide.GetHeight();

	RenderSlide(&memDC, slidePos.width, themeSlide.GetHeight(), slidePos.x, 0);

	int yMedium = (themeSlide.GetHeight() - renderFirst.y) / 2;
	CWindowMain::DrawTexte(&memDC, GetPositionValue()->GetStringValue(), 0, yMedium, themeSlide.font);
		
	yMedium = (themeSlide.GetHeight() - renderLast.y) / 2;
	CWindowMain::DrawTexte(&memDC, GetLastValue()->GetStringValue(), themeSlide.GetWidth() - renderLast.x, yMedium, themeSlide.font);

	moinsPos.x = renderLast.x + themeSlide.GetMarge();
	moinsPos.y = positionButton.y;
	moinsPos.width = themeSlide.GetButtonWidth();
	moinsPos.height = themeSlide.GetButtonHeight();

		if (!buttonMoins.IsOk() || (buttonMoins.GetWidth() != themeSlide.GetButtonWidth() || buttonMoins.GetHeight() != themeSlide.GetButtonHeight()))
			buttonMoins = CLibResource::CreatePictureFromSVG("IDB_MINUS", themeSlide.GetButtonWidth(), themeSlide.GetButtonHeight());
		memDC.DrawBitmap(buttonMoins.ConvertToDisabled(), moinsPos.x, moinsPos.y);


	plusPos.x = themeSlide.GetWidth() - renderLast.x - themeSlide.GetButtonWidth() - themeSlide.GetMarge();
	plusPos.y = positionButton.y;
	plusPos.width = themeSlide.GetButtonWidth();
	plusPos.height = themeSlide.GetButtonHeight();


		if (!buttonPlus.IsOk() || (buttonPlus.GetWidth() != themeSlide.GetButtonWidth() || buttonPlus.GetHeight() != themeSlide.GetButtonHeight()))
			buttonPlus = CLibResource::CreatePictureFromSVG("IDB_PLUS", themeSlide.GetButtonWidth(), themeSlide.GetButtonHeight());
		memDC.DrawBitmap(buttonPlus.ConvertToDisabled(), plusPos.x, plusPos.y);

	memDC.SelectObject(wxNullBitmap);

	deviceContext->DrawBitmap(bitmapBuffer, x, y);
}
