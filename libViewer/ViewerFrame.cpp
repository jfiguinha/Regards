#include "ViewerFrame.h"
#include "MainWindow.h"
#include <BitmapPrintout.h>
#include "ViewerParamInit.h"
#include <PrintEngine.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <wx/dirdlg.h>
#include <ConfigRegards.h>
#include <OpenCLDialog.h>
#include "ViewerParamInit.h"
#include "ViewerThemeInit.h"
#include "ViewerParam.h"
#include "ViewerTheme.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <FileUtility.h>
using namespace std;
using namespace Regards::Print;
using namespace Regards::Control;
using namespace Regards::Viewer;

#if !wxUSE_PRINTING_ARCHITECTURE
#error "You must set wxUSE_PRINTING_ARCHITECTURE to 1 in setup.h, and recompile the library."
#endif


// constants:
enum
{
	ID_Hello = 1,
	ID_Folder = 2,
	ID_Configuration = 3,
	ID_OpenCL = 4,
    ID_SIZEICONLESS = 5,
    ID_SIZEICONMORE = 6,
	WXPRINT_PAGE_SETUP,
	WXPRINT_PAGE_SETUP_PS,
#ifdef __WXMAC__
	WXPRINT_PAGE_MARGINS,
#endif
};

#include <ctype.h>
#include "wx/metafile.h"
#include "wx/print.h"
#include "wx/printdlg.h"
#include "wx/image.h"
#include "wx/accel.h"

#if wxUSE_POSTSCRIPT
#include "wx/generic/printps.h"
#include "wx/generic/prntdlgg.h"
#endif

#if wxUSE_GRAPHICS_CONTEXT
#include "wx/graphics.h"
#endif

#ifdef __WXMAC__
#include "wx/osx/printdlg.h"
#endif

#ifndef wxHAS_IMAGES_IN_RESOURCES
#ifdef __WXGTK__
#include "../Resource/sample.xpm"
#else
#include "../../Resource/sample.xpm"
#endif
#endif

bool CViewerFrame::viewerMode = true;

using namespace Regards::Viewer;

bool CViewerFrame::GetViewerMode()
{
    return viewerMode;
}

void CViewerFrame::SetViewerMode(const bool &mode)
{
    viewerMode = mode;
}

CViewerFrame::CViewerFrame(const wxString& title, const wxPoint& pos, const wxSize& size, IMainInterface * mainInterface, const wxString &fileToOpen)
	: wxFrame(nullptr, wxID_ANY, title, pos, size, wxMAXIMIZE | wxDEFAULT_FRAME_STYLE)
{

	SetIcon(wxICON(sample));

	viewerParam = new CViewerParam();
	CViewerParamInit::Initialize(viewerParam);

	viewerTheme = new CViewerTheme();
	CViewerThemeInit::Initialize(viewerTheme);

	this->mainInterface = mainInterface;

	//SetIcon(wxIcon(wxT("regards.xpm")));
	mainWindow = new CMainWindow(this, MAINVIEWERWINDOWID, this);
	m_previewModality = wxPreviewFrame_AppModal;

	wxMenu *menuFile = new wxMenu;

	//menuFile->Append(ID_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
    if(CViewerFrame::GetViewerMode())
        menuFile->Append(ID_Folder, "&Folder Select", "Select new folder");
    
    wxMenu *menuSizeIcon = new wxMenu;
    menuSizeIcon->Append(ID_SIZEICONLESS, "&Decrease Icon Size", "Decrease Icon Size");
    menuSizeIcon->Append(ID_SIZEICONMORE, "&Enlarge Icon Size", "Enlarge Icon Size");
    
	menuFile->Append(ID_Configuration, "&Configuration", "Configuration");
	menuFile->Append(ID_OpenCL, "&OpenCL Configuration", "Configuration OpenCL");
	//menuFile->Append(wxID_PRINT, wxT("&Print..."), wxT("Print"));
	menuFile->Append(WXPRINT_PAGE_SETUP, wxT("Page Set&up..."), wxT("Page setup"));
#ifdef __WXMAC__
	menuFile->Append(WXPRINT_PAGE_MARGINS, wxT("Page Margins..."), wxT("Page margins"));
#endif
	//menuFile->Append(wxID_PREVIEW, wxT("Print Pre&view"), wxT("Preview"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
    menuHelp->Append(wxID_HELP);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
    menuBar->Append(menuSizeIcon, "&Icon Size");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);

	SetLabel(wxT("Regards Viewer"));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CViewerFrame::OnSize));
	Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(CViewerFrame::OnClose));
	mainWindow->Bind(wxEVT_CHAR_HOOK, &CViewerFrame::OnKeyDown, this);

