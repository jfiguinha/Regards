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
#include "ffplaycore.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_thread.h>
#include <SDL_mutex.h>


const char program_name[] = "ffplaymfc";
const int program_birth_year = 2013;

//SOUND Volume
int volume = SDL_MIX_MAXVOLUME;
int percent = 100;

#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
#define MIN_FRAMES 5

/* SDL audio buffer size, in samples. Should be small to have precise
A/V sync as SDL does not have hardware buffer fullness info. */
#define SDL_AUDIO_BUFFER_SIZE 1024

/* no AV sync correction is done if below the AV sync threshold */
#define AV_SYNC_THRESHOLD 0.01
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

/* maximum audio speed change to get correct sync */
#define SAMPLE_CORRECTION_PERCENT_MAX 10

/* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
#define AUDIO_DIFF_AVG_NB   20

/* NOTE: the size must be big enough to compensate the hardware audio buffersize size */
/* TODO: We assume that a decoded and resampled frame fits into this buffer */
#define SAMPLE_ARRAY_SIZE (8 * 65536)

#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

static int sws_flags = SWS_BICUBIC;

//¶ÁÈ¡ÊäÈëÎÄ¼þÐ­ÒéµÄÊ±ºòÊ¹ÓÃ£»À´×ÔffmpegÔ´Âë
typedef struct URLContext {
	const AVClass *av_class; ///< information for av_log(). Set by url_open().
	struct URLProtocol *prot;
	int flags;
	int is_streamed;  /**< true if streamed (no seek possible), default = false */
	int max_packet_size;  /**< if non zero, the stream is packetized with this max packet size */
	void *priv_data;
	char *filename; /**< specified URL */
	int is_connected;
	AVIOInterruptCB interrupt_callback;
} URLContext;
typedef struct URLProtocol {
	const char *name;
	int (*url_open)(URLContext *h, const char *url, int flags);
	int (*url_read)(URLContext *h, unsigned char *buf, int size);
	int (*url_write)(URLContext *h, const unsigned char *buf, int size);
	int64_t (*url_seek)(URLContext *h, int64_t pos, int whence);
	int (*url_close)(URLContext *h);
	struct URLProtocol *next;
	int (*url_read_pause)(URLContext *h, int pause);
	int64_t (*url_read_seek)(URLContext *h, int stream_index,
		int64_t timestamp, int flags);
	int (*url_get_file_handle)(URLContext *h);
	int priv_data_size;
	const AVClass *priv_data_class;
	int flags;
	int (*url_check)(URLContext *h, int mask);
} URLProtocol;
//---------------------


typedef struct PacketQueue {
	AVPacketList *first_pkt, *last_pkt;
	int nb_packets;
	int size;
	int abort_request;
	SDL_mutex *mutex;
	SDL_cond *cond;
} PacketQueue;

#define VIDEO_PICTURE_QUEUE_SIZE 4
#define SUBPICTURE_QUEUE_SIZE 4


std::mutex videoStateMutex;

typedef struct VideoPicture {
	double pts;                                  ///< presentation time stamp for this picture
	int64_t pos;                                 ///< byte position in file
	int skip;
	SDL_Surface *bmp;
	int width, height; /* source height & width */
	AVRational sample_aspect_ratio;
	int allocated;
	int reallocate;

#if CONFIG_AVFILTER
	AVFilterBufferRef *picref;
#endif
} VideoPicture;

typedef struct SubPicture {
	double pts; /* presentation time stamp for this picture */
	AVSubtitle sub;
} SubPicture;

typedef struct AudioParams {
	int freq;
	int channels;
	int channel_layout;
	enum AVSampleFormat fmt;
} AudioParams;

enum {
	AV_SYNC_AUDIO_MASTER, /* default choice */
	AV_SYNC_VIDEO_MASTER,
	AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};
//ÊÓÆµÏÔÊ¾·½Ê½
enum V_Show_Mode {
	SHOW_MODE_YUV = 0, SHOW_MODE_Y, SHOW_MODE_U, SHOW_MODE_V, SHOW_MODE_RGB24
};

typedef struct VideoState {
	SDL_Thread *read_tid;
	SDL_Thread *video_tid;
	SDL_Thread *refresh_tid;
	AVInputFormat *iformat;
	int no_background;
	int abort_request;
	int force_refresh;
	int paused;
	int last_paused;
	int que_attachments_req;
	int seek_req;
	int seek_flags;
	int64_t seek_pos;
	int64_t seek_rel;
	int read_pause_return;
	AVFormatContext *ic;

	int audio_stream;

	int av_sync_type;
	double external_clock; /* external clock base */
	int64_t external_clock_time;

	double audio_clock;
	double audio_diff_cum; /* used for AV difference average computation */
	double audio_diff_avg_coef;
	double audio_diff_threshold;
	int audio_diff_avg_count;
	AVStream *audio_st;
	PacketQueue audioq;
	int audio_hw_buf_size;
	DECLARE_ALIGNED(16,uint8_t,audio_buf2)[AVCODEC_MAX_AUDIO_FRAME_SIZE * 4];
	uint8_t silence_buf[SDL_AUDIO_BUFFER_SIZE];
	uint8_t *audio_buf;
	uint8_t *audio_buf1;
	unsigned int audio_buf_size; /* in bytes */
	int audio_buf_index; /* in bytes */
	int audio_write_buf_size;
	AVPacket audio_pkt_temp;
	AVPacket audio_pkt;
	struct AudioParams audio_src;
	struct AudioParams audio_tgt;
	struct SwrContext *swr_ctx;
	double audio_current_pts;
	double audio_current_pts_drift;
	int frame_drops_early;
	int frame_drops_late;
	AVFrame *frame;

	enum ShowMode {
		SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
	} show_mode;
	int16_t sample_array[SAMPLE_ARRAY_SIZE];
	int sample_array_index;
	int last_i_start;
	RDFTContext *rdft;
	int rdft_bits;
	FFTSample *rdft_data;
	int xpos;

	SDL_Thread *subtitle_tid;
	int subtitle_stream;
	int subtitle_stream_changed;
	AVStream *subtitle_st;
	PacketQueue subtitleq;
	SubPicture subpq[SUBPICTURE_QUEUE_SIZE];
	int subpq_size, subpq_rindex, subpq_windex;
	SDL_mutex *subpq_mutex;
	SDL_cond *subpq_cond;

	double frame_timer;
	double frame_last_pts;
	double frame_last_duration;
	double frame_last_dropped_pts;
	double frame_last_returned_time;
	double frame_last_filter_delay;
	int64_t frame_last_dropped_pos;
	double video_clock;                          ///< pts of last decoded frame / predicted pts of next decoded frame
	int video_stream;
	AVStream *video_st;
	PacketQueue videoq;
	double video_current_pts;                    ///< current displayed pts (different from video_clock if frame fifos are used)
	double video_current_pts_drift;              ///< video_current_pts - time (av_gettime) at which we updated video_current_pts - used to have running video pts
	int64_t video_current_pos;                   ///< current displayed file pos
	VideoPicture pictq[VIDEO_PICTURE_QUEUE_SIZE];
	int pictq_size, pictq_rindex, pictq_windex;
	SDL_mutex *pictq_mutex;
	SDL_cond *pictq_cond;
#if !CONFIG_AVFILTER
	struct SwsContext *img_convert_ctx;
#endif

	char filename[1024];
	int width, height, xleft, ytop;
	int step;

#if CONFIG_AVFILTER
	AVFilterContext *in_video_filter;           ///< the first filter in the video chain
	AVFilterContext *out_video_filter;          ///< the last filter in the video chain
	int use_dr1;
	FrameBuffer *buffer_pool;
#endif

	int refresh;
	int last_video_stream, last_audio_stream, last_subtitle_stream;

	SDL_cond *continue_read_thread;
	//ÊÓÆµÏÔÊ¾·½Ê½------------
	enum V_Show_Mode v_show_mode;
} VideoState;

VideoState *g_is = nullptr;

enum ShowMode {
	SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
} ;
//Ö¸ÏòMFC´°¿ÚµÄÖ¸Õë
//CffplaymfcDlg * dlg;
CVideoControl * dlg;

/* options specified by the user */
static AVInputFormat *file_iformat;
static string input_filename;

static const char *window_title;
static int fs_screen_width;
static int fs_screen_height;
static int screen_width  = 0;
static int screen_height = 0;
static int audio_disable ;
static int video_disable ;
static int wanted_stream[AVMEDIA_TYPE_NB] = {-1,-1,0,-1,0};
//   [AVMEDIA_TYPE_AUDIO]    = -1,
//   [AVMEDIA_TYPE_VIDEO]    = -1,
//   [AVMEDIA_TYPE_SUBTITLE] = -1,};
static int seek_by_bytes = -1;
static int display_disable;
static int show_status = 1;
static int av_sync_type = AV_SYNC_AUDIO_MASTER;
static int64_t start_time = AV_NOPTS_VALUE;
static int64_t duration = AV_NOPTS_VALUE;
static int workaround_bugs = 1;
static int fast = 0;
static int genpts = 0;
static int lowres = 0;
static int idct = FF_IDCT_AUTO;
static enum AVDiscard skip_frame       = AVDISCARD_DEFAULT;
static enum AVDiscard skip_idct        = AVDISCARD_DEFAULT;
static enum AVDiscard skip_loop_filter = AVDISCARD_DEFAULT;
static int error_concealment = 3;
static int decoder_reorder_pts = -1;
static int autoexit;
static int exit_on_keydown;
static int exit_on_mousedown;
static int loop = 1;
static int framedrop = -1;
static int infinite_buffer = -1;
static enum ShowMode show_mode = SHOW_MODE_NONE;
static const char *audio_codec_name;
static const char *subtitle_codec_name;
static const char *video_codec_name;
static int rdftspeed = 20;
#if CONFIG_AVFILTER
static char *vfilters = nullptr;
#endif

/* current context */
static int is_full_screen;
static int64_t audio_callback_time;

static AVPacket flush_pkt;


#define FF_ALLOC_EVENT   (SDL_USEREVENT)
#define FF_REFRESH_EVENT (SDL_USEREVENT + 1)
#define FF_QUIT_EVENT    (SDL_USEREVENT + 2)

//×Ô¶¨ÒåÒ»¸öÊÂ¼þ£¬ÓÃÓÚµ÷Õû²¥·Å½ø¶È
#define FFMFC_SEEK_BAR_EVENT    (SDL_USEREVENT + 4)
#define FFMFC_VOLUME_UP_EVENT    (SDL_USEREVENT + 5)
#define FFMFC_VOLUME_DOWN_EVENT    (SDL_USEREVENT + 6)
#define FFMFC_SET_POSITION (SDL_USEREVENT + 7)
int seek_bar_pos;
int64_t time_position = 0;
//×¨ÃÅÉèÖÃµÄ±ê¼Ç£¬ÔÚ³ÌÐò½«ÒªÍË³öµÄÊ±ºò»áÖÃ1
static int exit_remark=0;

//---------------------------------
//static SDL_Surface *screen;

//ÊÓÆµÖ¡Ë÷Òý
int vframe_index=0;
//ÒôÆµÖ¡Ë÷Òý
int aframe_index=0;
//PacketË÷Òý
int packet_index=0;

int videoOutputMode = 24;

static void video_image_display(VideoState *is);

void ffmfc_SetOutputMode(int outputMode)
{
	videoOutputMode = outputMode;
}

//Calcul du pourcentage

int percentageToDb(int p, int maxValue) {
	if (p == 0)
		return 0;
	float value = (float)p / 100.0f;
	return maxValue * (1 + log(value));
}

//¸´Î»
//Reset
int ffmfc_reset_index(){
	vframe_index=0;
	aframe_index=0;
	packet_index=0;
	return 0;
}
//ÉèÖÃMFC²ÎÊý£¬³É¹¦µÄ»°·µ»Ø0£¬·ñÔò·µ»Ø-1
//È«¾ÖµÄ£¬Ö»ÉèÖÃÒ»´Î
int ffmfc_param_global(VideoState *is){
	//³õÊ¼»¯
	string input_protocol,input_format,wxh,decoder_name,
		decoder_type,bitrate,extention,pix_fmt,framerate,timelong,decoder_name_au,sample_rate_au,channels_au;
	float framerate_temp,timelong_temp,bitrate_temp;
	//×¢Òâ£º°ÑintµÈÀàÐÍ×ª»»³ÉLPCTSTR
	//CString¿ÉÒÔÖ±½Ó¸³Öµ¸øLPCTSTR
	AVFormatContext *pFormatCtx = is->ic;
	int video_stream=is->video_stream;
	int audio_stream=is->audio_stream;
	AVCodecContext *pCodecCtx = pFormatCtx->streams[video_stream]->codec;
	AVCodecContext *pCodecCtx_au = pFormatCtx->streams[audio_stream]->codec;

	//USES_CONVERSION;

	if(pFormatCtx->pb!=nullptr){
		URLContext *uc=(URLContext *)pFormatCtx->pb->opaque;
		URLProtocol *up=(URLProtocol *)uc->prot;
		//ÊäÈëÎÄ¼þµÄÐ­Òé----------

		input_protocol = up->name;
		dlg->SetFormatProtocol(input_protocol);
	}


	//ÊÓÆµ½âÂë²ÎÊý£¬ÓÐÊÓÆµµÄÊ±ºòÉèÖÃ
	if(video_stream!=-1){
		dlg->SetCodecResolution(to_string(pCodecCtx->width) + " x " + to_string(pCodecCtx->height));
		decoder_name = pCodecCtx->codec->long_name;
		dlg->SetDecoderName(decoder_name);
		//Ö¡ÂÊÏÔÊ¾»¹ÓÐÎÊÌâ
		framerate_temp=(pFormatCtx->streams[video_stream]->r_frame_rate.num)/(pFormatCtx->streams[video_stream]->r_frame_rate.den);
		char message[512];
		sprintf(message, "%5.2ffps", framerate_temp);
		dlg->SetFrameRate(message);

		switch(pCodecCtx->pix_fmt){
		case 0:
			pix_fmt = "YUV420P";break;
		case 1:
			pix_fmt="YUYV422";break;
		case 2:
			pix_fmt="RGB24";break;
		case 3:
			pix_fmt="BGR24";break;
		case 12:
			pix_fmt="PIX_FMT_YUVJ420P";break;	
		default:
			pix_fmt="UNKNOWN";
		}
		dlg->SetPixelFormat(pix_fmt);
	}
	//ÒôÆµ½âÂë²ÎÊý£¬ÓÐÒôÆµµÄÊ±ºòÉèÖÃ
	if(audio_stream!=-1){
		decoder_name_au = pCodecCtx_au->codec->long_name;
		//dlg->m_codecaname.SetWindowText(decoder_name_au);
		sample_rate_au = to_string(pCodecCtx_au->sample_rate);
		//dlg->m_codecasamplerate.SetWindowText(sample_rate_au);
		channels_au = to_string(pCodecCtx_au->channels);
		//dlg->m_codecachannels.SetWindowText(channels_au);
	}
	//ÏÔÊ¾³ÉÒÔkÎªµ¥Î»
	bitrate_temp=((float)(pFormatCtx->bit_rate))/1000;
	//bitrate.Format(_T("%5.2fkbps"),bitrate_temp);
	//dlg->m_formatbitrate.SetWindowText(bitrate);
	//durationÊÇÒÔÎ¢ÃëÎªµ¥Î»
	timelong_temp=(pFormatCtx->duration)/1000000;
	dlg->SetVideoDuration(pFormatCtx->duration);
	//×ª»»³Éhh:mm:ssÐÎÊ½
	int tns, thh, tmm, tss;
	tns  = (pFormatCtx->duration)/1000000;
	thh  = tns / 3600;
	tmm  = (tns % 3600) / 60;
	tss  = (tns % 60);
	//timelong.Format(_T("%02d:%02d:%02d"),thh,tmm,tss);
	//dlg->m_formatduration.SetWindowText(timelong);
	//dlg->m_duration.SetWindowText(timelong);
	//ÊäÈëÎÄ¼þµÄ·â×°¸ñÊ½------
	
	input_format = pFormatCtx->iformat->long_name;

	dlg->SetInputFormat(input_format);
	//------------------------


	//bitrate.Format("%d",pCodecCtx->bit_rate);
	//dlg->m_bitrate.SetWindowText(bitrate);

	//MetaData------------------------------------------------------------
	//´ÓAVDictionary»ñµÃ
	//ÐèÒªÓÃµ½AVDictionaryEntry¶ÔÏó
	//CString author,copyright,description;
	string key,value;
	vector<string> meta;
	AVDictionaryEntry *m = nullptr;
	//²»ÓÃÒ»¸öÒ»¸öÕÒ³öÀ´
	/*	m=av_dict_get(pFormatCtx->metadata,"author",m,0);
	author.Format("×÷Õß£º%s",m->value);
	m=av_dict_get(pFormatCtx->metadata,"copyright",m,0);
	copyright.Format("°æÈ¨£º%s",m->value);
	m=av_dict_get(pFormatCtx->metadata,"description",m,0);
	description.Format("ÃèÊö£º%s",m->value);
	*/
	//Ê¹ÓÃÑ­»·¶Á³ö
	//(ÐèÒª¶ÁÈ¡µÄÊý¾Ý£¬×Ö¶ÎÃû³Æ£¬Ç°Ò»Ìõ×Ö¶Î£¨Ñ­»·Ê±Ê¹ÓÃ£©£¬²ÎÊý)
	while(m=av_dict_get(pFormatCtx->metadata,"",m,AV_DICT_IGNORE_SUFFIX)){
		key = m->key;
		value = m->value;
		meta.push_back(key + ":" + value);
	}

	//EditControl»»ÐÐÓÃ\n²»ÐÐ£¬ÐèÒªÊ¹ÓÃ\r\n
	//³ýÁËÒªÓÃ\r\nÍâ£¬»¹Òª¶¼CEdit µÄÊôÐÔ½øÐÐÉèÖÃ£º
	//Auto HScroll ÉèÖÃÎª False
	//MultiLine  ÉèÖÃÎª True

	//dlg->m_metadata.SetWindowText(author+"\r\n"+copyright+"\r\n"+description);
	dlg->SetMetadata(meta);
	//--------------------------------------------------------------------
	return 0;
}
//Ã¿Ò»Ö¡ÉèÖÃÒ»´Î
//ÓÐÒ»Ð©È«¾Ö±äÁ¿

int ffmfc_param_packet(VideoState *is,AVPacket *packet){
	//--------------------------------------------------------------------
	//AVFormatContext *pFormatCtx = is->ic;
	//int video_stream=is->video_stream;
	//int audio_stream=is->audio_stream;
	//AVCodecContext *pCodecCtx = pFormatCtx->streams[video_stream]->codec;
	int packet_size;
	//±ÜÃâÊý¾ÝÌ«¶à£¬³¬¹ýÒ»¶¨Á¿Ö®ºó£¬¾Í»áÇåÁã--------------------------
	if(packet_index>=MAX_PACKET_NUM){
		dlg->SystemClear();
	}

	//ÂëÁ÷µ¥ÔªµÄ´óÐ¡£¬ÀýÈçH.264ÖÐµÄNAL----------------------------------------------
	packet_size=packet->size;

	//×ÔÔö-----------------------------------------------
	packet_index++;
	return 0;
}


