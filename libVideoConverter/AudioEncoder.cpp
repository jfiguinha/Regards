#include <header.h>
#include "AudioEncoder.h"

AudioEncoder::AudioEncoder() {}

AudioEncoder::~AudioEncoder() {
    Cleanup();
}

void AudioEncoder::Cleanup() {
    if (swr_ctx) swr_free(&swr_ctx);
    if (enc_ctx) avcodec_free_context(&enc_ctx);
    if (dec_ctx) avcodec_free_context(&dec_ctx);
    if (ifmt_ctx) avformat_close_input(&ifmt_ctx);
    if (ofmt_ctx) {
        if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
            avio_closep(&ofmt_ctx->pb);
        }
        avformat_free_context(ofmt_ctx);
    }
    ifmt_ctx = nullptr;
    ofmt_ctx = nullptr;
    dec_ctx = nullptr;
    enc_ctx = nullptr;
    swr_ctx = nullptr;
    audioStreamIndexInput = -1;
    audioStreamIndexOutput = -1;
    currentPts = 0;
    durationSeconds = 0.0;
}

int AudioEncoder::OpenInput(const std::string& inputPath) {
    int ret = avformat_open_input(&ifmt_ctx, inputPath.c_str(), nullptr, nullptr);
    if (ret < 0) return ret;

    ret = avformat_find_stream_info(ifmt_ctx, nullptr);
    if (ret < 0) return ret;

    for (unsigned int i = 0; i < ifmt_ctx->nb_streams; i++) {
        if (ifmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndexInput = i;
            break;
        }
    }

    if (audioStreamIndexInput == -1) {
        return AVERROR_STREAM_NOT_FOUND;
    }

    AVStream* inStream = ifmt_ctx->streams[audioStreamIndexInput];

    // Calcul de la durée totale du fichier en secondes
    if (ifmt_ctx->duration != AV_NOPTS_VALUE) {
        durationSeconds = static_cast<double>(ifmt_ctx->duration) / AV_TIME_BASE;
    }
    else if (inStream->duration != AV_NOPTS_VALUE) {
        durationSeconds = static_cast<double>(inStream->duration) * av_q2d(inStream->time_base);
    }

    const AVCodec* decoder = avcodec_find_decoder(inStream->codecpar->codec_id);
    if (!decoder) return AVERROR_DECODER_NOT_FOUND;

    dec_ctx = avcodec_alloc_context3(decoder);
    if (!dec_ctx) return AVERROR(ENOMEM);

    ret = avcodec_parameters_to_context(dec_ctx, inStream->codecpar);
    if (ret < 0) return ret;

    ret = avcodec_open2(dec_ctx, decoder, nullptr);
    return ret;
}

int AudioEncoder::OpenOutput(const std::string& outputPath, const AudioEncoderOptions& options) {
    int ret = avformat_alloc_output_context2(&ofmt_ctx, nullptr, nullptr, outputPath.c_str());
    if (ret < 0) return ret;

    AVCodecID targetCodecId = AV_CODEC_ID_AAC;
    std::string encoderName = "";

    switch (options.codec) {
    case AudioCodec::MP3:
        targetCodecId = AV_CODEC_ID_MP3;
        encoderName = "libmp3lame";
        break;
    case AudioCodec::VORBIS:
        targetCodecId = AV_CODEC_ID_VORBIS;
        encoderName = "libvorbis";
        break;
    case AudioCodec::AAC:
    default:
        targetCodecId = AV_CODEC_ID_AAC;
        break;
    }

    const AVCodec* encoder = encoderName.empty() ?
        avcodec_find_encoder(targetCodecId) : avcodec_find_encoder_by_name(encoderName.c_str());

    if (!encoder) return AVERROR_ENCODER_NOT_FOUND;

    AVStream* outStream = avformat_new_stream(ofmt_ctx, nullptr);
    if (!outStream) return AVERROR(ENOMEM);
    audioStreamIndexOutput = outStream->index;

    enc_ctx = avcodec_alloc_context3(encoder);
    if (!enc_ctx) return AVERROR(ENOMEM);

    enc_ctx->sample_rate = dec_ctx->sample_rate;
    av_channel_layout_copy(&enc_ctx->ch_layout, &dec_ctx->ch_layout);
    enc_ctx->time_base = { 1, enc_ctx->sample_rate };
    enc_ctx->sample_fmt = encoder->sample_fmts[0];

    if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER) {
        enc_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    AVDictionary* encoderOpts = nullptr;
    if (options.mode == EncodingMode::Bitrate) {
        enc_ctx->bit_rate = options.bitrateKbps * 1000;
    }
    else {
        enc_ctx->flags |= AV_CODEC_FLAG_QSCALE;
        if (targetCodecId == AV_CODEC_ID_AAC) {
            std::string vbrVal = std::to_string(static_cast<int>(options.quality));
            av_dict_set(&encoderOpts, "vbr", vbrVal.c_str(), 0);
        }
        else if (targetCodecId == AV_CODEC_ID_MP3 || targetCodecId == AV_CODEC_ID_VORBIS) {
            enc_ctx->global_quality = static_cast<int>(options.quality * FF_QP2LAMBDA);
            if (targetCodecId == AV_CODEC_ID_MP3) {
                std::string qVal = std::to_string(static_cast<int>(options.quality));
                av_dict_set(&encoderOpts, "q", qVal.c_str(), 0);
            }
        }
    }

    ret = avcodec_open2(enc_ctx, encoder, &encoderOpts);
    av_dict_free(&encoderOpts);
    if (ret < 0) return ret;

    ret = avcodec_parameters_from_context(outStream->codecpar, enc_ctx);
    if (ret < 0) return ret;

    if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, outputPath.c_str(), AVIO_FLAG_WRITE);
        if (ret < 0) return ret;
    }

    ret = avformat_write_header(ofmt_ctx, nullptr);
    return ret;
}

