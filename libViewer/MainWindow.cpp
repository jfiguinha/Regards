#include <header.h>
#include "MainWindow.h"
#include <LibResource.h>
#include "ExportDiaporama.h"
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <libPicture.h>
#include "window_mode_id.h"
#include <ImageLoadingFormat.h>
#include <SqlFaceRecognition.h>
#include "ThumbnailBuffer.h"
#include "SqlFindPhotos.h"
#include <BitmapWndViewer.h>
#include "Toolbar.h"
#include "ToolbarViewerMode.h"
#include <StatusBarInterface.h>
#include "CentralWindow.h"
#include "FileUtility.h"
#include <wx/filename.h>
#include <window_id.h>
#include <SqlFindFolderCatalog.h>
#include "StatusText.h"
#include <ThumbnailMessage.h>
#define LIBHEIC
#include <ShowElement.h>
#include <wx/filedlg.h>
#include <SqlFaceLabel.h>
#include "SqlFacePhoto.h"
#include <FiltreEffetCPU.h>
#include "CheckVersion.h"
#include <MainTheme.h>
#include <MainThemeInit.h>
#include <ParamInit.h>
#include "FolderProcess.h"
#include <wx/mimetype.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>

using namespace Regards::Picture;
using namespace Regards::Control;
using namespace Regards::Viewer;
using namespace std;
using namespace Regards::Sqlite;

bool firstTime = true;
constexpr auto TIMER_LOADPICTURESTART = 0x10005;
extern wxImage defaultPicture;

// ═════════════════════════════════════════════════════════════════════════════
// Construction / destruction
// ═════════════════════════════════════════════════════════════════════════════

CMainWindow::CMainWindow(wxWindow* parent,
                         wxWindowID id,
                         IStatusBarInterface* statusbar,
                         const wxString& fileToOpen)
    : CWindowMain("CMainWindow", parent, id)
{
    InitState();
    InitTheme();
    InitUI(statusbar);
    BindEvents();
    InitConfig(fileToOpen);
    InitBackgroundTasks();
}

CMainWindow::~CMainWindow()
{
    if (loadPictureStartTimer->IsRunning())
        loadPictureStartTimer->Stop();


    delete loadPictureStartTimer;
}

// ═════════════════════════════════════════════════════════════════════════════
// Initialisation
// ═════════════════════════════════════════════════════════════════════════════

void CMainWindow::InitState()
{
    multithread     = true;
    needToReload    = false;
    typeAffichage   = THUMB_SHOW_ALL;
    updateCriteria  = true;
    refreshFolder   = false;
    start           = true;
    criteriaSendMessage = false;
    checkVersion    = true;

    setViewerMode   = false;
    setPictureMode  = false;
    nbElementInIconeList = 0;
    nbPhotoElement  = 0;
    nbElement       = 0;
    isCheckingFile  = false;
    isCheckNewVersion = false;
    faceDetection   = 0;

    folderProcess    = std::make_unique<CFolderProcess>(this);
    thumbnailProcess = std::make_unique<CThumbnailProcess>(this);
    viewerParam      = CMainParamInit::getInstance();
}

void CMainWindow::InitTheme()
{
    CMainTheme* viewerTheme = CMainThemeInit::getInstance();

    if (viewerTheme != nullptr)
    {
        {
            CThemeToolbar themeInfos;
            viewerTheme->GetInfosToolbarTheme(&themeInfos);
            themeInfos.position = NAVIGATOR_CENTER;

            toolbarViewerMode = new CToolbarViewerMode(
                this, wxID_ANY, themeInfos, this, false);
        }

        {
            CThemeSplitter theme;
            viewerTheme->GetSplitterTheme(&theme);

            centralWnd = new CCentralWindow(
                this, CENTRALVIEWERWINDOWID, theme, false);
        }
    }

}

