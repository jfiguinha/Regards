#include "header.h"
#include "Slider.h"
#include <ConvertUtility.h>
#include <LibResource.h>
#include <wx/dcbuffer.h>
#include <ClosedHandCursor.h>
#include "SliderInterface.h"
using namespace Regards::Window;


int CSlider::GetWidth()
{
	return themeSlider.GetWidth();
}

int CSlider::GetHeight()
{
	return themeSlider.GetHeight();
}

CSlider::CSlider(wxWindow* parent, wxWindowID id, CSliderInterface* sliderEvent, const CThemeSlider& themeSlider)
	: CWindowMain("CSlider", parent, id)
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

	button.Create(0, 0);

	Connect(wxEVT_PAINT, wxPaintEventHandler(CSlider::on_paint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CSlider::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CSlider::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CSlider::OnLButtonUp));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CSlider::OnMouseLeave));
}

void CSlider::DrawShapeElement(wxDC* dc, const wxRect& rc)
{
	wxRect rcPast;
	rcPast.x = rc.x;

	if (secondTotalTime > 0)
	{
		const float pourcentage = secondTimePast / secondTotalTime;
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

void CSlider::SetTotalSecondTime(const int64_t& secondTime)
{
	totalTimeInMilliseconds = secondTime;
	secondTotalTime = static_cast<float>(secondTime) / static_cast<float>(1000);
	totalTime = CConvertUtility::GetTimeLibelle(secondTotalTime);
	Refresh(false);
}

void CSlider::SetPastTime(const int64_t& secondTime)
{
	totalPastTimeInMilliseconds = secondTime;
	secondTimePast = static_cast<float>(secondTime) / static_cast<float>(1000);
	timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
	Refresh(false);
}

void CSlider::SetPastSecondTime(const int64_t& secondTime)
{
	if (!mouseBlock)
	{
		SetPastTime(secondTime);
	}
}

int CSlider::DrawTotalTime(wxDC* context, const wxString& libelle)
{
	wxSize filenameSize = GetSizeTexte(context, libelle, themeSlider.font);
	int x = GetWindowWidth() - filenameSize.x - 5;
	int y = (GetWindowHeight() - filenameSize.y) / 2;
	DrawTexte(context, libelle, x, y, themeSlider.font);
	return x;
}

int CSlider::DrawTimePast(wxDC* context, const wxString& libelle)
{
	wxSize filenameSize = GetSizeTexte(context, libelle, themeSlider.font);
	int x = 5;
	int y = (GetWindowHeight() - filenameSize.y) / 2;
	DrawTexte(context, libelle, x, y, themeSlider.font);
	return filenameSize.x;
}

void CSlider::Draw(wxDC* context)
{
	if (GetWindowWidth() > 0 && GetWindowHeight() > 0)
	{
		wxRect rc = GetWindowRect();

		FillRect(context, rc, themeSlider.colorBack);

		//Ecriture du temps passé
		int sizeLibelleX = DrawTimePast(context, timePast);
		positionSlider.x = 10 + sizeLibelleX + 5;

		//Ecriture du temps restant
		int x = DrawTotalTime(context, totalTime);
		positionSlider.width = x - positionSlider.x - 5;
		positionSlider.y = (GetWindowHeight() - themeSlider.GetRectangleHeight()) / 2;
		positionSlider.height = themeSlider.GetRectangleHeight();

		DrawShapeElement(context, positionSlider);
		CalculPositionButton();

		if (!button.IsOk() || (button.GetWidth() != themeSlider.GetButtonWidth() || button.GetHeight() != themeSlider.
			GetButtonHeight()))
			button = CLibResource::CreatePictureFromSVG("IDB_BOULESLIDER", themeSlider.GetButtonWidth(),
			                                            themeSlider.GetButtonHeight());
		context->DrawBitmap(button, positionButton.x, positionButton.y);
	}
}

void CSlider::CalculPositionButton()
{
	if (secondTotalTime > 0)
	{
		float pourcentage = secondTimePast / secondTotalTime;
		CalculPositionButton(
			positionSlider.x + static_cast<int>(static_cast<float>(positionSlider.width) * pourcentage));
	}
	else
	{
		//int buttonWidth = button.GetWidth();
		CalculPositionButton(positionSlider.x);
	}
}

void CSlider::CalculPositionButton(const int& x)
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

void CSlider::CalculTimePosition(const int& x)
{
	float posX = x - positionSlider.x;
	float total = positionSlider.width;
	SetPastTime(static_cast<int>((posX / total) * totalTimeInMilliseconds));
}

void CSlider::ClickLeftPage(const int& x)
{
	//Click Top Triangle
	CalculTimePosition(x);
	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(totalPastTimeInMilliseconds);
}

void CSlider::ClickRightPage(const int& x)
{
	//Click Top Triangle
	CalculTimePosition(x);
	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(totalPastTimeInMilliseconds);
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
			Refresh(false);
		}
	}
}

void CSlider::OnLButtonDown(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();
	if ((xPos >= positionButton.x && xPos <= (positionButton.x + positionButton.width)) && (yPos >= positionButton.y &&
		yPos <= (positionButton.y + positionButton.height)))
	{
		mouseBlock = true;
		CaptureMouse();
		wxSetCursor(hCursorHand);
		//hCursorHand.SetCursor();
	}
	else if (xPos > positionButton.width)
	{
		ClickRightPage(xPos);
	}
	else if (xPos < positionButton.x)
	{
		ClickLeftPage(xPos);
	}
	Refresh(false);
}

void CSlider::OnLButtonUp(wxMouseEvent& event)
{
	mouseBlock = false;
	if (HasCapture())
		ReleaseMouse();

	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(totalPastTimeInMilliseconds);
}


void CSlider::OnMouseLeave(wxMouseEvent& event)
{
	mouseBlock = false;
	if (HasCapture())
		ReleaseMouse();


	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(totalPastTimeInMilliseconds);
}


void CSlider::on_paint(wxPaintEvent& event)
{
	wxSize size = GetClientSize();
	if (size.x <= 0 || size.y <= 0)
		return;

	wxBufferedPaintDC dc(this);

	Draw(&dc);
}
