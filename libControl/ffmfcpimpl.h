#pragma once

#include <header.h>

#include <atomic>
#include <cstdint>
#include <thread>

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>

#include "VideoControlInterface.h"

extern "C"
{
#include "libavutil/avstring.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/bprint.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/avassert.h"
#include "libavutil/time.h"
#include "libavutil/hwcontext.h"
#include "libavutil/opt.h"
#include "libavutil/display.h"
#include "libavutil/common.h"
#include "libavutil/fifo.h"

#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"

#include "libavfilter/buffersrc.h"
#include "libavfilter/buffersink.h"

#include "libswscale/swscale.h"
#include "libswresample/swresample.h"

#ifdef __APPLE__
#include "libavcodec/avcodec.h"
#include "libavcodec/videotoolbox.h"
#endif
}


// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------

#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
#define MIN_FRAMES 25

#define EXTERNAL_CLOCK_MIN_FRAMES 2
#define EXTERNAL_CLOCK_MAX_FRAMES 10

#define SDL_AUDIO_MIN_BUFFER_SIZE 512
#define SDL_AUDIO_MAX_CALLBACKS_PER_SEC 30

#define SDL_VOLUME_STEP (0.75)

#define AV_SYNC_THRESHOLD_MIN 0.04
#define AV_SYNC_THRESHOLD_MAX 0.1
#define AV_SYNC_FRAMEDUP_THRESHOLD 0.1
#define AV_NOSYNC_THRESHOLD 10.0

#define SAMPLE_CORRECTION_PERCENT_MAX 10

#define EXTERNAL_CLOCK_SPEED_MIN 0.900
#define EXTERNAL_CLOCK_SPEED_MAX 1.010
#define EXTERNAL_CLOCK_SPEED_STEP 0.001

#define AUDIO_DIFF_AVG_NB 20

#define REFRESH_RATE 0.01

#define SAMPLE_ARRAY_SIZE (8 * 65536)

#define CURSOR_HIDE_DELAY 1000000

#define USE_ONEPASS_SUBTITLE_RENDER 1

#define VIDEO_PICTURE_QUEUE_SIZE 3
#define SUBPICTURE_QUEUE_SIZE 16
#define SAMPLE_QUEUE_SIZE 9

#define FRAME_QUEUE_SIZE \
    FFMAX(SAMPLE_QUEUE_SIZE, \
    FFMAX(VIDEO_PICTURE_QUEUE_SIZE, SUBPICTURE_QUEUE_SIZE))


// -----------------------------------------------------------------------------
// Subtitle helpers
// -----------------------------------------------------------------------------

#define RGBA_IN(r, g, b, a, s)              \
{                                           \
    const unsigned int v =                 \
        ((const uint32_t*)(s))[0];          \
    (a) = (v >> 24) & 0xff;                 \
    (r) = (v >> 16) & 0xff;                 \
    (g) = (v >> 8) & 0xff;                  \
    (b) = v & 0xff;                         \
}

#define YUVA_IN(y, u, v, a, s, pal)         \
{                                           \
    const unsigned int val =               \
        ((const uint32_t*)(pal))            \
            [*(const uint8_t*)(s)];         \
    (a) = (val >> 24) & 0xff;               \
    (y) = (val >> 16) & 0xff;               \
    (u) = (val >> 8) & 0xff;                \
    (v) = val & 0xff;                       \
}


// -----------------------------------------------------------------------------
// Events
// -----------------------------------------------------------------------------

#define FF_ALLOC_EVENT              SDL_USEREVENT
#define FF_REFRESH_EVENT            (SDL_USEREVENT + 1)

#define FF_QUIT_EVENT               4096
#define FF_STEP_EVENT               (FF_QUIT_EVENT + 1)
#define FF_PAUSE_EVENT              (FF_QUIT_EVENT + 2)
#define FF_PLAY_EVENT               (FF_QUIT_EVENT + 3)
#define FF_ASPECT_EVENT             (FF_QUIT_EVENT + 4)
#define FF_AUDIODISPLAY_EVENT       (FF_QUIT_EVENT + 5)
#define FF_EXIT_EVENT               (FF_QUIT_EVENT + 6)
#define CLOSESTREAM_EVENT           (FF_QUIT_EVENT + 7)
#define FF_STOP_EVENT               (FF_QUIT_EVENT + 8)