void CMainWindow::InitUI(IStatusBarInterface* statusbar)
{
    statusBarViewer = statusbar;

    loadPictureStartTimer = new wxTimer(this, TIMER_LOADPICTURESTART);

    statusBar = new wxStatusBar(
        this, wxID_ANY, wxSTB_DEFAULT_STYLE, "wxStatusBar");

    int tabWidth[] = {100, 300, 300, 300};
    statusBar->SetFieldsCount(4);
    statusBar->SetStatusWidths(4, tabWidth);

    progressBar = new wxGauge(
        statusBar, wxID_ANY, 200,
        wxPoint(1000, 0),
        wxSize(200, statusBar->GetSize().y),
        wxGA_HORIZONTAL);

    progressBar->SetRange(100);
    progressBar->SetValue(50);

    // ── Création des services ────────────────────────────────────────────
    scheduler    = std::make_unique<ThumbnailScheduler>(this, thumbnailProcess.get());
    folderService = std::make_unique<FolderRefreshService>(centralWnd, this, faceDetection);
    viewerCtrl   = std::make_unique<CMainViewerController>(
        centralWnd, toolbarViewerMode,
        statusBar, progressBar,
        statusBarViewer, this);
}
/*
void CMainWindow::BindEvents()
{
    Bind(wxEVT_MENU,
        &CMainWindow::OnShowToolbar,
        this,
        TOOLBAR_UPDATE_ID);
    Bind(wxEVT_TIMER,
        &CMainWindow::OnOpenFile,
        this,
        TIMER_LOADPICTURESTART);

    //Bind<wxCommandEvent>(wxEVENT_FACEINFOSUPDATE, &CMainWindow::OnFaceInfosUpdate, this);

    Bind(wxEVENT_SETSCREEN, &CMainWindow::SetScreenEvent, this);
    Bind(wxEVENT_INFOS, &CMainWindow::OnUpdateInfos, this);
    Bind(wxEVENT_REFRESHFOLDERLIST, &CMainWindow::RefreshFolderList, this);
    Bind(wxEVT_MENU, &CMainWindow::OnShowToolbar, this, TOOLBAR_UPDATE_ID);
    Bind(wxEVENT_ENDCHECKFILE, &CMainWindow::OnEndCheckFile, this);
    Bind(wxEVENT_UPDATEFOLDER, &CMainWindow::OnUpdateFolder, this);
    Bind(wxEVENT_ONPICTURECLICK, &CMainWindow::OnPictureClick, this);
    Bind(wxEVT_CRITERIACHANGE, &CMainWindow::CriteriaChange, this);
    Bind(wxEVENT_PICTUREVIDEOCLICK, &CMainWindow::PictureVideoClick, this);
    Bind(wxEVENT_REFRESHFOLDER, &CMainWindow::InitPictures, this);
    Bind(wxEVENT_REFRESHPICTURE, &CMainWindow::OnRefreshPicture, this);
    Bind(wxEVENT_SETSTATUSTEXT, &CMainWindow::OnStatusSetText, this);
    Bind(wxEVT_EXIT, &CMainWindow::OnExit, this);
    Bind(wxEVENT_SETRANGEPROGRESSBAR, &CMainWindow::OnSetRangeProgressBar, this);
    Bind(wxEVENT_PRINTPICTURE, &CMainWindow::PrintPreview, this);
    Bind(wxEVENT_CRITERIAPHOTOUPDATE, &CMainWindow::OnCriteriaUpdate, this);
    Bind(wxEVENT_UPDATESTATUSBARMESSAGE, &CMainWindow::UpdateStatusBarMessage, this);
    Bind(wxEVENT_FACEADD, &CMainWindow::OnFaceAdd, this);
    Bind(wxEVENT_PRINT, &CMainWindow::OnPrint, this);
    Bind(wxEVENT_SETVALUEPROGRESSBAR, &CMainWindow::OnSetValueProgressBar, this);
    Bind(wxEVENT_SHOWSCANNER, &CMainWindow::OnScanner, this);
    Bind(wxEVENT_OPENFILEORFOLDER, &CMainWindow::OnOpenFileOrFolder, this);
    Bind(wxEVENT_EDITFILE, &CMainWindow::OnEditFile, this);
    Bind(wxEVENT_EXPORTFILE, &CMainWindow::OnExportFile, this);
    Bind(wxEVENT_UPDATETHUMBNAILEXIF, &CMainWindow::OnUpdateExifThumbnail, this);
    Bind(wxEVENT_EXPORTDIAPORAMA, &CMainWindow::OnExportDiaporama, this);
    Bind(wxEVENT_DELETEFACE, &CMainWindow::OnDeleteFace, this);
    Bind(wxEVENT_ICONEUPDATE, &CMainWindow::UpdateThumbnailIcone, this);
    Bind(wxVERSION_UPDATE_EVENT, &CMainWindow::OnVersionUpdate, this);
    Bind(wxEVENT_UPDATEMESSAGE, &CMainWindow::UpdateMessage, this);
    Bind(wxEVENT_REFRESHTHUMBNAIL, &CMainWindow::OnRefreshThumbnail, this);
    Bind(wxEVENT_ICONETHUMBNAILGENERATION, &CMainWindow::OnProcessThumbnail, this);
    Bind(wxEVENT_UPDATECHECKINSTATUS, &CMainWindow::OnCheckInUpdateStatus, this);
    Bind(wxEVENT_UPDATECHECKINFOLDER, &CMainWindow::OnRemoveFileFromCheckIn, this);

    Connect(TIMER_LOADPICTURESTART, wxEVT_TIMER,
        wxTimerEventHandler(CMainWindow::OnOpenFile),
        nullptr, this);
    
}
*/

