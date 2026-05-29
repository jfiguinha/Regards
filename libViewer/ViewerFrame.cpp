#include <header.h>
#include "ViewerFrame.h"
#include "WaitingWindow.h"
#include "MainWindow.h"
#include "PertinenceValue.h"
#include "ViewerParamInit.h"
#include "SQLRemoveData.h"
#include "MainThemeInit.h"
#include "ViewerParam.h"
#include "MainTheme.h"
#include "window_mode_id.h"
#include <LibResource.h>
#include <ConfigRegards.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <FileUtility.h>
#include <libPicture.h>
#include <SavePicture.h>
#include <ImageLoadingFormat.h>
#include <wx/filename.h>
#include <wx/display.h>
#include <wx/stdpaths.h>
#include <wx/busyinfo.h>

#ifdef __APPLE__
#include <ToggleFullscreen.h>
#endif

#ifndef wxHAS_IMAGES_IN_RESOURCES
#ifdef __WXGTK__
#include "../Resource/sample.xpm"
#else
#include "../../Resource/sample.xpm"
#endif
#endif

#if !wxUSE_PRINTING_ARCHITECTURE
#error "You must set wxUSE_PRINTING_ARCHITECTURE to 1 in setup.h, and recompile the library."
#endif

static constexpr int TIMER_EVENTFILEFS      = 3;
static constexpr int TIMER_LOADPICTURESTART = 5;

using namespace std;
using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::Picture;

bool CViewerFrame::viewerMode_ = true;

// ---------------------------------------------------------------------------
// static accessors
// ---------------------------------------------------------------------------

bool CViewerFrame::GetViewerMode()  { return viewerMode_; }
void CViewerFrame::SetViewerMode(const bool& mode) { viewerMode_ = mode; }

// ---------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------

CViewerFrame::CViewerFrame(const wxString& title, const wxPoint& pos, const wxSize& size,
                             IMainInterface* mainInterface, const wxString& openfile)
    : wxFrame(nullptr, FRAMEVIEWER_ID, title, pos, size,
              wxMAXIMIZE | wxFRAME_EX_METAL | wxDEFAULT_FRAME_STYLE)
    , mainInterface_(mainInterface)
    , fileToOpen_(openfile)
{
    SetIcon(wxICON(sample));

    mainInterface_->parent = this;

    // 1. Paramètres et thème
    InitParams();

    // 2. Vérification du modèle IA
    modelManager_ = std::make_unique<CModelManager>(this);
    if (!modelManager_->VerifyAndUpdate())
    {
        wxMessageBox(wxT("IA model not found. Program can't be started."),
                     wxT("Error"), wxICON_ERROR);
        mainInterface_->Close();
        return;
    }

    // 3. Fichier à ouvrir
    if (!wxFileExists(fileToOpen_))
        fileToOpen_.Clear();

    const bool openFirstFile = !fileToOpen_.IsEmpty();

    // 4. Fenêtre principale
    mainWindow_ = std::make_unique<CMainWindow>(this, MAINVIEWERWINDOWID, this, fileToOpen_);

    // 5. Services
    fileWatcherService_  = std::make_unique<CFileWatcherService>(this);
    printService_        = std::make_unique<CPrintService>(this);
    scannerLauncher_     = std::make_unique<CScannerLauncher>();
    navigationCtrl_      = std::make_unique<CViewerNavigationController>(this, mainWindow_.get());

    // 6. Timers propres à la frame
    InitTimers();

    // 7. Menu et binding
    InitMenuBar();
    BindEvents();

    mainInterface_->HideAbout();

    if (openFirstFile)
        loadPictureStartTimer_->Start(10, true);

    Maximize();
}

CViewerFrame::~CViewerFrame()
{
    if (mainWindow_)
        mainWindow_->SaveParameter();

    if (eventFileSysTimer_ && eventFileSysTimer_->IsRunning())
        eventFileSysTimer_->Stop();

    if (loadPictureStartTimer_ && loadPictureStartTimer_->IsRunning())
        loadPictureStartTimer_->Stop();
        
    if (exitTimer && exitTimer->IsRunning())
        exitTimer->Stop();

    viewerParam_->SaveFile();
    
	if (!onExit)
		Exit();
}

// ---------------------------------------------------------------------------
// Initialisation helpers
// ---------------------------------------------------------------------------

