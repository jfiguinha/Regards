#include "header.h"
#include "SliderVideo.h"
#include <LibResource.h>
#include <ConvertUtility.h>
#include <ClosedHandCursor.h>
#include "WindowMain.h"
#include <wx/sstream.h>
using namespace Regards::Video;

CSliderVideo::CSliderVideo(wxWindow* parent, wxWindowID id, CSliderInterface* sliderEvent,
                           const CThemeSlider& themeSlider)
	: CWindowMain("CSliderVideo", parent, id)
{
	buttonPlayActif = false;
	buttonPauseActif = false;
	buttonScreenActif = false;
	buttonZoomActif = false;
	buttonSpeakerActif = false;
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

	colorToReplace = wxColor(0, 0, 0);
	colorInactifReplacement = themeSlider.colorInactifReplacement; //wxColor(255,255,255);
	colorActifReplacement = themeSlider.colorActifReplacement; //wxColor(48, 128, 254);

	Connect(wxEVT_PAINT, wxPaintEventHandler(CSliderVideo::on_paint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CSliderVideo::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CSliderVideo::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CSliderVideo::OnLButtonUp));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CSliderVideo::OnSize));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CSliderVideo::OnMouseCaptureLost));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CSliderVideo::OnMouseHover));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CSliderVideo::OnMouseLeave));

	button.Create(0, 0);
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
	needToRefresh = true;
}

void CSliderVideo::SetPause()
{
	isPlay = false;
	needToRefresh = true;
}


int CSliderVideo::GetWidth()
{
	return themeSlider.GetWidth();
}

int CSliderVideo::GetHeight()
{
	return themeSlider.GetHeight();
}