#define SEEK_BAR_EVENT              (SDL_USEREVENT + 4)
#define VOLUME_UP_EVENT             (SDL_USEREVENT + 5)
#define VOLUME_DOWN_EVENT           (SDL_USEREVENT + 6)
#define SET_POSITION                (SDL_USEREVENT + 7)
#define CHANGE_AUDIO                (SDL_USEREVENT + 8)
#define CHANGE_SUBTITLE             (SDL_USEREVENT + 9)
#define VOLUME_EVENT                (SDL_USEREVENT + 10)
#define SET_SEEKPOSITION            (SDL_USEREVENT + 11)


class CFFmfcPimpl
{
public:

    // -------------------------------------------------------------------------
    // Hardware acceleration
    // -------------------------------------------------------------------------

    enum HWAccelID
    {
        HWACCEL_NONE = 0,
        HWACCEL_AUTO,
        HWACCEL_GENERIC,
        HWACCEL_VIDEOTOOLBOX,
        HWACCEL_QSV
    };

    struct HWAccel
    {
        const char* name;
        int (*init)(AVCodecContext* s);
        HWAccelID id;
        AVPixelFormat pix_fmt;
    };

    struct HWDevice
    {
        const char* name;
        AVHWDeviceType type;
        AVBufferRef* device_ref;
    };


    // -------------------------------------------------------------------------
    // Packet queue
    // -------------------------------------------------------------------------

    struct PacketQueue
    {
        AVFifo* pkt_list = nullptr;

        int nb_packets = 0;
        int size = 0;

        int64_t duration = 0;

        int abort_request = 0;
        int serial = 0;

        SDL_mutex* mutex = nullptr;
        SDL_cond* cond = nullptr;
    };


    // -------------------------------------------------------------------------
    // Video picture
    // -------------------------------------------------------------------------

    struct VideoPicture
    {
        double pts = 0.0;

        int64_t pos = -1;

        int skip = 0;

        int width = 0;
        int height = 0;

        AVRational sample_aspect_ratio{ 1, 1 };

        int allocated = 0;
        int reallocate = 0;
    };


    // -------------------------------------------------------------------------
    // Subtitle
    // -------------------------------------------------------------------------

    struct SubPicture
    {
        double pts = 0.0;

        AVSubtitle sub{};
    };


    // -------------------------------------------------------------------------
    // Audio
    // -------------------------------------------------------------------------

    struct AudioParams
    {
        int freq = 0;

        AVChannelLayout ch_layout{};

        AVSampleFormat fmt = AV_SAMPLE_FMT_NONE;

        int frame_size = 0;

        int bytes_per_sec = 0;
    };


    // -------------------------------------------------------------------------
    // Decoder
    // -------------------------------------------------------------------------

    struct Decoder
    {
        AVPacket* pkt = nullptr;

        PacketQueue* queue = nullptr;

        AVCodecContext* avctx = nullptr;

        int pkt_serial = 0;
        int finished = 0;
        int packet_pending = 0;

        SDL_cond* empty_queue_cond = nullptr;

        int64_t start_pts = AV_NOPTS_VALUE;

        AVRational start_pts_tb{ 0, 1 };

        int64_t next_pts = AV_NOPTS_VALUE;

        AVRational next_pts_tb{ 0, 1 };

        SDL_Thread* decoder_tid = nullptr;
    };


    // -------------------------------------------------------------------------
    // Frame
    // -------------------------------------------------------------------------

    struct Frame
    {
        AVFrame* frame = nullptr;

        AVSubtitle sub{};

        int serial = 0;

        double pts = 0.0;
        double duration = 0.0;

        int64_t pos = -1;

        int width = 0;
        int height = 0;
        int format = 0;

        AVRational sar{ 1, 1 };
        AVRational sample_aspect_ratio{ 1, 1 };

        int uploaded = 0;
        int flip_v = 0;
    };


    // -------------------------------------------------------------------------
    // Clock
    // -------------------------------------------------------------------------

    struct Clock
    {
        double pts = 0.0;
        double pts_drift = 0.0;
        double last_updated = 0.0;
        double speed = 1.0;

        int serial = -1;
        int paused = 0;

        int* queue_serial = nullptr;
    };


    // -------------------------------------------------------------------------
    // Frame queue
    // -------------------------------------------------------------------------

    struct FrameQueue
    {
        Frame queue[FRAME_QUEUE_SIZE];

        int rindex = 0;
        int windex = 0;

        int size = 0;

        int max_size = 0;

        int keep_last = 0;

        int rindex_shown = 0;

