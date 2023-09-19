//    Copyright (C) 2010 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "header.h"

#include "moviedecoder.h"

#include "videoframe.h"

extern "C" {
#ifdef LATEST_GREATEST_FFMPEG	
#include <libavutil/display.h>
#include <libavutil/opt.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#endif
#include <libswscale/swscale.h>
}

#include <libavutil/dict.h>

#ifndef __APPLE__
#pragma warning(disable : 4996)
#endif

using namespace std;

namespace ffmpegthumbnailer
{

MovieDecoder::MovieDecoder(const string& filename, AVFormatContext* pavContext)
: m_VideoStream(-1)
, m_pFormatContext(pavContext)
, m_pVideoCodecContext(nullptr)
, m_pVideoCodec(nullptr)
, m_pVideoStream(nullptr)
, m_pFrame(nullptr)
, m_pFrameBuffer(nullptr)
, m_pPacket(nullptr)
, m_FormatContextWasGiven(pavContext != nullptr)
, m_AllowSeek(true)
{
    initialize(filename);
}

MovieDecoder::~MovieDecoder()
{
    destroy();
}

void MovieDecoder::initialize(const string& filename)
{
    //av_register_all();
    //avcodec_register_all();
    //avformat_network_init();

    string inputFile = filename == "-" ? "pipe:" : filename;
    m_AllowSeek = (filename != "-") && (filename.find("rtsp://") != 0) && (filename.find("udp://") != 0);
    

    if ((!m_FormatContextWasGiven) && avformat_open_input(&m_pFormatContext, inputFile.c_str(), nullptr, nullptr) != 0)
    {
        destroy();
        throw logic_error(string("Could not open input file: ") + filename);
    }

	if (avformat_find_stream_info(m_pFormatContext, nullptr) < 0)
    {
        destroy();
        throw logic_error(string("Could not find stream information"));
    }


    initializeVideo();
	
    m_pFrame = av_frame_alloc();


	widthVideo = m_pVideoCodecContext->width;
	heightVideo = m_pVideoCodecContext->height;
}

int MovieDecoder::getOrientation()
{
    int32_t* matrix = reinterpret_cast<int32_t*>(av_stream_get_side_data(m_pVideoStream, AV_PKT_DATA_DISPLAYMATRIX, nullptr));
    if (matrix)
        return lround(av_display_rotation_get(matrix));
    return -1;
}

void MovieDecoder::destroy()
{
    if (m_pVideoCodecContext)
    {
        avcodec_close(m_pVideoCodecContext);
        m_pVideoCodecContext = nullptr;
    }

    if ((!m_FormatContextWasGiven) && m_pFormatContext)
    {
        avformat_close_input(&m_pFormatContext);
    }

    if (m_pPacket)
    {
        av_packet_unref(m_pPacket);
        delete m_pPacket;
        m_pPacket = nullptr;
    }

    if (m_pFrame)
    {
        av_frame_free(&m_pFrame);
    }

    if (m_pFrameBuffer)
    {
        av_free(m_pFrameBuffer);
        m_pFrameBuffer = nullptr;
    }

    m_VideoStream = -1;
    
    //avformat_network_deinit();
}

string MovieDecoder::getCodec()
{
    if (m_pVideoCodec)
    {
        return m_pVideoCodec->name;
    }

    return "";
}

void MovieDecoder::initializeVideo()
{
    for (unsigned int i = 0; i < m_pFormatContext->nb_streams; ++i)
    {
        if (m_pFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            m_pVideoStream = m_pFormatContext->streams[i];
            m_VideoStream = i;
            break;
        }
    }

    if (m_VideoStream == -1)
    {
        throw logic_error("Could not find video stream");
    }

    m_pVideoCodecContext = m_pFormatContext->streams[m_VideoStream];
    m_pVideoCodec = (AVCodec *)avcodec_find_decoder(m_pFormatContext->streams[m_VideoStream]->codecpar->codec_id);

    if (m_pVideoCodec == nullptr)
    {
        // set to nullptr, otherwise avcodec_close(m_pVideoCodecContext) crashes
        m_pVideoCodecContext = nullptr;
        throw logic_error("Video Codec not found");
    }

    m_pVideoCodecContext->workaround_bugs = 1;

	if (avcodec_open2(m_pVideoCodecContext, m_pVideoCodec, nullptr) < 0)
    {
        throw logic_error("Could not open video codec");
    }

}

int MovieDecoder::getWidth()
{
    if (m_pVideoCodecContext)
    {
        return GetVideoWidth();
    }

    return -1;
}



int MovieDecoder::getHeight()
{
    if (m_pVideoCodecContext)
    {
        return GetVideoHeight();
    }

    return -1;
}

int MovieDecoder::getDuration()
{
    if (m_pFormatContext)
    {
        return static_cast<int>(m_pFormatContext->duration / (AV_TIME_BASE / 100));
    }

    return 0;
}

void MovieDecoder::seek(int timeInSeconds)
{
    if (!m_AllowSeek)
    {
        return;
    }
    
    int64_t timestamp = (AV_TIME_BASE / 100) * static_cast<int64_t>(timeInSeconds);

    if (timestamp < 0)
    {
        timestamp = 0;
    }

    int ret = av_seek_frame(m_pFormatContext, -1, timestamp, 0);
    if (ret >= 0)
    {
        avcodec_flush_buffers(m_pFormatContext->streams[m_VideoStream]->codecpar);
    }
    else
    {
        throw logic_error("Seeking in video failed");
    }

    int keyFrameAttempts = 0;
    bool gotFrame = 0;
    
    do
    {
        int count = 0;
        gotFrame = 0;

        while (!gotFrame && count < 20)
        {
            getVideoPacket();
            try
            {
                gotFrame = decodeVideoPacket();
            }
            catch(logic_error&) {}
            ++count;
        }
        
        ++keyFrameAttempts;
    } while ((!gotFrame || !m_pFrame->key_frame) && keyFrameAttempts < 200);
    
    if (gotFrame == 0)
    {
        throw logic_error("Seeking in video failed");
    }
}


void MovieDecoder::decodeVideoFrame()
{
    bool frameFinished = false;

    while(!frameFinished && getVideoPacket())
    {
        frameFinished = decodeVideoPacket();
    }

    if (!frameFinished)
    {
        throw logic_error("decodeVideoFrame() failed: frame not finished");
    }
}

bool MovieDecoder::decodeVideoPacket()
{
    if (m_pPacket->stream_index != m_VideoStream)
    {
        return false;
    }

    av_frame_unref(m_pFrame);

    int frameFinished;

    int bytesDecoded = avcodec_decode_video2(m_pVideoCodecContext, m_pFrame, &frameFinished, m_pPacket);
    if (bytesDecoded < 0)
    {
        throw logic_error("Failed to decode video frame: bytesDecoded < 0");
    }

    return frameFinished > 0;
}

bool MovieDecoder::getVideoPacket()
{
    bool framesAvailable = true;
    bool frameDecoded = false;

    int attempts = 0;
    
    if (m_pPacket)
    {
        av_free_packet(m_pPacket);
        delete m_pPacket;
    }

    m_pPacket = new AVPacket();

    while (framesAvailable && !frameDecoded && (attempts++ < 1000))
    {
        framesAvailable = av_read_frame(m_pFormatContext, m_pPacket) >= 0;
        if (framesAvailable)
        {
            frameDecoded = m_pPacket->stream_index == m_VideoStream;
            if (!frameDecoded)
            {
                av_free_packet(m_pPacket);
            }
        }
    }
    
    return frameDecoded;
}

void MovieDecoder::checkRc(int ret, const std::string& message)
{
    if (ret < 0)
    {
        char buf[256];
        buf[0] = ' ';
        av_strerror(ret, &buf[1], sizeof(buf) - 1);
        throw std::logic_error(message + buf);
    }
}
#if CONFIG_AVFILTER

int MovieDecoder::GetVideoWidth()
{
	return widthVideo;
}

int MovieDecoder::GetVideoHeight()
{
	return heightVideo;
}

void MovieDecoder::initializeFilterGraph(const AVRational& timeBase, int size, bool maintainAspectRatio)
{
    static const AVPixelFormat pixelFormats[] = { AV_PIX_FMT_BGRA, AV_PIX_FMT_NONE };

    auto del = [] (AVBufferSinkParams* p) { av_freep(p); };
    std::unique_ptr<AVBufferSinkParams, decltype(del)> buffersinkParams(av_buffersink_params_alloc(), del);

    m_pFilterGraph = avfilter_graph_alloc();
    assert(m_pFilterGraph);

    std::stringstream ss;
    ss << "video_size=" << GetVideoWidth() << "x" << GetVideoHeight()
       << ":pix_fmt=" << m_pVideoCodecContext->pix_fmt
       << ":time_base=" << timeBase.num << "/" << timeBase.den
       << ":pixel_aspect=" << m_pVideoCodecContext->sample_aspect_ratio.num << "/" << FFMAX(m_pVideoCodecContext->sample_aspect_ratio.den, 1);

    checkRc(avfilter_graph_create_filter(&m_pFilterSource, avfilter_get_by_name("buffer"), "thumb_buffer", ss.str().c_str(), nullptr, m_pFilterGraph),
            "Failed to create filter source");
    buffersinkParams->pixel_fmts = pixelFormats;
    checkRc(avfilter_graph_create_filter(&m_pFilterSink, avfilter_get_by_name("buffersink"), "thumb_buffersink", nullptr, buffersinkParams.get(), m_pFilterGraph),
            "Failed to create filter sink");
    buffersinkParams.release();

    AVFilterContext* yadifFilter = nullptr;
    if (m_pFrame->interlaced_frame != 0)
    {
        checkRc(avfilter_graph_create_filter(&yadifFilter, avfilter_get_by_name("yadif"), "thumb_deint", "deint=1", nullptr, m_pFilterGraph),
                "Failed to create deinterlace filter");
    }

    AVFilterContext* scaleFilter = nullptr;
    checkRc(avfilter_graph_create_filter(&scaleFilter, avfilter_get_by_name("scale"), "thumb_scale", createScaleString(size, maintainAspectRatio).c_str(), nullptr, m_pFilterGraph),
            "Failed to create scale filter");

    AVFilterContext* formatFilter = nullptr;
    checkRc(avfilter_graph_create_filter(&formatFilter, avfilter_get_by_name("format"), "thumb_format", "pix_fmts=bgra", nullptr, m_pFilterGraph),
            "Failed to create format filter");

	
    AVFilterContext* rotateFilter = nullptr;
    auto rotation = getStreamRotation();
    if (rotation != -1)
    {
        checkRc(avfilter_graph_create_filter(&rotateFilter, avfilter_get_by_name("transpose"), "thumb_rotate", to_string(rotation).c_str(), nullptr, m_pFilterGraph),
            "Failed to create rotate filter");
    }

    checkRc(avfilter_link(rotateFilter ? rotateFilter : formatFilter, 0, m_pFilterSink, 0), "Failed to link final filter");

    if (rotateFilter)
    {
        checkRc(avfilter_link(formatFilter, 0, rotateFilter, 0), "Failed to link format filter");
    }
	
    checkRc(avfilter_link(scaleFilter, 0, formatFilter, 0), "Failed to link scale filter");
	
    if (yadifFilter)
    {
        checkRc(avfilter_link(yadifFilter, 0, scaleFilter, 0), "Failed to link yadif filter");
    }

    checkRc(avfilter_link(m_pFilterSource, 0, yadifFilter ? yadifFilter : scaleFilter, 0), "Failed to link source filter");

    checkRc(avfilter_graph_config(m_pFilterGraph, nullptr), "Failed to configure filter graph");
}
#endif



int32_t MovieDecoder::getStreamRotation()
{
    int32_t* matrix = reinterpret_cast<int32_t*>(av_stream_get_side_data(m_pVideoStream, AV_PKT_DATA_DISPLAYMATRIX, nullptr));
    if (matrix)
    {
        auto angle = lround(av_display_rotation_get(matrix));
        if (angle > 45 && angle < 135)
        {
            return 2;
        }
        else if (angle < -45 && angle > -135)
        {
            return 1;
        }
    }

    return -1;
}

#if CONFIG_AVFILTER
std::string MovieDecoder::createScaleString(int size, bool maintainAspectRatio)
{
    std::stringstream scale;

    if (!maintainAspectRatio)
    {
        scale << "w=" << size << ":h=" << size;
    }
    else
    {
        auto width      = GetVideoWidth();
        auto height     = GetVideoHeight();

        AVRational par = av_guess_sample_aspect_ratio(m_pFormatContext, m_pVideoStream, m_pFrame);
        // if the pixel aspect ratio is defined and is not 1, we have an anamorphic stream
        bool anamorphic = par.num != 0 && par.num != par.den;

        if (anamorphic)
        {
            width = width * par.num / par.den;
        }

        if (height > width)
        {
            scale << "w=-1:h=" << (size == 0 ? height : size);
        }
        else
        {
            scale << "h=-1:w=" << (size == 0 ? width : size);
        }
    }

    return scale.str();
}
#endif



#if CONFIG_AVFILTER
void MovieDecoder::getScaledVideoFrame(int scaledSize, bool maintainAspectRatio, VideoFrame& videoFrame)
{
    initializeFilterGraph(m_pFormatContext->streams[m_VideoStream]->time_base, scaledSize, maintainAspectRatio);

    auto del = [] (AVFrame* f) { av_frame_free(&f); };
    std::unique_ptr<AVFrame, decltype(del)> res(av_frame_alloc(), del);

    checkRc(av_buffersrc_write_frame(m_pFilterSource, m_pFrame), "Failed to write frame to filter graph");

    int attempts = 0;
    int rc = av_buffersink_get_frame(m_pFilterSink, res.get());
    while (rc == AVERROR(EAGAIN) && attempts++ < 10)
    {
        decodeVideoFrame();
        checkRc(av_buffersrc_write_frame(m_pFilterSource, m_pFrame), "Failed to write frame to filter graph");
        rc = av_buffersink_get_frame(m_pFilterSink, res.get());
    }

    checkRc(rc, "Failed to get buffer from filter");

    videoFrame.width = res->width;
    videoFrame.height = res->height;
    videoFrame.lineSize = videoFrame.width * 4;

	if(videoFrame.frameData != nullptr)
		delete videoFrame.frameData;

	uint8_t * framedata = res->data[0];

	videoFrame.frameData = new uint8_t[videoFrame.width * 4 * videoFrame.height];
	for(int y = 0;y < videoFrame.height;y++)
	{
		memcpy(videoFrame.frameData + ((videoFrame.height - y - 1) * videoFrame.lineSize), framedata + (y * res->linesize[0]), videoFrame.lineSize);
	}

    if (m_pFilterGraph)
    {
        avfilter_graph_free(&m_pFilterGraph);
    }
}
#else

void MovieDecoder::getScaledVideoFrame(int scaledSize, bool maintainAspectRatio, VideoFrame& videoFrame)
{
	
    if (m_pFrame->interlaced_frame)
    {
        avpicture_deinterlace((AVPicture*) m_pFrame, (AVPicture*) m_pFrame, m_pVideoCodecContext->pix_fmt,
                              GetVideoWidth(), GetVideoHeigth());
    }

    int scaledWidth, scaledHeight;
    convertAndScaleFrame(PIX_FMT_RGBA, scaledSize, maintainAspectRatio, scaledWidth, scaledHeight);

    videoFrame.width = scaledWidth;
    videoFrame.height = scaledHeight;
    videoFrame.lineSize = m_pFrame->linesize[0];

	if(videoFrame.frameData != nullptr)
		delete videoFrame.frameData;

	videoFrame.frameData = new uint8_t[videoFrame.lineSize * videoFrame.height];
    memcpy(videoFrame.frameData, m_pFrame->data[0], videoFrame.lineSize * videoFrame.height);

}

void MovieDecoder::convertAndScaleFrame(PixelFormat format, int scaledSize, bool maintainAspectRatio, int& scaledWidth, int& scaledHeight)
{
    calculateDimensions(scaledSize, maintainAspectRatio, scaledWidth, scaledHeight);

#ifdef LATEST_GREATEST_FFMPEG
	// Enable this when it hits the released ffmpeg version
    SwsContext* scaleContext = sws_alloc_context();
    if (scaleContext == nullptr)
    {
		throw std::logic_error("Failed to allocate scale context");
	}
	av_opt_set_int(scaleContext, "srcw", GetVideoWidth(),0);
    av_opt_set_int(scaleContext, "srch", GetVideoHeigth(),0);
    av_opt_set_int(scaleContext, "src_format", m_pVideoCodecContext->pix_fmt,0);
    av_opt_set_int(scaleContext, "dstw", scaledWidth,0);
    av_opt_set_int(scaleContext, "dsth", scaledHeight,0);
    av_opt_set_int(scaleContext, "dst_format", format,0);
	av_opt_set_int(scaleContext, "sws_flags", SWS_BICUBIC,0);
	
	const int* coeff = sws_getCoefficients(SWS_CS_DEFAULT);
    if (sws_setColorspaceDetails(scaleContext, coeff, m_pVideoCodecContext->pix_fmt, coeff, format, 0, 1<<16, 1<<16) < 0)
    {
		sws_freeContext(scaleContext);
		throw std::logic_error("Failed to set colorspace details");
	}

	if (sws_init_context(scaleContext, nullptr, nullptr) < 0)
	{
		sws_freeContext(scaleContext);
		throw std::logic_error("Failed to initialise scale context");
	}
#else 

	SwsContext* scaleContext = sws_getContext(GetVideoWidth(), GetVideoHeigth(),
		m_pVideoCodecContext->pix_fmt, scaledWidth, scaledHeight,
		format, SWS_BICUBIC, nullptr, nullptr, nullptr);

#endif
    


    if (nullptr == scaleContext)
    {
        throw logic_error("Failed to create resize context");
    }

    AVFrame* convertedFrame = nullptr;
    uint8_t* convertedFrameBuffer = nullptr;

    createAVFrame(&convertedFrame, &convertedFrameBuffer, scaledWidth, scaledHeight, format);
    
    sws_scale(scaleContext, m_pFrame->data, m_pFrame->linesize, 0, GetVideoHeigth(),
              convertedFrame->data, convertedFrame->linesize);
    sws_freeContext(scaleContext);

    av_free(m_pFrame);
    av_free(m_pFrameBuffer);
    
    m_pFrame        = convertedFrame;
    m_pFrameBuffer  = convertedFrameBuffer;
}



void MovieDecoder::calculateDimensions(int squareSize, bool maintainAspectRatio, int& destWidth, int& destHeight)
{
    if (squareSize == 0)
    {
        // use original video size
        squareSize = max(GetVideoWidth(), GetVideoHeigth());
    }
    
    if (!maintainAspectRatio)
    {
        destWidth = squareSize;
        destHeight = squareSize;
    }
    else
    {
        int srcWidth            = GetVideoWidth();
        int srcHeight           = GetVideoHeigth();
        int ascpectNominator    = m_pVideoCodecContext->sample_aspect_ratio.num;
        int ascpectDenominator  = m_pVideoCodecContext->sample_aspect_ratio.den;
        
        if (ascpectNominator != 0 && ascpectDenominator != 0)
        {
            srcWidth = srcWidth * ascpectNominator / ascpectDenominator;
        }
        
        if (srcWidth > srcHeight)
        {
            destWidth  = squareSize;
            destHeight = static_cast<int>(static_cast<float>(squareSize) / srcWidth * srcHeight);
        }
        else
        {
            destWidth  = static_cast<int>(static_cast<float>(squareSize) / srcHeight * srcWidth);
            destHeight = squareSize;
        }
    }
}

void MovieDecoder::createAVFrame(AVFrame** pAvFrame, uint8_t** pFrameBuffer, int width, int height, PixelFormat format)
{
    *pAvFrame = av_frame_alloc();

    int numBytes = avpicture_get_size(format, width, height);
    *pFrameBuffer = reinterpret_cast<uint8_t*>(av_malloc(numBytes));
    avpicture_fill((AVPicture*) *pAvFrame, *pFrameBuffer, format, width, height);
}
#endif
}

