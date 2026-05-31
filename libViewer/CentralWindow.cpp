// ReSharper disable All
#include <header.h>
#include "CentralWindow.h"
#include "ViewerParamInit.h"
#include <LibResource.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <wx/display.h>
#include "PanelPhotoWnd.h"
#include "ListPicture.h"
#include "WindowManager.h"
#include "ThumbnailViewerVideo.h"
#include "ThumbnailViewerPicture.h"
#ifndef __NOFACE_DETECTION__
#include "ListFace.h"
#endif
#include "ViewerParam.h"
#include "PanelInfosWnd.h"
#include <PanelWithClickToolbar.h>
#include "PreviewWnd.h"
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <ThumbnailFolder.h>

// ── Sub-controllers ──────────────────────────────────────────────────────────
#include "MusicController.h"
#include "ThumbnailController.h"
#include "MediaLoader.h"
#include "ViewerController.h"
#include "SlideshowController.h"
#include "WindowModeController.h"

#include "window_mode_id.h"

using namespace Regards::Picture;
using namespace Regards::Window;
using namespace Regards::Sqlite;
using namespace Regards::Viewer;
using namespace Regards::FiltreEffet;

// ─────────────────────────────────────────────────────────────────────────────
// Construction
// ─────────────────────────────────────────────────────────────────────────────