//ÊÓÆµÖ¡²ÎÊýÌáÈ¡
int ffmfc_param_vframe(VideoState *is,AVFrame *pFrame,AVPacket *packet){
	//--------------------------------------------------------------------
	string key_frame,pict_type,reference,f_index,pts,dts,codednum;
	//AVFormatContext *pFormatCtx = is->ic;
	//int video_stream=is->video_stream;
	//AVCodecContext *pCodecCtx = pFormatCtx->streams[video_stream]->codec;
	
	/*
	//USES_CONVERSION;
	//±ÜÃâÊý¾ÝÌ«¶à£¬³¬¹ýÒ»¶¨Á¿Ö®ºó£¬¾Í»áÇåÁã--------------------------

	if(vframe_index>=MAX_FRAME_NUM){
		dlg->SystemClear();
	}

	//------------------------------
	f_index = to_string(vframe_index);
	//»ñÈ¡µ±Ç°¼ÇÂ¼ÌõÊý
	int nIndex=dlg->vddlg->m_videodecodelist.GetItemCount();
	//¡°ÐÐ¡±Êý¾Ý½á¹¹
	LV_ITEM lvitem;
	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=nIndex;
	lvitem.iSubItem=0;
	//×¢£ºvframe_index²»¿ÉÒÔÖ±½Ó¸³Öµ£¡
	//Îñ±ØÊ¹ÓÃf_indexÖ´ÐÐFormat!ÔÙ¸³Öµ£¡
	lvitem.pszText=f_index.GetBuffer();
	//------------------------


	switch(pFrame->key_frame){
	case 0:
		key_frame.Format(_T("No"));break;
	case 1:
		key_frame.Format(_T("Yes"));break;
	default:
		key_frame.Format(_T("Unknown"));
	}

	switch(pFrame->pict_type){
	case 0:
		pict_type.Format(_T("Unknown"));break;
	case 1:
		pict_type.Format(_T("I"));break;
	case 2:
		pict_type.Format(_T("P"));break;
	case 3:
		pict_type.Format(_T("B"));break;
	case 4:
		pict_type.Format(_T("S"));break;
	case 5:
		pict_type.Format(_T("SI"));break;
	case 6:
		pict_type.Format(_T("SP"));break;
	case 7:
		pict_type.Format(_T("BI"));break;
	default:
		pict_type.Format(_T("Unknown"));
	}

	reference.Format(_T("%d"),pFrame->reference);
	pts.Format(_T("%d"),pFrame->pkt_pts);
	dts.Format(_T("%d"),pFrame->pkt_dts);
	codednum.Format(_T("%d"),pFrame->coded_picture_number);

	//²åÈë±í¸ñ------------------------
	dlg->vddlg->m_videodecodelist.InsertItem(&lvitem);
	dlg->vddlg->m_videodecodelist.SetItemText(nIndex,1,pict_type);
	dlg->vddlg->m_videodecodelist.SetItemText(nIndex,2,key_frame);
	dlg->vddlg->m_videodecodelist.SetItemText(nIndex,3,codednum);
	dlg->vddlg->m_videodecodelist.SetItemText(nIndex,4,pts);
	dlg->vddlg->m_videodecodelist.SendMessage(WM_VSCROLL, SB_BOTTOM, nullptr);
	vframe_index++;
	*/
	return 0;
}

//ÒôÆµÖ¡²ÎÊýÌáÈ¡
int ffmfc_param_aframe(VideoState *is,AVFrame *pFrame,AVPacket *packet){
	//--------------------------------------------------------------------
	//AVFormatContext *pFormatCtx = is->ic;
	//int audio_stream=is->audio_stream;
	//AVCodecContext *pCodecCtx = pFormatCtx->streams[audio_stream]->codec;
	//±ÜÃâÊý¾ÝÌ«¶à£¬³¬¹ýÒ»¶¨Á¿Ö®ºó£¬¾Í»áÇåÁã--------------------------

	if(aframe_index>=MAX_FRAME_NUM){
		dlg->SystemClear();
	}
	//------------------------------
	string f_index,packet_size,pts;
	//---------------
	f_index = to_string(aframe_index);
	//»ñÈ¡µ±Ç°¼ÇÂ¼ÌõÊý
	/*
	int nIndex=dlg->addlg->m_audiodecodelist.GetItemCount();
	//¡°ÐÐ¡±Êý¾Ý½á¹¹
	LV_ITEM lvitem;
	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=nIndex;
	lvitem.iSubItem=0;
	//×¢£ºframe_index²»¿ÉÒÔÖ±½Ó¸³Öµ£¡
	//Îñ±ØÊ¹ÓÃf_indexÖ´ÐÐFormat!ÔÙ¸³Öµ£¡
	lvitem.pszText=f_index.GetBuffer();
	//------------------------
	packet_size.Format(_T("%d"),packet->size);
	pts.Format(_T("%d"),packet->pts);
	//---------------
	dlg->addlg->m_audiodecodelist.InsertItem(&lvitem);
	dlg->addlg->m_audiodecodelist.SetItemText(nIndex,1,packet_size);
	dlg->addlg->m_audiodecodelist.SetItemText(nIndex,2,pts);
	dlg->addlg->m_audiodecodelist.SendMessage(WM_VSCROLL, SB_BOTTOM, nullptr);
	aframe_index++;
	*/
	return 0;
}

//--------------------------------------------------
static int packet_queue_put(PacketQueue *q, AVPacket *pkt);
//Íù¶ÓÁÐÀïÌí¼ÓPacket(Á½²ã)?
static int packet_queue_put_private(PacketQueue *q, AVPacket *pkt)
{
	AVPacketList *pkt1;

	if (q->abort_request)
		return -1;
	//PacketList½«Packet×é³ÉÒ»¸öÁ´±í
	//Ã¿¸öPacketListÖ»ÓÐÒ»¸öPacket
	pkt1 = (AVPacketList*)av_malloc(sizeof(AVPacketList));
	if (!pkt1)
		return -1;
	pkt1->pkt = *pkt;
	pkt1->next = nullptr;

	if (!q->last_pkt)
		//µÚÒ»¸ö
		q->first_pkt = pkt1;
	else
		//ÆäËûÇé¿ö
		q->last_pkt->next = pkt1;
	q->last_pkt = pkt1;
	q->nb_packets++;
	q->size += pkt1->pkt.size + sizeof(*pkt1);
	/* XXX: should duplicate packet data in DV case */
	SDL_CondSignal(q->cond);
	return 0;
}
//Íù¶ÓÁÐÀïÌí¼ÓPacket
static int packet_queue_put(PacketQueue *q, AVPacket *pkt)
{
	int ret;

	/* duplicate the packet */
	if (pkt != &flush_pkt && av_dup_packet(pkt) < 0)
		return -1;

	SDL_LockMutex(q->mutex);
	//Íù¶ÓÁÐÀïÌí¼ÓPacket(Á½²ã)?
	ret = packet_queue_put_private(q, pkt);
	SDL_UnlockMutex(q->mutex);

	if (pkt != &flush_pkt && ret < 0)
		av_free_packet(pkt);

	return ret;
}

/* packet queue handling */
static void packet_queue_init(PacketQueue *q)
{
	memset(q, 0, sizeof(PacketQueue));
	q->mutex = SDL_CreateMutex();
	q->cond = SDL_CreateCond();
	q->abort_request = 1;
}

static void packet_queue_flush(PacketQueue *q)
{
	AVPacketList *pkt, *pkt1;

	SDL_LockMutex(q->mutex);
	for (pkt = q->first_pkt; pkt != nullptr; pkt = pkt1) {
		pkt1 = pkt->next;
		av_free_packet(&pkt->pkt);
		av_freep(&pkt);
	}
	q->last_pkt = nullptr;
	q->first_pkt = nullptr;
	q->nb_packets = 0;
	q->size = 0;
	SDL_UnlockMutex(q->mutex);
}

static void packet_queue_destroy(PacketQueue *q)
{
	packet_queue_flush(q);
	SDL_DestroyMutex(q->mutex);
	SDL_DestroyCond(q->cond);
}

static void packet_queue_abort(PacketQueue *q)
{
	SDL_LockMutex(q->mutex);

	q->abort_request = 1;

	SDL_CondSignal(q->cond);

	SDL_UnlockMutex(q->mutex);
}

static void packet_queue_start(PacketQueue *q)
{
	SDL_LockMutex(q->mutex);
	q->abort_request = 0;
	packet_queue_put_private(q, &flush_pkt);
	SDL_UnlockMutex(q->mutex);
}

/* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
static int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block)
{
	AVPacketList *pkt1;
	int ret;

	SDL_LockMutex(q->mutex);

	for (;;) {
		if (q->abort_request) {
			ret = -1;
			break;
		}

		pkt1 = q->first_pkt;
		if (pkt1) {
			q->first_pkt = pkt1->next;
			if (!q->first_pkt)
				q->last_pkt = nullptr;
			q->nb_packets--;
			q->size -= pkt1->pkt.size + sizeof(*pkt1);
			*pkt = pkt1->pkt;
			av_free(pkt1);
			ret = 1;
			break;
		} else if (!block) {
			ret = 0;
			break;
		} else {
			SDL_CondWait(q->cond, q->mutex);
		}
	}
	SDL_UnlockMutex(q->mutex);
	return ret;
}

static inline void fill_rectangle(SDL_Surface *screen,
	int x, int y, int w, int h, int color)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_FillRect(screen, &rect, color);
}

#define ALPHA_BLEND(a, oldp, newp, s)\
	((((oldp << s) * (255 - (a))) + (newp * (a))) / (255 << s))

#define RGBA_IN(r, g, b, a, s)\
{\
	unsigned int v = ((const uint32_t *)(s))[0];\
	a = (v >> 24) & 0xff;\
	r = (v >> 16) & 0xff;\
	g = (v >> 8) & 0xff;\
	b = v & 0xff;\
}

#define YUVA_IN(y, u, v, a, s, pal)\
{\
	unsigned int val = ((const uint32_t *)(pal))[*(const uint8_t*)(s)];\
	a = (val >> 24) & 0xff;\
	y = (val >> 16) & 0xff;\
	u = (val >> 8) & 0xff;\
	v = val & 0xff;\
}

#define YUVA_OUT(d, y, u, v, a)\
{\
	((uint32_t *)(d))[0] = (a << 24) | (y << 16) | (u << 8) | v;\
}


#define BPP 1

static void blend_subrect(AVPicture *dst, const AVSubtitleRect *rect, int imgw, int imgh)
{
	int wrap, wrap3, width2, skip2;
	int y, u, v, a, u1, v1, a1, w, h;
	uint8_t *lum, *cb, *cr;
	const uint8_t *p;
	const uint32_t *pal;
	int dstx, dsty, dstw, dsth;

	dstw = av_clip(rect->w, 0, imgw);
	dsth = av_clip(rect->h, 0, imgh);
	dstx = av_clip(rect->x, 0, imgw - dstw);
	dsty = av_clip(rect->y, 0, imgh - dsth);
	lum = dst->data[0] + dsty * dst->linesize[0];
	cb  = dst->data[1] + (dsty >> 1) * dst->linesize[1];
	cr  = dst->data[2] + (dsty >> 1) * dst->linesize[2];

	width2 = ((dstw + 1) >> 1) + (dstx & ~dstw & 1);
	skip2 = dstx >> 1;
	wrap = dst->linesize[0];
	wrap3 = rect->pict.linesize[0];
	p = rect->pict.data[0];
	pal = (const uint32_t *)rect->pict.data[1];  /* Now in YCrCb! */

	if (dsty & 1) {
		lum += dstx;
		cb += skip2;
		cr += skip2;

		if (dstx & 1) {
			YUVA_IN(y, u, v, a, p, pal);
			lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
			cb[0] = ALPHA_BLEND(a >> 2, cb[0], u, 0);
			cr[0] = ALPHA_BLEND(a >> 2, cr[0], v, 0);
			cb++;
			cr++;
			lum++;
			p += BPP;
		}
		for (w = dstw - (dstx & 1); w >= 2; w -= 2) {
			YUVA_IN(y, u, v, a, p, pal);
			u1 = u;
			v1 = v;
			a1 = a;
			lum[0] = ALPHA_BLEND(a, lum[0], y, 0);

			YUVA_IN(y, u, v, a, p + BPP, pal);
			u1 += u;
			v1 += v;
			a1 += a;
			lum[1] = ALPHA_BLEND(a, lum[1], y, 0);
			cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u1, 1);
			cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v1, 1);
			cb++;
			cr++;
			p += 2 * BPP;
			lum += 2;
		}
		if (w) {
			YUVA_IN(y, u, v, a, p, pal);
			lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
			cb[0] = ALPHA_BLEND(a >> 2, cb[0], u, 0);
			cr[0] = ALPHA_BLEND(a >> 2, cr[0], v, 0);
			p++;
			lum++;
		}
		p += wrap3 - dstw * BPP;
		lum += wrap - dstw - dstx;
		cb += dst->linesize[1] - width2 - skip2;
		cr += dst->linesize[2] - width2 - skip2;
	}
	for (h = dsth - (dsty & 1); h >= 2; h -= 2) {
		lum += dstx;
		cb += skip2;
		cr += skip2;

		if (dstx & 1) {
			YUVA_IN(y, u, v, a, p, pal);
			u1 = u;
			v1 = v;
			a1 = a;
			lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
			p += wrap3;
			lum += wrap;
			YUVA_IN(y, u, v, a, p, pal);
			u1 += u;
			v1 += v;
			a1 += a;
			lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
			cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u1, 1);
			cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v1, 1);
			cb++;
			cr++;
			p += -wrap3 + BPP;
			lum += -wrap + 1;
		}
		for (w = dstw - (dstx & 1); w >= 2; w -= 2) {
			YUVA_IN(y, u, v, a, p, pal);
			u1 = u;
			v1 = v;
			a1 = a;
			lum[0] = ALPHA_BLEND(a, lum[0], y, 0);

			YUVA_IN(y, u, v, a, p + BPP, pal);
			u1 += u;
			v1 += v;
			a1 += a;
			lum[1] = ALPHA_BLEND(a, lum[1], y, 0);
			p += wrap3;
			lum += wrap;

			YUVA_IN(y, u, v, a, p, pal);
			u1 += u;
			v1 += v;
			a1 += a;
			lum[0] = ALPHA_BLEND(a, lum[0], y, 0);

			YUVA_IN(y, u, v, a, p + BPP, pal);
			u1 += u;
			v1 += v;
			a1 += a;
			lum[1] = ALPHA_BLEND(a, lum[1], y, 0);

			cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u1, 2);
			cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v1, 2);

			cb++;
			cr++;
			p += -wrap3 + 2 * BPP;
			lum += -wrap + 2;
		}
		if (w) {
			YUVA_IN(y, u, v, a, p, pal);
			u1 = u;
			v1 = v;
			a1 = a;
			lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
			p += wrap3;
			lum += wrap;
			YUVA_IN(y, u, v, a, p, pal);
			u1 += u;
			v1 += v;
			a1 += a;
			lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
			cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u1, 1);
			cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v1, 1);
			cb++;
			cr++;
			p += -wrap3 + BPP;
			lum += -wrap + 1;
		}
		p += wrap3 + (wrap3 - dstw * BPP);
		lum += wrap + (wrap - dstw - dstx);
		cb += dst->linesize[1] - width2 - skip2;
		cr += dst->linesize[2] - width2 - skip2;
	}
	/* handle odd height */
	if (h) {
		lum += dstx;
		cb += skip2;
		cr += skip2;

		if (dstx & 1) {
			YUVA_IN(y, u, v, a, p, pal);
			lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
			cb[0] = ALPHA_BLEND(a >> 2, cb[0], u, 0);
			cr[0] = ALPHA_BLEND(a >> 2, cr[0], v, 0);
			cb++;
			cr++;
			lum++;
			p += BPP;
		}
		for (w = dstw - (dstx & 1); w >= 2; w -= 2) {
			YUVA_IN(y, u, v, a, p, pal);
			u1 = u;
			v1 = v;
			a1 = a;
			lum[0] = ALPHA_BLEND(a, lum[0], y, 0);

			YUVA_IN(y, u, v, a, p + BPP, pal);
			u1 += u;
			v1 += v;
			a1 += a;
			lum[1] = ALPHA_BLEND(a, lum[1], y, 0);
			cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u, 1);
			cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v, 1);
			cb++;
			cr++;
			p += 2 * BPP;
			lum += 2;
		}
		if (w) {
			YUVA_IN(y, u, v, a, p, pal);
			lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
			cb[0] = ALPHA_BLEND(a >> 2, cb[0], u, 0);
			cr[0] = ALPHA_BLEND(a >> 2, cr[0], v, 0);
		}
	}
}

static void free_subpicture(SubPicture *sp)
{
	avsubtitle_free(&sp->sub);
}

void ffmfc_videoDisplaySize(int width, int height)
{
    //videoStateMutex.lock();
	if (g_is != nullptr)
	{
		g_is->width = width;
		g_is->height = height;
		//video_image_display(g_is);
	}
    //videoStateMutex.unlock();
}


static void calculate_display_rect(SDL_Rect *rect, int scr_xleft, int scr_ytop, int scr_width, int scr_height, VideoPicture *vp)
{
	float aspect_ratio;
	int width, height, x, y;

	if (vp->sample_aspect_ratio.num == 0)
		aspect_ratio = 0;
	else
		aspect_ratio = av_q2d(vp->sample_aspect_ratio);

	if (aspect_ratio <= 0.0)
		aspect_ratio = 1.0;
	aspect_ratio *= (float)vp->width / (float)vp->height;

	/* XXX: we suppose the screen has a 1.0 pixel ratio */
	height = scr_height;
	width = ((int)rint(height * aspect_ratio)) & ~1;
	if (width > scr_width) {
		width = scr_width;
		height = ((int)rint(width / aspect_ratio)) & ~1;
	}
	x = (scr_width - width) / 2;
	y = (scr_height - height) / 2;
	rect->x = scr_xleft + x;
	rect->y = scr_ytop  + y;
	rect->w = FFMAX(width,  1);
	rect->h = FFMAX(height, 1);
}

static void video_image_display(VideoState *is)
{
	VideoPicture *vp;
	SubPicture *sp;
	AVPicture pict;
	SDL_Rect rect;
	int i;

	vp = &is->pictq[is->pictq_rindex];
	if (vp->bmp) {
		if (is->subtitle_st) {
			if (is->subpq_size > 0) {
				sp = &is->subpq[is->subpq_rindex];

				if (vp->pts >= sp->pts + ((float) sp->sub.start_display_time / 1000))
				{
					SDL_LockSurface(vp->bmp);
					//Ò»°ã²»»áÖ´ÐÐ´Ë´¦-----------------
					pict.data[0] = (uint8_t *)vp->bmp->pixels;
					pict.linesize[0] = vp->bmp->pitch;

					for (i = 0; i < sp->sub.num_rects; i++)
						blend_subrect(&pict, sp->sub.rects[i],
						vp->bmp->w, vp->bmp->h);
					
					SDL_UnlockSurface(vp->bmp);
				}
			}
		}

		float aspect_ratio;

		if (vp->sample_aspect_ratio.num == 0)
			aspect_ratio = 0;
		else
			aspect_ratio = av_q2d(vp->sample_aspect_ratio);
		//calculate_display_rect(&rect, is->xleft, is->ytop, is->width, is->height, vp);
        
        CBitmapToShow * bitmapToShow = new CBitmapToShow();
        
        SDL_LockSurface(vp->bmp);
        
        int pictureSize = 0;
        
        if (videoOutputMode == 24)
            pictureSize =  vp->bmp->w * vp->bmp->h * 3;
        else
            pictureSize =  vp->bmp->w * vp->bmp->h * 4;
        
        
        bitmapToShow->data = new uint8_t[pictureSize];
        bitmapToShow->width = vp->bmp->w;
        bitmapToShow->height = vp->bmp->h;
        
        memcpy(bitmapToShow->data, (uint8_t *)vp->bmp->pixels, pictureSize);
        
        SDL_UnlockSurface(vp->bmp);
        
		 bitmapToShow->aspect_ratio = aspect_ratio;
        wxCommandEvent event(EVENT_REFRESHSCREEN);
        event.SetClientData(bitmapToShow);
        wxPostEvent(dlg, event);
        
		//dlg->RefreshScreen(vp->bmp, rect);
		
	}
}