int AudioEncoder::InitResampler() {
    int ret = swr_alloc_set_opts2(&swr_ctx,
        &enc_ctx->ch_layout, enc_ctx->sample_fmt, enc_ctx->sample_rate,
        &dec_ctx->ch_layout, dec_ctx->sample_fmt, dec_ctx->sample_rate,
        0, nullptr);

    if (ret < 0 || !swr_ctx) return ret;
    return swr_init(swr_ctx);
}

int AudioEncoder::EncodeFrame(AVFrame* frame) {
    int ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) return ret;

    AVPacket* pkt = av_packet_alloc();
    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            ret = 0;
            break;
        }
        else if (ret < 0) {
            break;
        }

        pkt->stream_index = audioStreamIndexOutput;
        av_packet_rescale_ts(pkt, enc_ctx->time_base, ofmt_ctx->streams[audioStreamIndexOutput]->time_base);

        ret = av_interleaved_write_frame(ofmt_ctx, pkt);
        av_packet_unref(pkt);
        if (ret < 0) break;
    }
    av_packet_free(&pkt);
    return ret;
}

int AudioEncoder::EncodeAudioOnly(const std::string& inputPath,
    const std::string& outputPath,
    const AudioEncoderOptions& options,
    ProgressCallback progressCallback) {
    Cleanup();

    int ret = OpenInput(inputPath);
    if (ret < 0) return ret;

    ret = OpenOutput(outputPath, options);
    if (ret < 0) return ret;

    ret = InitResampler();
    if (ret < 0) return ret;

    AVPacket* packet = av_packet_alloc();
    AVFrame* decFrame = av_frame_alloc();
    AVFrame* encFrame = av_frame_alloc();

    AVStream* inStream = ifmt_ctx->streams[audioStreamIndexInput];

    while (av_read_frame(ifmt_ctx, packet) >= 0) {
        if (packet->stream_index == audioStreamIndexInput) {
            ret = avcodec_send_packet(dec_ctx, packet);
            if (ret < 0) break;

            while (ret >= 0) {
                ret = avcodec_receive_frame(dec_ctx, decFrame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    ret = 0;
                    break;
                }
                else if (ret < 0) {
                    break;
                }

                // Envoi des informations de progression au callback à chaque frame traitée
                if (progressCallback && decFrame->pts != AV_NOPTS_VALUE) {
                    double currentSeconds = static_cast<double>(decFrame->pts) * av_q2d(inStream->time_base);

                    // Si l'utilisateur clique sur Annuler dans l'interface, le callback renverra false
                    if (!progressCallback(currentSeconds, durationSeconds)) {
                        ret = AVERROR_EXIT; // Sortie forcée par l'utilisateur
                        break;
                    }
                }

                encFrame->sample_rate = enc_ctx->sample_rate;
                encFrame->format = enc_ctx->sample_fmt;
                av_channel_layout_copy(&encFrame->ch_layout, &enc_ctx->ch_layout);
                encFrame->nb_samples = enc_ctx->frame_size > 0 ? enc_ctx->frame_size : decFrame->nb_samples;

                ret = av_frame_get_buffer(encFrame, 0);
                if (ret < 0) break;

                ret = swr_convert(swr_ctx,
                    encFrame->data, encFrame->nb_samples,
                    (const uint8_t**)decFrame->data, decFrame->nb_samples);
                if (ret < 0) break;

                encFrame->pts = currentPts;
                currentPts += encFrame->nb_samples;

                ret = EncodeFrame(encFrame);
                av_frame_unref(encFrame);
                if (ret < 0) break;
            }
        }
        av_packet_unref(packet);
        if (ret < 0) break;
    }

    if (ret >= 0) {
        EncodeFrame(nullptr);
        av_write_trailer(ofmt_ctx);
    }

    av_frame_free(&encFrame);
    av_frame_free(&decFrame);
    av_packet_free(&packet);

    int finalRet = ret;
    Cleanup();
    return finalRet;
}
