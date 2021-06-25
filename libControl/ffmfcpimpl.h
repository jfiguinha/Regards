#pragma once
#include <header.h>
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
#include <SDL.h>
#include <stdio.h>
#include <SDL_thread.h>
#include <SDL_mutex.h>
#include <Interpolation.h>
#include <RegardsBitmap.h>
#include "VideoControlInterface.h"
#include <thread>
using namespace std;

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




class CFFmfcPimpl;

//-----------------------------------------------------------------------------------------
//Define
//-----------------------------------------------------------------------------------------

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

#define MAX_FRAME_NUM 10000
#define MAX_PACKET_NUM 10000
#define MAX_URL_LENGTH 500

#define VIDEO_PICTURE_QUEUE_SIZE 4
#define SUBPICTURE_QUEUE_SIZE 4

//Event

#define FF_ALLOC_EVENT   (SDL_USEREVENT)
#define FF_REFRESH_EVENT (SDL_USEREVENT + 1)
#define FF_QUIT_EVENT    4096
#define FF_STEP_EVENT    FF_QUIT_EVENT + 1
#define FF_PAUSE_EVENT	FF_QUIT_EVENT + 2
#define FF_PLAY_EVENT	FF_QUIT_EVENT + 3
#define FF_ASPECT_EVENT	FF_QUIT_EVENT + 4
#define FF_AUDIODISPLAY_EVENT	FF_QUIT_EVENT + 5
#define FF_EXIT_EVENT FF_QUIT_EVENT + 6
#define CLOSESTREAM_EVENT FF_QUIT_EVENT + 7
#define FF_STOP_EVENT FF_QUIT_EVENT + 8

//×Ô¶¨ÒåÒ»¸öÊÂ¼þ£¬ÓÃÓÚµ÷Õû²¥·Å½ø¶È
#define SEEK_BAR_EVENT    (SDL_USEREVENT + 4)
#define VOLUME_UP_EVENT    (SDL_USEREVENT + 5)
#define VOLUME_DOWN_EVENT    (SDL_USEREVENT + 6)
#define SET_POSITION (SDL_USEREVENT + 7)
#define CHANGE_AUDIO (SDL_USEREVENT + 8)
#define CHANGE_SUBTITLE (SDL_USEREVENT + 9)
#define VOLUME_EVENT  (SDL_USEREVENT + 10)
#define SET_SEEKPOSITION (SDL_USEREVENT + 11)

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



class CFFmfcPimpl
{
public:



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
		int(*url_open)(URLContext *h, const char *url, int flags);
		int(*url_read)(URLContext *h, unsigned char *buf, int size);
		int(*url_write)(URLContext *h, const unsigned char *buf, int size);
		int64_t(*url_seek)(URLContext *h, int64_t pos, int whence);
		int(*url_close)(URLContext *h);
		struct URLProtocol *next;
		int(*url_read_pause)(URLContext *h, int pause);
		int64_t(*url_read_seek)(URLContext *h, int stream_index,
			int64_t timestamp, int flags);
		int(*url_get_file_handle)(URLContext *h);
		int priv_data_size;
		const AVClass *priv_data_class;
		int flags;
		int(*url_check)(URLContext *h, int mask);
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
		std::thread *read_tid = nullptr;
		std::thread *video_tid = nullptr;
		std::thread *refresh_tid = nullptr;
		std::thread *subtitle_tid = nullptr;

		AVCodecContext * videoCtx = nullptr;
		AVCodecContext * audioCtx = nullptr;
		AVCodecContext * subtitleCtx = nullptr;
		AVInputFormat *iformat = nullptr;
		int no_background = 0;
		int abort_request = 0;
		int force_refresh = 0;
		int paused = 0;
		int last_paused = 0;
		int que_attachments_req = 0;
		int seek_req = 0;
		int seek_flags = 0;
		int64_t seek_pos = 0;
		int64_t seek_rel = 0;
		int read_pause_return = 0;
		AVFormatContext *ic = nullptr;

		int audio_stream = 0;

		int av_sync_type = 0;
		double external_clock = 0; /* external clock base */
		int64_t external_clock_time = 0;

