#include "header.h"
#include "SliderVideoSelection.h"
#include <LibResource.h>
#include <ConfigParam.h>
#include <ConvertUtility.h>
#include <ClosedHandCursor.h>
#include "WindowMain.h"
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>

CSliderVideoSelection::CSliderVideoSelection(wxWindow* parent, wxWindowID id, wxWindow * eventWnd, const CThemeSlider & themeSlider)
	: CWindowMain("CSliderVideoSelection",parent, id)
{
	isPlay = false;
	m_bMouseOver = false;
	mouseBlockTo = false;
	m_bTracking = false;
	this->eventWnd = eventWnd;
	//CLoadingResource loadingResource;
	secondTimePast = 0;
	secondTimeTo = 0;
	secondTotalTime = 0;
	positionButton = wxRect(0, 0, 0, 0);
	hCursorHand = CResourceCursor::GetClosedHand();
	secondTimePast = 0;
	secondTotalTime = 0;
	timePast = L"00:00:00";
	totalTime = L"00:00:00";
	mouseBlock = false;
    
    colorToReplace = wxColor(0,0,0);
    colorInactifReplacement= themeSlider.colorInactifReplacement; //wxColor(255,255,255);
    colorActifReplacement = themeSlider.colorActifReplacement; //wxColor(48, 128, 254);

	Connect(wxEVT_PAINT, wxPaintEventHandler(CSliderVideoSelection::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CSliderVideoSelection::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CSliderVideoSelection::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CSliderVideoSelection::OnLButtonUp));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CSliderVideoSelection::OnSize));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CSliderVideoSelection::OnMouseCaptureLost));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CSliderVideoSelection::OnMouseHover));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CSliderVideoSelection::OnMouseLeave));
	
	button.Create(0,0);

}

CSliderVideoSelection::~CSliderVideoSelection()
{
}

bool CSliderVideoSelection::IsMouseOver()
{
	return m_bMouseOver;
}

void CSliderVideoSelection::SetPlay()
{
	isPlay = true;
    Refresh();
}

void CSliderVideoSelection::SetPause()
{
	isPlay = false;
    Refresh();
}


int CSliderVideoSelection::GetWidth()
{
	return themeSlider.GetWidth() ;
}

int CSliderVideoSelection::GetHeight()
{
    
	return themeSlider.GetHeight();
}

void CSliderVideoSelection::DrawShapeElement(wxDC * dc, const wxRect &rc)
{
	float pourcentage = 0.0;
	pourcentage = (float)secondTimePast / (float)secondTotalTime;

	wxRect rcPast;
	wxRect rcBefore;
	rcBefore.x = rc.x;
	rcBefore.width = (rc.width * pourcentage);
	rcBefore.y = rc.y;
	rcBefore.height = themeSlider.GetRectangleHeight();
	CWindowMain::FillRect(dc, rcBefore, themeSlider.rectangleNext);

	if (secondTotalTime > 0)
	{
		pourcentage = (float)secondTimeTo / (float)secondTotalTime;
		rcPast.x = rcBefore.x + rcBefore.width;
		rcPast.width = (rc.width * pourcentage) - rcBefore.width;
		rcPast.y = rc.y;
		rcPast.height = themeSlider.GetRectangleHeight();
		CWindowMain::FillRect(dc, rcPast, themeSlider.rectanglePast);
	}

	wxRect rcNext;
	rcNext.x = rcPast.x + rcPast.width;
	rcNext.width = rc.width - (rcPast.width + rcBefore.width);
	rcNext.y = rc.y;
	rcNext.height = themeSlider.GetRectangleHeight();
	CWindowMain::FillRect(dc, rcNext, themeSlider.rectangleNext);
}

void CSliderVideoSelection::SetTotalSecondTime(const int64_t &secondTime)
{
	secondTotalTime = secondTime;
	totalTime = CConvertUtility::GetTimeLibelle(secondTotalTime);
	secondTimeTo = secondTime;
	this->Refresh();
}

void CSliderVideoSelection::SetPastSecondTime(const int64_t &secondTime)
{
	if (!mouseBlock)
	{
        int timeToSecond = secondTime / 1000;
        if(timeToSecond != secondTimePast)
        {
            secondTimePast = timeToSecond;
            timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
          
        }
	}
}


int CSliderVideoSelection::DrawTimePast(wxDC * context, const wxString &libelle)
{
	CThemeFont font = themeSlider.font;
	font.SetColorFont(wxColour(0, 0, 0));
	wxSize filenameSize = GetSizeTexte(context, libelle, font);
	int x = 1;
	int y = (GetWindowHeight() - filenameSize.y) / 2;
	DrawTexte(context, libelle, x, y, font);
	return filenameSize.x;
}

