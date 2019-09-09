#pragma once
class ImageWindow;



#include <wx/ia/ia.h>

// Define a new frame type: this is going to be our main frame
class CScannerFrame : public wxFrame
{
public:
	// ctor(s)
	CScannerFrame(const wxString &title, const wxPoint &pos, const wxSize &size,
		long style = wxDEFAULT_FRAME_STYLE);

private:
	// event handlers (these functions should _not_ be virtual)
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
#ifdef USE_WIA_INTERFACE
	void OnAcquireImage(wxCommandEvent& event);
	wxImage GdiplusImageTowxImage(Gdiplus::Image * img, Gdiplus::Color bkgd = Gdiplus::Color::Transparent);
#else
	void OnAcquireImage(wxCommandEvent& event);
	void OnAcquireImages(wxCommandEvent& event);
	void OnSelectSource(wxCommandEvent& event);
#ifdef USE_IA_EVENTS
	void OnGetImage(wxIAEvent& event);
	void OnUpdateStatus(wxIAEvent& event);
#endif
#endif
	void OnUpdateUI(wxUpdateUIEvent& event);
	void OnZoomIn(wxCommandEvent& event);
	void OnZoomOut(wxCommandEvent& event);

	ImageWindow *m_imageWin;
	int m_imageCount;
};
