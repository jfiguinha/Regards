#include <header.h>
#include "audio.h"
#include "movie.h"


using std::chrono::duration_cast;

Audio::Audio(Movie &movie) : movie_(movie) {
    static std::once_flag once;
    std::call_once(once, []() { initAL(); });
}

Audio::~Audio() {
    /*
    if (source_) {
        alDeleteSources(1, &source_);
    }
    if (buffers_[0]) {
        alDeleteBuffers(buffers_.size(), buffers_.data());
    }
    if (samples_) {
        av_freep(&samples_);
    }
    */
}

int Audio::initAL() {
    ALCdevice *device = alcOpenDevice(nullptr);
    if (!device) {
        std::cerr << "Fail to alcOpenDevice\n";
        return -1;
    }

    ALCcontext *ctx = alcCreateContext(device, nullptr);
    if (!ctx || alcMakeContextCurrent(ctx) == ALC_FALSE) {
        if (ctx) {
            alcDestroyContext(ctx);
        }
        alcCloseDevice(device);
        std::cerr << "Fail to set alc context\n";
        return -1;
    }

    return 0;
}

void Audio::mute()
{
    mute_ = !mute_;
    if (mute_)
    {
        setVolume(0.0);
    }
    else
    {
        setVolume(volume_);
    }
}

void Audio::plus()
{
    //Max 1.0
    volumePercent_+=10;
    volume_ = (float)volumePercent_ / 100.0f;
    setVolume(volume_);

}

void Audio::minus()
{
    //Min 0.0
    volumePercent_ -= 10;
    volume_ = (float)volumePercent_ / 100.0f;
    setVolume(volume_);
}

void Audio::setVolume(float v)
{
    alListenerf(AL_GAIN, v);
}

float Audio::getVolume()
{
    ALfloat v;
    alGetListenerf(AL_GAIN, &v);
    return v;
}


