#pragma once

#include <wx/panel.h>

class wxFFmpegInnerView;
class wxFFmpegView : public wxPanel {
public:
    wxFFmpegView(wxWindow *parent,
                 wxWindowID winid = wxID_ANY,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = 0,
                 const wxString &name = _T("wxFFmpegView"));
    ~wxFFmpegView();

    void open(std::string filename);
    void pause();
    void start();
    void play();
    void stop();
    void close();

private:
    wxFFmpegInnerView *innerView_;
};
