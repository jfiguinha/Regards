#include <header.h>
#include "ffplaycore.h"
#include "ffmfcpimpl.h"

CFFmfc::CFFmfc()
{
	_pimpl = new CFFmfcPimpl();
}

CFFmfc::~CFFmfc()
{
	if(_pimpl)
		delete _pimpl;
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
		SDL_Event event;
		event.type = FF_QUIT_EVENT;
		event.user.data1 = _pimpl->g_is;
		{
			SDL_PushEvent(&event);
		}
	}
	else
	{
		_pimpl->do_exit(nullptr);
		isExitNow = true;
	}
	return isExitNow;
}

//·¢ËÍ¡°È«ÆÁ¡±ÃüÁî
//Send Command "FullScreen"
void CFFmfc::Play_fullcreen() {
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	event.key.keysym.sym = SDLK_f;
	SDL_PushEvent(&event);
}


//·¢ËÍ¡°ÖðÖ¡¡±ÃüÁî
//Send Command "Step"
void CFFmfc::Seek_step() {
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	event.key.keysym.sym = SDLK_s;
	SDL_PushEvent(&event);
}

//·¢ËÍ¡°ÔÝÍ£¡±ÃüÁî
//Send Command "Pause"
void CFFmfc::Play_pause()
{
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	event.key.keysym.sym = SDLK_p;
	SDL_PushEvent(&event);
}

void CFFmfc::Play()
{
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	event.key.keysym.sym = SDLK_z;
	SDL_PushEvent(&event);
}

//·¢ËÍ¡°¿í¸ß±È¡±ÃüÁî
//Send Command "AspectRatio"
void CFFmfc::Aspectratio(int num, int den) {
	//int w=g_is->width;
	int h = _pimpl->g_is->height;
	int w_re = h * num / den;
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
}

//·¢ËÍ¡°´óÐ¡¡±ÃüÁî
//Send Command "WindowSize"
void CFFmfc::Size(int percentage){
	int w= _pimpl->g_is->ic->streams[_pimpl->g_is->video_stream]->codec->width;
	int h= _pimpl->g_is->ic->streams[_pimpl->g_is->video_stream]->codec->height;
    
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
}
//·¢ËÍ¡°´°¿Ú»­ÃæÄÚÈÝ¡±ÃüÁî
//Send Command "Audio Display Mode"
void CFFmfc::Audio_display(int mode){

	SDL_Event event;
	event.type = SDL_KEYDOWN;
	switch(mode){
	case 0:event.key.keysym.sym=SDLK_w;break;
	case 1:event.key.keysym.sym=SDLK_e;break;
	case 2:event.key.keysym.sym=SDLK_r;break;
	}
	SDL_PushEvent(&event);
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void CFFmfc::Change_audio_stream(int newStreamIndex)
{
	SDL_Event event;
	event.type = CHANGE_AUDIO;
	event.window.data1= newStreamIndex;
	SDL_PushEvent(&event);
}

void CFFmfc::Change_subtitle_stream(int newStreamIndex)
{
	SDL_Event event;
	event.type = CHANGE_SUBTITLE;
	event.window.data1= newStreamIndex;
	SDL_PushEvent(&event);
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void CFFmfc::VolumeUp()
{
	if (_pimpl->volume < 100)
		_pimpl->volume += 10;

	SDL_Event event;
	event.type = VOLUME_UP_EVENT;
	event.window.data1= _pimpl->volume;
	SDL_PushEvent(&event);
}

int CFFmfc::GetVolume()
{
	return _pimpl->volume;
}


void CFFmfc::SetTimePosition(int64_t time)
{
	SDL_Event event;
	event.type = SET_POSITION;
	_pimpl->time_position = time;
	SDL_PushEvent(&event);
}

void CFFmfc::VolumeDown()
{
	if (_pimpl->volume > 0)
		_pimpl->volume -= 10;

	SDL_Event event;
	event.type = VOLUME_DOWN_EVENT;
	event.window.data1= _pimpl->volume;
	SDL_PushEvent(&event);
}

//·¢ËÍ¡°Ç°½ø/ºóÍË¡±ÃüÁî
//Send Command "Seek"
void CFFmfc::Seek(int time){
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
}
//²¥·Å½ø¶È
//Seek Bar
void CFFmfc::Seek_bar(int pos){
	SDL_Event event;
	event.type = SEEK_BAR_EVENT;
	_pimpl->seek_bar_pos=pos;
	SDL_PushEvent(&event);
}



/* Called from the main */
//½âÂëÖ÷º¯Êý
//Main function
//#define __MINGW32__
int CFFmfc::Play(CVideoControlInterface * control, string filename)
{
	_pimpl->dlg = control;

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
	

#ifndef SDL2
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
#endif
	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
	SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

	if (av_lockmgr_register(_pimpl->lockmgr)) {
		_pimpl->do_exit(nullptr);
	}

	av_init_packet(&_pimpl->flush_pkt);
	_pimpl->flush_pkt.data = (uint8_t *)(intptr_t)"FLUSH";
	_pimpl->g_is = _pimpl->stream_open(filename.c_str(), _pimpl->file_iformat);
	if (!_pimpl->g_is) {
		_pimpl->do_exit(nullptr);
	}

	_pimpl->g_is->width = _pimpl->dlg->getWidth();
	_pimpl->g_is->height = _pimpl->dlg->getHeight();

	_pimpl->dlg->SetVideoStart();

	_pimpl->event_loop(_pimpl->g_is);
    
	_pimpl->g_is = nullptr;

	//wxCommandEvent event(EVENT_VIDEOSTOP);
    //wxPostEvent(dlg, event);

	return 0;
}

