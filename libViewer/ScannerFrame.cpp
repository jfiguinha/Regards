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

#include "ImageWindow.h"
#include "ScannerFrame.h"
#include <wx/image.h>
#include <wx/numdlg.h>


#define MAX_ZOOM	10.0
#define MIN_ZOOM	0.1

// IDs for the controls and the menu commands
enum
{
	// menu items
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
	// create a menu bar
	wxMenu *menuFile = new wxMenu;
#ifdef USE_WIA_INTERFACE
	menuFile->Append(ID_ACQUIREIMAGE, _("&Acquire Image..."), _("Acquire an image"));
#else
	menuFile->Append(ID_ACQUIREIMAGE, _("&Acquire Image..."), _("Acquire an image"));
	menuFile->Append(ID_ACQUIREIMAGENOUI, _("Acquire Image (no ui)..."), _("Acquire an image without user interface"));
#ifdef USE_IA_EVENTS
	menuFile->Append(ID_ACQUIREIMAGES, _("Acquire Images..."), _("Acquire multiple images"));
	menuFile->Append(ID_ACQUIREIMAGESNOUI, _("Acquire Images (no ui)..."), _("Acquire multiple images without user interface"));
#endif
	menuFile->Append(ID_SELECTSOURCE, _("&Select Source..."), _("Select source"));
#ifdef USE_IA_EVENTS
	menuFile->AppendSeparator();
	menuFile->AppendCheckItem(ID_PROMPTONGETIMAGE, _("Prompt on wxEVT_IA_GETIMAGE event"), _("Display prompt after acquiring an image"));
	menuFile->Check(ID_PROMPTONGETIMAGE, TRUE);
#endif
#endif
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, _("E&xit\tAlt-X"), _("Quit this program"));

	wxMenu *menuView = new wxMenu;
	menuView->Append(ID_ZOOMIN, _("Zoom &In"), _("Zoom in"));
	menuView->Append(ID_ZOOMOUT, _("Zoom &Out"), _("Zoom out"));

	// the "About" item should be in the help menu
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_ABOUT, _("&About...\tF1"), _("Show about dialog"));

	// now append the freshly created menu to the menu bar…
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuView, _("&View"));
	menuBar->Append(helpMenu, _("&Help"));

	// … and attach this menu bar to the frame
	SetMenuBar(menuBar);

	// create a status bar just for fun (by default with 1 pane only)
	CreateStatusBar(1);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_imageWin = new ImageWindow(this, wxID_ANY), 1, wxEXPAND);
	SetSizer(sizer);

	// dynamically connect all event handles
	Connect(wxID_EXIT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnQuit));
	Connect(wxID_ABOUT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnAbout));
#ifdef USE_WIA_INTERFACE
	Connect(ID_ACQUIREIMAGE, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnAcquireImage));
#else
	Connect(ID_ACQUIREIMAGE, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnAcquireImage));
	Connect(ID_ACQUIREIMAGENOUI, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnAcquireImage));
#ifdef USE_IA_EVENTS
	Connect(ID_ACQUIREIMAGES, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnAcquireImages));
	Connect(ID_ACQUIREIMAGESNOUI, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnAcquireImages));
#endif
	Connect(ID_SELECTSOURCE, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnSelectSource));
#ifdef USE_IA_EVENTS
	Connect(wxEVT_IA_GETIMAGE, wxIAEventHandler(CScannerFrame::OnGetImage));
	Connect(wxEVT_IA_UPDATE, wxIAEventHandler(CScannerFrame::OnUpdateStatus));
#endif
#endif

	Connect(ID_ZOOMIN, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnZoomIn));
	Connect(ID_ZOOMOUT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnZoomOut));
	Connect(wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CScannerFrame::OnUpdateUI));

#ifndef USE_WIA_INTERFACE
#ifdef USE_IA_EVENTS
	if (wxIAManager::Get().GetDefaultProvider())
		wxIAManager::Get().GetDefaultProvider()->SetEvtHandler(this);
#endif
#endif
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
	wxIAProvider *provider = wxIAManager::Get().GetDefaultProvider();
	if (!provider)
		return;

	wxIAReturnCode rc = provider->SelectSource();
	if (rc != wxIA_RC_SUCCESS)
	{
		wxLogError(wxIAManager::Get().GetReturnCodeDesc(rc));
		return;
	}

	wxIASourceInfo source = provider->GetSelSourceInfo();

	wxString msg = wxString::Format(_("Name: %s\nModel: %s\nVendor: %s\nType: %d"),
		source.GetName(), source.GetModel(),
		source.GetVendor(), source.GetType());

	wxMessageBox(msg, _("Selected Source Information"), wxOK, this);
}