static inline int compute_mod(int a, int b)
{
	return a < 0 ? a%b + b : a%b;
}

static void video_audio_display(VideoState *s)
{


    int i, i_start, x, delay, n, nb_display_channels;//, y1, y, ys;
    int channels, h;//, h2, ch, bgcolor, fgcolor;
	int16_t time_diff;
	int rdft_bits, nb_freq;

	for (rdft_bits = 1; (1 << rdft_bits) < 2 * s->height; rdft_bits++)
		;
	nb_freq = 1 << (rdft_bits - 1);

	/* compute display index : center on currently output samples */
	channels = s->audio_tgt.channels;
	nb_display_channels = channels;
	if (!s->paused) {
		int data_used= s->show_mode == SHOW_MODE_WAVES ? s->width : (2*nb_freq);
		n = 2 * channels;
		delay = s->audio_write_buf_size;
		delay /= n;

		/* to be more precise, we take into account the time spent since
		the last buffer computation */
		if (audio_callback_time) {
			time_diff = av_gettime() - audio_callback_time;
			delay -= (time_diff * s->audio_tgt.freq) / 1000000;
		}

		delay += 2 * data_used;
		if (delay < data_used)
			delay = data_used;

		i_start= x = compute_mod(s->sample_array_index - delay * channels, SAMPLE_ARRAY_SIZE);
		if (s->show_mode == SHOW_MODE_WAVES) {
			h = INT_MIN;
			for (i = 0; i < 1000; i += channels) {
				int idx = (SAMPLE_ARRAY_SIZE + x - i) % SAMPLE_ARRAY_SIZE;
				int a = s->sample_array[idx];
				int b = s->sample_array[(idx + 4 * channels) % SAMPLE_ARRAY_SIZE];
				int c = s->sample_array[(idx + 5 * channels) % SAMPLE_ARRAY_SIZE];
				int d = s->sample_array[(idx + 9 * channels) % SAMPLE_ARRAY_SIZE];
				int score = a - d;
				if (h < score && (b ^ c) < 0) {
					h = score;
					i_start = idx;
				}
			}
		}

		s->last_i_start = i_start;
	} else {
		i_start = s->last_i_start;
	}

#ifdef DISPLAY_VIDEO
	bgcolor = SDL_MapRGB(dlg->screen->format, 0x00, 0x00, 0x00);
	if (s->show_mode == SHOW_MODE_WAVES) {
		fill_rectangle(dlg->screen,
			s->xleft, s->ytop, s->width, s->height,
			bgcolor);

		fgcolor = SDL_MapRGB(dlg->screen->format, 0xff, 0xff, 0xff);

		/* total height for one channel */
		h = s->height / nb_display_channels;
		/* graph height / 2 */
		h2 = (h * 9) / 20;
		for (ch = 0; ch < nb_display_channels; ch++) {
			i = i_start + ch;
			y1 = s->ytop + ch * h + (h / 2); /* position of center line */
			for (x = 0; x < s->width; x++) {
				y = (s->sample_array[i] * h2) >> 15;
				if (y < 0) {
					y = -y;
					ys = y1 - y;
				} else {
					ys = y1;
				}
				fill_rectangle(dlg->screen,
					s->xleft + x, ys, 1, y,
					fgcolor);
				i += channels;
				if (i >= SAMPLE_ARRAY_SIZE)
					i -= SAMPLE_ARRAY_SIZE;
			}
		}

		fgcolor = SDL_MapRGB(dlg->screen->format, 0x00, 0x00, 0xff);

		for (ch = 1; ch < nb_display_channels; ch++) {
			y = s->ytop + ch * h;
			fill_rectangle(dlg->screen,
				s->xleft, y, s->width, 1,
				fgcolor);
		}
		SDL_UpdateRect(dlg->screen, s->xleft, s->ytop, s->width, s->height);
	} else {
		nb_display_channels= FFMIN(nb_display_channels, 2);
		if (rdft_bits != s->rdft_bits) {
			av_rdft_end(s->rdft);
			av_free(s->rdft_data);
			s->rdft = av_rdft_init(rdft_bits, DFT_R2C);
			s->rdft_bits = rdft_bits;
			s->rdft_data = (FFTSample *)av_malloc(4 * nb_freq * sizeof(*s->rdft_data));
		}
		{
			FFTSample *data[2];
			for (ch = 0; ch < nb_display_channels; ch++) {
				data[ch] = s->rdft_data + 2 * nb_freq * ch;
				i = i_start + ch;
				for (x = 0; x < 2 * nb_freq; x++) {
					double w = (x-nb_freq) * (1.0 / nb_freq);
					data[ch][x] = s->sample_array[i] * (1.0 - w * w);
					i += channels;
					if (i >= SAMPLE_ARRAY_SIZE)
						i -= SAMPLE_ARRAY_SIZE;
				}
				av_rdft_calc(s->rdft, data[ch]);
			}
			// least efficient way to do this, we should of course directly access it but its more than fast enough
			for (y = 0; y < s->height; y++) {
				double w = 1 / sqrt((float)nb_freq);
				int a = sqrt(w * sqrt(data[0][2 * y + 0] * data[0][2 * y + 0] + data[0][2 * y + 1] * data[0][2 * y + 1]));
				int b = (nb_display_channels == 2 ) ? sqrt(w * sqrt(data[1][2 * y + 0] * data[1][2 * y + 0]
				+ data[1][2 * y + 1] * data[1][2 * y + 1])) : a;
				a = FFMIN(a, 255);
				b = FFMIN(b, 255);
				fgcolor = SDL_MapRGB(dlg->screen->format, a, b, (a + b) / 2);

				fill_rectangle(dlg->screen,
					s->xpos, s->height-y, 1, 1,
					fgcolor);
			}
		}
		SDL_UpdateRect(dlg->screen, s->xpos, s->ytop, 1, s->height);
		if (!s->paused)
			s->xpos++;
		if (s->xpos >= s->width)
			s->xpos= s->xleft;
	}
#endif
}

static void stream_close(VideoState *is)
{
	VideoPicture *vp;
	int i;
	/* XXX: use a special url_shutdown call to abort parse cleanly */
	is->abort_request = 1;
	SDL_WaitThread(is->read_tid, nullptr);
	SDL_WaitThread(is->refresh_tid, nullptr);
	packet_queue_destroy(&is->videoq);
	packet_queue_destroy(&is->audioq);
	packet_queue_destroy(&is->subtitleq);

	/* free all pictures */
	for (i = 0; i < VIDEO_PICTURE_QUEUE_SIZE; i++) {
		vp = &is->pictq[i];
#if CONFIG_AVFILTER
		avfilter_unref_bufferp(&vp->picref);
#endif
		if (vp->bmp) {
			SDL_FreeSurface(vp->bmp);
			vp->bmp = nullptr;
		}
	}
	SDL_DestroyMutex(is->pictq_mutex);
	SDL_DestroyCond(is->pictq_cond);
	SDL_DestroyMutex(is->subpq_mutex);
	SDL_DestroyCond(is->subpq_cond);
	SDL_DestroyCond(is->continue_read_thread);
#if !CONFIG_AVFILTER
	if (is->img_convert_ctx)
		sws_freeContext(is->img_convert_ctx);
#endif
	av_free(is);
}

//ÍË³ö
static void do_exit(VideoState *is)
{
	exit_remark=1;
	if (is) {
		//Ö±½Óabort_request = 1²¢sleepÊÇ²»¸ºÔðÈÎµÄ·½·¨
		//ÕâÑù³ÌÐò»áÖ±½ÓÍË³ö
		//²»ÄÜÍê³ÉÇåÀíÈÎÎñ£¡
		//is->abort_request = 1;
		//Sleep(2000);
		stream_close(is);
	}
	av_lockmgr_register(nullptr);
	uninit_opts();
#if CONFIG_AVFILTER
	avfilter_uninit();
	av_freep(&vfilters);
#endif
	avformat_network_deinit();
	if (show_status)
		printf("\n");
	//SDL_Quit();
	av_log(nullptr, AV_LOG_QUIET, "%s", "");

	//²»ÄÜÖ±½ÓÊ¹ÓÃexit(0)£¬·ñÔòÕû¸ö³ÌÐò»áÍË³ö
	//exit(0);
	//¸ÄÎªÒ»¸ö±ê¼Ç£¬ÔÚevent_loopÖÐÅÐ¶Ï²¢×Ô¶¯ÍË³ö
	//exit_remark=1;
}

//·¢ËÍ¡°ÍË³ö¡±ÃüÁî
//Send Command "Quit"
void ffmfc_quit()
{
	//do_exit(g_is);
	if (g_is)
	{
		SDL_Event event;
		event.type = FF_QUIT_EVENT;
		event.user.data1 = g_is;
		{
			//²»ÄÜÖ±½Ó°Ñis->abort_requestÉèÖÃÎª1£»·ñÔò»á´Óevent_loop()Ñ­»·ÖÐÖ±½ÓÌø³ö
			//´Ó¶øÎÞ·¨½øÐÐÇåÀí²Ù×÷
			//g_is->abort_request = 1;
			SDL_PushEvent(&event);
		}
	}
	else
	{
		do_exit(nullptr);
	}

}

static void sigterm_handler(int sig)
{
	exit(123);
}
//SDL³õÊ¼»¯ÉèÖÃ
static int video_open(VideoState *is, int force_set_video_mode)
{

#ifdef DISPLAY_VIDEO
    int w,h;
    VideoPicture *vp = &is->pictq[is->pictq_rindex];
    SDL_Rect rect;
    int flags = SDL_HWSURFACE | SDL_ASYNCBLIT | SDL_HWACCEL;

	if (is_full_screen) flags |= SDL_FULLSCREEN;
	else                flags |= SDL_RESIZABLE;

	if (is_full_screen && fs_screen_width) {
		w = fs_screen_width;
		h = fs_screen_height;
	} else if (!is_full_screen && screen_width) {
		w = screen_width;
		h = screen_height;
	} else if (vp->width) {
		calculate_display_rect(&rect, 0, 0, INT_MAX, vp->height, vp);
		w = rect.w;
		h = rect.h;
	} else {
		w = 640;
		h = 480;
	}
	if (screen && is->width == screen->w && screen->w == w
		&& is->height== screen->h && screen->h == h && !force_set_video_mode)
		return 0;
	screen = SDL_SetVideoMode(w, h, 0, flags);
	if (!screen) {
		//AfxMessageBox(_T("SDL: could not set video mode - exiting"));
		do_exit(is);
	}
	//×¢Òâ£ºÉèÖÃÊÓÆµ´°¿Ú±êÌâ£¡
	window_title="Video Window";
	if (!window_title)
		window_title = input_filename;
	SDL_WM_SetCaption(window_title, window_title);

	is->width  = dlg->screen->w;
	is->height = dlg->screen->h;
#endif
	return 0;
}

/* display the current picture, if any */
static void video_display(VideoState *is)
{
#ifdef DISPLAY_VIDEO
	if (!dlg->screen)
		video_open(is, 0);
#endif
	if (is->audio_st && is->show_mode != SHOW_MODE_VIDEO)
		video_audio_display(is);
	else if (is->video_st)
		video_image_display(is);
}

static int refresh_thread(void *opaque)
{
	VideoState *is= (VideoState *)opaque;
	while (!is->abort_request) {
		SDL_Event event;
		event.type = FF_REFRESH_EVENT;
		event.user.data1 = opaque;
        if(is->ic != nullptr)
        {
            if (!is->refresh && (!is->paused || is->force_refresh)) {
                is->refresh = 1;
                SDL_PushEvent(&event);
            }
        }
		//FIXME ideally we should wait the correct time but SDLs event passing is so slow it would be silly
		av_usleep(is->audio_st && is->show_mode != SHOW_MODE_VIDEO ? rdftspeed*1000 : 5000);
	}
	return 0;
}

/* get the current audio clock value */
static double get_audio_clock(VideoState *is)
{
	if (is->paused) {
		return is->audio_current_pts;
	} else {
		return is->audio_current_pts_drift + av_gettime() / 1000000.0;
	}
}

/* get the current video clock value */
static double get_video_clock(VideoState *is)
{
	if (is->paused) {
		return is->video_current_pts;
	} else {
		return is->video_current_pts_drift + av_gettime() / 1000000.0;
	}
}

/* get the current external clock value */
static double get_external_clock(VideoState *is)
{
	int64_t ti;
	ti = av_gettime();
	return is->external_clock + ((ti - is->external_clock_time) * 1e-6);
}

/* get the current master clock value */
static double get_master_clock(VideoState *is)
{
	double val;

	if (is->av_sync_type == AV_SYNC_VIDEO_MASTER) {
		if (is->video_st)
			val = get_video_clock(is);
		else
			val = get_audio_clock(is);
	} else if (is->av_sync_type == AV_SYNC_AUDIO_MASTER) {
		if (is->audio_st)
			val = get_audio_clock(is);
		else
			val = get_video_clock(is);
	} else {
		val = get_external_clock(is);
	}
	return val;
}

/* seek in the stream */
static void stream_seek(VideoState *is, int64_t pos, int64_t rel, int seek_by_bytes)
{
	if (!is->seek_req) {
		is->seek_pos = pos;
		is->seek_rel = rel;
		is->seek_flags &= ~AVSEEK_FLAG_BYTE;
		if (seek_by_bytes)
			is->seek_flags |= AVSEEK_FLAG_BYTE;
		is->seek_req = 1;
	}
}

/* pause or resume the video */
static void stream_toggle_pause(VideoState *is)
{
	//bool pause = GetPausedValue();
	if (is->paused) {
		is->frame_timer += av_gettime() / 1000000.0 + is->video_current_pts_drift - is->video_current_pts;
		if (is->read_pause_return != AVERROR(ENOSYS)) {
			is->video_current_pts = is->video_current_pts_drift + av_gettime() / 1000000.0;
		}
		is->video_current_pts_drift = is->video_current_pts - av_gettime() / 1000000.0;
	}
	//SetPausedValue(!pause);
	is->paused = !is->paused;

}

static double compute_target_delay(double delay, VideoState *is)
{
	double sync_threshold, diff;

	/* update delay to follow master synchronisation source */
	if (((is->av_sync_type == AV_SYNC_AUDIO_MASTER && is->audio_st) ||
		is->av_sync_type == AV_SYNC_EXTERNAL_CLOCK)) {
			/* if video is slave, we try to correct big delays by
			duplicating or deleting a frame */
			diff = get_video_clock(is) - get_master_clock(is);

			/* skip or repeat frame. We take into account the
			delay to compute the threshold. I still don't know
			if it is the best guess */
			sync_threshold = FFMAX(AV_SYNC_THRESHOLD, delay);
			if (fabs(diff) < AV_NOSYNC_THRESHOLD) {
				if (diff <= -sync_threshold)
					delay = 0;
				else if (diff >= sync_threshold)
					delay = 2 * delay;
			}
	}

	//av_dlog(nullptr, "video: delay=%0.3f A-V=%f\n",       delay, -diff);

	return delay;
}

static void pictq_next_picture(VideoState *is) {
	/* update queue size and signal for next picture */
	if (++is->pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE)
		is->pictq_rindex = 0;

	SDL_LockMutex(is->pictq_mutex);
	is->pictq_size--;
	SDL_CondSignal(is->pictq_cond);
	SDL_UnlockMutex(is->pictq_mutex);
}

static void pictq_prev_picture(VideoState *is) {
	VideoPicture *prevvp;
	/* update queue size and signal for the previous picture */
	prevvp = &is->pictq[(is->pictq_rindex + VIDEO_PICTURE_QUEUE_SIZE - 1) % VIDEO_PICTURE_QUEUE_SIZE];
	if (prevvp->allocated && !prevvp->skip) {
		SDL_LockMutex(is->pictq_mutex);
		if (is->pictq_size < VIDEO_PICTURE_QUEUE_SIZE - 1) {
			if (--is->pictq_rindex == -1)
				is->pictq_rindex = VIDEO_PICTURE_QUEUE_SIZE - 1;
			is->pictq_size++;
		}
		SDL_CondSignal(is->pictq_cond);
		SDL_UnlockMutex(is->pictq_mutex);
	}
}

static void update_video_pts(VideoState *is, double pts, int64_t pos) {
	double time = av_gettime() / 1000000.0;
	/* update current video pts */
	is->video_current_pts = pts;
	is->video_current_pts_drift = is->video_current_pts - time;
	is->video_current_pos = pos;
	is->frame_last_pts = pts;
}

