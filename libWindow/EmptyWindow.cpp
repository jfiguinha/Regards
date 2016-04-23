#include "EmptyWindow.h"


CEmptyWindow::CEmptyWindow(wxWindow* parent, wxWindowID id) :
CWindowMain(parent, id)
{
	Connect(wxEVT_PAINT, wxPaintEventHandler(CEmptyWindow::OnPaint));
}


CEmptyWindow::~CEmptyWindow()
{

}

void CEmptyWindow::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = width;
	rc.height = height;
	FillRect(&dc, rc, wxColour(128, 128, 128));
}

void CEmptyWindow::Resize()
{
	this->Refresh();
}