void CScannerFrame::OnAcquireImage(wxCommandEvent& event)
{
	wxIAProvider *provider = wxIAManager::Get().GetDefaultProvider();

	if (!provider || !provider->IsSourceSelected())
		return;
	wxIAUIMode uiMode;
	wxIAReturnCode rc;

	if (event.GetId() == ID_ACQUIREIMAGE)
		uiMode = wxIA_UIMODE_NORMAL;
	else
		uiMode = wxIA_UIMODE_NONE;

	m_imageCount = 1;

	rc = provider->AcquireImage(uiMode, this);

#ifndef USE_IA_EVENTS
	if (rc == wxIA_RC_SUCCESS)
		m_imageWin->SetImage(provider->GetImage());
#endif
	if (rc != wxIA_RC_SUCCESS)
		wxLogError(wxIAManager::Get().GetReturnCodeDesc(rc));
}

#ifdef USE_IA_EVENTS
void CScannerFrame::OnAcquireImages(wxCommandEvent& event)
{
	wxIAProvider *provider = wxIAManager::Get().GetDefaultProvider();

	if (!provider || !provider->IsSourceSelected())
		return;

	m_imageCount = wxGetNumberFromUser(_("Enter number of images to acquire"),
		_("Count:"), _("Acquire Images"), 1, 0, 10, this);
	if (m_imageCount <= 0)
		return;

	wxIAUIMode uiMode = event.GetId() == ID_ACQUIREIMAGES ?
		wxIA_UIMODE_NORMAL : wxIA_UIMODE_NONE;

	wxIAReturnCode rc = provider->AcquireImages(m_imageCount, uiMode, this);
	if (rc != wxIA_RC_SUCCESS)
		wxLogError(wxIAManager::Get().GetReturnCodeDesc(rc));

}
#endif
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
		m_imageWin->SetImage(GdiplusImageTowxImage(&m_Image));
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
	case ID_ACQUIREIMAGENOUI:
#ifdef USE_IA_EVENTS
	case ID_ACQUIREIMAGES:
	case ID_ACQUIREIMAGESNOUI:
#endif
		event.Enable(wxIAManager::Get().GetDefaultProvider() &&
			wxIAManager::Get().GetDefaultProvider()->IsSourceSelected());
		break;
	case ID_SELECTSOURCE:
		event.Enable(wxIAManager::Get().GetDefaultProvider());
		break;
#endif
	case ID_ZOOMIN:
		event.Enable(m_imageWin->GetZoomFactor() < MAX_ZOOM);
		break;

	case ID_ZOOMOUT:
		event.Enable(m_imageWin->GetZoomFactor() > MIN_ZOOM);
		break;
	}
}
#ifndef USE_WIA_INTERFACE
#ifdef USE_IA_EVENTS
void CScannerFrame::OnGetImage(wxIAEvent& event)
{
	m_imageCount--;
	m_imageWin->SetImage(event.GetProvider()->GetImage());

	if (GetMenuBar()->IsChecked(ID_PROMPTONGETIMAGE) &&
		m_imageCount && wxMessageBox(_("Got image, continue?"), _("Acquire Image"), wxYES_NO, this) != wxYES)
		event.Abort(TRUE);
}

void CScannerFrame::OnUpdateStatus(wxIAEvent& event)
{
	SetStatusText(wxString::Format(_("%s %d of %d"), event.GetText(),
		event.GetQuantum(), event.GetSpan()));
}
#endif
#endif

void CScannerFrame::OnZoomIn(wxCommandEvent& WXUNUSED(event))
{
	if (m_imageWin->GetZoomFactor() < MAX_ZOOM)
		m_imageWin->SetZoomFactor(m_imageWin->GetZoomFactor() * 2.0);
}

void CScannerFrame::OnZoomOut(wxCommandEvent& WXUNUSED(event))
{
	if (m_imageWin->GetZoomFactor() > MIN_ZOOM)
		m_imageWin->SetZoomFactor(m_imageWin->GetZoomFactor() / 2.0);
}