void CMainWindow::BindEvents()
{
    Connect(wxEVENT_FACEINFOSUPDATE,      wxCommandEventHandler(CMainWindow::OnFaceInfosUpdate));
    Connect(wxEVENT_SETSCREEN,            wxCommandEventHandler(CMainWindow::SetScreenEvent));
    Connect(wxEVENT_INFOS,                wxCommandEventHandler(CMainWindow::OnUpdateInfos));
    Connect(wxEVENT_REFRESHFOLDERLIST,    wxCommandEventHandler(CMainWindow::RefreshFolderList));
    Connect(TOOLBAR_UPDATE_ID,            wxCommandEventHandler(CMainWindow::OnShowToolbar));
    Connect(wxEVENT_ENDCHECKFILE,         wxCommandEventHandler(CMainWindow::OnEndCheckFile));
    Connect(wxEVENT_UPDATEFOLDER,         wxCommandEventHandler(CMainWindow::OnUpdateFolder));
    Connect(wxEVENT_ONPICTURECLICK,       wxCommandEventHandler(CMainWindow::OnPictureClick));
    Connect(wxEVT_CRITERIACHANGE,         wxCommandEventHandler(CMainWindow::CriteriaChange));
    Connect(wxEVENT_PICTUREVIDEOCLICK,    wxCommandEventHandler(CMainWindow::PictureVideoClick));
    Connect(wxEVENT_REFRESHFOLDER,        wxCommandEventHandler(CMainWindow::InitPictures));
    Connect(wxEVENT_REFRESHPICTURE,       wxCommandEventHandler(CMainWindow::OnRefreshPicture));
    Connect(wxEVENT_SETSTATUSTEXT,        wxCommandEventHandler(CMainWindow::OnStatusSetText));
    Connect(wxEVT_EXIT,                   wxCommandEventHandler(CMainWindow::OnExit));
    Connect(wxEVENT_SETRANGEPROGRESSBAR,  wxCommandEventHandler(CMainWindow::OnSetRangeProgressBar));
    Connect(wxEVENT_PRINTPICTURE,         wxCommandEventHandler(CMainWindow::PrintPreview));
    Connect(wxEVENT_CRITERIAPHOTOUPDATE,  wxCommandEventHandler(CMainWindow::OnCriteriaUpdate));
    Connect(wxEVENT_UPDATESTATUSBARMESSAGE, wxCommandEventHandler(CMainWindow::UpdateStatusBarMessage));
    Connect(wxEVENT_FACEADD,             wxCommandEventHandler(CMainWindow::OnFaceAdd));
    Connect(wxEVENT_PRINT,               wxCommandEventHandler(CMainWindow::OnPrint));
    Connect(wxEVENT_SETVALUEPROGRESSBAR, wxCommandEventHandler(CMainWindow::OnSetValueProgressBar));
    Connect(wxEVENT_SHOWSCANNER,         wxCommandEventHandler(CMainWindow::OnScanner));
    Connect(wxEVENT_OPENFILEORFOLDER,    wxCommandEventHandler(CMainWindow::OnOpenFileOrFolder));
    Connect(wxEVENT_EDITFILE,            wxCommandEventHandler(CMainWindow::OnEditFile));
    Connect(wxEVENT_EXPORTFILE,          wxCommandEventHandler(CMainWindow::OnExportFile));
    Connect(wxEVENT_UPDATETHUMBNAILEXIF, wxCommandEventHandler(CMainWindow::OnUpdateExifThumbnail));
    Connect(wxEVENT_EXPORTDIAPORAMA,     wxCommandEventHandler(CMainWindow::OnExportDiaporama));
    Connect(wxEVENT_DELETEFACE,          wxCommandEventHandler(CMainWindow::OnDeleteFace));
    Connect(wxEVENT_ICONEUPDATE,         wxCommandEventHandler(CMainWindow::UpdateThumbnailIcone));
    Connect(wxVERSION_UPDATE_EVENT,      wxCommandEventHandler(CMainWindow::OnVersionUpdate));
    Connect(wxEVENT_UPDATEMESSAGE,       wxCommandEventHandler(CMainWindow::UpdateMessage));
    Connect(wxEVENT_REFRESHTHUMBNAIL,    wxCommandEventHandler(CMainWindow::OnRefreshThumbnail));
    Connect(wxEVENT_ICONETHUMBNAILGENERATION, wxCommandEventHandler(CMainWindow::OnProcessThumbnail));
    Connect(wxEVENT_UPDATECHECKINSTATUS, wxCommandEventHandler(CMainWindow::OnCheckInUpdateStatus));
    Connect(wxEVENT_UPDATECHECKINFOLDER, wxCommandEventHandler(CMainWindow::OnRemoveFileFromCheckIn));

    Connect(TIMER_LOADPICTURESTART, wxEVT_TIMER,
            wxTimerEventHandler(CMainWindow::OnOpenFile),
            nullptr, this);
}

