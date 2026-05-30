#pragma once

namespace Regards::Window
{
    class CWindowManager;
    class CPanelWithClickToolbar;
}



namespace Regards::Viewer
{
    class CListPicture;
    class CListFace;
    class CPreviewWnd;
    class CPanelInfosWnd;

    class CWindowModeController
    {
    public:
        CWindowModeController(wxWindow*                            parent,
                              Regards::Window::CWindowManager*     windowManager,
                              Regards::Window::CPanelWithClickToolbar* panelInfosClick,
                              CPreviewWnd*                         previewWindow,
                              CPanelInfosWnd*                      panelInfosWindow,
                              CListPicture*                        listPicture,
                              CListFace*                           listFace,
                              const int&                           faceDetection);
        ~CWindowModeController() = default;

        // wxEVENT_SETMODEVIEWER
        void SetMode(wxCommandEvent& event);

        // Fullscreen toggle
        bool FullscreenMode();
        bool ScreenMode();
        bool IsCompatibleFullscreen() const;

        // Toolbar visibility (driven by mouse-hover in fullscreen)
        void HideToolbar();
        void ShowToolbar();

        // Called on DPI / screen-ratio change
        void UpdateScreenRatio();

        // Resize panel sizes after thumbnail icon size change
        void UpdateThumbnailIconeSize(wxCommandEvent& event);

        // Resize the window manager to fill the parent rect
        void Resize();

        // Save / restore pane states
        void SaveParameter();

        int  GetWindowMode()    const { return windowMode; }
        bool GetIsFullscreen()  const { return isFullscreen; }
        bool GetShowToolbar()   const { return showToolbar; }
        void ShowTopWindow();
        void HideTopWindow();
        // State flags shared with MediaLoader
        bool isVideo      = false;
        bool isAnimation  = false;

    private:
        wxWindow*                            parent           = nullptr;
        Regards::Window::CWindowManager*     windowManager    = nullptr;
        Regards::Window::CPanelWithClickToolbar* panelInfosClick = nullptr;
        CPreviewWnd*                         previewWindow    = nullptr;
        CPanelInfosWnd*                      panelInfosWindow = nullptr;
        CListPicture*                        listPicture      = nullptr;
        CListFace*                           listFace         = nullptr;

        int  windowMode    = 1;
        int  oldWindowMode = -1;
        bool isFullscreen  = false;
        bool showToolbar   = true;
        bool windowInit    = true;
        int  faceDetection = 0;
    };
}
