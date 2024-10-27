#include <header.h>
#include "wxffmpegFrame.h"
#include "wxFFmpegView.h"
#include <ShowElement.h>
#include <wx/filedlg.h>
#include <wx/timectrl.h>
#include <MainTheme.h>
#include <MainThemeInit.h>

using namespace Regards::Control;

wxFfmpegFrame::wxFfmpegFrame()
    : wxFrame(NULL,
        wxID_ANY,
        "Hello wxFFmpeg",
        wxDefaultPosition,
        wxSize(640, 480)) {
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_SelectFile,
        "&Open File...\tCtrl-H",
        "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* menuCommand = new wxMenu;
    wxMenu* menuSound = new wxMenu;

    menuCommand->Append(ID_Play,
        "Play",
        "Play movie");

    menuCommand->Append(ID_Pause,
        "Pause",
        "Pause movie");

    menuCommand->Append(ID_Start,
        "Start",
        "Start Movie");

    menuCommand->Append(ID_Stop,
        "Stop",
        "Stop movie");

    menuCommand->Append(ID_SetTimePos,
        "Time Position",
        "Time Position movie");

    menuSound->Append(ID_Mute,
        "Mute",
        "Mute Sound");

    menuSound->Append(ID_Plus,
        "Plus",
        "Plus Sound");

    menuSound->Append(ID_Minus,
        "Minus",
        "Minus Sound");

    menuBar->Append(menuFile, "&File");
#ifdef wxFFMPEG
    menuBar->Append(menuCommand, "&Command");
    menuBar->Append(menuSound, "&Sound");
#endif
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to wxFFmpeg!");
    Bind(wxEVT_MENU, &wxFfmpegFrame::OnSelectFile, this, ID_Play);
    Bind(wxEVT_MENU, &wxFfmpegFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &wxFfmpegFrame::OnExit, this, wxID_EXIT);

    Bind(wxEVT_MENU, &wxFfmpegFrame::OnSelectFile, this, ID_SelectFile);
    Bind(wxEVT_MENU, &wxFfmpegFrame::OnPlay, this, ID_Play);
    Bind(wxEVT_MENU, &wxFfmpegFrame::OnPause, this, ID_Pause);
    Bind(wxEVT_MENU, &wxFfmpegFrame::OnStart, this, ID_Start);
    Bind(wxEVT_MENU, &wxFfmpegFrame::OnStop, this, ID_Stop);
    Bind(wxEVT_MENU, &wxFfmpegFrame::OnSetTimePosition, this, ID_SetTimePos);

    Bind(wxEVT_MENU, &wxFfmpegFrame::OnMute, this, ID_Mute);
    Bind(wxEVT_MENU, &wxFfmpegFrame::OnPlus, this, ID_Plus);
    Bind(wxEVT_MENU, &wxFfmpegFrame::OnMinus, this, ID_Minus);

#ifdef wxFFMPEG
    ffmpegView = new wxFFmpegView(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(ffmpegView, 1, wxEXPAND);
    SetSizer(mainSizer);

#else

   // Connect(wxEVT_SIZE, wxSizeEventHandler(wxFfmpegFrame::OnSize));

    CMainTheme* viewerTheme = CMainThemeInit::getInstance();
    showElement = new CShowElement(this, SHOWBITMAPVIEWERID, BITMAPWINDOWVIEWERID, MAINVIEWERWINDOWID, this, viewerTheme, true);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(showElement, 1, wxEXPAND);
    SetSizer(mainSizer);
    showElement->Show(true);
#endif
}


void wxFfmpegFrame::OnSetTimePosition(wxCommandEvent& event)
{
    /*
    wxDialog dlg(this, wxID_ANY, "Time Picker Dialog",
        wxDefaultPosition, wxSize(200, 100));
    (new wxTimePickerCtrl(&dlg, wxID_ANY))->Centre();
    dlg.CentreOnParent();
    dlg.ShowModal();
    */
#ifdef wxFFMPEG
    ffmpegView->setTimePosition(90);
#endif
}

void wxFfmpegFrame::OnMute(wxCommandEvent& event)
{
#ifdef wxFFMPEG
    ffmpegView->mute();
#endif
}

void wxFfmpegFrame::OnPlus(wxCommandEvent& event)
{
#ifdef wxFFMPEG
    ffmpegView->plus();
#endif
}

void wxFfmpegFrame::OnMinus(wxCommandEvent& event)
{
#ifdef wxFFMPEG
    ffmpegView->minus();
#endif
}

void wxFfmpegFrame::OnPlay(wxCommandEvent& event)
{
#ifdef wxFFMPEG
    ffmpegView->play();
#endif
}

void wxFfmpegFrame::OnPause(wxCommandEvent& event)
{
#ifdef wxFFMPEG
    ffmpegView->pause();
#endif
}

void wxFfmpegFrame::OnStart(wxCommandEvent& event)
{
#ifdef wxFFMPEG
    ffmpegView->start();
#endif
}

void wxFfmpegFrame::OnStop(wxCommandEvent& event)
{
#ifdef wxFFMPEG
    ffmpegView->stop();
#endif
}


void wxFfmpegFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void wxFfmpegFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World",
        wxOK | wxICON_INFORMATION);
}

void wxFfmpegFrame::OnSelectFile(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, _("Open mp4 file"), "", "",
            "mp4 files (*.mp4)|*.mp4", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    wxString videoFile = openFileDialog.GetPath();

#ifdef wxFFMPEG
    ffmpegView->open(videoFile.ToStdString());
#else
    showElement->ShowToolbar();
    int rotation = 0;
    showElement->SetVideo(videoFile, rotation, true);
#endif

}