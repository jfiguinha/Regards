#include "header.h"
#include "SeparationBar.h"
#include <wx/dcbuffer.h>
using namespace Regards::Window;

CSeparationBar::CSeparationBar(IMoveWindow* moveWindow, wxWindow* parent, wxWindowID id,
                               const CThemeSeparationBar& theme)
	: CWindowMain("CSeparationBar", parent, id)
{
	bSplitterMoving = false;
	horizontal = false;
	this->moveWindow = moveWindow;
	Connect(wxEVT_PAINT, wxPaintEventHandler(CSeparationBar::on_paint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CSeparationBar::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CSeparationBar::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CSeparationBar::OnLButtonUp));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CSeparationBar::OnMouseHover));



	this->theme = theme;
}

void CSeparationBar::UpdateScreenRatio()
{
	Resize();
}

void CSeparationBar::SetHorizontal(const bool& horizontal)
{
	this->horizontal = horizontal;
}


void CSeparationBar::OnMouseHover(wxMouseEvent& event)
{
	wxSetCursor(wxCursor(horizontal ? wxCURSOR_SIZENS : wxCURSOR_SIZEWE));
}

void CSeparationBar::OnMouseMove(wxMouseEvent& event)
{
	if (horizontal)
	{
		if (bSplitterMoving)
			moveWindow->SetNewPosition(this);

		wxSetCursor(wxCursor(wxCURSOR_SIZENS));
	}
	else
	{
		if (bSplitterMoving)
			moveWindow->SetNewPosition(this);

		wxSetCursor(wxCursor(wxCURSOR_SIZEWE));
	}
}

void CSeparationBar::OnLButtonDown(wxMouseEvent& event)
{
	if (moveWindow->OnLButtonDown())
	{
		if (horizontal)
			wxSetCursor(wxCursor(wxCURSOR_SIZENS));
		else
			wxSetCursor(wxCursor(wxCURSOR_SIZEWE));

		bSplitterMoving = true;
		CaptureMouse();
	}
}


void CSeparationBar::on_paint(wxPaintEvent& event)
{
	wxSize size = GetClientSize();
	if (size.x <= 0 || size.y <= 0)
		return;

	wxBufferedPaintDC dc(this);
	wxRect rc = GetWindowRect();
	dc.GradientFillLinear(rc, theme.secondColor, theme.firstColor, horizontal ? wxDirection::wxSOUTH : wxEAST);
}

void CSeparationBar::OnLButtonUp(wxMouseEvent& event)
{
	moveWindow->OnLButtonUp();
	if (bSplitterMoving)
	{
		if (HasCapture())
			ReleaseMouse();
		bSplitterMoving = false;
	}
}