CCentralWindow::CCentralWindow(wxWindow* parent, wxWindowID id,
                               const CThemeSplitter& theme, const bool& horizontal)
    : CWindowMain("CentralWindow", parent, id)
{
    wxRect rect;
    wxRect left;
    wxRect right;
    int widthInfosWindow = 0;

    CMainParam*            config       = CMainParamInit::getInstance();
    CMainTheme*            viewerTheme  = CMainThemeInit::getInstance();
    CRegardsConfigParam*   regardsParam = CParamInit::getInstance();

    if (regardsParam != nullptr)
        faceDetection = regardsParam->GetFaceDetection();

    windowManager = new CWindowManager(this, wxID_ANY, theme);

    int initialWindowMode = 1;
    if (config != nullptr)
    {
        initialWindowMode = config->GetViewerMode();
        left  = config->GetPositionLeftPanel();
        right = config->GetPositionRightPanel();
    }

    if (!faceDetection && initialWindowMode == WINDOW_FACE)
        initialWindowMode = 1;

    if (left.width  < 50) left  = {0, 0, 0, 0};
    if (right.width < 50) right = {0, 0, 0, 0};

   
    if (viewerTheme != nullptr)
    {
        // ── Panel: photo search (left) ────────────────────────────────────
        {
            wxString libelle = CLibResource::LoadStringFromResource(L"LBLFOLDERCATEGORY", 1);
            CThemePane theme_pane;
            viewerTheme->GetPaneTheme(&theme_pane);

            panelPhotoWnd = new CPanelPhotoWnd(windowManager, CRITERIAFOLDERWINDOWID);
            windowManager->AddPanel(panelPhotoWnd, Pos::wxLEFT, false, widthInfosWindow, left,
                libelle, "PanelPhotoSearch", true, PHOTOSEEARCHPANEL, false, true);
        }
        // ── Panel: video thumbnail strip (top) ────────────────────────────
        {
            CThemePane     theme_pane;
            CThemeThumbnail themeVideo;
            viewerTheme->GetPaneTheme(&theme_pane);
            viewerTheme->GetThumbnailTheme(&themeVideo);

            wxString libelle = CLibResource::LoadStringFromResource(L"LBLTHUMBNAILVIDEO", 1);
            thumbnailVideo = new CThumbnailViewerVideo(windowManager, THUMBNAILVIDEOWINDOW, themeVideo, true);
            scrollVideoWindow = new CScrollbarWnd(windowManager, thumbnailVideo, wxID_ANY);
            scrollVideoWindow->HideVerticalScroll();
            scrollVideoWindow->SetPageSize(200);
            scrollVideoWindow->SetLineSize(200);
            windowManager->AddPanel(scrollVideoWindow, Pos::wxTOP, true,
                themeVideo.themeIcone.GetHeight() + theme_pane.GetHeight() * 2,
                rect, libelle, "ThumbnailVideoPanel", true, THUMBNAILVIDEOPANEL, true, true);
        }

        // ── Panel: picture thumbnail strip (bottom) ───────────────────────
        {
            bool checkValidity = false;
            if (config != nullptr)
                checkValidity = config->GetCheckThumbnailValidity();

            CThemePane     theme_pane;
            CThemeThumbnail themeThumbnail;
            viewerTheme->GetPaneTheme(&theme_pane);
            viewerTheme->GetThumbnailTheme(&themeThumbnail);

            wxString libelle = CLibResource::LoadStringFromResource(L"LBLTHUMBNAIL", 1);
            thumbnailPicture = new CThumbnailViewerPicture(windowManager, THUMBNAILVIEWERPICTURE,
                themeThumbnail, checkValidity);
            scrollPictureWindow = new CScrollbarWnd(windowManager, thumbnailPicture, wxID_ANY);
            scrollPictureWindow->HideVerticalScroll();
            scrollPictureWindow->SetPageSize(200);
            scrollPictureWindow->SetLineSize(200);
            thumbnailPicture->SetNoVScroll(true);
            windowManager->AddPanel(scrollPictureWindow, Pos::wxBOTTOM, true,
                themeThumbnail.themeIcone.GetHeight() + theme_pane.GetHeight() * 2,
                rect, libelle, "ThumbnailPicturePanel", true, THUMBNAILPICTUREPANEL, true, true);
        }

        // ── Panel: info panel (right) ─────────────────────────────────────
        {
            wxString libelle = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);
            panelInfosWindow = new CPanelInfosWnd(windowManager, PANELINFOSWNDID);
            panelInfosClick = windowManager->AddPanel(panelInfosWindow, Pos::wxRIGHT, false,
                widthInfosWindow, right, libelle,
                "PictureInfosPanel", true, PANELCLICKINFOSWNDID, false);
        }

        // ── Central preview ───────────────────────────────────────────────
        previewWindow = new CPreviewWnd(windowManager, PREVIEWVIEWERID);
        windowManager->AddWindow(previewWindow, Pos::wxCENTRAL, false, 0, rect, PREVIEWVIEWERID, false);

        // ── List views ────────────────────────────────────────────────────
        {
            listPicture = new CListPicture(windowManager, LISTPICTUREID);
            listPicture->Show(false);
#ifndef __NOFACE_DETECTION__
            listFace = new CListFace(windowManager, LISTFACEID);
            listFace->Show(false);
#endif
        }
    }

    // ── Sub-controllers ───────────────────────────────────────────────
    musicController = std::make_unique <CMusicController>(this);

    thumbnailController = std::make_unique < CThumbnailController>(thumbnailPicture, thumbnailVideo,listPicture, listFace);

    mediaLoader = std::make_unique < CMediaLoader>(this, previewWindow, panelInfosWindow,thumbnailPicture, thumbnailVideo, musicController);
    mediaLoader->windowMode = initialWindowMode;

    viewerController = std::make_unique < CViewerController>(this, thumbnailPicture, listPicture,listFace, previewWindow, mediaLoader);

    slideshowController = std::make_unique < CSlideshowController>(this, previewWindow,musicController, viewerController);

    windowModeController = std::make_unique < CWindowModeController>(this, windowManager, panelInfosClick,previewWindow, panelInfosWindow, listPicture, listFace, faceDetection);

    // ── wxWidgets event bindings ──────────────────────────────────────
    Connect(wxEVT_ANIMATIONTIMERSTOP,    wxCommandEventHandler(CCentralWindow::StopAnimationEvent));
    Connect(wxTIMER_DIAPORAMATIMERSTART,   wxCommandEventHandler(CCentralWindow::StartDiaporamaMessage));
    Connect(VIDEO_END_ID,                  wxCommandEventHandler(CCentralWindow::OnVideoEnd));
    Connect(wxEVENT_CHANGETYPEAFFICHAGE,   wxCommandEventHandler(CCentralWindow::ChangeTypeAffichage));
    Connect(wxEVENT_SETMODEVIEWER,         wxCommandEventHandler(CCentralWindow::SetMode));
    Connect(VIDEO_UPDATE_ID,               wxCommandEventHandler(CCentralWindow::SetVideoPos));
    Connect(wxTIMER_ANIMATION, wxEVT_TIMER, wxTimerEventHandler(CCentralWindow::OnTimerAnimation), nullptr, this);
    Connect(EVENT_SHOWPICTURE,             wxCommandEventHandler(CCentralWindow::OnShowPicture));
    Connect(wxEVENT_ENDLOADPICTURE,        wxCommandEventHandler(CCentralWindow::OnEndLoadPicture));
    Connect(VIDEO_START,                   wxCommandEventHandler(CCentralWindow::OnVideoStart));
    Connect(wxVIDEO_STOP,                  wxCommandEventHandler(CCentralWindow::OnVideoStop));
    Connect(wxEVENT_ANIMATIONSTART,        wxCommandEventHandler(CCentralWindow::OnAnimationStart));
    Connect(wxEVENT_ANIMATIONSTOP,         wxCommandEventHandler(CCentralWindow::OnAnimationStop));
    Connect(wxEVENT_STOPVIDEO,             wxCommandEventHandler(CCentralWindow::OnStopAudio));
    Connect(wxEVENT_ENDVIDEOTHREAD,        wxCommandEventHandler(CCentralWindow::OnQuitAudio));
    Connect(wxEVENT_PICTUREPREVIOUS,       wxCommandEventHandler(CCentralWindow::OnPicturePrevious));
    Connect(wxEVENT_PICTURENEXT,           wxCommandEventHandler(CCentralWindow::OnPictureNext));
    Connect(wxEVENT_PICTUREFIRST,          wxCommandEventHandler(CCentralWindow::OnPictureFirst));
    Connect(wxEVENT_PICTURELAST,           wxCommandEventHandler(CCentralWindow::OnPictureLast));
    Connect(wxEVENT_STOPDIAPORAMA,         wxCommandEventHandler(CCentralWindow::StopDiaporama));
    Connect(wxEVENT_STARTDIAPORAMA,        wxCommandEventHandler(CCentralWindow::StartDiaporama));
    Connect(wxEVENT_ICONEUPDATE,           wxCommandEventHandler(CCentralWindow::UpdateThumbnailIcone));
    Connect(wxEVENT_ICONESIZEREFRESH,      wxCommandEventHandler(CCentralWindow::UpdateThumbnailIconeSize));
    Connect(wxTIMER_DIAPORAMA, wxEVT_TIMER, wxTimerEventHandler(CCentralWindow::OnTimerDiaporama), nullptr, this);

    windowManager->HideWindow(Pos::wxTOP, false);

    wxCommandEvent event(wxEVENT_SETMODEVIEWER);
    event.SetInt(initialWindowMode);
    wxPostEvent(this, event);
}

