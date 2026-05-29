#include <header.h>
#include "ViewerNavigationController.h"
#include "MainWindow.h"
#include "window_mode_id.h"


using namespace Regards::Viewer;

CViewerNavigationController::CViewerNavigationController(wxEvtHandler* parent, CMainWindow* mainWindow)
    : parent_(parent), mainWindow_(mainWindow)
{
    // Les timers sont rattachés au parent (wxEvtHandler) pour que les callbacks
    // soient traités dans la boucle d'événements wxWidgets.
    // Le parent doit binder NAV_TIMER_LOADPICTURE et NAV_TIMER_LOADPICTUREEND
    // vers OnTimerNavigation() et OnTimerEndNavigation().
    loadPictureTimer_    = std::make_unique<wxTimer>(dynamic_cast<wxWindow*>(parent), TIMER_LOADPICTURE);
    endLoadPictureTimer_ = std::make_unique<wxTimer>(dynamic_cast<wxWindow*>(parent), TIMER_LOADPICTUREEND);
}

void CViewerNavigationController::OnPictureEndLoading()
{
    pictureEndLoading_ = true;
}

void CViewerNavigationController::OnLoadPicture(wxTimerEvent& event)
{
    //printf("void CViewerFrame::OnTimerLoadPicture(wxTimerEvent& event) \n");
    wxWindow* mainWindow = mainWindow_->FindWindowById(CENTRALVIEWERWINDOWID);
    if (mainWindow != nullptr)
    {
        wxCommandEvent evt(eventToLoop_);
        mainWindow->GetEventHandler()->AddPendingEvent(evt);
    }

    if (endLoadPictureTimer_->IsRunning())
        endLoadPictureTimer_->Stop();

    endLoadPictureTimer_->Start(1000, true);

    loadPictureTimer_->Stop();
}

void CViewerNavigationController::OnKeyUp(wxKeyEvent& event)
{
    if (loadPictureTimer_ && loadPictureTimer_->IsRunning())
        loadPictureTimer_->Stop();

    event.Skip();
}

void CViewerNavigationController::OnKeyDown(wxKeyEvent& event)
{
    const int keyCode = event.GetKeyCode();

    // ESC : quitter le plein écran quelle que soit l'état
    if (keyCode == WXK_ESCAPE)
    {
        mainWindow_->SetScreen();
        event.Skip();
        return;
    }

    switch (keyCode)
    {
    case WXK_SPACE:
    case WXK_PAGEUP:
        ScheduleNavigationEvent(wxEVENT_PICTURENEXT, true);
        break;

    case WXK_PAGEDOWN:
        ScheduleNavigationEvent(wxEVENT_PICTUREPREVIOUS, true);
        break;

    case WXK_END:
        ScheduleNavigationEvent(wxEVENT_PICTURELAST, false);
        break;

    case WXK_HOME:
        ScheduleNavigationEvent(wxEVENT_PICTUREFIRST, false);
        break;

    case WXK_F5:
        if (!fullscreen_)
        {
            if (mainWindow_->SetFullscreen())
                fullscreen_ = true;
        }
        break;

    case WXK_F2:
        SwitchWindowMode(WINDOW_FACE);
        break;

    case WXK_F3:
        SwitchWindowMode(WINDOW_EXPLORER);
        break;

    case WXK_F4:
        SwitchWindowMode(WINDOW_VIEWER);
        break;

    case WXK_F6:
        SwitchWindowMode(WINDOW_PICTURE);
        break;

    default:
        break;
    }

    event.Skip();
}

void CViewerNavigationController::ScheduleNavigationEvent(int eventType, bool repeatable)
{
    repeatEvent_ = repeatable;
    eventToLoop_ = eventType;

    if (pictureEndLoading_ && loadPictureTimer_)
        loadPictureTimer_->Start(50, true);

    pictureEndLoading_ = false;
}

void CViewerNavigationController::SwitchWindowMode(int modeId)
{
    // On envoie l'événement directement au parent qui le routera
    wxCommandEvent evt(wxEVENT_SETMODEVIEWER);
    evt.SetInt(modeId);
    wxPostEvent(parent_, evt);
}