/* called to display each frame */
//Ã¿40msµ÷ÓÃÒ»´Î£¬ÏÔÊ¾Í¼Æ¬ÓÃ
static void video_refresh(void *opaque)
{
	VideoState *is = (VideoState *)opaque;
	VideoPicture *vp;
	double time;

	SubPicture *sp, *sp2;
    
    if(g_is != is)
        return;
    
    if(is->ic == nullptr)
        return;

	if (is->video_st) {
		if (is->force_refresh)
			pictq_prev_picture(is);
retry:
		if (is->pictq_size == 0) {
			SDL_LockMutex(is->pictq_mutex);
			if (is->frame_last_dropped_pts != AV_NOPTS_VALUE && is->frame_last_dropped_pts > is->frame_last_pts) {
				update_video_pts(is, is->frame_last_dropped_pts, is->frame_last_dropped_pos);
				is->frame_last_dropped_pts = AV_NOPTS_VALUE;
			}
			SDL_UnlockMutex(is->pictq_mutex);
			// nothing to do, no picture to display in the que
		} else {
			double last_duration, duration, delay;
			/* dequeue the picture */
			vp = &is->pictq[is->pictq_rindex];

			if (vp->skip) {
				pictq_next_picture(is);
				goto retry;
			}

			if (is->paused)
				goto display;

			/* compute nominal last_duration */
			last_duration = vp->pts - is->frame_last_pts;
			if (last_duration > 0 && last_duration < 10.0) {
				/* if duration of the last frame was sane, update last_duration in video state */
				is->frame_last_duration = last_duration;
			}
			delay = compute_target_delay(is->frame_last_duration, is);

			time= av_gettime()/1000000.0;
			if (time < is->frame_timer + delay)
				return;

			if (delay > 0)
				is->frame_timer += delay * FFMAX(1, floor((time-is->frame_timer) / delay));

			SDL_LockMutex(is->pictq_mutex);
			update_video_pts(is, vp->pts, vp->pos);
			SDL_UnlockMutex(is->pictq_mutex);

			if (is->pictq_size > 1) {
				VideoPicture *nextvp = &is->pictq[(is->pictq_rindex + 1) % VIDEO_PICTURE_QUEUE_SIZE];
				duration = nextvp->pts - vp->pts;
				if((framedrop>0 || (framedrop && is->audio_st)) && time > is->frame_timer + duration){
					is->frame_drops_late++;
					pictq_next_picture(is);
					goto retry;
				}
			}

			if (is->subtitle_st) {
				if (is->subtitle_stream_changed) {
					SDL_LockMutex(is->subpq_mutex);

					while (is->subpq_size) {
						free_subpicture(&is->subpq[is->subpq_rindex]);

						/* update queue size and signal for next picture */
						if (++is->subpq_rindex == SUBPICTURE_QUEUE_SIZE)
							is->subpq_rindex = 0;

						is->subpq_size--;
					}
					is->subtitle_stream_changed = 0;

					SDL_CondSignal(is->subpq_cond);
					SDL_UnlockMutex(is->subpq_mutex);
				} else {
					if (is->subpq_size > 0) {
						sp = &is->subpq[is->subpq_rindex];

						if (is->subpq_size > 1)
							sp2 = &is->subpq[(is->subpq_rindex + 1) % SUBPICTURE_QUEUE_SIZE];
						else
							sp2 = nullptr;

						if ((is->video_current_pts > (sp->pts + ((float) sp->sub.end_display_time / 1000)))
							|| (sp2 && is->video_current_pts > (sp2->pts + ((float) sp2->sub.start_display_time / 1000))))
						{
							free_subpicture(sp);

							/* update queue size and signal for next picture */
							if (++is->subpq_rindex == SUBPICTURE_QUEUE_SIZE)
								is->subpq_rindex = 0;

							SDL_LockMutex(is->subpq_mutex);
							is->subpq_size--;
							SDL_CondSignal(is->subpq_cond);
							SDL_UnlockMutex(is->subpq_mutex);
						}
					}
				}
			}

display:
			/* display picture */
			if (!display_disable)
				video_display(is);

			pictq_next_picture(is);
		}
	} else if (is->audio_st) {
		/* draw the next audio frame */

		/* if only audio stream, then display the audio bars (better
		than nothing, just to test the implementation */

		/* display picture */
		if (!display_disable)
			video_display(is);
	}
	is->force_refresh = 0;
	if (show_status) {
		static int64_t last_time;
		int64_t cur_time;
		int aqsize, vqsize, sqsize;
		double av_diff;

		cur_time = av_gettime();
		if (!last_time || (cur_time - last_time) >= 30000) {
			aqsize = 0;
			vqsize = 0;
			sqsize = 0;
			if (is->audio_st)
				aqsize = is->audioq.size;
			if (is->video_st)
				vqsize = is->videoq.size;
			if (is->subtitle_st)
				sqsize = is->subtitleq.size;
			av_diff = 0;
			if (is->audio_st && is->video_st)
				av_diff = get_audio_clock(is) - get_video_clock(is);
			printf("%7.2f A-V:%7.3f fd=%4d aq=%5dKB vq=%5dKB sq=%5dB f=%" PRId64"/%" PRId64"   \r",
				get_master_clock(is),
				av_diff,
				is->frame_drops_early + is->frame_drops_late,
				aqsize / 1024,
				vqsize / 1024,
				sqsize,
				is->video_st ? is->video_st->codec->pts_correction_num_faulty_dts : 0,
				is->video_st ? is->video_st->codec->pts_correction_num_faulty_pts : 0);

			//int pos=1000*get_master_clock(is)/(is->ic->duration/1000000);
			dlg->SetPos(get_master_clock(is) * 1000);

#ifdef MFC
			CString currentclockstr;
			int tns, thh, tmm, tss;
			tns  = get_master_clock(is);
			thh  = tns / 3600;
			tmm  = (tns % 3600) / 60;
			tss  = (tns % 60);
			currentclockstr.Format(_T("%02d:%02d:%02d"),thh,tmm,tss);
			dlg->m_currentclock.SetWindowText(currentclockstr);
#endif
			fflush(stdout);
			last_time = cur_time;
		}
	}
}

/* allocate a picture (needs to do that in main thread to avoid
potential locking problems */
static void alloc_picture(VideoState *is)
{
	VideoPicture *vp;

	vp = &is->pictq[is->pictq_windex];

	if (vp->bmp)
		SDL_FreeSurface(vp->bmp);

#if CONFIG_AVFILTER
	avfilter_unref_bufferp(&vp->picref);
#endif

	video_open(is, 0);

	if (videoOutputMode == 24)
	{
		vp->bmp = SDL_CreateRGBSurface(SDL_SWSURFACE, vp->width, vp->height, 24, 0, 0, 0, 0);
	}
	else
	{
		vp->bmp = SDL_CreateRGBSurface(SDL_SWSURFACE, vp->width, vp->height, 32, 0, 0, 0, 0);
	}

	if (!vp->bmp) {
		/* SDL allocates a buffer smaller than requested if the video
		* overlay hardware is unable to support the requested size. */
		fprintf(stderr, "Error: the video system does not support an image\n"
			"size of %dx%d pixels. Try using -lowres or -vf \"scale=w:h\"\n"
			"to reduce the image size.\n", vp->width, vp->height );
		do_exit(is);
	}

	SDL_LockMutex(is->pictq_mutex);
	vp->allocated = 1;
	SDL_CondSignal(is->pictq_cond);
	SDL_UnlockMutex(is->pictq_mutex);
}
//½âÂë³É¹¦ºóÓÃÓÚÏÔÊ¾£¬Ò²ÊÇ·Åµ½ÁíÒ»¸ö¶ÓÁÐÖÐ£¿
static int queue_picture(VideoState *is, AVFrame *src_frame, double pts1, int64_t pos)
{
	VideoPicture *vp;
	double frame_delay, pts = pts1;

	/* compute the exact PTS for the picture if it is omitted in the stream
	* pts1 is the dts of the pkt / pts of the frame */
	if (pts != 0) {
		/* update video clock with pts, if present */
		is->video_clock = pts;
	} else {
		pts = is->video_clock;
	}
	/* update video clock for next frame */
	frame_delay = av_q2d(is->video_st->codec->time_base);
	/* for MPEG2, the frame can be repeated, so we update the
	clock accordingly */
	frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);
	is->video_clock += frame_delay;

#if defined(DEBUG_SYNC) && 0
	printf("frame_type=%c clock=%0.3f pts=%0.3f\n",
		av_get_picture_type_char(src_frame->pict_type), pts, pts1);
#endif

	/* wait until we have space to put a new picture */
	SDL_LockMutex(is->pictq_mutex);

	/* keep the last already displayed picture in the queue */
	while (is->pictq_size >= VIDEO_PICTURE_QUEUE_SIZE - 2 &&
		!is->videoq.abort_request) {
			SDL_CondWait(is->pictq_cond, is->pictq_mutex);
	}
	SDL_UnlockMutex(is->pictq_mutex);

	if (is->videoq.abort_request)
		return -1;

	vp = &is->pictq[is->pictq_windex];

#if CONFIG_AVFILTER
	vp->sample_aspect_ratio = ((AVFilterBufferRef *)src_frame->opaque)->video->sample_aspect_ratio;
#else
	vp->sample_aspect_ratio = av_guess_sample_aspect_ratio(is->ic, is->video_st, src_frame);
#endif

	/* alloc or resize hardware picture buffer */
	if (!vp->bmp || vp->reallocate || !vp->allocated ||
		vp->width  != src_frame->width ||
		vp->height != src_frame->height) {
			SDL_Event event;

			vp->allocated  = 0;
			vp->reallocate = 0;
			vp->width = src_frame->width;
			vp->height = src_frame->height;

			/* the allocation must be done in the main thread to avoid
			locking problems. */
			event.type = FF_ALLOC_EVENT;
			event.user.data1 = is;
			SDL_PushEvent(&event);

			/* wait until the picture is allocated */
			SDL_LockMutex(is->pictq_mutex);
			while (!vp->allocated && !is->videoq.abort_request) {
				SDL_CondWait(is->pictq_cond, is->pictq_mutex);
			}
        
#ifdef SDL2
        /* if the queue is aborted, we have to pop the pending ALLOC event or wait for the allocation to complete */
        if (is->videoq.abort_request && SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_USEREVENT, FF_ALLOC_EVENT) != 1) {
            while (!vp->allocated) {
                SDL_CondWait(is->pictq_cond, is->pictq_mutex);
            }
        }
#else
			/* if the queue is aborted, we have to pop the pending ALLOC event or wait for the allocation to complete */
			if (is->videoq.abort_request && SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_EVENTMASK(FF_ALLOC_EVENT)) != 1) {
				while (!vp->allocated) {
					SDL_CondWait(is->pictq_cond, is->pictq_mutex);
				}
			}
#endif
			SDL_UnlockMutex(is->pictq_mutex);

			if (is->videoq.abort_request)
				return -1;
	}

	/* if the frame is not skipped, then display it */
	if (vp->bmp) {
		AVPicture pict = { { 0 } };
#if CONFIG_AVFILTER
		avfilter_unref_bufferp(&vp->picref);
		vp->picref = (AVFilterBufferRef*)src_frame->opaque;
#endif

		/* get a pointer on the bitmap */
		SDL_LockSurface(vp->bmp);
		//ÏÔÊ¾YUVÊý¾Ý
		//ÉèÖÃÊä³ö¸ñÊ½-----------------
		
		is->v_show_mode = SHOW_MODE_RGB24;

		switch (is->v_show_mode)
		{
		case SHOW_MODE_RGB24:
			pict.data[0] = (uint8_t *)vp->bmp->pixels;
			pict.linesize[0] = vp->bmp->pitch;
			break;
			/*
		case SHOW_MODE_YUV:
			pict.data[0] = vp->bmp->pixels[0];
			pict.data[1] = vp->bmp->pixels[2];
			pict.data[2] = vp->bmp->pixels[1];

			pict.linesize[0] = vp->bmp->pitches[0];
			pict.linesize[1] = vp->bmp->pitches[2];
			pict.linesize[2] = vp->bmp->pitches[1];
			break;
		case SHOW_MODE_Y:	pict.data[0] = vp->bmp->pixels[0];
			pict.linesize[0] = vp->bmp->pitches[0];
			break;
		case SHOW_MODE_U:	pict.data[1] = vp->bmp->pixels[2];
			pict.linesize[1] = vp->bmp->pitches[2];
			break;
		case SHOW_MODE_V:	pict.data[2] = vp->bmp->pixels[1];
			pict.linesize[2] = vp->bmp->pitches[1];
			break;
			*/
		}
		
#if CONFIG_AVFILTER
		// FIXME use direct rendering
		av_picture_copy(&pict, (AVPicture *)src_frame,
			(AVPixelFormat)src_frame->format, vp->width, vp->height);
#else
		//sws_flags = av_get_int(sws_opts, "sws_flags", nullptr);

		if (videoOutputMode == 24)
		{
			is->img_convert_ctx = sws_getCachedContext(is->img_convert_ctx,
				vp->width, vp->height, (AVPixelFormat)(src_frame->format), vp->width, vp->height,
				AV_PIX_FMT_RGB24, sws_flags, nullptr, nullptr, nullptr);
		}
		else
		{
			is->img_convert_ctx = sws_getCachedContext(is->img_convert_ctx,
				vp->width, vp->height, (AVPixelFormat)(src_frame->format), vp->width, vp->height,
				AV_PIX_FMT_RGBA, sws_flags, nullptr, nullptr, nullptr);
		}
		if (is->img_convert_ctx == nullptr) {
			fprintf(stderr, "Cannot initialize the conversion context\n");
			exit(1);
		}
		sws_scale(is->img_convert_ctx, src_frame->data, src_frame->linesize,
			0, vp->height, pict.data, pict.linesize);
#endif
		/* update the bitmap content */
		SDL_UnlockSurface(vp->bmp);

		vp->pts = pts;
		vp->pos = pos;
		vp->skip = 0;

		/* now we can update the picture count */
		if (++is->pictq_windex == VIDEO_PICTURE_QUEUE_SIZE)
			is->pictq_windex = 0;
		SDL_LockMutex(is->pictq_mutex);
		is->pictq_size++;
		SDL_UnlockMutex(is->pictq_mutex);
	}
	return 0;
}
//½âÂëÒ»Ö¡ÊÓÆµ
static int get_video_frame(VideoState *is, AVFrame *frame, int64_t *pts, AVPacket *pkt)
{
	int got_picture, i;
	//´ÓPacket¶ÓÁÐÖÐ»ñÈ¡Packet
	if (packet_queue_get(&is->videoq, pkt, 1) < 0)
		return -1;

	if (pkt->data == flush_pkt.data) {
		avcodec_flush_buffers(is->video_st->codec);

		SDL_LockMutex(is->pictq_mutex);
		// Make sure there are no long delay timers (ideally we should just flush the que but thats harder)
		for (i = 0; i < VIDEO_PICTURE_QUEUE_SIZE; i++) {
			is->pictq[i].skip = 1;
		}
		while (is->pictq_size && !is->videoq.abort_request) {
			SDL_CondWait(is->pictq_cond, is->pictq_mutex);
		}
		is->video_current_pos = -1;
		is->frame_last_pts = AV_NOPTS_VALUE;
		is->frame_last_duration = 0;
		is->frame_timer = (double)av_gettime() / 1000000.0;
		is->frame_last_dropped_pts = AV_NOPTS_VALUE;
		SDL_UnlockMutex(is->pictq_mutex);

		return 0;
	}
	//½âÂë
	if(avcodec_decode_video2(is->video_st->codec, frame, &got_picture, pkt) < 0)
		return 0;

	if (got_picture) {
		//×¢Òâ£º´Ë´¦ÉèÖÃMFC²ÎÊý£¡
		ffmfc_param_vframe(is,frame,pkt);
		//--------------------------
		int ret = 1;

		if (decoder_reorder_pts == -1) {
			*pts = av_frame_get_best_effort_timestamp(frame);
		} else if (decoder_reorder_pts) {
			*pts = frame->pkt_pts;
		} else {
			*pts = frame->pkt_dts;
		}

		if (*pts == AV_NOPTS_VALUE) {
			*pts = 0;
		}

		if (((is->av_sync_type == AV_SYNC_AUDIO_MASTER && is->audio_st) || is->av_sync_type == AV_SYNC_EXTERNAL_CLOCK) &&
			(framedrop>0 || (framedrop && is->audio_st))) {
				SDL_LockMutex(is->pictq_mutex);
				if (is->frame_last_pts != AV_NOPTS_VALUE && *pts) {
					double clockdiff = get_video_clock(is) - get_master_clock(is);
					double dpts = av_q2d(is->video_st->time_base) * *pts;
					double ptsdiff = dpts - is->frame_last_pts;
					if (fabs(clockdiff) < AV_NOSYNC_THRESHOLD &&
						ptsdiff > 0 && ptsdiff < AV_NOSYNC_THRESHOLD &&
						clockdiff + ptsdiff - is->frame_last_filter_delay < 0) {
							is->frame_last_dropped_pos = pkt->pos;
							is->frame_last_dropped_pts = dpts;
							is->frame_drops_early++;
							ret = 0;
					}
				}
				SDL_UnlockMutex(is->pictq_mutex);
		}

		return ret;
	}
	return 0;
}

#if CONFIG_AVFILTER
static int configure_filtergraph(AVFilterGraph *graph, const char *filtergraph,
	AVFilterContext *source_ctx, AVFilterContext *sink_ctx)
{
	int ret;
	AVFilterInOut *outputs = nullptr, *inputs = nullptr;

	if (filtergraph) {
		outputs = avfilter_inout_alloc();
		inputs  = avfilter_inout_alloc();
		if (!outputs || !inputs) {
			ret = AVERROR(ENOMEM);
			goto fail;
		}

		outputs->name       = av_strdup("in");
		outputs->filter_ctx = source_ctx;
		outputs->pad_idx    = 0;
		outputs->next       = nullptr;

		inputs->name        = av_strdup("out");
		inputs->filter_ctx  = sink_ctx;
		inputs->pad_idx     = 0;
		inputs->next        = nullptr;

		if ((ret = avfilter_graph_parse(graph, filtergraph, inputs, outputs, nullptr)) < 0)
			goto fail;
	} else {
		if ((ret = avfilter_link(source_ctx, 0, sink_ctx, 0)) < 0)
			goto fail;
	}

	return avfilter_graph_config(graph, nullptr);
fail:
	avfilter_inout_free(&outputs);
	avfilter_inout_free(&inputs);
	return ret;
}

static int configure_video_filters(AVFilterGraph *graph, VideoState *is, const char *vfilters)
{
	static const enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE };
	char sws_flags_str[128];
	char buffersrc_args[256];
	int ret;
	AVBufferSinkParams *buffersink_params = av_buffersink_params_alloc();
	AVFilterContext *filt_src = nullptr, *filt_out = nullptr, *filt_format, *filt_crop;
	AVCodecContext *codec = is->video_st->codec;

	sprintf(sws_flags_str, "flags=%d", sws_flags);
	graph->scale_sws_opts = av_strdup(sws_flags_str);

	sprintf(buffersrc_args, 
		"video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
		codec->width, codec->height, codec->pix_fmt,
		is->video_st->time_base.num, is->video_st->time_base.den,
		codec->sample_aspect_ratio.num, codec->sample_aspect_ratio.den);

	if ((ret = avfilter_graph_create_filter(&filt_src,
		avfilter_get_by_name("buffer"),
		"ffplay_buffer", buffersrc_args, nullptr,
		graph)) < 0)
		return ret;

	buffersink_params->pixel_fmts = pix_fmts;
	ret = avfilter_graph_create_filter(&filt_out,
		avfilter_get_by_name("ffbuffersink"),
		"ffplay_buffersink", nullptr, buffersink_params, graph);
	av_freep(&buffersink_params);
	if (ret < 0)
		return ret;

	/* SDL YUV code is not handling odd width/height for some driver
	* combinations, therefore we crop the picture to an even width/height. */
	if ((ret = avfilter_graph_create_filter(&filt_crop,
		avfilter_get_by_name("crop"),
		"ffplay_crop", "floor(in_w/2)*2:floor(in_h/2)*2", nullptr, graph)) < 0)
		return ret;
	if ((ret = avfilter_graph_create_filter(&filt_format,
		avfilter_get_by_name("format"),
		"format", "yuv420p", nullptr, graph)) < 0)
		return ret;
	if ((ret = avfilter_link(filt_crop, 0, filt_format, 0)) < 0)
		return ret;
	if ((ret = avfilter_link(filt_format, 0, filt_out, 0)) < 0)
		return ret;

	if ((ret = configure_filtergraph(graph, vfilters, filt_src, filt_crop)) < 0)
		return ret;

	is->in_video_filter  = filt_src;
	is->out_video_filter = filt_out;

	return ret;
}

