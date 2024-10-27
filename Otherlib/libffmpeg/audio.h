#pragma once

#include "ffmpeg.h"
#include "packet_queue.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <array>

// Per-buffer size, in time
constexpr milliseconds AudioBufferTime{20};
// Buffer total size, in time (should be divisible by the buffer time)
constexpr milliseconds AudioBufferTotalTime{800};
constexpr auto AudioBufferCount = AudioBufferTotalTime / AudioBufferTime;

class Movie;
class Audio {
public:
    Audio(Movie &movie);
    ~Audio();

    static int initAL();
    void mute();
    void plus();
    void minus();
    int start();
    int decodeFrame();
    int readAudio(uint8_t *samples, unsigned int length);
    bool play();
    void setTimePosition(int64_t time);
    nanoseconds getClock();
    nanoseconds getClockNoLock();

private:
    friend class Movie;
    Movie &movie_;

    void setVolume(float v);
    float getVolume();

    AVStream *stream_{nullptr};
    AVCodecContextPtr codecctx_;

    PacketQueue<2*1024*1024> packets_;

    nanoseconds currentPts_{0};
    nanoseconds deviceStartTime_{nanoseconds::min()};

    AVFramePtr decodedFrame_;
    SwrContextPtr swrctx_;

    AVChannelLayout dstChanLayout_;
    AVSampleFormat dstSampleFmt_{(AVSampleFormat)AV_SAMPLE_FMT_NONE};

    uint8_t *samples_{nullptr};
    int samplesLen_{0};
    int samplesPos_{0};
    int samplesMax_{0};

    float volume_;
    int volumePercent_;
    bool mute_ = false;

    ALenum format_{AL_NONE};
    ALuint frameSize_{0};

    std::mutex srcMutex_;
    std::condition_variable srcCond_;
    ALuint source_{0};
    std::array<ALuint, AudioBufferCount> buffers_{};
    ALuint bufferIdx_{0};
};
