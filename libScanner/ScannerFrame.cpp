#include <header.h>
#include <BitmapPrintout.h>
#include <PrintEngine.h>
#include <SavePicture.h>
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
#include <PrintEngine.h>
#include "ScannerFrame.h"
#include <wx/image.h>
#include <wx/numdlg.h>
#include <LibResource.h>
#include <wx/wxsanedlg.h>
#include <libPicture.h>
#include "ScannerParam.h"
#include "ScannerParamInit.h"
#include "ScannerTheme.h"
#include "ScannerThemeInit.h"
#include "CentralWindow.h"
using namespace Regards::Print;

#define MAX_ZOOM	10.0
#define MIN_ZOOM	0.1


// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
CScannerFrame::CScannerFrame(const wxString &title, IMainInterface * mainInterface, const wxPoint &pos, const wxSize &size,
	long style) :
	wxFrame(NULL, wxID_ANY, title, pos, size, style)
{

	SetIcon(wxICON(sample));

	this->mainInterface = mainInterface;

	viewerParam = new CScannerParam();
	CScannerParamInit::Initialize(viewerParam);

	viewerTheme = new CScannerTheme();
	CScannerThemeInit::Initialize(viewerTheme);

#if __WXSCANSANE__  
    scanSane = new wxScanSane();	
#endif
    // create a menu bar
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_OPENIMAGE, _("&Open PDF..."), _("Open a pdf file"));
	menuFile->Append(ID_EXPORT, _("&Export PDF..."), _("Export PDF"));
	menuFile->Append(ID_OCR, _("&OCR PDF..."), _("OCR PDF"));
	menuFile->Append(ID_ACQUIREIMAGE, _("&Acquire Image..."), _("Acquire an image"));
#if __WXSCANSANE__  
	menuFile->Append(ID_SELECTSOURCE, _("&Select Source..."), _("Select source"));
#endif
	menuFile->AppendSeparator();
	menuFile->Append(ID_PRINT, _("&Print PDF..."), _("Print PDF"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, _("E&xit\tAlt-X"), _("Quit this program"));

	// the "About" item should be in the help menu
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_ABOUT, _("&About...\tF1"), _("Show about dialog"));

	// now append the freshly created menu to the menu bar

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(helpMenu, _("&Help"));

	//and attach this menu bar to the frame
	SetMenuBar(menuBar);

	// create a status bar just for fun (by default with 1 pane only)
	CreateStatusBar(1);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(centralWindow = new CCentralWindow(this, CENTRALVIEWERWINDOWID, this), 1, wxEXPAND);
	SetSizer(sizer);

	// dynamically connect all event handles
	Connect(ID_OPENIMAGE, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnOpenImage));
	Connect(wxID_EXIT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnQuit));
	Connect(wxID_ABOUT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnAbout));
	Connect(ID_EXPORT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnExport));
	Connect(ID_ACQUIREIMAGE, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnAcquireImage));
	Connect(ID_PRINT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnPrint));
	Connect(ID_OCR, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnOCR));
	
#if __WXSCANSANE__  
	Connect(ID_SELECTSOURCE, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnSelectSource));
	Connect(wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CScannerFrame::OnUpdateUI));
#endif
    
    
	mainInterface->HideAbout();
}

CScannerFrame::~CScannerFrame()
{
#if __WXSCANSANE__  
    if(scanSane != nullptr)
        delete scanSane;
#endif

	if (centralWindow != nullptr)
		delete centralWindow;
}

void CScannerFrame::OnOCR(wxCommandEvent& event)
{
	wxString filename = centralWindow->GetFilename();
	if (filename != "")
	{
		centralWindow->OcrPage();
	}
}

void CScannerFrame::OnExport(wxCommandEvent& event)
{
	wxString filename = centralWindow->GetFilename();
	if (filename != "")
	{
		CSavePicture::SavePicture(this, filename);
	}
}

void CScannerFrame::OnPrint(wxCommandEvent& event)
{
	CLibPicture libPicture;
	wxString filename = centralWindow->GetFilename();
	if (filename != "")
	{
		CImageLoadingFormat * image = libPicture.LoadPicture(filename);
		if (image != nullptr)
			PrintPreview(image);
	}
}


void CScannerFrame::PrintPreview(CImageLoadingFormat * imageToPrint)
{
	// Pass two printout objects: for preview, and possible printing.
	wxPrintData * g_printData = CPrintEngine::GetPrintData();
	wxPrintDialogData printDialogData(*g_printData);

	wxPrintPreview * preview = new wxPrintPreview(new CBitmapPrintout(imageToPrint), 0, &printDialogData);
	if (!preview->IsOk())
	{
		delete preview;
		wxLogError(wxT("There was a problem previewing.\nPerhaps your current printer is not set correctly?"));
		return;
	}
	wxString picture_print_label = CLibResource::LoadStringFromResource(L"PicturePrintPreview", 1);
	wxPreviewFrame * frame = new wxPreviewFrame(preview, this, picture_print_label, wxPoint(100, 100), wxSize(600, 650));
	frame->Centre(wxBOTH);
	frame->InitializeWithModality(wxPreviewFrame_AppModal);
	frame->Show();
}

void CScannerFrame::OnOpenImage(wxCommandEvent& event)
{
	centralWindow->LoadFile();
}

// event handlers

void CScannerFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	// TRUE is to force the frame to close
	mainInterface->Close();
	//Close(TRUE);
	
}

void CScannerFrame::OnClose()
{
	mainInterface->Close();
	//Close(TRUE);
}

void CScannerFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	mainInterface->ShowAbout();
}

#ifdef __WXSCANSANE__ 
void CScannerFrame::OnSelectSource(wxCommandEvent& WXUNUSED(event))
{
    scanSane->SelectSource("", true, this);
}
#endif
#ifdef __WXMSW__
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
#endif
void CScannerFrame::OnAcquireImage(wxCommandEvent& event)
{
	wxImage image = ScanPage();
	if (image.IsOk())
	{
		wxString file = centralWindow->SetImage(image);
		centralWindow->LoadFile(file);
	}
}



wxImage CScannerFrame::ScanPage()
{
	wxImage image;
#if __WXSCANSANE__  
	wxScanSaneAcquireDialog d(this, -1, _("Acquire"), scanSane);
	if (d.ShowModal() == wxID_OK)
	{
		image = d.GetImage();
	}
#else
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
		//for (int i = 0; i < ppStream.Count(); i++)
		//{
			Gdiplus::Image m_Image(*ppStream);
			image = GdiplusImageTowxImage(&m_Image);
		//}
	}

	//delete gdiplus;
#endif
	return image;
}

void CScannerFrame::OnUpdateUI(wxUpdateUIEvent& event)
{
	switch (event.GetId())
	{
#ifdef __WXSCANSANE__  
	case ID_ACQUIREIMAGE:
		if (scanSane != nullptr)
			event.Enable(scanSane->IsSourceSelected());
		else
			event.Enable(false);
		break;
#endif
	}
}