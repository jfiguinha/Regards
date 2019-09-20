#include <header.h>

#ifdef __WXMSW__
#include <ImageAcquisition.h>
#include <GdiPlusPixelFormats.h>
#include <iostream>
#include <gdiplus.h>
#include <shobjidl.h>
#include <wx/graphics.h>
#define USE_WIA_INTERFACE
#endif
#include <wx/filedlg.h>

#include "ScannerFrame.h"
#include <wx/image.h>
#include <wx/numdlg.h>

#include <wx/wxsanedlg.h>

#define MAX_ZOOM	10.0
#define MIN_ZOOM	0.1

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
	ID_ZOOMIN,
	ID_ZOOMOUT,
	ID_PROMPTONGETIMAGE
};

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
CScannerFrame::CScannerFrame(const wxString &title, const wxPoint &pos, const wxSize &size,
	long style) :
	wxFrame(NULL, wxID_ANY, title, pos, size, style)
{

#if __WXSCANSANE__  
    scanSane = new wxScanSane();	
#endif
    // create a menu bar
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_OPENIMAGE, _("&Open PDF..."), _("Open a pdf file"));
	menuFile->Append(ID_ACQUIREIMAGE, _("&Acquire Image..."), _("Acquire an image"));
	menuFile->Append(ID_SELECTSOURCE, _("&Select Source..."), _("Select source"));
#ifdef USE_IA_EVENTS
	menuFile->AppendSeparator();
	menuFile->AppendCheckItem(ID_PROMPTONGETIMAGE, _("Prompt on wxEVT_IA_GETIMAGE event"), _("Display prompt after acquiring an image"));
	menuFile->Check(ID_PROMPTONGETIMAGE, TRUE);
#endif
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, _("E&xit\tAlt-X"), _("Quit this program"));

	wxMenu *menuView = new wxMenu;
	menuView->Append(ID_ZOOMIN, _("Zoom &In"), _("Zoom in"));
	menuView->Append(ID_ZOOMOUT, _("Zoom &Out"), _("Zoom out"));

	// the "About" item should be in the help menu
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_ABOUT, _("&About...\tF1"), _("Show about dialog"));

	// now append the freshly created menu to the menu bar

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuView, _("&View"));
	menuBar->Append(helpMenu, _("&Help"));

	//and attach this menu bar to the frame
	SetMenuBar(menuBar);

	// create a status bar just for fun (by default with 1 pane only)
	CreateStatusBar(1);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_imagePDF = new CViewerPDF(this, wxID_ANY), 1, wxEXPAND);
	SetSizer(sizer);

	// dynamically connect all event handles
	Connect(ID_OPENIMAGE, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnOpenImage));
	Connect(wxID_EXIT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnQuit));
	Connect(wxID_ABOUT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnAbout));
	Connect(ID_ACQUIREIMAGE, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnAcquireImage));
#ifndef USE_WIA_INTERFACE    
	Connect(ID_SELECTSOURCE, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnSelectSource));
#endif
    Connect(wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CScannerFrame::OnUpdateUI));
    
    
}

CScannerFrame::~CScannerFrame()
{
#if __WXSCANSANE__  
    if(scanSane != nullptr)
        delete scanSane;
#endif

	if (m_imagePDF != nullptr)
		delete m_imagePDF;
}

void CScannerFrame::OnOpenImage(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Open PDF file"), "", "",
			"PDF files (*.pdf)|*.pdf", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea..

	m_imagePDF->LoadFile(openFileDialog.GetPath());
}

// event handlers

void CScannerFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	// TRUE is to force the frame to close
	Close(TRUE);
}

void CScannerFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxString msg = _("wxIA Image Acquisition Test Program.\n"
		"Written by Derry Bryson");

	wxMessageBox(msg, _T("About"), wxOK | wxICON_INFORMATION, this);
}

#ifndef USE_WIA_INTERFACE
void CScannerFrame::OnSelectSource(wxCommandEvent& WXUNUSED(event))
{

    scanSane->SelectSource("", true, this);
}

void CScannerFrame::OnAcquireImage(wxCommandEvent& event)
{
    wxScanSaneAcquireDialog d(this, -1, _("Acquire"), scanSane);
    if (d.ShowModal() == wxID_OK)
    {
        m_imageWin->SetImage(d.GetImage());
    }
}

