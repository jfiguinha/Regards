#include <header.h>

#include "ffplaycore.h"
#include <window_id.h>
#include <ConvertUtility.h>

#include <cmath>


CFFmfc::CFFmfc(wxWindow* parent, wxWindowID id)
    : wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0),
    _pimpl(nullptr)
{
    Connect(FF_EXIT_EVENT, wxCommandEventHandler(CFFmfc::ExitEvent));
    Connect(FF_QUIT_EVENT, wxCommandEventHandler(CFFmfc::QuitEvent));
    Connect(FF_STOP_EVENT, wxCommandEventHandler(CFFmfc::StopEvent));
    Connect(FF_STEP_EVENT, wxCommandEventHandler(CFFmfc::StepEvent));
    Connect(FF_PAUSE_EVENT, wxCommandEventHandler(CFFmfc::PauseEvent));
    Connect(FF_PLAY_EVENT, wxCommandEventHandler(CFFmfc::PlayEvent));
    Connect(FF_ASPECT_EVENT, wxCommandEventHandler(CFFmfc::AspectEvent));

    Connect(CHANGE_AUDIO, wxCommandEventHandler(CFFmfc::ChangeAudioEvent));
    Connect(CHANGE_SUBTITLE, wxCommandEventHandler(CFFmfc::ChangeSubtitleEvent));
    Connect(VOLUME_EVENT, wxCommandEventHandler(CFFmfc::ChangeVolumeEvent));
    Connect(SET_POSITION, wxCommandEventHandler(CFFmfc::PositionEvent));
    Connect(SET_SEEKPOSITION, wxCommandEventHandler(CFFmfc::PositionSeekEvent));
    Connect(SEEK_BAR_EVENT, wxCommandEventHandler(CFFmfc::SeekBarEvent));
    Connect(FF_REFRESH_EVENT, wxCommandEventHandler(CFFmfc::RefreshEvent));
}


CFFmfc::~CFFmfc()
{
    /*
     * Les événements wxWidgets sont automatiquement déconnectés
     * lors de la destruction de la fenêtre.
     *
     * La destruction de _pimpl est automatiquement effectuée par
     * std::unique_ptr.
     */
}


// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------

bool CFFmfc::IsReady() const noexcept
{
    return _pimpl != nullptr &&
        _pimpl->g_is != nullptr &&
        _pimpl->exit_remark == 0;
}


// ------------------------------------------------------------
// Refresh
// ------------------------------------------------------------

void CFFmfc::RefreshEvent(wxCommandEvent&)
{
    if (!IsReady() || cur_stream == nullptr)
        return;

    double remaining_time = 0.0;

    _pimpl->video_refresh(cur_stream, &remaining_time);
}


// ------------------------------------------------------------
// Seek bar
// ------------------------------------------------------------

void CFFmfc::SeekBarEvent(wxCommandEvent&)
{
    if (!IsReady() || cur_stream == nullptr)
        return;

    if (_pimpl->g_is->ic == nullptr)
        return;

    if (_pimpl->seek_by_bytes ||
        _pimpl->g_is->ic->duration <= 0)
    {
        const int64_t size = avio_size(_pimpl->g_is->ic->pb);

        if (size < 0)
            return;

        const int64_t position =
            size * _pimpl->seek_bar_pos / 1000;

        _pimpl->stream_seek(
            cur_stream,
            position,
            0,
            1);
    }
    else
    {
        const double frac =
            static_cast<double>(_pimpl->seek_bar_pos) / 1000.0;

        int64_t ts =
            static_cast<int64_t>(
                frac * _pimpl->g_is->ic->duration);

        if (_pimpl->g_is->ic->start_time != AV_NOPTS_VALUE)
            ts += _pimpl->g_is->ic->start_time;

        _pimpl->stream_seek(
            cur_stream,
            ts,
            0,
            0);
    }
}


// ------------------------------------------------------------
// Seek relative
// ------------------------------------------------------------

