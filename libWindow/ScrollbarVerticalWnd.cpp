#include "header.h"
#include "ScrollbarVerticalWnd.h"

#include <window_id.h>
#include <wx/dcbuffer.h>
using namespace Regards::Window;

#define BARSIZEMIN 20
#define LINESIZE 5
#define PAGESIZE 50

enum
{
	TIMER_TRIANGLETOP = 10,
	TIMER_TRIANGLEBOTTOM = 11,
	TIMER_PAGETOP = 12,
	TIMER_PAGEBOTTOM = 13,
	TIMER_STOPMOVING = 14
};

CScrollbarVerticalWnd::CScrollbarVerticalWnd(const wxString& windowName, wxWindow* parent, wxWindowID id,
                                             const CThemeScrollBar& theme)
	: CWindowMain(windowName, parent, id)
{
	scrollMoving = false;
	showTriangle = true;
	triangleTop = nullptr;
	triangleBottom = nullptr;
	pageTop = nullptr;
	pageBottom = nullptr;
	stopMoving = nullptr;
	m_bTracking = false;
	barSize = 40;
	barPosY = 0;
	showEmptyRectangle = false;
	captureBar = false;
	barStartY = 0;
	barEndY = 0;
	pageSize = 50;
	lineSize = 5;
	pageSizeDefault = 50;
	lineSizeDefault = 5;
	pictureHeight = 0;
	screenHeight = 0;
	currentYPos = 0;

	themeScroll = theme;

	triangleTop = new wxTimer(this, TIMER_TRIANGLETOP);
	triangleBottom = new wxTimer(this, TIMER_TRIANGLEBOTTOM);
	pageTop = new wxTimer(this, TIMER_PAGETOP);
	pageBottom = new wxTimer(this, TIMER_PAGEBOTTOM);
	stopMoving = new wxTimer(this, TIMER_STOPMOVING);

	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(CScrollbarVerticalWnd::OnEraseBackground));
	Connect(wxEVT_PAINT, wxPaintEventHandler(CScrollbarVerticalWnd::on_paint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CScrollbarVerticalWnd::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CScrollbarVerticalWnd::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CScrollbarVerticalWnd::OnLButtonUp));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CScrollbarVerticalWnd::OnMouseHover));
	//	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CScrollbarVerticalWnd::OnMouseLeave));
	Connect(TIMER_TRIANGLETOP, wxEVT_TIMER, wxTimerEventHandler(CScrollbarVerticalWnd::OnTimerTriangleTop), nullptr,
	        this);
	Connect(TIMER_TRIANGLEBOTTOM, wxEVT_TIMER, wxTimerEventHandler(CScrollbarVerticalWnd::OnTimerTriangleBottom),
	        nullptr, this);
	Connect(TIMER_PAGETOP, wxEVT_TIMER, wxTimerEventHandler(CScrollbarVerticalWnd::OnTimerPageTop), nullptr, this);
	Connect(TIMER_PAGEBOTTOM, wxEVT_TIMER, wxTimerEventHandler(CScrollbarVerticalWnd::OnTimerPageBottom), nullptr,
	        this);
	Connect(TIMER_STOPMOVING, wxEVT_TIMER, wxTimerEventHandler(CScrollbarVerticalWnd::OnTimerStopMoving), nullptr,
	        this);
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CScrollbarVerticalWnd::OnMouseCaptureLost));
}


void CScrollbarVerticalWnd::OnMouseCaptureLost(wxMouseEvent& event)
{
}

void CScrollbarVerticalWnd::UpdateScreenRatio()
{
	Resize();
}

int CScrollbarVerticalWnd::GetWidthSize()
{
	return themeScroll.GetRectangleSize() + (themeScroll.GetMarge() * 2);
}