#endif  /* CONFIG_AVFILTER */
//½âÂëÊÓÆµ
static int video_thread(void *arg)
{
	AVPacket pkt = { 0 };
	VideoState *is = (VideoState *)arg;
	AVFrame *frame = avcodec_alloc_frame();
	int64_t pts_int = AV_NOPTS_VALUE, pos = -1;
	double pts;
	int ret;

#if CONFIG_AVFILTER
	AVCodecContext *codec = is->video_st->codec;
	AVFilterGraph *graph = avfilter_graph_alloc();
	AVFilterContext *filt_out = nullptr, *filt_in = nullptr;
	int last_w = 0;
	int last_h = 0;
	enum AVPixelFormat last_format = (AVPixelFormat)-2;

	if (codec->codec->capabilities & CODEC_CAP_DR1) {
		is->use_dr1 = 1;
		codec->get_buffer     = codec_get_buffer;
		codec->release_buffer = codec_release_buffer;
		codec->opaque         = &is->buffer_pool;
	}
#endif

	for (;;) {
#if CONFIG_AVFILTER
		AVFilterBufferRef *picref;
		AVRational tb;
#endif
		while (is->paused && !is->videoq.abort_request)
			SDL_Delay(10);

		avcodec_get_frame_defaults(frame);
		av_free_packet(&pkt);
		//½âÂëÒ»Ö¡ÊÓÆµ
		ret = get_video_frame(is, frame, &pts_int, &pkt);
		if (ret < 0)
			goto the_end;

		if (!ret)
			continue;

#if CONFIG_AVFILTER
		if (   last_w != is->video_st->codec->width
			|| last_h != is->video_st->codec->height
			|| last_format != is->video_st->codec->pix_fmt) {
				av_log(nullptr, AV_LOG_INFO, "Frame changed from size:%dx%d to size:%dx%d\n",
					last_w, last_h, is->video_st->codec->width, is->video_st->codec->height);
				avfilter_graph_free(&graph);
				graph = avfilter_graph_alloc();
				if ((ret = configure_video_filters(graph, is, vfilters)) < 0) {
					SDL_Event event;
					event.type = FF_QUIT_EVENT;
					event.user.data1 = is;
					SDL_PushEvent(&event);
					av_free_packet(&pkt);
					goto the_end;
				}
				filt_in  = is->in_video_filter;
				filt_out = is->out_video_filter;
				last_w = is->video_st->codec->width;
				last_h = is->video_st->codec->height;
				last_format = is->video_st->codec->pix_fmt;
		}

		frame->pts = pts_int;
		frame->sample_aspect_ratio = av_guess_sample_aspect_ratio(is->ic, is->video_st, frame);
		if (is->use_dr1 && frame->opaque) {
			FrameBuffer      *buf = (FrameBuffer *)frame->opaque;
			AVFilterBufferRef *fb = avfilter_get_video_buffer_ref_from_arrays(
				frame->data, frame->linesize,
				AV_PERM_READ | AV_PERM_PRESERVE,
				frame->width, frame->height,
				(AVPixelFormat)frame->format);

			avfilter_copy_frame_props(fb, frame);
			fb->buf->priv           = buf;
			fb->buf->free           = filter_release_buffer;

			buf->refcount++;
			av_buffersrc_add_ref(filt_in, fb, AV_BUFFERSRC_FLAG_NO_COPY);

		} else
			av_buffersrc_write_frame(filt_in, frame);

		av_free_packet(&pkt);

		while (ret >= 0) {
			is->frame_last_returned_time = av_gettime() / 1000000.0;

			ret = av_buffersink_get_buffer_ref(filt_out, &picref, 0);
			if (ret < 0) {
				ret = 0;
				break;
			}

			is->frame_last_filter_delay = av_gettime() / 1000000.0 - is->frame_last_returned_time;
			if (fabs(is->frame_last_filter_delay) > AV_NOSYNC_THRESHOLD / 10.0)
				is->frame_last_filter_delay = 0;

			avfilter_copy_buf_props(frame, picref);

			pts_int = picref->pts;
			tb      = filt_out->inputs[0]->time_base;
			pos     = picref->pos;
			frame->opaque = picref;

			if (av_cmp_q(tb, is->video_st->time_base)) {
				av_unused int64_t pts1 = pts_int;
				pts_int = av_rescale_q(pts_int, tb, is->video_st->time_base);
				av_dlog(nullptr, "video_thread(): "
					"tb:%d/%d pts:%" PRId64" -> tb:%d/%d pts:%" PRId64"\n",
					tb.num, tb.den, pts1,
					is->video_st->time_base.num, is->video_st->time_base.den, pts_int);
			}
			pts = pts_int * av_q2d(is->video_st->time_base);
			ret = queue_picture(is, frame, pts, pos);
		}
#else
		pts = pts_int * av_q2d(is->video_st->time_base);
		//½âÂë³É¹¦ºóÓÃÓÚÏÔÊ¾£¬Ò²ÊÇ·Åµ½ÁíÒ»¸ö¶ÓÁÐÖÐ£¿
		ret = queue_picture(is, frame, pts, pkt.pos);
#endif

		if (ret < 0)
			goto the_end;

		if (is->step)
			stream_toggle_pause(is);
	}
the_end:
	avcodec_flush_buffers(is->video_st->codec);
#if CONFIG_AVFILTER
	avfilter_graph_free(&graph);
#endif
	av_free_packet(&pkt);
	avcodec_free_frame(&frame);
	return 0;
}

static int subtitle_thread(void *arg)
{
	VideoState *is = (VideoState *)arg;
	SubPicture *sp;
	AVPacket pkt1, *pkt = &pkt1;
	int got_subtitle;
	double pts;
	int i, j;
	int r, g, b, y, u, v, a;

	for (;;) {
		while (is->paused && !is->subtitleq.abort_request) {
			SDL_Delay(10);
		}
		if (packet_queue_get(&is->subtitleq, pkt, 1) < 0)
			break;

		if (pkt->data == flush_pkt.data) {
			avcodec_flush_buffers(is->subtitle_st->codec);
			continue;
		}
		SDL_LockMutex(is->subpq_mutex);
		while (is->subpq_size >= SUBPICTURE_QUEUE_SIZE &&
			!is->subtitleq.abort_request) {
				SDL_CondWait(is->subpq_cond, is->subpq_mutex);
		}
		SDL_UnlockMutex(is->subpq_mutex);

		if (is->subtitleq.abort_request)
			return 0;

		sp = &is->subpq[is->subpq_windex];

		/* NOTE: ipts is the PTS of the _first_ picture beginning in
		this packet, if any */
		pts = 0;
		if (pkt->pts != AV_NOPTS_VALUE)
			pts = av_q2d(is->subtitle_st->time_base) * pkt->pts;

		avcodec_decode_subtitle2(is->subtitle_st->codec, &sp->sub,
			&got_subtitle, pkt);
		if (got_subtitle && sp->sub.format == 0) {
			if (sp->sub.pts != AV_NOPTS_VALUE)
				pts = sp->sub.pts / (double)AV_TIME_BASE;
			sp->pts = pts;

			for (i = 0; i < sp->sub.num_rects; i++)
			{
				for (j = 0; j < sp->sub.rects[i]->nb_colors; j++)
				{
					RGBA_IN(r, g, b, a, (uint32_t*)sp->sub.rects[i]->pict.data[1] + j);
					y = RGB_TO_Y_CCIR(r, g, b);
					u = RGB_TO_U_CCIR(r, g, b, 0);
					v = RGB_TO_V_CCIR(r, g, b, 0);
					YUVA_OUT((uint32_t*)sp->sub.rects[i]->pict.data[1] + j, y, u, v, a);
				}
			}

			/* now we can update the picture count */
			if (++is->subpq_windex == SUBPICTURE_QUEUE_SIZE)
				is->subpq_windex = 0;
			SDL_LockMutex(is->subpq_mutex);
			is->subpq_size++;
			SDL_UnlockMutex(is->subpq_mutex);
		}
		av_free_packet(pkt);
	}
	return 0;
}
//²¥·ÅÒôÆµµÄÊ±ºòSDLµÄÏÔÊ¾
/* copy samples for viewing in editor window */
static void update_sample_display(VideoState *is, short *samples, int samples_size)
{
	int size, len;

	size = samples_size / sizeof(short);
	while (size > 0) {
		len = SAMPLE_ARRAY_SIZE - is->sample_array_index;
		if (len > size)
			len = size;
		memcpy(is->sample_array + is->sample_array_index, samples, len * sizeof(short));
		samples += len;
		is->sample_array_index += len;
		if (is->sample_array_index >= SAMPLE_ARRAY_SIZE)
			is->sample_array_index = 0;
		size -= len;
	}
}

/* return the wanted number of samples to get better sync if sync_type is video
* or external master clock */
static int synchronize_audio(VideoState *is, int nb_samples)
{
	int wanted_nb_samples = nb_samples;

	/* if not master, then we try to remove or add samples to correct the clock */
	if (((is->av_sync_type == AV_SYNC_VIDEO_MASTER && is->video_st) ||
		is->av_sync_type == AV_SYNC_EXTERNAL_CLOCK)) {
			double diff, avg_diff;
			int min_nb_samples, max_nb_samples;

			diff = get_audio_clock(is) - get_master_clock(is);

			if (diff < AV_NOSYNC_THRESHOLD) {
				is->audio_diff_cum = diff + is->audio_diff_avg_coef * is->audio_diff_cum;
				if (is->audio_diff_avg_count < AUDIO_DIFF_AVG_NB) {
					/* not enough measures to have a correct estimate */
					is->audio_diff_avg_count++;
				} else {
					/* estimate the A-V difference */
					avg_diff = is->audio_diff_cum * (1.0 - is->audio_diff_avg_coef);

					if (fabs(avg_diff) >= is->audio_diff_threshold) {
						wanted_nb_samples = nb_samples + (int)(diff * is->audio_src.freq);
						min_nb_samples = ((nb_samples * (100 - SAMPLE_CORRECTION_PERCENT_MAX) / 100));
						max_nb_samples = ((nb_samples * (100 + SAMPLE_CORRECTION_PERCENT_MAX) / 100));
						wanted_nb_samples = FFMIN(FFMAX(wanted_nb_samples, min_nb_samples), max_nb_samples);
					}
					av_dlog(nullptr, "diff=%f adiff=%f sample_diff=%d apts=%0.3f vpts=%0.3f %f\n",
						diff, avg_diff, wanted_nb_samples - nb_samples,
						is->audio_clock, is->video_clock, is->audio_diff_threshold);
				}
			} else {
				/* too big difference : may be initial PTS errors, so
				reset A-V filter */
				is->audio_diff_avg_count = 0;
				is->audio_diff_cum       = 0;
			}
	}

	return wanted_nb_samples;
}

/* decode one audio frame and returns its uncompressed size */
static int audio_decode_frame(VideoState *is, double *pts_ptr)
{
	AVPacket *pkt_temp = &is->audio_pkt_temp;
	AVPacket *pkt = &is->audio_pkt;
	AVCodecContext *dec = is->audio_st->codec;
	int len1, len2, data_size, resampled_data_size;
	int64_t dec_channel_layout;
	int got_frame;
	double pts;
	int new_packet = 0;
	int flush_complete = 0;
	int wanted_nb_samples;

	for (;;) {
		/* NOTE: the audio packet can contain several frames */
		while (pkt_temp->size > 0 || (!pkt_temp->data && new_packet)) {
			if (!is->frame) {
				if (!(is->frame = avcodec_alloc_frame()))
					return AVERROR(ENOMEM);
			} else
				avcodec_get_frame_defaults(is->frame);

			if (is->paused)
				return -1;

			if (flush_complete)
				break;
			new_packet = 0;
			len1 = avcodec_decode_audio4(dec, is->frame, &got_frame, pkt_temp);
			if (len1 < 0) {
				/* if error, we skip the frame */
				pkt_temp->size = 0;
				break;
			}
			
			//×¢Òâ£º´Ë´¦ÉèÖÃMFC²ÎÊý---
			ffmfc_param_aframe(is,is->frame,pkt_temp);
			//----------

			//------------------------
			pkt_temp->data += len1;
			pkt_temp->size -= len1;

			if (!got_frame) {
				/* stop sending empty packets if the decoder is finished */
				if (!pkt_temp->data && dec->codec->capabilities & CODEC_CAP_DELAY)
					flush_complete = 1;
				continue;
			}
			data_size = av_samples_get_buffer_size(nullptr, dec->channels,
				is->frame->nb_samples,
				dec->sample_fmt, 1);

			dec_channel_layout =
				(dec->channel_layout && dec->channels == av_get_channel_layout_nb_channels(dec->channel_layout)) ?
				dec->channel_layout : av_get_default_channel_layout(dec->channels);
			wanted_nb_samples = synchronize_audio(is, is->frame->nb_samples);

			if (dec->sample_fmt    != is->audio_src.fmt            ||
				dec_channel_layout != is->audio_src.channel_layout ||
				dec->sample_rate   != is->audio_src.freq           ||
				(wanted_nb_samples != is->frame->nb_samples && !is->swr_ctx)) {
					swr_free(&is->swr_ctx);
					is->swr_ctx = swr_alloc_set_opts(nullptr,
						is->audio_tgt.channel_layout, is->audio_tgt.fmt, is->audio_tgt.freq,
						dec_channel_layout,           dec->sample_fmt,   dec->sample_rate,
						0, nullptr);
					if (!is->swr_ctx || swr_init(is->swr_ctx) < 0) {
						fprintf(stderr, "Cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!\n",
							dec->sample_rate,   av_get_sample_fmt_name(dec->sample_fmt),   dec->channels,
							is->audio_tgt.freq, av_get_sample_fmt_name(is->audio_tgt.fmt), is->audio_tgt.channels);
						break;
					}
					is->audio_src.channel_layout = dec_channel_layout;
					is->audio_src.channels = dec->channels;
					is->audio_src.freq = dec->sample_rate;
					is->audio_src.fmt = dec->sample_fmt;
			}

			if (is->swr_ctx) {
				const uint8_t **in = (const uint8_t **)is->frame->extended_data;
				uint8_t *out[] = {is->audio_buf2};
				int out_count = sizeof(is->audio_buf2) / is->audio_tgt.channels / av_get_bytes_per_sample(is->audio_tgt.fmt);
				if (wanted_nb_samples != is->frame->nb_samples) {
					if (swr_set_compensation(is->swr_ctx, (wanted_nb_samples - is->frame->nb_samples) * is->audio_tgt.freq / dec->sample_rate,
						wanted_nb_samples * is->audio_tgt.freq / dec->sample_rate) < 0) {
							fprintf(stderr, "swr_set_compensation() failed\n");
							break;
					}
				}
				len2 = swr_convert(is->swr_ctx, out, out_count, in, is->frame->nb_samples);
				if (len2 < 0) {
					fprintf(stderr, "swr_convert() failed\n");
					break;
				}
				if (len2 == out_count) {
					fprintf(stderr, "warning: audio buffer is probably too small\n");
					swr_init(is->swr_ctx);
				}
				is->audio_buf = is->audio_buf2;
				resampled_data_size = len2 * is->audio_tgt.channels * av_get_bytes_per_sample(is->audio_tgt.fmt);
			} else {
				is->audio_buf = is->frame->data[0];
				resampled_data_size = data_size;
			}

			/* if no pts, then compute it */
			pts = is->audio_clock;
			*pts_ptr = pts;
			is->audio_clock += (double)data_size /
				(dec->channels * dec->sample_rate * av_get_bytes_per_sample(dec->sample_fmt));
#ifdef DEBUG
			{
				static double last_clock;
				printf("audio: delay=%0.3f clock=%0.3f pts=%0.3f\n",
					is->audio_clock - last_clock,
					is->audio_clock, pts);
				last_clock = is->audio_clock;
			}
#endif
			return resampled_data_size;
		}

		/* free the current packet */
		if (pkt->data)
			av_free_packet(pkt);
		memset(pkt_temp, 0, sizeof(*pkt_temp));

		if (is->paused || is->audioq.abort_request) {
			return -1;
		}

		if (is->audioq.nb_packets == 0)
			SDL_CondSignal(is->continue_read_thread);

		/* read next packet */
		if ((new_packet = packet_queue_get(&is->audioq, pkt, 1)) < 0)
			return -1;

		if (pkt->data == flush_pkt.data) {
			avcodec_flush_buffers(dec);
			flush_complete = 0;
		}

		*pkt_temp = *pkt;

		/* if update the audio clock with the pts */
		if (pkt->pts != AV_NOPTS_VALUE) {
			is->audio_clock = av_q2d(is->audio_st->time_base)*pkt->pts;
		}
	}
}

/* prepare a new audio buffer */
static void sdl_audio_callback(void *opaque, Uint8 *stream, int len)
{
	VideoState *is = (VideoState *)opaque;
	int audio_size, len1;
	int bytes_per_sec;
	int frame_size = av_samples_get_buffer_size(nullptr, is->audio_tgt.channels, 1, is->audio_tgt.fmt, 1);
	double pts;

	audio_callback_time = av_gettime();

	while (len > 0) {
		if (is->audio_buf_index >= is->audio_buf_size) {
			audio_size = audio_decode_frame(is, &pts);
			if (audio_size < 0) {
				/* if error, just output silence */
				is->audio_buf      = is->silence_buf;
				is->audio_buf_size = sizeof(is->silence_buf) / frame_size * frame_size;
			} else {
				if (is->show_mode != SHOW_MODE_VIDEO)
					update_sample_display(is, (int16_t *)is->audio_buf, audio_size);
				is->audio_buf_size = audio_size;
			}
			is->audio_buf_index = 0;
		}
		len1 = is->audio_buf_size - is->audio_buf_index;
		if (len1 > len)
			len1 = len;
		//memcpy(stream, (uint8_t *)is->audio_buf + is->audio_buf_index, len1);
        SDL_memset(stream, 0, len1);
		SDL_MixAudio(stream, (uint8_t *)is->audio_buf + is->audio_buf_index, len1, volume);

		len -= len1;
		stream += len1;
		is->audio_buf_index += len1;
	}
	bytes_per_sec = is->audio_tgt.freq * is->audio_tgt.channels * av_get_bytes_per_sample(is->audio_tgt.fmt);
	is->audio_write_buf_size = is->audio_buf_size - is->audio_buf_index;
	/* Let's assume the audio driver that is used by SDL has two periods. */
	is->audio_current_pts = is->audio_clock - (double)(2 * is->audio_hw_buf_size + is->audio_write_buf_size) / bytes_per_sec;
	is->audio_current_pts_drift = is->audio_current_pts - audio_callback_time / 1000000.0;
}

