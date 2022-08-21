/*
 * Copyright (c) 2017 Jun Zhao
 * Copyright (c) 2017 Kaixuan Liu
 *
 * HW Acceleration API (video decoding) decode sample
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

 /**
  * @file
  * HW-Accelerated decoding example.
  *
  * @example hw_decode.c
  * This example shows how to do HW-accelerated decoding with output
  * frames from the HW video surfaces.
  */

#include <header.h>
#include "VideoPlayer.h"
#include <stdio.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <ConvertUtility.h>
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/pixdesc.h>
    #include <libavutil/hwcontext.h>
    #include <libavutil/opt.h>
    #include <libavutil/avassert.h>
    #include <libavutil/imgutils.h>
    #include "libswscale/swscale.h"
    #include <libavutil/display.h>
}

using namespace Regards::Video;


class CVideoPlayerPimpl
{
public:

    static AVBufferRef* hw_device_ctx;
    static enum AVPixelFormat hw_pix_fmt;
    SwsContext* localContext = nullptr;
    AVFormatContext* input_ctx = NULL;
    int video_stream, ret;
    AVStream* video = NULL;
    AVCodecContext* decoder_ctx = NULL;
    AVCodec* decoder = NULL;
    AVPacket* packet = NULL;
    enum AVHWDeviceType type;
    int i;
    cv::Mat videoFrame;
    int rotation = 0;
    bool hw_decode = false;
    int64_t duration = 0;
    int64_t nbFrames = 0;
    int64_t nbFps = 0;
    bool isOpen = false;
    int widthVideo = 0;
    int heightVideo = 0;
    int skipFrame = 0;

    void GetAspectRatio(int & ascpectNominator, int& ascpectDenominator)
    {
        ascpectNominator = decoder_ctx->sample_aspect_ratio.num;
        ascpectDenominator = decoder_ctx->sample_aspect_ratio.den;
    }

    int SeekToPos(const int& timeInSeconds)
    {

        int64_t timestamp = (AV_TIME_BASE) * static_cast<int64_t>(timeInSeconds);

        if (timestamp < 0)
        {
            timestamp = 0;
        }

        int ret = av_seek_frame(input_ctx, -1, timestamp, 0);

        if (ret >= 0)
        {
            avcodec_flush_buffers(decoder_ctx);
        }
        else
        {
            throw logic_error("Seeking in video failed");
        }
        

        /*
  
        //int64_t timeBase = (int64_t(decoder_ctx->time_base.num) * AV_TIME_BASE) / int64_t(decoder_ctx->time_base.den);
        int64_t seekTarget = (double)((double)sec) * (double)AV_TIME_BASE;
        if (seekTarget < duration)
        {
            AVRational time_base = input_ctx->streams[video_stream]->time_base;
            int64_t ts = av_rescale_q(sec, AV_TIME_BASE_Q, time_base);           

            ret = av_seek_frame(input_ctx, video_stream, ts, 0);

            if (ret >= 0)
            {
                avcodec_flush_buffers(decoder_ctx);
            }
            else
            {
                throw logic_error("Seeking in video failed");
            }

            return ret;

        }
        
        return -1;
        */

        //avcodec_flush_buffers(decoder_ctx);
    }

    CVideoPlayerPimpl()
    {

    }

    ~CVideoPlayerPimpl()
    {
        av_packet_free(&packet);
        avcodec_free_context(&decoder_ctx);
        avformat_close_input(&input_ctx);
        av_buffer_unref(&hw_device_ctx);
        if (localContext != nullptr)
            sws_freeContext(localContext);
        localContext = nullptr;
    }

    static int hw_decoder_init(AVCodecContext* ctx, const enum AVHWDeviceType type)
    {
        int err = 0;

        if ((err = av_hwdevice_ctx_create(&hw_device_ctx, type,
            NULL, NULL, 0)) < 0) {
            fprintf(stderr, "Failed to create specified HW device.\n");
            return err;
        }
        ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);