void CSliderVideo::DrawShapeElement(wxDC* dc, const wxRect& rc)
{
	wxRect rcPast;
	rcPast.x = rc.x;

	if (secondTotalTime > 0)
	{
		float pourcentage = secondTimePast / secondTotalTime;
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

void CSliderVideo::SetTotalSecondTime(const int64_t& secondTime)
{
	secondTotalTime = secondTime / 1000;
	totalTime = CConvertUtility::GetTimeLibelle(secondTotalTime);
}

void CSliderVideo::SetPastSecondTime(const int64_t& secondTime)
{
	if (!mouseBlock)
	{
		int timeToSecond = secondTime / 1000;
		if (timeToSecond != secondTimePast)
		{
			secondTimePast = timeToSecond;
			timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
		}
	}
}


int CSliderVideo::DrawTimePast(wxDC* context, const wxString& libelle)
{
	wxSize filenameSize = GetSizeTexte(context, libelle, themeSlider.font);
	int x = themeSlider.GetMarge() + themeSlider.GetButtonPlayWidth();
	int y = (GetWindowHeight() - filenameSize.y) / 2;
	DrawTexte(context, libelle, x, y, themeSlider.font);
	return filenameSize.x;
}

int CSliderVideo::DrawVolumeLibelle(wxDC* context, const wxString& libelle)
{
	wxSize volumeSize = GetSizeTexte(context, libelle, themeSlider.font);

	int y = (GetWindowHeight() - volumeSize.y) / 2;
	int x = GetWindowWidth() - (volumeSize.x + themeSlider.GetMarge() / 2) - themeSlider.GetButtonVolumeUpWidth();

	DrawTexte(context, libelle, x, y, themeSlider.font);

	return volumeSize.x;
}


int CSliderVideo::DrawTotalTimeLibelle(wxDC* context, const wxString& libelle, const int& volumePos)
{
	wxSize totalTimeSize = GetSizeTexte(context, libelle, themeSlider.font);

	int y = (GetWindowHeight() - totalTimeSize.y) / 2;
	int x = GetWindowWidth() - (volumePos + themeSlider.GetMarge()) - (totalTimeSize.x + themeSlider.GetMarge() / 2) -
		((themeSlider.GetButtonSpeakerWidth() + themeSlider.GetMarge()) * 4);

	DrawTexte(context, libelle, x, y, themeSlider.font);

	return totalTimeSize.x;
}


void CSliderVideo::InsertPlayButton(wxDC* context)
{
	wxImage bmp;
	int y_pos;
	if (isPlay)
	{
		y_pos = themeSlider.GetButtonPauseHeight();

		if (!buttonPause.IsOk() || (buttonPause.GetWidth() != themeSlider.GetButtonPauseWidth() || buttonPause.
			GetHeight() != themeSlider.GetButtonPauseHeight()))
		{
			buttonPause = CLibResource::CreatePictureFromSVG("IDB_PAUSE_VIDEO", themeSlider.GetButtonPauseWidth(),
			                                                 themeSlider.GetButtonPauseHeight());
		}
		bmp = buttonPause;


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
		y_pos = themeSlider.GetButtonPlayHeight();

		if (!buttonPlay.IsOk() || (buttonPlay.GetWidth() != themeSlider.GetButtonPlayWidth() || buttonPlay.GetHeight()
			!= themeSlider.GetButtonPlayHeight()))
		{
			buttonPlay = CLibResource::CreatePictureFromSVG("IDB_PLAY_VIDEO", themeSlider.GetButtonPauseWidth(),
			                                                themeSlider.GetButtonPlayHeight());
		}
		bmp = buttonPlay;


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
		int yPosBitmap = (GetWindowHeight() - y_pos) / 2;
		context->DrawBitmap(bmp, themeSlider.GetMarge() / 2, yPosBitmap);

		positionPlayOrPauseButton.x = themeSlider.GetMarge() / 2;
		positionPlayOrPauseButton.width = bmp.GetWidth();
		positionPlayOrPauseButton.y = yPosBitmap;
		positionPlayOrPauseButton.height = bmp.GetHeight();
	}
}


void CSliderVideo::InsertButton(const int& xStart, wxDC* context, wxRect& position, wxImage& button,
                                const bool& isActif, const wxString& pictureName)
{
	int yPos = themeSlider.GetButtonVolumeUpHeight(); //buttonVolumeUp.GetHeight();
	wxImage bmp;

	if (!button.IsOk() || (button.GetWidth() != themeSlider.GetButtonVolumeUpWidth() || button.
		GetHeight() != themeSlider.GetButtonVolumeUpHeight()))
	{
		button = CLibResource::CreatePictureFromSVG(pictureName, themeSlider.GetButtonVolumeUpWidth(),
		                                            themeSlider.GetButtonVolumeUpHeight());
	}
	bmp = button;


	if (!isActif)
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
		int yPosBitmap = (GetWindowHeight() - yPos) / 2;


		context->DrawBitmap(bmp, xStart, yPosBitmap);

		position.x = xStart;
		position.width = bmp.GetWidth();
		position.y = yPosBitmap;
		position.height = bmp.GetHeight();
	}
}

void CSliderVideo::InsertRepeatButton(const int& xStart, wxDC* context)
{
	InsertButton(xStart, context, positionRepeatButton, buttonRepeat, buttonRepeatActif, "IDB_REFRESH");
}

void CSliderVideo::InsertSpeakerButton(const int& xStart, wxDC* context)
{
	InsertButton(xStart, context, positionSpeakerButton, buttonSpeaker, buttonSpeakerActif, "IDB_VOLUME_UP_VIDEO");
}

void CSliderVideo::InsertZoomButton(const int& xStart, wxDC* context)
{
	InsertButton(xStart, context, positionZoomButton, buttonVolumeDown, buttonZoomActif, "IDB_ZOOMPLUS");
}


void CSliderVideo::InsertScreenFormatButton(const int& xStart, wxDC* context)
{
	InsertButton(xStart, context, positionScreenFormatButton, buttonVolumeUp, buttonScreenActif, "IDB_SCREENPNG");
}

void CSliderVideo::UpdateScreenRatio()
{
	this->Resize();
}


void CSliderVideo::Draw(wxDC* context)
{
	if (GetWindowWidth() > 0 && GetWindowHeight() > 0)
	{
		wxRect rc = GetWindowRect();


		auto memBitmap = wxBitmap(GetWindowWidth(), GetWindowHeight());
		wxMemoryDC sourceDCContext(memBitmap);
		FillRect(&sourceDCContext, rc, themeSlider.colorBack);

		//Ecriture du temps pass�
		int timePastSize = DrawTimePast(&sourceDCContext, timePast);
		int volumeSize = 0; // DrawVolumeLibelle(&sourceDCContext, libelleVolume);
		int totalTimeSize = DrawTotalTimeLibelle(&sourceDCContext, totalTime, volumeSize);

		//Ecriture du slider
		positionSlider.x = themeSlider.GetMarge() + timePastSize + themeSlider.GetMarge() + themeSlider.
			GetButtonPlayWidth();
		positionSlider.width = (GetWindowWidth() - (volumeSize + themeSlider.GetMarge()) - (totalTimeSize + themeSlider.
			GetMarge())) - ((themeSlider.GetButtonSpeakerWidth() + themeSlider.GetMarge()) * 4) - positionSlider.x;
		positionSlider.y = (GetWindowHeight() - themeSlider.GetRectangleHeight()) / 2;
		positionSlider.height = themeSlider.GetRectangleHeight();
		DrawShapeElement(&sourceDCContext, positionSlider);


		CalculPositionButton();

		//Ecriture du bouton de lecture
		InsertPlayButton(&sourceDCContext);

		if (!button.IsOk() || (button.GetWidth() != themeSlider.GetButtonWidth() || button.GetHeight() != themeSlider.
			GetButtonHeight()))
			button = CLibResource::CreatePictureFromSVG("IDB_BOULESLIDER", themeSlider.GetButtonWidth(),
			                                            themeSlider.GetButtonHeight());
		sourceDCContext.DrawBitmap(button, positionButton.x, positionButton.y);

		int xButtonPos = positionSlider.x + positionSlider.width + (totalTimeSize + themeSlider.GetMarge());
		InsertRepeatButton(xButtonPos, &sourceDCContext);

		xButtonPos += themeSlider.GetButtonSpeakerWidth() + themeSlider.GetMarge();
		InsertSpeakerButton(xButtonPos, &sourceDCContext);

		xButtonPos += themeSlider.GetButtonSpeakerWidth() + themeSlider.GetMarge();
		InsertZoomButton(xButtonPos, &sourceDCContext);

		xButtonPos += themeSlider.GetButtonSpeakerWidth() + themeSlider.GetMarge();
		InsertScreenFormatButton(xButtonPos, &sourceDCContext);

		context->Blit(0, 0, GetWindowWidth(), GetWindowHeight(), &sourceDCContext, 0, 0);
		sourceDCContext.SelectObject(wxNullBitmap);
	}
}

void CSliderVideo::CalculPositionButton()
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

void CSliderVideo::CalculPositionButton(const int& x)
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

void CSliderVideo::CalculTimePosition(const int& x)
{
	float posX = x - positionSlider.x;
	float total = positionSlider.width;
	secondTimePast = static_cast<double>(posX / total) * secondTotalTime;
	timePast = CConvertUtility::GetTimeLibelle(secondTimePast);
}

void CSliderVideo::ClickLeftPage(const int& x)
{
	//Click Top Triangle
	CalculTimePosition(x);
	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(secondTimePast);
}

void CSliderVideo::ClickRightPage(const int& x)
{
	//Click Top Triangle
	CalculTimePosition(x);
	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(secondTimePast);
}

void CSliderVideo::UpdatePositionEvent()
{
	if (sliderEvent != nullptr)
		sliderEvent->MoveSlider(secondTimePast);
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
			if (sliderEvent != nullptr)
				sliderEvent->MoveSlider(secondTimePast);
		}
	}
	else
	{
		if (xPos >= positionPlayOrPauseButton.x && xPos <= (positionPlayOrPauseButton.x + positionPlayOrPauseButton.
			width))
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
		else if (xPos >= positionSpeakerButton.x && xPos <= (positionSpeakerButton.x + positionSpeakerButton.width))
		{
			buttonSpeakerActif = true;
		}
		else if (xPos >= positionZoomButton.x && xPos <= (positionZoomButton.x + positionZoomButton.width))
		{
			buttonZoomActif = true;
		}
		else if (xPos >= positionScreenFormatButton.x && xPos <= (positionScreenFormatButton.x +
			positionScreenFormatButton.width))
		{
			buttonScreenActif = true;
		}
		else
		{
			buttonPlayActif = false;
			buttonPauseActif = false;
			buttonSpeakerActif = false;
			buttonZoomActif = false;
			buttonScreenActif = false;
		}
	}
	needToRefresh = true;
}