int Audio::start() {
    std::unique_lock<std::mutex> srclck(srcMutex_, std::defer_lock);
    milliseconds sleep_time{AudioBufferTime / 3};

    currentPts_ = nanoseconds::min();
    deviceStartTime_ = nanoseconds::min();

    volume_ = getVolume();
    mute_ = false;
    volumePercent_ = volume_ * 100;

    ALenum FormatStereo8{AL_FORMAT_STEREO8};
    ALenum FormatStereo16{AL_FORMAT_STEREO16};

    if (codecctx_->sample_fmt == AV_SAMPLE_FMT_U8
        || codecctx_->sample_fmt == AV_SAMPLE_FMT_U8P) {
        dstSampleFmt_ = AV_SAMPLE_FMT_U8;
        frameSize_ = 1;
        if (codecctx_->ch_layout.nb_channels == AV_CH_LAYOUT_MONO) {
            dstChanLayout_ = codecctx_->ch_layout;
            frameSize_ *= 1;
            format_ = AL_FORMAT_MONO8;
        }
        if (!format_ || format_ == -1) {
            dstChanLayout_ = AV_CHANNEL_LAYOUT_MASK(2, AV_CH_LAYOUT_STEREO);
            frameSize_ *= 2;
            format_ = FormatStereo8;
        }
    }

    if (!format_ || format_ == -1) {
        dstSampleFmt_ = AV_SAMPLE_FMT_S16;
        frameSize_ = 2;
        if (codecctx_->ch_layout.nb_channels == AV_CH_LAYOUT_MONO) {
            dstChanLayout_ = codecctx_->ch_layout;
            frameSize_ *= 1;
            format_ = AL_FORMAT_MONO16;
        }
        if (!format_ || format_ == -1) {
            dstChanLayout_ = AV_CHANNEL_LAYOUT_MASK(2, AV_CH_LAYOUT_STEREO);
            frameSize_ *= 2;
            format_ = FormatStereo16;
        }
    }

    void *samples{nullptr};
    ALsizei buffer_len{0};

    samples_ = nullptr;
    samplesMax_ = 0;
    samplesPos_ = 0;
    samplesLen_ = 0;

    decodedFrame_.reset(av_frame_alloc());
    if (!decodedFrame_) {
        std::cerr << "Fail to av_frame_alloc\n";
        goto finish;
    }

    if (dstChanLayout_.nb_channels > 0) {

        SwrContext* swr = NULL;
        int ret = swr_alloc_set_opts2(&swr,         // we're allocating a new context
            &dstChanLayout_, // out_ch_layout
            dstSampleFmt_,    // out_sample_fmt
            codecctx_->sample_rate,                // out_sample_rate
            &codecctx_->ch_layout, // in_ch_layout
            codecctx_->sample_fmt,
            codecctx_->sample_rate,
            0,                    // log_offset
            NULL);                // log_ctx

        swrctx_.reset(swr);
        /*
        swr_alloc_set_opts2(
                nullptr,
                dstChanLayout_,
                dstSampleFmt_,
                codecctx_->sample_rate,
                codecctx_->ch_layout,
                codecctx_->sample_fmt,
                codecctx_->sample_rate,
                0,
                nullptr));*/
        if (!swrctx_ || swr_init(swrctx_.get()) != 0) {
            std::cerr << "Fail to initialize swr\n";
            goto finish;
        }
    } else {
        assert(0);
        goto finish;
    }

    alGenBuffers(buffers_.size(), buffers_.data());
    alGenSources(1, &source_);

    if (alGetError() != AL_NO_ERROR) {
        goto finish;
    }

    buffer_len =
            duration_cast<seconds>(codecctx_->sample_rate * AudioBufferTime)
                    .count()
            * frameSize_;
    samples = av_malloc(buffer_len);

    // Prefill the codec buffer.
    do {
        const int ret = packets_.sendTo(codecctx_.get());
        if (ret == AVERROR(EAGAIN) || ret == AVErrorEOF) {
            break;
        }
        if (ret != 0) {
            std::cout << "Fail to send packet audio: " << ret << std::endl;
        }
    } while (1);

    srclck.lock();

    play();
    //std::this_thread::sleep_for(milliseconds(100));

    while (!movie_.quit_.load(std::memory_order_relaxed)) {

        if (!movie_.pause_.load(std::memory_order_relaxed))
        {

            ALenum state;
            ALint processed, queued;

            // First remove any processed buffers.
            alGetSourcei(source_, AL_BUFFERS_PROCESSED, &processed);
            if (processed > 0) {
                std::vector<ALuint> bufids(processed);
                alSourceUnqueueBuffers(source_, processed, bufids.data());
            }

            // Refill the buffer queue.
            alGetSourcei(source_, AL_BUFFERS_QUEUED, &queued);
            while (queued < buffers_.size()) {
                if (!readAudio(static_cast<uint8_t*>(samples), buffer_len)) {
                    break;
                }
                ALuint bufid = buffers_[bufferIdx_];
                bufferIdx_ = (bufferIdx_ + 1) % buffers_.size();
                alBufferData(bufid,
                    format_,
                    samples,
                    buffer_len,
                    codecctx_->sample_rate);
                alSourceQueueBuffers(source_, 1, &bufid);
                ++queued;
            }

            alGetSourcei(source_, AL_SOURCE_STATE, &state);
            if (state == AL_PAUSED) {
                alSourceRewind(source_);
                alSourcei(source_, AL_BUFFER, 0);
                continue;
            }

            if (state != AL_PLAYING && state != AL_PAUSED) {
                if (!play()) {
                    break;
                }
            }

            if (alGetError() != AL_NO_ERROR) {
                return false;
            }

            
        }

        srcCond_.wait_for(srclck, sleep_time);
    }

    alSourceRewind(source_);
    alSourcei(source_, AL_BUFFER, 0);

    srclck.unlock();

finish:
    av_freep(&samples);
    packets_.Erase();
    if (source_) {
        alDeleteSources(1, &source_);
    }
    if (buffers_[0]) {
        alDeleteBuffers(buffers_.size(), buffers_.data());
    }
    if (samples_) {
        av_freep(&samples_);
    }
    source_ = 0;
    return 0;
}

