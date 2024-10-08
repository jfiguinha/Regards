#pragma once

#include "viewerPDF.h"
#ifndef __WXMSW__
#ifndef __APPLE__
#include <wx/wxscan.h>
#endif
#endif
#include <MainInterface.h>
#include <MainTheme.h>
#include <MainParam.h>


using namespace Regards::Scanner;

namespace Regards
{
	namespace Scanner
	{
		class CCentralWindow;
	}
}

// IDs for the controls and the menu commands
enum
{
	// menu items
	ID_OPENIMAGE,
	ID_EXPORT,
	ID_EXPORTHTML,
	ID_EXPORTTXT,
	ID_ACQUIREIMAGE,
	ID_OCR,
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
	CScannerFrame(const wxString &title, ISCannerInterface * mainInterface, const wxPoint &pos, const wxSize &size,
		long style = wxDEFAULT_FRAME_STYLE);

    ~CScannerFrame();

	void PrintPreview(CImageLoadingFormat * imageToPrint);

	wxString ScanPage();
	int OnOpen();
	void OnClose();

private:

	void Exit();
	// event handlers (these functions should _not_ be virtual)
	void OnQuit(wxCommandEvent& event);
	void OnPrint(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnExport(wxCommandEvent& event);
    void OnAcquireImage(wxCommandEvent& event);
	void OnCloseWindow(wxCloseEvent &event);
	//void OnExportText(wxCommandEvent& event);
	//void OnExportHTML(wxCommandEvent& event);

	void OnOpenImage(wxCommandEvent& event);
	void OnUpdateUI(wxUpdateUIEvent& event);
   
#ifdef __SCANNER_PROGRAM__
	Regards::Scanner::CMainParam * viewerParam;
	Regards::Scanner::CMainTheme * viewerTheme;
#else
	Regards::Viewer::CMainParam * viewerParam;
	Regards::Viewer::CMainTheme * viewerTheme;
#endif

#ifndef __APPLE__
#ifdef __WXSCANSANE__  
    wxScanSane * scanSane;
#endif
#endif

	//Toolbar
	Regards::Scanner::CCentralWindow * centralWindow;
	//int m_imageCount;
	ISCannerInterface * mainInterface;

	DECLARE_EVENT_TABLE()
};
