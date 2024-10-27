#include <header.h>
#include "movie.h"
#include "audio.h"
#include "chronons.h"
#include "video.h"

#include <cassert>
#include <iostream>

Movie::Movie() : audio_(*this), video_(*this) {
}

Movie::~Movie() {
    if (parseThread_.joinable()) {
        parseThread_.join();
    }
}

void Movie::setTimePosition(int64_t time)
{
    //currentPts_ = nanoseconds{ seconds{time} };
    //seek_ = true;
    switch (sync_)
    {
        case SyncMaster::Audio:
            audio_.setTimePosition(time);
            break;
    }
}

int Movie::open(std::string filename) {

    close();

    filename_ = filename;
    quit_ = false;
    parseThread_ = std::thread(&Movie::startMovie, this);

    return 0;
}

void Movie::play()
{
    pause_ = false;
    if (quit_)
    {
        if (parseThread_.joinable()) {
            parseThread_.join();
        }
        quit_ = false;
        parseThread_ = std::thread(&Movie::startMovie, this);
    }

}

void Movie::pause()
{
    pause_ = true;
}

void  Movie::start()
{
    if (quit_)
    {
        if (parseThread_.joinable()) {
            parseThread_.join();
        }
        quit_ = false;
        parseThread_ = std::thread(&Movie::startMovie, this);
    }

}

void Movie::stop()
{
    close();
}

void Movie::close() {
    quit_ = true;
    video_.pictQCond_.notify_one();
    if (parseThread_.joinable()) {
        parseThread_.join();
    }
}

std::pair<AVFrame *, int64_t> Movie::currentFrame() {
    return video_.currentFrame();
}

int Movie::streamComponentOpen(unsigned int stream_index) {
    AVCodecContextPtr avctx{avcodec_alloc_context3(nullptr)};
    if (!avctx) {
        std::cerr << "Fail to avcodec_alloc_context3\n";
        return -1;
    }

    int ret;

    ret = avcodec_parameters_to_context(
            avctx.get(),
            fmtctx_->streams[stream_index]->codecpar);
    if (ret < 0) {
        std::cerr << "Fail to avcodec_parameters_to_context\n";
        return -1;
    }

    const AVCodec *codec{avcodec_find_decoder(avctx->codec_id)};
    if (!codec || avcodec_open2(avctx.get(), codec, nullptr) < 0) {
        std::cerr << "Unsupported codec: " << avcodec_get_name(avctx->codec_id)
                  << std::endl;
        return -1;
    }

    switch (avctx->codec_type) {
    case AVMEDIA_TYPE_AUDIO:
        audio_.stream_ = fmtctx_->streams[stream_index];
        audio_.codecctx_ = std::move(avctx);
        break;
    case AVMEDIA_TYPE_VIDEO:
        video_.stream_ = fmtctx_->streams[stream_index];
        video_.codecctx_ = std::move(avctx);
        break;
    default:
        return -1;
    }

    return static_cast<int>(stream_index);
}

void Movie::mute()
{
    audio_.mute();
}

void Movie::plus()
{
    audio_.plus();
}

void Movie::minus()
{
    audio_.minus();
}

int Movie::startMovie() {
    AVFormatContext *fmtctx = avformat_alloc_context();
    AVIOInterruptCB intrcb{nullptr, this};
    fmtctx->interrupt_callback = intrcb;
    if (avformat_open_input(&fmtctx, filename_.c_str(), nullptr, nullptr)
        != 0) {
        std::cerr << "Fail to avformat_open_input\n";
        return -1;
    }
    fmtctx_.reset(fmtctx);

    if (avformat_find_stream_info(fmtctx_.get(), nullptr) < 0) {
        std::cerr << "Fail to avformat_find_stream_info\n";
        return -1;
    }

    int video_index{-1};
    int audio_index{-1};

    for (unsigned int i = 0; i < fmtctx_->nb_streams; i++) {
        auto *codecpar = fmtctx_->streams[i]->codecpar;
        if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_index = streamComponentOpen(i);
        } else if (codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_index = streamComponentOpen(i);
        }
    }

    if (video_index < 0 && audio_index < 0) {
        std::cerr << "Fail to open codecs\n";
        return -1;
    }

    clockbase_ = get_avtime() + milliseconds(750);
    sync_ = (audio_index > 0) ? SyncMaster::Audio : SyncMaster::Video;

    auto &audio_queue = audio_.packets_;
    auto &video_queue = video_.packets_;

    if (audio_index >= 0) {
        audioThread_ = std::thread(&Audio::start, &audio_);
    }
    if (video_index >= 0) {
        videoThread_ = std::thread(&Video::start, &video_);
    }

    while (!quit_.load(std::memory_order_relaxed)) {

        if (!pause_.load(std::memory_order_relaxed))
        {
            if (seek_.load(std::memory_order_relaxed))
            {
               
                int64_t seek_target = currentPts_.count();
               // int64_t seek_min = is->seek_rel > 0 ? seek_target - is->seek_rel + 2 : INT64_MIN;
                //int64_t seek_max = is->seek_rel < 0 ? seek_target - is->seek_rel - 2 : INT64_MAX;
                // FIXME the +-2 is due to rounding being not done in the correct direction in generation
                //      of the seek_pos/seek_rel variables

              //  int ret = avformat_seek_file(fmtctx_.get(), seek_min, seek_target, seek_max, is->seek_flags);


                int ret = avformat_seek_file(fmtctx_.get(), -1, INT64_MIN, seek_target, INT64_MAX, 0);

                seek_ = false;
            }

            AVPacket packet;
            if (av_read_frame(fmtctx_.get(), &packet) < 0) {
                break;
            }

            if (packet.stream_index == video_index) {
                while ((!quit_.load(std::memory_order_acquire) || !pause_.load(std::memory_order_acquire))
                    && !video_queue.put(&packet)) {
                    std::this_thread::sleep_for(milliseconds(100));
                }
            }
            else if (packet.stream_index == audio_index) {
                while ((!quit_.load(std::memory_order_acquire) || !pause_.load(std::memory_order_acquire))
                    && !audio_queue.put(&packet)) {
                    std::this_thread::sleep_for(milliseconds(100));
                }
            }

            av_packet_unref(&packet);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }

    }

    if (video_.codecctx_) {
        video_queue.setFinished();
    }
    if (audio_.codecctx_) {
        audio_queue.setFinished();
    }

    if (audioThread_.joinable()) {
        audioThread_.join();
    }
    if (videoThread_.joinable()) {
        videoThread_.join();
    }

    return 0;
}

nanoseconds Movie::getMasterClock() {
    switch (sync_) {
    case SyncMaster::Audio:
        return audio_.getClock();
    case SyncMaster::Video:
        return video_.getClock();
    default:
        assert(0);
        return getClock();
    }
}

nanoseconds Movie::getClock() {
    assert(0);
    return nanoseconds::min();
}
