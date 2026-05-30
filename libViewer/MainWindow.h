#pragma once
#include <WindowMain.h>
#include <ToolbarInterface.h>
#include "ThumbnailScheduler.h"
#include "FolderRefreshService.h"
#include "MainViewerController.h"
#include <memory>

class IStatusBarInterface;

namespace Regards::Viewer
{
    class CToolbarViewerMode;
    class CCentralWindow;
    class CMainParam;
    class CFolderProcess;
    class CThumbnailProcess;
    class CCategoryFolderWindow;
    class CImageList;
    class CExportDiaporama;
    class CToolbar;
    class CMainParam;
    class CCentralWindow;
    class CToolbarViewerMode;
    class CFolderProcess;
    class CThumbnailProcess;
    class CCategoryFolderWindow;

    class CMainWindow : public Regards::Window::CWindowMain, public Regards::Window::CToolbarInterface
    {
    public:
        CMainWindow(wxWindow* parent, wxWindowID id,
                    IStatusBarInterface* statusbar,
                    const wxString& fileToOpen);
        ~CMainWindow() override;

        void UpdateScreenRatio() override;
        void SaveParameter()     override;
        bool GetProcessEnd()     override;

        // ── API publique ─────────────────────────────────────────────
        bool     SetFullscreen();
        bool     SetFullscreenMode();
        bool     SetScreen();
        bool     OpenFolder(const wxString& path);
        bool     IsFullscreen();
        bool     IsVideo();
        void     ShowToolbar();
        void     TransitionEnd();
        void     SetPictureMode();
        void     SetViewerMode();
        void     OpenFile(const wxString& fileToOpen);
        wxString GetFilename();

        // Délégués à ViewerController (thread-safe via événements)
        void SetText(const int& numPos, const wxString& libelle);
        void SetRangeProgressBar(const int& range);
        void SetPosProgressBar(const int& position);

        bool GetInit() const noexcept { return folderService->GetInit(); }

        void OnOpenFileOrFolder(wxCommandEvent& event);

    private:
        // ── Initialisation ────────────────────────────────────────────
        void InitState();
        void InitTheme();
        void InitUI(IStatusBarInterface* statusbar);
        void BindEvents();
        void InitConfig(const wxString& fileToOpen);
        void InitBackgroundTasks();

        // ── Idle / boucle principale ──────────────────────────────────
        void ProcessIdle()  override;
        void IdleFunction() override;
        void Resize()       override;

        // ── Handlers d'événements ─────────────────────────────────────
        void OnOpenFile(wxTimerEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnUpdateInfos(wxCommandEvent& event);
        void OnShowToolbar(wxCommandEvent& event);
        void OnStatusSetText(wxCommandEvent& event);
        void OnEndCheckFile(wxCommandEvent& event);
        void OnSetRangeProgressBar(wxCommandEvent& event);
        void OnSetValueProgressBar(wxCommandEvent& event);
        void OnCriteriaUpdate(wxCommandEvent& event);
        void OnRefreshPicture(wxCommandEvent& event);
        void OnScanner(wxCommandEvent& event);
        void OnFaceAdd(wxCommandEvent& event);
        void OnVersionUpdate(wxCommandEvent& event);
        void UpdateStatusBarMessage(wxCommandEvent& event);
        void OnDeleteFace(wxCommandEvent& event);
        void InitPictures(wxCommandEvent& event);
        void PictureVideoClick(wxCommandEvent& event);
        void CriteriaChange(wxCommandEvent& event);
        void OnPrint(wxCommandEvent& event);
        void OnPictureClick(wxCommandEvent& event);
        void PrintPreview(wxCommandEvent& event);
        void OnRemoveFileFromCheckIn(wxCommandEvent& event);
        void OnCheckInUpdateStatus(wxCommandEvent& event);
        void OnFaceInfosUpdate(wxCommandEvent& event);
        void OnExportFile(wxCommandEvent& event);
        void OnUpdateExifThumbnail(wxCommandEvent& event);
        void SetScreenEvent(wxCommandEvent& event);
        void OnExportDiaporama(wxCommandEvent& event);
        void RefreshFolderList(wxCommandEvent& event);
        void OnUpdateFolder(wxCommandEvent& event);
        void OnRefreshThumbnail(wxCommandEvent& event);
        void OnProcessThumbnail(wxCommandEvent& event);
        void UpdateMessage(wxCommandEvent& event);
        void UpdateThumbnailIcone(wxCommandEvent& event);
        void ClickShowButton(const int& id, const int& refresh);

        void OnEditFile(wxCommandEvent& event);

        static void NewVersionAvailable(void* param);

        void SetDataToStatusBar(void* thumbnailMessage, const wxString& message);

        // ── Widgets UI ────────────────────────────────────────────────
        wxGauge*            progressBar        = nullptr;
        wxStatusBar*        statusBar          = nullptr;
        CCentralWindow*     centralWnd         = nullptr;
        CToolbarViewerMode* toolbarViewerMode  = nullptr;
        IStatusBarInterface* statusBarViewer   = nullptr;
        wxTimer*            loadPictureStartTimer = nullptr;

        // ── Services délégués ─────────────────────────────────────────
        std::unique_ptr<ThumbnailScheduler>   scheduler;
        std::unique_ptr<FolderRefreshService> folderService;
        std::unique_ptr<CMainViewerController>     viewerCtrl;

        // ── Processus de fond ─────────────────────────────────────────
        std::unique_ptr<CFolderProcess>   folderProcess;
        std::unique_ptr<CThumbnailProcess> thumbnailProcess;
        std::thread* versionUpdate = nullptr;

        // ── État général ──────────────────────────────────────────────
        CMainParam* viewerParam     = nullptr;
        bool        multithread     = true;
        bool        needToReload    = false;
        int         typeAffichage   = 0;
        bool        updateCriteria  = true;
        bool        updateFolder    = false;
        bool        refreshFolder   = false;
        bool        start           = true;
        bool        criteriaSendMessage = false;
        bool        checkVersion    = true;
        bool        setViewerMode   = false;
        bool        setPictureMode  = false;
        bool        isCheckingFile  = false;
        bool        isCheckNewVersion = false;
        int         faceDetection   = 0;
        bool        isThumbnailProcess = false;
        int         nbElementInIconeList = 0;
        int         nbPhotoElement  = 0;
        int         nbElement       = 0;
        wxString    tempVideoFile;
        wxString    tempAudioVideoFile;
        wxRect      posWindow;
    };

} // namespace Regards::Viewer
