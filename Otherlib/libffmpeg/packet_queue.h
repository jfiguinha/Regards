#pragma once

#include "ffmpeg.h"

#include <deque>
#include <thread>
#include <cassert>

template <size_t SizeLimit> class PacketQueue {
public:
    PacketQueue() = default;
    ~PacketQueue() {
        for (AVPacket &pkt : packets_) {
            av_packet_unref(&pkt);
        }
        packets_.clear();
        totalsize_ = 0;
    }

    int sendTo(AVCodecContext *codecctx) {
        std::unique_lock<std::mutex> lck(mtx_);

        AVPacket *pkt = getPacket(lck);
        int ret = avcodec_send_packet(codecctx, pkt);

        if (!pkt) {
            if (!ret) {
                return AVErrorEOF;
            }
            return ret;
        }

        if (ret != AVERROR(EAGAIN)) {
            pop();
        }

        return ret;
    }

    void setFinished() {
        {
            std::lock_guard<std::mutex> lck(mtx_);
            finished_ = true;
        }
        cond_.notify_one();
    }

    bool put(const AVPacket *pkt) {
        {
            std::lock_guard<std::mutex> lck(mtx_);
            if (totalsize_ >= SizeLimit) {
                return false;
            }
            packets_.push_back(AVPacket{});
            if (av_packet_ref(&packets_.back(), pkt) != 0) {
                assert(0);
                packets_.pop_back();
                return true;
            }
            totalsize_ += static_cast<size_t>(packets_.back().size);
        }
        cond_.notify_one();
        return true;
    }

private:
    AVPacket *getPacket(std::unique_lock<std::mutex> &lck) {
        while (packets_.empty() && !finished_) {
            cond_.wait(lck);
        }
        return packets_.empty() ? nullptr : &packets_.front();
    }

    void pop() {
        AVPacket *pkt = &packets_.front();
        totalsize_ -= static_cast<size_t>(pkt->size);
        av_packet_unref(pkt);
        packets_.pop_front();
    }

    std::mutex mtx_;
    std::condition_variable cond_;
    std::deque<AVPacket> packets_;
    size_t totalsize_{0};
    bool finished_{false};
};
