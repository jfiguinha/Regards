#include "SliderVideo.h"
#include <libResource.h>
#include <SDL.h>
#include <time.h>
#include <ParamInit.h>
#include <ConfigParam.h>
#include <wx/dcbuffer.h>
#include <ConvertUtility.h>
#include <ClosedHandCursor.h>
#include "WindowMain.h"
#include "LoadingResource.h"
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
using namespace Regards::Video;


CSliderVideo::CSliderVideo(wxWindow* parent, wxWindowID id, CSliderInterface * sliderEvent, const CThemeSlider & themeSlider)
	: CWindowMain(parent, id)
{
	CLoadingResource loadingResource;
	secondTimePast = 0;
	secondTotalTime = 0;
	positionButton = { 0, 0, 0, 0 };
	hCursorHand = CResourceCursor::GetClosedHand();
	secondTimePast = 0;
	secondTotalTime = 0;
	timePast = L"00:00:00";
	totalTime = L"00:00:00";
	libelleVolume = L"100%";
	mouseBlock = false;
	this->sliderEvent = sliderEvent;
    
    colorToReplace = wxColor(0,0,0);
    colorInactifReplacement= wxColor(255,255,255);
    colorActifReplacement = wxColor(48, 128, 254);

	Connect(wxEVT_PAINT, wxPaintEventHandler(CSliderVideo::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CSliderVideo::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CSliderVideo::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CSliderVideo::OnLButtonUp));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CSliderVideo::OnSize));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CSliderVideo::OnMouseCaptureLost));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CSliderVideo::OnMouseHover));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CSliderVideo::OnMouseLeave));

    //button = loadingResource.LoadImageResource(L"IDB_BOULESLIDER");
    
    if(isVector)
    {
        button.Create(0,0);
        buttonVector = CLibResource::GetVector(L"IDB_BOULESLIDER");
        buttonPlayVector = CLibResource::GetVector(L"IDB_PLAY_VIDEO");
        buttonPauseVector = CLibResource::GetVector(L"IDB_PAUSE_VIDEO");
        buttonVolumeUpVector = CLibResource::GetVector(L"IDB_PLUS");
        buttonVolumeDownVector = CLibResource::GetVector(L"IDB_MINUS");
        buttonSpeakerVector = CLibResource::GetVector(L"IDB_VOLUME_UP_VIDEO");
    }
    else
    {
        button = loadingResource.LoadImageResource(L"IDB_BOULESLIDER");
        buttonPlay = loadingResource.LoadImageResource(L"IDB_PLAY_VIDEO");
        buttonPause = loadingResource.LoadImageResource(L"IDB_PAUSE_VIDEO");
        buttonVolumeUp = loadingResource.LoadImageResource(L"IDB_PLUS");
        buttonVolumeDown = loadingResource.LoadImageResource(L"IDB_MINUS");
        buttonSpeaker = loadingResource.LoadImageResource(L"IDB_VOLUME_UP_VIDEO");
    }
}

wxImage CSliderVideo::CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vectorCode)
{
    wxStringInputStream memBuffer(vectorCode);
    wxSVGDocument svgDoc;
    svgDoc.Load(memBuffer);
    return svgDoc.Render(buttonWidth,buttonHeight,NULL,true,true);
}

CSliderVideo::~CSliderVideo()
{
}

bool CSliderVideo::IsMouseOver()
{
	return m_bMouseOver;
}

void CSliderVideo::SetPlay()
{
	isPlay = true;
	this->Refresh();
}

void CSliderVideo::SetPause()
{
	isPlay = false;
	this->Refresh();
}


int CSliderVideo::GetWidth()
{
	return themeSlider.GetWidth();
}

int CSliderVideo::GetHeight()
{
	return themeSlider.GetHeight();
}

void CSliderVideo::DrawShapeElement(wxDC * dc, const wxRect &rc)
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

