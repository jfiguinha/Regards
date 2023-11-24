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
#include "ffplay_struct.h"

class CFFmfc
{
public:
	CFFmfc();
	~CFFmfc();

	void SetTimePosition(int64_t time);
	bool Quit();
	int SetFile(const wxString& filename, ListFunction * functionList, const wxString& acceleratorHardware, const bool& isOpenGLDecoding, const int& volume);
	void PlayPause();
	void Change_subtitle_stream(int newStreamIndex);
	void Change_audio_stream(int newStreamIndex);
	void SetVolume(const int& pos);
	int GetVolume();
	void VideoDisplaySize(int width, int height);
	int64_t GetTimePosition();

	static int StartMovie(void* arg);

	wxString filename;
	std::thread* startMovie = nullptr;
	ListFunction* functionList;
	wxString acceleratorHardware;
	bool isOpenGLDecoding;
	int volume = 0;
};
