#pragma once
#include "wxFFmpegView.h"

class wxFfmpegFrame : public wxFrame {
public:
    wxFfmpegFrame();

private:
    void OnSelectFile(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnPlay(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnStart(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);

    wxFFmpegView* ffmpegView;
};

enum { ID_SelectFile = 1, ID_Play = 2, ID_Stop = 3, ID_Pause = 4, ID_Start = 5 };