void CViewerFrame::InitParams()
{
    viewerParam_ = std::make_unique<CMainParam>();
    viewerTheme_ = std::make_unique<CMainTheme>();
    CMainParamInit::Initialize(viewerParam_.get());
    CMainThemeInit::Initialize(viewerTheme_.get());
}

void CViewerFrame::InitTimers()
{
    loadPictureStartTimer_ = std::make_unique<wxTimer>(this, TIMER_LOADPICTURESTART);
    eventFileSysTimer_     = std::make_unique<wxTimer>(this, TIMER_EVENTFILEFS);
    exitTimer = std::make_unique<wxTimer>(this, wxTIMER_EXIT);
}

void CViewerFrame::InitMenuBar()
{
    const wxString labelDecreaseIconSize      = CLibResource::LoadStringFromResource(L"labelDecreaseIconSize", 1);
    const wxString labelDecreaseIconSize_link = CLibResource::LoadStringFromResource(L"labelDecreaseIconSize_link", 1);
    const wxString labelEnlargeIconSize       = CLibResource::LoadStringFromResource(L"labelEnlargeIconSize", 1);
    const wxString labelEnlargeIconSize_link  = CLibResource::LoadStringFromResource(L"labelEnlargeIconSize_link", 1);
    const wxString labelConfiguration        = CLibResource::LoadStringFromResource(L"labelConfiguration", 1);
    const wxString labelConfiguration_link   = CLibResource::LoadStringFromResource(L"labelConfiguration_link", 1);
    const wxString labelFile                  = CLibResource::LoadStringFromResource(L"labelFile", 1);
    const wxString labelSizeIcon              = CLibResource::LoadStringFromResource(L"labelSizeIcon", 1);
    const wxString labelHelp                  = CLibResource::LoadStringFromResource(L"labelHelp", 1);
    const wxString labelPageSetup             = CLibResource::LoadStringFromResource(L"labelPageSetup", 1);
    const wxString labelPageSetup_link        = CLibResource::LoadStringFromResource(L"labelPageSetup_link", 1);
    const wxString export_diaporama           = CLibResource::LoadStringFromResource(L"LBLEXPORTDIAPORAMA", 1);
    const wxString lblScanner                 = CLibResource::LoadStringFromResource(L"LBLSCANNER", 1);

    auto* menuFile = new wxMenu();
#ifdef WIN32
    menuFile->Append(ID_ASSOCIATE, "&Associate", "Associate");
    menuFile->AppendSeparator();
#endif
    menuFile->Append(WXPRINT_PAGE_SETUP, labelPageSetup_link, labelPageSetup);
#ifdef __WXMAC__
    menuFile->Append(WXPRINT_PAGE_MARGINS, labelPageMargins_link, labelPageMargins);
#endif
    menuFile->Append(wxID_PRINT, wxT("&Print..."), wxT("Print"));
    menuFile->Append(ID_SCANNER, lblScanner, lblScanner);
    menuFile->AppendSeparator();
    menuFile->Append(ID_Configuration, labelConfiguration_link, labelConfiguration);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    auto* menuSizeIcon = new wxMenu();
    menuSizeIcon->Append(ID_SIZEICONLESS, labelDecreaseIconSize_link, labelDecreaseIconSize);
    menuSizeIcon->Append(ID_SIZEICONMORE, labelEnlargeIconSize_link, labelEnlargeIconSize);

    auto* menuTools = new wxMenu();
    menuTools->Append(ID_DIAPORAMA, export_diaporama, export_diaporama);

    auto* menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT);
    menuHelp->Append(wxID_HELP);

    auto* menuBar = new wxMenuBar();
    menuBar->Append(menuFile,     labelFile);
    menuBar->Append(menuSizeIcon, labelSizeIcon);
    menuBar->Append(menuTools,    "Tools");
    menuBar->Append(menuHelp,     labelHelp);

    wxFrameBase::SetMenuBar(menuBar);
    wxWindow::SetLabel(wxT("Regards Viewer"));
}

void CViewerFrame::BindEvents()
{
    Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(CViewerFrame::OnClose));
	Connect(wxEVENT_CLOSESCANNER, wxCommandEventHandler(CViewerFrame::HideScanner));
	Connect(wxEVENT_PICTUREENDLOADING, wxCommandEventHandler(CViewerFrame::OnPictureEndLoading));
	Connect(wxID_PRINT, wxEVT_MENU, wxCommandEventHandler(CViewerFrame::OnPrint));
	Connect(ID_EXPORT, wxEVT_MENU, wxCommandEventHandler(CViewerFrame::OnExport));
