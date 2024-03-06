/* 
 * FFplay for MFC
 *
 * À×Ïöæè Lei Xiaohua
 * leixiaohua1020@126.com
 * ÖÐ¹ú´«Ã½´óÑ§/Êý×ÖµçÊÓ¼¼Êõ
 * Communication University of China / Digital TV Technology
 *
 * http://blog.csdn.net/leixiaohua1020
 * 
 * ±¾¹¤³Ì½«ffmpegÏîÄ¿ÖÐµÄffplay²¥·ÅÆ÷£¨ffplay.c£©ÒÆÖ²µ½ÁËVCµÄ»·¾³ÏÂ¡£
 * ²¢ÇÒÊ¹ÓÃMFC×öÁËÒ»Ì×¼òµ¥µÄ½çÃæ¡£
 * This software transplant ffplay to Microsoft VC++ environment. 
 * And use MFC to build a simple Graphical User Interface. 
 */

#pragma once
#include "VideoControlInterface.h"
#include "ffmfcpimpl.h"

class CBitmapToShow
{
public:
	CBitmapToShow(): width(0), height(0), aspect_ratio(0)
	{
		data = nullptr;
	}

	uint8_t* data;
	int width;
	int height;
	float aspect_ratio;
	//SDL_Rect displayRect;
};

class CFFmfc : public wxWindow
{
public:
	CFFmfc(wxWindow* parent, wxWindowID id);
	~CFFmfc() override;

	//Send Command "Pause"
	void Pause();
	//Send Command "Step"
	void Seek_step();
	//Send Command "Seek"
	void Seek(int time);
	//Send Command "AspectRatio"
	void Aspectratio(int num, int den);
	//Send Command "WindowSize"
	void Size(int percentage);

	void Change_audio_stream(int newStreamIndex);
	void Change_subtitle_stream(int newStreamIndex);
	//Send Command "Quit"
	bool Quit();
	void VolumeUp();
	void VolumeDown();
	int GetVolume();
	void SetVolume(const int& pos);
	void SetTimePosition(int64_t time);
	int64_t GetTimePosition();
	//Main function
	int SetFile(CVideoControlInterface* control, const wxString& filename, const wxString& acceleratorHardware,
	            const bool& isOpenGLDecoding, const int& volume);
	void Play();
	//Reset
	int Reset_index();
	//Seek Bar
	void Seek_bar(int pos);
	//Video display Size
	void VideoDisplaySize(int width, int height);
	void SetOutputMode(int outputMode);
	void SetVideoParameter(int angle, int flipV, int flipH);
	wxString Getfilename();

private:
    void ReadQuitEvent(wxCommandEvent& event);
	void StopEvent(wxCommandEvent& event);
	void ExitEvent(wxCommandEvent& event);
	void RefreshEvent(wxCommandEvent& event);
	void SeekBarEvent(wxCommandEvent& event);
	void PositionSeekEvent(wxCommandEvent& event);
	void PositionEvent(wxCommandEvent& event);
	void ChangeVolumeEvent(wxCommandEvent& event);
	void AspectEvent(wxCommandEvent& event);
	void PauseEvent(wxCommandEvent& event);
	void PlayEvent(wxCommandEvent& event);
	void StepEvent(wxCommandEvent& event);
	void QuitEvent(wxCommandEvent& event);
	void ChangeAudioEvent(wxCommandEvent& event);
	void ChangeSubtitleEvent(wxCommandEvent& event);

	wxString filename;
	CFFmfcPimpl::VideoState* cur_stream;
	CFFmfcPimpl* _pimpl;
};
