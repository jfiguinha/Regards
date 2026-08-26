#pragma once

#include <wx/wx.h>
#include <memory>
#include "ffmfcpimpl.h"

class CVideoControlInterface;

class CFFmfc : public wxWindow
{
public:
    CFFmfc(wxWindow* parent, wxWindowID id);
    ~CFFmfc() override;

    CFFmfc(const CFFmfc&) = delete;
    CFFmfc& operator=(const CFFmfc&) = delete;

    CFFmfc(CFFmfc&&) = delete;
    CFFmfc& operator=(CFFmfc&&) = delete;

    void RefreshEvent(wxCommandEvent& event);
    void SeekBarEvent(wxCommandEvent& event);
    void PositionSeekEvent(wxCommandEvent& event);
    void PositionEvent(wxCommandEvent& event);

    void ChangeVolumeEvent(wxCommandEvent& event);
    void ChangeAudioEvent(wxCommandEvent& event);
    void ChangeSubtitleEvent(wxCommandEvent& event);

    void AspectEvent(wxCommandEvent& event);

    void ExitEvent(wxCommandEvent& event);
    void QuitEvent(wxCommandEvent& event);
    void StopEvent(wxCommandEvent& event);
    void StepEvent(wxCommandEvent& event);
    void PauseEvent(wxCommandEvent& event);
    void PlayEvent(wxCommandEvent& event);

    void SetOutputMode(int outputMode);
    int Reset_index();

    void VideoDisplaySize(int width, int height);
    void SetVideoParameter(int angle, int flipV, int flipH);

    bool Quit();

    void Seek_step();
    void Pause();
    void Play();

    void Aspectratio(int num, int den);
    void Size(int percentage);

    void Change_audio_stream(int newStreamIndex);
    void Change_subtitle_stream(int newStreamIndex);

    void VolumeUp();
    void VolumeDown();

    void SetVolume(const int& pos);
    int GetVolume();

    int64_t GetTimePosition();
    void SetTimePosition(int64_t time);

    void Seek(int time);
    void Seek_bar(int pos);

    wxString Getfilename();

    int SetFile(CVideoControlInterface* control,
        const wxString& filename,
        const wxString& acceleratorHardware,
        const bool& isOpenGLDecoding,
        const int& volume);

private:
    bool IsReady() const noexcept;

private:
    std::unique_ptr<CFFmfcPimpl> _pimpl;
    CFFmfcPimpl::VideoState* cur_stream = nullptr;
    wxString filename;
};