#ifdef WIN32
	Connect(ID_ASSOCIATE, wxEVT_MENU, wxCommandEventHandler(CViewerFrame::OnAssociate));
#endif
	Connect(ID_SCANNER, wxEVT_MENU, wxCommandEventHandler(CViewerFrame::OnScanner));
    Connect(wxID_EDIT, wxEVT_MENU, wxCommandEventHandler(CViewerFrame::OnEdit));
    Connect(ID_DIAPORAMA, wxEVT_MENU, wxCommandEventHandler(CViewerFrame::OnExportDiaporama));  
	Connect(wxEVT_FULLSCREEN,  wxCommandEventHandler(CViewerFrame::OnWindowFullScreen));
    
	Connect(TIMER_EVENTFILEFS, wxEVT_TIMER, wxTimerEventHandler(CViewerFrame::OnTimereventFileSysTimer), nullptr, this);
    Connect(TIMER_LOADPICTURESTART, wxEVT_TIMER, wxTimerEventHandler(CViewerFrame::OnOpenFile), nullptr, this);
    Connect(wxTIMER_EXIT, wxEVT_TIMER, wxTimerEventHandler(CViewerFrame::CheckAllProcessEnd), nullptr, this);
    
	mainWindow_->Bind(wxEVT_CHAR_HOOK, &CViewerFrame::OnKeyDown, this);
	mainWindow_->Bind(wxEVT_KEY_UP, &CViewerFrame::OnKeyUp, this);
       
}   

// ---------------------------------------------------------------------------
// IStatusBarInterface
// ---------------------------------------------------------------------------

void CViewerFrame::SetText(const int& numPos, const wxString& libelle)
{
    if (mainWindow_)
        mainWindow_->SetText(numPos, libelle);
}

void CViewerFrame::SetRangeProgressBar(const int& range)
{
    if (mainWindow_)
        mainWindow_->SetRangeProgressBar(range);
}

void CViewerFrame::SetPosProgressBar(const int& position)
{
    if (mainWindow_)
        mainWindow_->SetPosProgressBar(position);
}

void CViewerFrame::SetWindowTitle(const wxString& libelle)
{
    SetLabel(libelle);
}

void CViewerFrame::SetFullscreen()
{
    if (!mainWindow_->SetFullscreenMode())
        return;

    fullscreen_ = true;
#ifdef __APPLE__
    CToggleScreen toggle;
    toggle.ToggleFullscreen(this);
#else
    this->ShowFullScreen(true);
#endif
}

void CViewerFrame::SetScreen()
{
    fullscreen_ = false;
#ifdef __APPLE__
    CToggleScreen toggle;
    toggle.ToggleFullscreen(this);
#else
    this->ShowFullScreen(false);
#endif
}

void CViewerFrame::PrintPreview(CImageLoadingFormat* imageToPrint)
{
    printService_->ShowMatrixPreview(imageToPrint);
}

void CViewerFrame::PrintImagePreview(CImageLoadingFormat* imageToPrint)
{
    printService_->ShowImagePreview(imageToPrint);
}

void CViewerFrame::CheckAllProcessEnd(wxTimerEvent& event)
{
	nbTime++;

    //printf("Get Nb Main Windows %d \n", CMasterWindow::listProcessWindow.size());
    

	if (nbTime < 50)
	{
		for (CMasterWindow* window : CMasterWindow::listProcessWindow)
		{
			if (window != nullptr)
			{
                //printf("CheckAllProcessEnd %s \n", window->GetWaitingMessage().ToStdString().c_str());
                
				if (!window->GetProcessEnd())
				{
                    wxTheApp->Yield();
                    
                    //printf("CheckAllProcessEnd %s not end \n", window->GetWaitingMessage().ToStdString().c_str());
                    
					const wxString message = window->GetWaitingMessage();
					mainWindowWaiting->SetTexte(message);
					exitTimer->Start(1000, wxTIMER_ONE_SHOT);
					return;
				}
			}
		}
	}

	onExit = true;
	Exit();
}

