#include <header.h>
#include "wxImagePanel.h"
// some useful events
/*
 void wxImagePanel::mouseMoved(wxMouseEvent& event) {}
 void wxImagePanel::mouseDown(wxMouseEvent& event) {}
 void wxImagePanel::mouseWheelMoved(wxMouseEvent& event) {}
 void wxImagePanel::mouseReleased(wxMouseEvent& event) {}
 void wxImagePanel::rightClick(wxMouseEvent& event) {}
 void wxImagePanel::mouseLeftWindow(wxMouseEvent& event) {}
 void wxImagePanel::keyPressed(wxKeyEvent& event) {}
 void wxImagePanel::keyReleased(wxKeyEvent& event) {}
 */

wxImagePanel::wxImagePanel(wxWindow* parent) :
wxPanel(parent)
{
    Connect(wxEVT_PAINT, wxPaintEventHandler(wxImagePanel::paintEvent));
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void wxImagePanel::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void wxImagePanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void wxImagePanel::render(wxDC&  dc)
{
   
	//wxPaintDC pdc(this);
	int w, h;

	GetClientSize(&w, &h);
	if (!m_dbBitmap.Ok() || m_dbBitmap.GetWidth() != w || m_dbBitmap.GetHeight() != h)
		m_dbBitmap = wxBitmap(w, h);

	if (!m_dbBitmap.Ok())
		return;

	//wxMemoryDC localdc;
	//localdc.SelectObject(m_dbBitmap);
	//
	//  Clear the window
	//
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(wxBrush(GetBackgroundColour(), wxSOLID));
	dc.SetBackground(wxBrush(GetBackgroundColour(), wxSOLID));
	dc.DrawRectangle(0, 0, w, h);
	dc.SetBrush(wxBrush(GetForegroundColour(), wxCROSSDIAG_HATCH));
	dc.DrawRectangle(0, 0, w, h);

	if (image.Ok())
	{
		//wxMemoryDC memdc;
		int bx, by, bw, bh;

        float scale_x = float(GetSize().x) / float(image.GetWidth());
        float scale_y = float(GetSize().y) / float(image.GetHeight());
        
        m_zoomFactor = std::min(scale_x, scale_y);
		bw = (int)(image.GetWidth() * m_zoomFactor);
		bh = (int)(image.GetHeight() * m_zoomFactor);

		if (bw < w)
		{
			bx = (w - bw) / 2;
		}
		else
			bx = 0;

		if (bh < h)
		{
            by = (h - bh) / 2;
		}
		else
			by = 0;
		//localdc.SetUserScale(m_zoomFactor, m_zoomFactor);
		//localdc.Blit(bx, by, zw + 1, zh + 1, &memdc, x, y);
        wxImage bitmapResize = image.ResampleBicubic(bw, bh);
        dc.DrawBitmap(wxBitmap(bitmapResize), wxPoint(bx, by));
	}
}

void wxImagePanel::SetImage(const wxImage &image)
{
    this->image = image;
}