CCentralWindow::~CCentralWindow()
{
    cout << "End Program" << endl;

    CMainParam* config = CMainParamInit::getInstance();
    if (config != nullptr)
        config->SetLastShowPicture(viewerController->GetFilename());
}

// ─────────────────────────────────────────────────────────────────────────────
// Public API — thin forwarders
// ─────────────────────────────────────────────────────────────────────────────
void CCentralWindow::OnEndLoadPicture(wxCommandEvent& event)
{
	wxString* filename = event.GetClientData() != nullptr ? static_cast<wxString*>(event.GetClientData()) : nullptr;
    if (mediaLoader->IsAnimation() || mediaLoader->IsVideo())
    {
        windowModeController->ShowTopWindow();
        needToRefresh = true;
    }
    else
    {
        windowModeController->HideTopWindow();
        needToRefresh = true;
    }

    if (filename != nullptr)
    {
        viewerController->SetFilename(*filename);
        delete filename;
        filename = nullptr;
    }
}

wxString CCentralWindow::ImageSuivante(const bool& lp)    
{ 
    return viewerController->ImageSuivante(lp);
}
wxString CCentralWindow::ImagePrecedente(const bool& lp)  
{ 
    return viewerController->ImagePrecedente(lp);
}
wxString CCentralWindow::ImageFin(const bool& lp)         
{   
    return viewerController->ImageFin(lp);
}
wxString CCentralWindow::ImageDebut(const bool& lp)       
{ 
    return viewerController->ImageDebut(lp);
}

