#pragma once

// Define a simple image viewer window
class ImageWindow : public wxScrolledWindow
{
public:
	ImageWindow(wxWindow *parent, wxWindowID id,
		const wxImage &image = wxNullImage,
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,
		long style = wxSUNKEN_BORDER | wxWANTS_CHARS);

	void SetImage(const wxImage &image);

	void SetZoomFactor(double factor) { m_zoomFactor = factor; DoSetScrollbars(); }
	double GetZoomFactor() { return m_zoomFactor; }


private:
	void SetZoom(float factor);
	//void SetBestFitZoom();
	void DoSetScrollbars(bool refresh = TRUE);

	// event handlers (these functions should _not_ be virtual)
	void OnEraseBackground(wxEraseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);

	wxBitmap m_bitmap, m_dbBitmap;
	double m_zoomFactor;
};