void CMainWindow::InitConfig(const wxString& fileToOpen)
{
    listProcessWindow.push_back(this);

    wxString firstFile;
    if (CMainParam* config = CMainParamInit::getInstance(); config != nullptr)
        firstFile = config->GetLastShowPicture();

    if (!fileToOpen.empty())
        firstFile = fileToOpen;

    folderService->SetLocalFilename(firstFile);
    folderService->SetFirstFileToShow(firstFile);

    if (firstFile.empty())
        loadPictureStartTimer->Start(100, true);

    if (CRegardsConfigParam* rp = CParamInit::getInstance(); rp != nullptr)
        faceDetection = rp->GetFaceDetection();

    folderService->UpdateFolderStatic(false);
}

void CMainWindow::InitBackgroundTasks()
{
    CThumbnailBuffer::GetVectorSize(); // pré-chauffe le buffer

    auto* checkFile = new CThreadCheckFile();
    checkFile->mainWindow = this;
    checkFile->checkFile = std::make_unique<std::thread>(
        CThreadCheckFile::CheckFile, checkFile);
    isCheckingFile = true;

    std::this_thread::sleep_for(100ms);

    scheduler->ReloadFromDatabase();

    versionUpdate = std::jthread(
        NewVersionAvailable,
        this
    );
    isCheckNewVersion = true;
    refreshFolder = true;
    processIdle   = true;
    processEnd    = false;
}

// ═════════════════════════════════════════════════════════════════════════════
// Boucle principale (idle)
// ═════════════════════════════════════════════════════════════════════════════

void CMainWindow::ProcessIdle()
{
    bool hasPendingWork = false;
    int nbProcesseur = 1;

    if (CRegardsConfigParam* cfg = CParamInit::getInstance(); cfg != nullptr)
        nbProcesseur = cfg->GetThumbnailProcess();

    if (processEnd)
        return;

    //---------------------------------------
    // Mise à jour des critères
    //---------------------------------------
    if (updateCriteria)
    {
        updateCriteria = false;
        hasPendingWork = true;

        if (auto* w = FindWindowById(CRITERIAFOLDERWINDOWID); w != nullptr)
        {
            wxCommandEvent evt(wxEVENT_UPDATECRITERIA);
            evt.SetExtraLong(1);
            w->GetEventHandler()->AddPendingEvent(evt);
        }
    }

    //---------------------------------------
    // Rafraîchissement du dossier
    //---------------------------------------
    if (refreshFolder)
    {
        folderService->UpdateFolderStatic(false);
        scheduler->ReloadFromDatabase();
        refreshFolder  = false;
        hasPendingWork = true;
    }

    //---------------------------------------
    // Changement de mode viewer
    //---------------------------------------
    if (setPictureMode)
    {
        setPictureMode = false;
        viewerCtrl->SetViewerModeEvent(4);
        hasPendingWork = true;
    }
    if (setViewerMode)
    {
        setViewerMode  = false;
        viewerCtrl->SetViewerModeEvent(1);
        hasPendingWork = true;
    }

    //---------------------------------------
    // Scheduling des miniatures
    //---------------------------------------
    nbElementInIconeList = CThumbnailBuffer::GetVectorSize();
    if (scheduler->Tick(nbProcesseur, nbElementInIconeList))
        hasPendingWork = true;

    processIdle = hasPendingWork;
}

void CMainWindow::IdleFunction()
{
    StartThread();
}

// ═════════════════════════════════════════════════════════════════════════════
// API publique — délégation aux services
// ═════════════════════════════════════════════════════════════════════════════

bool CMainWindow::GetProcessEnd()
{
    if (scheduler->GetNbProcess() > 0 || isCheckingFile)
        return false;
    return true;
}

void CMainWindow::SaveParameter()
{
    if (centralWnd) centralWnd->SaveParameter();
}

void CMainWindow::UpdateScreenRatio()
{
    viewerCtrl->UpdateScreenRatio();
    Resize();
}

bool CMainWindow::IsFullscreen()  { return viewerCtrl->IsFullscreen(); }
bool CMainWindow::IsVideo()       { return viewerCtrl->IsVideo(); }

bool CMainWindow::SetFullscreen()     { return viewerCtrl->SetFullscreen(); }
bool CMainWindow::SetFullscreenMode() { return viewerCtrl->SetFullscreenMode(); }
bool CMainWindow::SetScreen()         { return viewerCtrl->SetScreen(); }

void CMainWindow::ShowToolbar()   { viewerCtrl->ShowToolbar(); }
void CMainWindow::TransitionEnd() { viewerCtrl->TransitionEnd(); }

void CMainWindow::SetText(const int& numPos, const wxString& libelle)
    { viewerCtrl->SetText(numPos, libelle); }

void CMainWindow::SetRangeProgressBar(const int& range)
    { viewerCtrl->SetRangeProgressBar(range); }

