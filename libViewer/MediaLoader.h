#pragma once
class CImageLoadingFormat;
class CImageVideoThumbnail;


namespace Regards::Viewer
{
    class CThumbnailViewerPicture;
    class CThumbnailViewerVideo;
    class CMusicController;
    class CPreviewWnd;
    class CPanelInfosWnd;
    // ── Thread transfer objects ──────────────────────────────────────
    class CThreadPictureData
    {
    public:
        CThreadPictureData()
            : mainWindow(nullptr), isVisible(false), myThread(nullptr)
            , isThumbnail(false), isDiaporama(false)
        {}
        wxWindow*            mainWindow;
        wxString             picture;
        bool                 isVisible;
        bool                 isThumbnail;
        bool                 isDiaporama;
        std::atomic<bool>    processLoadPicture{false};
        std::thread*         myThread;
    };

    // ── MediaLoader ──────────────────────────────────────────────────
    class CMediaLoader
    {
    public:
        CMediaLoader(wxWindow*                parent,
                     CPreviewWnd*             previewWindow,
                     CPanelInfosWnd*          panelInfosWindow,
                     CThumbnailViewerPicture* thumbnailPicture,
                     CThumbnailViewerVideo*   thumbnailVideo,
                     CMusicController*        musicController);
        ~CMediaLoader();

        // Main entry point — dispatches to SetVideo / SetAnimation / async picture load
        int  LoadPicture(const wxString& filename, const bool& refresh = false);

        // Called on wxEVENT_VIDEOEND
        void OnVideoEnd(wxCommandEvent& event);

        // Playback position (video seek or animation frame)
        void SetPosition(const long& timePosition);

        // Animation frame navigation
        void AnimationPictureNext();
        void AnimationPicturePrevious();

        // Animation timer tick
        void OnTimerAnimation(wxTimerEvent& event);

        // Animation seek event
        void AnimationSetPosition(wxCommandEvent& event);

        // Video position update event (VIDEO_UPDATE_ID)
        void SetVideoPos(wxCommandEvent& event);

        // Start / stop the animation timer
        void StartAnimation();
        void StopAnimation();

        // Tell thumbnailPicture to start/stop its loading indicator
        void StartLoadingPicture();
        void StopLoadingPicture();

        // Update the info panel for the current media
        void SetPanelInfos(const bool& isThumbnail);

        bool IsVideo() const { return isVideo; }
        bool IsAnimation() const { return isAnimation; }

        void LoadAnimationBitmap(const wxString& filename, const int& numFrame);

        // ── State exposed to ViewerController via references ──────────
        int  windowMode        = 1;
        bool isDiaporama       = false;
        bool isPicture         = false;
        bool isAnimation       = false;
        bool isVideo           = false;
        bool processLoadPicture = false;

    private:
        // Internal media setters
        void SetVideo(const wxString& path);
        bool SetAnimation(const wxString& filename);

        // Static thread entry point
        static void LoadingNewPicture(CThreadPictureData* pictureData);

        int GetPhotoId(const wxString& filename) const;

        // Weak references (owned by CCentralWindow)
        wxWindow*                parent           = nullptr;
        CPreviewWnd*             previewWindow    = nullptr;
        CPanelInfosWnd*          panelInfosWindow = nullptr;
        CThumbnailViewerPicture* thumbnailPicture = nullptr;
        CThumbnailViewerVideo*   thumbnailVideo   = nullptr;
        CMusicController*        musicController  = nullptr;

        // Animation state
        wxTimer*                           animationTimer       = nullptr;
        int                                animationPosition    = 0;
        int                                nbThumbnail          = 0;
        int                                oldAnimationPosition = -1;
        wxString                           oldFilename;
        std::vector<CImageVideoThumbnail*> videoThumbnail;

        // Playback state
        wxString filename;
        bool     loadPicture = false;
        bool     videoStart  = false;
        bool     stopVideo   = false;
        bool     isNext      = false;   // direction hint shared with ViewerController
    };
}