void CScrollbarVerticalWnd::CalculBarSize()
{
	if (showTriangle)
		barStartY = themeScroll.GetMarge() + themeScroll.GetMarge() + themeScroll.GetRectangleSize();
	else
		barStartY = 0; //themeScroll.GetRectangleSize();

	barEndY = GetWindowHeight() - barStartY;

	if (showEmptyRectangle)
	{
		barEndY -= heightSize;
	}

	int diff = pictureHeight - screenHeight;
	float nbPos = static_cast<float>(diff) / static_cast<float>(lineSize);

	barPosY = barStartY + currentYPos / lineSize;
	barSize = (barEndY - barStartY) - static_cast<int>(nbPos);

	if (barSize < BARSIZEMIN)
	{
		barSize = BARSIZEMIN;
		nbPos = static_cast<float>((barEndY - barStartY) - barSize);
		lineSize = static_cast<int>(static_cast<float>(diff) / nbPos);
	}

	pageSize = lineSize * 10;
}


bool CScrollbarVerticalWnd::DefineSize(const int& screenHeight, const int& pictureHeight)
{
	if (this->pictureHeight == pictureHeight && this->screenHeight == screenHeight)
	{
		return false;
	}

	this->pictureHeight = pictureHeight;
	this->screenHeight = screenHeight;


	if (barEndY > 0)
	{
		pageSize = pageSizeDefault;
		lineSize = lineSizeDefault;
		CalculBarSize();
		rcPosBar.x = themeScroll.GetMarge();
		rcPosBar.width = themeScroll.GetRectangleSize() + themeScroll.GetMarge();
		rcPosBar.y = barPosY;
		rcPosBar.height = barPosY + barSize;
	}
	return true;
}

bool CScrollbarVerticalWnd::UpdateScrollBar(const int& posHauteur, const int& screenHeight, const int& pictureHeight)
{
	bool needToShow = false;


	if (pictureHeight > screenHeight && !this->IsShown())
	{
		Show(true);
		needToShow = true;
	}
	else if (pictureHeight <= screenHeight && IsShown())
	{
		Show(false);
		needToShow = true;
	}

	if (IsShown())
	{
		bool needToRedraw = false;
		bool return_value = DefineSize(screenHeight, pictureHeight);
		if (return_value)
			needToRedraw = true;
		return_value = SetPosition(posHauteur);
		if (return_value)
			needToRedraw = true;

		if (needToRedraw)
		{
			PaintNow();
		}
	}

	return needToShow;
}

int CScrollbarVerticalWnd::GetPosition()
{
	return currentYPos;
}

int CScrollbarVerticalWnd::GetScreenHeight()
{
	return screenHeight;
}

int CScrollbarVerticalWnd::GetPictureHeight()
{
	return pictureHeight;
}

bool CScrollbarVerticalWnd::SetPosition(const int& top)
{
	bool value = true;
	if (top != currentYPos)
	{
		currentYPos = top;
		MoveBar(currentYPos, themeScroll.colorBarActif);
	}
	else
		value = false;
	return value;
}

void CScrollbarVerticalWnd::DrawElement(wxDC* dc)
{
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = GetWindowWidth();
	rc.height = GetWindowHeight();

	FillRect(dc, rc, themeScroll.colorBack);

	if (showTriangle)
	{
		DrawTopTriangleElement(dc, rcPosTriangleTop, themeScroll.colorTriangle);
		DrawBottomTriangleElement(dc, rcPosTriangleBottom, themeScroll.colorTriangle);
	}
	if (captureBar)
		DrawRectangleElement(dc, themeScroll.colorBarActif);
	else
		DrawRectangleElement(dc, themeScroll.colorBar);
}

void CScrollbarVerticalWnd::ShowEmptyRectangle(const bool& show, const int& heightSize)
{
	if (showEmptyRectangle != show)
	{
		showEmptyRectangle = show;
		this->heightSize = heightSize;
		Resize();
	}
	//this->Redraw();
}

void CScrollbarVerticalWnd::SetPageSize(const int& pageSize)
{
	pageSizeDefault = pageSize;
	this->pageSize = pageSize;
}