        return err;
    }

    static enum AVPixelFormat get_hw_format(AVCodecContext* ctx, const enum AVPixelFormat* pix_fmts)
    {
        const enum AVPixelFormat* p;

        for (p = pix_fmts; *p != -1; p++) {
            if (*p == hw_pix_fmt)
                return *p;
        }

        fprintf(stderr, "Failed to get HW surface format.\n");
        return AV_PIX_FMT_NONE;
    }

    int decode_write(AVCodecContext* avctx, AVPacket* packet, bool & decode_video)
    {
        decode_video = false;
        AVFrame* frame = NULL, * sw_frame = NULL;
        AVFrame* tmp_frame = NULL;
        uint8_t* buffer = NULL;
        int size;
        int ret = 0;

        ret = avcodec_send_packet(avctx, packet);
        if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            return ret;
        }

        while (1) {
            if (!(frame = av_frame_alloc()) || !(sw_frame = av_frame_alloc())) {
                fprintf(stderr, "Can not alloc frame\n");
                ret = AVERROR(ENOMEM);
                goto fail;
            }

            ret = avcodec_receive_frame(avctx, frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                av_frame_free(&frame);
                av_frame_free(&sw_frame);
                return 0;
            }
            else if (ret < 0) {
                fprintf(stderr, "Error while decoding\n");
                goto fail;
            }

            if (frame->format == hw_pix_fmt && hw_decode) {
                /* retrieve data from GPU to CPU */
                if ((ret = av_hwframe_transfer_data(sw_frame, frame, 0)) < 0) {
                    fprintf(stderr, "Error transferring the data to system memory\n");
                    goto fail;
                }
                tmp_frame = sw_frame;
            }
            else
                tmp_frame = frame;

            GetBitmapRGBA(tmp_frame);
            decode_video = true;

        fail:
            av_frame_free(&frame);
            av_frame_free(&sw_frame);
            av_freep(&buffer);
            if (ret < 0)
                return ret;
        }
    }


    void GetBitmapRGBA(AVFrame* tmp_frame)
    {
        if (localContext == nullptr)
        {
            localContext = sws_alloc_context();

            av_opt_set_int(localContext, "srcw", tmp_frame->width, 0);
            av_opt_set_int(localContext, "srch", tmp_frame->height, 0);
            av_opt_set_int(localContext, "src_format", tmp_frame->format, 0);
            av_opt_set_int(localContext, "dstw", tmp_frame->width, 0);
            av_opt_set_int(localContext, "dsth", tmp_frame->height, 0);
            av_opt_set_int(localContext, "dst_format", AV_PIX_FMT_RGB24, 0);
            av_opt_set_int(localContext, "sws_flags", SWS_FAST_BILINEAR, 0);

            if (sws_init_context(localContext, nullptr, nullptr) < 0)
            {
                sws_freeContext(localContext);
                throw std::logic_error("Failed to initialise scale context");
            }
        }

        int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, tmp_frame->width, tmp_frame->height, 16);
        uint8_t* convertedFrameBuffer = videoFrame.data;
        int linesize = videoFrame.step1();

        sws_scale(localContext, tmp_frame->data, tmp_frame->linesize, 0, tmp_frame->height,
            &convertedFrameBuffer, &linesize);
    }

    bool isOpened()
    {
        return isOpen;
    }

    double get_rotation(AVStream* st)
    {
        uint8_t* displaymatrix = av_stream_get_side_data(st,
            AV_PKT_DATA_DISPLAYMATRIX, NULL);
        double theta = 0;
        if (displaymatrix)
            theta = -av_display_rotation_get((int32_t*)displaymatrix);

        theta -= 360 * floor(theta / 360 + 0.9 / 360);

        if (fabs(theta - 90 * round(theta / 90)) > 2)
            av_log(NULL, AV_LOG_WARNING, "Odd rotation angle.\n"
                "If you want to help, upload a sample "
                "of this file to https://streams.videolan.org/upload/ "
                "and contact the ffmpeg-devel mailing list. (ffmpeg-devel@ffmpeg.org)");

        return theta;
    }


    int OpenVideoFile(const char* hardwareDevice, const char* videoFilename, const bool& hw_decode = true)
    {
        printf("Filename OpenVideoFile : %s \n", videoFilename);
        
        this->hw_decode = hw_decode;
        if (hw_decode)
        {
            type = av_hwdevice_find_type_by_name(hardwareDevice);
            if (type == AV_HWDEVICE_TYPE_NONE) {
                fprintf(stderr, "Device type %s is not supported.\n", hardwareDevice);
                fprintf(stderr, "Available device types:");
                while ((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE)
                    fprintf(stderr, " %s", av_hwdevice_get_type_name(type));
                fprintf(stderr, "\n");
                return -1;
            }

            packet = av_packet_alloc();
            if (!packet) {
                fprintf(stderr, "Failed to allocate AVPacket\n");
                return -1;
            }

            // open the input file 
            if (avformat_open_input(&input_ctx, videoFilename, NULL, NULL) != 0) {
                fprintf(stderr, "Cannot open input file '%s'\n", videoFilename);
                return -1;
            }

            if (avformat_find_stream_info(input_ctx, NULL) < 0) {
                fprintf(stderr, "Cannot find input stream information.\n");
                return -1;
            }

            // find the video stream information 
#ifdef WIN32
            ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, (const AVCodec**)&decoder, 0);
#else
            ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
#endif
            if (ret < 0) {
                fprintf(stderr, "Cannot find a video stream in the input file\n");
                return -1;
            }
            video_stream = ret;

            for (i = 0;; i++) {
                const AVCodecHWConfig* config = avcodec_get_hw_config(decoder, i);
                if (!config) {
                    fprintf(stderr, "Decoder %s does not support device type %s.\n",
                        decoder->name, av_hwdevice_get_type_name(type));
                    return -1;
                }
                if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
                    config->device_type == type) {
                    hw_pix_fmt = config->pix_fmt;
                    break;
                }
            }

            if (!(decoder_ctx = avcodec_alloc_context3(decoder)))
                return AVERROR(ENOMEM);


            video = input_ctx->streams[video_stream];
            if (avcodec_parameters_to_context(decoder_ctx, video->codecpar) < 0)
                return -1;

            decoder_ctx->get_format = get_hw_format;

            if (hw_decoder_init(decoder_ctx, type) < 0)
                return -1;

            if ((ret = avcodec_open2(decoder_ctx, decoder, NULL)) < 0) {
                fprintf(stderr, "Failed to open codec for stream #%u\n", video_stream);
                return -1;
            }
        }
        else
        {
            packet = av_packet_alloc();
            if (!packet) {
                fprintf(stderr, "Failed to allocate AVPacket\n");
                return -1;
            }

            // open the input file 
            if (avformat_open_input(&input_ctx, videoFilename, NULL, NULL) != 0) {
                fprintf(stderr, "Cannot open input file '%s'\n", videoFilename);
                return -1;
            }

            if (avformat_find_stream_info(input_ctx, NULL) < 0) {
                fprintf(stderr, "Cannot find input stream information.\n");
                return -1;
            }

            // find the video stream information 
#ifdef WIN32
            ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, (const AVCodec**)&decoder, 0);
