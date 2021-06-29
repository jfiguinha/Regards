#include <header.h>
#include <BitmapPrintout.h>
#include <PrintEngine.h>
#include <SavePicture.h>
#ifdef __WXMSW__
#include <ImageAcquisition.h>
#include <GdiPlusPixelFormats.h>
#include <gdiplus.h>
#include <shobjidl.h>
#include <wx/graphics.h>
#define USE_WIA_INTERFACE
#endif
#include "ScannerFrame.h"
#include <wx/image.h>
#include <wx/numdlg.h>
#include <LibResource.h>
#include <wx/wxsanedlg.h>
#include <wx/dir.h>
#include <libPicture.h>
#include "CentralWindow.h"
#include <MyFrameIntro.h>
#include <FileUtility.h>
#include "OcrWnd.h"
#include <wx/filefn.h> 
#include <ImageLoadingFormat.h>
#include <MainThemeInit.h>
#include <MainParamInit.h>
using namespace Regards::Picture;
using namespace Regards::Print;
using namespace Regards::Introduction;
using namespace Regards::Scanner;
#define MAX_ZOOM	10.0
#define MIN_ZOOM	0.1


#ifndef wxHAS_IMAGES_IN_RESOURCES
#ifdef __WXGTK__
#include "../Resource/sample.xpm"
#else
#include "../../Resource/sample.xpm"
#endif
#endif

//Connect(wxEVT_MOVE, wxMoveEventHandler(Move::OnMove));
BEGIN_EVENT_TABLE(CScannerFrame, wxFrame)
EVT_CLOSE(CScannerFrame::OnCloseWindow)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
CScannerFrame::CScannerFrame(const wxString &title, IMainInterface * mainInterface, const wxPoint &pos, const wxSize &size,
	long style) :
	wxFrame(NULL, wxID_ANY, title, pos, size, style)
{

	SetIcon(wxICON(sample));
	Maximize();
	this->mainInterface = mainInterface;

	viewerParam = new CMainParam();
	CMainParamInit::Initialize(viewerParam);

	viewerTheme = new CMainTheme();
	CMainThemeInit::Initialize(viewerTheme);
#ifndef __APPLE__
#if __WXSCANSANE__  
    scanSane = new wxScanSane();	
#endif
#endif
    // create a menu bar
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_OPENIMAGE, _("&Open PDF..."), _("Open a pdf file"));
	menuFile->Append(ID_EXPORT, _("&Export PDF..."), _("Export PDF"));
	//menuFile->Append(ID_EXPORTHTML, _("&Export PDF to HTML ..."), _("Export PDF to HTML"));
	//menuFile->Append(ID_EXPORTTXT, _("&Export PDF to TEXT ..."), _("Export PDF to TEXT"));
	//menuFile->Append(ID_OCR, _("&OCR PDF..."), _("OCR PDF"));
	menuFile->Append(ID_ACQUIREIMAGE, _("&Acquire Image..."), _("Acquire an image"));
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
	sizer->Add(centralWindow = new CCentralWindow(this, SCANNER_CENTRALVIEWERWINDOWID, this), 1, wxEXPAND);
	SetSizer(sizer);

	// dynamically connect all event handles
	Connect(ID_OPENIMAGE, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnOpenImage));
	Connect(wxID_EXIT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnQuit));
	Connect(wxID_ABOUT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnAbout));
	Connect(ID_EXPORT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnExport));
	//Connect(ID_EXPORTHTML, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnExportHTML));
	//Connect(ID_EXPORTTXT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnExportText));
	Connect(ID_ACQUIREIMAGE, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnAcquireImage));
	Connect(ID_PRINT, wxEVT_MENU, wxCommandEventHandler(CScannerFrame::OnPrint));
	Connect(wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CScannerFrame::OnUpdateUI));

}

void CScannerFrame::OnCloseWindow(wxCloseEvent &event)
{
	if (mainInterface != nullptr)
	{
		wxCommandEvent evt(wxEVENT_CLOSESCANNER);
		mainInterface->parent->GetEventHandler()->AddPendingEvent(evt);
	}
	else
	{
		Exit();
	}
}

CScannerFrame::~CScannerFrame()
{
#ifndef __APPLE__
#if __WXSCANSANE__  
    if(scanSane != nullptr)
        delete scanSane;
#endif
#endif
	if (centralWindow != nullptr)
		delete centralWindow;
}

int CScannerFrame::OnOpen()
{
	int value = -1;
	if(centralWindow != nullptr)
		value = centralWindow->OnOpen(0);
	return value;
}

