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
//#include "ffplaymfc.h"
//#include "ffplaymfcDlg.h"
//#include "afxdialogex.h"
//--------------
#include <inttypes.h>
#include <math.h>
#include <limits.h>
#include <signal.h>

extern "C"
{
#include "libavutil/avstring.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/avassert.h"
#include "libavutil/time.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libswresample/swresample.h"
#include "libavutil/display.h"
}

#include <SDL.h>
#include <SDL_thread.h>

//#include "cmdutils.h"

#include <assert.h>

//²¹³ä¶¨Òå
//#define INT64_MIN       (-9223372036854775807i64 - 1)
//#define INT64_MAX       9223372036854775807i64

//×î¶à´æ´¢µÄÖ¡ÐÅÏ¢
#define MAX_FRAME_NUM 10000
//×î¶à´æ´¢µÄPacketÐÅÏ¢
#define MAX_PACKET_NUM 10000
//URL³¤¶È
#define MAX_URL_LENGTH 500

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


//¾²Ì¬º¯ÊýÖ»ÄÜÔÚÉùÃ÷ËüµÄÎÄ¼þµ±ÖÐ¿É¼û£¬²»ÄÜ±»ÆäËûÎÄ¼þËùµ÷ÓÃ£¬Ò²¾ÍÊÇËµ¸Ã¾²Ì¬º¯ÊýÖ»
//ÄÜÔÚÆä¶¨ÒåµÄ.cpp»ò.cÖÐµ÷ÓÃ£¬ÔÚÆäËü.cpp»ò.cÎÄ¼þµÄº¯ÊýÀïÊÇ²»ÄÜ±»µ÷ÓÃµÄ¡£¡±
//ÐèÒª¶Ôstatic void toggle_pause(VideoState *is)½øÐÐ·â×°

//·¢ËÍ¡°ÔÝÍ£¡±ÃüÁî
//Send Command "Pause"
void ffmfc_play_pause();

//·¢ËÍ¡°ÖðÖ¡¡±ÃüÁî
//Send Command "Step"
void ffmfc_seek_step();

//·¢ËÍ¡°È«ÆÁ¡±ÃüÁî
//Send Command "FullScreen"
void ffmfc_play_fullcreen();

//·¢ËÍ¡°Ç°½ø/ºóÍË¡±ÃüÁî
//Send Command "Seek"
void ffmfc_seek(int time);

//·¢ËÍ¡°¿í¸ß±È¡±ÃüÁî
//Send Command "AspectRatio"
void ffmfc_aspectratio(int num,int den);

//·¢ËÍ¡°´óÐ¡¡±ÃüÁî
//Send Command "WindowSize"
void ffmfc_size(int percentage);

//·¢ËÍ¡°´°¿Ú»­ÃæÄÚÈÝ¡±ÃüÁî
//Send Command "Audio Display Mode"
void ffmfc_audio_display(int mode);

void ffmfc_change_audio_stream(int newStreamIndex);
void ffmfc_change_subtitle_stream(int newStreamIndex);
//·¢ËÍ¡°ÍË³ö¡±ÃüÁî
//Send Command "Quit"
bool ffmfc_quit();

void ffmfc_VolumeUp();
void ffmfc_VolumeDown();
int ffmfc_GetVolume();
void ffmfc_SetTimePosition(int64_t time);
//½âÂëÖ÷º¯Êý
//Main function
int ffmfc_play(CVideoControlInterface * control, string filename);
void ffmfc_play();
//¸´Î»
//Reset
int ffmfc_reset_index();

//²¥·Å½ø¶È
//Seek Bar
void ffmfc_seek_bar(int pos);

//Video display Size
void ffmfc_videoDisplaySize(int width, int height);

void ffmfc_SetOutputMode(int outputMode);

void ffmfc_SetVideoParameter(int angle, int flipV, int flipH);