#ifdef __APPLE__
	if (!viewerMode)
		mainWindow->SetDatabase();
	else
	{
		if (viewerParam != nullptr)
		{
			wxString folderPath = viewerParam->GetLastFolder();
			mainWindow->SetFolder(folderPath);
		}
		else
		{
			mainWindow->SetFolder("");
		}
	}
#else
    if(!viewerMode)
        mainWindow->SetDatabase();
    else
    {
		if (fileToOpen == "")
		{
			if (viewerParam != nullptr)
			{
				wxString folderPath = viewerParam->GetLastFolder();
				mainWindow->SetFolder(folderPath);
			}
			else
			{
				mainWindow->SetFolder("");
			}
		}
		else
		{
			OpenFile(fileToOpen);
		}
    }
#endif
    
    mainInterface->HideAbout();
}

void CViewerFrame::OnHelp(wxCommandEvent& event)
{
    wxString helpFile = CFileUtility::GetResourcesFolderPath();
    helpFile.Append("//NoticeRegardsViewer.pdf");
    wxLaunchDefaultApplication(helpFile);
}

void CViewerFrame::Exit()
{
	this->Close(true);
}

void CViewerFrame::OnClose(wxCloseEvent& event)
{
	mainInterface->HideViewer();
	return;
}



void CViewerFrame::OnKeyDown(wxKeyEvent& event)
{
	if (event.m_keyCode == WXK_ESCAPE && fullscreen)
	{
		mainWindow->SetScreen();
	}
	event.Skip();
}

void CViewerFrame::SetFullscreen()
{
	fullscreen = true;
	this->ShowFullScreen(true);
	mainWindow->SetFullscreenMode();
}

void CViewerFrame::SetScreen()
{
	fullscreen = false;
	this->ShowFullScreen(false);
}

void CViewerFrame::SetWindowTitle(const wxString &libelle)
{
	SetLabel(libelle);
}

CViewerFrame::~CViewerFrame()
{
	viewerParam->SaveFile();

	if (mainWindow != nullptr)
		delete(mainWindow);

	if (viewerParam != nullptr)
		delete(viewerParam);

	if (viewerTheme != nullptr)
		delete(viewerTheme);
}

void CViewerFrame::SetText(const int &numPos, const wxString &libelle)
{
	if (mainWindow != nullptr)
		mainWindow->SetText(numPos, libelle);
}

void CViewerFrame::SetRangeProgressBar(const int &range)
{
	if (mainWindow != nullptr)
		mainWindow->SetRangeProgressBar(range);
}

void CViewerFrame::SetPosProgressBar(const int &position)
{
	if (mainWindow != nullptr)
		mainWindow->SetPosProgressBar(position);
}

void CViewerFrame::OnConfiguration(wxCommandEvent& event)
{
	ConfigRegards configFile(this);
	configFile.ShowModal();
}

void CViewerFrame::OnConfigurationOpenCL(wxCommandEvent& event)
{
	OpenCLDialog openCLConfig(this);
	openCLConfig.ShowModal();
	if (openCLConfig.IsOk())
	{
		int platformIndex = openCLConfig.GetIndexItem();
		wxString platformName = openCLConfig.GetSelectItem();
		CRegardsConfigParam * regardsParam = CParamInit::getInstance();
		regardsParam->SetOpenCLPlatformName(platformName);
		regardsParam->SetOpenCLPlatformIndex(platformIndex);
	}
}