#else
            ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
#endif
            if (ret < 0) {
                fprintf(stderr, "Cannot find a video stream in the input file\n");
                return -1;
            }
            video_stream = ret;


            if (!(decoder_ctx = avcodec_alloc_context3(decoder)))
                return AVERROR(ENOMEM);


            video = input_ctx->streams[video_stream];
            if (avcodec_parameters_to_context(decoder_ctx, video->codecpar) < 0)
                return -1;

            if ((ret = avcodec_open2(decoder_ctx, decoder, NULL)) < 0) {
                fprintf(stderr, "Failed to open codec for stream #%u\n", video_stream);
                return -1;
            }
        }

        rotation = get_rotation(video);
        widthVideo = decoder_ctx->width;
        heightVideo = decoder_ctx->height;
        duration = input_ctx->duration;
        nbFrames = input_ctx->streams[video_stream]->nb_frames;
        
        int durationInSec = (duration / AV_TIME_BASE);
        if(durationInSec > 0)
            nbFps = (double)nbFrames / durationInSec;
        else
            nbFps = 30;
        videoFrame = cv::Mat(cv::Size(widthVideo, heightVideo), CV_8UC3);
       
        cv::Mat temp = GetVideoFrame(true);
        isOpen = true;

        return 1;
    }

    int GetDuration()
    {
        return (duration / AV_TIME_BASE);
    }

    int SeekToBegin()
    {
        return avformat_seek_file(input_ctx, video_stream, INT64_MIN, 0, INT64_MAX, 0);
    }

    cv::Mat GetVideoFrame(const bool &applyOrientation)
    {
        bool decode_video = false;
        bool exit_white = false;

       
        int nbFrame = 0;
       do
        {
           decode_video = false;
            
            if ((ret = av_read_frame(input_ctx, packet)) < 0)
            {
                return cv::Mat();
                break;
            }

            if (video_stream == packet->stream_index)
            {
                decode_write(decoder_ctx, packet, decode_video);
                exit_white = true;
            }

            av_packet_unref(packet);

       } while (!exit_white || !decode_video);

       if (!decode_video)
           return cv::Mat();

       if (applyOrientation && rotation != 0)
       {
           cv::Mat src = videoFrame;
           cv::Mat dst;
           //Rotate Frame
           if (rotation == 90)
           {
               cv::rotate(src, dst, cv::ROTATE_90_CLOCKWISE);
               // Rotate clockwise 270 degrees
              // transpose(src, dst);
              // flip(dst, src, 0);
           }
           else if (rotation == 180)
           {
               // Rotate clockwise 180 degrees
               flip(src, src, -1);
           }
           else if (rotation == 270)
           {
               cv::rotate(src, dst, cv::ROTATE_90_COUNTERCLOCKWISE);
               // Rotate clockwise 90 degrees
              // transpose(src, dst);
              // flip(dst, src, 1);
           }
           return dst;
       }

        return videoFrame;
    }

    int GetOrientation()
    {
        return rotation;
    }

    void SkipFrame(const int& nbFrame)
    {
        skipFrame = nbFrame;
    }
};

