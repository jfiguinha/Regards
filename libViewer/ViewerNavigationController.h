#pragma once

static constexpr auto TIMER_LOADPICTURE = 2;
static constexpr int TIMER_EVENTFILEFS = 3;
static constexpr auto TIMER_LOADPICTUREEND = 4;
static constexpr int TIMER_LOADPICTURESTART = 5;

namespace Regards::Viewer
{
    class CMainWindow;
    // Gère la navigation clavier (image suivante/précédente, plein écran, modes d'affichage)
    // et les timers associés au chargement des images.
    class CViewerNavigationController
    {
    public:
        // parent    : handler qui recevra les événements propagés
        // mainWindow: fenêtre principale, cible des commandes de navigation
        CViewerNavigationController(wxEvtHandler* parent, Regards::Viewer::CMainWindow* mainWindow);

        void OnKeyDown(wxKeyEvent& event);
        void OnKeyUp(wxKeyEvent& event);

        // Appelé quand le chargement d'une image est terminé (réarme la navigation).
        void OnPictureEndLoading();
        void OnLoadPicture(wxTimerEvent& event);
        bool IsFullscreen() const { return fullscreen_; }
        void SetFullscreen(bool value) { fullscreen_ = value; }

    private:
        void ScheduleNavigationEvent(int eventType, bool repeatable);
        void TriggerNavigationEvent();
        void SwitchWindowMode(int modeId);

        wxEvtHandler*  parent_     = nullptr;
        CMainWindow*   mainWindow_ = nullptr;

        std::unique_ptr<wxTimer> loadPictureTimer_;
        std::unique_ptr<wxTimer> endLoadPictureTimer_;

        bool pictureEndLoading_ = true;
        bool fullscreen_        = false;
        bool repeatEvent_       = false;
        int  eventToLoop_       = 0;
    };
}