wxString CCentralWindow::GetFilename()                     { return viewerController->GetFilename(); }
int      CCentralWindow::GetNbElement()                    { return viewerController->GetNbElement(); }


int CCentralWindow::LoadPicture(const wxString& f, const bool& r)
{ 
    viewerController->SetFilename(f);
    return mediaLoader->LoadPicture(f, r);
}

bool     CCentralWindow::IsVideo()                         { return mediaLoader->IsVideo(); }
bool     CCentralWindow::IsDiaporamaStart()                { return slideshowController->IsDiaporamaStart(); }
void     CCentralWindow::TransitionEnd()                   { slideshowController->TransitionEnd(); }
void     CCentralWindow::AnimationPictureNext()            { mediaLoader->AnimationPictureNext(); }
void     CCentralWindow::AnimationPicturePrevious()        { mediaLoader->AnimationPicturePrevious(); }
void     CCentralWindow::SetPosition(const long& pos)      { mediaLoader->SetPosition(pos); }
bool     CCentralWindow::FullscreenMode()                  { return windowModeController->FullscreenMode(); }
bool     CCentralWindow::ScreenMode()                      { return windowModeController->ScreenMode(); }
bool     CCentralWindow::IsCompatibleFullscreen()          { return windowModeController->IsCompatibleFullscreen(); }
void     CCentralWindow::HideToolbar()                     { windowModeController->HideToolbar(); }
void     CCentralWindow::ShowToolbar()                     { windowModeController->ShowToolbar(); }
void     CCentralWindow::UpdateScreenRatio()               { windowModeController->UpdateScreenRatio(); }
void     CCentralWindow::SaveParameter()                   { windowModeController->SaveParameter(); }
void     CCentralWindow::Resize()                          { windowModeController->Resize(); }

void CCentralWindow::SetListeFile(const wxString& filename, const bool& isDeleteFolder,
                                   const bool& isSqlUpdate, const int& typeAffichage)
{
    
    if (listPicture != nullptr)
        listPicture->SetListeFile(typeAffichage, isDeleteFolder, isSqlUpdate);
    if (thumbnailPicture != nullptr)
        thumbnailPicture->ApplyListeFile(isDeleteFolder, isSqlUpdate);
    mediaLoader->LoadPicture(filename);
}

// ─────────────────────────────────────────────────────────────────────────────
// GetProcessEnd (CWindowMain override)
// ─────────────────────────────────────────────────────────────────────────────