void CFFmfc::PositionSeekEvent(wxCommandEvent& event)
{
    if (!IsReady() || cur_stream == nullptr)
        return;

    double incr = 10.0;

    switch (event.GetInt())
    {
    case -10:
        incr = -10.0;
        break;

    case 10:
        incr = 10.0;
        break;

    case -60:
        incr = -60.0;
        break;

    case 60:
        incr = 60.0;
        break;

    case -600:
        incr = -600.0;
        break;

    case 600:
        incr = 600.0;
        break;

    default:
        incr = 10.0;
        break;
    }

    double pos = 0.0;

    if (_pimpl->seek_by_bytes)
    {
        pos = -1.0;

        if (cur_stream->video_stream >= 0)
            pos = _pimpl->frame_queue_last_pos(
                &cur_stream->pictq);

        if (pos < 0.0 && cur_stream->audio_stream >= 0)
            pos = _pimpl->frame_queue_last_pos(
                &cur_stream->sampq);

        if (pos < 0.0 && cur_stream->ic != nullptr &&
            cur_stream->ic->pb != nullptr)
        {
            pos = static_cast<double>(
                avio_tell(cur_stream->ic->pb));
        }

        if (pos < 0.0)
            return;

        if (cur_stream->ic != nullptr &&
            cur_stream->ic->bit_rate != 0)
        {
            incr *= cur_stream->ic->bit_rate / 8.0;
        }
        else
        {
            incr *= 180000.0;
        }

        pos += incr;

        _pimpl->stream_seek(
            cur_stream,
            static_cast<int64_t>(pos),
            static_cast<int64_t>(incr),
            1);
    }
    else
    {
        pos = _pimpl->get_master_clock(cur_stream);

        if (std::isnan(pos))
        {
            pos = static_cast<double>(
                cur_stream->seek_pos) / AV_TIME_BASE;
        }

        pos += incr;

        if (cur_stream->ic != nullptr &&
            cur_stream->ic->start_time != AV_NOPTS_VALUE)
        {
            const double start =
                cur_stream->ic->start_time /
                static_cast<double>(AV_TIME_BASE);

            if (pos < start)
                pos = start;
        }

        _pimpl->stream_seek(
            cur_stream,
            static_cast<int64_t>(
                pos * AV_TIME_BASE),
            static_cast<int64_t>(
                incr * AV_TIME_BASE),
            0);
    }
}


// ------------------------------------------------------------
// Absolute position
// ------------------------------------------------------------

void CFFmfc::PositionEvent(wxCommandEvent& event)
{
    auto* ts =
        static_cast<int64_t*>(event.GetClientData());

    if (ts == nullptr)
        return;

    const int64_t position = *ts;

    delete ts;

    if (!IsReady() ||
        cur_stream == nullptr ||
        _pimpl->g_is->ic == nullptr)
    {
        return;
    }

    int64_t seekPosition = position;

    if (_pimpl->g_is->ic->start_time != AV_NOPTS_VALUE)
        seekPosition += _pimpl->g_is->ic->start_time;

    _pimpl->stream_seek(
        cur_stream,
        seekPosition,
        0,
        0);
}


// ------------------------------------------------------------
// Volume
// ------------------------------------------------------------

void CFFmfc::ChangeVolumeEvent(wxCommandEvent& event)
{
    if (_pimpl == nullptr ||
        _pimpl->exit_remark != 0)
    {
        return;
    }

    _pimpl->percentVolume = event.GetInt();
}


void CFFmfc::ChangeAudioEvent(wxCommandEvent& event)
{
    if (!IsReady() || cur_stream == nullptr)
        return;

    _pimpl->stream_change_stream(
        cur_stream,
        AVMEDIA_TYPE_AUDIO,
        event.GetInt());
}


void CFFmfc::ChangeSubtitleEvent(wxCommandEvent& event)
{
    if (!IsReady() || cur_stream == nullptr)
        return;

    _pimpl->stream_change_stream(
        cur_stream,
        AVMEDIA_TYPE_SUBTITLE,
        event.GetInt());
}


// ------------------------------------------------------------
// Aspect
// ------------------------------------------------------------

void CFFmfc::AspectEvent(wxCommandEvent& event)
{
    auto* size =
        static_cast<wxSize*>(event.GetClientData());

    if (size == nullptr)
        return;

    const wxSize newSize = *size;

    delete size;

    if (!IsReady())
        return;

    _pimpl->screen_width =
        _pimpl->g_is->width =
        newSize.x;

    _pimpl->screen_height =
        _pimpl->g_is->height =
        newSize.y;

    _pimpl->g_is->force_refresh = 1;
}


// ------------------------------------------------------------
// Exit
// ------------------------------------------------------------

void CFFmfc::ExitEvent(wxCommandEvent&)
{
    if (_pimpl == nullptr)
        return;

    _pimpl->do_exit(cur_stream);
}


// ------------------------------------------------------------
// Quit
// ------------------------------------------------------------

void CFFmfc::QuitEvent(wxCommandEvent&)
{
    if (GetParent() == nullptr)
        return;

    wxCommandEvent evt(wxEVENT_ENDVIDEOTHREAD);

    wxPostEvent(
        GetParent()->GetEventHandler(),
        evt);
}


