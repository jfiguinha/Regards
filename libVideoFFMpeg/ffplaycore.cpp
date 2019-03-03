#include "header.h"
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
#include "WindowMain.h"
#include "ffplaycore.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_thread.h>
#include <SDL_mutex.h>
#include <Interpolation.h>
#include <RegardsBitmap.h>

using namespace Regards::Window;

//#include "ColorSpaceConverter.h"
//#include <DShow.h>
//const char program_name[] = "ffplaymfc";
//const int program_birth_year = 2013;

#ifdef WIN32

#include "ffmpeg_dxva2.h"

AVPixelFormat GetHwFormat(AVCodecContext *s, const AVPixelFormat *pix_fmts)
{
    InputStream* ist = (InputStream*)s->opaque;
    ist->active_hwaccel_id = HWACCEL_DXVA2;
    ist->hwaccel_pix_fmt = AV_PIX_FMT_DXVA2_VLD;
    return ist->hwaccel_pix_fmt;
}

DXVA2Context * dxva2 = nullptr;
#endif

//SOUND Volume
int percentVolume = 100;
int volume = 100;

#define MAX_QUEUE_SIZE (30 * 1024 * 1024)
#define MIN_FRAMES 5

/* SDL audio buffer size, in samples. Should be small to have precise
A/V sync as SDL does not have hardware buffer fullness info. */
#define SDL_AUDIO_BUFFER_SIZE 4096

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


mutex videoStateMutex;