bool CCentralWindow::GetProcessEnd()
{
    mediaLoader->StopAnimation();

    // Let the slideshow controller stop its timer
    if (slideshowController->IsDiaporamaStart())
    {
        // StopDiaporama sends a command event; call directly here
        wxCommandEvent dummy;
        slideshowController->StopDiaporama(dummy);
    }

    if (mediaLoader->processLoadPicture)
        return false;

    if (slideshowController->IsDiaporamaStart())
        return musicController->IsMusicStop();

    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Event handlers — all one-liners delegating to sub-controllers
// ─────────────────────────────────────────────────────────────────────────────

void CCentralWindow::OnVideoEnd(wxCommandEvent& e)           
{ 
    mediaLoader->OnVideoEnd(e); 
    
}
void CCentralWindow::OnTimerAnimation(wxTimerEvent& e)       { mediaLoader->OnTimerAnimation(e); }
void CCentralWindow::SetVideoPos(wxCommandEvent& e)          { mediaLoader->SetVideoPos(e); }
void CCentralWindow::AnimationSetPosition(wxCommandEvent& e) { mediaLoader->AnimationSetPosition(e); }

void CCentralWindow::OnVideoStart(wxCommandEvent& /*e*/)
{
    mediaLoader->SetVideoStart();

    if (musicController != nullptr)
    {
        musicController->StopMusic();
        musicController->SetMusicPause(true);
    }
    if (thumbnailVideo != nullptr)
        thumbnailVideo->SetVideoPosition(0);
}

void CCentralWindow::OnVideoStop(wxCommandEvent& /*e*/)
{
    mediaLoader->SetVideoStop();
}

void CCentralWindow::OnAnimationStart(wxCommandEvent& /*e*/) { mediaLoader->StartAnimation(); }
void CCentralWindow::OnAnimationStop(wxCommandEvent& /*e*/)  { mediaLoader->StopAnimation(); }

void CCentralWindow::OnShowPicture(wxCommandEvent& e)         { viewerController->OnShowPicture(e); }
void CCentralWindow::OnPicturePrevious(wxCommandEvent& e)     { viewerController->OnPicturePrevious(e); }
void CCentralWindow::OnPictureNext(wxCommandEvent& e)         { viewerController->OnPictureNext(e); }
void CCentralWindow::OnPictureFirst(wxCommandEvent& e)        { viewerController->OnPictureFirst(e); }
void CCentralWindow::OnPictureLast(wxCommandEvent& e)         { viewerController->OnPictureLast(e);  }

void CCentralWindow::StartDiaporama(wxCommandEvent& e)        { slideshowController->StartDiaporama(e); }
void CCentralWindow::StopDiaporama(wxCommandEvent& e)         { slideshowController->StopDiaporama(e); }
void CCentralWindow::OnTimerDiaporama(wxTimerEvent& e)        { slideshowController->OnTimerDiaporama(e); }
void CCentralWindow::StartDiaporamaMessage(wxCommandEvent& e) { slideshowController->StartDiaporamaMessage(e); }
void CCentralWindow::StopAnimationEvent(wxCommandEvent& e)    { slideshowController->StopAnimationEvent(e); }

void CCentralWindow::SetMode(wxCommandEvent& e)
{
    // Keep mediaLoader in sync with the new window mode
    mediaLoader->windowMode = e.GetInt();
    windowModeController->isVideo     = mediaLoader->isVideo;
    windowModeController->isAnimation = mediaLoader->isAnimation;
    windowModeController->SetMode(e);
}

void CCentralWindow::UpdateThumbnailIcone(wxCommandEvent& e)
{
    thumbnailController->UpdateThumbnailIcone(e);
    // Type-30 dispatch: forward to the specific window after the controller ran
}
void CCentralWindow::UpdateThumbnailIconeSize(wxCommandEvent& e)
{
    windowModeController->UpdateThumbnailIconeSize(e);
}
void CCentralWindow::OnRefreshThumbnail(wxCommandEvent& e)    { thumbnailController->OnRefreshThumbnail(e); }

void CCentralWindow::ChangeTypeAffichage(wxCommandEvent& event)
{
    if (listPicture != nullptr)
        listPicture->ChangeTypeAffichage(event.GetExtraLong());
}

void CCentralWindow::OnStopAudio(wxCommandEvent& e) { musicController->OnStopAudio(e); }
void CCentralWindow::OnQuitAudio(wxCommandEvent& e) { musicController->OnQuitAudio(e); }