void CMainWindow::SetPosProgressBar(const int& position)
    { viewerCtrl->SetPosProgressBar(position); }

wxString CMainWindow::GetFilename()
{
    folderService->SetLocalFilename(viewerCtrl->GetFilename());
    return folderService->GetLocalFilename();
}

void CMainWindow::SetPictureMode()
{
    setPictureMode = true;
    processIdle    = true;
    if (toolbarViewerMode) toolbarViewerMode->SetPictureWindowPush();
}

void CMainWindow::SetViewerMode()
{
    setViewerMode = true;
    processIdle   = true;
    if (toolbarViewerMode) toolbarViewerMode->SetViewerWindowPush();
}

// ═════════════════════════════════════════════════════════════════════════════
// Navigation / ouverture de fichiers
// ═════════════════════════════════════════════════════════════════════════════

bool CMainWindow::OpenFolder(const wxString& path)
{
    if (!wxDirExists(path))
        return false;

    if (viewerParam) viewerParam->SetCatalogCriteria("");

    if (auto* w = FindWindowById(CRITERIAFOLDERWINDOWID); w != nullptr)
    {
        auto* newFolder = new wxString(path);
        wxCommandEvent evt(wxEVENT_SETFOLDER);
        evt.SetClientData(newFolder);
        w->GetEventHandler()->AddPendingEvent(evt);
    }
    return true;
}

void CMainWindow::OpenFile(const wxString& fileToOpen)
{
    FolderCatalogVector folderList;
    CSqlFindFolderCatalog folderCatalog;
    folderCatalog.GetFolderCatalog(&folderList, NUMCATALOGID);

    wxFileName fn(fileToOpen);
    wxString folder = fn.GetPath();
    bool find = false;

    for (CFolderCatalog& fl : folderList)
    {
        if (folder == fl.GetFolderPath()) { find = true; break; }
    }

    if (!find) OpenFolder(folder);

    updateCriteria = true;
    folderService->SetFirstFileToShow(fileToOpen);
    folderService->UpdateFolderStatic(false);
    processIdle = true;

    viewerCtrl->LoadPicture(fileToOpen);
}

void CMainWindow::OnOpenFileOrFolder(wxCommandEvent& event)
{
    auto* file = static_cast<wxString*>(event.GetClientData());
    if (file == nullptr) return;

    folderService->SetLocalFilename(*file);
    folderService->SetFirstFileToShow(*file);

    const int type = event.GetInt();
    if (type == 1) OpenFile(*file);
    else           OpenFolder(*file);

    if (auto* w = FindWindowById(CRITERIAFOLDERWINDOWID); w != nullptr)
    {
        auto* fp = new wxString(*file);
        wxCommandEvent evt(wxEVENT_SELCHANGED);
        evt.SetExtraLong(1);
        evt.SetInt(1);
        evt.SetClientData(fp);
        w->GetEventHandler()->AddPendingEvent(evt);
    }
    delete file;
}

// ═════════════════════════════════════════════════════════════════════════════
// Handlers d'événements
// ═════════════════════════════════════════════════════════════════════════════

void CMainWindow::OnOpenFile(wxTimerEvent& /*event*/)
{
    CLibPicture libPicture;
    wxString dirpath = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Pictures);

    wxArrayString files;
    wxDir::GetAllFiles(dirpath, &files, wxEmptyString, wxDIR_FILES);
    if (files.size() > 0) sort(files.begin(), files.end());

    wxString firstFile;
    for (const wxString& f : files)
    {
        if (libPicture.TestImageFormat(f) != 0) { firstFile = f; break; }
    }

    if (!firstFile.empty())
    {
        auto* file = new wxString(firstFile);
        wxCommandEvent evt(wxEVENT_OPENFILEORFOLDER);
        evt.SetInt(1);
        evt.SetClientData(file);
        GetEventHandler()->AddPendingEvent(evt);
    }
}

void CMainWindow::OnExit(wxCommandEvent& /*event*/)
{
    statusBarViewer->Exit();
}

void CMainWindow::OnScanner(wxCommandEvent& /*event*/)
{
    statusBarViewer->ShowScanner();
}

void CMainWindow::InitPictures(wxCommandEvent& /*event*/)
{
    refreshFolder = true;
    processIdle   = true;
}

void CMainWindow::OnFaceInfosUpdate(wxCommandEvent& /*event*/)
{
    updateCriteria = true;
    processIdle    = true;
}

void CMainWindow::OnRefreshPicture(wxCommandEvent& /*event*/)
{
    wxString fn = viewerCtrl->GetFilename();
    folderService->SetLocalFilename(fn);
    viewerCtrl->LoadPicture(fn, true);
}

void CMainWindow::OnShowToolbar(wxCommandEvent& /*event*/)
{
    viewerCtrl->ShowToolbar();
}

