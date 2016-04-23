#include "TreeWindow.h"
#include <algorithm>    // std::sort
#include "ScrollbarWnd.h"
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include <wx/dcbuffer.h>
using namespace Regards::Window;

CTreeWindow::CTreeWindow(wxWindow* parent, wxWindowID id, const CThemeTree & theme)
	: CWindowMain(parent, id)
{
	controlWidth = 0;
	controlHeight = 0;
	defaultPageSize = 50;
	defaultLineSize = 5;
	posHauteur = 0;
	posLargeur = 0;
	themeTree = theme;
	defaultPageSize = 50;
	defaultLineSize = themeTree.GetRowHeight();
	themeTree.themeTriangle.SetHeight(themeTree.GetRowHeight());
	themeTree.themeCheckbox.SetHeight(themeTree.GetRowHeight());
	themeTree.themeDelete.SetHeight(themeTree.GetRowHeight());
	themeTree.themeSlide.SetHeight(themeTree.GetRowHeight());
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	Connect(wxEVT_PAINT, wxPaintEventHandler(CTreeWindow::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CTreeWindow::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CTreeWindow::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CTreeWindow::OnLButtonUp));
	Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(CTreeWindow::OnLDoubleClick));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CTreeWindow::OnMouseWheel));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CTreeWindow::OnKeyDown));
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(CTreeWindow::OnEraseBackground));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CTreeWindow::OnMouseCaptureLost));
}

void CTreeWindow::UpdateElement(CTreeElement * treeElement)
{
	wxWindowDC dc(this);
	int xPos = 0;
	if (treeElement->GetRow() > 0)
		xPos = treeControl->GetWidthRow(treeElement->GetRow() - 1);
	

	treeElement->DrawElement(&dc, treeElement->GetPosX() + xPos, treeElement->GetPosY());
}

CTreeWindow::~CTreeWindow()
{
}

int CTreeWindow::GetWidth()
{
	return controlWidth;
}