int CSliderVideoSelection::DrawTotalTimeLibelle(wxDC * context, const wxString &libelle)
{
	CThemeFont font = themeSlider.font;
	font.SetColorFont(wxColour(0, 0, 0));
	wxSize totalTimeSize = GetSizeTexte(context, libelle, font);

	int y = (GetWindowHeight() - totalTimeSize.y) / 2;
	int x = GetWindowWidth() - (totalTimeSize.x + 1);

	DrawTexte(context, libelle, x, y, font);

	return totalTimeSize.x;
}

void CSliderVideoSelection::UpdateScreenRatio()
{
    this->Resize();
}

void CSliderVideoSelection::Draw(wxDC * context)
{
	if (GetWindowWidth() > 0 && GetWindowHeight() > 0)
	{
		wxRect rc = GetWindowRect();

		wxSize filenameSize;
		wxBitmap memBitmap = wxBitmap(GetWindowWidth(), GetWindowHeight());
		wxMemoryDC sourceDCContext(memBitmap);
		CWindowMain::FillRect(&sourceDCContext, rc, this->GetBackgroundColour());
		
		//Ecriture du temps pass�
		int timePastSize = DrawTimePast(&sourceDCContext, timePast);
		int totalTimeSize = DrawTotalTimeLibelle(&sourceDCContext, totalTime);

		//Ecriture du slider
		positionSlider.x = themeSlider.GetMarge() + timePastSize;
		positionSlider.width = GetWindowWidth() - (2 * themeSlider.GetMarge() + totalTimeSize + timePastSize);
		positionSlider.y = (GetWindowHeight() - themeSlider.GetRectangleHeight()) / 2;
		positionSlider.height = themeSlider.GetRectangleHeight();
		DrawShapeElement(&sourceDCContext, positionSlider);

		
		CalculPositionButton();
		CalculPositionButtonTo();

		if (!button.IsOk() || (button.GetWidth() != themeSlider.GetButtonWidth() || button.GetHeight() != themeSlider.GetButtonHeight()))
			button = CLibResource::CreatePictureFromSVG("IDB_BOULESLIDER", themeSlider.GetButtonWidth(), themeSlider.GetButtonHeight());
		sourceDCContext.DrawBitmap(button, positionButton.x, positionButton.y);

		
		if (!buttonTo.IsOk() || (buttonTo.GetWidth() != themeSlider.GetButtonWidth() || buttonTo.GetHeight() != themeSlider.GetButtonHeight()))
			buttonTo = CLibResource::CreatePictureFromSVG("IDB_BOULESLIDER", themeSlider.GetButtonWidth(), themeSlider.GetButtonHeight());
		sourceDCContext.DrawBitmap(buttonTo, positionButtonTo.x, positionButtonTo.y);
		
		context->Blit(0, 0, GetWindowWidth(), GetWindowHeight(), &sourceDCContext, 0, 0);
		sourceDCContext.SelectObject(wxNullBitmap);
	}

}

void CSliderVideoSelection::CalculPositionButton()
{
	if (secondTotalTime > 0)
	{
		float pourcentage = (float)secondTimePast / (float)secondTotalTime;
		CalculPositionButton(positionSlider.x + int((float)(positionSlider.width) * pourcentage));
	}
	else
	{
		//int buttonWidth = button.GetWidth();
		CalculPositionButton(positionSlider.x);
	}
}


void CSliderVideoSelection::CalculPositionButtonTo()
{
	if (secondTotalTime > 0)
	{
		float pourcentage = (float)secondTimeTo / (float)secondTotalTime;
		CalculPositionButtonTo(positionSlider.x + int((float)(positionSlider.width) * pourcentage));
	}
	else
	{
		//int buttonWidth = button.GetWidth();
		CalculPositionButtonTo(positionSlider.x);
	}
}

void CSliderVideoSelection::CalculPositionButton(const int &x)
{
	positionXSlider = x;
	positionYSlider = (positionSlider.height + positionSlider.y) / 2;

    int buttonWidth = themeSlider.GetButtonWidth();
    int buttonHeight = themeSlider.GetButtonHeight();
	int xPos = positionXSlider - (buttonWidth / 2);
	int yPos = (GetWindowHeight() - buttonHeight) / 2;

	positionButton.x = xPos;
	positionButton.width = buttonWidth;
	positionButton.y = yPos;
	positionButton.height = buttonHeight;
}

long CSliderVideoSelection::GetTimeStart()
{
	return secondTimePast;
}
long CSliderVideoSelection::GetTimeEnd()
{
	return secondTimeTo;
}

