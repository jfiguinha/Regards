#pragma once

#include "config.h"
#include "config_components.h"
#include <inttypes.h>
#include <math.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>

extern "C"
{
    #include "libavutil/avstring.h"
    #include "libavcodec/packet.h"
    #include "libavutil/channel_layout.h"
    #include "libavutil/eval.h"
    #include "libavutil/mathematics.h"
    #include "libavutil/pixdesc.h"
    #include "libavutil/imgutils.h"
    #include "libavutil/dict.h"
    #include "libavutil/fifo.h"
    #include "libavutil/parseutils.h"
    #include "libavutil/samplefmt.h"
    #include "libavutil/time.h"
    #include "libavutil/bprint.h"
    #include "libavformat/avformat.h"
    #include "libavdevice/avdevice.h"
    #include "libswscale/swscale.h"
    #include "libavutil/opt.h"
    #include "libavutil/tx.h"
    #include "libswresample/swresample.h"
    #include "libavcodec/avcodec.h"
    #include "libavfilter/avfilter.h"
    #include "libavfilter/buffersink.h"
    #include "libavfilter/buffersrc.h"
    #include "libavutil/display.h"
    #include "libavutil/common.h"
}

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <string>
using namespace std;

#define VIDEO_PICTURE_QUEUE_SIZE 3
#define SUBPICTURE_QUEUE_SIZE 16
#define SAMPLE_QUEUE_SIZE 9
#define FRAME_QUEUE_SIZE FFMAX(SAMPLE_QUEUE_SIZE, FFMAX(VIDEO_PICTURE_QUEUE_SIZE, SUBPICTURE_QUEUE_SIZE))

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



class CFFPlay
{
public:



    CFFPlay();
    ~CFFPlay();
    int PlayMovie(const std::string& filename);
   
private:


    enum ShowMode { SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB };


    struct MyAVPacketList {
        AVPacket* pkt;
        int serial;
    };

    struct PacketQueue {
        AVFifo* pkt_list;
        int nb_packets;
        int size;
        int64_t duration;
        int abort_request;
        int serial;
        SDL_mutex* mutex;
        SDL_cond* cond;
    };



    struct AudioParams {
        int freq;
        AVChannelLayout ch_layout;
        enum AVSampleFormat fmt;
        int frame_size;
        int bytes_per_sec;
    };

    struct Clock {
        double pts;           /* clock base */
        double pts_drift;     /* clock base minus time at which we updated the clock */
        double last_updated;
        double speed;
        int serial;           /* clock is based on a packet with this serial */
        int paused;
        int* queue_serial;    /* pointer to the current packet queue serial, used for obsolete clock detection */
    };

    struct FrameData {
        int64_t pkt_pos;
    };

    /* Common struct for handling all types of decoded data and allocated render buffers. */
    struct Frame {
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
        int uploaded;
        bool flip_v;
    };

    struct FrameQueue {
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
    };

    enum {
        AV_SYNC_AUDIO_MASTER, /* default choice */
        AV_SYNC_VIDEO_MASTER,
        AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
    };

    struct Decoder {
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
    };

    struct VideoState {
        SDL_Thread* read_tid;
        const AVInputFormat* iformat;
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
        struct AudioParams audio_src;
        struct AudioParams audio_filter_src;
        struct AudioParams audio_tgt;
        struct SwrContext* swr_ctx;
        int frame_drops_early;
        int frame_drops_late;

        ShowMode show_mode;
        int16_t sample_array[SAMPLE_ARRAY_SIZE];
        int sample_array_index;
        int last_i_start;
        AVTXContext* rdft;
        av_tx_fn rdft_fn;
        int rdft_bits;
        float* real_data;
        AVComplexFloat* rdft_data;
        int xpos;
        double last_vis_time;
        SDL_Texture* vis_texture;
        SDL_Texture* sub_texture;
        SDL_Texture* vid_texture;

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
        struct SwsContext* sub_convert_ctx;
        int eof;

        std::string filename;
        int width, height, xleft, ytop;
        int step;

        int vfilter_idx;
        AVFilterContext* in_video_filter;   // the first filter in the video chain
        AVFilterContext* out_video_filter;  // the last filter in the video chain
        AVFilterContext* in_audio_filter;   // the first filter in the audio chain
        AVFilterContext* out_audio_filter;  // the last filter in the audio chain
        AVFilterGraph* agraph;              // audio filter graph

        int last_video_stream, last_audio_stream, last_subtitle_stream;

        SDL_cond* continue_read_thread;

        CFFPlay* ffplay = nullptr;
    };
   