void CViewerFrame::Exit()
{
	if (!onExit)
	{
		nbTime = 0;
		CWindowMain::SetEndProgram();
        if(eventFileSysTimer_ != nullptr)
            eventFileSysTimer_->Stop();
            
        if(loadPictureStartTimer_ != nullptr)
            loadPictureStartTimer_->Stop();
            
        if(mainWindow_ != nullptr)
            mainWindow_->Show(false);
        
		mainWindowWaiting = std::make_unique<CWaitingWindow>(this, wxID_ANY);
        if(mainWindowWaiting != nullptr)
        {
            mainWindowWaiting->Show(true);
            mainWindowWaiting->SetSize(0, 0, mainWindow_->GetWindowWidth(), mainWindow_->GetWindowHeight());
            mainWindowWaiting->Refresh();      
        }
        
        if(exitTimer != nullptr)
            exitTimer->Start(10, wxTIMER_ONE_SHOT);
	}
	else
	{
		CMainThemeInit::SaveTheme();
        if(mainInterface_ != nullptr)
            mainInterface_->Close();
		onExit = true;
	}
}

bool CViewerFrame::AddFSEntry(const wxString& dirPath)
{
    return fileWatcherService_->AddPath(dirPath);
}

bool CViewerFrame::RemoveFSEntry(const wxString& dirPath)
{
    return fileWatcherService_->RemovePath(dirPath);
}

int CViewerFrame::ShowScanner()
{
    return scannerLauncher_->Launch();
}

void CViewerFrame::CreateWatcherIfNecessary()
{
    fileWatcherService_->EnsureStarted();
}

// ---------------------------------------------------------------------------
// Navigation / affichage
// ---------------------------------------------------------------------------

void CViewerFrame::PostWindowModeEvent(int modeId)
{
    wxWindow* central = FindWindowById(CENTRALVIEWERWINDOWID);
    if (central)
    {
        wxCommandEvent evt(wxEVENT_SETMODEVIEWER);
        evt.SetInt(modeId);
        wxPostEvent(central, evt);
    }
}

void CViewerFrame::OnWindowFace(wxCommandEvent&)    { PostWindowModeEvent(WINDOW_FACE);     }
void CViewerFrame::OnWindowFolder(wxCommandEvent&)  { PostWindowModeEvent(WINDOW_EXPLORER); }
void CViewerFrame::OnWindowViewer(wxCommandEvent&)  { PostWindowModeEvent(WINDOW_VIEWER);   }
void CViewerFrame::OnWindowPicture(wxCommandEvent&) { PostWindowModeEvent(WINDOW_PICTURE);  }

void CViewerFrame::OnWindowFullScreen(wxCommandEvent&)
{
    if (!fullscreen_)
        SetFullscreen();
}

// ---------------------------------------------------------------------------
// Keyboard — délégué au NavigationController
// ---------------------------------------------------------------------------

void CViewerFrame::OnKeyDown(wxKeyEvent& event)
{
    navigationCtrl_->OnKeyDown(event);
    navigationCtrl_->SetFullscreen(fullscreen_);
}

void CViewerFrame::OnKeyUp(wxKeyEvent& event)
{
    navigationCtrl_->OnKeyUp(event);
}

void CViewerFrame::OnPictureEndLoading(wxCommandEvent&)
{
    navigationCtrl_->OnPictureEndLoading();
}

// ---------------------------------------------------------------------------
// Timers
// ---------------------------------------------------------------------------

void CViewerFrame::OnOpenFile(wxTimerEvent&)
{
    OpenPictureFile();
}

void CViewerFrame::OnTimerLoadPicture(wxTimerEvent&)
{
    // Délégué au NavigationController (timer interne)
}

void CViewerFrame::OnTimerEndLoadPicture(wxTimerEvent&)
{
    navigationCtrl_->OnPictureEndLoading();
}

void CViewerFrame::OnTimereventFileSysTimer(wxTimerEvent&)
{
    if (mainWindow_)
    {
        wxCommandEvent evt(wxEVENT_REFRESHFOLDER);
        mainWindow_->GetEventHandler()->AddPendingEvent(evt);
    }
    eventFileSysTimer_->Stop();
}

// ---------------------------------------------------------------------------
// Filesystem watcher
// ---------------------------------------------------------------------------

void CViewerFrame::OnFileSystemModified(wxFileSystemWatcherEvent&)
{
    if (!eventFileSysTimer_)
        return;

    eventFileSysTimer_->Stop();
    if (mainWindow_)
        eventFileSysTimer_->Start(1000);
}

// ---------------------------------------------------------------------------
// Handlers de menu
// ---------------------------------------------------------------------------

void CViewerFrame::OnClose(wxCloseEvent&)
{
    Exit();
}

void CViewerFrame::OnExit(wxCommandEvent&)
{
    Exit();
}

void CViewerFrame::OnAbout(wxCommandEvent&)
{
    mainInterface_->ShowAbout();
}