void CFFmfc::StopEvent(wxCommandEvent&)
{
    if (GetParent() == nullptr)
        return;

    wxCommandEvent evt(wxEVENT_STOPVIDEO);

    wxPostEvent(
        GetParent()->GetEventHandler(),
        evt);
}


// ------------------------------------------------------------
// Playback
// ------------------------------------------------------------

void CFFmfc::StepEvent(wxCommandEvent&)
{
    if (!IsReady() || cur_stream == nullptr)
        return;

    _pimpl->step_to_next_frame(cur_stream);
}


void CFFmfc::PauseEvent(wxCommandEvent&)
{
    if (!IsReady() || cur_stream == nullptr)
        return;

    _pimpl->toggle_pause(cur_stream);
}


void CFFmfc::PlayEvent(wxCommandEvent&)
{
    if (!IsReady() || cur_stream == nullptr)
        return;

    _pimpl->toggle_play(cur_stream);
}


// ------------------------------------------------------------
// Configuration
// ------------------------------------------------------------

void CFFmfc::SetOutputMode(int outputMode)
{
    if (_pimpl == nullptr)
        return;

    _pimpl->videoOutputMode = outputMode;
}


int CFFmfc::Reset_index()
{
    if (_pimpl == nullptr)
        return -1;

    _pimpl->vframe_index = 0;
    _pimpl->aframe_index = 0;
    _pimpl->packet_index = 0;

    return 0;
}


void CFFmfc::VideoDisplaySize(int width, int height)
{
    if (_pimpl == nullptr ||
        _pimpl->g_is == nullptr)
    {
        return;
    }

    _pimpl->g_is->width = width;
    _pimpl->g_is->height = height;
}


void CFFmfc::SetVideoParameter(
    int angle,
    int flipV,
    int flipH)
{
    if (_pimpl == nullptr)
        return;

    _pimpl->video_angle = angle;
    _pimpl->video_flipV = flipV;
    _pimpl->video_flipH = flipH;
}


// ------------------------------------------------------------
// Quit
// ------------------------------------------------------------

bool CFFmfc::Quit()
{
    if (_pimpl == nullptr)
        return true;

    if (_pimpl->exit_remark != 0)
        return true;

    bool isExitNow = false;

    _pimpl->StopStream();

    if (_pimpl->g_is != nullptr)
    {
        wxCommandEvent evt(FF_EXIT_EVENT);

        evt.SetClientData(cur_stream);

        wxPostEvent(
            this,
            evt);
    }
    else
    {
        _pimpl->do_exit(nullptr);
        isExitNow = true;
    }

    return isExitNow;
}


// ------------------------------------------------------------
// Commands
// ------------------------------------------------------------

void CFFmfc::Seek_step()
{
    wxCommandEvent evt(FF_STEP_EVENT);
    wxPostEvent(this, evt);
}


void CFFmfc::Pause()
{
    wxCommandEvent evt(FF_PAUSE_EVENT);
    wxPostEvent(this, evt);
}


void CFFmfc::Play()
{
    wxCommandEvent evt(FF_PLAY_EVENT);
    wxPostEvent(this, evt);
}


// ------------------------------------------------------------
// Aspect ratio
// ------------------------------------------------------------

void CFFmfc::Aspectratio(int num, int den)
{
    if (!IsReady() || den == 0)
        return;

    const int h = _pimpl->g_is->height;

    if (h <= 0)
        return;

    const int w = h * num / den;

    auto* size = new wxSize(w, h);

    wxCommandEvent evt(FF_ASPECT_EVENT);
    evt.SetClientData(size);

    wxPostEvent(this, evt);
}


// ------------------------------------------------------------
// Size
// ------------------------------------------------------------

void CFFmfc::Size(int percentage)
{
    if (!IsReady() ||
        _pimpl->g_is->ic == nullptr ||
        _pimpl->g_is->video_stream < 0)
    {
        return;
    }

    AVStream* stream =
        _pimpl->g_is->ic->streams[
            _pimpl->g_is->video_stream];

    if (stream == nullptr ||
        stream->codecpar == nullptr)
    {
        return;
    }

    int width = stream->codecpar->width;
    int height = stream->codecpar->height;

    /*
     * L'ancien code ignorait percentage.
     *
     * On conserve le comportement historique si percentage
     * est invalide ou égal à 0.
     */
    if (percentage > 0)
    {
        width = width * percentage / 100;
        height = height * percentage / 100;
    }

    auto* size = new wxSize(width, height);

    wxCommandEvent evt(FF_ASPECT_EVENT);
    evt.SetClientData(size);

    wxPostEvent(this, evt);
}


// ------------------------------------------------------------
// Audio / Subtitle
// ------------------------------------------------------------

