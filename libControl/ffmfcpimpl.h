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
#include "libavutil/hwcontext.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libswresample/swresample.h"
#include "libavutil/display.h"
#include "libavutil/common.h"
#include "libavutil/fifo.h"
#include "libavutil/avstring.h"
#include "libavutil/pixdesc.h"
#include "libavfilter/buffersink.h"
#ifdef __APPLE__
#include "libavcodec/avcodec.h"
#include "libavcodec/videotoolbox.h"
#include "libavutil/imgutils.h"
#endif
}

#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
#define MIN_FRAMES 25
#define EXTERNAL_CLOCK_MIN_FRAMES 2
#define EXTERNAL_CLOCK_MAX_FRAMES 10

/* Minimum SDL audio buffer size, in samples. */
#define SDL_AUDIO_MIN_BUFFER_SIZE 512
/* Calculate actual buffer size keeping in mind not cause too frequent audio callbacks */
#define SDL_AUDIO_MAX_CALLBACKS_PER_SEC 30

/* Step size for volume control in dB */
#define SDL_VOLUME_STEP (0.75)

/* no AV sync correction is done if below the minimum AV sync threshold */
#define AV_SYNC_THRESHOLD_MIN 0.04
/* AV sync correction is done if above the maximum AV sync threshold */
#define AV_SYNC_THRESHOLD_MAX 0.1
/* If a frame duration is longer than this, it will not be duplicated to compensate AV sync */
#define AV_SYNC_FRAMEDUP_THRESHOLD 0.1
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

/* maximum audio speed change to get correct sync */
#define SAMPLE_CORRECTION_PERCENT_MAX 10

/* external clock speed adjustment constants for realtime sources based on buffer fullness */
#define EXTERNAL_CLOCK_SPEED_MIN  0.900
#define EXTERNAL_CLOCK_SPEED_MAX  1.010
#define EXTERNAL_CLOCK_SPEED_STEP 0.001

/* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
#define AUDIO_DIFF_AVG_NB   20

/* polls for possible required screen refresh at least this often, should be less than 1/fps */
#define REFRESH_RATE 0.01

/* NOTE: the size must be big enough to compensate the hardware audio buffersize size */
/* TODO: We assume that a decoded and resampled frame fits into this buffer */
#define SAMPLE_ARRAY_SIZE (8 * 65536)

#define CURSOR_HIDE_DELAY 1000000

#define USE_ONEPASS_SUBTITLE_RENDER 1

#define VIDEO_PICTURE_QUEUE_SIZE 3
#define SUBPICTURE_QUEUE_SIZE 16
#define SAMPLE_QUEUE_SIZE 9
#define FRAME_QUEUE_SIZE FFMAX(SAMPLE_QUEUE_SIZE, FFMAX(VIDEO_PICTURE_QUEUE_SIZE, SUBPICTURE_QUEUE_SIZE))

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

class CFFmfcPimpl
{
public:



	enum HWAccelID {
		HWACCEL_NONE = 0,
		HWACCEL_AUTO,
		HWACCEL_GENERIC,
		HWACCEL_VIDEOTOOLBOX,
		HWACCEL_QSV,
	};

	typedef struct HWAccel {
		const char* name;
		int (*init)(AVCodecContext* s);
		enum HWAccelID id;
		enum AVPixelFormat pix_fmt;
	} HWAccel;

	typedef struct HWDevice {
		const char* name;
		enum AVHWDeviceType type;
		AVBufferRef* device_ref;
	} HWDevice;

