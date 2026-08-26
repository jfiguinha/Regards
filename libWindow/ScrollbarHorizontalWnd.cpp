#include "header.h"
#include "ScrollbarHorizontalWnd.h"
#include <window_id.h>
#include <wx/dcbuffer.h>
using namespace Regards::Window;

#define BARSIZEMIN 20
#define LINESIZE 5
#define PAGESIZE 50

enum
{
	TIMER_TRIANGLELEFT = 10,
	TIMER_TRIANGLERIGHT = 11,
	TIMER_PAGELEFT = 12,
	TIMER_PAGERIGHT = 13,
	TIMER_STOPMOVING = 14
};


CScrollbarHorizontalWnd::CScrollbarHorizontalWnd(const wxString& windowName, wxWindow* parent, wxWindowID id,
                                                 const CThemeScrollBar& theme)
	: CWindowMain(windowName, parent, id)
{
	showTriangle = true;
	scrollMoving = false;
	m_bTracking = false;
	captureBar = false;
	pictureWidth = 0;
	screenWidth = 0;
	pageSize = 50;
	lineSize = 5;
	xPositionStart = 0;
	barStartX = 0;
	barEndX = 0;
	barSize = 0;
	barPosX = 0;
	pageSizeDefault = 50;
	lineSizeDefault = 5;
	currentXPos = 0;

	triangleLeft = std::make_unique<wxTimer>(this, TIMER_TRIANGLELEFT);
	triangleRight = std::make_unique<wxTimer>(this, TIMER_TRIANGLERIGHT);
	pageLeft = std::make_unique<wxTimer>(this, TIMER_PAGELEFT);
	pageRight = std::make_unique<wxTimer>(this, TIMER_PAGERIGHT);
	stopMoving = std::make_unique<wxTimer>(this, TIMER_STOPMOVING);

	themeScroll = theme;

	Connect(wxEVT_PAINT, wxPaintEventHandler(CScrollbarHorizontalWnd::on_paint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CScrollbarHorizontalWnd::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CScrollbarHorizontalWnd::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CScrollbarHorizontalWnd::OnLButtonUp));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CScrollbarHorizontalWnd::OnMouseHover));
	Connect(TIMER_TRIANGLELEFT, wxEVT_TIMER, wxTimerEventHandler(CScrollbarHorizontalWnd::OnTimerTriangleLeft), nullptr,
	        this);
	Connect(TIMER_TRIANGLERIGHT, wxEVT_TIMER, wxTimerEventHandler(CScrollbarHorizontalWnd::OnTimerTriangleRight),
	        nullptr, this);
	Connect(TIMER_PAGELEFT, wxEVT_TIMER, wxTimerEventHandler(CScrollbarHorizontalWnd::OnTimerPageLeft), nullptr, this);
	Connect(TIMER_PAGERIGHT, wxEVT_TIMER, wxTimerEventHandler(CScrollbarHorizontalWnd::OnTimerPageRight), nullptr,
	        this);
	Connect(TIMER_STOPMOVING, wxEVT_TIMER, wxTimerEventHandler(CScrollbarHorizontalWnd::OnTimerStopMoving), nullptr,
	        this);
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CScrollbarHorizontalWnd::OnMouseCaptureLost));
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(CScrollbarHorizontalWnd::OnEraseBackground));
	//	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CScrollbarHorizontalWnd::OnMouseLeave));
}

void CScrollbarHorizontalWnd::OnMouseCaptureLost(wxMouseEvent& event)
{
}

void CScrollbarHorizontalWnd::SetIsMoving()
{
	if (stopMoving->IsRunning())
		stopMoving->Stop();

	if (!scrollMoving)
	{
		scrollMoving = true;

		wxCommandEvent evt(wxEVENT_SCROLLMOVE);
		evt.SetInt(1);

		if (wxWindow* parent = GetParent())
		{
			parent->GetEventHandler()->AddPendingEvent(evt);
		}
	}

	stopMoving->Start(1000);
}
bool CScrollbarHorizontalWnd::IsMoving()
{
	return scrollMoving;
}

int CScrollbarHorizontalWnd::GetHeightSize()
{
	return themeScroll.GetRectangleSize() + (themeScroll.GetMarge() * 2);
}

