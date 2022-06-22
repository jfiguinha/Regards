#include <header.h>
#include "ffplaycore.h"
#include <window_id.h>
#include <fstream>
#include <ConvertUtility.h>
CFFmfc::CFFmfc(wxWindow* parent, wxWindowID id)
	: wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0)
{
	_pimpl = nullptr;
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
	if (_pimpl)
		delete _pimpl;
}


void CFFmfc::RefreshEvent(wxCommandEvent& event)
{
	if (_pimpl->exit_remark == 0)
	{
		double remaining_time = 0;
		_pimpl->video_refresh(cur_stream, &remaining_time);
		//video_refresh_timer(event.user.data1);
		//_pimpl->g_is->refresh = 0;
	}
}

void CFFmfc::SeekBarEvent(wxCommandEvent& event)
{
	if (_pimpl->exit_remark == 0)
	{
		double frac;

		if (_pimpl->seek_by_bytes || _pimpl->g_is->ic->duration <= 0)
		{
			uint64_t size = avio_size(_pimpl->g_is->ic->pb);
			_pimpl->stream_seek(cur_stream, size * _pimpl->seek_bar_pos / 1000, 0, 1);
		}
		else
		{
			int64_t ts;
			frac = static_cast<double>(_pimpl->seek_bar_pos) / 1000;
			ts = frac * _pimpl->g_is->ic->duration;
			if (_pimpl->g_is->ic->start_time != AV_NOPTS_VALUE)
				ts += _pimpl->g_is->ic->start_time;
			_pimpl->stream_seek(cur_stream, ts, 0, 0);
		}
	}
}

