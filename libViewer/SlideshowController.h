#pragma once
namespace Regards::Window { class CWindowManager; }


namespace Regards::Viewer
{
    class CMusicController;
    class CViewerController;   // forward — SlideshowController calls ImageSuivante
    class CPreviewWnd;

    class CSlideshowController
    {
    public:
        CSlideshowController(wxWindow*                      eventSink,
                             CPreviewWnd*                   previewWindow,
                             CMusicController*              musicController,
                             CViewerController*             viewerController);
        ~CSlideshowController();

        void StartDiaporama(wxCommandEvent& event);
        void StopDiaporama(wxCommandEvent& event);

        // Called by CPreviewWnd when a cross-fade transition finishes
        void TransitionEnd();

        bool IsDiaporamaStart() const { return isDiaporama; }

        // Event dispatched by an animation timer stop
        void StopAnimationEvent(wxCommandEvent& event);

        // Received from the main viewer window to (re)start the inter-slide timer
        void StartDiaporamaMessage(wxCommandEvent& event);

        // wxEVT_TIMER handler for diaporamaTimer
        void OnTimerDiaporama(wxTimerEvent& event);

    private:
        wxWindow*          eventSink       = nullptr;
        CPreviewWnd*       previewWindow   = nullptr;
        CMusicController*  musicController = nullptr;
        CViewerController* viewerController = nullptr;

        bool       isDiaporama  = false;
        wxTimer*   diaporamaTimer = nullptr;
    };
}
