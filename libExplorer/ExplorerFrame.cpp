#include "ExplorerFrame.h"
#include "MainWindow.h"
#include <BitmapPrintout.h>
#include <PrintEngine.h>
#include <ConfigRegards.h>
#include <OpenCLDialog.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include "ExplorerParam.h"
#include "ExplorerParamInit.h"
#include "ExplorerTheme.h"
#include "ExplorerThemeInit.h"
#include <RegardsConfigParam.h>
#include <FileUtility.h>
#include <ParamInit.h>
using namespace std;
using namespace Regards::Print;
using namespace Regards::Control;
using namespace Regards::Explorer;

//const LONG ONE_MSEC = 1000;
// constants:
enum
{
	ID_Hello = 1,
	ID_Configuration = 3,
	ID_OpenCL = 4,
    ID_SIZEICON = 2,
    ID_SIZEICONLESS = 5,
    ID_SIZEICONMORE = 6,
	WXPRINT_PAGE_SETUP,
	WXPRINT_PAGE_SETUP_PS,

#ifdef __WXMAC__
	WXPRINT_PAGE_MARGINS,
#endif

};

#if !wxUSE_PRINTING_ARCHITECTURE
#error "You must set wxUSE_PRINTING_ARCHITECTURE to 1 in setup.h, and recompile the library."
#endif

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

CExplorerFrame::CExplorerFrame(const wxString& title, const wxPoint& pos, const wxSize& size, IMainInterface * mainInterface)
	: wxFrame(nullptr, wxID_ANY, title, pos, size, wxMAXIMIZE | wxDEFAULT_FRAME_STYLE)
{

	SetIcon(wxICON(sample));

	explorerParam = new CExplorerParam();
	CExplorerParamInit::Initialize(explorerParam);

	explorerTheme = new CExplorerTheme();
	CExplorerThemeInit::Initialize(explorerTheme);

	this->mainInterface = mainInterface;

	mainWindow = new CMainWindow(this, MAINEXPLORERWINDOWID, this, mainInterface);

	//mainWindow->Bind(wxEVT_CHAR_HOOK, &CExplorerFrame::OnKeyDown, this);

    wxMenu *menuSizeIcon = new wxMenu;
    menuSizeIcon->Append(ID_SIZEICONLESS, "&Smallest", "Smallest");
    menuSizeIcon->Append(ID_SIZEICONMORE, "&Biger", "Biger");
    
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_Configuration, "&Configuration", "Configuration");
	menuFile->Append(ID_OpenCL, "&OpenCL Configuration", "Configuration OpenCL");
	menuFile->Append(WXPRINT_PAGE_SETUP, wxT("Page Set&up..."), wxT("Page setup"));
#ifdef __WXMAC__
	menuFile->Append(WXPRINT_PAGE_MARGINS, wxT("Page Margins..."), wxT("Page margins"));
#endif
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_HELP);
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
    menuBar->Append(menuSizeIcon, "&Icon Size");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);

	SetLabel(wxT("Regards Explorer"));

	mainInterface->HideAbout();
}

void CExplorerFrame::OnKeyDown(wxKeyEvent& event)
{
	//wxMessageBox(wxString::Format("KeyDown: %i\n", (int)event.GetKeyCode()));
	event.Skip();
}

void CExplorerFrame::ShowViewer()
{
	mainInterface->ShowViewer();
}

void CExplorerFrame::Exit()
{
	this->Close();
}

CExplorerFrame::~CExplorerFrame()
{
	explorerParam->SaveFile();

	if (mainWindow != nullptr)
		delete(mainWindow);

	mainInterface->Close();
}


void CExplorerFrame::SetText(const int &numPos, const wxString &libelle)
{
	if (mainWindow != nullptr)
	{
		mainWindow->SetText(numPos, libelle);
	}
}

void CExplorerFrame::SetRangeProgressBar(const int &range)
{
	if (mainWindow != nullptr)
		mainWindow->SetRangeProgressBar(range);
}

void CExplorerFrame::SetPosProgressBar(const int &position)
{
	if (mainWindow != nullptr)
		mainWindow->SetPosProgressBar(position);
}

void CExplorerFrame::OnConfiguration(wxCommandEvent& event)
{
	ConfigRegards configFile(this);
	configFile.ShowModal();
}

void CExplorerFrame::OnConfigurationOpenCL(wxCommandEvent& event)
{
	OpenCLDialog openCLConfig(this);
	openCLConfig.ShowModal();
	if (openCLConfig.IsOk())
	{
		int platformIndex = openCLConfig.GetIndexItem();
		wxString platform = openCLConfig.GetSelectItem();

		CRegardsConfigParam * regardsParam = CParamInit::getInstance();
		regardsParam->SetOpenCLPlatformName(platform);
		regardsParam->SetOpenCLPlatformIndex(platformIndex);
	}
}

void CExplorerFrame::SetWindowTitle(const wxString &libelle)
{
	SetLabel(libelle);
}

void CExplorerFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void CExplorerFrame::OnAbout(wxCommandEvent& event)
{
	mainInterface->ShowAbout();
}

void CExplorerFrame::OnHelp(wxCommandEvent& event)
{
    wxString helpFile = CFileUtility::GetResourcesFolderPath();
    helpFile.Append("//NoticeRegardsExplorer.pdf");
    wxLaunchDefaultApplication(helpFile);
}

void CExplorerFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}

void CExplorerFrame::PrintPreview(CRegardsBitmap * imageToPrint)
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

void CExplorerFrame::OnIconSizeLess(wxCommandEvent& event)
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

void CExplorerFrame::OnIconSizeMore(wxCommandEvent& event)
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

void CExplorerFrame::OnPageSetup(wxCommandEvent& WXUNUSED(event))
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
void CExplorerFrame::OnPageMargins(wxCommandEvent& WXUNUSED(event))
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

wxBEGIN_EVENT_TABLE(CExplorerFrame, wxFrame)
	EVT_MENU(ID_Hello, CExplorerFrame::OnHello)
	EVT_MENU(ID_Configuration, CExplorerFrame::OnConfiguration)
	EVT_MENU(ID_OpenCL, CExplorerFrame::OnConfigurationOpenCL)
    EVT_MENU(ID_SIZEICONLESS, CExplorerFrame::OnIconSizeLess)
    EVT_MENU(ID_SIZEICONMORE, CExplorerFrame::OnIconSizeMore)
	EVT_MENU(wxID_ABOUT, CExplorerFrame::OnAbout)
    EVT_MENU(wxID_HELP, CExplorerFrame::OnHelp)
	EVT_MENU(WXPRINT_PAGE_SETUP, CExplorerFrame::OnPageSetup)
	EVT_MENU(wxID_EXIT, CExplorerFrame::OnExit)
	#ifdef __WXMAC__
	EVT_MENU(WXPRINT_PAGE_MARGINS, CExplorerFrame::OnPageMargins)
	#endif
wxEND_EVENT_TABLE()