void CViewerFrame::OnHello(wxCommandEvent&)
{
    wxLogMessage("Hello world from wxWidgets!");
}

void CViewerFrame::OnHelp(wxCommandEvent&)
{
    wxString helpFile = CFileUtility::GetResourcesFolderPath();
    helpFile.Append(wxFILE_SEP_PATH + "NoticeRegardsViewer.pdf");
    wxLaunchDefaultApplication(helpFile);
}

void CViewerFrame::OnPrint(wxCommandEvent&)
{
    if (mainWindow_)
        printService_->PrintFile(mainWindow_->GetFilename());
}

void CViewerFrame::OnPageSetup(wxCommandEvent&)
{
    printService_->ShowPageSetup();
}

#ifdef __WXMAC__
void CViewerFrame::OnPageMargins(wxCommandEvent&)
{
    printService_->ShowPageMargins();
}
#endif

void CViewerFrame::OnScanner(wxCommandEvent&)
{
    ShowScanner();
}

void CViewerFrame::HideScanner(wxCommandEvent&)
{
    // Géré par le processus externe — rien à faire côté frame
}

void CViewerFrame::OnExport(wxCommandEvent&)
{
    if (!mainWindow_)
        return;
    const wxString filename = mainWindow_->GetFilename();
    if (!filename.IsEmpty())
        CSavePicture::SavePicture(nullptr, nullptr, filename);
}

#ifdef WIN32
void CViewerFrame::OnAssociate(wxCommandEvent&)
{
    const wxString path = CFileUtility::GetProgramFolderPath() + "\\associate.exe";
    ShellExecute(GetHWND(), L"runas", path, nullptr, nullptr, SW_SHOWNORMAL);
}
#endif

void CViewerFrame::OnConfiguration(wxCommandEvent&)
{
    auto* regardsParam = CParamInit::getInstance();
    if (!regardsParam)
        return;

    const int pictureSizeBefore = regardsParam->GetFaceDetectionPictureSize();
    ConfigRegards configFile(this);
    configFile.ShowModal();

    if (configFile.IsOk())
    {
        if (pictureSizeBefore != regardsParam->GetFaceDetectionPictureSize())
        {
            CSQLRemoveData::DeleteFaceDatabase();
            if (mainWindow_)
            {
                wxCommandEvent evt(wxEVENT_REFRESHFOLDERLIST);
                mainWindow_->GetEventHandler()->AddPendingEvent(evt);
            }
            wxWindow* criteria = FindWindowById(CRITERIAFOLDERWINDOWID);
            if (criteria)
            {
                wxCommandEvent evt(wxEVENT_UPDATECRITERIA);
                evt.SetExtraLong(0);
                criteria->GetEventHandler()->AddPendingEvent(evt);
            }
        }
    }
}

void CViewerFrame::OnEraseDatabase(wxCommandEvent&)
{
    const wxString msg  = CLibResource::LoadStringFromResource(L"EraseDatabase", 1);
    const wxString info = CLibResource::LoadStringFromResource(L"labelInformations", 1);
    if (wxMessageBox(msg, info, wxYES_NO | wxICON_WARNING) == wxYES)
    {
        CSQLRemoveData::DeleteCatalog(1);
        if (mainWindow_)
        {
            wxCommandEvent evt(wxEVENT_REFRESHFOLDER);
            mainWindow_->GetEventHandler()->AddPendingEvent(evt);
        }
    }
}

void CViewerFrame::OnIconSizeLess(wxCommandEvent&)
{
    auto* config = CParamInit::getInstance();
    if (!config)
        return;

    float ratio = config->GetIconSizeRatio();
    if (ratio > 1.0f)
        config->SetIconSizeRatio(ratio - 0.25f);
    else
        config->SetIconSizeRatio(ratio);

    mainWindow_->UpdateScreenRatio();
}

void CViewerFrame::OnIconSizeMore(wxCommandEvent&)
{
    auto* config = CParamInit::getInstance();
    if (!config)
        return;

    float ratio = config->GetIconSizeRatio();
    if (ratio < 2.0f)
        config->SetIconSizeRatio(ratio + 0.25f);
    else
        config->SetIconSizeRatio(ratio);

    mainWindow_->UpdateScreenRatio();
}

