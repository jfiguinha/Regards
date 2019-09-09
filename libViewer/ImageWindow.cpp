#include <header.h>
#include "ImageWindow.h"
#include <wx/image.h>
#include <wx/numdlg.h>

// ----------------------------------------------------------------------------
// image viewer window
// ----------------------------------------------------------------------------
ImageWindow::ImageWindow(wxWindow *parent, wxWindowID id, const wxImage &image,
	const wxPoint &pos, const wxSize &size, long style) :
	wxScrolledWindow(parent, id, pos, size, style)
{
	m_zoomFactor = 1.0;
	if (image.Ok())
	{
		m_bitmap = wxBitmap(image);
		DoSetScrollbars(FALSE);
	}
	SetForegroundColour(*wxWHITE);

	Connect(wxEVT_PAINT, wxPaintEventHandler(ImageWindow::OnPaint));
	Connect(wxEVT_SIZE, wxSizeEventHandler(ImageWindow::OnSize));
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(ImageWindow::OnEraseBackground));
}

void ImageWindow::DoSetScrollbars(bool refresh)
{
	int x, y;
	GetViewStart(&x, &y);

	if (m_bitmap.Ok())
		SetScrollbars(1, 1, m_bitmap.GetWidth() * m_zoomFactor,
			m_bitmap.GetHeight() * m_zoomFactor, x, y, TRUE);

	if (refresh)
		Refresh();
}

void ImageWindow::SetImage(const wxImage &image)
{
	m_bitmap = wxNullBitmap;

	if (image.Ok())
		m_bitmap = wxBitmap(image);

	DoSetScrollbars(TRUE);
}

void ImageWindow::OnEraseBackground(wxEraseEvent& event)
{
	// Don't do anything here, we redraw the entire window in OnPaint()
}

void ImageWindow::OnPaint(wxPaintEvent& event)
{
	wxPaintDC pdc(this);
	int x, y, w, h;

	GetClientSize(&w, &h);
	if (!m_dbBitmap.Ok() || m_dbBitmap.GetWidth() != w || m_dbBitmap.GetHeight() != h)
		m_dbBitmap = wxBitmap(w, h);

	if (!m_dbBitmap.Ok())
		return;

	wxMemoryDC dc;
	dc.SelectObject(m_dbBitmap);
	GetViewStart(&x, &y);

	//
	//  Clear the window
	//
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(wxBrush(GetBackgroundColour(), wxSOLID));
	dc.SetBackground(wxBrush(GetBackgroundColour(), wxSOLID));
	dc.DrawRectangle(0, 0, w, h);
	dc.SetBrush(wxBrush(GetForegroundColour(), wxCROSSDIAG_HATCH));
	dc.DrawRectangle(0, 0, w, h);

	if (m_bitmap.Ok())
	{
		wxMemoryDC memdc;
		int bx, by, zw, zh, bw, bh;

		x = (int)(x / m_zoomFactor + 0.5);
		y = (int)(y / m_zoomFactor + 0.5);
		zw = (int)((w / m_zoomFactor) + 0.5);
		zh = (int)((h / m_zoomFactor) + 0.5);
		bw = (int)(m_bitmap.GetWidth() * m_zoomFactor + 0.5);
		bh = (int)(m_bitmap.GetHeight() * m_zoomFactor + 0.5);

		if (m_bitmap.GetWidth() < zw)
		{
			bx = (zw - m_bitmap.GetWidth()) / 2;
			zw = m_bitmap.GetWidth();
		}
		else
			bx = 0;

		if (m_bitmap.GetHeight() < zh)
		{
			by = (zh - m_bitmap.GetHeight()) / 2;
			zh = m_bitmap.GetHeight();
		}
		else
			by = 0;

		memdc.SelectObject(m_bitmap);
		dc.SetUserScale(m_zoomFactor, m_zoomFactor);
		dc.Blit(bx, by, zw + 1, zh + 1, &memdc, x, y);
		memdc.SelectObject(wxNullBitmap);
	}
	dc.SetUserScale(1.0, 1.0);
	pdc.Blit(0, 0, w, h, &dc, 0, 0);
	dc.SelectObject(wxNullBitmap);
}

void ImageWindow::OnSize(wxSizeEvent& event)
{
	DoSetScrollbars();
}