typedef struct VideoPicture {
	double pts;                                  ///< presentation time stamp for this picture
	int64_t pos;                                 ///< byte position in file
	int skip;
	int width, height; /* source height & width */
	AVRational sample_aspect_ratio;
	int allocated;
	int reallocate;
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
	double frame_last_delay;
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
	char filename[1024];
	int width, height, xleft, ytop;
	int step;
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
CVideoControlInterface * dlg;

/* options specified by the user */
static AVInputFormat *file_iformat;
static string input_filename;
static void uninit_opts(void);
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
static int show_status = 0;
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


/* current context */
static int64_t audio_callback_time;

static AVPacket flush_pkt;


#define FF_ALLOC_EVENT   (SDL_USEREVENT)
#define FF_REFRESH_EVENT (SDL_USEREVENT + 1)
#define FF_QUIT_EVENT    4096

//×Ô¶¨ÒåÒ»¸öÊÂ¼þ£¬ÓÃÓÚµ÷Õû²¥·Å½ø¶È
#define FFMFC_SEEK_BAR_EVENT    (SDL_USEREVENT + 4)
#define FFMFC_VOLUME_UP_EVENT    (SDL_USEREVENT + 5)
#define FFMFC_VOLUME_DOWN_EVENT    (SDL_USEREVENT + 6)
#define FFMFC_SET_POSITION (SDL_USEREVENT + 7)
#define FFMFC_CHANGE_AUDIO (SDL_USEREVENT + 8)
#define FFMFC_CHANGE_SUBTITLE (SDL_USEREVENT + 9)
int seek_bar_pos;
int64_t time_position = 0;
int video_angle = 0;
int video_flipV = 0;
int video_flipH = 0;
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

//static void video_image_display(VideoState *is);
static double get_master_clock(VideoState *is);
#ifndef CMDUTILS
AVDictionary *format_opts, *codec_opts;
#endif

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

void ffmfc_SetVideoParameter(int angle, int flipV, int flipH)
{
	video_angle = angle;
	video_flipV = flipV;
	video_flipH = flipH;
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
	for (i = 0; i < VIDEO_PICTURE_QUEUE_SIZE; i++)
	{
		vp = &is->pictq[i];
	}

	SDL_DestroyMutex(is->pictq_mutex);
	SDL_DestroyCond(is->pictq_cond);
	SDL_DestroyMutex(is->subpq_mutex);
	SDL_DestroyCond(is->subpq_cond);
	SDL_DestroyCond(is->continue_read_thread);
	av_free(is);
}

//ÍË³ö
static void do_exit(VideoState *is)
{
	exit_remark=1;
	if (is) {
		stream_close(is);
	}
	av_lockmgr_register(nullptr);
	uninit_opts();
	//avformat_network_deinit();
	if (show_status)
		printf("\n");

	av_log(nullptr, AV_LOG_QUIET, "%s", "");

}

//·¢ËÍ¡°ÍË³ö¡±ÃüÁî
//Send Command "Quit"
bool ffmfc_quit()
{
	//do_exit(g_is);
	bool isExitNow = false;
	if (g_is)
	{
		SDL_Event event;
		event.type = FF_QUIT_EVENT;
		event.user.data1 = g_is;
		{
			SDL_PushEvent(&event);
		}
	}
	else
	{
		do_exit(nullptr);
		isExitNow = true;
	}
	return isExitNow;
}
/*
static void sigterm_handler(int sig)
{
	exit(123);
}
*/
/* display the current picture, if any */
static void video_display(VideoState *is)
{
#ifdef DISPLAY_VIDEO
	if (!dlg->screen)
		video_open(is, 0);
#endif
    if(!CMasterWindow::endProgram)
    {

        if (is->audio_st && is->show_mode != SHOW_MODE_VIDEO)
            video_audio_display(is);
        else if (is->video_st)
		{
			VideoPicture *vp;
			SubPicture *sp;
			AVPicture pict;
			SDL_Rect rect;
			int i;
			
			if (is->subtitle_st)
			{
				if (is->subpq_size > 0)
				{
					sp = &is->subpq[is->subpq_rindex];
					vp = &is->pictq[is->pictq_rindex];

					if (vp->pts >= sp->pts + ((float) sp->sub.start_display_time / 1000))
					{
						for (i = 0; i < sp->sub.num_rects; i++)
						{
							AVSubtitleRect *rect = sp->sub.rects[i];
							AVPicture picture = rect->pict;
							CRegardsBitmap * bitmap = new CRegardsBitmap(rect->w, rect->h);
							uint8_t * data = picture.data[0];
							uint8_t * colorIndex = picture.data[1];
							uint8_t * ptBitmap = bitmap->GetPtBitmap();
							for(int y = 0;y < rect->h;y++)
							{
								for(int x = 0;x < rect->w;x++)
								{
									int r,g,b,a;
									int j = *data++;
									RGBA_IN(r, g, b, a, (uint32_t*)picture.data[1] + j);
									CRgbaquad color(r,g,b,a);
									bitmap->SetColorValue(x,y,color);
								}
							}
							dlg->SetSubtitulePicture(bitmap);
						}
					}
				}

			}
			else
			{
				dlg->DeleteSubtitulePicture();
			}
			
            //dlg->Refresh();
		}
        
    }

	//video_image_display(is);
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
        
        if(CMasterWindow::endProgram)
            return 0;
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
			// dequeue the picture
			vp = &is->pictq[is->pictq_rindex];

			if (vp->skip) {
				pictq_next_picture(is);
				goto retry;
			}

			if (is->paused)
				goto display;

			// compute nominal last_duration
			last_duration = vp->pts - is->frame_last_pts;
			if (last_duration > 0 && last_duration < 10.0) {
				// if duration of the last frame was sane, update last_duration in video state 
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

						// update queue size and signal for next picture 
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

							// update queue size and signal for next picture
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
			//display picture 
			if (!display_disable)
				video_display(is);

			pictq_next_picture(is);
		}
	} else if (is->audio_st) {
		// draw the next audio frame 

		// if only audio stream, then display the audio bars (better
		//than nothing, just to test the implementation 

		//display picture 
		if (!display_disable)
			video_display(is);
	}
	is->force_refresh = 0;
	/*
	wxString currentclockstr;
	int tns, thh, tmm, tss;
	tns  = get_master_clock(is);
	thh  = tns / 3600;
	tmm  = (tns % 3600) / 60;
	tss  = (tns % 60);

	double avgFrame = av_q2d(g_is->ic->streams[g_is->video_stream]->avg_frame_rate);
	currentclockstr = wxString::Format(_T("%02d:%02d:%02d FPS : %0.3f"),thh,tmm,tss, avgFrame);
		
	dlg->SetCurrentclock(currentclockstr);*/
	dlg->SetPos(get_master_clock(is) * 1000);

	/*
	if (show_status) 
	{
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
#ifndef WIN32
			printf("%7.2f A-V:%7.3f fd=%4d aq=%5dKB vq=%5dKB sq=%5dB f=%" PRId64"/%" PRId64"   \r",
				get_master_clock(is),
				av_diff,
				is->frame_drops_early + is->frame_drops_late,
				aqsize / 1024,
				vqsize / 1024,
				sqsize,
				is->video_st ? is->video_st->codec->pts_correction_num_faulty_dts : 0,
				is->video_st ? is->video_st->codec->pts_correction_num_faulty_pts : 0);
#endif
			//int pos=1000*get_master_clock(is)/(is->ic->duration/1000000);
			dlg->SetPos(get_master_clock(is) * 1000);

			wxString currentclockstr;
			int tns, thh, tmm, tss;
			tns  = get_master_clock(is);
			thh  = tns / 3600;
			tmm  = (tns % 3600) / 60;
			tss  = (tns % 60);
			currentclockstr.Format(_T("%02d:%02d:%02d"),thh,tmm,tss);
			dlg->SetCurrentclock(currentclockstr);

			fflush(stdout);
			last_time = cur_time;
		}

	}
	else
	{
		wxString currentclockstr;
		int tns, thh, tmm, tss;
		tns  = get_master_clock(is);
		thh  = tns / 3600;
		tmm  = (tns % 3600) / 60;
		tss  = (tns % 60);
		currentclockstr = wxString::Format(_T("%02d:%02d:%02d"),thh,tmm,tss);
		dlg->SetCurrentclock(currentclockstr);
		dlg->SetPos(get_master_clock(is) * 1000);
	}*/
}