void CSliderVideo::OnMouseLeave(wxMouseEvent& event)
{
	m_bTracking = FALSE;
	m_bMouseOver = false;

	if (HasCapture())
		ReleaseMouse();
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
		if ((xPos >= positionButton.x && xPos <= (positionButton.x + positionButton.width)) && (yPos >= positionButton.y
			&& yPos <= (positionButton.y + positionButton.height)))
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
		if (xPos >= positionPlayOrPauseButton.x && xPos <= (positionPlayOrPauseButton.width + positionPlayOrPauseButton.
			x))
		{
			wxSetCursor(hCursorHand);
			if (isPlay)
			{
				sliderEvent->ClickButton(PAUSEBUTTONID);
			}
			else
			{
				sliderEvent->ClickButton(PLAYBUTTONID);
			}
		}
		else if (xPos >= positionSpeakerButton.x && xPos <= (positionSpeakerButton.x + positionSpeakerButton.width))
		{
			wxSetCursor(hCursorHand);
			sliderEvent->ClickButton(SPEAKERBUTTONID);
		}
		else if (xPos >= positionRepeatButton.x && xPos <= (positionRepeatButton.x + positionRepeatButton.width))
		{
			wxSetCursor(hCursorHand);
			sliderEvent->ClickButton(REPEATID);
			if (buttonRepeatActif)
				buttonRepeatActif = false;
			else
				buttonRepeatActif = true;
		}
		else if (xPos >= positionZoomButton.x && xPos <= (positionZoomButton.x + positionZoomButton.width))
		{
			wxSetCursor(hCursorHand);
			sliderEvent->ClickButton(ZOOMBUTTONID);
		}
		else if (xPos >= positionScreenFormatButton.x && xPos <= (positionScreenFormatButton.x +
			positionScreenFormatButton.width))
		{
			wxSetCursor(hCursorHand);
			sliderEvent->ClickButton(SCREENFORMATID);
		}
		else
		{
			wxSetCursor(wxCURSOR_ARROW);
		}
	}
	needToRefresh = true;
}

void CSliderVideo::OnLButtonUp(wxMouseEvent& event)
{
	if (mouseBlock)
	{
		mouseBlock = false;
		ReleaseMouse();

		if (sliderEvent != nullptr)
			sliderEvent->MoveSlider(secondTimePast);
	}
}

void CSliderVideo::on_paint(wxPaintEvent& event)
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