int CScrollbarVerticalWnd::GetPageSize()
{
	return pageSize;
}

void CScrollbarVerticalWnd::SetLineSize(const int& lineSize)
{
	lineSizeDefault = lineSize;
	this->lineSize = lineSize;
}

int CScrollbarVerticalWnd::GetLineSize()
{
	return lineSize;
}


CScrollbarVerticalWnd::~CScrollbarVerticalWnd()
{
	if (triangleTop->IsRunning())
		triangleTop->Stop();
	delete(triangleTop);

	if (triangleBottom->IsRunning())
		triangleBottom->Stop();
	delete(triangleBottom);

	if (pageTop->IsRunning())
		pageTop->Stop();
	delete(pageTop);

	if (pageBottom->IsRunning())
		pageBottom->Stop();
	delete(pageBottom);

	if (stopMoving->IsRunning())
		stopMoving->Stop();
	delete(stopMoving);
}

void CScrollbarVerticalWnd::DrawTopTriangleElement(wxDC* dc, const wxRect& rc, const wxColour& color)
{
	wxBrush brushHatch(color);
	dc->SetBrush(brushHatch);
	wxPoint star[3];
	star[0] = wxPoint((rc.width - rc.x) / 2 + themeScroll.GetMarge(), rc.y);
	star[1] = wxPoint(rc.x, rc.height);
	star[2] = wxPoint(rc.width, rc.height);
	dc->DrawPolygon(WXSIZEOF(star), star, 0, 0);
	dc->SetBrush(wxNullBrush);
}


void CScrollbarVerticalWnd::DrawBottomTriangleElement(wxDC* dc, const wxRect& rc, const wxColour& color)
{
	wxBrush brushHatch(color);
	dc->SetBrush(brushHatch);
	wxPoint star[3];
	star[0] = wxPoint((rc.width - rc.x) / 2 + themeScroll.GetMarge(), rc.height);
	star[1] = wxPoint(rc.x, rc.y);
	star[2] = wxPoint(rc.width, rc.y);
	dc->DrawPolygon(WXSIZEOF(star), star, 0, 0);
}

void CScrollbarVerticalWnd::DrawRectangleElement(wxDC* dc, const wxColour& colorBar)
{
	/*
	wxRect rc = rcPosBar;
	if (rcPosBar.height > barEndY)
	{
		rcPosBar.y = barEndY - barSize;
		rcPosBar.height = barEndY;
	}

	if (rcPosBar.y < barStartY)
	{
		rcPosBar.y = barStartY;
		rcPosBar.height = barStartY + barSize;
	}

	rc.height = rcPosBar.height - rcPosBar.y;
	rc.width = width - (themeScroll.GetMarge() * 2);
	FillRect(dc, rc, colorBar);
	*/

	auto brush = wxBrush(colorBar);
	dc->SetBrush(brush);
	wxRect rc = rcPosBar;
	if (rcPosBar.height > barEndY)
	{
		rcPosBar.y = barEndY - barSize;
		rcPosBar.height = barEndY;
	}

	if (rcPosBar.y < barStartY)
	{
		rcPosBar.y = barStartY;
		rcPosBar.height = barStartY + barSize;
	}

	rc.height = rcPosBar.height - rcPosBar.y;
	rc.width = GetWindowWidth() - (themeScroll.GetMarge() * 2);
	dc->DrawRoundedRectangle(rc, (GetWindowWidth() / 2) - themeScroll.GetMarge());
	dc->SetBrush(wxNullBrush);
}

void CScrollbarVerticalWnd::SetIsMoving()
{
	if (stopMoving->IsRunning())
		stopMoving->Stop();

	scrollMoving = true;
	stopMoving->Start(1000);

	wxCommandEvent evt(wxEVENT_SCROLLMOVE);
	evt.SetInt(1);
	GetParent()->GetEventHandler()->AddPendingEvent(evt);
}