void CFFmfc::PositionSeekEvent(wxCommandEvent& event)
{
	if (_pimpl->exit_remark)
		return;

	double incr, pos;

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
	if (_pimpl->seek_by_bytes)
	{
		pos = -1;
		if (pos < 0 && cur_stream->video_stream >= 0)
			pos = _pimpl->frame_queue_last_pos(&cur_stream->pictq);
		if (pos < 0 && cur_stream->audio_stream >= 0)
			pos = _pimpl->frame_queue_last_pos(&cur_stream->sampq);
		if (pos < 0)
			pos = avio_tell(cur_stream->ic->pb);
		if (cur_stream->ic->bit_rate)
			incr *= cur_stream->ic->bit_rate / 8.0;
		else
			incr *= 180000.0;
		pos += incr;
		_pimpl->stream_seek(cur_stream, pos, incr, 1);
	}
	else
	{
		pos = _pimpl->get_master_clock(cur_stream);
		if (isnan(pos))
			pos = (double)cur_stream->seek_pos / AV_TIME_BASE;
		pos += incr;
		if (cur_stream->ic->start_time != AV_NOPTS_VALUE && pos < cur_stream->ic->start_time / (double)AV_TIME_BASE)
			pos = cur_stream->ic->start_time / (double)AV_TIME_BASE;
		_pimpl->stream_seek(cur_stream, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
	}
}

void CFFmfc::PositionEvent(wxCommandEvent& event)
{
	if (_pimpl->exit_remark)
	{
		auto ts = static_cast<int64_t*>(event.GetClientData());
		delete ts;
		return;
	}

	auto ts = static_cast<int64_t*>(event.GetClientData());
	if (ts != nullptr)
	{
		//if (*ts > 0)
		//{
			if (_pimpl->g_is->ic != nullptr)
			{
				if (_pimpl->g_is->ic->start_time != AV_NOPTS_VALUE)
					*ts += _pimpl->g_is->ic->start_time;
				_pimpl->stream_seek(cur_stream, *ts, 0, 0);
				_pimpl->stream_cycle_channel(cur_stream, AVMEDIA_TYPE_SUBTITLE);
			}
		//}
		delete ts;
	}
}

void CFFmfc::ChangeVolumeEvent(wxCommandEvent& event)
{
	if (_pimpl->exit_remark)
		return;

	//toggle_pause(cur_stream);
	int newaudioIndex = event.GetInt();
	_pimpl->percentVolume = newaudioIndex;
	printf("ChangeVolumeEvent Volume index : %d \n", _pimpl->percentVolume);
}


void CFFmfc::ChangeAudioEvent(wxCommandEvent& event)
{
	if (_pimpl->exit_remark)
		return;

	//toggle_pause(cur_stream);
	int newaudioIndex = event.GetInt();
	_pimpl->stream_change_stream(cur_stream, AVMEDIA_TYPE_AUDIO, newaudioIndex);
	//toggle_play(cur_stream);
}

void CFFmfc::ChangeSubtitleEvent(wxCommandEvent& event)
{
	if (_pimpl->exit_remark)
		return;

	//toggle_pause(cur_stream);
	int newSubtitleIndex = event.GetInt();
	_pimpl->stream_change_stream(cur_stream, AVMEDIA_TYPE_SUBTITLE, newSubtitleIndex);
	//toggle_play(cur_stream);
}


void CFFmfc::AspectEvent(wxCommandEvent& event)
{
	if (_pimpl->exit_remark)
	{
		auto size = static_cast<wxSize*>(event.GetClientData());
		if (size != nullptr)
			delete size;
		return;
	}

	auto size = static_cast<wxSize*>(event.GetClientData());
	if (size != nullptr)
	{
		_pimpl->screen_width = _pimpl->g_is->width = size->x;
		_pimpl->screen_height = _pimpl->g_is->height = size->y;
		_pimpl->g_is->force_refresh = 1;
		delete size;
	}
}

void CFFmfc::ExitEvent(wxCommandEvent& event)
{
	_pimpl->do_exit(cur_stream);
}

void CFFmfc::QuitEvent(wxCommandEvent& event)
{
	wxCommandEvent evt(wxEVENT_ENDVIDEOTHREAD);
	this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
}

void CFFmfc::StopEvent(wxCommandEvent& event)
{
	wxCommandEvent evt(wxEVENT_STOPVIDEO);
	this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
}

void CFFmfc::StepEvent(wxCommandEvent& event)
{
	_pimpl->step_to_next_frame(cur_stream);
}

void CFFmfc::PauseEvent(wxCommandEvent& event)
{
	_pimpl->toggle_pause(cur_stream);
}

void CFFmfc::PlayEvent(wxCommandEvent& event)
{
	_pimpl->toggle_play(cur_stream);
}

void CFFmfc::SetOutputMode(int outputMode)
{
	_pimpl->videoOutputMode = outputMode;
}

//¸´Î»
//Reset
int CFFmfc::Reset_index()
{
	_pimpl->vframe_index = 0;
	_pimpl->aframe_index = 0;
	_pimpl->packet_index = 0;
	return 0;
}


void CFFmfc::VideoDisplaySize(int width, int height)
{
	if (_pimpl->g_is != nullptr)
	{
		_pimpl->g_is->width = width;
		_pimpl->g_is->height = height;
	}
}

void CFFmfc::SetVideoParameter(int angle, int flipV, int flipH)
{
	_pimpl->video_angle = angle;
	_pimpl->video_flipV = flipV;
	_pimpl->video_flipH = flipH;
}


//·¢ËÍ¡°ÍË³ö¡±ÃüÁî
//Send Command "Quit"
bool CFFmfc::Quit()
{
	bool isExitNow = false;
	if (_pimpl->g_is)
	{
		_pimpl->StopStream();
		wxCommandEvent evt(FF_EXIT_EVENT);
		evt.SetClientData(cur_stream);
		this->GetEventHandler()->AddPendingEvent(evt);
	}
	else
	{
		_pimpl->StopStream();
		_pimpl->do_exit(nullptr);
		isExitNow = true;
	}

	return isExitNow;
}

//·¢ËÍ¡°ÖðÖ¡¡±ÃüÁî
//Send Command "Step"
void CFFmfc::Seek_step()
{
	wxCommandEvent evt(FF_STEP_EVENT);
	this->GetEventHandler()->AddPendingEvent(evt);
}

//·¢ËÍ¡°ÔÝÍ£¡±ÃüÁî
//Send Command "Pause"
void CFFmfc::Pause()
{
	/*
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	event.key.keysym.sym = SDLK_p;
	SDL_PushEvent(&event);
	*/
	wxCommandEvent evt(FF_PAUSE_EVENT);
	this->GetEventHandler()->AddPendingEvent(evt);
}

void CFFmfc::Play()
{
	/*
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	event.key.keysym.sym = SDLK_z;
	SDL_PushEvent(&event);
	*/
	wxCommandEvent evt(FF_PLAY_EVENT);
	this->GetEventHandler()->AddPendingEvent(evt);
}

//·¢ËÍ¡°¿í¸ß±È¡±ÃüÁî
//Send Command "AspectRatio"
void CFFmfc::Aspectratio(int num, int den)
{
	//int w=g_is->width;
	int h = _pimpl->g_is->height;
	int w_re = h * num / den;
	auto size = new wxSize();
	size->x = w_re;
	size->y = h;
	wxCommandEvent evt(FF_ASPECT_EVENT);
	evt.SetClientData(size);
	this->GetEventHandler()->AddPendingEvent(evt);
}

//·¢ËÍ¡°´óÐ¡¡±ÃüÁî
//Send Command "WindowSize"
void CFFmfc::Size(int percentage)
{
	int w = _pimpl->g_is->ic->streams[_pimpl->g_is->video_stream]->codecpar->width;
	int h = _pimpl->g_is->ic->streams[_pimpl->g_is->video_stream]->codecpar->height;

	auto size = new wxSize();
	size->x = w;
	size->y = h;
	wxCommandEvent evt(FF_ASPECT_EVENT);
	evt.SetClientData(size);
	this->GetEventHandler()->AddPendingEvent(evt);
}


//--------------------------------------------------------------
//
//--------------------------------------------------------------
void CFFmfc::Change_audio_stream(int newStreamIndex)
{
	wxCommandEvent evt(CHANGE_AUDIO);
	evt.SetInt(newStreamIndex);
	this->GetEventHandler()->AddPendingEvent(evt);
}

void CFFmfc::Change_subtitle_stream(int newStreamIndex)
{
	wxCommandEvent evt(CHANGE_SUBTITLE);
	evt.SetInt(newStreamIndex);
	this->GetEventHandler()->AddPendingEvent(evt);
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void CFFmfc::VolumeUp()
{
	if (_pimpl->volume < 100)
		_pimpl->volume += 10;

	wxCommandEvent evt(VOLUME_EVENT);
	evt.SetInt(_pimpl->volume);
	this->GetEventHandler()->AddPendingEvent(evt);
}

void CFFmfc::SetVolume(const int& pos)
{
	_pimpl->volume = pos;

	wxCommandEvent evt(VOLUME_EVENT);
	evt.SetInt(_pimpl->volume);
	this->GetEventHandler()->AddPendingEvent(evt);
}

int CFFmfc::GetVolume()
{
	return _pimpl->volume;
}

int64_t CFFmfc::GetTimePosition()
{
	int64_t time_position = 0;
	if (_pimpl != nullptr)
	{
		time_position = _pimpl->GetPosition(cur_stream);
	}

	return time_position;
}


void CFFmfc::SetTimePosition(int64_t time)
{
	auto pos = new int64_t();
	*pos = time;
	_pimpl->time_position = time;
	wxCommandEvent evt(SET_POSITION);
	evt.SetClientData(pos);
	this->GetEventHandler()->AddPendingEvent(evt);
}

void CFFmfc::VolumeDown()
{
	if (_pimpl->volume > 0)
		_pimpl->volume -= 10;

	wxCommandEvent evt(VOLUME_EVENT);
	evt.SetInt(_pimpl->volume);
	this->GetEventHandler()->AddPendingEvent(evt);
}

//·¢ËÍ¡°Ç°½ø/ºóÍË¡±ÃüÁî
//Send Command "Seek"
void CFFmfc::Seek(int time)
{
	wxCommandEvent evt(SET_SEEKPOSITION);
	evt.SetInt(time);
	this->GetEventHandler()->AddPendingEvent(evt);
}

//²¥·Å½ø¶È
//Seek Bar
void CFFmfc::Seek_bar(int pos)
{
	wxCommandEvent evt(SEEK_BAR_EVENT);
	evt.SetInt(pos);
	this->GetEventHandler()->AddPendingEvent(evt);
}

wxString CFFmfc::Getfilename()
{
	return this->filename;
}

/* Called from the main */
//½âÂëÖ÷º¯Êý
//Main function
//#define __MINGW32__
int CFFmfc::SetFile(CVideoControlInterface* control, string filename, const wxString& acceleratorHardware,
                    const bool& isOpenGLDecoding, const int& volume)
{
	//Save volume infos;
	/*
	int volume = 100;

	if (_pimpl != nullptr)
	{
		volume = _pimpl->volume;
		delete _pimpl;
	}
	*/

	if (_pimpl == nullptr)
		_pimpl = new CFFmfcPimpl();

	this->filename = filename;

	//_pimpl->acceleratorHardware = acceleratorHardware;
	_pimpl->isOpenGLDecoding = isOpenGLDecoding;
	_pimpl->percentVolume = volume;
	printf("SetFile Volume index : %d \n", _pimpl->percentVolume);
	CFFmfcPimpl::dlg = control;
	_pimpl->parent = this;
	Reset_index();
	//ÍË³ö·ûºÅÖÃÁã
	_pimpl->exit_remark = 0;
	//int flags;
	//ºËÐÄ½á¹¹Ìå

	//char dummy_videodriver[] = "SDL_VIDEODRIVER=dummy";
	_pimpl->input_filename = filename;


	if (_pimpl->display_disable)
	{
		_pimpl->video_disable = 1;
	}


	_pimpl->autoexit = 1;

	av_init_packet(&_pimpl->flush_pkt);
	_pimpl->flush_pkt.data = (uint8_t*)(intptr_t)"FLUSH";
	cur_stream = _pimpl->g_is = _pimpl->stream_open(CConvertUtility::ConvertToUTF8(filename), _pimpl->file_iformat);
	if (!_pimpl->g_is)
	{
		_pimpl->do_exit(nullptr);
	}

	if (_pimpl->dlg != nullptr)
	{
		_pimpl->g_is->width = _pimpl->dlg->getWidth();
		_pimpl->g_is->height = _pimpl->dlg->getHeight();
	}


	wxCommandEvent event(EVENT_VIDEOSTART);
	//event.SetId(EVENT_VIDEOSTART);
	wxPostEvent(_pimpl->parent->GetParent(), event);

	return 0;
}