void CViewerFrame::OnFacePertinence(wxCommandEvent&)
{
    auto* param = CMainParamInit::getInstance();
    if (!param)
        return;

    PertinenceValue dialog(this);
    dialog.SetValue(param->GetPertinenceValue());
    dialog.ShowModal();
    if (dialog.IsOk())
    {
        param->SetPertinenceValue(dialog.GetValue());
        if (mainWindow_)
        {
            wxCommandEvent evt(wxEVENT_REFRESHFOLDERLIST);
            mainWindow_->GetEventHandler()->AddPendingEvent(evt);
        }
    }
}

void CViewerFrame::OnEdit(wxCommandEvent&)
{
    wxWindow* central = FindWindowById(MAINVIEWERWINDOWID);
    if (central)
        wxQueueEvent(central, new wxCommandEvent(wxEVENT_EDITFILE));
}

void CViewerFrame::OnExportDiaporama(wxCommandEvent&)
{
    wxWindow* central = FindWindowById(MAINVIEWERWINDOWID);
    if (central)
        wxQueueEvent(central, new wxCommandEvent(wxEVENT_EXPORTDIAPORAMA));
}

// ---------------------------------------------------------------------------
// Ouverture fichier initial
// ---------------------------------------------------------------------------
void CViewerFrame::OpenPictureFile()
{
    CLibPicture libPicture;
    wxString dirpath;

    if (fileToOpen_.IsEmpty())
    {
        dirpath = wxStandardPaths::Get()
                      .GetUserDir(wxStandardPaths::Dir_Pictures);

        wxDir dir(dirpath);

        wxString bestImage;

        std::function<void(const wxString&)> scanDirectory;
        scanDirectory = [&](const wxString& path)
        {
            wxDir localDir(path);

            if (!localDir.IsOpened())
                return;

            wxString filename;
            bool cont = localDir.GetFirst(&filename);

            while (cont)
            {
                wxString fullPath =
                    path + wxFILE_SEP_PATH + filename;

                if (wxDirExists(fullPath))
                {
                    scanDirectory(fullPath);
                }
                else
                {
                    // filtre rapide par extension
                    wxFileName fn(fullPath);

                    if (libPicture.TestImageFormat(fullPath) != 0)
                    {
                        if (bestImage.IsEmpty() ||
                            fullPath.CmpNoCase(bestImage) < 0)
                        {
                            bestImage = fullPath;
                        }
                    }
                    
                }

                cont = localDir.GetNext(&filename);
            }
        };

        scanDirectory(dirpath);

        fileToOpen_ = bestImage;
    }

    if (!fileToOpen_.IsEmpty())
    {
        auto file = new wxString(fileToOpen_);

        wxCommandEvent evt(wxEVENT_OPENFILEORFOLDER);
        evt.SetInt(1);
        evt.SetClientData(file);

        mainWindow_->GetEventHandler()
                  ->AddPendingEvent(evt);

        if (dirpath.IsEmpty())
            mainWindow_->SetPictureMode();
        else
            mainWindow_->SetViewerMode();
    }
}

// ---------------------------------------------------------------------------
// Event table (uniquement pour les entrées menu non bindées via Bind())
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(CViewerFrame, wxFrame)
    EVT_MENU(ID_Hello,         CViewerFrame::OnHello)
    EVT_MENU(wxID_HELP,        CViewerFrame::OnHelp)
    EVT_MENU(ID_Configuration, CViewerFrame::OnConfiguration)
    EVT_MENU(ID_SIZEICONLESS,  CViewerFrame::OnIconSizeLess)
    EVT_MENU(ID_SIZEICONMORE,  CViewerFrame::OnIconSizeMore)
    EVT_MENU(ID_ERASEDATABASE, CViewerFrame::OnEraseDatabase)
    EVT_MENU(ID_WINDOWFACE,    CViewerFrame::OnWindowFace)
    EVT_MENU(ID_WINDOWFOLDER,  CViewerFrame::OnWindowFolder)
    EVT_MENU(ID_WINDOWVIEWER,  CViewerFrame::OnWindowViewer)
    EVT_MENU(ID_WINDOWPICTURE, CViewerFrame::OnWindowPicture)
    EVT_MENU(wxID_ABOUT,       CViewerFrame::OnAbout)
    EVT_MENU(WXPRINT_PAGE_SETUP, CViewerFrame::OnPageSetup)
    EVT_MENU(wxID_EXIT,        CViewerFrame::OnExit)
#ifdef __WXMAC__
    EVT_MENU(WXPRINT_PAGE_MARGINS, CViewerFrame::OnPageMargins)
#endif
wxEND_EVENT_TABLE()