bool CScrollbarVerticalWnd::IsMoving()
{
	return scrollMoving;
}


void CScrollbarVerticalWnd::OnMouseLeave(wxMouseEvent& event)
{
	m_bTracking = FALSE;

	if (triangleTop->IsRunning())
		triangleTop->Stop();

	if (triangleBottom->IsRunning())
		triangleBottom->Stop();

	if (pageTop->IsRunning())
		pageTop->Stop();

	if (pageBottom->IsRunning())
		pageBottom->Stop();

	if (stopMoving->IsRunning())
		stopMoving->Stop();

	scrollMoving = false;

	wxCommandEvent evt(wxEVENT_SCROLLMOVE);
	evt.SetInt(0);
	GetParent()->GetEventHandler()->AddPendingEvent(evt);

	if (HasCapture())
		ReleaseMouse();
	captureBar = false;
}

void CScrollbarVerticalWnd::OnMouseHover(wxMouseEvent& events)
{
	//::wxSetCursor(wxCursor( wxSTANDARD_CURSOR ) );
	wxSetCursor(wxCursor(wxCURSOR_ARROW));
}

void CScrollbarVerticalWnd::Resize()
{
	if (showTriangle)
		barStartY = themeScroll.GetMarge() + themeScroll.GetMarge() + themeScroll.GetRectangleSize();
	else
		barStartY = themeScroll.GetRectangleSize();
	barEndY = GetWindowHeight() - barStartY;
	int tailleY = GetWindowHeight();

	if (showEmptyRectangle)
	{
		barEndY -= heightSize;
		tailleY -= heightSize;
	}

	if (barPosY == 0)
		barPosY = barStartY;

	if (showTriangle)
	{
		rcPosTriangleTop.x = themeScroll.GetMarge();
		rcPosTriangleTop.width = themeScroll.GetMarge() + themeScroll.GetRectangleSize();
		rcPosTriangleTop.y = themeScroll.GetMarge();
		rcPosTriangleTop.height = themeScroll.GetMarge() + themeScroll.GetRectangleSize();

		rcPosTriangleBottom.x = themeScroll.GetMarge();
		rcPosTriangleBottom.width = themeScroll.GetMarge() + themeScroll.GetRectangleSize();
		rcPosTriangleBottom.y = tailleY - themeScroll.GetRectangleSize() - themeScroll.GetMarge();
		rcPosTriangleBottom.height = tailleY - themeScroll.GetMarge();
	}


	if (barEndY > 0)
	{
		pageSize = pageSizeDefault;
		lineSize = lineSizeDefault;
		CalculBarSize();
		rcPosBar.x = themeScroll.GetMarge();
		rcPosBar.width = themeScroll.GetRectangleSize() + themeScroll.GetMarge();
		rcPosBar.y = barPosY;
		rcPosBar.height = barPosY + barSize;
	}

	needToRefresh = true;
}


bool CScrollbarVerticalWnd::FindTopTriangle(const int& yPosition, const int& xPosition)
{
	if (yPosition > rcPosTriangleTop.y && yPosition < rcPosTriangleTop.height && xPosition > rcPosTriangleTop.x &&
		xPosition < rcPosTriangleTop.width)
	{
		return true;
	}
	return false;
}

bool CScrollbarVerticalWnd::FindBottomTriangle(const int& yPosition, const int& xPosition)
{
	if (yPosition > rcPosTriangleBottom.y && yPosition < rcPosTriangleBottom.height && xPosition > rcPosTriangleBottom.x
		&& xPosition < rcPosTriangleBottom.width)
	{
		return true;
	}
	return false;
}

bool CScrollbarVerticalWnd::FindRectangleBar(const int& yPosition, const int& xPosition)
{
	if (yPosition > rcPosBar.y && yPosition < rcPosBar.height && xPosition > rcPosBar.x && xPosition < rcPosBar.width)
	{
		return true;
	}
	return false;
}

