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

#include "VideoControlInterface.h"

class CFFmfcPimpl;


class CBitmapToShow
{
public:
	CBitmapToShow()
	{
		data = nullptr;
	}
    uint8_t * data;
    int width;
    int height;
	float aspect_ratio;
    //SDL_Rect displayRect;
};

class CFFmfc
{
public:
	CFFmfc();
	~CFFmfc();

	//Send Command "Pause"
	void Play_pause();
	//Send Command "Step"
	void Seek_step();
	//Send Command "FullScreen"
	void Play_fullcreen();
	//Send Command "Seek"
	void Seek(int time);
	//Send Command "AspectRatio"
	void Aspectratio(int num, int den);
	//Send Command "WindowSize"
	void Size(int percentage);
	//Send Command "Audio Display Mode"
	void Audio_display(int mode);
	void Change_audio_stream(int newStreamIndex);
	void Change_subtitle_stream(int newStreamIndex);
	//Send Command "Quit"
	bool Quit();
	void VolumeUp();
	void VolumeDown();
	int GetVolume();
	void SetTimePosition(int64_t time);
	//Main function
	int Play(CVideoControlInterface * control, string filename);
	void Play();
	//Reset
	int Reset_index();
	//Seek Bar
	void Seek_bar(int pos);
	//Video display Size
	void VideoDisplaySize(int width, int height);
	void SetOutputMode(int outputMode);
	void SetVideoParameter(int angle, int flipV, int flipH);

private:
	CFFmfcPimpl * _pimpl;
};

