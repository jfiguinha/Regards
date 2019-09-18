#pragma once
#ifndef USE_WIA_INTERFACE  
#include <wx/wxscan.h>
#endif
class ImageWindow;

// Define a new frame type: this is going to be our main frame
class CScannerFrame : public wxFrame
{
public:
	// ctor(s)
	CScannerFrame(const wxString &title, const wxPoint &pos, const wxSize &size,
		long style = wxDEFAULT_FRAME_STYLE);

    ~CScannerFrame();

private:
	// event handlers (these functions should _not_ be virtual)
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
    void OnAcquireImage(wxCommandEvent& event);
#ifdef USE_WIA_INTERFACE
	wxImage GdiplusImageTowxImage(Gdiplus::Image * img, Gdiplus::Color bkgd = Gdiplus::Color::Transparent);
#else
	void OnSelectSource(wxCommandEvent& event);
#endif
	void OnZoomIn(wxCommandEvent& event);
	void OnZoomOut(wxCommandEvent& event);
	void OnUpdateUI(wxUpdateUIEvent& event);
    
#ifdef __WXSCANSANE__
    wxScanSane * scanSane;
#endif
	ImageWindow *m_imageWin;
	int m_imageCount;
};