/* allocate a picture (needs to do that in main thread to avoid
potential locking problems */
static void alloc_picture(VideoState *is)
{
	VideoPicture *vp;

	vp = &is->pictq[is->pictq_windex];

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

	vp->sample_aspect_ratio = av_guess_sample_aspect_ratio(is->ic, is->video_st, src_frame);


#ifdef YUV
	if (is->videoq.abort_request)
		return -1;

	if(TRUE)
	{
		float video_aspect_ratio = 0;
		if (vp->sample_aspect_ratio.num == 0)
			video_aspect_ratio = 0;
		else
			video_aspect_ratio = av_q2d(vp->sample_aspect_ratio);
#ifdef WIN32
        if(dlg->GetDXVA2Compatible())
            dlg->SetData(src_frame, video_aspect_ratio, dxva2);
        else
            dlg->SetData(src_frame, video_aspect_ratio, nullptr);
#else
        dlg->SetData(src_frame, video_aspect_ratio, nullptr);
#endif
		//av_frame_unref(src_frame);
#else


	/* if the frame is not skipped, then display it */
	if (vp->bmp) 
	{
		AVPicture pict = { { 0 } };

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
		}
		
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
		//SDL_UnlockSurface(vp->bmp);

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
		//ffmfc_param_vframe(is,frame,pkt);
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

//½âÂëÊÓÆµ
static int video_thread(void *arg)
{
	AVPacket pkt = { 0 };
	VideoState *is = (VideoState *)arg;
	//AVFrame *frame = avcodec_alloc_frame();
	AVFrame *frame = av_frame_alloc();
	int64_t pts_int = AV_NOPTS_VALUE, pos = -1;
	double pts;
	int ret;


	for (;;) 
    {
		while (is->paused && !is->videoq.abort_request)
        {
           SDL_Delay(10);
            
        }
			

        

		//avcodec_get_frame_defaults(frame);
		av_frame_unref(frame);
		av_free_packet(&pkt);
		//½âÂëÒ»Ö¡ÊÓÆµ
		ret = get_video_frame(is, frame, &pts_int, &pkt);
		if (ret < 0)
			goto the_end;

		if (!ret)
			continue;

		pts = pts_int * av_q2d(is->video_st->time_base);
		//½âÂë³É¹¦ºóÓÃÓÚÏÔÊ¾£¬Ò²ÊÇ·Åµ½ÁíÒ»¸ö¶ÓÁÐÖÐ£¿
		ret = queue_picture(is, frame, pts, pkt.pos);

		if (ret < 0)
			goto the_end;

		if (is->step)
			stream_toggle_pause(is);
	}
the_end:
	avcodec_flush_buffers(is->video_st->codec);
	av_free_packet(&pkt);
	av_frame_free(&frame);
	//avcodec_free_frame(&frame);
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

			/*
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
			*/
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
                    /*
					av_dlog(nullptr, "diff=%f adiff=%f sample_diff=%d apts=%0.3f vpts=%0.3f %f\n",
						diff, avg_diff, wanted_nb_samples - nb_samples,
						is->audio_clock, is->video_clock, is->audio_diff_threshold);
                         * */
                    printf("diff=%f adiff=%f sample_diff=%d apts=%0.3f vpts=%0.3f %f\n",
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
				if (!(is->frame = av_frame_alloc()))//avcodec_alloc_frame()))
					return AVERROR(ENOMEM);
			} else
				av_frame_unref(is->frame);
				//avcodec_get_frame_defaults(is->frame);

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
			//ffmfc_param_aframe(is,is->frame,pkt_temp);
			//----------

			//------------------------
			pkt_temp->data += len1;
			pkt_temp->size -= len1;

			if (!got_frame) {
				/* stop sending empty packets if the decoder is finished */
				if (!pkt_temp->data && dec->codec->capabilities & AV_CODEC_CAP_DELAY)
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
		float volume = (((float)percentVolume / 100.0f) * SDL_MIX_MAXVOLUME);
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

#ifndef CMDUTILS

int check_stream_specifier(AVFormatContext *s, AVStream *st, const char *spec)
{
    int ret = avformat_match_stream_specifier(s, st, spec);
    if (ret < 0)
        av_log(s, AV_LOG_ERROR, "Invalid stream specifier: %s.\n", spec);
    return ret;
}

AVDictionary *filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
                                AVFormatContext *s, AVStream *st, AVCodec *codec)
{
    AVDictionary    *ret = nullptr;
    AVDictionaryEntry *t = nullptr;
    int            flags = s->oformat ? AV_OPT_FLAG_ENCODING_PARAM
                                      : AV_OPT_FLAG_DECODING_PARAM;
    char          prefix = 0;
    const AVClass    *cc = avcodec_get_class();

    if (!codec)
        codec            = s->oformat ? avcodec_find_encoder(codec_id)
                                      : avcodec_find_decoder(codec_id);
    if (!codec)
        return nullptr;

    switch (codec->type) {
    case AVMEDIA_TYPE_VIDEO:
        prefix  = 'v';
        flags  |= AV_OPT_FLAG_VIDEO_PARAM;
        break;
    case AVMEDIA_TYPE_AUDIO:
        prefix  = 'a';
        flags  |= AV_OPT_FLAG_AUDIO_PARAM;
        break;
    case AVMEDIA_TYPE_SUBTITLE:
        prefix  = 's';
        flags  |= AV_OPT_FLAG_SUBTITLE_PARAM;
        break;
    }

    while (t = av_dict_get(opts, "", t, AV_DICT_IGNORE_SUFFIX)) {
        char *p = strchr(t->key, ':');

        /* check stream specification in opt name */
        if (p)
            switch (check_stream_specifier(s, st, p + 1)) {
            case  1: *p = 0; break;
            case  0:         continue;
            default:         return nullptr;
            }

        if (av_opt_find(&cc, t->key, nullptr, flags, AV_OPT_SEARCH_FAKE_OBJ) ||
            (codec && codec->priv_class &&
             av_opt_find(&codec->priv_class, t->key, nullptr, flags,
                         AV_OPT_SEARCH_FAKE_OBJ)))
            av_dict_set(&ret, t->key, t->value, 0);
        else if (t->key[0] == prefix &&
                 av_opt_find(&cc, t->key + 1, nullptr, flags,
                             AV_OPT_SEARCH_FAKE_OBJ))
            av_dict_set(&ret, t->key + 1, t->value, 0);

        if (p)
            *p = ':';
    }
    return ret;
}

void uninit_opts(void)
{
	/*
#if CONFIG_SWSCALE
    sws_freeContext(sws_opts);
    sws_opts = nullptr;
#endif

    if(CONFIG_SWRESAMPLE)
        swr_free(&swr_opts);
	*/
    av_dict_free(&format_opts);
    av_dict_free(&codec_opts);
}

AVDictionary **setup_find_stream_info_opts(AVFormatContext *s,
                                           AVDictionary *codec_opts)
{
    int i;
    AVDictionary **opts;

    if (!s->nb_streams)
        return nullptr;
    opts = (AVDictionary **)av_mallocz(s->nb_streams * sizeof(*opts));
    if (!opts) {
        av_log(nullptr, AV_LOG_ERROR,
               "Could not alloc memory for stream options.\n");
        return nullptr;
    }
    for (i = 0; i < s->nb_streams; i++)
        opts[i] = filter_codec_opts(codec_opts, s->streams[i]->codec->codec_id,
                                    s, s->streams[i], nullptr);
    return opts;
}
#endif

const char *getExt (const char *fspec) {
    const char *e = strrchr (fspec, '.');
    if (e == NULL)
        e = ""; // fast method, could also use &(fspec[strlen(fspec)]).
    return e;
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
	bool dvxa2 = false;

	if (stream_index < 0 || stream_index >= ic->nb_streams)
		return -1;
	avctx = ic->streams[stream_index]->codec;
	// ÎªÊÓÆµÁ÷Ñ°ÕÒ½âÂëÆ÷
	//×¢Òâ£º´Ë´¦¿ÉÒÔÖ¸¶¨½âÂëÆ÷
	codec = avcodec_find_decoder(avctx->codec_id);
//#ifdef CMDUTILS
	opts = filter_codec_opts(codec_opts, avctx->codec_id, ic, ic->streams[stream_index], codec);
//#endif

	

	switch(avctx->codec_type){
	case AVMEDIA_TYPE_AUDIO   : 
		is->last_audio_stream    = stream_index; 
		if(audio_codec_name   ) 
			codec= avcodec_find_decoder_by_name(   audio_codec_name); 
		break;
	case AVMEDIA_TYPE_SUBTITLE: is->last_subtitle_stream = stream_index; if(subtitle_codec_name) codec= avcodec_find_decoder_by_name(subtitle_codec_name); break;
	case AVMEDIA_TYPE_VIDEO   : 
		is->last_video_stream    = stream_index; 
		if(video_codec_name   ) 
			codec= avcodec_find_decoder_by_name(   video_codec_name); 
            
#ifdef __APPLE__

            //if(avctx->codec_id == AV_CODEC_ID_H264)
            //    codec= avcodec_find_decoder_by_name("h264_vda");

#endif
            
#ifdef WIN32
            if(dlg->GetDXVA2Compatible())
            {
                avctx->thread_count = 1;  // Multithreading is apparently not compatible with hardware decoding
                InputStream *ist = new InputStream();
                ist->hwaccel_id = HWACCEL_AUTO;
                ist->hwaccel_device = "dxva2";
                ist->dec = codec;
                ist->dec_ctx = avctx;
                avctx->coded_height = ic->streams[stream_index]->codec->height;
                avctx->coded_width = ic->streams[stream_index]->codec->width;
                
                
        //printf("ÊÓÆµ¸ß£º%d\n",ic->streams[video_index]->codec->height);

                avctx->opaque = ist;
                if(dxva2_init(avctx, dlg, avctx->coded_width, avctx->coded_height) == 0)
                {
                    InputStream *ist = (InputStream *)avctx->opaque;
                    dxva2 = (DXVA2Context *)ist->hwaccel_ctx;
                    //dlg->SetOpenCLDevice((void *)ist);
                    dlg->SetDXVA2Compatible(true);
                    avctx->get_buffer2 = ist->hwaccel_get_buffer;
                    avctx->get_format = GetHwFormat;
                    avctx->thread_safe_callbacks = 1;

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

                    /*
                    if(avctx->lowres) avctx->flags |= CODEC_FLAG_EMU_EDGE;
                    if (fast)   avctx->flags2 |= CODEC_FLAG2_FAST;
                    if(codec->capabilities & CODEC_CAP_DR1)
                        avctx->flags |= CODEC_FLAG_EMU_EDGE;
                    */
                    //MetaData?
                    if (!av_dict_get(opts, "threads", nullptr, 0))
                        av_dict_set(&opts, "threads", "auto", 0);
                    // ´ò¿ª½âÂëÆ÷£¬¶þÕßÖ®¼ä½¨Á¢ÁªÏµ

                    if (avcodec_open2(avctx, codec, nullptr) < 0)
                    {
                        std::cout << "Video codec open error" << std::endl;
                        return false;
                    }
                    dvxa2 = true;
                }
                else
                {
                    dlg->SetDXVA2Compatible(false);
                }
            }
#endif

		break;
	}
	if (!codec)
		return -1;

	if(!dvxa2)
	{

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

		//if(avctx->lowres) 
        //    avctx->flags |= CODEC_FLAG_EMU_EDGE;
		//if (fast)   
        //    avctx->flags2 |= AV_CODEC_FLAG2_FAST;
		//if(codec->capabilities & AV_CODEC_CAP_DR1)
		//	avctx->flags |= CODEC_FLAG_EMU_EDGE;
		//MetaData?
#ifdef CMDUTILS
		if (!av_dict_get(opts, "threads", nullptr, 0))
			av_dict_set(&opts, "threads", "auto", 0);
#else
		av_dict_set(&opts, "threads", "auto", 0);
#endif
		// ´ò¿ª½âÂëÆ÷£¬¶þÕßÖ®¼ä½¨Á¢ÁªÏµ
		if (!codec ||
			avcodec_open2(avctx, codec, &opts) < 0)
			return -1;
		if ((t = av_dict_get(opts, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
			av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
			return AVERROR_OPTION_NOT_FOUND;
		}
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
    
	switch (avctx->codec_type)
    {
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
            

            
	case AVMEDIA_TYPE_VIDEO:
        {
            is->video_stream = stream_index;
            is->video_st = ic->streams[stream_index];
            
            packet_queue_start(&is->videoq);
            //ÊÓÆµÏß³Ì
#ifdef SDL2
            is->video_tid = SDL_CreateThread(video_thread, "Video Thread", is);
#else
            is->video_tid = SDL_CreateThread(video_thread, is);
#endif
            
            int32_t* matrix = reinterpret_cast<int32_t*>(av_stream_get_side_data(is->video_st, AV_PKT_DATA_DISPLAYMATRIX, nullptr));
            if (matrix)
            {
                long rotation = lround(av_display_rotation_get(matrix));
                dlg->SetRotation(rotation);
            }
        }
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
		av_frame_free(&is->frame);
		//avcodec_free_frame(&is->frame);

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

#ifdef WIN32
        
		if(dlg->GetDXVA2Compatible())
			dxva2_uninit(avctx);
#endif

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
#ifdef CMDUTILS
		print_error(is->filename, err);
#endif
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


	for(i = 0; i < orig_nb_streams; i++)
	{	
		av_dict_free(&opts[i]);
	}
	
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
	{
		AVDictionaryEntry * tag = nullptr;

		const char * stream_type = av_get_media_type_string(ic->streams[i]->codec->codec_type);


		if(ic->streams[i]->codec->codec_type == 	AVMEDIA_TYPE_AUDIO)
		{
			while ((tag = av_dict_get(ic->streams[i]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
			{
				printf("STREAM_%s_%s=%s\n", stream_type,tag->key, tag->value);
			}
		}
		else if(ic->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			while ((tag = av_dict_get(ic->streams[i]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
			{
				printf("STREAM_%s_%s=%s\n", stream_type,tag->key, tag->value);
			}
		}
		else if(ic->streams[i]->codec->codec_type == AVMEDIA_TYPE_SUBTITLE)
		{
			while ((tag = av_dict_get(ic->streams[i]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
			{
				printf("STREAM_%s_%s=%s\n", stream_type,tag->key, tag->value);
			}
		}

		ic->streams[i]->discard = AVDISCARD_ALL;
	}

	float zhenlv;
	if (!video_disable)
		st_index[AVMEDIA_TYPE_VIDEO] = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, wanted_stream[AVMEDIA_TYPE_VIDEO], -1, nullptr, 0);


	//int video_index = st_index[AVMEDIA_TYPE_VIDEO];
	//printf("ÊÓÆµ±àÂëÆ÷Ãû³Æ£º%s\n",ic->streams[video_index]->codec->codec_name);
	//printf("ÊÓÆµ¿í£º%d\n",ic->streams[video_index]->codec->width);
	//printf("ÊÓÆµ¸ß£º%d\n",ic->streams[video_index]->codec->height);
	//zhenlv=(ic->streams[video_index]->codec->time_base.den)/(ic->streams[video_index]->codec->time_base.num);
	//printf("ÊÓÆµÖ¡ÂÊ£º%f\n",zhenlv);


	if (!audio_disable)
		st_index[AVMEDIA_TYPE_AUDIO] = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, wanted_stream[AVMEDIA_TYPE_AUDIO], st_index[AVMEDIA_TYPE_VIDEO], nullptr, 0);

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
	//ffmfc_param_global(is);
	dlg->SetVideoDuration(is->ic->duration);

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
				is->audio_st->codec->codec->capabilities & AV_CODEC_CAP_DELAY) {
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

		//ffmfc_param_packet(is,pkt);
		//--------------------
		if (ret < 0) {
			if (ret == AVERROR_EOF || ic->pb->eof_reached)//url_feof(ic->pb))
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


//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
static void stream_change_stream(VideoState *is, int codec_type, int newIndex)
{
	//AVFormatContext *ic = is->ic;
	int start_index, stream_index = newIndex;
	int old_index;
	//AVStream *st;

	if (codec_type == AVMEDIA_TYPE_VIDEO)
	{
		start_index = newIndex;//is->last_video_stream;
		old_index = is->video_stream;
	} 
	else if (codec_type == AVMEDIA_TYPE_AUDIO) 
	{
		start_index = newIndex;//is->last_audio_stream;
		old_index = is->audio_stream;
	} 
	else 
	{
		start_index = newIndex;//is->last_subtitle_stream;
		old_index = is->subtitle_stream;
	}
	stream_component_close(is, old_index);
	stream_component_open(is, stream_index);
	if (codec_type == AVMEDIA_TYPE_VIDEO)
		is->que_attachments_req = 1;
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

static void toggle_play(VideoState *is)
{
    if (is->paused)
    {
        stream_toggle_pause(is);
        is->step = 0;
    }
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
    
void ffmfc_play()
{
    SDL_Event event;
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym=SDLK_z;
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
	//int w=g_is->width;
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

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void ffmfc_change_audio_stream(int newStreamIndex)
{
	SDL_Event event;
	event.type = FFMFC_CHANGE_AUDIO;
	event.window.data1= newStreamIndex;
	SDL_PushEvent(&event);
}

void ffmfc_change_subtitle_stream(int newStreamIndex)
{
	SDL_Event event;
	event.type = FFMFC_CHANGE_SUBTITLE;
	event.window.data1= newStreamIndex;
	SDL_PushEvent(&event);
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void ffmfc_VolumeUp()
{
	if (volume < 100)
		volume += 10;

	SDL_Event event;
	event.type = FFMFC_VOLUME_UP_EVENT;
	event.window.data1= volume;
	SDL_PushEvent(&event);
}

int ffmfc_GetVolume()
{
	return volume;
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
	if (volume > 0)
		volume -= 10;

	SDL_Event event;
	event.type = FFMFC_VOLUME_DOWN_EVENT;
	event.window.data1= volume;
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
			case SDLK_p:
				//ÔÝÍ£
			case SDLK_SPACE:
				toggle_pause(cur_stream);
				break;
            case SDLK_z:
                toggle_play(cur_stream);
                break;
			case SDLK_s: // S: Step to next frame
				step_to_next_frame(cur_stream);
				break;
			case SDLK_a:
				{
					//stream_cycle_channel(cur_stream, AVMEDIA_TYPE_AUDIO);
					int newaudioIndex = event.window.data1;
					stream_change_stream(cur_stream, AVMEDIA_TYPE_AUDIO, newaudioIndex);
				}
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
		
        // case SDL_QUIT:
		case FF_QUIT_EVENT:
			do_exit(cur_stream);
			break;
		case FFMFC_CHANGE_AUDIO:
			{
				//toggle_pause(cur_stream);
				int newaudioIndex = event.window.data1;
				stream_change_stream(cur_stream, AVMEDIA_TYPE_AUDIO, newaudioIndex);
				//toggle_play(cur_stream);
			}
			break;
		case FFMFC_CHANGE_SUBTITLE:
			{
				//toggle_pause(cur_stream);
				int newSubtitleIndex = event.window.data1;
				stream_change_stream(cur_stream, AVMEDIA_TYPE_SUBTITLE, newSubtitleIndex);
				//toggle_play(cur_stream);
			}
			break;

		case FFMFC_VOLUME_UP_EVENT:
		case FFMFC_VOLUME_DOWN_EVENT:
			percentVolume = event.window.data1;
			break;
		case FFMFC_SET_POSITION:{
				int64_t ts = time_position;
				if(cur_stream->ic != nullptr)
				{
					if (cur_stream->ic->start_time != AV_NOPTS_VALUE)
						ts += cur_stream->ic->start_time;
					stream_seek(cur_stream, ts, 0, 0);
					stream_cycle_channel(cur_stream, AVMEDIA_TYPE_SUBTITLE);
				}
			}
			break;


		case FF_ALLOC_EVENT:
			alloc_picture((VideoState *)(event.user.data1));
			break;
		case FF_REFRESH_EVENT:
			{
				video_refresh(event.user.data1);
				//video_refresh_timer(event.user.data1);
				cur_stream->refresh = 0;
			}
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
int ffmfc_play(CVideoControlInterface * control, string filename)
{
	dlg = control;

	ffmfc_reset_index();
	//ÍË³ö·ûºÅÖÃÁã
	exit_remark=0;
	//int flags;
	//ºËÐÄ½á¹¹Ìå
	VideoState *is;

	//char dummy_videodriver[] = "SDL_VIDEODRIVER=dummy";
	input_filename = filename;

	if (display_disable) 
	{
		video_disable = 1;
	}

	autoexit=1;

	


#ifndef SDL2
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
#endif
	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
	SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

	if (av_lockmgr_register(lockmgr)) {
		do_exit(nullptr);
	}

	av_init_packet(&flush_pkt);
	flush_pkt.data = (uint8_t *)(intptr_t)"FLUSH";
	is = stream_open(filename.c_str(), file_iformat);
	if (!is) {
		do_exit(nullptr);
	}

	g_is = is;

	g_is->width = dlg->getWidth();
	g_is->height = dlg->getHeight();

    dlg->SetVideoStart();

	event_loop(is);
    
    g_is = nullptr;

	//wxCommandEvent event(EVENT_VIDEOSTOP);
    //wxPostEvent(dlg, event);

	return 0;
}

