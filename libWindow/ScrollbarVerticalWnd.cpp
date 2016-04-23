#include "ScrollbarVerticalWnd.h"
#include "ScrollInterface.h"
using namespace Regards::Window;

#define BARSIZEMIN 10
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

CScrollbarVerticalWnd::CScrollbarVerticalWnd(CScrollInterface * scrollInterface, wxWindow* parent, wxWindowID id, const CThemeScrollBar & theme)
	:wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0)
{
	m_bTracking = false;
	this->scrollInterface = scrollInterface;
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
	Connect(wxEVT_PAINT, wxPaintEventHandler(CScrollbarVerticalWnd::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CScrollbarVerticalWnd::OnMouseMove));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CScrollbarVerticalWnd::OnSize));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CScrollbarVerticalWnd::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CScrollbarVerticalWnd::OnLButtonUp));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CScrollbarVerticalWnd::OnMouseHover));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CScrollbarVerticalWnd::OnMouseLeave));
	Connect(TIMER_TRIANGLETOP, wxEVT_TIMER, wxTimerEventHandler(CScrollbarVerticalWnd::OnTimerTriangleTop), nullptr, this);
	Connect(TIMER_TRIANGLEBOTTOM, wxEVT_TIMER, wxTimerEventHandler(CScrollbarVerticalWnd::OnTimerTriangleBottom), nullptr, this);
	Connect(TIMER_PAGETOP, wxEVT_TIMER, wxTimerEventHandler(CScrollbarVerticalWnd::OnTimerPageTop), nullptr, this);
	Connect(TIMER_PAGEBOTTOM, wxEVT_TIMER, wxTimerEventHandler(CScrollbarVerticalWnd::OnTimerPageBottom), nullptr, this);
	Connect(TIMER_STOPMOVING, wxEVT_TIMER, wxTimerEventHandler(CScrollbarVerticalWnd::OnTimerStopMoving), nullptr, this);
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
	barStartY = themeScroll.GetMarge() + themeScroll.GetMarge() + themeScroll.GetRectangleSize();
	barEndY = height - barStartY;

	if (showEmptyRectangle)
	{
		barEndY -= heightSize;
	}

	int diff = pictureHeight - screenHeight;
	float nbPos = float(diff) / float(lineSize);

	barPosY = barStartY + currentYPos / lineSize;
	barSize = (barEndY - barStartY) - int(nbPos);

	if (barSize < BARSIZEMIN)
	{
		barSize = BARSIZEMIN;
		nbPos = float((barEndY - barStartY) - barSize);
		lineSize = int(float(diff) / nbPos);
	}

	pageSize = lineSize * 10;
}


bool CScrollbarVerticalWnd::DefineSize(const int &screenHeight, const int &pictureHeight)
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