void CMainWindow::SetScreenEvent(wxCommandEvent& /*event*/)
{
    Resize();
}

void CMainWindow::OnUpdateInfos(wxCommandEvent& event)
{
    auto* pictureInfos = static_cast<CPictureInfosMessage*>(event.GetClientData());
    if (pictureInfos == nullptr) return;

    if (!pictureInfos->filename.empty())
        statusBarViewer->SetText(1, pictureInfos->filename);

    statusBarViewer->SetText(0, pictureInfos->infos);

    wxString label = CLibResource::LoadStringFromResource(L"LBLUPDATEINFOS", 1);
    statusBarViewer->SetWindowTitle(label + CFileUtility::GetFileName(pictureInfos->filename));

    delete pictureInfos;
}

void CMainWindow::OnStatusSetText(wxCommandEvent& event)
{
    auto* st = static_cast<CStatusText*>(event.GetClientData());
    if (st)
    {
        statusBar->SetStatusText(st->text, st->position);
        delete st;
    }
}

void CMainWindow::OnSetRangeProgressBar(wxCommandEvent& event)
{
    viewerCtrl->UpdateProgressRange(event.GetInt());
}

void CMainWindow::OnSetValueProgressBar(wxCommandEvent& event)
{
    viewerCtrl->UpdateProgressValue(event.GetInt());
}

void CMainWindow::OnEndCheckFile(wxCommandEvent& event)
{
    auto* checkFile = static_cast<CThreadCheckFile*>(event.GetClientData());
    if (checkFile)
    {
        if (checkFile->checkFile) checkFile->checkFile->join();
        isCheckingFile = false;
        delete checkFile;
    }
    processIdle = true;
}

void CMainWindow::OnRefreshThumbnail(wxCommandEvent& /*event*/)
{
    scheduler->Reset();
    scheduler->ReloadFromDatabase();
    processIdle = true;
}

void CMainWindow::UpdateThumbnailIcone(wxCommandEvent& event)
{
    scheduler->OnThumbnailDone();
    auto* localevent = new wxCommandEvent(wxEVENT_ICONEUPDATE);
    localevent->SetClientData(event.GetClientData());
    wxQueueEvent(centralWnd, localevent);
}

void CMainWindow::OnCriteriaUpdate(wxCommandEvent& /*event*/)
{
    folderService->SetLocalFilename(viewerCtrl->GetFilename());
    folderService->UpdateFolderStatic(false);
}

void CMainWindow::RefreshFolderList(wxCommandEvent& /*event*/)
{
    folderService->SetLocalFilename(viewerCtrl->GetFilename());
    folderService->UpdateFolderStatic(false);
}

void CMainWindow::OnUpdateFolder(wxCommandEvent& event)
{
    const int typeId = event.GetInt();
    auto* newPath = static_cast<wxString*>(event.GetClientData());
    bool isDelete = false;

    if (newPath != nullptr)
    {
        if (typeId == wxEVENT_ADDFOLDER)
        {
            folderService->SetFirstFileToShow(*newPath);
            wxFileName fn(*newPath);
            statusBarViewer->AddFSEntry(fn.GetPath());
        }
        else if (typeId == wxEVENT_REMOVEFOLDER)
        {
            statusBarViewer->RemoveFSEntry(*newPath);
            isDelete = true;
        }
    }

    updateCriteria = true;
    scheduler->ReloadFromDatabase();

    if (newPath) delete newPath;

    folderService->UpdateFolderStatic(isDelete);
    processIdle = true;
}

void CMainWindow::OnRemoveFileFromCheckIn(wxCommandEvent& /*event*/)
{
    updateCriteria = true;
    folderService->UpdateFolderStatic(true);
    processIdle = true;
}

void CMainWindow::OnCheckInUpdateStatus(wxCommandEvent& event)
{
    wxString label   = CLibResource::LoadStringFromResource(L"LBLFILECHECKING", 1);
    wxString message = wxString::Format("%s%d/%s",
                         label,
                         event.GetInt(),
                         event.GetString());
    if (statusBarViewer) statusBarViewer->SetText(3, message);
}

void CMainWindow::CriteriaChange(wxCommandEvent& /*event*/)
{
    if (auto* w = FindWindowById(CRITERIAFOLDERWINDOWID); w != nullptr)
    {
        wxCommandEvent evt(wxEVENT_UPDATECRITERIA);
        evt.SetExtraLong(1);
        w->GetEventHandler()->AddPendingEvent(evt);
    }
    processIdle = true;
}

void CMainWindow::PictureVideoClick(wxCommandEvent& event)
{
    viewerCtrl->SetPosition(event.GetExtraLong());
}

void CMainWindow::OnPictureClick(wxCommandEvent& event)
{
    wxString filename = CThumbnailBuffer::FindPhotoById(
        static_cast<int>(event.GetExtraLong()));
    viewerCtrl->LoadPicture(filename);
}

