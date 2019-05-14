#include "header.h"
#include "Slider.h"
#include <ConvertUtility.h>
#include <LibResource.h>
#include <time.h>
#include <wx/dcbuffer.h>
#include <ClosedHandCursor.h>
#include "LoadingResource.h"
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
using namespace Regards::Window;


int CSlider::GetWidth()
{
	return themeSlider.GetWidth();
}

int CSlider::GetHeight()
{
	return themeSlider.GetHeight();
}

CSlider::CSlider(wxWindow* parent, wxWindowID id, CSliderInterface * sliderEvent, const CThemeSlider & themeSlider)
	: CWindowMain("CSlider",parent, id)
{
	//CLoadingResource loadingResource;
	positionButton = wxRect(0, 0, 0, 0);
	hCursorHand = CResourceCursor::GetClosedHand();
	secondTimePast = 0;
	secondTotalTime = 0;
	timePast = L"00:00:00";
	totalTime = L"00:00:00";
	mouseBlock = false;
	this->sliderEvent = sliderEvent;
	this->themeSlider = themeSlider;

    button.Create(0,0);

	Connect(wxEVT_PAINT, wxPaintEventHandler(CSlider::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CSlider::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CSlider::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CSlider::OnLButtonUp));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CSlider::OnMouseCaptureLost));

}

CSlider::~CSlider()
{

}

void CSlider::DrawShapeElement(wxDC * dc, const wxRect &rc)
{
	float pourcentage = 0.0;
	wxRect rcPast;
	rcPast.x = rc.x;
	rcPast.y = rc.y;

	if (secondTotalTime > 0)
	{
		pourcentage = (float)secondTimePast / (float)secondTotalTime;
		rcPast.width = (rc.width * pourcentage);
		rcPast.y = rc.y;
		rcPast.height = themeSlider.GetRectangleHeight();
		FillRect(dc, rcPast, themeSlider.rectanglePast);
	}

	wxRect rcNext;
	rcNext.x = rcPast.x + rcPast.width;
	rcNext.width = rc.width - rcPast.width;
	rcNext.y = rc.y;
	rcNext.height = themeSlider.GetRectangleHeight();
	FillRect(dc, rcNext, themeSlider.rectangleNext);
}

void CSlider::SetTotalSecondTime(const int64_t &secondTime)
{
	wxWindowDC winDC(this);
	totalTimeInMilliseconds = secondTime;
	secondTotalTime = (float)secondTime / (float)1000;
	totalTime = CConvertUtility::GetTimeLibelle(secondTotalTime);
	DrawTexte(&winDC, totalTime, positionTexteTotal.x, positionTexteTotal.y, themeSlider.font);
}

void CSlider::SetPastTime(const int64_t &secondTime)
{
	wxWindowDC winDC(this);
	totalPastTimeInMilliseconds = secondTime;
	secondTimePast = (float)secondTime / (float)1000;
	timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
	//Draw(&winDC);
	DrawTimePast(&winDC, timePast);
}

void CSlider::SetPastSecondTime(const int64_t &secondTime)
{
	if (!mouseBlock)
	{
		SetPastTime(secondTime);
	}
}

int CSlider::DrawTotalTime(wxDC * context, const wxString &libelle)
{
	wxSize filenameSize = GetSizeTexte(context, libelle, themeSlider.font);
	int x = GetWindowWidth() - filenameSize.x - 5;
	int y = (GetWindowHeight() - filenameSize.y) / 2;
	DrawTexte(context, libelle, x, y, themeSlider.font);
	return x;
}

int CSlider::DrawTimePast(wxDC * context, const wxString &libelle)
{
	wxSize filenameSize = GetSizeTexte(context, libelle, themeSlider.font);
	int x = 5;
	int y = (GetWindowHeight() - filenameSize.y) / 2;
	DrawTexte(context, libelle, x, y, themeSlider.font);
	return filenameSize.x;
}