        SDL_mutex* mutex = nullptr;
        SDL_cond* cond = nullptr;

        PacketQueue* pktq = nullptr;
    };


    struct FrameData
    {
        int64_t pkt_pos = -1;
    };


    // -------------------------------------------------------------------------
    // Video state
    // -------------------------------------------------------------------------

    struct VideoState
    {
        int refresh = 0;

        std::thread* refresh_tid = nullptr;

        SDL_Thread* read_tid = nullptr;

        AVInputFormat* iformat = nullptr;

        int abort_request = 0;

        int force_refresh = 0;

        int paused = 0;
        int last_paused = 0;

        int queue_attachments_req = 0;

        int seek_req = 0;
        int seek_flags = 0;

        int64_t seek_pos = 0;
        int64_t seek_rel = 0;

        int read_pause_return = 0;

        AVFormatContext* ic = nullptr;

        int realtime = 0;

        Clock audclk;
        Clock vidclk;
        Clock extclk;

        FrameQueue pictq;
        FrameQueue subpq;
        FrameQueue sampq;

        Decoder auddec;
        Decoder viddec;
        Decoder subdec;

        int audio_stream = -1;

        int av_sync_type = AV_SYNC_AUDIO_MASTER;

        double audio_clock = 0.0;
        int audio_clock_serial = -1;

        double audio_diff_cum = 0.0;
        double audio_diff_avg_coef = 0.0;
        double audio_diff_threshold = 0.0;

        int audio_diff_avg_count = 0;

        AVStream* audio_st = nullptr;

        PacketQueue audioq;

        int audio_hw_buf_size = 0;

        uint8_t* audio_buf = nullptr;
        uint8_t* audio_buf1 = nullptr;

        unsigned int audio_buf_size = 0;
        unsigned int audio_buf1_size = 0;

        int audio_buf_index = 0;

        int audio_write_buf_size = 0;

        int audio_volume = 100;

        int muted = 0;

        AudioParams audio_src;
        AudioParams audio_filter_src;
        AudioParams audio_tgt;

        SwrContext* swr_ctx = nullptr;

        int frame_drops_early = 0;
        int frame_drops_late = 0;

        enum ShowMode
        {
            SHOW_MODE_NONE = -1,
            SHOW_MODE_VIDEO = 0,
            SHOW_MODE_WAVES,
            SHOW_MODE_RDFT,
            SHOW_MODE_NB
        } show_mode = SHOW_MODE_NONE;

        int16_t sample_array[SAMPLE_ARRAY_SIZE]{};

        int sample_array_index = 0;

        int last_i_start = 0;

        int rdft_bits = 0;

        int xpos = 0;

        double last_vis_time = 0.0;

        int subtitle_stream = -1;

        AVStream* subtitle_st = nullptr;

        PacketQueue subtitleq;

        double frame_timer = 0.0;
        double frame_last_returned_time = 0.0;
        double frame_last_filter_delay = 0.0;

        int video_stream = -1;

        AVStream* video_st = nullptr;

        PacketQueue videoq;

        double max_frame_duration = 10.0;

        SwsContext* img_convert_ctx = nullptr;
        SwsContext* sub_convert_ctx = nullptr;

        int eof = 0;

        char* filename = nullptr;

        int width = 0;
        int height = 0;

        int xleft = 0;
        int ytop = 0;

        int step = 0;

        int last_video_stream = -1;
        int last_audio_stream = -1;
        int last_subtitle_stream = -1;

        SDL_cond* continue_read_thread = nullptr;

        CFFmfcPimpl* _pimpl = nullptr;

        // Hardware acceleration
        HWAccelID hwaccel_id = HWACCEL_AUTO;

        AVHWDeviceType hwaccel_device_type =
            AV_HWDEVICE_TYPE_NONE;

        char* hwaccel_device = nullptr;

        AVPixelFormat hwaccel_output_format =
            AV_PIX_FMT_NONE;

        int wanted_nb_channels = 2;

        void* hwaccel_ctx = nullptr;

        void (*hwaccel_uninit)(AVCodecContext* s) = nullptr;

        int (*hwaccel_get_buffer)(
            AVCodecContext* s,
            AVFrame* frame,
            int flags) = nullptr;

        int (*hwaccel_retrieve_data)(
            AVCodecContext* s,
            AVFrame* frame) = nullptr;

        AVPixelFormat hwaccel_pix_fmt =
            AV_PIX_FMT_NONE;

        AVPixelFormat hwaccel_retrieved_pix_fmt =
            AV_PIX_FMT_NONE;