void CViewerFrame::OnSelectFolder(wxCommandEvent& event)
{
    
    
    
	wxDirDialog dlg(nullptr, "Choose input directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dlg.ShowModal() == wxID_OK)
	{
		wxString folder = dlg.GetPath();
		mainWindow->SetFolder(folder);
	}
}

void CViewerFrame::SetFolder(const wxString &lastFolder)
{
	mainWindow->SetFolder(lastFolder);
}

void CViewerFrame::SetDatabase()
{
	mainWindow->SetDatabase();
}

void CViewerFrame::OpenFile(const wxString &filename)
{
    mainWindow->SetPictureFile(filename);
}

void CViewerFrame::OnIconSizeLess(wxCommandEvent& event)
{
    float ratio = 1.0;
    CRegardsConfigParam * config = CParamInit::getInstance();
    if(config != nullptr)
    {
        ratio = config->GetIconSizeRatio();
        if(ratio > 1.0)
            ratio = ratio - 0.25;
        config->SetIconSizeRatio(ratio);
        
        mainWindow->UpdateScreenRatio();
        //this->Refresh();
    }
    
}

void CViewerFrame::OnIconSizeMore(wxCommandEvent& event)
{
    float ratio = 1.0;
    CRegardsConfigParam * config = CParamInit::getInstance();
    if(config != nullptr)
    {
        ratio = config->GetIconSizeRatio();
        if(ratio < 2.0)
            ratio = ratio + 0.25;
        config->SetIconSizeRatio(ratio);
        
        mainWindow->UpdateScreenRatio();
        //this->Refresh();
    }
}

void CViewerFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}
void CViewerFrame::OnAbout(wxCommandEvent& event)
{
	mainInterface->ShowAbout();
}

void CViewerFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}

void CViewerFrame::PrintPreview(CRegardsBitmap * imageToPrint)
{
	// Pass two printout objects: for preview, and possible printing.
	wxPrintData * g_printData = CPrintEngine::GetPrintData();
	wxPrintDialogData printDialogData(*g_printData);
	wxPrintPreview *preview = new wxPrintPreview(new CBitmapPrintout(imageToPrint), new CBitmapPrintout(imageToPrint), &printDialogData);
	if (!preview->IsOk())
	{
		delete preview;
		wxLogError(wxT("There was a problem previewing.\nPerhaps your current printer is not set correctly?"));
		return;
	}

	wxPreviewFrame *frame =
		new wxPreviewFrame(preview, this, wxT("Picture Print Preview"), wxPoint(100, 100), wxSize(600, 650));
	frame->Centre(wxBOTH);
	frame->InitializeWithModality(m_previewModality);
	frame->Show();
}

void CViewerFrame::OnPageSetup(wxCommandEvent& WXUNUSED(event))
{
	wxPrintData * g_printData = CPrintEngine::GetPrintData();
	wxPageSetupDialogData * g_pageSetupData = CPrintEngine::GetPageSetupDialogData();
	(*g_pageSetupData) = *g_printData;

	wxPageSetupDialog pageSetupDialog(this, g_pageSetupData);
	pageSetupDialog.ShowModal();

	(*g_printData) = pageSetupDialog.GetPageSetupDialogData().GetPrintData();
	(*g_pageSetupData) = pageSetupDialog.GetPageSetupDialogData();
}

#ifdef __WXMAC__
void CViewerFrame::OnPageMargins(wxCommandEvent& WXUNUSED(event))
{
    wxPrintData * g_printData = CPrintEngine::GetPrintData();
    wxPageSetupDialogData * g_pageSetupData = CPrintEngine::GetPageSetupDialogData();
    (*g_pageSetupData) = *g_printData;

	wxMacPageMarginsDialog pageMarginsDialog(this, g_pageSetupData);
	pageMarginsDialog.ShowModal();

	(*g_printData) = pageMarginsDialog.GetPageSetupDialogData().GetPrintData();
	(*g_pageSetupData) = pageMarginsDialog.GetPageSetupDialogData();
}
#endif

wxBEGIN_EVENT_TABLE(CViewerFrame, wxFrame)
EVT_MENU(ID_Hello, CViewerFrame::OnHello)
EVT_MENU(wxID_HELP, CViewerFrame::OnHelp)
EVT_MENU(ID_Folder, CViewerFrame::OnSelectFolder)
EVT_MENU(ID_Configuration, CViewerFrame::OnConfiguration)
EVT_MENU(ID_OpenCL, CViewerFrame::OnConfigurationOpenCL)
EVT_MENU(ID_SIZEICONLESS, CViewerFrame::OnIconSizeLess)
EVT_MENU(ID_SIZEICONMORE, CViewerFrame::OnIconSizeMore)
EVT_MENU(wxID_ABOUT, CViewerFrame::OnAbout)
EVT_MENU(WXPRINT_PAGE_SETUP, CViewerFrame::OnPageSetup)
EVT_MENU(wxID_EXIT, CViewerFrame::OnExit)
#ifdef __WXMAC__
	EVT_MENU(WXPRINT_PAGE_MARGINS, CViewerFrame::OnPageMargins)
#endif
wxEND_EVENT_TABLE()