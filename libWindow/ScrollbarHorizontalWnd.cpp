#include "header.h"
#include "ScrollbarHorizontalWnd.h"

#include <ClosedHandCursor.h>
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


CScrollbarHorizontalWnd::CScrollbarHorizontalWnd(const wxString &windowName, wxWindow* parent, wxWindowID id, const CThemeScrollBar & theme)
	: CWindowMain(windowName, parent, id)
{


	scrollMoving = false;
	width = 0;
	height = 0;
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

	triangleLeft = new wxTimer(this, TIMER_TRIANGLELEFT);
	triangleRight = new wxTimer(this, TIMER_TRIANGLERIGHT);
	pageLeft = new wxTimer(this, TIMER_PAGELEFT);
	pageRight = new wxTimer(this, TIMER_PAGERIGHT);
	stopMoving = new wxTimer(this, TIMER_STOPMOVING);

	themeScroll = theme;

	Connect(wxEVT_PAINT, wxPaintEventHandler(CScrollbarHorizontalWnd::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CScrollbarHorizontalWnd::OnMouseMove));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CScrollbarHorizontalWnd::OnSize));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CScrollbarHorizontalWnd::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CScrollbarHorizontalWnd::OnLButtonUp));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CScrollbarHorizontalWnd::OnMouseHover));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CScrollbarHorizontalWnd::OnMouseLeave));
	Connect(TIMER_TRIANGLELEFT, wxEVT_TIMER, wxTimerEventHandler(CScrollbarHorizontalWnd::OnTimerTriangleLeft), nullptr, this);
	Connect(TIMER_TRIANGLERIGHT, wxEVT_TIMER, wxTimerEventHandler(CScrollbarHorizontalWnd::OnTimerTriangleRight), nullptr, this);
	Connect(TIMER_PAGELEFT, wxEVT_TIMER, wxTimerEventHandler(CScrollbarHorizontalWnd::OnTimerPageLeft), nullptr, this);
	Connect(TIMER_PAGERIGHT, wxEVT_TIMER, wxTimerEventHandler(CScrollbarHorizontalWnd::OnTimerPageRight), nullptr, this);
	Connect(TIMER_STOPMOVING, wxEVT_TIMER, wxTimerEventHandler(CScrollbarHorizontalWnd::OnTimerStopMoving), nullptr, this);
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CScrollbarHorizontalWnd::OnMouseCaptureLost));
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(CScrollbarHorizontalWnd::OnEraseBackground));
}

void CScrollbarHorizontalWnd::OnMouseCaptureLost(wxMouseEvent& event)
{

}