	typedef struct PacketQueue {
		AVFifoBuffer* pkt_list;
		int nb_packets;
		int size;
		int64_t duration;
		int abort_request;
		int serial;
		SDL_mutex* mutex;
		SDL_cond* cond;
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
		int64_t channel_layout;
		enum AVSampleFormat fmt;
		int frame_size;
		int bytes_per_sec;
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

	typedef struct Decoder {
		AVPacket* pkt;
		PacketQueue* queue;
		AVCodecContext* avctx;
		int pkt_serial;
		int finished;
		int packet_pending;
		SDL_cond* empty_queue_cond;
		int64_t start_pts;
		AVRational start_pts_tb;
		int64_t next_pts;
		AVRational next_pts_tb;
		SDL_Thread* decoder_tid;
	} Decoder;

	/* Common struct for handling all types of decoded data and allocated render buffers. */
	typedef struct Frame {
		AVFrame* frame;
		AVSubtitle sub;
		int serial;
		double pts;           /* presentation timestamp for the frame */
		double duration;      /* estimated duration of the frame */
		int64_t pos;          /* byte position of the frame in the input file */
		int width;
		int height;
		int format;
		AVRational sar;
		AVRational sample_aspect_ratio;
		int uploaded;
		int flip_v;
	} Frame;

	typedef struct Clock {
		double pts;           /* clock base */
		double pts_drift;     /* clock base minus time at which we updated the clock */
		double last_updated;
		double speed;
		int serial;           /* clock is based on a packet with this serial */
		int paused;
		int* queue_serial;    /* pointer to the current packet queue serial, used for obsolete clock detection */
	} Clock;


	typedef struct FrameQueue {
		Frame queue[FRAME_QUEUE_SIZE];
		int rindex;
		int windex;
		int size;
		int max_size;
		int keep_last;
		int rindex_shown;
		SDL_mutex* mutex;
		SDL_cond* cond;
		PacketQueue* pktq;
	} FrameQueue;

	typedef struct VideoState {

		int refresh = 0;
		std::thread* refresh_tid = nullptr;
		SDL_Thread* read_tid;
		AVInputFormat* iformat;
		int abort_request;
		int force_refresh;
		int paused;
		int last_paused;
		int queue_attachments_req;
		int seek_req;
		int seek_flags;
		int64_t seek_pos;
		int64_t seek_rel;
		int read_pause_return;
		AVFormatContext* ic;
		int realtime;

		Clock audclk;
		Clock vidclk;
		Clock extclk;

		FrameQueue pictq;
		FrameQueue subpq;
		FrameQueue sampq;

		Decoder auddec;
		Decoder viddec;
		Decoder subdec;

		int audio_stream;

		int av_sync_type;

		double audio_clock;
		int audio_clock_serial;
		double audio_diff_cum; /* used for AV difference average computation */
		double audio_diff_avg_coef;
		double audio_diff_threshold;
		int audio_diff_avg_count;
		AVStream* audio_st;
		PacketQueue audioq;
		int audio_hw_buf_size;
		uint8_t* audio_buf;
		uint8_t* audio_buf1;
		unsigned int audio_buf_size; /* in bytes */
		unsigned int audio_buf1_size;
		int audio_buf_index; /* in bytes */
		int audio_write_buf_size;
		int audio_volume;
		int muted;
        AudioParams audio_src;
#if CONFIG_AVFILTER
        AudioParams audio_filter_src;
#endif
        AudioParams audio_tgt;
        SwrContext* swr_ctx;
		int frame_drops_early;
		int frame_drops_late;

		enum ShowMode {
			SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
		} show_mode;

		int16_t sample_array[SAMPLE_ARRAY_SIZE];
		int sample_array_index;
		int last_i_start;
		RDFTContext* rdft;
		int rdft_bits;
		FFTSample* rdft_data;
		int xpos;
		double last_vis_time;

		int subtitle_stream;
		AVStream* subtitle_st;
		PacketQueue subtitleq;

		double frame_timer;
		double frame_last_returned_time;
		double frame_last_filter_delay;
		int video_stream;
		AVStream* video_st;
		PacketQueue videoq;
		double max_frame_duration;      // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity
		struct SwsContext* img_convert_ctx;
		struct SwsContext* sub_convert_ctx;
		int eof;

		char* filename;
		int width, height, xleft, ytop;
		int step;
		int last_video_stream, last_audio_stream, last_subtitle_stream;

		SDL_cond* continue_read_thread;

		CFFmfcPimpl * _pimpl;


		/* hwaccel options */
		HWAccelID hwaccel_id = HWACCEL_AUTO;
		AVHWDeviceType hwaccel_device_type;
		char* hwaccel_device;
		AVPixelFormat hwaccel_output_format;

		/* hwaccel context */
		void* hwaccel_ctx;
		void (*hwaccel_uninit)(AVCodecContext* s);
		int  (*hwaccel_get_buffer)(AVCodecContext* s, AVFrame* frame, int flags);
		int  (*hwaccel_retrieve_data)(AVCodecContext* s, AVFrame* frame);
		AVPixelFormat hwaccel_pix_fmt;
		AVPixelFormat hwaccel_retrieved_pix_fmt;
		AVBufferRef* hw_frames_ctx;
		AVCodecContext* avctx;
		AVCodec* codec;
	} VideoState;




	enum ShowMode {
		SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
	};
	//Ö¸ÏòMFC´°¿ÚµÄÖ¸Õë

	typedef struct MyAVPacketList {
		AVPacket* pkt;
		int serial;
	} MyAVPacketList;

	CFFmfcPimpl()
	{
	}

	~CFFmfcPimpl()
	{

	}


	static int refresh_thread(void* opaque);

	void video_refresh(void* opaque, double* remaining_time);

	double vp_duration(VideoState* is, Frame* vp, Frame* nextvp);

	void video_image_display(VideoState* is);

	void frame_queue_unref_item(Frame* vp);

	int frame_queue_init(FrameQueue* f, PacketQueue* pktq, int max_size, int keep_last);

	void frame_queue_destory(FrameQueue* f);

	void frame_queue_signal(FrameQueue* f);

	Frame* frame_queue_peek(FrameQueue* f);

	Frame* frame_queue_peek_next(FrameQueue* f);

	Frame* frame_queue_peek_last(FrameQueue* f);

	Frame* frame_queue_peek_writable(FrameQueue* f);

	Frame* frame_queue_peek_readable(FrameQueue* f);

	void frame_queue_push(FrameQueue* f);

	void frame_queue_next(FrameQueue* f);

	/* return the number of undisplayed frames in the queue */
	int frame_queue_nb_remaining(FrameQueue* f);

	/* return last shown position */
	int64_t frame_queue_last_pos(FrameQueue* f);
	void decoder_abort(Decoder* d, FrameQueue* fq);
	int decoder_start(Decoder* d, int (*fn)(void*), const char* thread_name, void* arg);

	int get_master_sync_type(VideoState* is);
	
	int hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type);
	static enum AVPixelFormat get_format(AVCodecContext* s, const enum AVPixelFormat* pix_fmts);
	//static enum AVPixelFormat get_hw_format(AVCodecContext *s, const AVPixelFormat *pix_fmts);
	static int get_buffer(AVCodecContext* s, AVFrame* frame, int flags);
	//Calcul du pourcentage
	void StopStream();
	int percentageToDb(int p, int maxValue);
	int packet_queue_put_private(PacketQueue *q, AVPacket *pkt);
	int packet_queue_put(PacketQueue *q, AVPacket *pkt);