void CScannerFrame::OnExport(wxCommandEvent& event)
{
	wxString filename = centralWindow->GetFilename();
	if (filename != "")
	{
		CSavePicture::SavePicture(nullptr, nullptr, filename);
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
	if (centralWindow != nullptr)
		centralWindow->OnOpen(0);
}

// event handlers

void CScannerFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	if (mainInterface != nullptr)
	{
		wxCommandEvent evt(wxEVENT_CLOSESCANNER);
		mainInterface->parent->GetEventHandler()->AddPendingEvent(evt);
	}
	else
	{
		Exit();
	}
}

void CScannerFrame::Exit()
{
	if (mainInterface != nullptr)
		mainInterface->Close();
}

void CScannerFrame::OnClose()
{
	if (mainInterface != nullptr)
	{
		wxCommandEvent evt(wxEVENT_CLOSESCANNER);
		mainInterface->parent->GetEventHandler()->AddPendingEvent(evt);
	}
	else
	{
		Exit();
	}
}

void CScannerFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	if(mainInterface != nullptr)
		mainInterface->ShowAbout();
}

#ifdef __WXMSW__
wxImage CScannerFrame::GdiplusImageTowxImage(Gdiplus::Image * img, Gdiplus::Color bkgd)
{
	const UINT w = img->GetWidth();
	const UINT h = img->GetHeight();
	wxImage image(w, h);
	//Gdiplus::Bitmap* bmp = nullptr;
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
	wxString pdfFile = ScanPage();
	if (pdfFile != "")
	{
		centralWindow->LoadFile(pdfFile);
	}
}



wxString CScannerFrame::ScanPage()
{
    wxString pdfFile = "";
	wxImage image;
#ifdef __APPLE__
    wxArrayString output;
    printf("CScannerFrame::ScanPage \n");
    //Find all file with Scan
    pdfFile = CFileUtility::GetTempFile("Scan.pdf");
        
   // wxString scanFile = CFileUtility::GetTempFile("Scan.tiff");
    wxExecute("ScannerBrowser.app/Contents/MacOS/ScannerBrowser", output);
	
	if (!wxFileExists(pdfFile))
		pdfFile = "";
    
    /*
    CScannerWindow d(this, -1, _("Acquire"));
    if (d.ShowModal() == wxID_OK)
    {
        //image = d.GetImage();
    }
    */
    
#else
#if __WXSCANSANE__  

    scanSane->SelectSource("", true, this);

    if(scanSane->IsSourceSelected())
    {
        wxScanSaneAcquireDialog d(this, -1, _("Acquire"), scanSane);
        if (d.ShowModal() == wxID_OK)
        {
            image = d.GetImage();
        }
    }
    else
    {
		wxString selectScanner = CLibResource::LoadStringFromResource("LBLSELECTSCANNERSOURCE", 1);
		wxString infos = CLibResource::LoadStringFromResource("labelInformations", 1);
        wxMessageBox(selectScanner, infos);
    }
#else
	{

		DataClasses myData;
		
		LONG sample = myData.getNumDevices();
		if (sample > 0)
		{
			myData.Register();
			wxGraphicsRenderer * gdiplus = wxGraphicsRenderer::GetGDIPlusRenderer();
			gdiplus->CreateContext(this);
			CComPtrArray<IStream> ppStream;
			// Register the callback interface

			CDataCallback callBack(NULL, NULL, &sample, &ppStream);


			// Scan the Image into the stream object ppStream
			WiaGetImage(
				(HWND)this->GetHWND(),
				StiDeviceTypeScanner,
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
		}
		else
		{
			wxString scannerdetected = CLibResource::LoadStringFromResource("LBLNOSCANNERDETECTED", 1);
			wxString infos = CLibResource::LoadStringFromResource("informationserror", 1);
			wxMessageBox(scannerdetected, infos, wxICON_ERROR);
		}


		myData.Release();
	}


	//delete gdiplus;
#endif
#endif

#ifndef __APPLE__
	if (image.IsOk())
	{
		CLibPicture picture;
		pdfFile = CFileUtility::GetTempFile("scanner.pdf");
		if (wxFileExists(pdfFile))
			wxRemoveFile(pdfFile);
		picture.SaveToPDF(&image, pdfFile);

	}
#endif
	return pdfFile;
}

void CScannerFrame::OnUpdateUI(wxUpdateUIEvent& event)
{
	switch (event.GetId())
	{
    case ID_ACQUIREIMAGE:
		event.Enable(true);
		break;
	case ID_EXPORTHTML:
	case ID_EXPORTTXT:
	case ID_EXPORT:
	
	case ID_OCR:
	case ID_PRINT:
	{
		wxString filename = centralWindow->GetFilename();
		if (filename != "")
			event.Enable(true);
		else
			event.Enable(false);
		break;
	}
	default: ;
	}
}