void CSliderVideo::SetTotalSecondTime(const int64_t &secondTime)
{
	//wxWindowDC winDC = wxWindowDC(this);
	totalTimeInMilliseconds = secondTime;
	secondTotalTime = (float)secondTime / (float)1000000;
	totalTime = CConvertUtility::GetTimeLibelle(secondTotalTime);
	//DrawTexte(&winDC, totalTime, positionTexteTotal.x, positionTexteTotal.y);
	this->Refresh();

}



void CSliderVideo::SetPastSecondTime(const int64_t &secondTime)
{
	if (!mouseBlock)
	{
		totalPastTimeInMilliseconds = secondTime;
		secondTimePast = (float)secondTime / (float)1000;
		timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
		this->Refresh();
	}
}

void CSliderVideo::SetVolumePos(const long &volume)
{
	this->volume = volume;
	libelleVolume = L"";
	
	if (volume < 10)
	{
		libelleVolume.append(L"0");
		libelleVolume.append(std::to_string(volume));
		libelleVolume.append(L"%");
	}
	else
	{
		libelleVolume.append(std::to_string(volume));
		libelleVolume.append(L"%");
	}
}

int CSliderVideo::DrawTimePast(wxDC * context, const wxString &libelle)
{
	wxSize filenameSize = GetSizeTexte(context, libelle, themeSlider.font);
	int x = themeSlider.GetMarge() + themeSlider.GetButtonPlayWidth();
	int y = (height - filenameSize.y) / 2;
	DrawTexte(context, libelle, x, y, themeSlider.font);
	return filenameSize.x;
}

int CSliderVideo::DrawVolumeLibelle(wxDC * context, const wxString &libelle)
{
	wxSize volumeSize = GetSizeTexte(context, libelle, themeSlider.font);

	int y = (height - volumeSize.y) / 2;
	int x = width - (volumeSize.x + themeSlider.GetMarge() / 2) - themeSlider.GetButtonVolumeUpWidth();

	DrawTexte(context, libelle, x, y, themeSlider.font);

	return volumeSize.x;
}


int CSliderVideo::DrawTotalTimeLibelle(wxDC * context, const wxString &libelle, const int &volumePos)
{
	wxSize totalTimeSize = GetSizeTexte(context, libelle, themeSlider.font);

	int y = (height - totalTimeSize.y) / 2;
	int x = width - (volumePos + themeSlider.GetMarge()) - (totalTimeSize.x + themeSlider.GetMarge() / 2) - themeSlider.GetButtonSpeakerWidth() - themeSlider.GetButtonVolumeUpWidth() - themeSlider.GetButtonVolumeDownWidth();

	DrawTexte(context, libelle, x, y, themeSlider.font);

	return totalTimeSize.x;
}


void CSliderVideo::InsertPlayButton(wxDC * context)
{
	wxImage bmp;
	int yPos = 0;
	if (isPlay)
	{
		yPos = themeSlider.GetButtonPauseHeight();
        if(isVector)
        {
            if(!buttonPause.IsOk() || (buttonPause.GetWidth() != themeSlider.GetButtonPauseWidth() || buttonPause.GetHeight() != themeSlider.GetButtonPauseHeight()))
            {
                buttonPause = CreateFromSVG(themeSlider.GetButtonPauseWidth(), themeSlider.GetButtonPauseHeight(), buttonPauseVector);
            }
            bmp = buttonPause;
        }
        else
            bmp = buttonPause.ResampleBicubic(themeSlider.GetButtonPauseWidth(), themeSlider.GetButtonPauseHeight());
        
		if (!buttonPauseActif)
        {
            bmp = bmp.ConvertToGreyscale();
            bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                        colorInactifReplacement.Red(), colorInactifReplacement.Green(), colorInactifReplacement.Blue());
        }
        else
            bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                        colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());
	}
	else
	{
		yPos = themeSlider.GetButtonPlayHeight();
        if(isVector)
        {
            if(!buttonPlay.IsOk() || (buttonPlay.GetWidth() != themeSlider.GetButtonPlayWidth() || buttonPlay.GetHeight() != themeSlider.GetButtonPlayHeight()))
            {
                buttonPlay = CreateFromSVG(themeSlider.GetButtonPauseWidth(), themeSlider.GetButtonPlayHeight(), buttonPlayVector);
            }
            bmp = buttonPlay;
        }
        else
            bmp = buttonPlay.ResampleBicubic(themeSlider.GetButtonPlayWidth(), themeSlider.GetButtonPlayHeight());
        
		if (!buttonPlayActif)
        {
            bmp = bmp.ConvertToGreyscale();
            bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                        colorInactifReplacement.Red(), colorInactifReplacement.Green(), colorInactifReplacement.Blue());
        }
        else
            bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                        colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());
	}

	if (bmp.IsOk())
	{
		int yPosBitmap = (height - yPos) / 2;
		context->DrawBitmap(bmp, themeSlider.GetMarge() / 2, yPosBitmap);

		positionPlayOrPauseButton.x = themeSlider.GetMarge() / 2;
		positionPlayOrPauseButton.width = bmp.GetWidth();
		positionPlayOrPauseButton.y = yPosBitmap;
		positionPlayOrPauseButton.height = bmp.GetHeight();
	}


}

