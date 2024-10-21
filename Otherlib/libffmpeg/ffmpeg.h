#pragma once

extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/time.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavcodec/avcodec.h"
}

#include "chronons.h"

#include <memory>

constexpr auto AVNoPtsValue = AV_NOPTS_VALUE;
constexpr auto AVErrorEOF = AVERROR_EOF;

enum class SyncMaster {
    Audio,
    Video,
    External,

    Default = Audio,
};

inline microseconds get_avtime() {
    return microseconds{av_gettime()};
}

/* Define unique_ptrs to auto-cleanup associated ffmpeg objects. */
struct AVIOContextDeleter {
    void operator()(AVIOContext *ptr) { avio_closep(&ptr); }
};
using AVIOContextPtr = std::unique_ptr<AVIOContext, AVIOContextDeleter>;

struct AVFormatContextDeleter {
    void operator()(AVFormatContext *ptr) { avformat_close_input(&ptr); }
};
using AVFormatContextPtr =
        std::unique_ptr<AVFormatContext, AVFormatContextDeleter>;

struct AVCodecContextDeleter {
    void operator()(AVCodecContext *ptr) { avcodec_free_context(&ptr); }
};
using AVCodecContextPtr =
        std::unique_ptr<AVCodecContext, AVCodecContextDeleter>;

struct AVFrameDeleter {
    void operator()(AVFrame *ptr) { av_frame_free(&ptr); }
};
using AVFramePtr = std::unique_ptr<AVFrame, AVFrameDeleter>;

struct SwrContextDeleter {
    void operator()(SwrContext *ptr) { swr_free(&ptr); }
};
using SwrContextPtr = std::unique_ptr<SwrContext, SwrContextDeleter>;

struct SwsContextDeleter {
    void operator()(SwsContext *ptr) { sws_freeContext(ptr); }
};
using SwsContextPtr = std::unique_ptr<SwsContext, SwsContextDeleter>;