bool CScrollbarVerticalWnd::UpdateScrollBar(const int &posHauteur, const int &screenHeight, const int &pictureHeight)
{
	bool needToShow = false;
	bool needToRedraw = false;
	bool returnValue = false;


	
	if (pictureHeight > screenHeight && !this->IsShown())
	{
		Show(true); needToShow = true;
	}
	else if (pictureHeight <= screenHeight && IsShown())
	{
		Show(false); needToShow = true;
	}

	if (IsShown())
	{
		returnValue = DefineSize(screenHeight, pictureHeight);
		if (returnValue)
			needToRedraw = true;
		returnValue = SetPosition(posHauteur);
		if (returnValue)
			needToRedraw = true;

		if (needToRedraw)
			Refresh();
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

bool CScrollbarVerticalWnd::SetPosition(const int &top)
{
	if (top != currentYPos)
	{
		currentYPos = top;

		if (TestMaxY())
		{
			ClickBottomTriangle();
		}
		else
		{
			barPosY = barStartY + currentYPos / lineSize;

			//Test si bar est trop haut


			rcPosBar.x = themeScroll.GetMarge();
			rcPosBar.width = themeScroll.GetRectangleSize() + themeScroll.GetMarge();
			rcPosBar.y = barPosY;
			rcPosBar.height = barPosY + barSize;
		}
		return true;
	}
	else
	{
		if (TestMaxY())
		{
			barPosY = barStartY + currentYPos / lineSize;
			rcPosBar.x = themeScroll.GetMarge();
			rcPosBar.width = themeScroll.GetRectangleSize() + themeScroll.GetMarge();
			rcPosBar.y = barPosY;
			rcPosBar.height = barPosY + barSize;
		}
		if (TestMinY())
		{
			barPosY = barStartY + currentYPos / lineSize;
			rcPosBar.x = themeScroll.GetMarge();
			rcPosBar.width = themeScroll.GetRectangleSize() + themeScroll.GetMarge();
			rcPosBar.y = barPosY;
			rcPosBar.height = barPosY + barSize;
		}
	}
	return false;
}

void CScrollbarVerticalWnd::ShowEmptyRectangle(const bool &show, const int &heightSize)
{
	if (showEmptyRectangle != show)
	{
		showEmptyRectangle = show;
		this->heightSize = heightSize;
		Resize();
	}
	//this->Redraw();
}

void CScrollbarVerticalWnd::SetPageSize(const int &pageSize)
{
	pageSizeDefault = pageSize;
	this->pageSize = pageSize;
}

int CScrollbarVerticalWnd::GetPageSize()
{
	return pageSize;
}

void CScrollbarVerticalWnd::SetLineSize(const int &lineSize)
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

void CScrollbarVerticalWnd::DrawTopTriangleElement(wxDC * dc, const wxRect &rc, const wxColour &color)
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


void CScrollbarVerticalWnd::DrawBottomTriangleElement(wxDC * dc, const wxRect &rc, const wxColour &color)
{
	wxBrush brushHatch(color);
	dc->SetBrush(brushHatch);
	wxPoint star[3];
	star[0] = wxPoint((rc.width - rc.x) / 2 + themeScroll.GetMarge(), rc.height);
	star[1] = wxPoint(rc.x, rc.y);
	star[2] = wxPoint(rc.width, rc.y);
	dc->DrawPolygon(WXSIZEOF(star), star, 0, 0);
}

void CScrollbarVerticalWnd::DrawRectangleElement(wxDC * dc, const wxColour &colorBar)
{
	wxRect rc = rcPosBar;
	rc.height = rcPosBar.height - rcPosBar.y;
	rc.width = width - (themeScroll.GetMarge() * 2);
	FillRect(dc, rc, colorBar);
}

void CScrollbarVerticalWnd::SetIsMoving()
{
	if (stopMoving->IsRunning())
		stopMoving->Stop();

	scrollMoving = true;
	stopMoving->Start(1000);
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
}

void CScrollbarVerticalWnd::OnMouseHover(wxMouseEvent& events)
{
}

void CScrollbarVerticalWnd::Resize()
{
	barStartY = themeScroll.GetMarge() + themeScroll.GetMarge() + themeScroll.GetRectangleSize();
	barEndY = height - barStartY;
	int tailleY = height;

	if (showEmptyRectangle)
	{
		barEndY -= heightSize;
		tailleY -= heightSize;
	}

	if (barPosY == 0)
		barPosY = barStartY;

	rcPosTriangleTop.x = themeScroll.GetMarge();
	rcPosTriangleTop.width = themeScroll.GetMarge() + themeScroll.GetRectangleSize();
	rcPosTriangleTop.y = themeScroll.GetMarge();
	rcPosTriangleTop.height = themeScroll.GetMarge() + themeScroll.GetRectangleSize();

	rcPosTriangleBottom.x = themeScroll.GetMarge();
	rcPosTriangleBottom.width = themeScroll.GetMarge() + themeScroll.GetRectangleSize();
	rcPosTriangleBottom.y = tailleY - themeScroll.GetRectangleSize() - themeScroll.GetMarge();
	rcPosTriangleBottom.height = tailleY - themeScroll.GetMarge();

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
}

void CScrollbarVerticalWnd::OnSize(wxSizeEvent& event)
{
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	Resize();
	this->Refresh();
}

bool CScrollbarVerticalWnd::FindTopTriangle(const int &yPosition, const int &xPosition)
{
	if (yPosition > rcPosTriangleTop.y && yPosition < rcPosTriangleTop.height && xPosition > rcPosTriangleTop.x && xPosition < rcPosTriangleTop.width)
	{
		return true;
	}
	return false;
}

bool CScrollbarVerticalWnd::FindBottomTriangle(const int &yPosition, const int &xPosition)
{
	if (yPosition > rcPosTriangleBottom.y && yPosition < rcPosTriangleBottom.height && xPosition > rcPosTriangleBottom.x && xPosition < rcPosTriangleBottom.width)
	{
		return true;
	}
	return false;
}

bool CScrollbarVerticalWnd::FindRectangleBar(const int &yPosition, const int &xPosition)
{
	if (yPosition > rcPosBar.y && yPosition < rcPosBar.height && xPosition > rcPosBar.x && xPosition < rcPosBar.width)
	{
		return true;
	}
	return false;
}

void CScrollbarVerticalWnd::MoveBar(const int &diffY, wxColour color)
{
	wxWindowDC dc(this);

	wxRect rc;
	rc.x = 0;
	rc.width = width;
	rc.y = rcPosTriangleTop.height;
	rc.height = rcPosTriangleBottom.y - rcPosTriangleTop.height;

	FillRect(&dc, rc, themeScroll.colorBack);

	rcPosBar.y += diffY;
	rcPosBar.height += diffY;
	
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

	DrawRectangleElement(&dc, color);
}

void CScrollbarVerticalWnd::OnMouseMove(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();
	wxWindowDC dc(this);
	SetIsMoving();

	if (captureBar)
	{
		scrollMoving = true;
		int diffY = yPos - yPositionStart;
		MoveBar(diffY, themeScroll.colorBarActif);
		yPositionStart = yPos;
	}
	else
	{

		if (FindTopTriangle(yPos, xPos))
			DrawTopTriangleElement(&dc, rcPosTriangleTop, themeScroll.colorTriangleActif);
		else if (FindBottomTriangle(yPos, xPos))
			DrawBottomTriangleElement(&dc, rcPosTriangleBottom, themeScroll.colorTriangleActif);
		else if (FindRectangleBar(yPos, xPos))
			DrawRectangleElement(&dc, themeScroll.colorBarActif);
		else
		{
			DrawTopTriangleElement(&dc, rcPosTriangleTop, themeScroll.colorTriangle);
			DrawBottomTriangleElement(&dc, rcPosTriangleBottom, themeScroll.colorTriangle);
			DrawRectangleElement(&dc, themeScroll.colorBar);
		}
	}
}

bool CScrollbarVerticalWnd::TestMaxY()
{
	int diff = pictureHeight - screenHeight;
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
	MoveBar(-1, themeScroll.colorBar);
	currentYPos -= lineSize;
	TestMinY();
	scrollInterface->SetTopPosition(currentYPos);
}

void CScrollbarVerticalWnd::ClickBottomTriangle()
{
	//Click Bottom Triangle
	MoveBar(1, themeScroll.colorBar);
	currentYPos += lineSize;
	TestMaxY();
	scrollInterface->SetTopPosition(currentYPos);
}

void CScrollbarVerticalWnd::ClickTopPage()
{
	int sizeMove = pageSize / lineSize;
	MoveBar(-sizeMove, themeScroll.colorBar);
	currentYPos -= pageSize;
	TestMinY();
	scrollInterface->SetTopPosition(currentYPos);
}

void CScrollbarVerticalWnd::ClickBottomPage()
{
	int sizeMove = pageSize / lineSize;
	MoveBar(sizeMove, themeScroll.colorBar);
	currentYPos += pageSize;
	TestMaxY();
	scrollInterface->SetTopPosition(currentYPos);
}


void CScrollbarVerticalWnd::OnLButtonDown(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();
	//bool initTimer = false;

	if (FindTopTriangle(yPos, xPos))
	{
		scrollMoving = true;
		//initTimer = true;
		ClickTopTriangle();
		triangleTop->Start(100);
	}
	else if (FindBottomTriangle(yPos, xPos))
	{
		SetIsMoving();
		//initTimer = true;
		ClickBottomTriangle();
		triangleBottom->Start(100);
	}
	else if (FindRectangleBar(yPos, xPos))
	{
		SetIsMoving();
		yPositionStartMove = yPositionStart = yPos;
		CaptureMouse();
		captureBar = true;	
	}
	else if (yPos > rcPosBar.height)
	{
		SetIsMoving();
		//initTimer = true;
		ClickBottomPage();
		pageBottom->Start(100);
	}
	else if (yPos < rcPosBar.y)
	{
		//initTimer = true;
		ClickTopPage();
		pageTop->Start(100);
		SetIsMoving();
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
        if(HasCapture())
            ReleaseMouse();
		captureBar = false;
		int diff = yPos - yPositionStartMove;

		currentYPos += diff * lineSize;
		TestMinY();
		TestMaxY();
		scrollInterface->SetTopPosition(currentYPos);
	}
		
	if (triangleTop->IsRunning())
		triangleTop->Stop();
	if (triangleBottom->IsRunning())
		triangleBottom->Stop();
	if (pageTop->IsRunning())
		pageTop->Stop();
	if (pageBottom->IsRunning())
		pageBottom->Stop();
}

void CScrollbarVerticalWnd::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = width;
	rc.height = height;

	FillRect(&dc, rc, themeScroll.colorBack);

	DrawTopTriangleElement(&dc, rcPosTriangleTop, themeScroll.colorTriangle);
	DrawBottomTriangleElement(&dc, rcPosTriangleBottom, themeScroll.colorTriangle);
	DrawRectangleElement(&dc, themeScroll.colorBar);
}

void CScrollbarVerticalWnd::FillRect(wxDC * dc, const wxRect &rc, const wxColour &color)
{
	wxBrush brush = wxBrush(color);
	dc->SetBrush(brush);
	dc->DrawRectangle(rc);
	dc->SetBrush(wxNullBrush);
}