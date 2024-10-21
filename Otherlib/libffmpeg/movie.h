#pragma once

#include "audio.h"
#include "video.h"

#include "chronons.h"
#include <atomic>
#include <string>
#include <thread>

class Movie {
public:
    Movie();
    ~Movie();

    int open(std::string filename);
    int start();
    void play();
    void pause();
    void stop();
    void close();
    std::pair<AVFrame *, int64_t> currentFrame();

    nanoseconds getMasterClock();
    nanoseconds getClock();

private:

    int streamComponentOpen(unsigned int stream_index);

    friend class Audio;
    friend class Video;

    Audio audio_;
    Video video_;
    std::string filename_;
    AVFormatContextPtr fmtctx_;

    microseconds clockbase_{microseconds::min()};
    SyncMaster sync_{SyncMaster::Default};

    std::atomic<bool> quit_{false};
    std::atomic<bool> pause_{ false };

    std::thread parseThread_;
    std::thread audioThread_;
    std::thread videoThread_;
};
