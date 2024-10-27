#include <header.h>
#include "video.h"
#include "movie.h"

#include <iostream>

using std::chrono::duration_cast;

Video::Video(Movie &movie) : movie_(movie) {
    std::for_each(pictQ_.begin(), pictQ_.end(), [](Picture& pic) {
        pic.frame = AVFramePtr{ av_frame_alloc() };
        pic.pts = nanoseconds::min();
        });
}

void Video::setTimePosition(int64_t time)
{

}

int Video::start() {

    displayPts_ = nanoseconds::min();
    displayPtsTime_ = microseconds::min();

    std::for_each(pictQ_.begin(), pictQ_.end(), [](Picture& pic) {
        pic.pts = nanoseconds::min();
        });

    // Prefill the codec buffer.
    do {
        int ret = packets_.sendTo(codecctx_.get());
        if (ret == AVERROR(EAGAIN) || ret == AVErrorEOF) {
            break;
        }
        if (ret != 0) {
            std::cout << "Fail to send packet video: " << ret << std::endl;
        }
    } while (1);
    

    auto current_pts = nanoseconds::zero();

    while (!movie_.quit_.load(std::memory_order_relaxed)) {

        if (!movie_.pause_.load(std::memory_order_relaxed))
        {

            size_t write_idx = pictQWrite_.load(std::memory_order_relaxed);
            Picture* vp = &pictQ_[write_idx];

            // Retrieve video frame.
            AVFrame* decoded_frame = vp->frame.get();
            int ret, pret;
            while ((ret = avcodec_receive_frame(codecctx_.get(), decoded_frame))
                == AVERROR(EAGAIN)
                && (pret = packets_.sendTo(codecctx_.get())) != AVErrorEOF) {
            }
            if (ret != 0) {
                if (ret == AVErrorEOF || pret == AVErrorEOF) {
                    break;
                }
                std::cerr << "Fail to receive frame: " << ret << std::endl;
                continue;
            }

            // Update pts
            if (decoded_frame->best_effort_timestamp != AVNoPtsValue) {
                current_pts = duration_cast<nanoseconds>(
                    seconds_d64{ av_q2d(stream_->time_base)
                                * static_cast<double>(
                                        decoded_frame->best_effort_timestamp) });
            }
            vp->pts = current_pts;

            // Update the video clock to the next expected pts.
            auto frame_delay = av_q2d(codecctx_->time_base);
            frame_delay += decoded_frame->repeat_pict * (frame_delay * 0.5);
            current_pts += duration_cast<nanoseconds>(seconds_d64{ frame_delay });

            // Put the frame in the queue to be loaded into a texture and displayed
            // by the rendering thread.
            write_idx = (write_idx + 1) % pictQ_.size();
            pictQWrite_.store(write_idx, std::memory_order_release);

            // Send a packet for next receive.
            packets_.sendTo(codecctx_.get());

            if (write_idx == pictQRead_.load(std::memory_order_acquire)) {
                // Wait until we have space
                std::unique_lock<std::mutex> lck(pictQMutex_);
                while (write_idx == pictQRead_.load(std::memory_order_acquire)
                    && !movie_.quit_.load(std::memory_order_relaxed)) {
                    pictQCond_.wait(lck);
                }
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }

    }


    packets_.Erase();
    return 0;
}

std::pair<AVFrame *, int64_t> Video::currentFrame() {
    size_t read_idx = pictQRead_.load(std::memory_order_relaxed);
    Picture *vp = &pictQ_[read_idx];

    auto clocktime = movie_.getMasterClock();
    size_t current_idx = read_idx;
    while (1) {
        size_t next_idx = (current_idx + 1) % pictQ_.size();
        if (next_idx == pictQWrite_.load(std::memory_order_acquire)) {
            break;
        }
        Picture *nextvp = &pictQ_[next_idx];
        if (clocktime < nextvp->pts) {
            break;
        }

        current_idx = next_idx;
    }

    if (movie_.quit_.load(std::memory_order_relaxed)) {
        return {nullptr, 0};
    }

    if (current_idx != read_idx) {
        vp = &pictQ_[current_idx];
        pictQRead_.store(current_idx, std::memory_order_release);
        std::unique_lock<std::mutex>{pictQMutex_}.unlock();
        pictQCond_.notify_one();
    }

    if (vp->pts == nanoseconds::min()) {
        return {nullptr, 0};
    }

    if (current_idx != read_idx) {
        std::lock_guard<std::mutex> lck(dispPtsMutex_);
        displayPts_ = vp->pts;
        displayPtsTime_ = get_avtime();
    }

    return {vp->frame.get(), vp->pts.count()};
}

nanoseconds Video::getClock() {
    std::lock_guard<std::mutex> _{dispPtsMutex_};
    if (displayPtsTime_ == microseconds::min()) {
        displayPtsTime_ = get_avtime();
        return displayPts_;
    }
    auto delta = get_avtime() - displayPtsTime_;
    return displayPts_ + delta;
}
