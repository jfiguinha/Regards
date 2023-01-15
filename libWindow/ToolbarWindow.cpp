#include "header.h"
#include "ToolbarWindow.h"
#include <wx/dcbuffer.h>
using namespace Regards::Window;

#define TIMER_PUSHID 1


CToolbarWindow::CToolbarWindow(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical) :
	CWindowMain("CToolbarWindow", parent, id)
{
	isVertical = vertical;
	m_bMouseOver = false;
	m_bMouseMove = false;
	saveLastPush = false;
	m_bIconeOn = false;
	numButtonActif = -1;
	navPush = nullptr;
	pushButton = new wxTimer(this, TIMER_PUSHID);
	themeToolbar = theme;
	Connect(wxEVT_PAINT, wxPaintEventHandler(CToolbarWindow::on_paint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CToolbarWindow::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CToolbarWindow::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CToolbarWindow::OnLButtonUp));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CToolbarWindow::OnMouseHover));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CToolbarWindow::OnMouseLeave));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CToolbarWindow::OnMouseCaptureLost));
	Connect(TIMER_PUSHID, wxEVT_TIMER, wxTimerEventHandler(CToolbarWindow::OnTimerPushButton), nullptr, this);
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(CToolbarWindow::OnEraseBackground));
}


void CToolbarWindow::OnMouseCaptureLost(wxMouseEvent& event)
{
}


int CToolbarWindow::GetHeight()
{
	return themeToolbar.GetHeight();
}

int CToolbarWindow::GetWidth()
{
	return themeToolbar.GetWidth();
}

CToolbarWindow::~CToolbarWindow()
{
	if (pushButton->IsRunning())
	{
		pushButton->Stop();
	}
	EmptyNavigator();
	delete(pushButton);
}


void CToolbarWindow::OnTimerPushButton(wxTimerEvent& event)
{
	if (navPush != nullptr)
	{
		EventManager(navPush->GetCommandId());
	}
	//pushButton->Stop();
}

void CToolbarWindow::EmptyNavigator()
{
	for (CToolbarElement* nav : navElement)
	{
		delete(nav);
	}

	navElement.clear();
}

void CToolbarWindow::UpdateScreenRatio()
{
	needToRefresh = true;
}

int CToolbarWindow::GetNavigatorHeight()
{
	int size = 0;
	if (isVertical)
	{
		for (CToolbarElement* nav : navElement)
		{
			if (nav->IsVisible())
				size += themeToolbar.GetMargeX() + nav->GetHeight();
		}
	}
	else
	{
		for (CToolbarElement* nav : navElement)
		{
			if (nav->IsVisible())
				if (size < nav->GetHeight())
					size = nav->GetHeight();
		}
	}
	return size;
}

int CToolbarWindow::GetNavigatorWidth()
{
	int size = 0;
	if (isVertical)
	{
		for (CToolbarElement* nav : navElement)
		{
			if (nav->IsVisible())
				if (size < nav->GetWidth())
					size = nav->GetWidth();
		}
	}
	else
	{
		for (CToolbarElement* nav : navElement)
		{
			if (nav->IsVisible())
				size += themeToolbar.GetMargeX() + nav->GetWidth();
		}
	}
	return size;
}

void CToolbarWindow::OnMouseMove(wxMouseEvent& event)
{
	bool needToRedraw = false;
	int xPos = event.GetX();
	int yPos = event.GetY();
	wxClientDC dc(this);
	int i = 0;

	for (CToolbarElement* nav : navElement)
	{
		if (nav != nullptr)
		{
			if (nav->IsVisible())
			{
				if (nav->FindElement(xPos, yPos))
				{
					if (nav->MouseOver(&dc, xPos, yPos))
						needToRedraw = true;

					if (nav->SetActif())
						needToRedraw = true;

					//findActif = true;
					if (numButtonActif != i)
					{
						this->SetToolTip(nav->GetLibelleTooltip());
						numButtonActif = i;
					}
				}
				else
				{
					if (nav->SetInactif())
						needToRedraw = true;
				}
			}
		}
		i++;
	}

	if (needToRedraw)
		needToRefresh = true;
}


void CToolbarWindow::OnLButtonUp(wxMouseEvent& event)
{
	wxClientDC dc(this);
	int xPos = event.GetX();
	int yPos = event.GetY();

	for (CToolbarElement* nav : navElement)
	{
		if (nav->IsVisible())
		{
			if (nav->FindElement(xPos, yPos))
			{
				nav->UnclickElement(this, xPos, yPos);
				if (!saveLastPush)
					nav->SetPush(false);
				break;
			}
		}
	}

	if (pushButton != nullptr)
	{
		if (pushButton->IsRunning())
			pushButton->Stop();
	}
	if (navPush != nullptr)
	{
		if (navPush->GetRepeatable())
		{
			EventManager(navPush->GetCommandId());
			navPush = nullptr;
		}
	}

	needToRefresh = true;
}

void CToolbarWindow::RedrawElement(wxDC* dc, CToolbarElement* nav)
{
	DrawButton(dc, nav);
}