void CSliderVideo::InsertSpeakerButton(const int &xStart, wxDC * context)
{
    wxImage bmp;
	int yPosBitmap = (height - themeSlider.GetButtonSpeakerHeight()) / 2;
    
    if(isVector)
    {
        if(!buttonSpeaker.IsOk() || (buttonSpeaker.GetWidth() != themeSlider.GetButtonSpeakerWidth() || buttonSpeaker.GetHeight() != themeSlider.GetButtonSpeakerHeight()))
        {
            buttonSpeaker = CreateFromSVG(themeSlider.GetButtonSpeakerWidth(), themeSlider.GetButtonSpeakerHeight(), buttonSpeakerVector);
        }
        bmp = buttonSpeaker;
    }
    else
        bmp = buttonSpeaker.ResampleBicubic(themeSlider.GetButtonSpeakerWidth(), themeSlider.GetButtonSpeakerHeight());
    
    bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                       colorInactifReplacement.Red(), colorInactifReplacement.Green(), colorInactifReplacement.Blue());
    
	context->DrawBitmap(bmp, xStart, yPosBitmap);

	positionSpeakerButton.x = xStart;
	positionSpeakerButton.width = bmp.GetWidth();
	positionSpeakerButton.y = yPosBitmap;
	positionSpeakerButton.height = bmp.GetHeight();
}

void CSliderVideo::InsertVolumeUpButton(const int &xStart, wxDC * context)
{
    int yPos = themeSlider.GetButtonVolumeUpHeight();//buttonVolumeUp.GetHeight();
    wxImage bmp;
    
    if(isVector)
    {
        if(!buttonVolumeUp.IsOk() || (buttonVolumeUp.GetWidth() != themeSlider.GetButtonVolumeUpWidth() || buttonVolumeUp.GetHeight() != themeSlider.GetButtonVolumeUpHeight()))
        {
            buttonVolumeUp = CreateFromSVG(themeSlider.GetButtonVolumeUpWidth(), themeSlider.GetButtonVolumeUpHeight(), buttonVolumeUpVector);
        }
        bmp = buttonVolumeUp;
        
    }
    else
        bmp = buttonVolumeUp.ResampleBicubic(themeSlider.GetButtonVolumeUpWidth(), themeSlider.GetButtonVolumeUpHeight());
    
	if (!buttonVolumeUpActif)
    {
		bmp = bmp.ConvertToGreyscale();
        bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                    colorInactifReplacement.Red(), colorInactifReplacement.Green(), colorInactifReplacement.Blue());
    }
    else
        bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                    colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());
    
	if (bmp.IsOk())
	{
		int yPosBitmap = (height - yPos) / 2;
        

        
		context->DrawBitmap(bmp, xStart, yPosBitmap);

		positionVolumeUpButton.x = xStart;
		positionVolumeUpButton.width = bmp.GetWidth();
		positionVolumeUpButton.y = yPosBitmap;
		positionVolumeUpButton.height = bmp.GetHeight();
	}
}