void CScrollbarHorizontalWnd::SetIsMoving()
{
	if (stopMoving->IsRunning())
		stopMoving->Stop();
	scrollMoving = true;
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

bool CScrollbarHorizontalWnd::SetPosition(const int &left)
{
	bool value = true;
    if (left != currentXPos)
	{
		currentXPos = left;
		TestMaxX();

		if (TestMaxX() && showTriangle)
		{
			ClickRightTriangle();
		}
		else
		{
			barPosX = barStartX + currentXPos / lineSize;

			rcPosBar.x = barPosX;
			rcPosBar.width = barPosX + barSize;
			rcPosBar.y = themeScroll.GetMarge();
			rcPosBar.height = themeScroll.GetRectangleSize() + themeScroll.GetMarge();
		}
	}
	else
	{
		if (TestMaxX())
		{
			barPosX = barStartX + currentXPos / lineSize;
			rcPosBar.x = barPosX;
			rcPosBar.width = barPosX + barSize;
			rcPosBar.y = themeScroll.GetMarge();
			rcPosBar.height = themeScroll.GetRectangleSize() + themeScroll.GetMarge();
		}
		if (TestMinX())
		{
			barPosX = barStartX + currentXPos / lineSize;
			rcPosBar.x = barPosX;
			rcPosBar.width = barPosX + barSize;
			rcPosBar.y = themeScroll.GetMarge();
			rcPosBar.height = themeScroll.GetRectangleSize() + themeScroll.GetMarge();
		}
		value = false;
	}

	PaintNow();
	return value;
}

bool CScrollbarHorizontalWnd::UpdateScrollBar(const int &posLargeur, const int &screenWidth, const int &pictureWidth)
{
	bool needToShow = false;
	bool needToRedraw = false;
	bool returnValue = false;

	if (pictureWidth > screenWidth && !this->IsShown())
	{
		Show(true); needToShow = true;
	}
	else if (pictureWidth <= screenWidth && this->IsShown())
	{
		Show(false); needToShow = true;
	}

	if (this->IsShown())
	{
		returnValue = DefineSize(screenWidth, pictureWidth);
		if (returnValue)
			needToRedraw = true;
		returnValue = SetPosition(posLargeur);
		if (returnValue)
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
    if(showTriangle)
        barStartX = themeScroll.GetMarge() + themeScroll.GetMarge() + themeScroll.GetRectangleSize();
    else
        barStartX = 0;
	barEndX = width - barStartX;

	int diff = pictureWidth - screenWidth;
	float nbPos = float(diff) / float(lineSize);

	barPosX = barStartX + currentXPos / lineSize;
	barSize = (barEndX - barStartX) - nbPos;

	if (barSize < BARSIZEMIN)
	{
		barSize = BARSIZEMIN;
		nbPos = (barEndX - barStartX) - barSize;
		lineSize = float(diff) / float(nbPos);
		pageSize = lineSize * 10;
	}
}

bool CScrollbarHorizontalWnd::DefineSize(const int &screenWidth, const int &pictureWidth)
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


void CScrollbarHorizontalWnd::SetPageSize(const int &pageSize)
{
	pageSizeDefault = pageSize;
	this->pageSize = pageSize;
}

int CScrollbarHorizontalWnd::GetPageSize()
{
	return pageSize;
}

void CScrollbarHorizontalWnd::SetLineSize(const int &lineSize)
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
	delete(triangleLeft);

	if (triangleRight->IsRunning())
		triangleRight->Stop();
	delete(triangleRight);

	if (pageLeft->IsRunning())
		pageLeft->Stop();
	delete(pageLeft);

	if (pageRight->IsRunning())
		pageRight->Stop();
	delete(pageRight);

	if (stopMoving->IsRunning())
		stopMoving->Stop();
	delete(stopMoving);
}

void CScrollbarHorizontalWnd::DrawLeftTriangleElement(wxDC * dc, const wxRect &rc, const wxColour &colorTriangle)
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

void CScrollbarHorizontalWnd::DrawRightTriangleElement(wxDC * dc, const wxRect &rc, const wxColour &colorTriangle)
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

void CScrollbarHorizontalWnd::DrawRectangleElement(wxDC * dc, const wxColour &colorBar)
{
	wxBrush brush = wxBrush(colorBar);
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
	rc.height = height - (themeScroll.GetMarge() * 2);
	dc->DrawRoundedRectangle(rc, (height / 2) - themeScroll.GetMarge());
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
}

void CScrollbarHorizontalWnd::OnMouseHover(wxMouseEvent& event)
{
    wxSetCursor(wxCursor(wxCURSOR_ARROW));
}

void CScrollbarHorizontalWnd::Resize()
{
    if(showTriangle)
        barStartX = themeScroll.GetMarge() + themeScroll.GetMarge() + themeScroll.GetRectangleSize();
    else
        barStartX = 0;
        
	barEndX = width - barStartX;
	//int tailleY = height;

	if (barPosX == 0)
		barPosX = barStartX;

	rcPosTriangleLeft.x = themeScroll.GetMarge();
	rcPosTriangleLeft.width = themeScroll.GetMarge() + themeScroll.GetRectangleSize();
	rcPosTriangleLeft.y = themeScroll.GetMarge();
	rcPosTriangleLeft.height = themeScroll.GetMarge() + themeScroll.GetRectangleSize();

	rcPosTriangleRight.x = (width - barStartX) + themeScroll.GetMarge();
	rcPosTriangleRight.width = width - themeScroll.GetMarge();
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
}

void CScrollbarHorizontalWnd::OnSize(wxSizeEvent& event)
{
    
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	Resize();
    //wxPaintEvent paintevent(wxEVT_PAINT);
    //wxPostEvent(this, paintevent); 
}

void CScrollbarHorizontalWnd::OnMouseMove(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();
	//wxWindowDC dc(this);
	SetIsMoving();

	wxCommandEvent evt(wxEVENT_SCROLLMOVE);
	evt.SetInt(1);
	GetParent()->GetEventHandler()->AddPendingEvent(evt);

	if (captureBar)
	{
		int diffX = xPos - xPositionStart;
		xPositionStartMove = xPositionStart = xPos;
        currentXPos += diffX * lineSize;
        TestMinX();
        TestMaxX();
		MoveBar(currentXPos, themeScroll.colorBarActif);

		SendLeftPosition(currentXPos);

        //scrollInterface->SetLeftPosition(currentXPos);
		PaintNow();
    }
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


bool CScrollbarHorizontalWnd::FindLeftTriangle(const int &yPosition, const int &xPosition)
{
	if (yPosition > rcPosTriangleLeft.y && yPosition < rcPosTriangleLeft.height && xPosition > rcPosTriangleLeft.x && xPosition < rcPosTriangleLeft.width)
	{
		return true;
	}
	return false;
}

bool CScrollbarHorizontalWnd::FindRightTriangle(const int &yPosition, const int &xPosition)
{
	if (yPosition > rcPosTriangleRight.y && yPosition < rcPosTriangleRight.height && xPosition > rcPosTriangleRight.x && xPosition < rcPosTriangleRight.width)
	{
		return true;
	}
	return false;
}

bool CScrollbarHorizontalWnd::FindRectangleBar(const int &yPosition, const int &xPosition)
{
	if (yPosition > rcPosBar.y && yPosition < rcPosBar.height && xPosition > rcPosBar.x && xPosition < rcPosBar.width)
	{
		return true;
	}
	return false;
}


void CScrollbarHorizontalWnd::MoveBar(const int &currentPos, wxColour color)
{
	/*
	wxBufferedPaintDC dc(this);

	wxRect rc;
	rc.x = rcPosTriangleLeft.width;
	rc.width = rcPosTriangleRight.x - rcPosTriangleLeft.width;
	rc.y = 0;
	rc.height = height;

	FillRect(&dc, rc, themeScroll.colorBack);
	*/

	int diff = pictureWidth - screenWidth;
	float currentPosPourcentage = ((float)currentPos / (float)diff);
	float sizeFree = (barEndX - barStartX) - barSize;
	int posX = sizeFree * currentPosPourcentage;

	rcPosBar.x = barStartX + posX;
	rcPosBar.width = barStartX + posX + barSize;

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

	//DrawRectangleElement(&dc, color);
	PaintNow();
}

void CScrollbarHorizontalWnd::OnLButtonDown(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();
	//bool initTimer = false;
    
    //if(showTriangle)

	if (showTriangle && FindLeftTriangle(yPos, xPos))
	{
		ClickLeftTriangle();
		//initTimer = true;
		triangleLeft->Start(100);
	}
	else if (showTriangle && FindRightTriangle(yPos, xPos))
	{
		ClickRightTriangle();
		//initTimer = true;
		triangleRight->Start(100);
	}
	else if (FindRectangleBar(yPos, xPos))
	{
		xPositionStart = xPos;
		xPositionStartMove = xPos;
		CaptureMouse();
		captureBar = true;
	}
	else if (xPos > rcPosBar.width)
	{
		ClickRightPage();
		//initTimer = true;
		pageRight->Start(100);
	}
	else if (xPos < rcPosBar.x)
	{
		ClickLeftPage();
		//initTimer = true;
		pageLeft->Start(100);
	}

}

void CScrollbarHorizontalWnd::OnLButtonUp(wxMouseEvent& event)
{
	int xPos = event.GetX();
	//int yPos = event.GetY();
	if (captureBar)
	{
        if(HasCapture())
            ReleaseMouse();
		captureBar = false;
		int diff = xPos - xPositionStartMove;

		currentXPos += diff * lineSize;
		TestMinX();
		TestMaxX();

		SendLeftPosition(currentXPos);
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
	scrollMoving = false;
	if (stopMoving->IsRunning())
		stopMoving->Stop();
}

void CScrollbarHorizontalWnd::SendLeftPosition(const int &value)
{
	wxWindow * window = this->GetParent();
	if (window != nullptr)
	{
		wxCommandEvent evt(wxEVENT_LEFTPOSITION);
		evt.SetInt(value);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CScrollbarHorizontalWnd::ClickLeftTriangle()
{
	//Click Top Triangle
	//MoveBar(-1, themeScroll.colorBar);
	currentXPos -= lineSize;
	TestMinX();
	MoveBar(currentXPos, themeScroll.colorBar);
	SendLeftPosition(currentXPos);
}

void CScrollbarHorizontalWnd::ClickRightTriangle()
{
	//Click Top Triangle
	//MoveBar(1, themeScroll.colorBar);
	
	currentXPos += lineSize;
	TestMaxX();
	//SetPosition(currentXPos);
	MoveBar(currentXPos, themeScroll.colorBar);
	SendLeftPosition(currentXPos);
}

void CScrollbarHorizontalWnd::ClickLeftPage()
{
	//Click Top Triangle
	//int sizeMove = pageSize / lineSize;
	//MoveBar(-sizeMove, themeScroll.colorBar);
	currentXPos -= pageSize;
	TestMinX();
	//SetPosition(currentXPos);
	MoveBar(currentXPos, themeScroll.colorBar);
	SendLeftPosition(currentXPos);
}

void CScrollbarHorizontalWnd::ClickRightPage()
{
	//Click Top Triangle
	//int sizeMove = pageSize / lineSize;
	//MoveBar(sizeMove, themeScroll.colorBar);
	currentXPos += pageSize;
	TestMaxX();
	//SetPosition(currentXPos);
	MoveBar(currentXPos, themeScroll.colorBar);
	SendLeftPosition(currentXPos);
}

void CScrollbarHorizontalWnd::DrawElement(wxDC * dc)
{
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = width;
	rc.height = height;
	FillRect(dc, rc, themeScroll.colorBack);
    if(showTriangle)
    {
        DrawLeftTriangleElement(dc, rcPosTriangleLeft, themeScroll.colorTriangle);
        DrawRightTriangleElement(dc, rcPosTriangleRight, themeScroll.colorTriangle);
    }
	//DrawRectangleElement(dc, themeScroll.colorBar);
    if (captureBar)
        DrawRectangleElement(dc, themeScroll.colorBarActif);
    else
        DrawRectangleElement(dc, themeScroll.colorBar);    
#if defined(WIN32) && defined(_DEBUG)
	//OutputDebugString(L"CScrollbarHorizontalWnd::OnPaint \n");
#endif
}

void CScrollbarHorizontalWnd::PaintNow()
{
	wxClientDC dc(this);
	DrawElement(&dc);
}

void CScrollbarHorizontalWnd::OnPaint(wxPaintEvent& event)
{
	//wxBufferedPaintDC dc(this);
    wxPaintDC dc(this);
	DrawElement(&dc);
}

void CScrollbarHorizontalWnd::FillRect(wxDC * dc, const wxRect &rc, const wxColour &color)
{
	wxBrush brush = wxBrush(color);
	dc->SetBrush(brush);
	dc->DrawRectangle(rc);
	dc->SetBrush(wxNullBrush);
}

/*
void CScrollbarHorizontalWnd::DrawRectangleElement(wxDC* dc, const wxColour& colorBar)
{
	wxBrush brush = wxBrush(colorBar);
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
	rc.height = height - (themeScroll.GetMarge() * 2);
	dc->DrawRoundedRectangle(rc, (height / 2) - themeScroll.GetMarge());
	dc->SetBrush(wxNullBrush);
	//FillRect(dc, rc, colorBar);
}
*/