AVBufferRef* CVideoPlayerPimpl::hw_device_ctx = NULL;
enum AVPixelFormat CVideoPlayerPimpl::hw_pix_fmt;

void CVideoPlayer::SeekToBegin()
{
    pimpl->SeekToBegin();
}

void CVideoPlayer::SkipFrame(const int& nbFrame)
{
    pimpl->SkipFrame(nbFrame);
}

bool CVideoPlayer::IsOk()
{
    return isOk;
}

int CVideoPlayer::GetFps()
{
    return pimpl->nbFps;
}

int CVideoPlayer::GetTotalFrame()
{
    return pimpl->nbFrames;
}

int CVideoPlayer::GetWidth()
{
    return pimpl->widthVideo;
}

int CVideoPlayer::GetHeight()
{
    return pimpl->heightVideo;
}

void CVideoPlayer::GetAspectRatio(int& ascpectNominator, int& ascpectDenominator)
{
    return pimpl->GetAspectRatio(ascpectNominator, ascpectDenominator);
}

int CVideoPlayer::GetDuration()
{
    return pimpl->GetDuration();
}

int CVideoPlayer::GetOrientation()
{
    return pimpl->GetOrientation();
}

bool CVideoPlayer::isOpened()
{
    return pimpl->isOpened();
}

int CVideoPlayer::SeekToPos(const int& sec)
{
    return pimpl->SeekToPos(sec);
}

CVideoPlayer::CVideoPlayer(const wxString& filename, const bool& useHardware)
{
    int ret = 0;
    pimpl = new CVideoPlayerPimpl();
    wxString decoderHardware = "";

    if (useHardware)
    {
        CRegardsConfigParam* config = CParamInit::getInstance();
        if (config != nullptr)
        {
            decoderHardware = config->GetHardwareDecoder();
        }

        if (decoderHardware != "")
        {
            ret = pimpl->OpenVideoFile(CConvertUtility::ConvertToUTF8(decoderHardware), CConvertUtility::ConvertToUTF8(filename));
            delete pimpl;
            pimpl = new CVideoPlayerPimpl();
        }

        if (ret <= 0)
            ret = pimpl->OpenVideoFile("", CConvertUtility::ConvertToUTF8(filename), false);
    }
    else
        ret = pimpl->OpenVideoFile("", CConvertUtility::ConvertToUTF8(filename), useHardware);

    if (ret > 0)
        isOk = true;
}

cv::Mat CVideoPlayer::GetVideoFrame(const bool& applyOrientation)
{
    if(isOk)
        return pimpl->GetVideoFrame(applyOrientation);
    return cv::Mat();
}

CVideoPlayer::~CVideoPlayer()
{
    delete pimpl;
}