    int video_open(CFFPlay::VideoState* is);

    int cmp_audio_fmts(enum AVSampleFormat fmt1, int64_t channel_count1,
        enum AVSampleFormat fmt2, int64_t channel_count2);

    void video_display(CFFPlay::VideoState* is);
    double get_clock(CFFPlay::Clock* c);
    int packet_queue_put_private(CFFPlay::PacketQueue* q, AVPacket* pkt);
    int packet_queue_put(CFFPlay::PacketQueue* q, AVPacket* pkt);
    int packet_queue_put_nullpacket(CFFPlay::PacketQueue* q, AVPacket* pkt, int stream_index);
    int packet_queue_init(CFFPlay::PacketQueue* q);
    void packet_queue_flush(CFFPlay::PacketQueue* q);
    void packet_queue_destroy(CFFPlay::PacketQueue* q);
    void packet_queue_abort(CFFPlay::PacketQueue* q);
    void packet_queue_start(CFFPlay::PacketQueue* q);
    int packet_queue_get(PacketQueue* q, AVPacket* pkt, int block, int* serial);
    int decoder_init(CFFPlay::Decoder* d, AVCodecContext* avctx, PacketQueue* queue, SDL_cond* empty_queue_cond);
    int decoder_decode_frame(CFFPlay::Decoder* d, AVFrame* frame, AVSubtitle* sub);
    int frame_queue_init(CFFPlay::FrameQueue* f, CFFPlay::PacketQueue* pktq, int max_size, int keep_last);
    void decoder_destroy(CFFPlay::Decoder* d);
    void frame_queue_unref_item(Frame* vp);
    void frame_queue_destroy(CFFPlay::FrameQueue* f);
    void frame_queue_signal(CFFPlay::FrameQueue* f);
    CFFPlay::Frame* frame_queue_peek(CFFPlay::FrameQueue* f);
    CFFPlay::Frame* frame_queue_peek_next(CFFPlay::FrameQueue* f);
    CFFPlay::Frame* frame_queue_peek_last(CFFPlay::FrameQueue* f);
    CFFPlay::Frame* frame_queue_peek_writable(CFFPlay::FrameQueue* f);
    CFFPlay::Frame* frame_queue_peek_readable(CFFPlay::FrameQueue* f);
    void frame_queue_push(CFFPlay::FrameQueue* f);
    void frame_queue_next(CFFPlay::FrameQueue* f);
    int frame_queue_nb_remaining(CFFPlay::FrameQueue* f);
    int64_t frame_queue_last_pos(CFFPlay::FrameQueue* f);
    void decoder_abort(CFFPlay::Decoder* d, CFFPlay::FrameQueue* fq);

    //SDL Function
    void fill_rectangle(int x, int y, int w, int h);
    int realloc_texture(SDL_Texture** texture, Uint32 new_format, int new_width, int new_height, SDL_BlendMode blendmode, int init_texture);

    void calculate_display_rect(SDL_Rect* rect,
        int scr_xleft, int scr_ytop, int scr_width, int scr_height,
        int pic_width, int pic_height, AVRational pic_sar);

    void get_sdl_pix_fmt_and_blendmode(int format, Uint32* sdl_pix_fmt, SDL_BlendMode* sdl_blendmode);
    int  upload_texture(SDL_Texture** tex, AVFrame* frame);
    void set_sdl_yuv_conversion_mode(AVFrame* frame);
    void video_image_display(VideoState* is);

    void toggle_audio_display(VideoState* is);
    void toggle_full_screen(VideoState* is);

    int compute_mod(int a, int b);
    void video_audio_display(VideoState* s);
    void set_default_window_size(int width, int height, AVRational sar);
    //----
    int stream_component_open(VideoState* is, int stream_index);
   
    VideoState* stream_open(std::string filename, const AVInputFormat* iformat);
    void stream_cycle_channel(VideoState* is, int codec_type);
    void stream_component_close(VideoState* is, int stream_index);
    void stream_close(VideoState* is);
    void do_exit(VideoState* is);

    double get_master_clock(VideoState* is);
    void refresh_loop_wait_event(VideoState* is, SDL_Event* event);
    void seek_chapter(VideoState* is, int incr);
    void event_loop(VideoState* cur_stream);
    int get_master_sync_type(VideoState* is);
    void sync_clock_to_slave(Clock* c, Clock* slave);
    void init_clock(Clock* c, int* queue_serial);
    void set_clock_speed(Clock* c, double speed);
    void set_clock(Clock* c, double pts, int serial);
    void set_clock_at(Clock* c, double pts, int serial, double time);
    void check_external_clock_speed(VideoState* is);
    void stream_seek(VideoState* is, int64_t pos, int64_t rel, int by_bytes);

