#pragma once
class IStatusBarInterface;
class CImageLoadingFormat;

namespace Regards::Viewer
{
    class CCentralWindow;
    class CToolbarViewerMode;

    /**
     * ViewerController
     *
     * Responsabilités :
     *   - Gérer le mode plein écran / fenêtré (fullscreen / SetScreen)
     *   - Piloter la visibilité de la toolbar et de la barre de statut
     *   - Déléguer les actions utilisateur (Print, Export, Edit, Diaporama…)
     *   - Exposer les méthodes de navigation (IsVideo, GetFilename)
     *   - Mettre à jour la barre de statut (texte, progress bar, titre fenêtre)
     */
    class CMainViewerController
    {
    public:
        CMainViewerController(CCentralWindow*    centralWnd,
                              CToolbarViewerMode* toolbarViewerMode,
                              wxStatusBar*        statusBar,
                         wxGauge*            progressBar,
                         IStatusBarInterface* statusBarViewer,
                         wxWindow*            eventSink);

        // ── Plein écran ──────────────────────────────────────────────────
        bool IsFullscreen() const;

        /** Vérifie la compatibilité et demande le plein écran. */
        bool SetFullscreen();

        /** Active le mode plein écran (cache toolbar + statusbar). */
        bool SetFullscreenMode();

        /** Revient en mode fenêtré. */
        bool SetScreen();

        // ── Toolbar ──────────────────────────────────────────────────────
        void ShowToolbar();
        bool IsToolbarVisible() const { return showToolbar; }

        // ── Lecture courante ─────────────────────────────────────────────
        bool     IsVideo() const;
        wxString GetFilename() const;
        void     LoadPicture(const wxString& filename, bool forceReload = false);
        void     SetPosition(long timePosition);
        void     TransitionEnd();
        void     UpdateScreenRatio();

        // ── Actions utilisateur ──────────────────────────────────────────
        void OnPrint(const wxString& localFilename);
        void OnExportFile(const wxString& localFilename);
        void OnEditFile(wxString& localFilename);           ///< peut modifier localFilename
        void OnExportDiaporama();

        // ── Barre de statut ──────────────────────────────────────────────
        void SetText(int numPos, const wxString& libelle);
        void SetRangeProgressBar(int range);
        void SetPosProgressBar(int position);
        void UpdateStatusBarText(const wxString& text, int position);
        void UpdateProgressRange(int range);
        void UpdateProgressValue(int position);

        // ── Mode viewer ──────────────────────────────────────────────────
        /** Envoie wxEVENT_SETMODEVIEWER à CENTRALVIEWERWINDOWID. */
        void SetViewerModeEvent(int mode);

    private:
        CCentralWindow*     centralWnd;
        CToolbarViewerMode* toolbarViewerMode;
        wxStatusBar*        statusBar;
        wxGauge*            progressBar;
        IStatusBarInterface* statusBarViewer;
        wxWindow*            eventSink;   ///< CMainWindow — pour FindWindowById

        bool fullscreen  = false;
        bool showToolbar = true;
    };

} // namespace Regards::Viewer
