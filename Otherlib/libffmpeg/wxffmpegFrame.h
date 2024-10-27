#pragma once
#include "wxFFmpegView.h"
#include <ShowElement.h>
#include <BitmapInterface.h>


using namespace Regards::Control;
using namespace Regards::Window;

class wxFfmpegFrame : public wxFrame, public Regards::Window::CBitmapInterface {
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
    void OnSetTimePosition(wxCommandEvent& event);

    void OnMute(wxCommandEvent& event);
    void OnPlus(wxCommandEvent& event);
    void OnMinus(wxCommandEvent& event);

    void TransitionEnd() {};
    void ImageSuivante() {};
    void ImagePrecedente() {};
   // void OnSize(wxSizeEvent& event);

#ifdef wxFFMPEG
    wxFFmpegView* ffmpegView;
#else
    CShowElement* showElement;
#endif
    
   
};

enum { ID_SelectFile = 1, ID_Play = 2, ID_Stop = 3, ID_Pause = 4, ID_Start = 5, ID_Mute = 6, ID_Plus = 7, ID_Minus = 8, ID_SetTimePos = 9};