int CScrollbarHorizontalWnd::GetPosition()
{
	return currentXPos;
}

int CScrollbarHorizontalWnd::GetScreenWidth()
{
	return screenWidth;
}

int CScrollbarHorizontalWnd::GetPictureWidth()
{
	return pictureWidth;
}

void CScrollbarHorizontalWnd::UpdateScreenRatio()
{
	Resize();
}

bool CScrollbarHorizontalWnd::SetPosition(const int& left)
{
	bool value = true;
	if (left != currentXPos)
	{
		currentXPos = left;
		MoveBar(currentXPos, themeScroll.colorBarActif);
	}
	else
		value = false;
	return value;
}

bool CScrollbarHorizontalWnd::UpdateScrollBar(const int& posLargeur, const int& screenWidth, const int& pictureWidth)
{
	bool needToShow = false;

	if (pictureWidth > screenWidth && !this->IsShown())
	{
		Show(true);
		needToShow = true;
	}
	else if (pictureWidth <= screenWidth && this->IsShown())
	{
		Show(false);
		needToShow = true;
	}

	if (this->IsShown())
	{
		bool needToRedraw = false;
		bool return_value = DefineSize(screenWidth, pictureWidth);
		if (return_value)
			needToRedraw = true;
		return_value = SetPosition(posLargeur);
		if (return_value)
			needToRedraw = true;
		if (needToRedraw)
		{
			PaintNow();
		}
	}

	return needToShow;
}

void CScrollbarHorizontalWnd::CalculBarSize()
{
	if (showTriangle)
		barStartX = themeScroll.GetMarge() + themeScroll.GetMarge() + themeScroll.GetRectangleSize();
	else
		barStartX = 0;
	barEndX = GetWindowWidth() - barStartX;

	int diff = pictureWidth - screenWidth;
	float nbPos = static_cast<float>(diff) / static_cast<float>(lineSize);

	barPosX = barStartX + currentXPos / lineSize;
	barSize = (barEndX - barStartX) - nbPos;

	if (barSize < BARSIZEMIN)
	{
		barSize = BARSIZEMIN;
		nbPos = (barEndX - barStartX) - barSize;
		lineSize = static_cast<float>(diff) / nbPos;
		pageSize = lineSize * 10;
	}
}

bool CScrollbarHorizontalWnd::DefineSize(const int& screenWidth, const int& pictureWidth)
{
	if (this->pictureWidth == pictureWidth && this->screenWidth == screenWidth)
	{
		return false;
	}

	this->pictureWidth = pictureWidth;
	this->screenWidth = screenWidth;

	if (barEndX > 0)
	{
		pageSize = pageSizeDefault;
		lineSize = lineSizeDefault;
		CalculBarSize();
		rcPosBar.x = barPosX;
		rcPosBar.width = barPosX + barSize;
		rcPosBar.y = themeScroll.GetMarge();
		rcPosBar.height = themeScroll.GetRectangleSize() + themeScroll.GetMarge();
	}
	return true;
}


void CScrollbarHorizontalWnd::SetPageSize(const int& pageSize)
{
	pageSizeDefault = pageSize;
	this->pageSize = pageSize;
}

int CScrollbarHorizontalWnd::GetPageSize()
{
	return pageSize;
}

void CScrollbarHorizontalWnd::SetLineSize(const int& lineSize)
{
	lineSizeDefault = lineSize;
	this->lineSize = lineSize;
}

int CScrollbarHorizontalWnd::GetLineSize()
{
	return lineSize;
}

CScrollbarHorizontalWnd::~CScrollbarHorizontalWnd()
{
	if (triangleLeft->IsRunning())
		triangleLeft->Stop();

	if (triangleRight->IsRunning())
		triangleRight->Stop();

	if (pageLeft->IsRunning())
		pageLeft->Stop();

	if (pageRight->IsRunning())
		pageRight->Stop();

	if (stopMoving->IsRunning())
		stopMoving->Stop();
}

