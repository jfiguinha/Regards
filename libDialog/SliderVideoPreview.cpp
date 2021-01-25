#include "header.h"
#include "SliderVideoPreview.h"
#include <LibResource.h>
#include <ConfigParam.h>
#include <ConvertUtility.h>
#include <ClosedHandCursor.h>
#include "WindowMain.h"
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
using namespace Regards::Control;

wxDEFINE_EVENT(TIMER_MOUSE, wxTimerEvent);

CSliderVideoPreview::CSliderVideoPreview(wxWindow* parent, wxWindowID id, CSliderInterface * sliderEvent, const CThemeSlider & themeSlider)
	: CWindowMain("CSliderVideoPreview",parent, id)
{
	isPlay = false;
	m_bMouseOver = false;
	m_bTracking = false;

	//CLoadingResource loadingResource;
	secondTimePast = 0;
	secondTotalTime = 0;
	positionButton = wxRect(0, 0, 0, 0);
	hCursorHand = CResourceCursor::GetClosedHand();
	secondTimePast = 0;
	secondTotalTime = 0;
	timePast = L"00:00:00";
	totalTime = L"00:00:00";
	mouseBlock = false;
	this->sliderEvent = sliderEvent;
    
    colorToReplace = wxColor(0,0,0);
    colorInactifReplacement= themeSlider.colorInactifReplacement; //wxColor(255,255,255);
    colorActifReplacement = themeSlider.colorActifReplacement; //wxColor(48, 128, 254);

	Connect(wxEVT_PAINT, wxPaintEventHandler(CSliderVideoPreview::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CSliderVideoPreview::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CSliderVideoPreview::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CSliderVideoPreview::OnLButtonUp));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CSliderVideoPreview::OnSize));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CSliderVideoPreview::OnMouseCaptureLost));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CSliderVideoPreview::OnMouseHover));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CSliderVideoPreview::OnMouseLeave));
	
	button.Create(0,0);

	mouseTimer = new wxTimer(this, TIMER_MOUSE);
	Connect(TIMER_MOUSE, wxEVT_TIMER, wxTimerEventHandler(CSliderVideoPreview::OnMouseClickTimer), nullptr, this);
	m_animation = new wxActivityIndicator(this, wxID_ANY);
	m_animation->SetSize(wxSize(this->GetHeight(), this->GetHeight()));
	m_animation->SetBackgroundColour(themeSlider.colorBack);
	
}

void CSliderVideoPreview::OnMouseClickTimer(wxTimerEvent& event)
{
	if (isNextButton)
	{
		secondTimePast++;
		if (secondTimePast > secondTotalTime)
			secondTimePast = secondTotalTime;
		timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
		if (sliderEvent != nullptr)
			sliderEvent->SlidePosChange(secondTimePast, "Click");
	}
	else 
	{
		secondTimePast--;
		if (secondTimePast < 0)
			secondTimePast = 0;

		timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
		if (sliderEvent != nullptr)
			sliderEvent->SlidePosChange(secondTimePast, "Click");
	}
	this->Refresh();
	mouseTimer->Start(100, true);
}

CSliderVideoPreview::~CSliderVideoPreview()
{
	mouseTimer->Stop();
	delete mouseTimer;
	delete m_animation;
}

bool CSliderVideoPreview::IsMouseOver()
{
	return m_bMouseOver;
}

void CSliderVideoPreview::SetPlay()
{
	isPlay = true;
    Refresh();
}

void CSliderVideoPreview::SetPause()
{
	isPlay = false;
    Refresh();
}


int CSliderVideoPreview::GetWidth()
{
	return themeSlider.GetWidth() ;
}

int CSliderVideoPreview::GetHeight()
{
    
	return themeSlider.GetHeight();
}