void CScrollbarVerticalWnd::MoveBar(const int& currentPos, wxColour color)
{
	int diff = pictureHeight - screenHeight;
	float currentPosPourcentage = (static_cast<float>(currentPos) / static_cast<float>(diff));
	float sizeFree = (barEndY - barStartY) - barSize;
	int posY = sizeFree * currentPosPourcentage;

	rcPosBar.y = barStartY + posY;
	rcPosBar.height = barStartY + posY + barSize;

	if (rcPosBar.height > barEndY)
	{
		rcPosBar.y = barEndY - barSize;
		rcPosBar.height = barEndY;
	}

	if (rcPosBar.y < barStartY)
	{
		rcPosBar.y = barStartY;
		rcPosBar.height = barStartY + barSize;
	}
	PaintNow();
}

void CScrollbarVerticalWnd::OnMouseMove(wxMouseEvent& event)
{
	//int xPos = event.GetX();
	int yPos = event.GetY();
	//wxClientDC dc(this);


	if (captureBar)
	{
		scrollMoving = true;
		int diffY = yPos - yPositionStart;
		yPositionStartMove = yPositionStart = yPos;
		currentYPos += diffY * lineSize;
		TestMinY();
		TestMaxY();
		MoveBar(currentYPos, themeScroll.colorBarActif);
		SendTopPosition(currentYPos);
		PaintNow();
		SetIsMoving();
	}
}

void CScrollbarVerticalWnd::SetShowWindow(const bool& showValue)
{
	this->showWindow = showValue;
}

void CScrollbarVerticalWnd::SendTopPosition(const int& value)
{
	if (showWindow)
	{
		wxWindow* window = this->GetParent();
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_TOPPOSITION);
			evt.SetInt(value);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
	}
}

bool CScrollbarVerticalWnd::TestMaxY()
{
	int diff = pictureHeight - screenHeight;
	//if (showEmptyRectangle)
	//	diff -= heightSize;
	if (currentYPos > diff)
	{
		currentYPos = diff;
		return true;
	}
	return false;
}

bool CScrollbarVerticalWnd::TestMinY()
{
	if (currentYPos < 0)
	{
		currentYPos = 0;
		return true;
	}
	return false;
}

void CScrollbarVerticalWnd::ClickTopTriangle()
{
	//Click Top Triangle
	//MoveBar(-1, themeScroll.colorBar);
	currentYPos -= lineSize;
	TestMinY();
	MoveBar(currentYPos, themeScroll.colorBar);
	SendTopPosition(currentYPos);
}

void CScrollbarVerticalWnd::ClickBottomTriangle()
{
	//Click Bottom Triangle
	//MoveBar(1, themeScroll.colorBar);
	currentYPos += lineSize;
	TestMaxY();
	MoveBar(currentYPos, themeScroll.colorBar);
	SendTopPosition(currentYPos);
}

void CScrollbarVerticalWnd::ClickTopPage()
{
	//int sizeMove = pageSize / lineSize;
	//MoveBar(-sizeMove, themeScroll.colorBar);
	currentYPos -= pageSize;
	TestMinY();
	MoveBar(currentYPos, themeScroll.colorBar);
	SendTopPosition(currentYPos);
}

void CScrollbarVerticalWnd::ClickBottomPage()
{
	//int sizeMove = pageSize / lineSize;
	//MoveBar(sizeMove, themeScroll.colorBar);
	currentYPos += pageSize;
	TestMaxY();
	MoveBar(currentYPos, themeScroll.colorBar);
	SendTopPosition(currentYPos);
}


