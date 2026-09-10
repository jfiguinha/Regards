#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <functional>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
}

enum class AudioCodec {
    AAC,
    MP3,
    VORBIS
};

enum class EncodingMode {
    Bitrate,
    Quality
};

struct AudioEncoderOptions {
    AudioCodec codec = AudioCodec::AAC;
    EncodingMode mode = EncodingMode::Bitrate;
    int bitrateKbps = 128;
    float quality = 4.0f;
};

class AudioEncoder {
public:
    AudioEncoder();
    ~AudioEncoder();

    // Définition du prototype de la fonction de rappel (Callback)
    // Retourne 'true' pour continuer, 'false' pour annuler le traitement à la demande de l'utilisateur
    using ProgressCallback = std::function<bool(double currentSeconds, double totalDurationSeconds)>;

    // Fonction principale incluant désormais le paramètre de callback
    int EncodeAudioOnly(const std::string& inputPath,
        const std::string& outputPath,
        const AudioEncoderOptions& options,
        ProgressCallback progressCallback = nullptr);

private:
    int OpenInput(const std::string& inputPath);
    int OpenOutput(const std::string& outputPath, const AudioEncoderOptions& options);
    int InitResampler();
    int EncodeFrame(AVFrame* frame);
    void Cleanup();

    // FFmpeg Contexts
    AVFormatContext* ifmt_ctx = nullptr;
    AVFormatContext* ofmt_ctx = nullptr;
    AVCodecContext* dec_ctx = nullptr;
    AVCodecContext* enc_ctx = nullptr;
    SwrContext* swr_ctx = nullptr;

    // Index des flux
    int audioStreamIndexInput = -1;
    int audioStreamIndexOutput = -1;

    // Gestion des Timestamps (PTS) et de la progression
    int64_t currentPts = 0;
    double durationSeconds = 0.0;
};