void CMainWindow::OnPrint(wxCommandEvent& /*event*/)
{
    viewerCtrl->OnPrint(folderService->GetLocalFilename());
}

void CMainWindow::PrintPreview(wxCommandEvent& event)
{
    auto* bitmap = static_cast<CImageLoadingFormat*>(event.GetClientData());
    if (bitmap) statusBarViewer->PrintImagePreview(bitmap);
}

void CMainWindow::OnExportFile(wxCommandEvent& /*event*/)
{
    viewerCtrl->OnExportFile(folderService->GetLocalFilename());
}

void CMainWindow::OnEditFile(wxCommandEvent& /*event*/)
{
    wxString fn = folderService->GetLocalFilename();
    viewerCtrl->OnEditFile(fn);
    folderService->SetLocalFilename(fn);
}

void CMainWindow::OnExportDiaporama(wxCommandEvent& /*event*/)
{
    viewerCtrl->OnExportDiaporama();
}

void CMainWindow::OnUpdateExifThumbnail(wxCommandEvent& event)
{
    const int numPhoto = event.GetInt();
    const int rotate   = static_cast<int>(event.GetExtraLong());

    wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
    CFiltreEffetCPU::LoadAndRotate(thumbnail, rotate);

    if (auto* w = FindWindowById(THUMBNAILVIEWERPICTURE); w != nullptr)
    {
        wxCommandEvent evt(wxEVENT_REFRESHTHUMBNAIL);
        evt.SetInt(numPhoto);
        w->GetEventHandler()->AddPendingEvent(evt);
    }
}

void CMainWindow::OnFaceAdd(wxCommandEvent& event)
{
    wxTextEntryDialog dlg(this, "", "Add a New Person");
    if (dlg.ShowModal() != wxID_OK) return;

    auto* numFace  = static_cast<vector<int>*>(event.GetClientData());
    wxString label = dlg.GetValue();

    if (label.size() > 3)
    {
        CSqlFaceLabel sqlFaceLabel;
        sqlFaceLabel.InsertFaceLabel(numFace->at(0), label, 1);

        CSqlFaceRecognition faceRecognition;
        for (int id : *numFace)
            faceRecognition.MoveFaceRecognition(id, numFace->at(0));

        numFace->clear();
        delete numFace;

        if (auto* w = FindWindowById(LISTFACEID); w != nullptr)
        {
            wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
            w->GetEventHandler()->AddPendingEvent(evt);
        }
        wxQueueEvent(this, new wxCommandEvent(wxEVT_CRITERIACHANGE));
    }
    else
    {
        numFace->clear();
        delete numFace;
        wxString wrong = CLibResource::LoadStringFromResource(L"wronglabelsize", 1);
        wxString err   = CLibResource::LoadStringFromResource(L"erroronlabelsize", 1);
        wxMessageBox(wrong, err, wxICON_ERROR);
    }
}

void CMainWindow::OnDeleteFace(wxCommandEvent& event)
{
    wxString info = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
    if (wxMessageBox("Do you want to delete this Person ?",
                     info, wxYES_NO | wxCANCEL, nullptr) != wxYES)
        return;

    CSqlFacePhoto sqlFacePhoto;
    sqlFacePhoto.DeleteNumFaceMaster(event.GetInt());

    if (auto* w = FindWindowById(LISTFACEID); w != nullptr)
    {
        wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
        w->GetEventHandler()->AddPendingEvent(evt);
    }
    wxQueueEvent(this, new wxCommandEvent(wxEVT_CRITERIACHANGE));
}

void CMainWindow::OnVersionUpdate(wxCommandEvent& event)
{
    cout << "OnVersionUpdate" << endl;

    if (event.GetInt() && toolbarViewerMode)
        toolbarViewerMode->SetUpdateVisible(true);

    isCheckNewVersion = false;
}

void CMainWindow::OnProcessThumbnail(wxCommandEvent& event)
{
    if (event.GetInt() == 0)
    {
        auto* list = static_cast<vector<wxString>*>(event.GetClientData());
        if (list)
        {
            scheduler->PrioritizeFiles(*list);
            list->clear();
            delete list;
        }
    }
    else
    {
        auto* fn = static_cast<wxString*>(event.GetClientData());
        if (fn)
        {
            scheduler->PrioritizeFile(*fn);
            delete fn;
        }
    }
    processIdle = true;
}

void CMainWindow::SetDataToStatusBar(void* thumbMessage, const wxString& picture)
{
    auto* msg = static_cast<CThumbnailMessage*>(thumbMessage);
    if (msg == nullptr) return;

    const wxString text = picture + to_string(msg->nbPhoto);
    if (statusBarViewer)
    {
        statusBarViewer->SetRangeProgressBar(msg->nbElement);
        statusBarViewer->SetText(2, text);
        statusBarViewer->SetPosProgressBar(msg->thumbnailPos + 1);
    }
    delete msg;
}