void CScrollbarVerticalWnd::OnLButtonDown(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();
	//bool initTimer = false;

	//if(showTriangle)

	if (showTriangle && FindTopTriangle(yPos, xPos))
	{
		scrollMoving = true;
		//initTimer = true;
		ClickTopTriangle();
		triangleTop->Start(100);
	}
	else if (showTriangle && FindBottomTriangle(yPos, xPos))
	{
		//SetIsMoving();
		//initTimer = true;
		ClickBottomTriangle();
		triangleBottom->Start(100);
	}
	else if (FindRectangleBar(yPos, xPos))
	{
		//SetIsMoving();
		yPositionStartMove = yPositionStart = yPos;
		CaptureMouse();
		captureBar = true;
	}
	else if (yPos > rcPosBar.height)
	{
		//SetIsMoving();
		//initTimer = true;
		ClickBottomPage();
		pageBottom->Start(100);
	}
	else if (yPos < rcPosBar.y)
	{
		//initTimer = true;
		ClickTopPage();
		pageTop->Start(100);
		//SetIsMoving();
	}
}

void CScrollbarVerticalWnd::OnTimerTriangleTop(wxTimerEvent& event)
{
	ClickTopTriangle();
}

void CScrollbarVerticalWnd::OnTimerTriangleBottom(wxTimerEvent& event)
{
	ClickBottomTriangle();
}

void CScrollbarVerticalWnd::OnTimerPageTop(wxTimerEvent& event)
{
	ClickTopPage();
}

void CScrollbarVerticalWnd::OnTimerPageBottom(wxTimerEvent& event)
{
	ClickBottomPage();
}


void CScrollbarVerticalWnd::OnTimerStopMoving(wxTimerEvent& event)
{
	scrollMoving = false;
	if (stopMoving->IsRunning())
		stopMoving->Stop();
}

void CScrollbarVerticalWnd::OnLButtonUp(wxMouseEvent& event)
{
	//	int xPos = event.GetX();
	int yPos = event.GetY();

	if (captureBar)
	{
		if (HasCapture())
			ReleaseMouse();
		captureBar = false;
		int diff = yPos - yPositionStartMove;

		currentYPos += diff * lineSize;
		TestMinY();
		TestMaxY();
		SendTopPosition(currentYPos);
	}

	if (triangleTop->IsRunning())
		triangleTop->Stop();
	if (triangleBottom->IsRunning())
		triangleBottom->Stop();
	if (pageTop->IsRunning())
		pageTop->Stop();
	if (pageBottom->IsRunning())
		pageBottom->Stop();

	captureBar = false;
}

void CScrollbarVerticalWnd::PaintNow()
{
	//wxClientDC dc(this);
	//DrawElement(&dc);

	this->Refresh();
}

void CScrollbarVerticalWnd::on_paint(wxPaintEvent& event)
{
	//wxBufferedPaintDC dc(this);
	wxPaintDC dc(this);
	//wxAutoBufferedPaintDC dc(this);
	//wxGraphicsContext* gc = wxGraphicsContext::Create(dc);    
	DrawElement(&dc);
}

void CScrollbarVerticalWnd::FillRect(wxDC* dc, const wxRect& rc, const wxColour& color)
{
	auto brush = wxBrush(color);
	dc->SetBrush(brush);
	dc->DrawRectangle(rc);
	dc->SetBrush(wxNullBrush);
}

/*
void CScrollbarVerticalWnd::DrawRectangleElement(wxDC * dc, const wxColour &colorBar)
{
	wxBrush brush = wxBrush(colorBar);
	dc->SetBrush(brush);
	wxRect rc = rcPosBar;
	if (rcPosBar.height > barEndY)
	{
		rcPosBar.y = barEndY - barSize;
		rcPosBar.height = barEndY;
	}

	if (rcPosBar.y < barStartY)
	{
		rcPosBar.y = barStartY;
		rcPosBar.height = barStartY + barSize;
	}

	rc.height = rcPosBar.height - rcPosBar.y;
	rc.width = width - (themeScroll.GetMarge() * 2);
	//FillRect(dc, rc, colorBar);

	dc->DrawRoundedRectangle(rc, (width / 2) - themeScroll.GetMarge());
	dc->SetBrush(wxNullBrush);
}

*/