void CSlider::Draw(wxDC * context)
{
	if (GetWindowWidth() > 0 && GetWindowHeight() > 0)
	{
		wxRect rc = GetWindowRect();
			
		wxSize filenameSize;
		wxBitmap memBitmap = wxBitmap(GetWindowWidth(), GetWindowHeight());
		wxMemoryDC sourceDCContext(memBitmap);
		FillRect(&sourceDCContext, rc, themeSlider.colorBack);

		//Ecriture du temps passé
		int sizeLibelleX = DrawTimePast(&sourceDCContext, timePast);
		positionSlider.x = 10 + sizeLibelleX + 5;

		//Ecriture du temps restant
		int x = DrawTotalTime(&sourceDCContext, totalTime);
		positionSlider.width = x - positionSlider.x - 5;
		positionSlider.y = (GetWindowHeight() - themeSlider.GetRectangleHeight()) / 2;
		positionSlider.height = themeSlider.GetRectangleHeight();

		DrawShapeElement(&sourceDCContext, positionSlider);
		CalculPositionButton();

        if(!button.IsOk() || (button.GetWidth() != themeSlider.GetButtonWidth() || button.GetHeight() != themeSlider.GetButtonHeight()))
            button = CLibResource::CreatePictureFromSVG("IDB_BOULESLIDER", themeSlider.GetButtonWidth(), themeSlider.GetButtonHeight());
        sourceDCContext.DrawBitmap(button, positionButton.x, positionButton.y);
		
		sourceDCContext.SelectObject(wxNullBitmap);

#ifdef __WXGTK__
    double scale_factor = context->GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif 

        if(scale_factor != 1.0)
        {
            wxImage image = memBitmap.ConvertToImage();
            wxBitmap resized(image, wxBITMAP_SCREEN_DEPTH, scale_factor);
            context->DrawBitmap(resized, 0, 0);
        }
        else
        {
            context->DrawBitmap(memBitmap, 0, 0);
        }

		
	}

}

void CSlider::CalculPositionButton()
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

void CSlider::CalculPositionButton(const int &x)
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

void CSlider::CalculTimePosition(const int &x)
{
	float posX = x - positionSlider.x;
	float total = positionSlider.width;
	SetPastTime((int)((posX / total) * totalTimeInMilliseconds));
}

void CSlider::ClickLeftPage(const int &x)
{
	//Click Top Triangle
	CalculTimePosition(x);
	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(totalPastTimeInMilliseconds);

}

void CSlider::ClickRightPage(const int &x)
{
	//Click Top Triangle
	CalculTimePosition(x);
	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(totalPastTimeInMilliseconds);

}

void CSlider::Resize()
{
	this->FastRefresh(this);
}

void CSlider::UpdatePositionEvent()
{
	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(totalPastTimeInMilliseconds);
}

void CSlider::OnMouseMove(wxMouseEvent& event)
{
	if (mouseBlock)
	{
		int xPos = event.GetX();
		//int yPos = event.GetY();
		if ((xPos >= positionSlider.x && xPos <= (positionSlider.x + positionSlider.width)))
		{
			CalculTimePosition(xPos);
			this->FastRefresh(this);
		}
	}
}

void CSlider::OnLButtonDown(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();
	if ((xPos >= positionButton.x && xPos <= (positionButton.x + positionButton.width)) && (yPos >= positionButton.y && yPos <= (positionButton.y + positionButton.height)))
	{
		mouseBlock = true;
		CaptureMouse();
		wxSetCursor(hCursorHand);
		//hCursorHand.SetCursor();
	}
	else if (xPos > positionButton.width)
	{
		ClickRightPage(xPos);
		this->FastRefresh(this);
	}
	else if (xPos < positionButton.x)
	{
		ClickLeftPage(xPos);
		this->FastRefresh(this);
	}
}

void CSlider::OnLButtonUp(wxMouseEvent& event)
{
	mouseBlock = false;
    if(HasCapture())
        ReleaseMouse();

	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(totalPastTimeInMilliseconds);

}


void CSlider::OnPaint(wxPaintEvent& event)
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();
    if(width == 0 || height == 0)
        return;

    
	wxBufferedPaintDC dc(this);
	Draw(&dc);
}