void CSliderVideoPreview::DrawShapeElement(wxDC * dc, const wxRect &rc)
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
		CWindowMain::FillRect(dc, rcPast, themeSlider.rectanglePast);
	}

	wxRect rcNext;
	rcNext.x = rcPast.x + rcPast.width;
	rcNext.width = rc.width - rcPast.width;
	rcNext.y = rc.y;
	rcNext.height = themeSlider.GetRectangleHeight();
	CWindowMain::FillRect(dc, rcNext, themeSlider.rectangleNext);
}

void CSliderVideoPreview::SetTotalSecondTime(const int64_t &secondTime)
{
	secondTotalTime = secondTime / 1000;
	totalTime = CConvertUtility::GetTimeLibelle(secondTotalTime);

}

float CSliderVideoPreview::GetTimePos()
{
	return secondTimePast;
}

void CSliderVideoPreview::SetPastSecondTime(const int64_t &secondTime)
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

void CSliderVideoPreview::Start()
{
	m_animation->Show();
	m_animation->Start();
}

void CSliderVideoPreview::Stop()
{
	m_animation->Stop();
	m_animation->Hide();
}

int CSliderVideoPreview::DrawTimePast(wxDC * context, const wxString &libelle)
{
	wxSize filenameSize = GetSizeTexte(context, libelle, themeSlider.font);
	int x = themeSlider.GetMarge() + themeSlider.GetButtonPlayWidth();
	int y = (GetWindowHeight() - filenameSize.y) / 2;
	DrawTexte(context, libelle, x, y, themeSlider.font);
	return filenameSize.x;
}

int CSliderVideoPreview::DrawVolumeLibelle(wxDC * context, const wxString &libelle)
{
	wxSize volumeSize = GetSizeTexte(context, libelle, themeSlider.font);

	int y = (GetWindowHeight() - volumeSize.y) / 2;
	int x = GetWindowWidth() - (volumeSize.x + themeSlider.GetMarge() / 2) - themeSlider.GetButtonVolumeUpWidth();

	DrawTexte(context, libelle, x, y, themeSlider.font);

	return volumeSize.x;
}


int CSliderVideoPreview::DrawTotalTimeLibelle(wxDC * context, const wxString &libelle, const int &volumePos)
{
	wxSize totalTimeSize = GetSizeTexte(context, libelle, themeSlider.font);

	int y = (GetWindowHeight() - totalTimeSize.y) / 2;
	int x = GetWindowWidth() - (volumePos + themeSlider.GetMarge()) - (totalTimeSize.x + themeSlider.GetMarge() / 2) - themeSlider.GetButtonSpeakerWidth() - themeSlider.GetButtonVolumeUpWidth() - themeSlider.GetButtonVolumeDownWidth();

	DrawTexte(context, libelle, x, y, themeSlider.font);

	return totalTimeSize.x;
}

void CSliderVideoPreview::UpdateScreenRatio()
{
    this->Resize();
}