int Audio::decodeFrame() {
    while (!movie_.quit_.load(std::memory_order_relaxed)) {
        int ret, pret;
        while ((ret = avcodec_receive_frame(codecctx_.get(),
                                            decodedFrame_.get()))
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
        if (decodedFrame_->nb_samples <= 0) {
            continue;
        }
        if (decodedFrame_->best_effort_timestamp != AVNoPtsValue) {
            currentPts_ = duration_cast<nanoseconds>(
                    seconds_d64{av_q2d(stream_->time_base)
                                * static_cast<double>(
                                        decodedFrame_->best_effort_timestamp)});
        }
        if (decodedFrame_->nb_samples > samplesMax_) {
            av_freep(&samples_);
            av_samples_alloc(&samples_,
                             nullptr,
                             codecctx_->ch_layout.nb_channels,
                             decodedFrame_->nb_samples,
                             dstSampleFmt_,
                             0);
            samplesMax_ = decodedFrame_->nb_samples;
        }
        int nsamples = swr_convert(swrctx_.get(),
                                   &samples_,
                                   decodedFrame_->nb_samples,
                                   (const uint8_t **)decodedFrame_->data,
                                   decodedFrame_->nb_samples);
        av_frame_unref(decodedFrame_.get());
        return nsamples;
    }

    return 0;
}

int Audio::readAudio(uint8_t *samples, unsigned int length) {
    unsigned int audio_size = 0;
    length /= frameSize_;
    while (audio_size < length) {
        if (samplesPos_ == samplesLen_) {
            int frame_len = decodeFrame();
            if (frame_len <= 0) {
                break;
            }
            samplesLen_ = frame_len;
            samplesPos_ = 0;
        }

        const unsigned int len = samplesLen_ - samplesPos_;
        unsigned int rem = length - audio_size;
        if (rem > len) {
            rem = len;
        }
        std::copy_n(samples_ + samplesPos_ * frameSize_,
                    rem * frameSize_,
                    samples);
        samplesPos_ += rem;
        currentPts_ += nanoseconds{seconds{rem}} / codecctx_->sample_rate;
        samples += rem * frameSize_;
        audio_size += rem;
    }

    if (audio_size <= 0) {
        return false;
    }

    if (audio_size < length) {
        const unsigned int rem = length - audio_size;
        std::fill_n(samples,
                    rem * frameSize_,
                    (dstSampleFmt_ == AV_SAMPLE_FMT_U8 ? 0x80 : 0x00));
        currentPts_ += nanoseconds{seconds{rem}} / codecctx_->sample_rate;
        audio_size += rem;
    }

    return true;
}

bool Audio::play() {
    ALint queued{};
    alGetSourcei(source_, AL_BUFFERS_QUEUED, &queued);
    if (queued == 0) {
        return false;
    }

    alSourcePlay(source_);

    std::this_thread::sleep_for(milliseconds(100));
    return true;
}

nanoseconds Audio::getClock() {
    std::lock_guard<std::mutex> lck(srcMutex_);
    return getClockNoLock();
}

nanoseconds Audio::getClockNoLock() {
    nanoseconds pts = currentPts_;

    if (source_) {
        ALint offset;
        alGetSourcei(source_, AL_SAMPLE_OFFSET, &offset);
        ALint queued, status;
        alGetSourcei(source_, AL_BUFFERS_QUEUED, &queued);
        alGetSourcei(source_, AL_SOURCE_STATE, &status);

        if (status != AL_STOPPED) {
            pts -= AudioBufferTime * queued;
            pts += nanoseconds{seconds{offset}} / codecctx_->sample_rate;
        }
    }

    return std::max(pts, nanoseconds::zero());
}
