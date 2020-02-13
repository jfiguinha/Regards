#include <header.h>
#include "ffplaycore.h"


CFFmfc::CFFmfc(wxWindow* parent, wxWindowID id)
	: wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0)
{
	_pimpl = new CFFmfcPimpl();
	Connect(FF_QUIT_EVENT, wxCommandEventHandler(CFFmfc::QuitEvent));
	Connect(FF_STEP_EVENT, wxCommandEventHandler(CFFmfc::StepEvent));
	Connect(FF_PAUSE_EVENT, wxCommandEventHandler(CFFmfc::PauseEvent));
	Connect(FF_PLAY_EVENT, wxCommandEventHandler(CFFmfc::PlayEvent));
	Connect(FF_ASPECT_EVENT, wxCommandEventHandler(CFFmfc::AspectEvent));
	Connect(FF_AUDIODISPLAY_EVENT, wxCommandEventHandler(CFFmfc::AudioDisplay));
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
	if(_pimpl)
		delete _pimpl;
}


void CFFmfc::RefreshEvent(wxCommandEvent& event)
{
	if (_pimpl->exit_remark == 0)
	{
		_pimpl->video_refresh(cur_stream);
		//video_refresh_timer(event.user.data1);
		_pimpl->g_is->refresh = 0;
	}
}