void CToolbarWindow::DrawButton(wxDC* dc, CToolbarElement* nav)
{
	wxBitmap pictureBuffer(nav->GetWidth(), nav->GetHeight());
	wxMemoryDC memDC(pictureBuffer);

	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = nav->GetWidth();
	rc.height = nav->GetHeight();
	DrawBackground(&memDC, rc);

	nav->DrawButton(&memDC, 0, 0);
	memDC.SelectObject(wxNullBitmap);

	if (pictureBuffer.IsOk())
	{
		dc->DrawBitmap(pictureBuffer, nav->GetXPos(), nav->GetYPos());
	}
}

void CToolbarWindow::OnLButtonDown(wxMouseEvent& event)
{
	wxClientDC dc(this);
	this->SetFocus();
	int xPos = event.GetX();
	int yPos = event.GetY();
	bool repeatable = false;

	for (CToolbarElement* nav : navElement)
	{
		if (nav->IsVisible())
		{
			if (nav->FindElement(xPos, yPos))
			{
				nav->ClickElement(this, xPos, yPos);
				nav->SetPush(true);
				RedrawElement(&dc, nav);
				navPush = nav;

				if (navPush->GetRepeatable())
				{
					if (pushButton->IsRunning())
						pushButton->Stop();

					pushButton->Start(500);

					repeatable = true;
				}
			}
			else
			{
				nav->SetPush(false);
				RedrawElement(&dc, nav);
			}
		}
	}

	if (!repeatable)
		if (navPush != nullptr)
			EventManager(navPush->GetCommandId());

	//Refresh();
}

void CToolbarWindow::OnMouseLeave(wxMouseEvent& event)
{
	wxClientDC dc(this);

	//if (pushButton->IsRunning())
	//	pushButton->Stop();

	m_bMouseOver = false;
	if (HasCapture())
		ReleaseMouse();

	for (CToolbarElement* nav : navElement)

	{
		if (nav->SetInactif())
			RedrawElement(&dc, nav);
	}
}

void CToolbarWindow::OnMouseHover(wxMouseEvent& event)
{
	wxSetCursor(wxCursor(wxCURSOR_ARROW));
	m_bMouseOver = true;
}

bool CToolbarWindow::IsMouseOver()
{
	return m_bMouseOver;
}

void CToolbarWindow::DrawBackground(wxDC* deviceContext, const wxRect& rc)
{
	if (isVertical)
		deviceContext->GradientFillLinear(rc, themeToolbar.colorTop, themeToolbar.colorBottom);
	else
		deviceContext->GradientFillLinear(rc, themeToolbar.colorTop, themeToolbar.colorBottom, wxNORTH);
}


void CToolbarWindow::DrawBackground(wxDC* deviceContext)
{
	if (GetWindowWidth() > 0 && GetWindowHeight() > 0)
	{
		background.Create(GetWindowWidth(), GetWindowHeight());
		wxMemoryDC memDC(background);
		wxRect rc = GetWindowRect();
		DrawBackground(&memDC, rc);
		//CWindowMain::FillRect(&memDC, rc, themeToolbar.colorTop);
		memDC.SelectObject(wxNullBitmap);

		backPicture = background.ConvertToImage();
		deviceContext->DrawBitmap(background, 0, 0);
	}
}

void CToolbarWindow::GenerateNavigatorButton(wxDC* deviceContext)
{
	for (CToolbarElement* nav : navElement)
	{
		if (nav->IsVisible())
		{
			DrawButton(deviceContext, nav);
		}
	}
}

void CToolbarWindow::on_paint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);

	DrawBackground(&dc);

	int navigatorWidth = GetNavigatorWidth();
	int navigatorHeight = GetNavigatorHeight();

	if (isVertical)
	{
		int xStart = (GetWindowWidth() - navigatorWidth) / 2;
		int yStart = 0;

		switch (themeToolbar.position)
		{
		case NAVIGATOR_LEFT:
			yStart = 0;
			break;
		case NAVIGATOR_CENTER:
			yStart = (GetWindowHeight() - navigatorHeight) / 2;
			break;
		case NAVIGATOR_RIGHT:
			yStart = GetWindowHeight() - navigatorHeight;
			break;
		default: ;
		}

		for (CToolbarElement* nav : navElement)
		{
			if (nav->IsVisible())
			{
				//nav->Resize(navigatorWidth, nav->GetHeight());
				// nav->SetBackgroundBitmap(backPicture);
				nav->SetPosition(xStart, yStart);
				yStart += themeToolbar.GetMargeY() + nav->GetHeight();
			}
		}
	}
	else
	{
		int xStart = 0;
		int yStart = (GetWindowHeight() - navigatorHeight) / 2;

		switch (themeToolbar.position)
		{
		case NAVIGATOR_LEFT:
			xStart = 0;
			break;
		case NAVIGATOR_CENTER:
			xStart = (GetWindowWidth() - navigatorWidth) / 2;
			break;
		case NAVIGATOR_RIGHT:
			xStart = GetWindowWidth() - navigatorWidth;
			break;
		default: ;
		}

		for (CToolbarElement* nav : navElement)
		{
			if (nav->IsVisible())
			{
				//nav->Resize(nav->GetWidth(), navigatorHeight);
				// nav->SetBackgroundBitmap(backPicture);
				nav->SetPosition(xStart, yStart);
				xStart += themeToolbar.GetMargeX() + nav->GetWidth();
			}
		}
	}

	GenerateNavigatorButton(&dc);
}