		double audio_clock = 0;
		double audio_diff_cum = 0; /* used for AV difference average computation */
		double audio_diff_avg_coef = 0;
		double audio_diff_threshold = 0;
		int audio_diff_avg_count = 0;
		AVStream *audio_st = nullptr;
		PacketQueue audioq;
		int audio_hw_buf_size = 0;
		DECLARE_ALIGNED(16, uint8_t, audio_buf2)[AVCODEC_MAX_AUDIO_FRAME_SIZE * 4];
		uint8_t silence_buf[SDL_AUDIO_BUFFER_SIZE];
		uint8_t *audio_buf  = nullptr;
		uint8_t *audio_buf1 = nullptr;
		unsigned int audio_buf_size = 3565; /* in bytes */
		int audio_buf_index = 983040; /* in bytes */
		int audio_write_buf_size = 8560;
		AVPacket audio_pkt_temp;
		AVPacket audio_pkt;
		AudioParams audio_src;
		AudioParams audio_tgt;
		SwrContext *swr_ctx;
		double audio_current_pts = 0;
		double audio_current_pts_drift = 0;
		int frame_drops_early = 7920;
		int frame_drops_late = 0;
		double frame_last_delay = 0;
		AVFrame *frame = nullptr;

		enum ShowMode {
			SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
		} show_mode;
		int16_t sample_array[SAMPLE_ARRAY_SIZE];
		int sample_array_index = 0;
		int last_i_start = 0;
		RDFTContext *rdft = nullptr;
		int rdft_bits = 0;
		FFTSample *rdft_data = nullptr;
		int xpos = 0;

		
		int subtitle_stream = 0;
		int subtitle_stream_changed = 0;
		AVStream *subtitle_st = nullptr;
		PacketQueue subtitleq;
		SubPicture subpq[SUBPICTURE_QUEUE_SIZE];
		int subpq_size = 0, subpq_rindex = 0, subpq_windex = 0;
		SDL_mutex *subpq_mutex = nullptr;
		SDL_cond *subpq_cond = nullptr;

		double frame_timer = 0;
		double frame_last_pts = 0;
		double frame_last_duration = 0;
		double frame_last_dropped_pts = 0;
		double frame_last_returned_time = 0;
		double frame_last_filter_delay = 0;
		int64_t frame_last_dropped_pos = 0;
		double video_clock = 0;                          ///< pts of last decoded frame / predicted pts of next decoded frame
		int video_stream = 0;
		AVStream *video_st = nullptr;
		PacketQueue videoq;
		double video_current_pts = 0;                    ///< current displayed pts (different from video_clock if frame fifos are used)
		double video_current_pts_drift = 0;              ///< video_current_pts - time (av_gettime) at which we updated video_current_pts - used to have running video pts
		int64_t video_current_pos = 0;                   ///< current displayed file pos
		VideoPicture pictq[VIDEO_PICTURE_QUEUE_SIZE];
		int pictq_size = 0, pictq_rindex = 0, pictq_windex = 0;
		SDL_mutex *pictq_mutex = nullptr;
		SDL_cond *pictq_cond = nullptr;
		char filename[1024];
		int width = 0, height = 0, xleft = 0, ytop = 0;
		int step = 0;
		int refresh = 0;
		int last_video_stream = 0, last_audio_stream = 0, last_subtitle_stream = 0;

		SDL_cond *continue_read_thread;
		//ÊÓÆµÏÔÊ¾·½Ê½------------
		enum V_Show_Mode v_show_mode;