    void video_refresh(void* opaque, double* remaining_time);
    void update_video_pts(VideoState* is, double pts, int serial);
    double vp_duration(VideoState* is, Frame* vp, Frame* nextvp);
    double compute_target_delay(double delay, VideoState* is);
    void step_to_next_frame(VideoState* is);
    void toggle_pause(VideoState* is);
    void toggle_mute(VideoState* is);
    void update_volume(VideoState* is, int sign, double step);
    void stream_toggle_pause(VideoState* is);
    int queue_picture(VideoState* is, AVFrame* src_frame, double pts, double duration, int64_t pos, int serial);
    int get_video_frame(VideoState* is, AVFrame* frame);
    int configure_filtergraph(AVFilterGraph* graph, const char* filtergraph,
        AVFilterContext* source_ctx, AVFilterContext* sink_ctx);
    int configure_video_filters(AVFilterGraph* graph, VideoState* is, const char* vfilters, AVFrame* frame);
    int configure_audio_filters(VideoState* is, const char* afilters, int force_output_format);

    static int audio_thread(void* arg);
    static int video_thread(void* arg);
    static int subtitle_thread(void* arg);
    static int decode_interrupt_cb(void* ctx);
    static void sdl_audio_callback(void* opaque, Uint8* stream, int len);
    static int read_thread(void* arg);


    int decoder_start(Decoder* d, int (*fn)(void*), const char* thread_name, void* arg);
    void update_sample_display(VideoState* is, short* samples, int samples_size);
    int synchronize_audio(VideoState* is, int nb_samples);
    int audio_decode_frame(VideoState* is);
    
    int is_realtime(AVFormatContext* s);
    int stream_has_enough_packets(AVStream* st, int stream_id, PacketQueue* queue);
    int audio_open(void* opaque, AVChannelLayout* wanted_channel_layout, int wanted_sample_rate, struct AudioParams* audio_hw_params);

    AVDictionary** setup_find_stream_info_opts(AVFormatContext* s, AVDictionary* codec_opts);
    void uninit_opts(void);

    AVDictionary* filter_codec_opts(AVDictionary* opts, enum AVCodecID codec_id,
        AVFormatContext* s, AVStream* st, AVCodec* codec);
    int check_stream_specifier(AVFormatContext* s, AVStream* st, const char* spec);

    /* options specified by the user */
    const AVInputFormat* file_iformat;
    std::string input_filename;
    std::string window_title;
    int default_width = 640;
    int default_height = 480;
    int screen_width = 0;
    int screen_height = 0;
    int screen_left = SDL_WINDOWPOS_CENTERED;
    int screen_top = SDL_WINDOWPOS_CENTERED;
    bool audio_disable = false;
    bool video_disable = false;
    bool subtitle_disable = false;
    const char* wanted_stream_spec[AVMEDIA_TYPE_NB] = { 0 };
    int seek_by_bytes = -1;
    float seek_interval = 10;
    int display_disable;
    int borderless;
    int alwaysontop;
    int startup_volume = 100;
    int show_status = -1;
    int av_sync_type = AV_SYNC_AUDIO_MASTER;
    int64_t start_time = AV_NOPTS_VALUE;
    int64_t duration = AV_NOPTS_VALUE;
    int fast = 0;
    int lowres = 0;
    int decoder_reorder_pts = -1;
    bool autoexit = false;
    bool exit_on_keydown = false;
    bool exit_on_mousedown = false;
    int loop = 1;
    int framedrop = -1;
    int infinite_buffer = -1;
    enum ShowMode show_mode = SHOW_MODE_NONE;
    const char* audio_codec_name;
    const char* subtitle_codec_name;
    const char* video_codec_name;
    double rdftspeed = 0.02;
    int64_t cursor_last_shown;
    int cursor_hidden = 0;
    const char** vfilters_list = NULL;
    int nb_vfilters = 0;
    char* afilters = NULL;
    int autorotate = 1;
    int find_stream_info = 1;
    int filter_nbthreads = 0;

    /* current context */
    bool is_full_screen;
    int64_t audio_callback_time;
    bool genpts = false;

#define FF_QUIT_EVENT    (SDL_USEREVENT + 2)

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_RendererInfo renderer_info = { 0 };
    SDL_AudioDeviceID audio_dev;


    int opt_default(const char* opt, const char* arg);
    AVDictionary* format_opts = nullptr, * codec_opts = nullptr, * sws_dict = nullptr, * swr_opts = nullptr;

};