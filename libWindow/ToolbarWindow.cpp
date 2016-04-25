#include "ToolbarWindow.h"
#include <wx/dcbuffer.h>
using namespace Regards::Window;

#define TIMER_PUSHID 1

CToolbarWindow::CToolbarWindow(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme) :
CWindowMain(parent, id)
{
	m_bMouseMove = false;
	m_bIconeOn = false;
	numButtonActif = -1;
	navPush = nullptr;
	pushButton = new wxTimer(this, TIMER_PUSHID);
	themeToolbar = theme;
	Connect(wxEVT_PAINT, wxPaintEventHandler(CToolbarWindow::OnPaint));
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
	for (CToolbarElement * nav : navElement)
	{
		delete(nav);
	}

	navElement.clear();

}

void CToolbarWindow::UpdateScreenRatio()
{
    this->Refresh();
}

int CToolbarWindow::GetNavigatorHeight()
{
	int size = 0;
	if (themeToolbar.isVertical)
	{
		for (CToolbarElement * nav : navElement)
		{
			if (nav->IsVisible())
				size += themeToolbar.GetMargeX() + nav->GetHeight();
		}
	}
	else
	{
		for (CToolbarElement * nav : navElement)
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
	if (themeToolbar.isVertical)
	{
		for (CToolbarElement * nav : navElement)
		{

			if (nav->IsVisible())
				if (size < nav->GetWidth())
					size = nav->GetWidth();
		}
	}
	else
	{
		for (CToolbarElement * nav : navElement)
		{
			if (nav->IsVisible())
				size += themeToolbar.GetMargeX()  + nav->GetWidth();
		}
	}
	return size;
}

void CToolbarWindow::Resize()
{
	this->Refresh();
}

void CToolbarWindow::OnMouseMove(wxMouseEvent& event)
{
#ifdef WIN32
	wxWindowDC dc(this);
	int xPos = event.GetX();
	int yPos = event.GetY();
	//bool findActif = false;
	int i = 0;
    
    //printf("CToolbarWindow::OnMouseMove x : %d y : %d \n", xPos, yPos);

	for (CToolbarElement * nav : navElement)
	{
		if (nav != nullptr)
		{
			if (nav->IsVisible())
			{
				if (nav->FindElement(xPos, yPos))
				{
					if(nav->MouseOver(&dc, xPos, yPos))
						RedrawElement(&dc, nav);

					if(nav->SetActif())
						RedrawElement(&dc, nav);

					//findActif = true;
					if (numButtonActif != i)
					{
						this->SetToolTip(nav->GetLibelleTooltip().c_str());
						numButtonActif = i;

					}
				}
				else
				{
					if(nav->SetInactif())
						RedrawElement(&dc, nav);
				}
			}
		}
		i++;
	}
#else
    bool needToRedraw = false;
    int xPos = event.GetX();
    int yPos = event.GetY();
    wxWindowDC dc(this);
    int i = 0;
    
    //printf("CToolbarWindow::OnMouseMove x : %d y : %d \n", xPos, yPos);
    
    for (CToolbarElement * nav : navElement)
    {
        if (nav != nullptr)
        {
            if (nav->IsVisible())
            {
                if (nav->FindElement(xPos, yPos))
                {
                    if(nav->MouseOver(&dc, xPos, yPos))
                        needToRedraw = true;
                    
                    if(nav->SetActif())
                        needToRedraw = true;
                    
                    //findActif = true;
                    if (numButtonActif != i)
                    {
                        this->SetToolTip(nav->GetLibelleTooltip().c_str());
                        numButtonActif = i;
                        
                    }
                }
                else
                {
                    if(nav->SetInactif())
                        needToRedraw = true;
                }
            }
        }
        i++;
    }
    
    if(needToRedraw)
        this->Refresh();
#endif
    
}


void CToolbarWindow::OnLButtonUp(wxMouseEvent& event)
{
	wxWindowDC dc(this);
	int xPos = event.GetX();
	int yPos = event.GetY();
	for (CToolbarElement * nav : navElement)
	{
		if (nav->IsVisible())
		{
			if (nav->FindElement(xPos, yPos))
			{
				nav->UnclickElement(this, xPos, yPos);
                if(!saveLastPush)
                    nav->SetPush(false);
				//RedrawElement(&dc, nav);
				break;
			}
		}
	}
	
	if (pushButton->IsRunning())
        pushButton->Stop();

	if(navPush != nullptr)
	{	
		if (navPush->GetRepeatable())
		{ 
			EventManager(navPush->GetCommandId());
			navPush = nullptr;
		}
    }
    this->Refresh();
}

void CToolbarWindow::RedrawElement(wxDC * dc, CToolbarElement * nav)
{
	wxBitmap bitmapBackground = wxBitmap(width, height);
	wxMemoryDC memDCbackground(bitmapBackground);
	wxRect rc;
	rc.y = 0;
	rc.height = height;
	rc.x = 0;
	rc.width = width;
	DrawBackground(&memDCbackground, rc);
	memDCbackground.SelectObject(wxNullBitmap);


	rc.x = nav->GetXPos();
	rc.y = nav->GetYPos();
	rc.width = nav->GetWidth();
	rc.height = nav->GetHeight();

	//Test if si valid sub bitmap;
	try
	{
		if (rc.x < 0)
		{
			rc.width = rc.width + rc.x;
			rc.x = 0;
		}

		if (rc.y < 0)
		{
			rc.height = rc.height + rc.y;
			rc.y = 0;
		}

		if (rc.x + rc.width > bitmapBackground.GetWidth())
			rc.width = bitmapBackground.GetWidth() - rc.x;

		if (rc.y + rc.height > bitmapBackground.GetHeight())
			rc.height = bitmapBackground.GetHeight() - rc.y;



		if (rc.width > 0 && rc.height > 0)
			dc->DrawBitmap(bitmapBackground.GetSubBitmap(rc), rc.x, rc.y);
	}
	catch (...)
	{

	}
	nav->DrawButton(dc);

}

void CToolbarWindow::OnLButtonDown(wxMouseEvent& event)
{
	wxWindowDC dc(this);
	this->SetFocus();
	int xPos = event.GetX();
	int yPos = event.GetY();
    bool repeatable = false;
	for (CToolbarElement * nav : navElement)
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
                        
                    pushButton->Start(250);
                    
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

    if(!repeatable)
        if (navPush != nullptr)
            EventManager(navPush->GetCommandId());
    
    //this->Refresh();
}

void CToolbarWindow::OnMouseLeave(wxMouseEvent& event)
{
	wxWindowDC dc(this);

	//if (pushButton->IsRunning())
	//	pushButton->Stop();

	m_bMouseOver = false;

	for (CToolbarElement * nav : navElement)
	{
		if(nav->SetInactif())
			RedrawElement(&dc, nav);
	}
}

void CToolbarWindow::OnMouseHover(wxMouseEvent& event)
{
	m_bMouseOver = true;
}

bool CToolbarWindow::IsMouseOver()
{
	return m_bMouseOver;
}

void CToolbarWindow::DrawBackground(wxDC * deviceContext, const wxRect &rc)
{
	if (themeToolbar.isVertical)
		deviceContext->GradientFillLinear(rc, themeToolbar.colorTop, themeToolbar.colorBottom);
	else
		deviceContext->GradientFillLinear(rc, themeToolbar.colorTop, themeToolbar.colorBottom, wxNORTH);
}


void CToolbarWindow::DrawBackground(wxDC * deviceContext)
{
	if(width > 0 && height > 0)
	{
		background.Create(width,height);
		wxMemoryDC memDC(background);
		wxRect rc;
		rc.y = 0;
		rc.height = height;
		rc.x = 0;
		rc.width = width;
		DrawBackground(&memDC, rc);
		memDC.SelectObject(wxNullBitmap);
		
		backPicture = background.ConvertToImage();
		
		deviceContext->DrawBitmap(background, 0, 0);
	
	}
}


void CToolbarWindow::GenerateNavigatorButton(wxDC * deviceContext)
{
	for (CToolbarElement * nav : navElement)
	{
		if (nav->IsVisible())
		{
			nav->DrawButton(deviceContext);
		}
	}
}

void CToolbarWindow::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);

	DrawBackground(&dc);

	int navigatorWidth = GetNavigatorWidth();
	int navigatorHeight = GetNavigatorHeight();

	if (themeToolbar.isVertical)
	{
		int xStart = (width - navigatorWidth) / 2;
		int yStart = 0;

		switch (themeToolbar.position)
		{
		case NAVIGATOR_LEFT:
			yStart = 0;
			break;
		case NAVIGATOR_CENTER:
			yStart = (height - navigatorHeight) / 2;
			break;
		case NAVIGATOR_RIGHT:
			yStart = height - navigatorHeight;
			break;
		}

		for (CToolbarElement * nav : navElement)
		{
			if (nav->IsVisible())
			{
                //nav->Resize(navigatorWidth, nav->GetHeight());
                nav->SetBackgroundBitmap(backPicture);
				nav->SetPosition(xStart, yStart);
				yStart += themeToolbar.GetMargeY()  + nav->GetHeight();
			}
		}
	}
	else
	{
		int xStart = 0;
		int yStart = (height - navigatorHeight) / 2;

		switch (themeToolbar.position)
		{
		case NAVIGATOR_LEFT:
			xStart = 0;
			break;
		case NAVIGATOR_CENTER:
			xStart = (width - navigatorWidth) / 2;
			break;
		case NAVIGATOR_RIGHT:
			xStart = width - navigatorWidth;
			break;
		}

		for (CToolbarElement * nav : navElement)
		{
			if (nav->IsVisible())
			{
                //nav->Resize(nav->GetWidth(), navigatorHeight);
                nav->SetBackgroundBitmap(backPicture);
				nav->SetPosition(xStart, yStart);
				xStart += themeToolbar.GetMargeX()  + nav->GetWidth();
			}
		}
	}

	GenerateNavigatorButton(&dc);
}