	/* packet queue handling */
	int packet_queue_init(PacketQueue *q);
	void packet_queue_flush(PacketQueue *q);

	void packet_queue_destroy(PacketQueue *q);

	void packet_queue_abort(PacketQueue *q);

	void packet_queue_start(PacketQueue *q);

	enum AVPixelFormat find_fmt_by_hw_type(const enum AVHWDeviceType type);

	int packet_queue_put_nullpacket(PacketQueue* q, AVPacket* pkt, int stream_index);

	int GetPosition(VideoState* is);

	int stream_has_enough_packets(AVStream* st, int stream_id, PacketQueue* queue);

	/* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
	int packet_queue_get(PacketQueue* q, AVPacket* pkt, int block, int* serial);

	static void free_subpicture(SubPicture *sp);

	void stream_close(VideoState *is);

	void do_exit(VideoState *is);

	/* display the current picture, if any */
	void video_display(VideoState *is);

	void CopyFrameToDest(AVFrame * frame);

	/* get the current audio clock value */
	//double get_audio_clock(VideoState *is);

	/* get the current video clock value */
	//double get_video_clock(VideoState *is);

	/* get the current external clock value */
	//double get_external_clock(VideoState *is);

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

	void check_external_clock_speed(VideoState* is);

	void update_video_pts(VideoState* is, double pts, int64_t pos, int serial);

	void sync_clock_to_slave(Clock* c, Clock* slave);