void CSliderVideo::UpdateScreenRatio()
{
    this->Resize();
}

void CSliderVideo::InsertVolumeDownButton(const int &xStart, wxDC * context)
{
    wxImage bmp;
    
    if(isVector)
    {
        if(!buttonVolumeDown.IsOk() || (buttonVolumeDown.GetWidth() != themeSlider.GetButtonVolumeDownWidth() || buttonVolumeDown.GetHeight() != themeSlider.GetButtonVolumeDownHeight()))
        {
            buttonVolumeDown = CreateFromSVG(themeSlider.GetButtonVolumeDownWidth(), themeSlider.GetButtonVolumeDownHeight(), buttonVolumeDownVector);
        }
        bmp = buttonVolumeDown;
    }
    else
        bmp = buttonVolumeDown.ResampleBicubic(themeSlider.GetButtonVolumeDownWidth(), themeSlider.GetButtonVolumeDownHeight());
    
	int yPos = themeSlider.GetButtonVolumeDownHeight();//buttonVolumeDown.GetHeight();
	if (!buttonVolumeDownActif)
    {
        bmp = bmp.ConvertToGreyscale();
        bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                    colorInactifReplacement.Red(), colorInactifReplacement.Green(), colorInactifReplacement.Blue());
    }
    else
        bmp.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
                    colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());
    
	int yPosBitmap = (height - yPos) / 2;
	context->DrawBitmap(bmp, xStart, yPosBitmap);

	positionVolumeDownButton.x = xStart;
	positionVolumeDownButton.width = bmp.GetWidth();
	positionVolumeDownButton.y = yPosBitmap;
	positionVolumeDownButton.height = bmp.GetHeight();
}

void CSliderVideo::Draw(wxDC * context)
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
		CWindowMain::FillRect(&sourceDCContext, rc, themeSlider.colorBack);
		
		//Ecriture du temps passé
		int timePastSize = DrawTimePast(&sourceDCContext, timePast);
		int volumeSize = DrawVolumeLibelle(&sourceDCContext, libelleVolume);
		int totalTimeSize = DrawTotalTimeLibelle(&sourceDCContext, totalTime, volumeSize);

		//Ecriture du slider
		positionSlider.x = themeSlider.GetMarge() + timePastSize + themeSlider.GetMarge() + themeSlider.GetButtonPlayWidth();
		positionSlider.width = (width - (volumeSize + themeSlider.GetMarge()) - (totalTimeSize + themeSlider.GetMarge()) - themeSlider.GetButtonSpeakerWidth() - themeSlider.GetButtonVolumeUpWidth() - themeSlider.GetButtonVolumeDownWidth()) - positionSlider.x;
		positionSlider.y = (height - themeSlider.GetRectangleHeight()) / 2;
		positionSlider.height = themeSlider.GetRectangleHeight();
		DrawShapeElement(&sourceDCContext, positionSlider);

		
		CalculPositionButton();

		//Ecriture du bouton de lecture
		InsertPlayButton(&sourceDCContext);

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
		int xButtonPos = positionSlider.x + positionSlider.width + (totalTimeSize + themeSlider.GetMarge());
		InsertSpeakerButton(xButtonPos, &sourceDCContext);

		xButtonPos += themeSlider.GetButtonSpeakerWidth();
		InsertVolumeDownButton(xButtonPos, &sourceDCContext);

		xButtonPos = width - themeSlider.GetButtonVolumeUpWidth();
		InsertVolumeUpButton(xButtonPos, &sourceDCContext);

		context->Blit(0, 0, width, height, &sourceDCContext, 0, 0);
		sourceDCContext.SelectObject(wxNullBitmap);
	}

}

void CSliderVideo::CalculPositionButton()
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

void CSliderVideo::CalculPositionButton(const int &x)
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