void CScrollbarHorizontalWnd::DrawLeftTriangleElement(wxDC* dc, const wxRect& rc, const wxColour& colorTriangle)
{
	wxBrush brushHatch(colorTriangle);
	dc->SetBrush(brushHatch);
	wxPoint star[3];
	star[0] = wxPoint(rc.x, (rc.height - rc.y) / 2 + themeScroll.GetMarge());
	star[1] = wxPoint(rc.width, rc.y);
	star[2] = wxPoint(rc.width, rc.height);
	dc->DrawPolygon(WXSIZEOF(star), star, 0, 0);
	dc->SetBrush(wxNullBrush);
}

void CScrollbarHorizontalWnd::DrawRightTriangleElement(wxDC* dc, const wxRect& rc, const wxColour& colorTriangle)
{
	wxBrush brushHatch(colorTriangle);
	dc->SetBrush(brushHatch);
	wxPoint star[3];
	star[0] = wxPoint(rc.x, rc.y);
	star[1] = wxPoint(rc.x, rc.height);
	star[2] = wxPoint(rc.width, (rc.height - rc.y) / 2 + themeScroll.GetMarge());
	dc->DrawPolygon(WXSIZEOF(star), star, 0, 0);
	dc->SetBrush(wxNullBrush);
}

void CScrollbarHorizontalWnd::DrawRectangleElement(wxDC* dc, const wxColour& colorBar)
{
	auto brush = wxBrush(colorBar);
	dc->SetBrush(brush);
	wxRect rc = rcPosBar;


	if (rcPosBar.width > barEndX)
	{
		rcPosBar.x = barEndX - barSize;
		rcPosBar.width = barEndX;
	}

	if (rcPosBar.x < barStartX)
	{
		rcPosBar.x = barStartX;
		rcPosBar.width = barStartX + barSize;
	}

	rc.width = rcPosBar.width - rcPosBar.x;
	rc.height = GetWindowHeight() - (themeScroll.GetMarge() * 2);
	dc->DrawRoundedRectangle(rc, (GetWindowHeight() / 2) - themeScroll.GetMarge());
	dc->SetBrush(wxNullBrush);
	//FillRect(dc, rc, colorBar);    
}

void CScrollbarHorizontalWnd::OnMouseLeave(wxMouseEvent& event)
{
	m_bTracking = false;
	if (triangleLeft->IsRunning())
		triangleLeft->Stop();
	if (triangleRight->IsRunning())
		triangleRight->Stop();
	if (pageLeft->IsRunning())
		pageLeft->Stop();
	if (pageRight->IsRunning())
		pageRight->Stop();

	wxCommandEvent evt(wxEVENT_SCROLLMOVE);
	evt.SetInt(0);
	GetParent()->GetEventHandler()->AddPendingEvent(evt);

	if (HasCapture())
		ReleaseMouse();
	captureBar = false;
}

void CScrollbarHorizontalWnd::OnMouseHover(wxMouseEvent& event)
{
	wxSetCursor(wxCursor(wxCURSOR_ARROW));
}

void CScrollbarHorizontalWnd::Resize()
{
	if (showTriangle)
		barStartX = themeScroll.GetMarge() + themeScroll.GetMarge() + themeScroll.GetRectangleSize();
	else
		barStartX = 0;

	barEndX = GetWindowWidth() - barStartX;
	//int tailleY = height;

	if (barPosX == 0)
		barPosX = barStartX;

	rcPosTriangleLeft.x = themeScroll.GetMarge();
	rcPosTriangleLeft.width = themeScroll.GetMarge() + themeScroll.GetRectangleSize();
	rcPosTriangleLeft.y = themeScroll.GetMarge();
	rcPosTriangleLeft.height = themeScroll.GetMarge() + themeScroll.GetRectangleSize();

	rcPosTriangleRight.x = (GetWindowWidth() - barStartX) + themeScroll.GetMarge();
	rcPosTriangleRight.width = GetWindowWidth() - themeScroll.GetMarge();
	rcPosTriangleRight.y = themeScroll.GetMarge();
	rcPosTriangleRight.height = themeScroll.GetMarge() + themeScroll.GetRectangleSize();


	if (barEndX > 0)
	{
		pageSize = pageSizeDefault;
		lineSize = lineSizeDefault;
		CalculBarSize();
		rcPosBar.x = barPosX;
		rcPosBar.width = barPosX + barSize;
		rcPosBar.y = themeScroll.GetMarge();
		rcPosBar.height = themeScroll.GetRectangleSize() + themeScroll.GetMarge();
	}

	needToRefresh = true;
}