void CMainWindow::UpdateStatusBarMessage(wxCommandEvent& event)
{
    auto* msg = static_cast<CThumbnailMessage*>(event.GetClientData());
    if (msg == nullptr) return;

    static const std::unordered_map<int, wxString> labels =
    {
        {0, "LBLCRITERIANBIMAGE"},
        {2, "LBLFOLDERPROCESSING"},
        {3, "LBLPICTURERENDER"},
        {4, "LBLFACEPROCESS"},
        {5, "LBLFACERECOGNITIONPROCESS"},
        {6, "LBLGEOLOCALISATIONGPS"}
    };

    auto it = labels.find(msg->typeMessage);
    if (it != labels.end())
        SetDataToStatusBar(msg, CLibResource::LoadStringFromResource(it->second, 1));
    else if (statusBarViewer)
    {
        statusBarViewer->SetRangeProgressBar(msg->nbElement);
        statusBarViewer->SetPosProgressBar(0);
    }
}

void CMainWindow::UpdateMessage(wxCommandEvent& event)
{
    const int nbPhoto = static_cast<int>(event.GetExtraLong());
    auto* msg         = new CThumbnailMessage();
    msg->nbPhoto      = nbPhoto;
    msg->thumbnailPos = scheduler->GetThumbnailPos();
    msg->nbElement    = nbElementInIconeList;
    msg->typeMessage  = 3;

    if (auto* mw = FindWindowById(MAINVIEWERWINDOWID); mw != nullptr)
    {
        wxCommandEvent evt(wxEVENT_UPDATESTATUSBARMESSAGE);
        evt.SetClientData(msg);
        evt.SetInt(3);
        mw->GetEventHandler()->AddPendingEvent(evt);
    }
}

void CMainWindow::ClickShowButton(const int& id, const int& /*refresh*/)
{
    switch (id)
    {
    case WINDOW_VIEWER:   viewerCtrl->SetViewerModeEvent(1); break;
    case WINDOW_FACE:     viewerCtrl->SetViewerModeEvent(2); break;
    case WINDOW_EXPLORER: viewerCtrl->SetViewerModeEvent(3); break;
    case WINDOW_PICTURE:  viewerCtrl->SetViewerModeEvent(4); break;
    case IDM_NEWVERSION:
    {
        wxString siteweb = CLibResource::LoadStringFromResource("SITEWEB", 1);
        wxMimeTypesManager manager;
        wxFileType* ft = manager.GetFileTypeFromExtension("html");
        if (ft) wxExecute(ft->GetOpenCommand(siteweb));
    }
    break;
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// Mise en page
// ═════════════════════════════════════════════════════════════════════════════

void CMainWindow::Resize()
{
    if (centralWnd == nullptr || statusBar == nullptr)
        return;

    const int windowWidth  = GetWindowWidth();
    const int windowHeight = GetWindowHeight();

    if (viewerCtrl->IsFullscreen())
    {
        centralWnd->SetSize(0, 0, windowWidth, windowHeight);
        return;
    }

    const int toolbarHeight = toolbarViewerMode
        ? toolbarViewerMode->GetNavigatorHeight() : 0;
    const int statusBarHeight = statusBar->GetSize().y;

    if (toolbarViewerMode)
        toolbarViewerMode->SetSize(0, 0, windowWidth, toolbarHeight);

    centralWnd->SetSize(0, toolbarHeight, windowWidth,
                        windowHeight - toolbarHeight - statusBarHeight);

    statusBar->SetSize(0, windowHeight - statusBarHeight,
                       windowWidth, statusBarHeight);
}

// ═════════════════════════════════════════════════════════════════════════════
// Thread de vérification de version
// ═════════════════════════════════════════════════════════════════════════════

void CMainWindow::NewVersionAvailable(void* param)
{
    auto* main = static_cast<CMainWindow*>(param);

    wxString localVersion  = CLibResource::LoadStringFromResource("REGARDSVERSION", 1);
    wxString serverURL     = CLibResource::LoadStringFromResource("ADRESSEWEBVERSION", 1);

    Regards::Internet::CCheckVersion checker(serverURL);
    wxString serverVersion = checker.GetLastVersion();
    serverVersion = serverVersion.SubString(0, serverVersion.length() - 2);

    localVersion.Replace(".", "");
    serverVersion.Replace(".", "");

    long lv = 0, sv = 0;
    localVersion.ToLong(&lv);
    serverVersion.ToLong(&sv);

    int hasUpdate = 0;
    if (serverVersion != "error" && !serverVersion.empty() && lv < sv)
        hasUpdate = 1;

    wxCommandEvent evt(wxVERSION_UPDATE_EVENT);
    evt.SetInt(hasUpdate);
    wxPostEvent(main, evt);
}