void CFFmfc::SeekBarEvent(wxCommandEvent& event)
{
	if (_pimpl->exit_remark == 0)
	{
		double frac;

		if (_pimpl->seek_by_bytes || _pimpl->g_is->ic->duration <= 0) {
			uint64_t size = avio_size(_pimpl->g_is->ic->pb);
			_pimpl->stream_seek(cur_stream, size*_pimpl->seek_bar_pos / 1000, 0, 1);
		}
		else {
			int64_t ts;
			frac = (double)_pimpl->seek_bar_pos / 1000;
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

	if (_pimpl->seek_by_bytes) {
		if (_pimpl->g_is->video_stream >= 0 && _pimpl->g_is->video_current_pos >= 0) {
			pos = _pimpl->g_is->video_current_pos;
		}
		else if (_pimpl->g_is->audio_stream >= 0 && _pimpl->g_is->audio_pkt.pos >= 0) {
			pos = _pimpl->g_is->audio_pkt.pos;
		}
		else
			pos = avio_tell(_pimpl->g_is->ic->pb);
		if (_pimpl->g_is->ic->bit_rate)
			incr *= _pimpl->g_is->ic->bit_rate / 8.0;
		else
			incr *= 180000.0;
		pos += incr;
		_pimpl->stream_seek(cur_stream, pos, incr, 1);
	}
	else {
		pos = _pimpl->get_master_clock(cur_stream);
		pos += incr;
		_pimpl->stream_seek(cur_stream, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
	}
}

void CFFmfc::PositionEvent(wxCommandEvent& event)
{
	if (_pimpl->exit_remark)
	{
		int64_t * ts = (int64_t *)event.GetClientData();
		delete ts;
		return;
	}

	int64_t * ts = (int64_t *)event.GetClientData();
	if (ts != nullptr)
	{
		if (*ts > 0)
		{
			if (_pimpl->g_is->ic != nullptr)
			{
				if (_pimpl->g_is->ic->start_time != AV_NOPTS_VALUE)
					ts += _pimpl->g_is->ic->start_time;
				_pimpl->stream_seek(cur_stream, *ts, 0, 0);
				_pimpl->stream_cycle_channel(cur_stream, AVMEDIA_TYPE_SUBTITLE);
			}
		}
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

void CFFmfc::AudioDisplay(wxCommandEvent& event)
{
	if (_pimpl->exit_remark)
		return;

	int value = event.GetInt();
	switch (value)
	{
	case 0:
		_pimpl->toggle_audio_display(cur_stream, CFFmfcPimpl::ShowMode::SHOW_MODE_VIDEO);
		_pimpl->g_is->force_refresh = 1;
		break;
	case 1:
		_pimpl->toggle_audio_display(cur_stream, CFFmfcPimpl::ShowMode::SHOW_MODE_WAVES);
		_pimpl->g_is->force_refresh = 1;
		break;
	case 2:
		_pimpl->toggle_audio_display(cur_stream, CFFmfcPimpl::ShowMode::SHOW_MODE_RDFT);
		_pimpl->g_is->force_refresh = 1;
		break;
	}

}


void CFFmfc::AspectEvent(wxCommandEvent& event)
{
	if (_pimpl->exit_remark)
	{
		wxSize * size = (wxSize *)event.GetClientData();
		if (size != nullptr)
			delete size;
		return;
	}

	wxSize * size = (wxSize *)event.GetClientData();
	if (size != nullptr)
	{
		_pimpl->screen_width = _pimpl->g_is->width = size->x;
		_pimpl->screen_height = _pimpl->g_is->height = size->y;
		_pimpl->g_is->force_refresh = 1;
		delete size;
	}
}

void CFFmfc::QuitEvent(wxCommandEvent& event)
{
	_pimpl->do_exit(cur_stream);
	//cur_stream = nullptr;

	wxCommandEvent evt(wxEVENT_ENDVIDEOTHREAD);
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
int CFFmfc::Reset_index(){
	_pimpl->vframe_index=0;
	_pimpl->aframe_index=0;
	_pimpl->packet_index=0;
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
	//do_exit(g_is);
	bool isExitNow = false;
	if (_pimpl->g_is)
	{
		/*
		SDL_Event event;
		event.type = FF_QUIT_EVENT;
		event.user.data1 = _pimpl->g_is;
		{
			SDL_PushEvent(&event);
		}
		*/
		wxCommandEvent evt(FF_QUIT_EVENT);
		evt.SetClientData(cur_stream);
		this->GetEventHandler()->AddPendingEvent(evt);
	}
	else
	{
		_pimpl->do_exit(nullptr);
		isExitNow = true;
	}
	return isExitNow;
}

//·¢ËÍ¡°ÖðÖ¡¡±ÃüÁî
//Send Command "Step"
void CFFmfc::Seek_step() {
	/*
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	event.key.keysym.sym = SDLK_s;
	SDL_PushEvent(&event);
	*/
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
void CFFmfc::Aspectratio(int num, int den) {
	//int w=g_is->width;
	int h = _pimpl->g_is->height;
	int w_re = h * num / den;
	wxSize * size = new wxSize();
	size->x = w_re;
	size->y = h;
	wxCommandEvent evt(FF_ASPECT_EVENT);
	evt.SetClientData(size);
	this->GetEventHandler()->AddPendingEvent(evt);
	/*
#ifdef SDL2
	SDL_Event event;
	event.type = SDL_WINDOWEVENT;
	event.window.event = SDL_WINDOWEVENT_RESIZED;
	event.window.data1 = w_re;
	event.window.data2 = h;
#else
	SDL_Event event;
	event.type = SDL_VIDEORESIZE;
	event.resize.w = w_re;
	event.resize.h = h;
#endif
	SDL_PushEvent(&event);
	*/
}

//·¢ËÍ¡°´óÐ¡¡±ÃüÁî
//Send Command "WindowSize"
void CFFmfc::Size(int percentage){
	int w= _pimpl->g_is->ic->streams[_pimpl->g_is->video_stream]->codec->width;
	int h= _pimpl->g_is->ic->streams[_pimpl->g_is->video_stream]->codec->height;
    
	wxSize * size = new wxSize();
	size->x = w;
	size->y = h;
	wxCommandEvent evt(FF_ASPECT_EVENT);
	evt.SetClientData(size);
	this->GetEventHandler()->AddPendingEvent(evt);

	/*
#ifdef SDL2
	SDL_Event event;
	event.type = SDL_WINDOWEVENT;
	event.window.event = SDL_WINDOWEVENT_RESIZED;
	event.window.data1=w*percentage/100;
	event.window.data2=h*percentage/100;
#else
	SDL_Event event;
	event.type = SDL_VIDEORESIZE;
	event.resize.w=w*percentage/100;
	event.resize.h=h*percentage/100;
#endif

	SDL_PushEvent(&event);
	*/
}
//·¢ËÍ¡°´°¿Ú»­ÃæÄÚÈÝ¡±ÃüÁî
//Send Command "Audio Display Mode"
void CFFmfc::Audio_display(int mode){

	wxCommandEvent evt(FF_AUDIODISPLAY_EVENT);
	evt.SetInt(mode);
	this->GetEventHandler()->AddPendingEvent(evt);
	/*
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	switch(mode){
	case 0:event.key.keysym.sym=SDLK_w;break;
	case 1:event.key.keysym.sym=SDLK_e;break;
	case 2:event.key.keysym.sym=SDLK_r;break;
	}
	SDL_PushEvent(&event);
	*/
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void CFFmfc::Change_audio_stream(int newStreamIndex)
{
	/*
	SDL_Event event;
	event.type = CHANGE_AUDIO;
	event.window.data1= newStreamIndex;
	SDL_PushEvent(&event);
	*/
	wxCommandEvent evt(CHANGE_AUDIO);
	evt.SetInt(newStreamIndex);
	this->GetEventHandler()->AddPendingEvent(evt);
}

void CFFmfc::Change_subtitle_stream(int newStreamIndex)
{
	/*
	SDL_Event event;
	event.type = CHANGE_SUBTITLE;
	event.window.data1= newStreamIndex;
	SDL_PushEvent(&event);
	*/
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

	/*
	SDL_Event event;
	event.type = VOLUME_EVENT;
	event.window.data1= _pimpl->volume;
	SDL_PushEvent(&event);
	*/
}

int CFFmfc::GetVolume()
{
	return _pimpl->volume;
}


void CFFmfc::SetTimePosition(int64_t time)
{
	/*
	SDL_Event event;
	event.type = SET_POSITION;
	_pimpl->time_position = time;
	SDL_PushEvent(&event);
	*/

	int64_t * pos = new int64_t();
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

	/*
	SDL_Event event;
	event.type = VOLUME_DOWN_EVENT;
	event.window.data1= _pimpl->volume;
	SDL_PushEvent(&event);
	*/
	wxCommandEvent evt(VOLUME_EVENT);
	evt.SetInt(_pimpl->volume);
	this->GetEventHandler()->AddPendingEvent(evt);
}

//·¢ËÍ¡°Ç°½ø/ºóÍË¡±ÃüÁî
//Send Command "Seek"
void CFFmfc::Seek(int time){
	wxCommandEvent evt(SET_SEEKPOSITION);
	evt.SetInt(time);
	this->GetEventHandler()->AddPendingEvent(evt);
	/*
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	switch (time){
	case -10 :event.key.keysym.sym=SDLK_LEFT;break;
	case 10 :event.key.keysym.sym=SDLK_RIGHT;break;
	case -60 :event.key.keysym.sym=SDLK_DOWN;break;
	case 60 :event.key.keysym.sym=SDLK_UP;break;
	case -600 :event.key.keysym.sym=SDLK_PAGEDOWN;break;
	case 600 :event.key.keysym.sym=SDLK_PAGEUP;break;
	default :event.key.keysym.sym=SDLK_RIGHT;break;
	}
	SDL_PushEvent(&event);
	*/
}
//²¥·Å½ø¶È
//Seek Bar
void CFFmfc::Seek_bar(int pos){
	wxCommandEvent evt(SEEK_BAR_EVENT);
	evt.SetInt(pos);
	this->GetEventHandler()->AddPendingEvent(evt);
	/*
	SDL_Event event;
	event.type = SEEK_BAR_EVENT;
	_pimpl->seek_bar_pos=pos;
	SDL_PushEvent(&event);
	*/
}



/* Called from the main */
//½âÂëÖ÷º¯Êý
//Main function
//#define __MINGW32__
int CFFmfc::Play(CVideoControlInterface * control, string filename)
{
	_pimpl->dlg = control;
	_pimpl->parent = this;
	Reset_index();
	//ÍË³ö·ûºÅÖÃÁã
	_pimpl->exit_remark=0;
	//int flags;
	//ºËÐÄ½á¹¹Ìå

	//char dummy_videodriver[] = "SDL_VIDEODRIVER=dummy";
	_pimpl->input_filename = filename;

	
	if (_pimpl->display_disable)
	{
		_pimpl->video_disable = 1;
	}
	

	_pimpl->autoexit=1;
	
	/*
#ifndef SDL2
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
#endif
	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
	SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

	if (av_lockmgr_register(_pimpl->lockmgr)) {
		_pimpl->do_exit(nullptr);
	}
	*/

	av_init_packet(&_pimpl->flush_pkt);
	_pimpl->flush_pkt.data = (uint8_t *)(intptr_t)"FLUSH";
	cur_stream = _pimpl->g_is = _pimpl->stream_open(filename.c_str(), _pimpl->file_iformat);
	if (!_pimpl->g_is) {
		_pimpl->do_exit(nullptr);
	}

	_pimpl->g_is->width = _pimpl->dlg->getWidth();
	_pimpl->g_is->height = _pimpl->dlg->getHeight();

    wxCommandEvent event(EVENT_VIDEOSTART);
    wxPostEvent(_pimpl->parent, event);

	//_pimpl->event_loop(_pimpl->g_is);
    
	//_pimpl->g_is = nullptr;

	//wxCommandEvent event(EVENT_VIDEOSTOP);
    //wxPostEvent(dlg, event);

	return 0;
}