void CScrollbarHorizontalWnd::OnMouseMove(wxMouseEvent& event)
{
	if (!captureBar)
		return;

	const int mouseX = event.GetX();

	const int scrollRange = pictureWidth - screenWidth;
	const int barRange = (barEndX - barStartX) - barSize;

	if (scrollRange <= 0 || barRange <= 0)
		return;

	const double mouseDelta =
		static_cast<double>(mouseX) - dragStartMouseX;

	const double scrollDelta =
		mouseDelta *
		static_cast<double>(scrollRange) /
		static_cast<double>(barRange);

	const int newPosition =
		dragStartScrollX +
		static_cast<int>(std::lround(scrollDelta));

	if (newPosition == currentXPos)
		return;

	currentXPos = newPosition;

	TestMinX();
	TestMaxX();

	MoveBar(currentXPos, themeScroll.colorBarActif);

	if (currentXPos != lastSentScrollX)
	{
		lastSentScrollX = currentXPos;
		SendLeftPosition(currentXPos);
	}

	SetIsMoving();

	event.Skip(false);
}

bool CScrollbarHorizontalWnd::TestMaxX()
{
	int diff = pictureWidth - screenWidth;

	if (currentXPos > diff)
	{
		currentXPos = diff;
		return true;
	}
	return false;
}

bool CScrollbarHorizontalWnd::TestMinX()
{
	if (currentXPos < 0)
	{
		currentXPos = 0;
		return true;
	}
	return false;
}


bool CScrollbarHorizontalWnd::FindLeftTriangle(const int& yPosition, const int& xPosition)
{
	if (yPosition > rcPosTriangleLeft.y && yPosition < rcPosTriangleLeft.height && xPosition > rcPosTriangleLeft.x &&
		xPosition < rcPosTriangleLeft.width)
	{
		return true;
	}
	return false;
}

bool CScrollbarHorizontalWnd::FindRightTriangle(const int& yPosition, const int& xPosition)
{
	if (yPosition > rcPosTriangleRight.y && yPosition < rcPosTriangleRight.height && xPosition > rcPosTriangleRight.x &&
		xPosition < rcPosTriangleRight.width)
	{
		return true;
	}
	return false;
}

bool CScrollbarHorizontalWnd::FindRectangleBar(const int& yPosition, const int& xPosition)
{
	if (yPosition > rcPosBar.y && yPosition < rcPosBar.height && xPosition > rcPosBar.x && xPosition < rcPosBar.width)
	{
		return true;
	}
	return false;
}


void CScrollbarHorizontalWnd::MoveBar(const int& currentPos, wxColour color)
{
	const int diff = pictureWidth - screenWidth;

	if (diff <= 0)
		return;

	const int freeSpace = (barEndX - barStartX) - barSize;

	if (freeSpace <= 0)
		return;

	const double percentage =
		static_cast<double>(currentPos) /
		static_cast<double>(diff);

	const int posX =
		static_cast<int>(
			std::lround(
				static_cast<double>(freeSpace) * percentage));

	rcPosBar.x = barStartX + posX;
	rcPosBar.width = rcPosBar.x + barSize;

	if (rcPosBar.width > barEndX)
	{
		rcPosBar.width = barEndX;
		rcPosBar.x = barEndX - barSize;
	}

	if (rcPosBar.x < barStartX)
	{
		rcPosBar.x = barStartX;
		rcPosBar.width = barStartX + barSize;
	}

	Refresh(false);
}