        AVBufferRef* hw_frames_ctx = nullptr;

        AVCodecContext* avctx = nullptr;

        AVCodec* codec = nullptr;

        AVFilterContext* in_audio_filter = nullptr;
        AVFilterContext* out_audio_filter = nullptr;

        AVFilterGraph* agraph = nullptr;
    };


    // -------------------------------------------------------------------------
    // Misc
    // -------------------------------------------------------------------------

    enum ShowMode
    {
        SHOW_MODE_NONE = -1,
        SHOW_MODE_VIDEO = 0,
        SHOW_MODE_WAVES,
        SHOW_MODE_RDFT,
        SHOW_MODE_NB
    };


    struct MyAVPacketList
    {
        AVPacket* pkt = nullptr;
        int serial = 0;
    };


public:

    CFFmfcPimpl() = default;

    ~CFFmfcPimpl() = default;


    // -------------------------------------------------------------------------
    // Thread control
    // -------------------------------------------------------------------------

    static int refresh_thread(void* opaque);

    void StopStream();


    // -------------------------------------------------------------------------
    // Video
    // -------------------------------------------------------------------------

    void video_refresh(
        void* opaque,
        double* remaining_time);

    void video_display(VideoState* is);

    void video_image_display(VideoState* is);

    static int video_thread(void* arg);

    int get_video_frame(
        VideoState* is,
        AVFrame* frame);

    int queue_picture(
        VideoState* is,
        AVFrame* src_frame,
        double pts,
        double duration,
        int64_t pos,
        int serial);


    // -------------------------------------------------------------------------
    // Frame queue
    // -------------------------------------------------------------------------

    int frame_queue_init(
        FrameQueue* f,
        PacketQueue* pktq,
        int max_size,
        int keep_last);

    void frame_queue_destory(FrameQueue* f);

    void frame_queue_unref_item(Frame* vp);

    void frame_queue_signal(FrameQueue* f);

    Frame* frame_queue_peek(FrameQueue* f);
    Frame* frame_queue_peek_next(FrameQueue* f);
    Frame* frame_queue_peek_last(FrameQueue* f);

    Frame* frame_queue_peek_writable(FrameQueue* f);
    Frame* frame_queue_peek_readable(FrameQueue* f);

    void frame_queue_push(FrameQueue* f);
    void frame_queue_next(FrameQueue* f);

    int frame_queue_nb_remaining(FrameQueue* f);

    int64_t frame_queue_last_pos(FrameQueue* f);


    // -------------------------------------------------------------------------
    // Packet queue
    // -------------------------------------------------------------------------

    int packet_queue_init(PacketQueue* q);

    void packet_queue_flush(PacketQueue* q);
    void packet_queue_destroy(PacketQueue* q);

    void packet_queue_abort(PacketQueue* q);
    void packet_queue_start(PacketQueue* q);

    int packet_queue_put_private(
        PacketQueue* q,
        AVPacket* pkt);

    int packet_queue_put(
        PacketQueue* q,
        AVPacket* pkt);

    int packet_queue_put_nullpacket(
        PacketQueue* q,
        AVPacket* pkt,
        int stream_index);

    int packet_queue_get(
        PacketQueue* q,
        AVPacket* pkt,
        int block,
        int* serial);


    // -------------------------------------------------------------------------
    // Decoder
    // -------------------------------------------------------------------------

    int decoder_start(
        Decoder* d,
        int (*fn)(void*),
        const char* thread_name,
        void* arg);

    void decoder_abort(
        Decoder* d,
        FrameQueue* fq);

    int decoder_init(
        Decoder* d,
        AVCodecContext* avctx,
        PacketQueue* queue,
        SDL_cond* empty_queue_cond);

    void decoder_destroy(Decoder* d);

    int decoder_decode_frame(
        VideoState* is,
        Decoder* d,
        AVFrame* frame,
        AVSubtitle* sub);


    // -------------------------------------------------------------------------
    // Audio
    // -------------------------------------------------------------------------

    static int audio_thread(void* arg);

    static void sdl_audio_callback(
        void* opaque,
        Uint8* stream,
        int len);

    void update_sample_display(
        VideoState* is,
        short* samples,
        int samples_size);

    int synchronize_audio(
        VideoState* is,
        int nb_samples);

    int audio_decode_frame(VideoState* is);

    int audio_open(
        void* opaque,
        AVChannelLayout* wanted_channel_layout,
        int wanted_sample_rate,
        AudioParams* audio_hw_params);