	//½âÂë³É¹¦ºóÓÃÓÚÏÔÊ¾£¬Ò²ÊÇ·Åµ½ÁíÒ»¸ö¶ÓÁÐÖÐ£¿
	int queue_picture(VideoState* is, AVFrame* src_frame, double pts, double duration, int64_t pos, int serial);

	//½âÂëÒ»Ö¡ÊÓÆµ
	int get_video_frame(VideoState* is, AVFrame* frame);

	void set_clock_at(Clock* c, double pts, int serial, double time);

	void set_clock(Clock* c, double pts, int serial);

	void init_clock(Clock* c, int* queue_serial);

	void set_clock_speed(Clock* c, double speed);
	double get_clock(Clock* c);

	//½âÂëÊÓÆµ
	static int audio_thread(void* arg);
	static int video_thread(void *arg);
	static int subtitle_thread(void *arg);
	//static int refresh_thread(void *opaque);
	/* prepare a new audio buffer */
	static void sdl_audio_callback(void *opaque, Uint8 *stream, int len);

	//²¥·ÅÒôÆµµÄÊ±ºòSDLµÄÏÔÊ¾
	/* copy samples for viewing in editor window */
	void update_sample_display(VideoState *is, short *samples, int samples_size);

	/* return the wanted number of samples to get better sync if sync_type is video
	* or external master clock */
	int synchronize_audio(VideoState *is, int nb_samples);

	/* decode one audio frame and returns its uncompressed size */
	int audio_decode_frame(VideoState *is);
	   
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

	int is_realtime(AVFormatContext *s, char * filename);
	
	/* this thread gets the stream from the disk or the network */
	//½âÂëÏß³Ì£¬»ñµÃÊÓÒôÆµPacket²¢·ÅÈë¶ÓÁÐ
	static int read_thread(void *arg);

	//ÒÔÏÂ¼¸¸öº¯Êý¶¼ÊÇ´¦Àíevent_loop()ÖÐµÄ¸÷ÖÖ²Ù×÷µÄ
	void stream_cycle_channel(VideoState *is, int codec_type);

	int decoder_decode_frame(VideoState* is, Decoder* d, AVFrame* frame, AVSubtitle* sub);
	int decoder_init(Decoder* d, AVCodecContext* avctx, PacketQueue* queue, SDL_cond* empty_queue_cond);
	void decoder_destroy(Decoder* d);
	//-------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------
	void stream_change_stream(VideoState *is, int codec_type, int newIndex);

	void step_to_next_frame(VideoState *is);

	AVFrame * CopyFrame(AVFrame * frame);

	AVBufferRef* hw_device_ctx = NULL;
	static enum AVPixelFormat hw_pix_fmt;

	//SOUND Volume
	int muted = 0;
	int percentVolume = 100;
	int volume = 100;
	SDL_AudioDeviceID audio_dev;
	//CffplaymfcDlg * dlg;
	static CVideoControlInterface * dlg;
	VideoState *g_is = nullptr;
	/* options specified by the user */
	AVInputFormat *file_iformat = nullptr;
	string input_filename;
	int screen_width = 0;
	int screen_height = 0;
	int audio_disable = 0;
	int disable_framedelay = 0;
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
	int subtitle_disable = 0;
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
	//enum ShowMode show_mode = SHOW_MODE_NONE;
	const char *audio_codec_name = 0;
	const char *subtitle_codec_name = 0;
	const char *video_codec_name = 0;
	int rdftspeed = 20;

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
	wxString acceleratorHardware = "d3d11va";
#elif defined(__APPLE__)
	wxString acceleratorHardware = "videotoolbox";
#else
	wxString acceleratorHardware = "cuda";
#endif
	bool isOpenGLDecoding = false;
	int find_stream_info = 1;
#ifndef CMDUTILS
	AVDictionary *format_opts = nullptr, *codec_opts = nullptr;
#endif
	const char* wanted_stream_spec[AVMEDIA_TYPE_NB] = { 0 };


//------------------------------------------------------------------------
//Hardware Accelerator
//------------------------------------------------------------------------

	static int hwaccel_retrieve_data(AVCodecContext* avctx, AVFrame* input);
	int hwaccel_decode_init(AVCodecContext* avctx);
	static enum AVPixelFormat get_hw_format(AVCodecContext* ctx, const enum AVPixelFormat* pix_fmts);

};