void CSliderVideoPreview::Draw(wxDC * context)
{
	if (GetWindowWidth() > 0 && GetWindowHeight() > 0)
	{
		wxRect rc = GetWindowRect();

		wxSize filenameSize;
		wxBitmap memBitmap = wxBitmap(GetWindowWidth(), GetWindowHeight());
		wxMemoryDC sourceDCContext(memBitmap);
		CWindowMain::FillRect(&sourceDCContext, rc, themeSlider.colorBack);
		
		//Ecriture du temps pass�
		int timePastSize = DrawTimePast(&sourceDCContext, timePast);
		int volumeSize = 0;// DrawVolumeLibelle(&sourceDCContext, libelleVolume);
		int totalTimeSize = DrawTotalTimeLibelle(&sourceDCContext, totalTime, volumeSize);

		//Ecriture du slider
		positionSlider.x = themeSlider.GetMarge() + timePastSize + themeSlider.GetMarge() + themeSlider.GetButtonPlayWidth();
		positionSlider.width = (GetWindowWidth() - (volumeSize + themeSlider.GetMarge()) - (totalTimeSize + themeSlider.GetMarge()) - themeSlider.GetButtonSpeakerWidth() - themeSlider.GetButtonVolumeUpWidth() - themeSlider.GetButtonVolumeDownWidth()) - positionSlider.x;
		positionSlider.y = (GetWindowHeight() - themeSlider.GetRectangleHeight()) / 2;
		positionSlider.height = themeSlider.GetRectangleHeight();
		DrawShapeElement(&sourceDCContext, positionSlider);

		
		CalculPositionButton();

        if(!button.IsOk() || (button.GetWidth() != themeSlider.GetButtonWidth() || button.GetHeight() != themeSlider.GetButtonHeight()))
            button = CLibResource::CreatePictureFromSVG("IDB_BOULESLIDER", themeSlider.GetButtonWidth(), themeSlider.GetButtonHeight());
        sourceDCContext.DrawBitmap(button, positionButton.x, positionButton.y);

		int xButtonPos = positionSlider.x + positionSlider.width + (totalTimeSize + themeSlider.GetMarge());
		InsertPreviousButton(xButtonPos, &sourceDCContext);

		xButtonPos += themeSlider.GetButtonSpeakerWidth() + 5;
		InsertNextButton(xButtonPos, &sourceDCContext);

		context->Blit(0, 0, GetWindowWidth(), GetWindowHeight(), &sourceDCContext, 0, 0);
		sourceDCContext.SelectObject(wxNullBitmap);
	}

}


void CSliderVideoPreview::InsertPreviousButton(const int &xStart, wxDC * context)
{
	wxImage bmp;


	if (!buttonImagePrevious.IsOk() || (buttonImagePrevious.GetWidth() != themeSlider.GetButtonVolumeDownWidth() || buttonImagePrevious.GetHeight() != themeSlider.GetButtonVolumeDownHeight()))
	{
		buttonImagePrevious = CLibResource::CreatePictureFromSVG("IDB_ARROWLPNG", themeSlider.GetButtonVolumeDownWidth(), themeSlider.GetButtonVolumeDownHeight());
	}
	bmp = buttonImagePrevious;


	int yPos = themeSlider.GetButtonVolumeDownHeight();//buttonVolumeDown.GetHeight();
	if (!buttonPreviousActif)
	{
		bmp = bmp.ConvertToGreyscale();
		bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
			colorInactifReplacement.Red(), colorInactifReplacement.Green(), colorInactifReplacement.Blue());
	}
	else
		bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
			colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());

	int yPosBitmap = (GetWindowHeight() - yPos) / 2;
	context->DrawBitmap(bmp, xStart, yPosBitmap);

	positionPreviousButton.x = xStart;
	positionPreviousButton.width = bmp.GetWidth();
	positionPreviousButton.y = yPosBitmap;
	positionPreviousButton.height = bmp.GetHeight();
}


void CSliderVideoPreview::InsertNextButton(const int &xStart, wxDC * context)
{
	wxImage bmp;


	if (!buttonImageNext.IsOk() || (buttonImageNext.GetWidth() != themeSlider.GetButtonVolumeDownWidth() || buttonImageNext.GetHeight() != themeSlider.GetButtonVolumeDownHeight()))
	{
		buttonImageNext = CLibResource::CreatePictureFromSVG("IDB_ARROWRPNG", themeSlider.GetButtonVolumeDownWidth(), themeSlider.GetButtonVolumeDownHeight());
	}
	bmp = buttonImageNext;


	int yPos = themeSlider.GetButtonVolumeDownHeight();//buttonVolumeDown.GetHeight();
	if (!buttonNextActif)
	{
		bmp = bmp.ConvertToGreyscale();
		bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
			colorInactifReplacement.Red(), colorInactifReplacement.Green(), colorInactifReplacement.Blue());
	}
	else
		bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
			colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());

	int yPosBitmap = (GetWindowHeight() - yPos) / 2;
	context->DrawBitmap(bmp, xStart, yPosBitmap);

	positionNextButton.x = xStart;
	positionNextButton.width = bmp.GetWidth();
	positionNextButton.y = yPosBitmap;
	positionNextButton.height = bmp.GetHeight();
}