void CFFmfc::Change_audio_stream(int newStreamIndex)
{
    wxCommandEvent evt(CHANGE_AUDIO);
    evt.SetInt(newStreamIndex);

    wxPostEvent(this, evt);
}


void CFFmfc::Change_subtitle_stream(int newStreamIndex)
{
    wxCommandEvent evt(CHANGE_SUBTITLE);
    evt.SetInt(newStreamIndex);

    wxPostEvent(this, evt);
}


// ------------------------------------------------------------
// Volume
// ------------------------------------------------------------

void CFFmfc::VolumeUp()
{
    if (_pimpl == nullptr)
        return;

    if (_pimpl->volume < 100)
        _pimpl->volume =
        std::min(100, _pimpl->volume + 10);

    wxCommandEvent evt(VOLUME_EVENT);
    evt.SetInt(_pimpl->volume);

    wxPostEvent(this, evt);
}


void CFFmfc::VolumeDown()
{
    if (_pimpl == nullptr)
        return;

    if (_pimpl->volume > 0)
        _pimpl->volume =
        std::max(0, _pimpl->volume - 10);

    wxCommandEvent evt(VOLUME_EVENT);
    evt.SetInt(_pimpl->volume);

    wxPostEvent(this, evt);
}


void CFFmfc::SetVolume(const int& pos)
{
    if (_pimpl == nullptr)
        return;

    _pimpl->volume =
        std::clamp(pos, 0, 100);

    wxCommandEvent evt(VOLUME_EVENT);
    evt.SetInt(_pimpl->volume);

    wxPostEvent(this, evt);
}


int CFFmfc::GetVolume()
{
    if (_pimpl == nullptr)
        return 0;

    return _pimpl->volume;
}


// ------------------------------------------------------------
// Position
// ------------------------------------------------------------

int64_t CFFmfc::GetTimePosition()
{
    if (_pimpl == nullptr ||
        cur_stream == nullptr)
    {
        return 0;
    }

    return _pimpl->GetPosition(cur_stream);
}


void CFFmfc::SetTimePosition(int64_t time)
{
    if (_pimpl == nullptr)
        return;

    _pimpl->time_position = time;

    auto* pos = new int64_t(time);

    wxCommandEvent evt(SET_POSITION);
    evt.SetClientData(pos);

    wxPostEvent(this, evt);
}


// ------------------------------------------------------------
// Seek
// ------------------------------------------------------------

void CFFmfc::Seek(int time)
{
    wxCommandEvent evt(SET_SEEKPOSITION);
    evt.SetInt(time);

    wxPostEvent(this, evt);
}


void CFFmfc::Seek_bar(int pos)
{
    wxCommandEvent evt(SEEK_BAR_EVENT);
    evt.SetInt(pos);

    wxPostEvent(this, evt);
}


// ------------------------------------------------------------
// Filename
// ------------------------------------------------------------

wxString CFFmfc::Getfilename()
{
    return filename;
}


// ------------------------------------------------------------
// Open file
// ------------------------------------------------------------

int CFFmfc::SetFile(
    CVideoControlInterface* control,
    const wxString& filename,
    const wxString& acceleratorHardware,
    const bool& isOpenGLDecoding,
    const int& volume)
{
    if (!_pimpl)
        _pimpl = std::make_unique<CFFmfcPimpl>();

    this->filename = filename;

    _pimpl->acceleratorHardware = acceleratorHardware;
    _pimpl->isOpenGLDecoding = isOpenGLDecoding;
    _pimpl->percentVolume = volume;

    CFFmfcPimpl::dlg = control;
    _pimpl->parent = this;

    Reset_index();

    _pimpl->exit_remark = 0;

    if (_pimpl->display_disable)
        _pimpl->video_disable = 1;

    _pimpl->autoexit = 1;

    const std::string path =
        CConvertUtility::ConvertToStdString(filename);

    cur_stream =
        _pimpl->stream_open(
            path.c_str(),
            _pimpl->file_iformat);

    _pimpl->g_is = cur_stream;

    /*
     * IMPORTANT :
     * stream_open() peut échouer.
     * Il faut impérativement sortir ici.
     */
    if (_pimpl->g_is == nullptr)
    {
        _pimpl->do_exit(nullptr);
        cur_stream = nullptr;

        return -1;
    }

    if (_pimpl->dlg != nullptr)
    {
        _pimpl->g_is->width =
            _pimpl->dlg->getWidth();

        _pimpl->g_is->height =
            _pimpl->dlg->getHeight();
    }

    wxCommandEvent event(EVENT_VIDEOSTART);

    if (GetParent() != nullptr)
    {
        wxPostEvent(
            GetParent()->GetEventHandler(),
            event);
    }

    return 0;
}