void CSliderVideo::CalculTimePosition(const int &x)
{
	float posX = x - positionSlider.x;
	float total = positionSlider.width;
	totalPastTimeInMilliseconds = (double)(posX / total)* totalTimeInMilliseconds;
	secondTimePast = (float)totalPastTimeInMilliseconds / (float)1000000;
	timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
	//DrawTimePast(&winDC, timePast);
	this->Refresh();
}

void CSliderVideo::ClickLeftPage(const int &x)
{
	//Click Top Triangle
	CalculTimePosition(x);
	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(totalPastTimeInMilliseconds);

}

void CSliderVideo::ClickRightPage(const int &x)
{
	//Click Top Triangle
	CalculTimePosition(x);
	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(totalPastTimeInMilliseconds);

}

void CSliderVideo::Resize()
{
	this->Refresh();
}

void CSliderVideo::UpdatePositionEvent()
{
	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(totalPastTimeInMilliseconds);
}

void CSliderVideo::OnMouseMove(wxMouseEvent& event)
{
	int xPos = event.GetX();
	//int yPos = event.GetY();

	if (mouseBlock)
	{
		if ((xPos >= positionSlider.x && xPos <= (positionSlider.x + positionSlider.width)))
		{
			CalculTimePosition(xPos);
			
		}
	}
	else
	{
		if (xPos >= positionPlayOrPauseButton.x && xPos <= (positionPlayOrPauseButton.x + positionPlayOrPauseButton.width))
		{
			if (isPlay)
			{
				buttonPauseActif = true;
			}
			else
			{
				buttonPlayActif = true;
			}
		}
		else if (xPos >= positionVolumeUpButton.x && xPos <= (positionVolumeUpButton.x + positionVolumeUpButton.width))
		{
			buttonVolumeUpActif = true;
		}
		else if (xPos >= positionVolumeDownButton.x && xPos <= (positionVolumeDownButton.x + positionVolumeDownButton.width))
		{
			buttonVolumeDownActif = true;
		}
		else
		{
			buttonPlayActif = false;
			buttonPauseActif = false;
			buttonVolumeUpActif = false;
			buttonVolumeDownActif = false;
		}
	}
	this->Refresh();
}

void CSliderVideo::OnMouseLeave(wxMouseEvent& event)
{
	m_bTracking = FALSE;
	m_bMouseOver = false;
}

void CSliderVideo::OnMouseHover(wxMouseEvent& event)
{
	m_bMouseOver = true;
}

void CSliderVideo::OnLButtonDown(wxMouseEvent& event)
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
			this->Refresh();
		}
		else if (xPos < positionButton.x)
		{
			ClickLeftPage(xPos);
			this->Refresh();
		}
	}
	else
	{
		if (xPos >= positionPlayOrPauseButton.x && xPos <= (positionPlayOrPauseButton.width + positionPlayOrPauseButton.x))
		{
            wxSetCursor(hCursorHand);
			if (isPlay)
			{
				sliderEvent->ClickButton(PAUSEBUTTONID);
				this->Refresh();
			}
			else
			{
				sliderEvent->ClickButton(PLAYBUTTONID);
				this->Refresh();
			}
		}
		else if (xPos >= positionVolumeUpButton.x && xPos <= (positionVolumeUpButton.x + positionVolumeUpButton.width))
		{
            wxSetCursor(hCursorHand);
			sliderEvent->ClickButton(VOLUMEUPBUTTONID);
			this->Refresh();
		}
		else if (xPos >= positionVolumeDownButton.x && xPos <= (positionVolumeDownButton.x + positionVolumeDownButton.width))
		{
            wxSetCursor(hCursorHand);
			sliderEvent->ClickButton(VOLUMEDOWNBUTTONID);
			this->Refresh();
		}
        else
        {
             wxSetCursor(wxCURSOR_ARROW);
        }
	}
    
}

void CSliderVideo::OnLButtonUp(wxMouseEvent& event)
{
	if (mouseBlock)
	{
		mouseBlock = false;
		ReleaseMouse();

		if (sliderEvent != nullptr)
			sliderEvent->MoveSlider(totalPastTimeInMilliseconds);
	}
}

void CSliderVideo::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	Draw(&dc);
}