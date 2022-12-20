#include "header.h"
#include "EmptyWindow.h"


CEmptyWindow::CEmptyWindow(wxWindow* parent, wxWindowID id) :
	CWindowMain("CEmptyWindow", parent, id)
{
	Connect(wxEVT_PAINT, wxPaintEventHandler(CEmptyWindow::on_paint));
}


CEmptyWindow::~CEmptyWindow()
{
}

void CEmptyWindow::on_paint(wxPaintEvent& event)
{
	int width = GetWindowWidth();
	int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;


	wxPaintDC dc(this);
	wxRect rc = GetWindowRect();
	FillRect(&dc, rc, wxColour(128, 128, 128));
}