		CFFmfcPimpl * _pimpl;
	} VideoState;



	enum ShowMode {
		SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
	};
	//Ö¸ÏòMFC´°¿ÚµÄÖ¸Õë



	CFFmfcPimpl()
	{
	}

	~CFFmfcPimpl()
	{

	}

	int hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type);
	//static AVPixelFormat GetHwFormat(AVCodecContext *s, const AVPixelFormat *pix_fmts);
	static enum AVPixelFormat get_hw_format(AVCodecContext *ctx, const enum AVPixelFormat *pix_fmts);
	static enum AVPixelFormat GetHwFormat(AVCodecContext *s, const AVPixelFormat *pix_fmts);
	//Calcul du pourcentage
	void StopStream();
	int percentageToDb(int p, int maxValue);
	int packet_queue_put_private(PacketQueue *q, AVPacket *pkt);
	int packet_queue_put(PacketQueue *q, AVPacket *pkt);

	/* packet queue handling */
	void packet_queue_init(PacketQueue *q);
	void packet_queue_flush(PacketQueue *q);

	void packet_queue_destroy(PacketQueue *q);

	void packet_queue_abort(PacketQueue *q);

	void packet_queue_start(PacketQueue *q);

	int GetPosition(VideoState* is);

	/* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
	int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block);

	static void free_subpicture(SubPicture *sp);
	
	void video_audio_display(VideoState *s) const;

	void stream_close(VideoState *is);

	void do_exit(VideoState *is);

	/* display the current picture, if any */
	void video_display(VideoState *is);

	void CloseStream(VideoState *is);

	void CopyFrameToDest(AVFrame * frame);

	/* get the current audio clock value */
	double get_audio_clock(VideoState *is);

	/* get the current video clock value */
	double get_video_clock(VideoState *is);

	/* get the current external clock value */
	double get_external_clock(VideoState *is);

	/* get the current master clock value */
	double get_master_clock(VideoState *is);
	/* seek in the stream */
	void stream_seek(VideoState *is, int64_t pos, int64_t rel, int seek_by_bytes);

	VideoState * stream_open(const char *filename, AVInputFormat *iformat);

	/* pause or resume the video */
	void stream_toggle_pause(VideoState *is);

	void toggle_pause(VideoState *is);

	void toggle_play(VideoState *is);

	double compute_target_delay(double delay, VideoState *is);

	void pictq_next_picture(VideoState *is);

	void pictq_prev_picture(VideoState *is);

	void update_video_pts(VideoState *is, double pts, int64_t pos);

	void video_refresh(VideoState *is);

	/* allocate a picture (needs to do that in main thread to avoid
	potential locking problems */
	void alloc_picture(VideoState *is);

	//½âÂë³É¹¦ºóÓÃÓÚÏÔÊ¾£¬Ò²ÊÇ·Åµ½ÁíÒ»¸ö¶ÓÁÐÖÐ£¿
	int queue_picture(VideoState *is, AVFrame *src_frame, double pts1, int64_t pos);

	//½âÂëÒ»Ö¡ÊÓÆµ
	int get_video_frame(VideoState *is, AVFrame *frame, int64_t *pts, AVPacket *pkt, bool & frame_destination);

	//½âÂëÊÓÆµ
	static int video_thread(void *arg);
	static int subtitle_thread(void *arg);
	static int refresh_thread(void *opaque);
	/* prepare a new audio buffer */
	static void sdl_audio_callback(void *opaque, Uint8 *stream, int len);

	//²¥·ÅÒôÆµµÄÊ±ºòSDLµÄÏÔÊ¾
	/* copy samples for viewing in editor window */
	void update_sample_display(VideoState *is, short *samples, int samples_size);

	/* return the wanted number of samples to get better sync if sync_type is video
	* or external master clock */
	int synchronize_audio(VideoState *is, int nb_samples);

	/* decode one audio frame and returns its uncompressed size */
	int audio_decode_frame(VideoState *is, double *pts_ptr);
	   
	int audio_open(void *opaque, int64_t wanted_channel_layout, int wanted_nb_channels, int wanted_sample_rate, AudioParams *audio_hw_params);

#ifndef CMDUTILS

	int check_stream_specifier(AVFormatContext *s, AVStream *st, const char *spec);

	AVDictionary *filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
		AVFormatContext *s, AVStream *st, AVCodec *codec);

	void uninit_opts(void);

	AVDictionary **setup_find_stream_info_opts(AVFormatContext *s,
		AVDictionary *codec_opts);
	