    int configure_audio_filters(
        VideoState* is,
        const char* afilters,
        int force_output_format);

    int configure_filtergraph(
        AVFilterGraph* graph,
        const char* filtergraph,
        AVFilterContext* source_ctx,
        AVFilterContext* sink_ctx);


    // -------------------------------------------------------------------------
    // Clock / synchronization
    // -------------------------------------------------------------------------

    int get_master_sync_type(VideoState* is);

    double get_master_clock(VideoState* is);

    double vp_duration(
        VideoState* is,
        Frame* vp,
        Frame* nextvp);

    double compute_target_delay(
        double delay,
        VideoState* is);

    void check_external_clock_speed(VideoState* is);

    void update_video_pts(
        VideoState* is,
        double pts,
        int64_t pos,
        int serial);

    void sync_clock_to_slave(
        Clock* c,
        Clock* slave);

    void init_clock(
        Clock* c,
        int* queue_serial);

    void set_clock_at(
        Clock* c,
        double pts,
        int serial,
        double time);

    void set_clock(
        Clock* c,
        double pts,
        int serial);

    void set_clock_speed(
        Clock* c,
        double speed);

    double get_clock(Clock* c);


    // -------------------------------------------------------------------------
    // Stream
    // -------------------------------------------------------------------------

    VideoState* stream_open(
        const char* filename,
        AVInputFormat* iformat);

    void stream_close(VideoState* is);

    void do_exit(VideoState* is);

    void stream_seek(
        VideoState* is,
        int64_t pos,
        int64_t rel,
        int seek_by_bytes);

    void stream_toggle_pause(VideoState* is);

    void toggle_pause(VideoState* is);
    void toggle_play(VideoState* is);

    void stream_cycle_channel(
        VideoState* is,
        int codec_type);

    void stream_change_stream(
        VideoState* is,
        int codec_type,
        int newIndex);

    int stream_component_open(
        VideoState* is,
        int stream_index);

    void stream_component_close(
        VideoState* is,
        int stream_index);

    int stream_has_enough_packets(
        AVStream* st,
        int stream_id,
        PacketQueue* queue);


    // -------------------------------------------------------------------------
    // FFmpeg helpers
    // -------------------------------------------------------------------------

    static int decode_interrupt_cb(void* ctx);

    int is_realtime(
        AVFormatContext* s,
        char* filename);

    const char* getExt(const char* fspec);

    static void free_subpicture(SubPicture* sp);

    AVFrame* CopyFrame(AVFrame* src);


#ifndef CMDUTILS

    int check_stream_specifier(
        AVFormatContext* s,
        AVStream* st,
        const char* spec);

    AVDictionary* filter_codec_opts(
        AVDictionary* opts,
        enum AVCodecID codec_id,
        AVFormatContext* s,
        AVStream* st,
        AVCodec* codec);

    void uninit_opts();

    AVDictionary** setup_find_stream_info_opts(
        AVFormatContext* s,
        AVDictionary* codec_opts);

#endif


    // -------------------------------------------------------------------------
    // Hardware
    // -------------------------------------------------------------------------

    int hw_decoder_init(
        AVCodecContext* ctx,
        enum AVHWDeviceType type);

    static int get_buffer(
        AVCodecContext* s,
        AVFrame* frame,
        int flags);

    enum AVPixelFormat find_fmt_by_hw_type(
        enum AVHWDeviceType type);

    static int hwaccel_retrieve_data(
        AVCodecContext* avctx,
        AVFrame* input);

    static enum AVPixelFormat get_hw_format(
        AVCodecContext* ctx,
        const enum AVPixelFormat* pix_fmts);

    bool TestHardware(
        const wxString& acceleratorHardware,
        AVHWDeviceType& type,
        AVCodecContext* avct,
        AVCodec* codec,
        AVDictionary*& opts,
        VideoState* is,
        AVStream* video);

    enum
    {
        AV_SYNC_AUDIO_MASTER,
        /* default choice */
        AV_SYNC_VIDEO_MASTER,
        AV_SYNC_EXTERNAL_CLOCK,
        /* synchronize to an external clock */
    };



    // -------------------------------------------------------------------------
    // Application
    // -------------------------------------------------------------------------

    int GetPosition(VideoState* is);

    int percentageToDb(int p, int maxValue);

    int IsSupportOpenCL();

    double get_rotation(AVStream* st);


private:

    // -------------------------------------------------------------------------
    // Optimized video conversion
    // -------------------------------------------------------------------------

    bool EnsureVideoConversionContext(
        const AVFrame* frame);

    static void ConvertSubtitleBitmap(
        const AVSubtitleRect* rect,
        cv::Mat& bitmap);

    void PostSubtitleImage(
        const cv::Mat& bitmap);

    void PostSubtitleText(
        const wxString& text,
        int endDisplayTime);


    static int read_thread(void* arg);
    static int subtitle_thread(void* arg);

public:
    int volume = 100;
    int64_t time_position = 0;


#ifdef WIN32
    wxString acceleratorHardware = "d3d11va";
#elif defined(__APPLE__)
    wxString acceleratorHardware = "videotoolbox";
#else
    wxString acceleratorHardware = "cuda";
#endif

    bool isOpenGLDecoding = false;
    int percentVolume = 100;
    static CVideoControlInterface* dlg;
    wxWindow* parent = nullptr;

    int exit_remark = 0;

    VideoState* g_is = nullptr;

    int autoexit = 0;
    int display_disable = 0;
    int video_disable = 0;

    AVInputFormat* file_iformat = nullptr;
    int seek_bar_pos = 0;

    // -------------------------------------------------------------------------
// Options
// -------------------------------------------------------------------------

    int screen_width = 0;
    int screen_height = 0;
    int videoOutputMode = 24;
    int seek_by_bytes = 0;

    void step_to_next_frame(VideoState* is);

    // -------------------------------------------------------------------------
// Statistics
// -------------------------------------------------------------------------

    int vframe_index = 0;
    int aframe_index = 0;
    int packet_index = 0;

    int video_angle = 0;

    int video_flipV = 0;
    int video_flipH = 0;


private:


    // -------------------------------------------------------------------------
    // Global/thread state
    // -------------------------------------------------------------------------

    static std::atomic_bool exit_video;



    static enum AVPixelFormat hw_pix_fmt;

    AVBufferRef* hw_device_ctx = nullptr;






    // -------------------------------------------------------------------------
    // Conversion context
    // -------------------------------------------------------------------------

    SwsContext* localContext = nullptr;

    int localWidth = 0;
    int localHeight = 0;

    AVPixelFormat localFormat =
        AV_PIX_FMT_NONE;


    // -------------------------------------------------------------------------
    // Audio
    // -------------------------------------------------------------------------

    int muted = 0;



    SDL_AudioDeviceID audio_dev = 0;




    int audio_disable = 0;
    int disable_framedelay = 0;


    int wanted_stream[AVMEDIA_TYPE_NB] =
    { -1, -1, 0, -1, 0 };




    int show_status = 0;

    int av_sync_type =
        AV_SYNC_AUDIO_MASTER;

    int64_t start_time =
        AV_NOPTS_VALUE;

    int64_t duration =
        AV_NOPTS_VALUE;

    int workaround_bugs = 1;
    int fast = 0;
    int genpts = 0;

    int subtitle_disable = 0;
    int lowres = 0;

    int idct = FF_IDCT_AUTO;

    AVDiscard skip_frame =
        AVDISCARD_DEFAULT;

    AVDiscard skip_idct =
        AVDISCARD_DEFAULT;

    AVDiscard skip_loop_filter =
        AVDISCARD_DEFAULT;

    int error_concealment = 3;

    int decoder_reorder_pts = -1;

    int exit_on_keydown = 0;
    int exit_on_mousedown = 0;

    int loop = 1;

    int framedrop = -1;

    int infinite_buffer = -1;


    // -------------------------------------------------------------------------
    // Codec configuration
    // -------------------------------------------------------------------------

    const char* audio_codec_name = nullptr;
    const char* subtitle_codec_name = nullptr;
    const char* video_codec_name = nullptr;

    int rdftspeed = 20;


    // -------------------------------------------------------------------------
    // Playback
    // -------------------------------------------------------------------------

    int64_t audio_callback_time = 0;










    bool first = true;


    // -------------------------------------------------------------------------
    // Hardware configuration
    // -------------------------------------------------------------------------

    wxString colorRange;
    wxString colorSpace;

    bool isHardwareDecoding = false;


    int find_stream_info = 1;


#ifndef CMDUTILS
    AVDictionary* format_opts = nullptr;
    AVDictionary* codec_opts = nullptr;
#endif

    const char* wanted_stream_spec[AVMEDIA_TYPE_NB] =
    { nullptr };
};