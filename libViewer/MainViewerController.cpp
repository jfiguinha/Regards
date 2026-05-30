#include <header.h>
#include "MainViewerController.h"
#include "CentralWindow.h"
#include "ToolbarViewerMode.h"
#include "ExportDiaporama.h"
#include "FileUtility.h"
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <StatusBarInterface.h>
#include <ImageLoadingFormat.h>
#include <LibResource.h>
#include <IBitmapWnd.h>
#include <BitmapWndViewer.h>
#include <ShowElement.h>
#include <window_id.h>
#include <wx/mimetype.h>
#include <wx/filedlg.h>
#include <wx/event.h>
#include <libPicture.h>
#include "StatusText.h"

using namespace Regards::Viewer;
using namespace Regards::Picture;
using namespace Regards::Control;

CMainViewerController::CMainViewerController(CCentralWindow*     centralWnd,
                                   CToolbarViewerMode* toolbarViewerMode,
                                   wxStatusBar*        statusBar,
                                   wxGauge*            progressBar,
                                   IStatusBarInterface* statusBarViewer,
                                   wxWindow*           eventSink)
    : centralWnd(centralWnd)
    , toolbarViewerMode(toolbarViewerMode)
    , statusBar(statusBar)
    , progressBar(progressBar)
    , statusBarViewer(statusBarViewer)
    , eventSink(eventSink)
{}

// ── Plein écran ───────────────────────────────────────────────────────────────

bool CMainViewerController::IsFullscreen() const { return fullscreen; }

bool CMainViewerController::SetFullscreen()
{
    const bool work = centralWnd->IsCompatibleFullscreen();
    if (work)
        statusBarViewer->SetFullscreen();
    return work;
}

bool CMainViewerController::SetFullscreenMode()
{
    if (fullscreen)
        return false;

    if (!centralWnd->FullscreenMode())
        return false;

    fullscreen = true;
    if (toolbarViewerMode) toolbarViewerMode->Show(false);
    statusBar->Show(false);

    wxCommandEvent evt(wxEVENT_SETSCREEN);
    wxPostEvent(eventSink, evt);
    return true;
}

bool CMainViewerController::SetScreen()
{
    if (!fullscreen)
        return false;

    if (!centralWnd->ScreenMode())
        return false;

    statusBarViewer->SetScreen();
    fullscreen = false;
    if (toolbarViewerMode) toolbarViewerMode->Show(true);
    statusBar->Show(true);

    wxCommandEvent evt(wxEVENT_SETSCREEN);
    wxPostEvent(eventSink, evt);
    return true;
}

// ── Toolbar ───────────────────────────────────────────────────────────────────

void CMainViewerController::ShowToolbar()
{
    showToolbar = !showToolbar;
    if (centralWnd == nullptr)
        return;

    if (showToolbar)
        centralWnd->ShowToolbar();
    else
        centralWnd->HideToolbar();
}

// ── Lecture courante ──────────────────────────────────────────────────────────

bool CMainViewerController::IsVideo() const
{
    return centralWnd != nullptr && centralWnd->IsVideo();
}

wxString CMainViewerController::GetFilename() const
{
    return centralWnd ? centralWnd->GetFilename() : wxString{};
}

void CMainViewerController::LoadPicture(const wxString& filename, bool forceReload)
{
    if (centralWnd)
        centralWnd->LoadPicture(filename, forceReload);
}

void CMainViewerController::SetPosition(long timePosition)
{
    if (centralWnd)
        centralWnd->SetPosition(timePosition);
}

void CMainViewerController::TransitionEnd()
{
    if (centralWnd)
        centralWnd->TransitionEnd();

    auto* showBitmap = static_cast<CShowElement*>(
        eventSink->FindWindowById(SHOWBITMAPVIEWERID));
    if (showBitmap)
        showBitmap->TransitionEnd();
}

void CMainViewerController::UpdateScreenRatio()
{
    if (centralWnd)
        centralWnd->UpdateScreenRatio();
}

// ── Mode viewer ───────────────────────────────────────────────────────────────

void CMainViewerController::SetViewerModeEvent(int mode)
{
    if (auto* central = eventSink->FindWindowById(CENTRALVIEWERWINDOWID))
    {
        wxCommandEvent evt(wxEVENT_SETMODEVIEWER);
        evt.SetInt(mode);
        wxPostEvent(central, evt);
    }
}

// ── Actions utilisateur ───────────────────────────────────────────────────────

