#include <header.h>
#include "SlideshowController.h"
#include "MusicController.h"
#include "ViewerController.h"
#include "PreviewWnd.h"
#include <ViewerParam.h>
#include "window_mode_id.h"
#include <ViewerParamInit.h>
using namespace Regards::Viewer;

CSlideshowController::CSlideshowController(wxWindow*          eventSink,
                                           CPreviewWnd*       previewWindow,
                                           CMusicController*  musicController,
                                           CViewerController* viewerController)
    : eventSink(eventSink)
    , previewWindow(previewWindow)
    , musicController(musicController)
    , viewerController(viewerController)
{
    diaporamaTimer = new wxTimer(eventSink, wxTIMER_DIAPORAMA);
}

CSlideshowController::~CSlideshowController()
{
    if (diaporamaTimer->IsRunning())
        diaporamaTimer->Stop();
    delete diaporamaTimer;
}

void CSlideshowController::StartDiaporama(wxCommandEvent& /*event*/)
{
    CMainParam* viewerParam = CMainParamInit::getInstance();
    if (viewerParam != nullptr)
    {
        if (previewWindow != nullptr)
            previewWindow->SetDiaporamaMode();

        const int timeDelai = viewerParam->GetDelaiDiaporamaOption();
        diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
    }

    if (musicController != nullptr)
        musicController->StartMusic();

    isDiaporama = true;
}

void CSlideshowController::StopDiaporama(wxCommandEvent& /*event*/)
{
    if (isDiaporama)
    {
        if (previewWindow != nullptr)
            previewWindow->StopDiaporamaMode();

        if (diaporamaTimer->IsRunning())
            diaporamaTimer->Stop();
    }

    if (musicController != nullptr)
    {
        musicController->StopMusic();
        musicController->SetMusicPause(true);
    }

    isDiaporama = false;
}

void CSlideshowController::TransitionEnd()
{
    if (isDiaporama)
    {
        CMainParam* viewerParam = CMainParamInit::getInstance();
        if (viewerParam != nullptr)
        {
            const int timeDelai = viewerParam->GetDelaiDiaporamaOption();
            diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
        }

        if (musicController != nullptr && musicController->IsMusicPause())
        {
            musicController->SetMusicPause(false);
            musicController->StartMusic();
        }
    }
}

void CSlideshowController::OnTimerDiaporama(wxTimerEvent& /*event*/)
{
    if (viewerController != nullptr)
        viewerController->ImageSuivante();
}

void CSlideshowController::StartDiaporamaMessage(wxCommandEvent& /*event*/)
{
    CMainParam* viewerParam = CMainParamInit::getInstance();
    if (viewerParam != nullptr)
    {
        const int timeDelai = viewerParam->GetDelaiDiaporamaOption();
        diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
    }
}

void CSlideshowController::StopAnimationEvent(wxCommandEvent& /*event*/)
{
    if (isDiaporama)
    {
        CMainParam* viewerParam = CMainParamInit::getInstance();
        if (viewerParam != nullptr)
        {
            const int timeDelai = viewerParam->GetDelaiDiaporamaOption();
            diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
        }
    }
}