void CScrollbarHorizontalWnd::OnLButtonDown(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();

	if (showTriangle && FindLeftTriangle(yPos, xPos))
	{
		ClickLeftTriangle();
		triangleLeft->Start(100);
	}
	else if (showTriangle && FindRightTriangle(yPos, xPos))
	{
		ClickRightTriangle();
		triangleRight->Start(100);
	}
	else if (FindRectangleBar(yPos, xPos))
	{
		captureBar = true;

		dragStartMouseX = static_cast<double>(xPos);
		dragStartScrollX = currentXPos;

		CaptureMouse();

		SetIsMoving();
	}
	else if (xPos > rcPosBar.width)
	{
		ClickRightPage();
		pageRight->Start(100);
	}
	else if (xPos < rcPosBar.x)
	{
		ClickLeftPage();
		pageLeft->Start(100);
	}
}
void CScrollbarHorizontalWnd::OnLButtonUp(wxMouseEvent& event)
{
	if (captureBar)
	{
		if (HasCapture())
			ReleaseMouse();

		captureBar = false;

		if (currentXPos != lastSentScrollX)
		{
			lastSentScrollX = currentXPos;
			SendLeftPosition(currentXPos);
		}
	}

	if (triangleLeft->IsRunning())
		triangleLeft->Stop();

	if (triangleRight->IsRunning())
		triangleRight->Stop();

	if (pageLeft->IsRunning())
		pageLeft->Stop();

	if (pageRight->IsRunning())
		pageRight->Stop();
}

void CScrollbarHorizontalWnd::OnTimerTriangleLeft(wxTimerEvent& event)
{
	ClickLeftTriangle();
}

void CScrollbarHorizontalWnd::OnTimerTriangleRight(wxTimerEvent& event)
{
	ClickRightTriangle();
}

void CScrollbarHorizontalWnd::OnTimerPageLeft(wxTimerEvent& event)
{
	ClickLeftPage();
}

void CScrollbarHorizontalWnd::OnTimerPageRight(wxTimerEvent& event)
{
	ClickRightPage();
}

void CScrollbarHorizontalWnd::OnTimerStopMoving(wxTimerEvent& event)
{
	if (stopMoving->IsRunning())
		stopMoving->Stop();

	if (!scrollMoving)
		return;

	scrollMoving = false;

	wxCommandEvent evt(wxEVENT_SCROLLMOVE);
	evt.SetInt(0);

	if (wxWindow* parent = GetParent())
	{
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CScrollbarHorizontalWnd::SetShowWindow(const bool& showValue)
{
	this->showWindow = showValue;
}

void CScrollbarHorizontalWnd::SendLeftPosition(const int& value)
{
	if (showWindow)
	{
		wxWindow* window = this->GetParent();
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_LEFTPOSITION);
			evt.SetInt(value);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
	}
}

void CScrollbarHorizontalWnd::ClickLeftTriangle()
{
	currentXPos -= lineSize;
	TestMinX();
	MoveBar(currentXPos, themeScroll.colorBar);
	SendLeftPosition(currentXPos);
}

void CScrollbarHorizontalWnd::ClickRightTriangle()
{
	currentXPos += lineSize;
	TestMaxX();
	MoveBar(currentXPos, themeScroll.colorBar);
	SendLeftPosition(currentXPos);
}

void CScrollbarHorizontalWnd::ClickLeftPage()
{
	currentXPos -= pageSize;
	TestMinX();
	MoveBar(currentXPos, themeScroll.colorBar);
	SendLeftPosition(currentXPos);
}

void CScrollbarHorizontalWnd::ClickRightPage()
{
	currentXPos += pageSize;
	TestMaxX();
	MoveBar(currentXPos, themeScroll.colorBar);
	SendLeftPosition(currentXPos);
}

void CScrollbarHorizontalWnd::DrawElement(wxDC* dc)
{
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = GetWindowWidth();
	rc.height = GetWindowHeight();
	FillRect(dc, rc, themeScroll.colorBack);
	if (showTriangle)
	{
		DrawLeftTriangleElement(dc, rcPosTriangleLeft, themeScroll.colorTriangle);
		DrawRightTriangleElement(dc, rcPosTriangleRight, themeScroll.colorTriangle);
	}
	//DrawRectangleElement(dc, themeScroll.colorBar);
	if (captureBar)
		DrawRectangleElement(dc, themeScroll.colorBarActif);
	else
		DrawRectangleElement(dc, themeScroll.colorBar);
}

void CScrollbarHorizontalWnd::PaintNow()
{
	this->Refresh(false);
}

void CScrollbarHorizontalWnd::on_paint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	DrawElement(&dc);
}

void CScrollbarHorizontalWnd::FillRect(wxDC* dc, const wxRect& rc, const wxColour& color)
{
	auto brush = wxBrush(color);
	dc->SetBrush(brush);
	dc->DrawRectangle(rc);
	dc->SetBrush(wxNullBrush);
}