static int audio_open(void *opaque, int64_t wanted_channel_layout, int wanted_nb_channels, int wanted_sample_rate, struct AudioParams *audio_hw_params)
{
	SDL_AudioSpec wanted_spec, spec;
	const char *env;
	const int next_nb_channels[] = {0, 0, 1, 6, 2, 6, 4, 6};

	env = SDL_getenv("SDL_AUDIO_CHANNELS");
	if (env) {
		wanted_nb_channels = atoi(env);
		wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
	}
	if (!wanted_channel_layout || wanted_nb_channels != av_get_channel_layout_nb_channels(wanted_channel_layout)) {
		wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
		wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
	}
	wanted_spec.channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
	wanted_spec.freq = wanted_sample_rate;
	if (wanted_spec.freq <= 0 || wanted_spec.channels <= 0) {
		fprintf(stderr, "Invalid sample rate or channel count!\n");
		return -1;
	}
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.silence = 0;
	wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
	wanted_spec.callback = sdl_audio_callback;
	wanted_spec.userdata = opaque;
	while (SDL_OpenAudio(&wanted_spec, &spec) < 0) {
		fprintf(stderr, "SDL_OpenAudio (%d channels): %s\n", wanted_spec.channels, SDL_GetError());
		wanted_spec.channels = next_nb_channels[FFMIN(7, wanted_spec.channels)];
		if (!wanted_spec.channels) {
			fprintf(stderr, "No more channel combinations to try, audio open failed\n");
			return -1;
		}
		wanted_channel_layout = av_get_default_channel_layout(wanted_spec.channels);
	}
	if (spec.format != AUDIO_S16SYS) {
		fprintf(stderr, "SDL advised audio format %d is not supported!\n", spec.format);
		return -1;
	}
	if (spec.channels != wanted_spec.channels) {
		wanted_channel_layout = av_get_default_channel_layout(spec.channels);
		if (!wanted_channel_layout) {
			fprintf(stderr, "SDL advised channel count %d is not supported!\n", spec.channels);
			return -1;
		}
	}

	audio_hw_params->fmt = AV_SAMPLE_FMT_S16;
	audio_hw_params->freq = spec.freq;
	audio_hw_params->channel_layout = wanted_channel_layout;
	audio_hw_params->channels =  spec.channels;
	return spec.size;
}

