#pragma once

#include "chronons.h"
#include "ffmpeg.h"
#include "packet_queue.h"

#include <array>

#define VIDEO_PICTURE_QUEUE_SIZE 24

class Movie;
class Video {
public:
    Video(Movie &movie);
    std::pair<AVFrame *, int64_t> currentFrame();

private:
    int start();
    nanoseconds getClock();

    friend class Movie;
    Movie &movie_;

    AVStream *stream_{nullptr};
    AVCodecContextPtr codecctx_;

    PacketQueue<14 * 1024 * 1024> packets_;

    nanoseconds displayPts_{0};
    microseconds displayPtsTime_{microseconds::min()};
    std::mutex dispPtsMutex_;

    struct Picture {
        AVFramePtr frame;
        nanoseconds pts{nanoseconds::min()};
    };
    std::array<Picture, VIDEO_PICTURE_QUEUE_SIZE> pictQ_;
    std::atomic<size_t> pictQRead_{0u}, pictQWrite_{1u};
    std::mutex pictQMutex_;
    std::condition_variable pictQCond_;
};