int CTreeWindow::GetHeight()
{
	return controlHeight;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CTreeWindow::TestMaxX()
{
	long xValue = GetWidth() - width;

	if (posLargeur >= xValue)
		posLargeur = xValue;

	if (posLargeur < 0)
		posLargeur = 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CTreeWindow::TestMaxY()
{
	long yValue = GetHeight() - height;

	if (posHauteur >= yValue)
		posHauteur = yValue;

	if (posHauteur < 0)
		posHauteur = 0;
}


void CTreeWindow::OnKeyDown(wxKeyEvent& event)
{
	m_lock.lock();
	{
		switch (event.m_keyCode)
		{
		case WXK_UP:
			this->MoveTop();
			break;
		case WXK_LEFT:
			this->MoveLeft();
			break;
		case WXK_DOWN:
			this->MoveBottom();
			break;
		case WXK_RIGHT:
			this->MoveRight();
			break;

		}
	}
	m_lock.unlock();
}

void CTreeWindow::OnMouseWheel(wxMouseEvent& event)
{	
	m_lock.lock();
#ifdef __APPLE__
    
    if (event.m_wheelRotation == 1)
        this->MoveTop();
    else if (event.m_wheelRotation == -1)
        this->MoveBottom();
    
#else
    
    if (event.m_wheelRotation == 120)
        this->MoveTop();
    else
        this->MoveBottom();
    
#endif
	m_lock.unlock();
}

void CTreeWindow::DrawBackgroundRectangle(wxDC * deviceContext, const int &y, const int &rowHeight, const wxColour &color)
{
	wxRect rc;
	rc.x = 0;
	rc.width = width;
	rc.y = y;
    rc.height = y + rowHeight;//themeTree.GetRowHeight();
	deviceContext->GradientFillLinear(rc, color, color);

}

void CTreeWindow::CalculControlSize()
{
	if (treeControl != nullptr)
	{
		controlWidth = treeControl->GetWidth();
		controlHeight = treeControl->GetNbRow() * themeTree.GetRowHeight();
		scrollbar->SetControlSize(controlWidth, controlHeight);
	}

}

wxColour CTreeWindow::GetBackgroundColour(const int &yPos)
{
    int moduloValue = yPos % (themeTree.GetRowHeight() * 2);
    if(moduloValue < themeTree.GetRowHeight())
        return themeTree.bgColorOne;

    return themeTree.bgColorTwo;
}

void CTreeWindow::GenerateBackgroundBitmap(wxDC * deviceContext, const int &posLargeur, const int &posHauteur)
{
    int yMaxPos = posHauteur;//-(posHauteur % themeTree.GetRowHeight());
	//bool style = true;
    int diff = themeTree.GetRowHeight() - posHauteur % themeTree.GetRowHeight();


	if (yMaxPos < (height + posHauteur))
	{
		while (yMaxPos < (height + posHauteur))
		{
			DrawBackgroundRectangle(deviceContext, yMaxPos - posHauteur, diff, GetBackgroundColour(yMaxPos));
			yMaxPos += diff;
            diff = themeTree.GetRowHeight();
		}
	}
}

void CTreeWindow::Resize()
{
	UpdateTreeControl();
}

void CTreeWindow::OnMouseMove(wxMouseEvent& event)
{
	if (treeControl == nullptr)
		return;

	int xPos = event.GetX();
	int yPos = event.GetY();

	m_lock.lock();
	{
		wxWindowDC dc(this);
		try
		{
			CPositionElement * element = treeControl->FindElement(xPos + posLargeur, yPos + posHauteur);
			if (element != nullptr)
			{
				treeControl->MouseOver(&dc, element, xPos, yPos, posLargeur, posHauteur);
			}
			else
			{
				treeControl->MouseOut(&dc, element, xPos, yPos, posLargeur, posHauteur);
				::wxSetCursor(wxCursor(*wxSTANDARD_CURSOR));
			}
		}
		catch (...)
		{

		}
	}
	m_lock.unlock();
}

void CTreeWindow::OnLButtonUp(wxMouseEvent& event)
{
	if (treeControl == nullptr)
		return;

	int xPos = event.GetX();
	int yPos = event.GetY();

	m_lock.lock();
	{
		CPositionElement * element = treeControl->FindElement(xPos + posLargeur, yPos + posHauteur);
		if (element != nullptr)
		{
			treeControl->UnclickOnElement(element, this, xPos, yPos, posLargeur, posHauteur);
		}
		else
		{
			//SetCursor(IDC_ARROW);
			::wxSetCursor(wxCursor(*wxSTANDARD_CURSOR));
		}
	}
	m_lock.unlock();
}

void CTreeWindow::OnLButtonDown(wxMouseEvent& event)
{
	if (treeControl == nullptr)
		return;

	SetFocus();

	int xPos = event.GetX();
	int yPos = event.GetY();

	m_lock.lock();
	{
		CPositionElement * element = treeControl->FindElement(xPos + posLargeur, yPos + posHauteur);
		if (element != nullptr)
		{
			treeControl->ClickOnElement(element, this, xPos, yPos, posLargeur, posHauteur);
		}
		else
		{
			//SetCursor(IDC_ARROW);
			::wxSetCursor(wxCursor(*wxSTANDARD_CURSOR));
		}
	}
	m_lock.unlock();
}

void CTreeWindow::OnLDoubleClick(wxMouseEvent& event)
{
	if (treeControl == nullptr)
		return;

	int xPos = event.GetX();
	int yPos = event.GetY();

	m_lock.lock();
	{
		CPositionElement * element = treeControl->FindElement(xPos + posLargeur, yPos + posHauteur);
		if (element != nullptr)
		{
			treeControl->DoubleClickOnElement(element);
		}
		else
		{
			::wxSetCursor(wxCursor(*wxSTANDARD_CURSOR));
		}
	}
	m_lock.unlock();
}

void CTreeWindow::UpdateTreeControl()
{
	CalculControlSize();
	this->Refresh();
}

void CTreeWindow::UpdateScreenRatio()
{
    UpdateTreeControl();
}


void CTreeWindow::SetTreeControl(CTreeControl * treeControl)
{
	this->treeControl = treeControl;
	CalculControlSize();
	this->Refresh();
}

void CTreeWindow::OnPaint(wxPaintEvent& event)
{
	m_lock.lock();

	CalculControlSize();

	wxPaintDC dc(this);
	
	if (controlWidth < width)
	{
		controlWidth = width;
		posLargeur = 0;
	}

	if (controlHeight < height)
	{
		controlHeight = height;
		posHauteur = 0;
	}

	if ((posLargeur + width) > controlWidth)
	{
		posLargeur = controlWidth - width;
	}

	if ((posHauteur + height) > controlHeight)
	{
		posHauteur = controlHeight - height;
	}

	scrollbar->SetControlSize(controlWidth, controlHeight);
	scrollbar->SetPosition(posLargeur, posHauteur);

	wxBitmap background = wxBitmap(width, height);

	wxMemoryDC memDC(background);

	GenerateBackgroundBitmap(&memDC, posLargeur, posHauteur);
	if (treeControl != nullptr)
		treeControl->GenerateWindowBitmap(&memDC, width, height, posLargeur, posHauteur);

	if (treeControl != nullptr)
		treeControl->AfterDrawBitmap();

	memDC.SelectObject(wxNullBitmap);

	dc.DrawBitmap(background, 0, 0);

	m_lock.unlock();
}