void CSliderVideoSelection::CalculPositionButtonTo(const int &x)
{
	positionXSliderTo = x;
	positionYSliderTo = (positionSliderTo.height + positionSliderTo.y) / 2;

	int buttonWidth = themeSlider.GetButtonWidth();
	int buttonHeight = themeSlider.GetButtonHeight();
	int xPos = positionXSliderTo - (buttonWidth / 2);
	int yPos = (GetWindowHeight() - buttonHeight) / 2;

	positionButtonTo.x = xPos;
	positionButtonTo.width = buttonWidth;
	positionButtonTo.y = yPos;
	positionButtonTo.height = buttonHeight;
}

void CSliderVideoSelection::CalculTimePosition(const int &x)
{
	float posX = x - positionSlider.x;
	float total = positionSlider.width;
	secondTimePast = (double)(posX / total)* secondTotalTime;
	timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
}

void CSliderVideoSelection::CalculTimePositionTo(const int &x)
{
	float posX = x - positionSlider.x;
	float total = positionSlider.width;
	secondTimeTo = (double)(posX / total)* secondTotalTime;
	totalTime = CConvertUtility::GetTimeLibelle(secondTimeTo);
}


void CSliderVideoSelection::OnMouseMove(wxMouseEvent& event)
{
	int xPos = event.GetX();
	//int yPos = event.GetY();

	if (mouseBlock)
	{
		if ((xPos >= positionSlider.x && xPos <= (positionSlider.x + positionSlider.width)))
		{
			if ((xPos) < positionButtonTo.x)
			{
				CalculTimePosition(xPos);

				wxCommandEvent* event = new wxCommandEvent(wxEvent_SLIDERMOVE);
				event->SetExtraLong(secondTimePast);
				event->SetInt(1);
				wxQueueEvent(eventWnd, event);
			}
		}
	}
	if (mouseBlockTo)
	{
		if ((xPos >= positionSlider.x && xPos <= (positionSlider.x + positionSlider.width)))
		{
			if ((xPos - positionButton.width) > positionButton.x)
			{
				CalculTimePositionTo(xPos);
				wxCommandEvent* event = new wxCommandEvent(wxEvent_SLIDERMOVE);
				event->SetExtraLong(secondTimeTo);
				event->SetInt(2);
				wxQueueEvent(eventWnd, event);
			}

		}
	}
	Refresh();
}

void CSliderVideoSelection::OnMouseLeave(wxMouseEvent& event)
{
	m_bTracking = FALSE;
	m_bMouseOver = false;
}

void CSliderVideoSelection::OnMouseHover(wxMouseEvent& event)
{
	m_bMouseOver = true;
}

void CSliderVideoSelection::SetStartTime(const long &pos)
{
	if (secondTimePast < secondTimeTo)
	{
		secondTimePast = pos;
		timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
		this->Refresh();
	}

}

void CSliderVideoSelection::SetEndTime(const long &pos)
{
	if (secondTimeTo > secondTimePast)
	{
		secondTimeTo = pos;
		totalTime = CConvertUtility::GetTimeLibelle(secondTimeTo);
		this->Refresh();
	}
}

void CSliderVideoSelection::OnLButtonDown(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();

	if ((xPos >= positionSlider.x && xPos <= (positionSlider.x + positionSlider.width)))
	{
		if ((xPos >= positionButton.x && xPos <= (positionButton.x + positionButton.width)) && (yPos >= positionButton.y && yPos <= (positionButton.y + positionButton.height)))
		{
			mouseBlock = true;
			CaptureMouse();
			wxSetCursor(hCursorHand);
		}
		if ((xPos >= positionButtonTo.x && xPos <= (positionButtonTo.x + positionButtonTo.width)) && (yPos >= positionButtonTo.y && yPos <= (positionButtonTo.y + positionButtonTo.height)))
		{
			mouseBlockTo = true;
			CaptureMouse();
			wxSetCursor(hCursorHand);
		}
	}

    this->Refresh();
}

void CSliderVideoSelection::OnLButtonUp(wxMouseEvent& event)
{
	if (mouseBlock)
	{
		mouseBlock = false;
		ReleaseMouse();
	}
	if (mouseBlockTo)
	{
		mouseBlockTo = false;
		ReleaseMouse();
	}
}

void CSliderVideoSelection::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    int width = GetWindowWidth();
    int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;
    
    wxBitmap memBitmap(width, height);
    wxMemoryDC memDC(memBitmap);
	//wxBufferedPaintDC dc(this);
	Draw(&memDC);
    memDC.SelectObject(wxNullBitmap);
    dc.DrawBitmap(memBitmap, 0, 0);   
}