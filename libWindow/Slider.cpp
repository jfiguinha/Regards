#include "Slider.h"
#include <ConvertUtility.h>
#include <libResource.h>
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
	: CWindowMain(parent, id)
{
	CLoadingResource loadingResource;
	positionButton = { 0, 0, 0, 0 };
	hCursorHand = CResourceCursor::GetClosedHand();
	secondTimePast = 0;
	secondTotalTime = 0;
	timePast = L"00:00:00";
	totalTime = L"00:00:00";
	mouseBlock = false;
	this->sliderEvent = sliderEvent;
	this->themeSlider = themeSlider;
    if(isVector)
    {
        button.Create(0,0);
        buttonVector = CLibResource::GetVector("IDB_BOULESLIDER");
    }
    else
    {
        button = loadingResource.LoadImageResource("IDB_BOULESLIDER");
    }
	Connect(wxEVT_PAINT, wxPaintEventHandler(CSlider::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CSlider::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CSlider::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CSlider::OnLButtonUp));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CSlider::OnMouseCaptureLost));

}

wxImage CSlider::CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vectorCode)
{
    wxStringInputStream memBuffer(vectorCode);
    wxSVGDocument svgDoc;
    svgDoc.Load(memBuffer);
    return svgDoc.Render(buttonWidth,buttonHeight,NULL,true,true);
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
	int x = width - filenameSize.x - 5;
	int y = (height - filenameSize.y) / 2;
	DrawTexte(context, libelle, x, y, themeSlider.font);
	return x;
}

int CSlider::DrawTimePast(wxDC * context, const wxString &libelle)
{
	wxSize filenameSize = GetSizeTexte(context, libelle, themeSlider.font);
	int x = 5;
	int y = (height - filenameSize.y) / 2;
	DrawTexte(context, libelle, x, y, themeSlider.font);
	return filenameSize.x;
}

void CSlider::Draw(wxDC * context)
{
	if (width > 0 && height > 0)
	{
		wxRect rc;
		rc.x = 0;
		rc.width = width;
		rc.y = 0;
		rc.height = height;
			
		wxSize filenameSize;
		wxBitmap memBitmap = wxBitmap(width, height);
		wxMemoryDC sourceDCContext(memBitmap);
		FillRect(&sourceDCContext, rc, themeSlider.colorBack);

		//Ecriture du temps passé
		int sizeLibelleX = DrawTimePast(&sourceDCContext, timePast);
		positionSlider.x = 10 + sizeLibelleX + 5;

		//Ecriture du temps restant
		int x = DrawTotalTime(&sourceDCContext, totalTime);
		positionSlider.width = x - positionSlider.x - 5;
		positionSlider.y = (height - themeSlider.GetRectangleHeight()) / 2;
		positionSlider.height = themeSlider.GetRectangleHeight();

		DrawShapeElement(&sourceDCContext, positionSlider);
		CalculPositionButton();
        
        if(isVector)
        {
            if(!button.IsOk() || (button.GetWidth() != themeSlider.GetButtonWidth() || button.GetHeight() != themeSlider.GetButtonHeight()))
                button = CreateFromSVG(themeSlider.GetButtonWidth(), themeSlider.GetButtonHeight(), buttonVector);
            sourceDCContext.DrawBitmap(button, positionButton.x, positionButton.y);
        }
        else
        {
            sourceDCContext.DrawBitmap(button.Rescale(themeSlider.GetButtonWidth(), themeSlider.GetButtonHeight()), positionButton.x, positionButton.y);
        }



		
		sourceDCContext.SelectObject(wxNullBitmap);

		context->DrawBitmap(memBitmap, 0, 0);
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
	int yPos = (height - buttonHeight) / 2;

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
	this->Refresh();
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
			this->Refresh();
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
		this->Refresh();
	}
	else if (xPos < positionButton.x)
	{
		ClickLeftPage(xPos);
		this->Refresh();
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
	wxBufferedPaintDC dc(this);
	Draw(&dc);
}