#endif

	const char *getExt(const char *fspec);

	/* open a given stream. Return 0 if OK */
	//´ò¿ªÒ»¸öStream£¬ÊÓÆµ»òÒôÆµ
	int stream_component_open(VideoState *is, int stream_index);

	void stream_component_close(VideoState *is, int stream_index);

	static int decode_interrupt_cb(void *ctx);

	int is_realtime(AVFormatContext *s);
	
	/* this thread gets the stream from the disk or the network */
	//½âÂëÏß³Ì£¬»ñµÃÊÓÒôÆµPacket²¢·ÅÈë¶ÓÁÐ
	static int read_thread(void *arg);

	//ÒÔÏÂ¼¸¸öº¯Êý¶¼ÊÇ´¦Àíevent_loop()ÖÐµÄ¸÷ÖÖ²Ù×÷µÄ
	void stream_cycle_channel(VideoState *is, int codec_type);

	//-------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------
	void stream_change_stream(VideoState *is, int codec_type, int newIndex);

	void step_to_next_frame(VideoState *is);

	void toggle_audio_display(VideoState *is, int mode);

	AVFrame * CopyFrame(AVFrame * frame);

	static int lockmgr(void **mtx, enum AVLockOp op);

	//SOUND Volume
	int percentVolume = 100;
	int volume = 100;

	//CffplaymfcDlg * dlg;
	static CVideoControlInterface * dlg;
	VideoState *g_is = nullptr;
	/* options specified by the user */
	AVInputFormat *file_iformat = nullptr;
	string input_filename;
	int screen_width = 0;
	int screen_height = 0;
	int audio_disable = 0;
	int video_disable = 0;
	int wanted_stream[AVMEDIA_TYPE_NB] = { -1,-1,0,-1,0 };
	int seek_by_bytes = 0;
	int display_disable = 0;
	int show_status = 0;
	int av_sync_type = AV_SYNC_AUDIO_MASTER;
	int64_t start_time = AV_NOPTS_VALUE;
	int64_t duration = AV_NOPTS_VALUE;
	int workaround_bugs = 1;
	int fast = 0;
	int genpts = 0;
	int lowres = 0;
	int idct = FF_IDCT_AUTO;
	enum AVDiscard skip_frame = AVDISCARD_DEFAULT;
	enum AVDiscard skip_idct = AVDISCARD_DEFAULT;
	enum AVDiscard skip_loop_filter = AVDISCARD_DEFAULT;
	int error_concealment = 3;
	int decoder_reorder_pts = -1;
	int autoexit = 0;
	int exit_on_keydown = 0;
	int exit_on_mousedown = 0;
	int loop = 1;
	int framedrop = -1;
	int infinite_buffer = -1;
	enum ShowMode show_mode = SHOW_MODE_NONE;
	const char *audio_codec_name = 0;
	const char *subtitle_codec_name = 0;
	const char *video_codec_name = 0;
	int rdftspeed = 20;

	AVBufferRef *hw_device_ctx = NULL;
	static enum AVPixelFormat hw_pix_fmt;
	wxWindow * parent = nullptr;
	/* current context */
	int64_t audio_callback_time = 0;

	AVPacket flush_pkt;


	int seek_bar_pos = 0;
	int64_t time_position = 0;
	int video_angle = 0;
	int video_flipV = 0;
	int video_flipH = 0;
	//×¨ÃÅÉèÖÃµÄ±ê¼Ç£¬ÔÚ³ÌÐò½«ÒªÍË³öµÄÊ±ºò»áÖÃ1
	int exit_remark = 0;

	//---------------------------------
	//static SDL_Surface *screen;
	int vframe_index = 0;
	int aframe_index = 0;
	int packet_index = 0;
	int videoOutputMode = 24;
	bool first = true;
	AVFrame * dst = av_frame_alloc();
	SwsContext* scaleContext = nullptr;
#ifdef WIN32
	wxString acceleratorHardware = "dxva2";
#else
	wxString acceleratorHardware = "";
#endif
	bool isOpenGLDecoding = false;

#ifndef CMDUTILS
	AVDictionary *format_opts = nullptr, *codec_opts = nullptr;
#endif

};