#pragma once
#include <wx/wxscan.h>
#include "viewerPDF.h"
#ifdef __WXMSW__
#include <gdiplus.h>
#endif
using namespace Regards::Viewer;

// IDs for the controls and the menu commands
enum
{
	// menu items
	ID_OPENIMAGE,
	ID_ACQUIREIMAGE,
	ID_ACQUIREIMAGES,
	ID_ACQUIREIMAGENOUI,
	ID_ACQUIREIMAGESNOUI,
	ID_SELECTSOURCE,
	ID_PRINT,
	ID_ZOOMIN,
	ID_ZOOMOUT,
	ID_PROMPTONGETIMAGE
};


// Define a new frame type: this is going to be our main frame
class CScannerFrame : public wxFrame
{
public:
	// ctor(s)
	CScannerFrame(const wxString &title, const wxPoint &pos, const wxSize &size,
		long style = wxDEFAULT_FRAME_STYLE);

    ~CScannerFrame();

	void PrintPreview(CImageLoadingFormat * imageToPrint);

	void ScanPage();

private:
	// event handlers (these functions should _not_ be virtual)
	void OnQuit(wxCommandEvent& event);
	void OnPrint(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
    void OnAcquireImage(wxCommandEvent& event);
	
#ifdef __WXMSW__
	wxImage GdiplusImageTowxImage(Gdiplus::Image * img, Gdiplus::Color bkgd = Gdiplus::Color::Transparent);
#endif
#ifdef __WXSCANSANE__ 
	void OnSelectSource(wxCommandEvent& event);
#endif
	void OnOpenImage(wxCommandEvent& event);
	void OnUpdateUI(wxUpdateUIEvent& event);
    
#if __WXSCANSANE__  
    wxScanSane * scanSane;
#endif
	CViewerPDF * m_imagePDF;
	int m_imageCount;
};