void CSliderVideoPreview::CalculPositionButton()
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

void CSliderVideoPreview::CalculPositionButton(const int &x)
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

void CSliderVideoPreview::CalculTimePosition(const int &x)
{
	float posX = x - positionSlider.x;
	float total = positionSlider.width;
	secondTimePast = (double)(posX / total)* secondTotalTime;
	timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
}

void CSliderVideoPreview::ClickLeftPage(const int &x)
{
	//Click Top Triangle
	CalculTimePosition(x);
	if (sliderEvent != nullptr)
		sliderEvent->SlidePosChange(secondTimePast, "Click");

}

void CSliderVideoPreview::ClickRightPage(const int &x)
{
	//Click Top Triangle
	CalculTimePosition(x);
	if (sliderEvent != nullptr)
		sliderEvent->SlidePosChange(secondTimePast, "Click");

}

void CSliderVideoPreview::UpdatePositionEvent()
{
	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(secondTimePast);

}

void CSliderVideoPreview::OnMouseMove(wxMouseEvent& event)
{
	int xPos = event.GetX();

	if (mouseBlock)
	{
		if ((xPos >= positionSlider.x && xPos <= (positionSlider.x + positionSlider.width)))
		{
			CalculTimePosition(xPos);
			if (sliderEvent != nullptr)
				sliderEvent->SlidePosChange(secondTimePast, "Move");
		}
	}
	else
	{
		if (xPos >= positionNextButton.x && xPos <= (positionNextButton.x + positionNextButton.width))
		{
			buttonNextActif = true;
		}
		else if (xPos >= positionPreviousButton.x && xPos <= (positionPreviousButton.x + positionPreviousButton.width))
		{
			buttonPreviousActif = true;
		}
		else
		{
			buttonNextActif = false;
			buttonPreviousActif = false;
		}
	}
	Refresh();
}

void CSliderVideoPreview::OnMouseLeave(wxMouseEvent& event)
{
	m_bTracking = FALSE;
	m_bMouseOver = false;
}

void CSliderVideoPreview::OnMouseHover(wxMouseEvent& event)
{
	m_bMouseOver = true;
}

void CSliderVideoPreview::OnLButtonDown(wxMouseEvent& event)
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
		else if (xPos > (positionButton.width + positionButton.x))
		{
			ClickRightPage(xPos);
		}
		else if (xPos < positionButton.x)
		{
			ClickLeftPage(xPos);
		}
	}
	else
	{
        wxSetCursor(wxCURSOR_ARROW);
		if (xPos >= positionNextButton.x && xPos <= (positionNextButton.x + positionNextButton.width))
		{
			isNextButton = true;
			wxSetCursor(hCursorHand);
			secondTimePast++;
			if (secondTimePast > secondTotalTime)
				secondTimePast = secondTotalTime;
			timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
			if (sliderEvent != nullptr)
				sliderEvent->SlidePosChange(secondTimePast, "Click");

			mouseTimer->Start(100, true);
		}
		else if (xPos >= positionPreviousButton.x && xPos <= (positionPreviousButton.x + positionPreviousButton.width))
		{
			isNextButton = false;
			secondTimePast--;
			if (secondTimePast < 0)
				secondTimePast = 0;
			wxSetCursor(hCursorHand);
			timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
			if (sliderEvent != nullptr)
				sliderEvent->SlidePosChange(secondTimePast, "Click");

			mouseTimer->Start(100, true);
		}
	}
    this->Refresh();
}

void CSliderVideoPreview::OnLButtonUp(wxMouseEvent& event)
{
	mouseTimer->Stop();
	if (mouseBlock)
	{
		mouseBlock = false;
		ReleaseMouse();

		if (sliderEvent != nullptr)
			sliderEvent->MoveSlider(secondTimePast);
	}
}

void CSliderVideoPreview::OnPaint(wxPaintEvent& event)
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