/* open a given stream. Return 0 if OK */
//´ò¿ªÒ»¸öStream£¬ÊÓÆµ»òÒôÆµ
static int stream_component_open(VideoState *is, int stream_index)
{
	AVFormatContext *ic = is->ic;
	AVCodecContext *avctx;
	AVCodec *codec;
	AVDictionary *opts;
	AVDictionaryEntry *t = nullptr;

	if (stream_index < 0 || stream_index >= ic->nb_streams)
		return -1;
	avctx = ic->streams[stream_index]->codec;
	// ÎªÊÓÆµÁ÷Ñ°ÕÒ½âÂëÆ÷
	//×¢Òâ£º´Ë´¦¿ÉÒÔÖ¸¶¨½âÂëÆ÷
	codec = avcodec_find_decoder(avctx->codec_id);
	opts = filter_codec_opts(codec_opts, avctx->codec_id, ic, ic->streams[stream_index], codec);

	switch(avctx->codec_type){
	case AVMEDIA_TYPE_AUDIO   : is->last_audio_stream    = stream_index; if(audio_codec_name   ) codec= avcodec_find_decoder_by_name(   audio_codec_name); break;
	case AVMEDIA_TYPE_SUBTITLE: is->last_subtitle_stream = stream_index; if(subtitle_codec_name) codec= avcodec_find_decoder_by_name(subtitle_codec_name); break;
	case AVMEDIA_TYPE_VIDEO   : is->last_video_stream    = stream_index; if(video_codec_name   ) codec= avcodec_find_decoder_by_name(   video_codec_name); break;
	}
	if (!codec)
		return -1;

	avctx->workaround_bugs   = workaround_bugs;
	avctx->lowres            = lowres;
	if(avctx->lowres > codec->max_lowres){
		av_log(avctx, AV_LOG_WARNING, "The maximum value for lowres supported by the decoder is %d\n",
			codec->max_lowres);
		avctx->lowres= codec->max_lowres;
	}
	avctx->idct_algo         = idct;
	avctx->skip_frame        = skip_frame;
	avctx->skip_idct         = skip_idct;
	avctx->skip_loop_filter  = skip_loop_filter;
	avctx->error_concealment = error_concealment;

	if(avctx->lowres) avctx->flags |= CODEC_FLAG_EMU_EDGE;
	if (fast)   avctx->flags2 |= CODEC_FLAG2_FAST;
	if(codec->capabilities & CODEC_CAP_DR1)
		avctx->flags |= CODEC_FLAG_EMU_EDGE;
	//MetaData?
	if (!av_dict_get(opts, "threads", nullptr, 0))
		av_dict_set(&opts, "threads", "auto", 0);
	// ´ò¿ª½âÂëÆ÷£¬¶þÕßÖ®¼ä½¨Á¢ÁªÏµ
	if (!codec ||
		avcodec_open2(avctx, codec, &opts) < 0)
		return -1;
	if ((t = av_dict_get(opts, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
		av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
		return AVERROR_OPTION_NOT_FOUND;
	}

	/* prepare audio output */
	//×¼±¸SDLÒôÆµÊä³ö
	if (avctx->codec_type == AVMEDIA_TYPE_AUDIO) {
		int audio_hw_buf_size = audio_open(is, avctx->channel_layout, avctx->channels, avctx->sample_rate, &is->audio_src);
		if (audio_hw_buf_size < 0)
			return -1;
		is->audio_hw_buf_size = audio_hw_buf_size;
		is->audio_tgt = is->audio_src;
	}

	ic->streams[stream_index]->discard = AVDISCARD_DEFAULT;
	switch (avctx->codec_type) {
		//ÉèÖÃis½á¹¹Ìå
	case AVMEDIA_TYPE_AUDIO:
		is->audio_stream = stream_index;
		is->audio_st = ic->streams[stream_index];
		is->audio_buf_size  = 0;
		is->audio_buf_index = 0;

		/* init averaging filter */
		is->audio_diff_avg_coef  = exp(log(0.01) / AUDIO_DIFF_AVG_NB);
		is->audio_diff_avg_count = 0;
		/* since we do not have a precise anough audio fifo fullness,
		we correct audio sync only if larger than this threshold */
		is->audio_diff_threshold = 2.0 * is->audio_hw_buf_size / av_samples_get_buffer_size(nullptr, is->audio_tgt.channels, is->audio_tgt.freq, is->audio_tgt.fmt, 1);

		memset(&is->audio_pkt, 0, sizeof(is->audio_pkt));
		memset(&is->audio_pkt_temp, 0, sizeof(is->audio_pkt_temp));
		//³õÊ¼»¯Packet¶ÓÁÐ
		packet_queue_start(&is->audioq);
		//¿ª²¥
		SDL_PauseAudio(0);
		break;
	case AVMEDIA_TYPE_VIDEO:
		is->video_stream = stream_index;
		is->video_st = ic->streams[stream_index];

		packet_queue_start(&is->videoq);
		//ÊÓÆµÏß³Ì
#ifdef SDL2
        is->video_tid = SDL_CreateThread(video_thread, "Video Thread", is);
#else
		is->video_tid = SDL_CreateThread(video_thread, is);
#endif
		break;
	case AVMEDIA_TYPE_SUBTITLE:
		is->subtitle_stream = stream_index;
		is->subtitle_st = ic->streams[stream_index];
		packet_queue_start(&is->subtitleq);
#ifdef SDL2
            is->subtitle_tid = SDL_CreateThread(subtitle_thread, "Subtitle Thread",is);
#else
            is->subtitle_tid = SDL_CreateThread(subtitle_thread, is);
#endif
		
		break;
	default:
		break;
	}
	return 0;
}

static void stream_component_close(VideoState *is, int stream_index)
{
	AVFormatContext *ic = is->ic;
	AVCodecContext *avctx;

	if (stream_index < 0 || stream_index >= ic->nb_streams)
		return;
	avctx = ic->streams[stream_index]->codec;

	switch (avctx->codec_type) {
	case AVMEDIA_TYPE_AUDIO:
		packet_queue_abort(&is->audioq);

		SDL_CloseAudio();

		packet_queue_flush(&is->audioq);
		av_free_packet(&is->audio_pkt);
		swr_free(&is->swr_ctx);
		av_freep(&is->audio_buf1);
		is->audio_buf = nullptr;
		avcodec_free_frame(&is->frame);

		if (is->rdft) {
			av_rdft_end(is->rdft);
			av_freep(&is->rdft_data);
			is->rdft = nullptr;
			is->rdft_bits = 0;
		}
		break;
	case AVMEDIA_TYPE_VIDEO:
		packet_queue_abort(&is->videoq);

		/* note: we also signal this mutex to make sure we deblock the
		video thread in all cases */
		SDL_LockMutex(is->pictq_mutex);
		SDL_CondSignal(is->pictq_cond);
		SDL_UnlockMutex(is->pictq_mutex);

		SDL_WaitThread(is->video_tid, nullptr);

		packet_queue_flush(&is->videoq);
		break;
	case AVMEDIA_TYPE_SUBTITLE:
		packet_queue_abort(&is->subtitleq);

		/* note: we also signal this mutex to make sure we deblock the
		video thread in all cases */
		SDL_LockMutex(is->subpq_mutex);
		is->subtitle_stream_changed = 1;

		SDL_CondSignal(is->subpq_cond);
		SDL_UnlockMutex(is->subpq_mutex);

		SDL_WaitThread(is->subtitle_tid, nullptr);

		packet_queue_flush(&is->subtitleq);
		break;
	default:
		break;
	}

	ic->streams[stream_index]->discard = AVDISCARD_ALL;
	avcodec_close(avctx);
#if CONFIG_AVFILTER
	free_buffer_pool(&is->buffer_pool);
#endif
	switch (avctx->codec_type) {
	case AVMEDIA_TYPE_AUDIO:
		is->audio_st = nullptr;
		is->audio_stream = -1;
		break;
	case AVMEDIA_TYPE_VIDEO:
		is->video_st = nullptr;
		is->video_stream = -1;
		break;
	case AVMEDIA_TYPE_SUBTITLE:
		is->subtitle_st = nullptr;
		is->subtitle_stream = -1;
		break;
	default:
		break;
	}
}

static int decode_interrupt_cb(void *ctx)
{
	VideoState *is = (VideoState *)ctx;
	return is->abort_request;
}

static int is_realtime(AVFormatContext *s)
{
	if(   !strcmp(s->iformat->name, "rtp")
		|| !strcmp(s->iformat->name, "rtsp")
		|| !strcmp(s->iformat->name, "sdp")
		)
		return 1;

	if(s->pb && (   !strncmp(s->filename, "rtp:", 4)
		|| !strncmp(s->filename, "udp:", 4)
		)
		)
		return 1;
	return 0;
}

/* this thread gets the stream from the disk or the network */
//½âÂëÏß³Ì£¬»ñµÃÊÓÒôÆµPacket²¢·ÅÈë¶ÓÁÐ
static int read_thread(void *arg)
{
	VideoState *is = (VideoState *)arg;
	AVFormatContext *ic = nullptr;
	int err, i, ret;
	int st_index[AVMEDIA_TYPE_NB];
	AVPacket pkt1, *pkt = &pkt1;
	int eof = 0;
	int pkt_in_play_range = 0;
	AVDictionaryEntry *t;
	AVDictionary **opts;
	int orig_nb_streams;
	SDL_mutex *wait_mutex = SDL_CreateMutex();

	memset(st_index, -1, sizeof(st_index));
	is->last_video_stream = is->video_stream = -1;
	is->last_audio_stream = is->audio_stream = -1;
	is->last_subtitle_stream = is->subtitle_stream = -1;

	ic = avformat_alloc_context();
	ic->interrupt_callback.callback = decode_interrupt_cb;
	ic->interrupt_callback.opaque = is;
	// ´ò¿ªÊÓÆµÎÄ¼þ£¨Ã»ÓÐ´ò¿ª±à½âÂëÆ÷£©£¬³õÊ¼»¯AVFormatContext
	// ÐÂ°æÖÐÒÑ¾­½«Æä·ÏÆú
	//if(av_open_input_file(&pFormatCtx, filename, nullptr, 0, nullptr)!=0)
	//{
	//	printf("Couldn't open file.\n");
	//	return -1;
	//}
	//¸ÄÎªavformat_open_input()
	//--------------------------
	//if(avformat_open_input(&pFormatCtx,filename,nullptr,nullptr)!=0){
	//char rtspurl[100]="rtsp://169.254.197.35:8554/sh1943.mpg";
	//ÎªÁËÊ¹FFMPEG¿ÉÒÔÖ±½Ó´ÓÄÚ´æÖÐ¶ÁÈ¡Êý¾Ý
	//×¥°üµÄÊ±ºòÖ±½Ó´ÓÄÚ´æÖÐ¶ÁÈ¡Êý¾Ý

	err = avformat_open_input(&ic, is->filename, is->iformat, &format_opts);

	//if(avformat_open_input(&pFormatCtx,"sprink_12M.ts",nullptr,nullptr)!=0){
	////AfxMessageBox()ÊÇÈ«¾ÖµÄº¯Êý£¬¼Ç×¡ÔÚVCÀïÓÐAFX¿ª¹ØµÄ¶¼ÊÇÈ«¾Öº¯Êý¡£
	//¶øMessageBox()ÊÇCWndµÄ³ÉÔ±º¯Êý£¬Ò²¾ÍÊÇËµÔÚÄÄ¶¼¿ÉÒÔµ÷ÓÃAfxMessageBox£¨£©£¬
	//µ«ÊÇMessageBox()Ö»ÄÜÔÚ»ùÀàÊÇCWndµÄÀàÀïÃæÖ±½Óµ÷ÓÃ¡£ 
	//printf("ÊäÈë¸ñÊ½:%s\n",ic->iformat->name);
	//printf("IO»º´æ´óÐ¡:%d\n",ic->pb.buffer_size);
	if (err < 0) {
		print_error(is->filename, err);
		ret = -1;
		goto fail;
	}
	if ((t = av_dict_get(format_opts, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
		av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
		ret = AVERROR_OPTION_NOT_FOUND;
		goto fail;
	}
	is->ic = ic;

	if (genpts)
		ic->flags |= AVFMT_FLAG_GENPTS;

	opts = setup_find_stream_info_opts(ic, codec_opts);
	orig_nb_streams = ic->nb_streams;
	// »ñÈ¡Á÷ÐÅÏ¢£¬¼ÓÈëAVFormatContextÖÐ
	//This is useful for file formats with no headers such as MPEG. 
	err = avformat_find_stream_info(ic, opts);
	if (err < 0) {
		fprintf(stderr, "%s: could not find codec parameters\n", is->filename);
		ret = -1;
		goto fail;
	}
	for (i = 0; i < orig_nb_streams; i++)
		av_dict_free(&opts[i]);
	av_freep(&opts);

	if (ic->pb)
		ic->pb->eof_reached = 0; // FIXME hack, ffplay maybe should not use url_feof() to test for the end

	if (seek_by_bytes < 0)
		seek_by_bytes = !!(ic->iformat->flags & AVFMT_TS_DISCONT);

	/* if seeking requested, we execute it */
	if (start_time != AV_NOPTS_VALUE) {
		int64_t timestamp;

		timestamp = start_time;
		/* add the stream start time */
		if (ic->start_time != AV_NOPTS_VALUE)
			timestamp += ic->start_time;
		ret = avformat_seek_file(ic, -1, INT64_MIN, timestamp, INT64_MAX, 0);
		if (ret < 0) {
			fprintf(stderr, "%s: could not seek to position %0.3f\n",
				is->filename, (double)timestamp / AV_TIME_BASE);
		}
	}
	// »ñÈ¡µÚ¸öÊÓÆµÁ÷£¬ÒôÆµÁ÷Ë÷Òý
	for (i = 0; i < ic->nb_streams; i++)
		ic->streams[i]->discard = AVDISCARD_ALL;
	float zhenlv;
	if (!video_disable)
		st_index[AVMEDIA_TYPE_VIDEO] =
		av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO,
		wanted_stream[AVMEDIA_TYPE_VIDEO], -1, nullptr, 0);
	//int video_index = st_index[AVMEDIA_TYPE_VIDEO];
	//printf("ÊÓÆµ±àÂëÆ÷Ãû³Æ£º%s\n",ic->streams[video_index]->codec->codec_name);
	//printf("ÊÓÆµ¿í£º%d\n",ic->streams[video_index]->codec->width);
	//printf("ÊÓÆµ¸ß£º%d\n",ic->streams[video_index]->codec->height);
	//zhenlv=(ic->streams[video_index]->codec->time_base.den)/(ic->streams[video_index]->codec->time_base.num);
	//printf("ÊÓÆµÖ¡ÂÊ£º%f\n",zhenlv);
	if (!audio_disable)
		st_index[AVMEDIA_TYPE_AUDIO] =
		av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO,
		wanted_stream[AVMEDIA_TYPE_AUDIO],
		st_index[AVMEDIA_TYPE_VIDEO],
		nullptr, 0);
	//int audio_index = st_index[AVMEDIA_TYPE_AUDIO];
	//printf("ÒôÆµ±àÂëÆ÷Ãû³Æ£º%s\n",ic->streams[audio_index]->codec->codec_name);
	//printf("²ÉÑùÂÊ£º%d\n",ic->streams[audio_index]->codec->sample_rate);
	//printf("Ö¡´óÐ¡£º%d\n",ic->streams[audio_index]->codec->frame_size);
	//printf("ÉùµÀÊý£º%d\n",ic->streams[audio_index]->codec->channels);
	if (!video_disable)
		st_index[AVMEDIA_TYPE_SUBTITLE] =
		av_find_best_stream(ic, AVMEDIA_TYPE_SUBTITLE,
		wanted_stream[AVMEDIA_TYPE_SUBTITLE],
		(st_index[AVMEDIA_TYPE_AUDIO] >= 0 ?
		st_index[AVMEDIA_TYPE_AUDIO] :
	st_index[AVMEDIA_TYPE_VIDEO]),
		nullptr, 0);
	if (show_status) {
		av_dump_format(ic, 0, is->filename, 0);
	}

	is->show_mode = (VideoState::ShowMode)show_mode;

	/* open the streams */
	//´ò¿ªStream£¬ÊÓÆµ»òÒôÆµ
	if (st_index[AVMEDIA_TYPE_AUDIO] >= 0) {
		//´ò¿ª
		stream_component_open(is, st_index[AVMEDIA_TYPE_AUDIO]);
	}

	ret = -1;
	if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
		ret = stream_component_open(is, st_index[AVMEDIA_TYPE_VIDEO]);
	}
#ifdef SDL2
    is->refresh_tid = SDL_CreateThread(refresh_thread, "refresh thread", is);
#else
	is->refresh_tid = SDL_CreateThread(refresh_thread, is);
#endif

	if (is->show_mode == SHOW_MODE_NONE)
		is->show_mode = ret >= 0 ? (VideoState::ShowMode)SHOW_MODE_VIDEO : (VideoState::ShowMode)SHOW_MODE_RDFT;

	if (st_index[AVMEDIA_TYPE_SUBTITLE] >= 0) {
		stream_component_open(is, st_index[AVMEDIA_TYPE_SUBTITLE]);
	}

	if (is->video_stream < 0 && is->audio_stream < 0) {
		//AfxMessageBox(_T("could not open codecs"));
		ret = -1;
		goto fail;
	}

	if (infinite_buffer < 0 && is_realtime(ic))
		infinite_buffer = 1;
	//»ñÈ¡Ò»Ð©ÏµÍ³ÐÅÏ¢

	//×¢Òâ£ºÔÚ´Ë´¦ÉèÖÃMFC²ÎÊý
	ffmfc_param_global(is);
	

	for (;;) {
		if (is->abort_request)
			break;

		if (is->paused != is->last_paused) {
			is->last_paused = is->paused;
			if (is->paused)
				is->read_pause_return = av_read_pause(ic);
			else
				av_read_play(ic);
		}
#if CONFIG_RTSP_DEMUXER || CONFIG_MMSH_PROTOCOL
		if (is->paused &&
			(!strcmp(ic->iformat->name, "rtsp") ||
			(ic->pb && !strncmp(input_filename.c_str(), "mmsh:", 5)))) {
			/* wait 10 ms to avoid trying to get another packet */
			/* XXX: horrible */
			SDL_Delay(10);
			continue;
		}
#endif


		//µ±µ÷Õû²¥·Å½ø¶ÈÒÔºó
		if (is->seek_req) {
			int64_t seek_target = is->seek_pos;
			int64_t seek_min    = is->seek_rel > 0 ? seek_target - is->seek_rel + 2: INT64_MIN;
			int64_t seek_max    = is->seek_rel < 0 ? seek_target - is->seek_rel - 2: INT64_MAX;
			// FIXME the +-2 is due to rounding being not done in the correct direction in generation
			//      of the seek_pos/seek_rel variables

			ret = avformat_seek_file(is->ic, -1, seek_min, seek_target, seek_max, is->seek_flags);
			if (ret < 0) {
				fprintf(stderr, "%s: error while seeking\n", is->ic->filename);
			} else {
				if (is->audio_stream >= 0) {
					//Çå¿Õµ±Ç°µÄPAcket¶ÓÁÐ
					packet_queue_flush(&is->audioq);
					packet_queue_put(&is->audioq, &flush_pkt);
				}
				if (is->subtitle_stream >= 0) {
					packet_queue_flush(&is->subtitleq);
					packet_queue_put(&is->subtitleq, &flush_pkt);
				}
				if (is->video_stream >= 0) {
					packet_queue_flush(&is->videoq);
					packet_queue_put(&is->videoq, &flush_pkt);
				}
			}
			is->seek_req = 0;
			eof = 0;
		}
		if (is->que_attachments_req) {
			avformat_queue_attached_pictures(ic);
			is->que_attachments_req = 0;
		}

		/* if the queue are full, no need to read more */
		//Èç¹ûPacket¶ÓÁÐÂúÁË£¬ÔòµÈ´ý
		if (infinite_buffer<1 &&
			(is->audioq.size + is->videoq.size + is->subtitleq.size > MAX_QUEUE_SIZE
			|| (   (is->audioq   .nb_packets > MIN_FRAMES || is->audio_stream < 0 || is->audioq.abort_request)
			&& (is->videoq   .nb_packets > MIN_FRAMES || is->video_stream < 0 || is->videoq.abort_request)
			&& (is->subtitleq.nb_packets > MIN_FRAMES || is->subtitle_stream < 0 || is->subtitleq.abort_request)))) {
				/* wait 10 ms */
				SDL_LockMutex(wait_mutex);
				SDL_CondWaitTimeout(is->continue_read_thread, wait_mutex, 10);
				SDL_UnlockMutex(wait_mutex);
				continue;
		}
		if (eof) {
			if (is->video_stream >= 0) {
				av_init_packet(pkt);
				pkt->data = nullptr;
				pkt->size = 0;
				pkt->stream_index = is->video_stream;
				packet_queue_put(&is->videoq, pkt);
			}
			if (is->audio_stream >= 0 &&
				is->audio_st->codec->codec->capabilities & CODEC_CAP_DELAY) {
					av_init_packet(pkt);
					pkt->data = nullptr;
					pkt->size = 0;
					pkt->stream_index = is->audio_stream;
					packet_queue_put(&is->audioq, pkt);
			}
			SDL_Delay(10);
			if (is->audioq.size + is->videoq.size + is->subtitleq.size == 0) {
				if (loop != 1 && (!loop || --loop)) {
					stream_seek(is, start_time != AV_NOPTS_VALUE ? start_time : 0, 0, 0);
				} else if (autoexit) {
					ret = AVERROR_EOF;
					dlg->OnBnClickedStop();
					goto fail;
				}
			}
			eof=0;
			continue;
		}

		//¶ÁÈ¡Ò»¸öPacket
		ret = av_read_frame(ic, pkt);
		//printf("Packet dts£º%d\n",pkt->dts);
		//printf("Packet pts£º%d\n",pkt->pts);
		//printf("Packet Size£º%d\n",pkt->size);
		//printf("Packet dts£º%d\n",pkt->dts);
		//´Ë´¦ÉèÖÃÍ¼±í²ÎÊý

		ffmfc_param_packet(is,pkt);
		//--------------------
		if (ret < 0) {
			if (ret == AVERROR_EOF || url_feof(ic->pb))
				eof = 1;
			if (ic->pb && ic->pb->error)
				break;
			SDL_LockMutex(wait_mutex);
			SDL_CondWaitTimeout(is->continue_read_thread, wait_mutex, 10);
			SDL_UnlockMutex(wait_mutex);
			continue;
		}
		/* check if packet is in play range specified by user, then queue, otherwise discard */
		pkt_in_play_range = duration == AV_NOPTS_VALUE ||
			(pkt->pts - ic->streams[pkt->stream_index]->start_time) *
			av_q2d(ic->streams[pkt->stream_index]->time_base) -
			(double)(start_time != AV_NOPTS_VALUE ? start_time : 0) / 1000000
			<= ((double)duration / 1000000);
		if (pkt->stream_index == is->audio_stream && pkt_in_play_range) {
			packet_queue_put(&is->audioq, pkt);
			//printf("ÒôÆµPacket£¬¼ÓÈëÒôÆµ¶ÓÁÐ\n");
			//---------------------------------
		} else if (pkt->stream_index == is->video_stream && pkt_in_play_range) {
			packet_queue_put(&is->videoq, pkt);
			//printf("ÊÓÆµPacket£¬¼ÓÈëÊÓÆµ¶ÓÁÐ\n");
		} else if (pkt->stream_index == is->subtitle_stream && pkt_in_play_range) {
			packet_queue_put(&is->subtitleq, pkt);
		} else {
			av_free_packet(pkt);
		}
	}
	/* wait until the end */
	while (!is->abort_request) {
		SDL_Delay(100);
	}

	ret = 0;
fail:
	/* close each stream */
	if (is->audio_stream >= 0)
		stream_component_close(is, is->audio_stream);
	if (is->video_stream >= 0)
		stream_component_close(is, is->video_stream);
	if (is->subtitle_stream >= 0)
		stream_component_close(is, is->subtitle_stream);
	if (is->ic) {
		avformat_close_input(&is->ic);
	}

	if (ret != 0) {
		SDL_Event event;

		event.type = FF_QUIT_EVENT;
		event.user.data1 = is;
		SDL_PushEvent(&event);
	}
	SDL_DestroyMutex(wait_mutex);
	return 0;
}
//ÉèÖÃ¸÷ÖÖSDLÐÅºÅ£¬¿ªÊ¼½âÂëÏß³Ì
static VideoState *stream_open(const char *filename, AVInputFormat *iformat)
{
	VideoState *is;

	is = (VideoState *)av_mallocz(sizeof(VideoState));
	if (!is)
		return nullptr;
	av_strlcpy(is->filename, filename, sizeof(is->filename));
	is->iformat = iformat;
	is->ytop    = 0;
	is->xleft   = 0;

	/* start video display */
	//³õÊ¼»¯¸÷ÖÖ±äÁ¿
	is->pictq_mutex = SDL_CreateMutex();
	is->pictq_cond  = SDL_CreateCond();

	is->subpq_mutex = SDL_CreateMutex();
	is->subpq_cond  = SDL_CreateCond();
	//³õÊ¼»¯Packet¶ÓÁÐ
	packet_queue_init(&is->videoq);
	packet_queue_init(&is->audioq);
	packet_queue_init(&is->subtitleq);

	is->continue_read_thread = SDL_CreateCond();

	is->av_sync_type = av_sync_type;
	//½âÂëÏß³Ì
    
#ifdef SDL2
   is->read_tid     = SDL_CreateThread(read_thread,"Read Thread", is);
#else
    is->read_tid     = SDL_CreateThread(read_thread, is);
#endif
    
	
	if (!is->read_tid) {
		av_free(is);
		return nullptr;
	}
	return is;
}
//ÒÔÏÂ¼¸¸öº¯Êý¶¼ÊÇ´¦Àíevent_loop()ÖÐµÄ¸÷ÖÖ²Ù×÷µÄ
static void stream_cycle_channel(VideoState *is, int codec_type)
{
	AVFormatContext *ic = is->ic;
	int start_index, stream_index;
	int old_index;
	AVStream *st;

	if (codec_type == AVMEDIA_TYPE_VIDEO) {
		start_index = is->last_video_stream;
		old_index = is->video_stream;
	} else if (codec_type == AVMEDIA_TYPE_AUDIO) {
		start_index = is->last_audio_stream;
		old_index = is->audio_stream;
	} else {
		start_index = is->last_subtitle_stream;
		old_index = is->subtitle_stream;
	}
	stream_index = start_index;
	for (;;) {
		if (++stream_index >= is->ic->nb_streams)
		{
			if (codec_type == AVMEDIA_TYPE_SUBTITLE)
			{
				stream_index = -1;
				is->last_subtitle_stream = -1;
				goto the_end;
			}
			if (start_index == -1)
				return;
			stream_index = 0;
		}
		if (stream_index == start_index)
			return;
		st = ic->streams[stream_index];
		if (st->codec->codec_type == codec_type) {
			/* check that parameters are OK */
			switch (codec_type) {
			case AVMEDIA_TYPE_AUDIO:
				if (st->codec->sample_rate != 0 &&
					st->codec->channels != 0)
					goto the_end;
				break;
			case AVMEDIA_TYPE_VIDEO:
			case AVMEDIA_TYPE_SUBTITLE:
				goto the_end;
			default:
				break;
			}
		}
	}
the_end:
	stream_component_close(is, old_index);
	stream_component_open(is, stream_index);
	if (codec_type == AVMEDIA_TYPE_VIDEO)
		is->que_attachments_req = 1;
}
//¾²Ì¬º¯ÊýÖ»ÄÜÔÚÉùÃ÷ËüµÄÎÄ¼þµ±ÖÐ¿É¼û£¬²»ÄÜ±»ÆäËûÎÄ¼þËùµ÷ÓÃ£¬Ò²¾ÍÊÇËµ¸Ã¾²Ì¬º¯ÊýÖ»
//ÄÜÔÚÆä¶¨ÒåµÄ.cpp»ò.cÖÐµ÷ÓÃ£¬ÔÚÆäËü.cpp»ò.cÎÄ¼þµÄº¯ÊýÀïÊÇ²»ÄÜ±»µ÷ÓÃµÄ¡£
//ÏëÒªÔÚMFCÖÐµ÷ÓÃ£¬¾Í±ØÐëÓÃÒ»¸öÆÕÍ¨º¯Êý¶ÔËû½øÐÐ·â×°

static void toggle_full_screen(VideoState *is)
{
#if defined(__APPLE__) && SDL_VERSION_ATLEAST(1, 2, 14)
	/* OS X needs to reallocate the SDL overlays */
	int i;
	for (i = 0; i < VIDEO_PICTURE_QUEUE_SIZE; i++)
		is->pictq[i].reallocate = 1;
#endif
	is_full_screen = !is_full_screen;
	video_open(is, 1);
}
//·¢ËÍ¡°È«ÆÁ¡±ÃüÁî
//Send Command "FullScreen"
void ffmfc_play_fullcreen(){
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	event.key.keysym.sym=SDLK_f;
	SDL_PushEvent(&event);
}

static void toggle_pause(VideoState *is)
{
	stream_toggle_pause(is);
	is->step = 0;
}


//·¢ËÍ¡°ÔÝÍ£¡±ÃüÁî
//Send Command "Pause"
void ffmfc_play_pause()
{
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	event.key.keysym.sym=SDLK_p;
	SDL_PushEvent(&event);
}

static void step_to_next_frame(VideoState *is)
{
	/* if the stream is paused unpause it, then step */
	if (is->paused)
		stream_toggle_pause(is);
	is->step = 1;
}
//·¢ËÍ¡°ÖðÖ¡¡±ÃüÁî
//Send Command "Step"
void ffmfc_seek_step(){
	SDL_Event event;
	event.type = SDL_KEYDOWN;
	event.key.keysym.sym=SDLK_s;
	SDL_PushEvent(&event);
}
//·¢ËÍ¡°¿í¸ß±È¡±ÃüÁî
//Send Command "AspectRatio"
void ffmfc_aspectratio(int num,int den){
	int w=g_is->width;
	int h=g_is->height;
	int w_re=h*num/den;
#ifdef SDL2
    SDL_Event event;
    event.type = SDL_WINDOWEVENT;
    event.window.event = SDL_WINDOWEVENT_RESIZED;
    event.window.data1=w_re;
    event.window.data2=h;
#else
	SDL_Event event;
	event.type = SDL_VIDEORESIZE;
	event.resize.w=w_re;
	event.resize.h=h;
#endif
	SDL_PushEvent(&event);
}
//·¢ËÍ¡°´óÐ¡¡±ÃüÁî
//Send Command "WindowSize"
void ffmfc_size(int percentage){
	int w=g_is->ic->streams[g_is->video_stream]->codec->width;
	int h=g_is->ic->streams[g_is->video_stream]->codec->height;
    
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
void ffmfc_audio_display(int mode){

	SDL_Event event;
	event.type = SDL_KEYDOWN;
	switch(mode){
	case 0:event.key.keysym.sym=SDLK_w;break;
	case 1:event.key.keysym.sym=SDLK_e;break;
	case 2:event.key.keysym.sym=SDLK_r;break;
	}
	SDL_PushEvent(&event);
}

void ffmfc_VolumeUp()
{
	if (percent < 100)
		percent += 10;

	SDL_Event event;
	event.type = FFMFC_VOLUME_UP_EVENT;
	SDL_PushEvent(&event);
}

int ffmfc_GetVolume()
{
	return percent;
}


void ffmfc_SetTimePosition(int64_t time)
{
	SDL_Event event;
	event.type = FFMFC_SET_POSITION;
	time_position = time;
	SDL_PushEvent(&event);
}

void ffmfc_VolumeDown()
{
	if (percent > 0)
		percent -= 10;

	SDL_Event event;
	event.type = FFMFC_VOLUME_DOWN_EVENT;
	SDL_PushEvent(&event);
}

//·¢ËÍ¡°Ç°½ø/ºóÍË¡±ÃüÁî
//Send Command "Seek"
void ffmfc_seek(int time){
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
void ffmfc_seek_bar(int pos){
	SDL_Event event;
	event.type = FFMFC_SEEK_BAR_EVENT;
	seek_bar_pos=pos;
	SDL_PushEvent(&event);
}



static void toggle_audio_display(VideoState *is,int mode)
{
#ifdef DISPLAY_VIDEO
	int bgcolor = SDL_MapRGB(dlg->screen->format, 0x00, 0x00, 0x00);
	//is->show_mode = (VideoState::ShowMode)((is->show_mode + 1) % SHOW_MODE_NB);
	switch(mode){
	case SHOW_MODE_VIDEO:is->show_mode=(VideoState::ShowMode)SHOW_MODE_VIDEO;break;
	case SHOW_MODE_WAVES:is->show_mode=(VideoState::ShowMode)SHOW_MODE_WAVES;break;
	case SHOW_MODE_RDFT:is->show_mode=(VideoState::ShowMode)SHOW_MODE_RDFT;break;
	default:is->show_mode=(VideoState::ShowMode)SHOW_MODE_VIDEO;break;
	}
	fill_rectangle(dlg->screen,
		is->xleft, is->ytop, is->width, is->height,
		bgcolor);
	SDL_UpdateRect(dlg->screen, is->xleft, is->ytop, is->width, is->height);
#endif
}

/* handle an event sent by the GUI */
//´¦Àí¸÷ÖÖÊó±ê¼üÅÌÃüÁî,°üÀ¨¸÷ÖÖÊÂ¼þ
static void event_loop(VideoState *cur_stream)
{
	SDL_Event event;
	double incr, pos, frac;

	for (;;) {

		double x;
		//ÅÐ¶ÏÍË³ö-------
		if(exit_remark==1)
			break;
		//---------------
		if (cur_stream->abort_request)
			break;

		SDL_WaitEvent(&event);
		switch (event.type) {
#ifdef SDL2
            case SDL_WINDOWEVENT:
                switch(event.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                        screen_width  = cur_stream->width  = event.window.data1;
                        screen_height = cur_stream->height = event.window.data2;
                        cur_stream->force_refresh = 1;
                        break;
                        
                    case SDL_WINDOWEVENT_EXPOSED:
                        cur_stream->force_refresh = 1;
                        break;
                }
                break;
#endif
		case SDL_KEYDOWN:
			if (exit_on_keydown) 
			{
				do_exit(cur_stream);
				break;
			}
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
			case SDLK_q:
				do_exit(cur_stream);
				break;
			case SDLK_f:
				//È«ÆÁ
				toggle_full_screen(cur_stream);
				cur_stream->force_refresh = 1;
				break;
			case SDLK_p:
				//ÔÝÍ£
			case SDLK_SPACE:
				toggle_pause(cur_stream);
				break;
			case SDLK_s: // S: Step to next frame
				step_to_next_frame(cur_stream);
				break;
			case SDLK_a:
				stream_cycle_channel(cur_stream, AVMEDIA_TYPE_AUDIO);
				break;
			case SDLK_v:
				stream_cycle_channel(cur_stream, AVMEDIA_TYPE_VIDEO);
				break;
			case SDLK_t:
				stream_cycle_channel(cur_stream, AVMEDIA_TYPE_SUBTITLE);
				break;
				//ÐÞ¸ÄÁËÒ»ÏÂ£¬ÈýÖÐÏÔÊ¾Ä£Ê½·Ö³ÉÁËÈý¸ö¼ü
			case SDLK_w:
				toggle_audio_display(cur_stream,SHOW_MODE_VIDEO);
				cur_stream->force_refresh = 1;
				break;
			case SDLK_e:
				toggle_audio_display(cur_stream,SHOW_MODE_WAVES);
				cur_stream->force_refresh = 1;
				break;
			case SDLK_r:
				toggle_audio_display(cur_stream,SHOW_MODE_RDFT);
				cur_stream->force_refresh = 1;
				break;
			case SDLK_y:
				cur_stream->v_show_mode=SHOW_MODE_Y;
				break;
			case SDLK_PAGEUP:
				incr = 600.0;
				goto do_seek;
			case SDLK_PAGEDOWN:
				incr = -600.0;
				goto do_seek;
				//×ó·½Ïò¼ü
			case SDLK_LEFT:
				incr = -10.0;
				goto do_seek;
			case SDLK_RIGHT:
				incr = 10.0;
				goto do_seek;
			case SDLK_UP:
				incr = 60.0;
				goto do_seek;
			case SDLK_DOWN:
				incr = -60.0;
do_seek:
				if (seek_by_bytes) {
					if (cur_stream->video_stream >= 0 && cur_stream->video_current_pos >= 0) {
						pos = cur_stream->video_current_pos;
					} else if (cur_stream->audio_stream >= 0 && cur_stream->audio_pkt.pos >= 0) {
						pos = cur_stream->audio_pkt.pos;
					} else
						pos = avio_tell(cur_stream->ic->pb);
					if (cur_stream->ic->bit_rate)
						incr *= cur_stream->ic->bit_rate / 8.0;
					else
						incr *= 180000.0;
					pos += incr;
					stream_seek(cur_stream, pos, incr, 1);
				} else {
					pos = get_master_clock(cur_stream);
					pos += incr;
					stream_seek(cur_stream, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
				}
				break;
			default:
				break;
			}
			break;
#ifndef SDL2
		case SDL_VIDEOEXPOSE:
			cur_stream->force_refresh = 1;
			break;
#endif
			//Êó±êµ¥»÷
		case SDL_MOUSEBUTTONDOWN:
			if (exit_on_mousedown) {
				do_exit(cur_stream);
				break;
			}
		case SDL_MOUSEMOTION:
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				x = event.button.x;
			} else {
				if (event.motion.state != SDL_PRESSED)
					break;
				x = event.motion.x;
			}
			if (seek_by_bytes || cur_stream->ic->duration <= 0) {
				uint64_t size =  avio_size(cur_stream->ic->pb);
				stream_seek(cur_stream, size*x/cur_stream->width, 0, 1);
			} else {
				int64_t ts;
				int ns, hh, mm, ss;
				int tns, thh, tmm, tss;
				tns  = cur_stream->ic->duration / 1000000LL;
				thh  = tns / 3600;
				tmm  = (tns % 3600) / 60;
				tss  = (tns % 60);
				frac = x / cur_stream->width;
				ns   = frac * tns;
				hh   = ns / 3600;
				mm   = (ns % 3600) / 60;
				ss   = (ns % 60);
				fprintf(stderr, "Seek to %2.0f%% (%2d:%02d:%02d) of total duration (%2d:%02d:%02d)       \n", frac*100,
					hh, mm, ss, thh, tmm, tss);
				ts = frac * cur_stream->ic->duration;
				if (cur_stream->ic->start_time != AV_NOPTS_VALUE)
					ts += cur_stream->ic->start_time;
				stream_seek(cur_stream, ts, 0, 0);
			}
			break;
#ifndef SDL2
		case SDL_VIDEORESIZE:
			/*
			dlg->screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 0,
				SDL_HWSURFACE|SDL_RESIZABLE|SDL_ASYNCBLIT|SDL_HWACCEL);
				*/
			screen_width  = cur_stream->width  = event.resize.w;
			screen_height = cur_stream->height = event.resize.h;
			cur_stream->force_refresh = 1;
			break;
#endif
		case SDL_QUIT:
		case FF_QUIT_EVENT:
			do_exit(cur_stream);
			break;
		case FFMFC_VOLUME_UP_EVENT:
		case FFMFC_VOLUME_DOWN_EVENT:
			volume = percentageToDb(percent, 128);
			break;
		case FFMFC_SET_POSITION:{
				int64_t ts = time_position;
				if (cur_stream->ic->start_time != AV_NOPTS_VALUE)
					ts += cur_stream->ic->start_time;
				stream_seek(cur_stream, ts, 0, 0);
			}
			break;


		case FF_ALLOC_EVENT:
			alloc_picture((VideoState *)(event.user.data1));
			break;
		case FF_REFRESH_EVENT:
			video_refresh(event.user.data1);
			cur_stream->refresh = 0;
			break;
		case FFMFC_SEEK_BAR_EVENT:{
			if (seek_by_bytes || cur_stream->ic->duration <= 0) {
				uint64_t size =  avio_size(cur_stream->ic->pb);
				stream_seek(cur_stream, size*seek_bar_pos/1000, 0, 1);
			} else {
				int64_t ts;
				frac=(double)seek_bar_pos/1000;
				ts = frac * cur_stream->ic->duration;
				if (cur_stream->ic->start_time != AV_NOPTS_VALUE)
					ts += cur_stream->ic->start_time;
				stream_seek(cur_stream, ts, 0, 0);
			}
			break;
							   }

		default:
			break;
		}
	}
}

static int opt_frame_size(void *optctx, const char *opt, const char *arg)
{
	av_log(nullptr, AV_LOG_WARNING, "Option -s is deprecated, use -video_size.\n");
	return opt_default(nullptr, "video_size", arg);
}

static int opt_width(void *optctx, const char *opt, const char *arg)
{
	screen_width = parse_number_or_die(opt, arg, OPT_INT64, 1, INT_MAX);
	return 0;
}

static int opt_height(void *optctx, const char *opt, const char *arg)
{
	screen_height = parse_number_or_die(opt, arg, OPT_INT64, 1, INT_MAX);
	return 0;
}

static int opt_format(void *optctx, const char *opt, const char *arg)
{
	file_iformat = av_find_input_format(arg);
	if (!file_iformat) {
		fprintf(stderr, "Unknown input format: %s\n", arg);
		return AVERROR(EINVAL);
	}
	return 0;
}

static int opt_frame_pix_fmt(void *optctx, const char *opt, const char *arg)
{
	av_log(nullptr, AV_LOG_WARNING, "Option -pix_fmt is deprecated, use -pixel_format.\n");
	return opt_default(nullptr, "pixel_format", arg);
}

static int opt_sync(void *optctx, const char *opt, const char *arg)
{
	if (!strcmp(arg, "audio"))
		av_sync_type = AV_SYNC_AUDIO_MASTER;
	else if (!strcmp(arg, "video"))
		av_sync_type = AV_SYNC_VIDEO_MASTER;
	else if (!strcmp(arg, "ext"))
		av_sync_type = AV_SYNC_EXTERNAL_CLOCK;
	else {
		fprintf(stderr, "Unknown value for %s: %s\n", opt, arg);
		exit(1);
	}
	return 0;
}

static int opt_seek(void *optctx, const char *opt, const char *arg)
{
	start_time = parse_time_or_die(opt, arg, 1);
	return 0;
}

static int opt_duration(void *optctx, const char *opt, const char *arg)
{
	duration = parse_time_or_die(opt, arg, 1);
	return 0;
}

static int opt_show_mode(void *optctx, const char *opt, const char *arg)
{
	show_mode = !strcmp(arg, "video") ? SHOW_MODE_VIDEO :
		!strcmp(arg, "waves") ? SHOW_MODE_WAVES :
		!strcmp(arg, "rdft" ) ? SHOW_MODE_RDFT  :
		(ShowMode)(int)parse_number_or_die(opt, arg, OPT_INT, 0, SHOW_MODE_NB-1);
return 0;
}

static void opt_input_file(void *optctx, const char *filename)
{
	//if (input_filename) {
	//    fprintf(stderr, "Argument '%s' provided as input filename, but '%s' was already specified.\n",
	//            filename, input_filename);
	//    exit(1);
	//}
	//if (!strcmp(filename, "-"))
	//    filename = "pipe:";
	//input_filename = filename;
}

static int opt_codec(void *o, const char *opt, const char *arg)
{
	switch(opt[strlen(opt)-1]){
	case 'a' :    audio_codec_name = arg; break;
	case 's' : subtitle_codec_name = arg; break;
	case 'v' :    video_codec_name = arg; break;
	}
	return 0;
}

static int dummy;

static const OptionDef options[] = {
#include "cmdutils_common_opts.h"
	{ "x", HAS_ARG, { (void*) opt_width }, "force displayed width", "width" },
	{ "y", HAS_ARG, { (void*) opt_height }, "force displayed height", "height" },
	{ "s", HAS_ARG | OPT_VIDEO, { (void*) opt_frame_size }, "set frame size (WxH or abbreviation)", "size" },
	{ "fs", OPT_BOOL, { &is_full_screen }, "force full screen" },
	{ "an", OPT_BOOL, { &audio_disable }, "disable audio" },
	{ "vn", OPT_BOOL, { &video_disable }, "disable video" },
	{ "ast", OPT_INT | HAS_ARG | OPT_EXPERT, { &wanted_stream[AVMEDIA_TYPE_AUDIO] }, "select desired audio stream", "stream_number" },
	{ "vst", OPT_INT | HAS_ARG | OPT_EXPERT, { &wanted_stream[AVMEDIA_TYPE_VIDEO] }, "select desired video stream", "stream_number" },
	{ "sst", OPT_INT | HAS_ARG | OPT_EXPERT, { &wanted_stream[AVMEDIA_TYPE_SUBTITLE] }, "select desired subtitle stream", "stream_number" },
	{ "ss", HAS_ARG, { (void*) opt_seek }, "seek to a given position in seconds", "pos" },
	{ "t", HAS_ARG, { (void*) opt_duration }, "play  \"duration\" seconds of audio/video", "duration" },
	{ "bytes", OPT_INT | HAS_ARG, { &seek_by_bytes }, "seek by bytes 0=off 1=on -1=auto", "val" },
	{ "nodisp", OPT_BOOL, { &display_disable }, "disable graphical display" },
	{ "f", HAS_ARG, { (void*) opt_format }, "force format", "fmt" },
	{ "pix_fmt", HAS_ARG | OPT_EXPERT | OPT_VIDEO, { (void*) opt_frame_pix_fmt }, "set pixel format", "format" },
	{ "stats", OPT_BOOL | OPT_EXPERT, { &show_status }, "show status", "" },
	{ "bug", OPT_INT | HAS_ARG | OPT_EXPERT, { &workaround_bugs }, "workaround bugs", "" },
	{ "fast", OPT_BOOL | OPT_EXPERT, { &fast }, "non spec compliant optimizations", "" },
	{ "genpts", OPT_BOOL | OPT_EXPERT, { &genpts }, "generate pts", "" },
	{ "drp", OPT_INT | HAS_ARG | OPT_EXPERT, { &decoder_reorder_pts }, "let decoder reorder pts 0=off 1=on -1=auto", ""},
	{ "lowres", OPT_INT | HAS_ARG | OPT_EXPERT, { &lowres }, "", "" },
	{ "skiploop", OPT_INT | HAS_ARG | OPT_EXPERT, { &skip_loop_filter }, "", "" },
	{ "skipframe", OPT_INT | HAS_ARG | OPT_EXPERT, { &skip_frame }, "", "" },
	{ "skipidct", OPT_INT | HAS_ARG | OPT_EXPERT, { &skip_idct }, "", "" },
	{ "idct", OPT_INT | HAS_ARG | OPT_EXPERT, { &idct }, "set idct algo",  "algo" },
	{ "ec", OPT_INT | HAS_ARG | OPT_EXPERT, { &error_concealment }, "set error concealment options",  "bit_mask" },
	{ "sync", HAS_ARG | OPT_EXPERT, { (void*) opt_sync }, "set audio-video sync. type (type=audio/video/ext)", "type" },
	{ "autoexit", OPT_BOOL | OPT_EXPERT, { &autoexit }, "exit at the end", "" },
	{ "exitonkeydown", OPT_BOOL | OPT_EXPERT, { &exit_on_keydown }, "exit on key down", "" },
	{ "exitonmousedown", OPT_BOOL | OPT_EXPERT, { &exit_on_mousedown }, "exit on mouse down", "" },
	{ "loop", OPT_INT | HAS_ARG | OPT_EXPERT, { &loop }, "set number of times the playback shall be looped", "loop count" },
	{ "framedrop", OPT_BOOL | OPT_EXPERT, { &framedrop }, "drop frames when cpu is too slow", "" },
	{ "infbuf", OPT_BOOL | OPT_EXPERT, { &infinite_buffer }, "don't limit the input buffer size (useful with realtime streams)", "" },
	{ "window_title", OPT_STRING | HAS_ARG, { &window_title }, "set window title", "window title" },
#if CONFIG_AVFILTER
	{ "vf", OPT_STRING | HAS_ARG, { &vfilters }, "video filters", "filter list" },
#endif
	{ "rdftspeed", OPT_INT | HAS_ARG| OPT_AUDIO | OPT_EXPERT, { &rdftspeed }, "rdft speed", "msecs" },
	{ "showmode", HAS_ARG, { (void*) opt_show_mode}, "select show mode (0 = video, 1 = waves, 2 = RDFT)", "mode" },
	{ "default", HAS_ARG | OPT_AUDIO | OPT_VIDEO | OPT_EXPERT, { (void*)opt_default }, "generic catch all option", "" },
	{ "i", OPT_BOOL, { &dummy}, "read specified file", "input_file"},
	{ "codec", HAS_ARG, { (void*) opt_codec}, "force decoder", "decoder" },
	{ nullptr, },
};

static void show_usage(void)
{
	av_log(nullptr, AV_LOG_INFO, "Simple media player\n");
	av_log(nullptr, AV_LOG_INFO, "usage: %s [options] input_file\n", program_name);
	av_log(nullptr, AV_LOG_INFO, "\n");
}

void show_help_default(const char *opt, const char *arg)
{
	av_log_set_callback(log_callback_help);
	show_usage();
	show_help_options(options, "Main options:", 0, OPT_EXPERT, 0);
	show_help_options(options, "Advanced options:", OPT_EXPERT, 0, 0);
	printf("\n");
	show_help_children(avcodec_get_class(), AV_OPT_FLAG_DECODING_PARAM);
	show_help_children(avformat_get_class(), AV_OPT_FLAG_DECODING_PARAM);
#if !CONFIG_AVFILTER
	show_help_children(sws_get_class(), AV_OPT_FLAG_ENCODING_PARAM);
#else
	show_help_children(avfilter_get_class(), AV_OPT_FLAG_FILTERING_PARAM);
#endif
	printf("\nWhile playing:\n"
		"q, ESC              quit\n"
		"f                   toggle full screen\n"
		"p, SPC              pause\n"
		"a                   cycle audio channel\n"
		"v                   cycle video channel\n"
		"t                   cycle subtitle channel\n"
		"w                   show audio waves\n"
		"s                   activate frame-step mode\n"
		"left/right          seek backward/forward 10 seconds\n"
		"down/up             seek backward/forward 1 minute\n"
		"page down/page up   seek backward/forward 10 minutes\n"
		"mouse click         seek to percentage in file corresponding to fraction of width\n"
		);
}

static int lockmgr(void **mtx, enum AVLockOp op)
{
	switch(op) {
	case AV_LOCK_CREATE:
		*mtx = SDL_CreateMutex();
		if(!*mtx)
			return 1;
		return 0;
	case AV_LOCK_OBTAIN:
		return !!SDL_LockMutex((SDL_mutex *)*mtx);
	case AV_LOCK_RELEASE:
		return !!SDL_UnlockMutex((SDL_mutex *)*mtx);
	case AV_LOCK_DESTROY:
		SDL_DestroyMutex((SDL_mutex *)*mtx);
		return 0;
	}
	return 1;
}

/* Called from the main */
//½âÂëÖ÷º¯Êý
//Main function
//#define __MINGW32__
int ffmfc_play(CVideoControl * control, string filename)
{
	dlg = control;

	ffmfc_reset_index();
	//ÍË³ö·ûºÅÖÃÁã
	exit_remark=0;
	int flags;
	//ºËÐÄ½á¹¹Ìå
	VideoState *is;

	char dummy_videodriver[] = "SDL_VIDEODRIVER=dummy";
	input_filename = filename;

	//   av_log_set_flags(AV_LOG_SKIP_REPEATED);
	//    parse_loglevel(argc, argv, options);
/*
	// register all codecs, demux and protocols 
	avcodec_register_all();
#if CONFIG_AVDEVICE
	avdevice_register_all();
#endif
#if CONFIG_AVFILTER
	avfilter_register_all();
#endif
	av_register_all();
	//Èç¹ûÃ»ÓÐ±¾¾ä»á³ö´íÎó£º
	//Warning:Using network protocols without global network initialization.
	//²âÊÔÍøÂçÊý¾ÝµÄÊ±ºòÖ±½ÓÊ¹ÓÃrtsp://169.254.197.35:8554/sh1943.mpg¸ñÊ½µÄ²ÎÊý¾ÍÐÐ
	//»òÊÇÊ¹ÓÃ rtmp://localhost/vod/sample.flv¶¼¿ÉÒÔ
	//ÒòÎª´Ë°æ±¾ffmpeg¼¯³ÉÁËlibrtmp
	//ºÍÎÄ¼þºÜÀàËÆ
	avformat_network_init();
*/
	//it_opts();

	//signal(SIGINT , sigterm_handler); /* Interrupt (ANSI).    */
	//signal(SIGTERM, sigterm_handler); /* Termination (ANSI).  */
	//input_filename = "rtmp://live.hkstv.hk.lxdns.com/live/hks live=1";
	//´«ÈëÎÄ¼þÂ·¾¶
	//input_filename=(const char *)malloc(MAX_URL_LENGTH);

	//GetWindowTextA(dlg->m_inputurl,(LPSTR)input_filename,MAX_URL_LENGTH);
	//show_banner(argc, argv, options);
	//»ñÈ¡²¥·ÅµÄÊ×Ñ¡Ïî
	//int opt_argc=0;
	//char **opt_argv=nullptr;

	//parse_options(nullptr, opt_argc, opt_argv, options, opt_input_file);
	/*
	if (!input_filename) 
	{
	show_usage();
	fprintf(stderr, "An input file must be specified\n");
	fprintf(stderr, "Use -h to get full help or, even better, run 'man %s'\n", program_name);
	exit(1);
	}
	*/
	if (display_disable) 
	{
		video_disable = 1;
	}
	//ÉèÖÃÎª×Ô¶¯ÍË³ö-------------------
	//½âÂëÍ¼Æ¬µÄÊ±ºò£¬»á×Ô¶¯ÍË³ö£¬ËùÒÔÏÈ²»ÉèÖÃÁË~
	autoexit=1;
	/*
	//---------------------------------
	flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
	if (audio_disable)
		flags &= ~SDL_INIT_AUDIO;

#ifndef SDL2
	if (display_disable)
		SDL_putenv(dummy_videodriver); // For the event queue, we always need a video driver. 
#endif
*/
    /*
	char sdl_var[128]; 
	//sprintf(sdl_var, "SDL_WINDOWID=0x%lx", hWnd );//Ö÷´°¿Ú¾ä±ú 
	SDL_putenv(sdl_var); 
	char *myvalue = SDL_getenv("SDL_WINDOWID");
*/

#if !defined(__MINGW32__) && !defined(__APPLE__)
//	flags |= SDL_INIT_EVENTTHREAD; /* Not supported on Windows or Mac OS X */
#endif
	//------SDL------------------------
	//³õÊ¼»¯
	/*
	if (SDL_Init (flags)) {
		wxMessageBox(_T("Could not initialize SDL "));
		exit(1);
	}*/

	if (!display_disable) {
#if HAFFMFC_SDL_VIDEO_SIZE
		const SDL_VideoInfo *vi = SDL_GetVideoInfo();
		fs_screen_width = vi->current_w;
		fs_screen_height = vi->current_h;
#endif
	}

#ifndef SDL2
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
#endif
	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
	SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

	if (av_lockmgr_register(lockmgr)) {
		//AfxMessageBox(_T("Could not initialize lock manager!"));
		do_exit(nullptr);
	}

	av_init_packet(&flush_pkt);
	flush_pkt.data = (uint8_t *)(intptr_t)"FLUSH";
	//½âÂëÖ÷º¯Êý
	is = stream_open(filename.c_str(), file_iformat);
	if (!is) {
		//AfxMessageBox(_T("Failed to initialize VideoState!"));
		do_exit(nullptr);
	}

	g_is = is;

	g_is->width = dlg->getWidth();
	g_is->height = dlg->getHeight();

	//const char *info=(char *)malloc(500);
	//info=avformat_configuration();

	//--------------------

	//Video Start
	//dlg->VideoStart();
    wxCommandEvent event(EVENT_VIDEOSTART);
    wxPostEvent(dlg, event);

	event_loop(is);
    
    g_is = nullptr;

	return 0;
}

