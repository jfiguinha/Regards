#pragma once
class CFFmfc;

namespace Regards::Viewer
{
    class CMusicController
    {
    public:
        explicit CMusicController(wxWindow* eventSink);
        ~CMusicController();

        void StartMusic();
        void StopMusic();

        // Called from wxEvent handlers in CCentralWindow
        void OnStopAudio(wxCommandEvent& event);
        void OnQuitAudio(wxCommandEvent& event);

        bool IsMusicStop() const { return musicStop; }
        bool IsMusicPause() const { return musicPause; }
        void SetMusicPause(const bool& value) { musicPause = value; }

    private:
        wxWindow*  eventSink;   // used to receive async audio events
        CFFmfc*    ffmfc        = nullptr;
        bool       musicStop    = true;
        bool       ffmfcQuit    = false;
        bool       musicPause   = false;
        int64_t    musicPosition = 0;
    };
}