void CMainViewerController::OnPrint(const wxString& localFilename)
{
    bool showPrintPicture = true;

    if (IsVideo())
    {
        auto* video = static_cast<CShowElement*>(
            eventSink->FindWindowById(SHOWBITMAPVIEWERID));
        if (video && video->IsPause())
        {
            cv::Mat image = video->GetVideoBitmap();
            if (!image.empty())
            {
                auto* imageLoading = new CImageLoadingFormat();
                imageLoading->SetPicture(image);
                statusBarViewer->PrintImagePreview(imageLoading);
                showPrintPicture = false;
            }
        }
    }

    if (showPrintPicture)
    {
        CLibPicture libPicture;
        CImageLoadingFormat* image = libPicture.LoadPicture(localFilename);
        if (image)
            statusBarViewer->PrintPreview(image);
    }
}

void CMainViewerController::OnExportFile(const wxString& localFilename)
{
    if (IsVideo())
    {
        wxString pathProgram;
#ifdef __APPLE__
        pathProgram = CFileUtility::GetProgramFolderPath()
                    + "/RegardsViewer \""
                    + centralWnd->GetFilename()
                    + "\" -p RegardsConverter";
#elif defined(__WXMSW__)
        pathProgram = "RegardsViewer.exe \""
                    + centralWnd->GetFilename()
                    + "\"  -p RegardsConverter";
#else
        pathProgram = CFileUtility::GetProgramFolderPath()
                    + "/RegardsViewer \""
                    + centralWnd->GetFilename()
                    + "\" -p RegardsConverter";
#endif
        wxExecute(pathProgram);
        eventSink->SetFocus();
        eventSink->Raise();
        eventSink->Show(true);
    }
    else
    {
        auto* bitmapWindow = dynamic_cast<IBitmapWnd*>(
            eventSink->FindWindowById(BITMAPWINDOWVIEWERID));
        if (bitmapWindow)
        {
            auto* viewer = static_cast<CBitmapWndViewer*>(
                bitmapWindow->GetWndPt());
            if (viewer)
                viewer->ExportPicture();
        }
    }
}

void CMainViewerController::OnEditFile(wxString& localFilename)
{
    CMainParam* config = CMainParamInit::getInstance();
    wxString pathProgram;
    wxString title = CLibResource::LoadStringFromResource(L"LBLSELECTPICTUREEDITOR", 1);

    if (IsVideo())
    {
        if (config) pathProgram = config->GetPathForVideoEdit();
        title = CLibResource::LoadStringFromResource(L"LBLSELECTVIDEOEDITOR", 1);
    }
    else
    {
        if (config) pathProgram = config->GetPathForPictureEdit();
    }

    if (!wxFileExists(pathProgram))
        pathProgram.clear();

    if (pathProgram.empty())
    {
        const wxString allfiles = CLibResource::LoadStringFromResource(L"LBLALLFILES", 1);
        wxFileDialog dlg(nullptr, title, "", "",
                         allfiles, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        dlg.SetDirectory(CFileUtility::GetDocumentFolderPath());
        if (dlg.ShowModal() == wxID_OK)
            pathProgram = dlg.GetPath();
    }

    if (IsVideo()) { if (config) config->SetPathForVideoEdit(pathProgram);   }
    else           { if (config) config->SetPathForPictureEdit(pathProgram); }

    pathProgram = "\"" + pathProgram + "\" \"" + localFilename + "\"";
    wxExecute(pathProgram);
}

void CMainViewerController::OnExportDiaporama()
{
    CExportDiaporama exportDiaporama(eventSink);
    exportDiaporama.OnExportDiaporama();
}

// ── Barre de statut ───────────────────────────────────────────────────────────

void CMainViewerController::SetText(int numPos, const wxString& libelle)
{
    // Thread-safe : passage par événement
    auto* evt = new wxCommandEvent(wxEVENT_SETSTATUSTEXT);
    auto* st  = new CStatusText();
    st->position = numPos;
    st->text     = libelle;
    evt->SetClientData(st);
    wxQueueEvent(eventSink, evt);
}

void CMainViewerController::SetRangeProgressBar(int range)
{
    auto* evt = new wxCommandEvent(wxEVENT_SETRANGEPROGRESSBAR);
    evt->SetInt(range);
    wxQueueEvent(eventSink, evt);
}

void CMainViewerController::SetPosProgressBar(int position)
{
    auto* evt = new wxCommandEvent(wxEVENT_SETVALUEPROGRESSBAR);
    evt->SetInt(position);
    wxQueueEvent(eventSink, evt);
}

void CMainViewerController::UpdateStatusBarText(const wxString& text, int position)
{
    if (statusBar)
        statusBar->SetStatusText(text, position);
}

void CMainViewerController::UpdateProgressRange(int range)
{
    if (progressBar)
        progressBar->SetRange(range);
}

void CMainViewerController::UpdateProgressValue(int position)
{
    if (progressBar == nullptr || progressBar->GetRange() <= 0)
        return;

    const int clamped = (position >= progressBar->GetRange())
                        ? progressBar->GetRange() - 1
                        : position;
    progressBar->SetValue(clamped);
    progressBar->Refresh();
}
