#pragma once
#include <WindowMain.h>
using namespace std;
using namespace Regards::Window;

class CImageLoadingFormat;

namespace Regards::Window
{
    class CPanelWithClickToolbar;
    class CScrollbarWnd;
    class CWindowManager;
}

namespace Regards::Viewer
{
    class CPanelPhotoWnd;
    class CListFace;
    class CListPicture;
    class CThumbnailViewerVideo;
    class CPanelInfosWnd;
    class CPreviewWnd;
    class CThumbnailViewerPicture;

    // ── New sub-controllers ──
    class CViewerController;
    class CSlideshowController;
    class CMediaLoader;
    class CWindowModeController;
    class CMusicController;
    class CThumbnailController;

    class CCentralWindow : public CWindowMain
    {
    public:
        CCentralWindow(wxWindow* parent, wxWindowID id, const CThemeSplitter& theme,
                       const bool& horizontal = true);
        ~CCentralWindow() override;

        // ── Public API (unchanged from original) ────────────────────────
        wxString ImageSuivante(const bool& loadPicture = true);
        wxString ImagePrecedente(const bool& loadPicture = true);
        wxString ImageFin(const bool& loadPicture = true);
        wxString ImageDebut(const bool& loadPicture = true);

        wxString GetFilename();
        int      GetNbElement();

        void UpdateScreenRatio() override;
        bool FullscreenMode();
        bool ScreenMode();
        void HideToolbar();
        void ShowToolbar();
        void AnimationPictureNext();
        void AnimationPicturePrevious();
        void SetPosition(const long& timePosition);
        void SetListeFile(const wxString& filename, const bool& isDeleteFolder,
                          const bool& isSqlUpdate, const int& typeAffichage);
        int  LoadPicture(const wxString& filename, const bool& refresh = false);
        bool IsVideo();
        void SaveParameter() override;
        bool IsCompatibleFullscreen();
        void TransitionEnd();
        bool IsDiaporamaStart();

    private:
        bool GetProcessEnd() override;
        void Resize()        override;

        // ── Event handlers (thin forwarders to sub-controllers) ──────────
        void OnVideoEnd(wxCommandEvent& event);
        void OnVideoStart(wxCommandEvent& event);
        void OnVideoStop(wxCommandEvent& event);
        void OnAnimationStart(wxCommandEvent& event);
        void OnAnimationStop(wxCommandEvent& event);
        void OnTimerAnimation(wxTimerEvent& event);
        void OnTimerDiaporama(wxTimerEvent& event);
        void OnShowPicture(wxCommandEvent& event);
        void OnPicturePrevious(wxCommandEvent& event);
        void OnPictureNext(wxCommandEvent& event);
        void OnPictureFirst(wxCommandEvent& event);
        void OnPictureLast(wxCommandEvent& event);
        void StartDiaporama(wxCommandEvent& event);
        void StopDiaporama(wxCommandEvent& event);
        void StartDiaporamaMessage(wxCommandEvent& event);
        void StopAnimationEvent(wxCommandEvent& event);
        void UpdateThumbnailIcone(wxCommandEvent& event);
        void UpdateThumbnailIconeSize(wxCommandEvent& event);
        void OnRefreshThumbnail(wxCommandEvent& event);
        void SetMode(wxCommandEvent& event);
        void SetVideoPos(wxCommandEvent& event);
        void AnimationSetPosition(wxCommandEvent& event);
        void ChangeTypeAffichage(wxCommandEvent& event);
        void OnStopAudio(wxCommandEvent& event);
        void OnQuitAudio(wxCommandEvent& event);
		void OnEndLoadPicture(wxCommandEvent& event);
       

        // ── UI widgets (still owned here, passed by pointer to sub-controllers)
        std::unique_ptr<CListPicture>         listPicture      = nullptr;
        std::unique_ptr<CThumbnailViewerPicture> thumbnailPicture = nullptr;
        std::unique_ptr<CPanelPhotoWnd>          panelPhotoWnd    = nullptr;
        CPanelWithClickToolbar *  panelInfosClick  = nullptr;
#ifndef __NOFACE_DETECTION__
        std::unique_ptr<CListFace>               listFace         = nullptr;
#endif
        std::unique_ptr<CScrollbarWnd>           scrollVideoWindow   = nullptr;
        std::unique_ptr<CThumbnailViewerVideo>   thumbnailVideo      = nullptr;
        std::unique_ptr<CPanelInfosWnd>          panelInfosWindow    = nullptr;
        std::unique_ptr<CPreviewWnd>             previewWindow       = nullptr;
        std::unique_ptr<CScrollbarWnd>           scrollPictureWindow = nullptr;
        std::unique_ptr<CWindowManager>          windowManager       = nullptr;

        // ── Sub-controllers ─────────────────────────────────────────────
        std::unique_ptr<CMusicController>        musicController      = nullptr;
        std::unique_ptr<CThumbnailController>    thumbnailController  = nullptr;
        std::unique_ptr<CMediaLoader>            mediaLoader          = nullptr;
        std::unique_ptr<CViewerController>      viewerController     = nullptr;
        std::unique_ptr<CSlideshowController>    slideshowController  = nullptr;
        std::unique_ptr<CWindowModeController>       windowModeController = nullptr;

        int  faceDetection = 0;
    };
}