#else

wxImage CScannerFrame::GdiplusImageTowxImage(Gdiplus::Image * img, Gdiplus::Color bkgd)
{
	const UINT w = img->GetWidth();
	const UINT h = img->GetHeight();
	wxImage image(w, h);
	Gdiplus::Bitmap* bmp = nullptr;
	try {

		auto format = img->GetPixelFormat();
		Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(w, h, format);
		auto g = std::unique_ptr<Gdiplus::Graphics>(Gdiplus::Graphics::FromImage(bmp));
		g->Clear(bkgd);
		g->DrawImage(img, 0, 0, w, h);

		// We need to use Bitmap::LockBits() to convert bitmap to wxImage
		// because this way we can retrieve also alpha channel data.
		// Alternative way by retrieving bitmap handle with Bitmap::GetHBITMAP
		// (to pass it to wxBitmap) doesn't preserve real alpha channel data.
		Gdiplus::REAL hdpi = img->GetHorizontalResolution();
		Gdiplus::REAL vdpi = img->GetVerticalResolution();

	
		// Set up wxImage buffer for alpha channel values
		// only if bitmap contains alpha channel.
		if (IsAlphaPixelFormat(bmp->GetPixelFormat()))
		{
			image.InitAlpha();
		}

		Gdiplus::BitmapData bitmapData;
		Gdiplus::Rect rect(0, 0, w, h);
		bmp->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);

		unsigned char *imgRGB = image.GetData();    // destination RGB buffer
		unsigned char *imgAlpha = image.GetAlpha(); // destination alpha buffer
		const BYTE* pixels = static_cast<const BYTE*>(bitmapData.Scan0);
		for (UINT y = 0; y < h; y++)
		{
			for (UINT x = 0; x < w; x++)
			{
				ARGB c = reinterpret_cast<const ARGB*>(pixels)[x];
				*imgRGB++ = (c >> 16) & 0xFF;  // R
				*imgRGB++ = (c >> 8) & 0xFF;   // G
				*imgRGB++ = (c >> 0) & 0xFF;   // B
				if (imgAlpha)
					*imgAlpha++ = (c >> 24) & 0xFF;
			}

			pixels += bitmapData.Stride;
		}
		bmp->UnlockBits(&bitmapData);


		image.SetOption(wxIMAGE_OPTION_RESOLUTIONUNIT, wxIMAGE_RESOLUTION_INCHES);
		image.SetOption(wxIMAGE_OPTION_RESOLUTIONX, hdpi);
		image.SetOption(wxIMAGE_OPTION_RESOLUTIONY, vdpi);
		image.SetOption(wxIMAGE_OPTION_RESOLUTION, hdpi);
	}
	catch (...) {
		// this might happen if img->GetPixelFormat() is something exotic
		// ... not sure
	}
	return image;
}

void CScannerFrame::OnAcquireImage(wxCommandEvent& event)
{
	wxGraphicsRenderer * gdiplus = wxGraphicsRenderer::GetGDIPlusRenderer();
	gdiplus->CreateContext(this);
	CComPtrArray<IStream> ppStream;
	// Register the callback interface
	DataClasses myData;
	myData.Register();
	LONG sample = myData.getNumDevices();
	CDataCallback callBack(NULL, NULL, &sample, &ppStream);


	// Scan the Image into the stream object ppStream
	HRESULT hr = WiaGetImage(
		(HWND)this->GetHWND(),
		StiDeviceTypeDefault,
		0,
		WIA_INTENT_NONE,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&ppStream.Count(),
		&ppStream
	);

	if (ppStream.Count() != 0)
	{
		Gdiplus::Image m_Image(*ppStream);
		m_imagePDF->SetImage(GdiplusImageTowxImage(&m_Image));
	}

	//delete gdiplus;
}


#endif

void CScannerFrame::OnUpdateUI(wxUpdateUIEvent& event)
{
	switch (event.GetId())
	{
#ifndef USE_WIA_INTERFACE
	case ID_ACQUIREIMAGE:
        if(scanSane != nullptr)
            event.Enable(scanSane->IsSourceSelected());
        else
            event.Enable(false);
		break;
#endif
	}
}
