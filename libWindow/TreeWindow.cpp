#include "header.h"
#include "TreeWindow.h"

#include <Tracing.h>

#include "ScrollbarWnd.h"
#include <wx/dcbuffer.h>
#include "TreeControl.h"
using namespace Regards::Window;


CTreeWindow::CTreeWindow(wxWindow* parent, wxWindowID id, const CThemeTree& theme)
	: CWindowMain("CTreeWindow", parent, id)
{
	treeControl = nullptr;
	renderBitmap = nullptr;
	bitmapWidth = 0;
	bitmapHeight = 0;
	oldPosLargeur = 0;
	oldPosHauteur = 0;
	controlWidth = 0;
	controlHeight = 0;
	//defaultPageSize = 50;
	//defaultLineSize = 5;
	posHauteur = 0;
	posLargeur = 0;
	themeTree = theme;
	//defaultPageSize = 50;

	//Buffer
	/*
	controlWidthBuffer = 0;
	controlHeightBuffer = 0;
	posLargeurBuffer = 0;
	posHauteurBuffer = 0;         
	widthBuffer = 0;
	heightBuffer = 0;     
	*/
	//defaultLineSize = themeTree.GetRowHeight();
	themeTree.themeTriangle.SetHeight(themeTree.GetRowHeight());
	themeTree.themeCheckbox.SetHeight(themeTree.GetRowHeight());
	themeTree.themeDelete.SetHeight(themeTree.GetRowHeight());
	themeTree.themeSlide.SetHeight(themeTree.GetRowHeight());
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	Connect(wxEVT_PAINT, wxPaintEventHandler(CTreeWindow::on_paint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CTreeWindow::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CTreeWindow::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CTreeWindow::OnLButtonUp));
	Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(CTreeWindow::OnLDoubleClick));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CTreeWindow::OnMouseWheel));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CTreeWindow::OnKeyDown));
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(CTreeWindow::OnEraseBackground));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CTreeWindow::OnMouseCaptureLost));

	Connect(wxEVENT_SCROLLMOVE, wxCommandEventHandler(CTreeWindow::OnScrollMove));
	Connect(wxEVENT_LEFTPOSITION, wxCommandEventHandler(CTreeWindow::OnLeftPosition));
	Connect(wxEVENT_TOPPOSITION, wxCommandEventHandler(CTreeWindow::OnTopPosition));
	Connect(wxEVENT_MOVELEFT, wxCommandEventHandler(CTreeWindow::OnMoveLeft));
	Connect(wxEVENT_MOVERIGHT, wxCommandEventHandler(CTreeWindow::OnMoveRight));
	Connect(wxEVENT_MOVETOP, wxCommandEventHandler(CTreeWindow::OnMoveTop));
	Connect(wxEVENT_MOVEBOTTOM, wxCommandEventHandler(CTreeWindow::OnMoveBottom));
}


void CTreeWindow::OnMoveLeft(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posLargeur = pos;
	Resize();
}

void CTreeWindow::OnMoveRight(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posLargeur = pos;
	Resize();
}

void CTreeWindow::OnMoveTop(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
	Resize();
}

void CTreeWindow::OnMoveBottom(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
	Resize();
}

void CTreeWindow::OnLeftPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posLargeur = pos;
	Resize();
}

void CTreeWindow::OnTopPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
	Resize();
}

void CTreeWindow::OnScrollMove(wxCommandEvent& event)
{
	isMoving = event.GetInt();
}

