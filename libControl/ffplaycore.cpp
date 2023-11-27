#include <header.h>
#include "ffplaycore.h"
#include <window_id.h>
#include <fstream>
#include <ConvertUtility.h>

CFFmfc::CFFmfc(wxWindow* parent, wxWindowID id)
	: wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0)
{
	_pimpl = new CFFmfcPimpl();
	//Connect(FF_EXIT_EVENT, wxCommandEventHandler(CFFmfc::ExitEvent));
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
	Connect(FF_REFRESH_EVENT, wxCommandEventHandler(CFFmfc::RefreshEvent));
}

CFFmfc::~CFFmfc()
{
	if (_pimpl)
		delete _pimpl;
}


void CFFmfc::RefreshEvent(wxCommandEvent& event)
{
	if (cur_stream != nullptr)
	{
		double remaining_time = 0;
		_pimpl->video_refresh(cur_stream, &remaining_time);
		//video_refresh_timer(event.user.data1);
		//_pimpl->g_is->refresh = 0;
	}
}


void CFFmfc::PositionEvent(wxCommandEvent& event)
{
	auto ts = static_cast<int64_t*>(event.GetClientData());
	if (cur_stream != nullptr)
	{
		if (_pimpl->g_is->ic != nullptr)
		{
			if (_pimpl->g_is->ic->start_time != AV_NOPTS_VALUE)
				*ts += _pimpl->g_is->ic->start_time;
			_pimpl->stream_seek(cur_stream, *ts, 0, 0);
			_pimpl->stream_cycle_channel(cur_stream, AVMEDIA_TYPE_SUBTITLE);
		}
	}

	delete ts;

}

void CFFmfc::ChangeVolumeEvent(wxCommandEvent& event)
{
	if (cur_stream != nullptr)
	{
		int newaudioIndex = event.GetInt();
		_pimpl->percentVolume = newaudioIndex;
		printf("ChangeVolumeEvent Volume index : %d \n", _pimpl->percentVolume);
	}
}


void CFFmfc::ChangeAudioEvent(wxCommandEvent& event)
{
	if (cur_stream != nullptr)
	{
		int newaudioIndex = event.GetInt();
		_pimpl->stream_change_stream(cur_stream, AVMEDIA_TYPE_AUDIO, newaudioIndex);
	}
	//toggle_play(cur_stream);
}

void CFFmfc::ChangeSubtitleEvent(wxCommandEvent& event)
{
	if (cur_stream != nullptr)
	{
		//toggle_pause(cur_stream);
		int newSubtitleIndex = event.GetInt();
		_pimpl->stream_change_stream(cur_stream, AVMEDIA_TYPE_SUBTITLE, newSubtitleIndex);
		//toggle_play(cur_stream);
	}
}


void CFFmfc::AspectEvent(wxCommandEvent& event)
{
	auto size = static_cast<wxSize*>(event.GetClientData());

	if (cur_stream != nullptr)
	{
		if (size != nullptr)
		{
			_pimpl->screen_width = _pimpl->g_is->width = size->x;
			_pimpl->screen_height = _pimpl->g_is->height = size->y;
			_pimpl->g_is->force_refresh = 1;

		}
	}

	if (size != nullptr)
		delete size;
}

void CFFmfc::ExitEvent(wxCommandEvent& event)
{
	if (cur_stream != nullptr)
	{
		Quit();
	}
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
	if (cur_stream != nullptr)
	{
		_pimpl->StopStream();
		_pimpl->do_exit(cur_stream);
		cur_stream = nullptr;
		isExitNow = true;
	}
	return isExitNow;
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



wxString CFFmfc::Getfilename()
{
	return this->filename;
}

/* Called from the main */
//½âÂëÖ÷º¯Êý
//Main function
//#define __MINGW32__
int CFFmfc::SetFile(CVideoControlInterface* control, const wxString& filename, const wxString& acceleratorHardware,
                    const bool& isOpenGLDecoding, const int& volume)
{

	Quit();

	this->filename = filename;

	_pimpl->acceleratorHardware = acceleratorHardware;
	_pimpl->isOpenGLDecoding = isOpenGLDecoding;
	_pimpl->percentVolume = volume;
	printf("SetFile Volume index : %d \n", _pimpl->percentVolume);
	CFFmfcPimpl::dlg = control;
	_pimpl->parent = this;
	Reset_index();

	_pimpl->autoexit = 1;

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