void CTreeWindow::MoveTop()
{
	wxWindow* parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVETOP);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CTreeWindow::MoveLeft()
{
	wxWindow* parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVELEFT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CTreeWindow::MoveBottom()
{
	wxWindow* parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVEBOTTOM);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CTreeWindow::MoveRight()
{
	wxWindow* parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVERIGHT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

/*
void CTreeWindow::UpdateElement(CTreeElement * treeElement)
{
    TRACE();
	wxClientDC dc(this);
	int xPos = 0;
	if (treeElement->GetRow() > 0)
		xPos = treeControl->GetWidthRow(treeElement->GetRow() - 1);
	

	//treeElement->DrawElement(&dc, treeElement->GetPosX() + xPos, treeElement->GetPosY());
    
	//GenerateScreenBuffer();
	//this->Refresh();
}
*/
CTreeWindow::~CTreeWindow()
{
	TRACE();
}

int CTreeWindow::GetWidth()
{
	TRACE();
	return controlWidth;
}

int CTreeWindow::GetHeight()
{
	TRACE();
	return controlHeight;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CTreeWindow::TestMaxX()
{
	TRACE();
	long xValue = GetWidth() - GetWindowWidth();

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
	TRACE();
	long yValue = GetHeight() - GetWindowHeight();

	if (posHauteur >= yValue)
		posHauteur = yValue;

	if (posHauteur < 0)
		posHauteur = 0;
}


void CTreeWindow::OnKeyDown(wxKeyEvent& event)
{
	TRACE();
	bool update = false;


	switch (event.m_keyCode)
	{
	case WXK_UP:
		this->MoveTop();
		update = true;
		break;
	case WXK_LEFT:
		this->MoveLeft();
		update = true;
		break;
	case WXK_DOWN:
		this->MoveBottom();
		update = true;
		break;
	case WXK_RIGHT:
		this->MoveRight();
		update = true;
		break;
	default: ;
	}

	if (update)
	{
		GenerateScreenBuffer();
		needToRefresh = true;
	}
}

void CTreeWindow::OnMouseWheel(wxMouseEvent& event)
{
	TRACE();

	bool update;
    
    if (event.GetWheelRotation() > 0)
        this->MoveTop();
    else
        this->MoveBottom();
    update = true; 

	if (update)
	{
		GenerateScreenBuffer();
		needToRefresh = true;
	}
}

void CTreeWindow::DrawBackgroundRectangle(wxDC* deviceContext, const int& y, const int& rowHeight,
                                          const wxColour& color)
{
	TRACE();
	wxRect rc;
	rc.x = 0;
	rc.width = GetWindowWidth();
	rc.y = y;
	rc.height = y + rowHeight; //themeTree.GetRowHeight();
	if (deviceContext != nullptr)
		deviceContext->GradientFillLinear(rc, color, color);
}

void CTreeWindow::CalculControlSize()
{
	TRACE();
	if (treeControl != nullptr)
	{
		controlWidth = treeControl->GetWidth();
		controlHeight = treeControl->GetNbRow() * themeTree.GetRowHeight();

	}
}

wxColour CTreeWindow::GetBackgroundColour(const int& yPos)
{
	TRACE();
	int moduloValue = yPos % (themeTree.GetRowHeight() * 2);
	if (moduloValue < themeTree.GetRowHeight())
		return themeTree.bgColorOne;

	return themeTree.bgColorTwo;
}

void CTreeWindow::GenerateBackgroundBitmap(wxDC* deviceContext, const int& posLargeur, const int& posHauteur)
{
	TRACE();
	int yMaxPos = posHauteur; //-(posHauteur % themeTree.GetRowHeight());
	//bool style = true;
	int diff = themeTree.GetRowHeight() - posHauteur % themeTree.GetRowHeight();


	if (yMaxPos < (GetWindowHeight() + posHauteur))
	{
		while (yMaxPos < (GetWindowHeight() + posHauteur))
		{
			DrawBackgroundRectangle(deviceContext, yMaxPos - posHauteur, diff, GetBackgroundColour(yMaxPos));
			yMaxPos += diff;
			diff = themeTree.GetRowHeight();
		}
	}
}

void CTreeWindow::Resize()
{
	TRACE();
	GenerateScreenBuffer();
	needToRefresh = true;
	/*
	wxClientDC dc(this);
	if (backgroundBuffer.IsOk())
	{

		printf("CTreeWindow::OnPaint not bufferUpdate \n");
		dc.DrawBitmap(backgroundBuffer, 0, 0);
	}
	*/
}

void CTreeWindow::OnMouseMove(wxMouseEvent& event)
{
	TRACE();
	if (treeControl == nullptr)
		return;

	int xPos = event.GetX();
	int yPos = event.GetY();

	wxClientDC dc(this);
	try
	{
		bool update = false;
		CPositionElement* element = treeControl->FindElement(xPos + posLargeur, yPos + posHauteur);
		if (element != nullptr)
		{
			treeControl->MouseOver(&dc, element, xPos, yPos, posLargeur, posHauteur, update);
		}
		else
		{
			treeControl->MouseOut(&dc, element, xPos, yPos, posLargeur, posHauteur, update);
			wxSetCursor(wxCursor(*wxSTANDARD_CURSOR));
		}
		if (update)
		{
			//GenerateScreenBuffer();
			//this->Refresh();
		}
	}
	catch (...)
	{
	}
	this->GetParent()->GetEventHandler()->ProcessEvent(event);
}

void CTreeWindow::OnLButtonUp(wxMouseEvent& event)
{
	TRACE();

	if (treeControl == nullptr)
		return;

	int xPos = event.GetX();
	int yPos = event.GetY();

	CPositionElement* element = treeControl->FindElement(xPos + posLargeur, yPos + posHauteur);
	if (element != nullptr)
	{
		treeControl->UnclickOnElement(element, this, xPos, yPos, posLargeur, posHauteur);
		GenerateScreenBuffer();
		needToRefresh = true;
	}
	else
	{
		//SetCursor(IDC_ARROW);
		wxSetCursor(wxCursor(*wxSTANDARD_CURSOR));
	}
}

void CTreeWindow::OnLButtonDown(wxMouseEvent& event)
{
	TRACE();

	if (treeControl == nullptr)
		return;

	SetFocus();

	int xPos = event.GetX();
	int yPos = event.GetY();

	CPositionElement* element = treeControl->FindElement(xPos + posLargeur, yPos + posHauteur);
	if (element != nullptr)
	{
		treeControl->ClickOnElement(element, this, xPos, yPos, posLargeur, posHauteur);
		GenerateScreenBuffer();
		needToRefresh = true;
	}
	else
	{
		//SetCursor(IDC_ARROW);
		wxSetCursor(wxCursor(*wxSTANDARD_CURSOR));
	}
}

void CTreeWindow::OnLDoubleClick(wxMouseEvent& event)
{
	TRACE();

	if (treeControl == nullptr)
		return;

	int xPos = event.GetX();
	int yPos = event.GetY();

	CPositionElement* element = treeControl->FindElement(xPos + posLargeur, yPos + posHauteur);
	if (element != nullptr)
	{
		treeControl->DoubleClickOnElement(element);
		GenerateScreenBuffer();
		needToRefresh = true;
	}
	else
	{
		wxSetCursor(wxCursor(*wxSTANDARD_CURSOR));
	}
}

void CTreeWindow::UpdateTreeControl()
{
	TRACE();
	printf("CTreeWindow::UpdateTreeControl \n");
	GenerateScreenBuffer();
	needToRefresh = true;
}

void CTreeWindow::UpdateScreenRatio()
{
	TRACE();
	printf("CTreeWindow::UpdateScreenRatio \n");
	//bufferUpdate = true;
	if (this->treeControl != nullptr)
		this->treeControl->UpdateScreenRatio();
	GenerateScreenBuffer();
	needToRefresh = true;
}


void CTreeWindow::SetTreeControl(CTreeControl* treeControl)
{
	TRACE();
	printf("CTreeWindow::SetTreeControl \n");
	this->treeControl = treeControl;
	GenerateScreenBuffer();
	needToRefresh = true;
}

void CTreeWindow::GenerateScreenBuffer()
{
	TRACE();
	int width = GetWindowWidth();
	int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;

	CalculControlSize();

	if (controlWidth < GetWindowWidth())
	{
		controlWidth = GetWindowWidth();
		posLargeur = 0;
	}

	if (controlHeight < GetWindowHeight())
	{
		controlHeight = GetWindowHeight();
		posHauteur = 0;
	}

	if ((posLargeur + GetWindowWidth()) > controlWidth)
	{
		posLargeur = controlWidth - GetWindowWidth();
	}

	if ((posHauteur + GetWindowHeight()) > controlHeight)
	{
		posHauteur = controlHeight - GetWindowHeight();
	}

	//printf("CTreeWindow::OnPaint bufferUpdate \n");

	auto background = wxBitmap(width, height);

	wxMemoryDC memDC(background);

	GenerateBackgroundBitmap(&memDC, posLargeur, posHauteur);
	if (treeControl != nullptr)
		treeControl->GenerateWindowBitmap(&memDC, width, height, posLargeur, posHauteur);

	if (treeControl != nullptr)
		treeControl->AfterDrawBitmap();

	memDC.SelectObject(wxNullBitmap);

	backgroundBuffer = background;

	oldPosLargeur = posLargeur;
	oldPosHauteur = posHauteur;


	wxWindow* parent = this->GetParent();

	if (parent != nullptr)
	{
		auto controlSize = new CControlSize();
		wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
		controlSize->controlWidth = controlWidth;
		controlSize->controlHeight = controlHeight;
		evt.SetClientData(controlSize);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}

	if (parent != nullptr)
	{
		auto size = new wxSize();
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		size->x = posLargeur;
		size->y = posHauteur;
		evt.SetClientData(size);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CTreeWindow::on_paint(wxPaintEvent& event)
{
	//printf("CTreeWindow::OnPaint \n");
	wxPaintDC dc(this);
	if (backgroundBuffer.IsOk())
	{
		//printf("CTreeWindow::OnPaint not bufferUpdate \n");
		dc.DrawBitmap(backgroundBuffer, 0, 0);
	}
}
