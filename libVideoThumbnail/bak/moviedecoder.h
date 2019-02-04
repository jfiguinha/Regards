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

#ifndef MOVIEDECODER_H
#define MOVIEDECODER_H

#include <inttypes.h>
#include <string>
#include <vector>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <config.h>
}
#if CONFIG_AVFILTER
struct AVFilterGraph;
struct AVFilterContext;
#endif
struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;
struct AVStream;
struct AVFrame;
struct AVPacket;
struct AVRational;

namespace ffmpegthumbnailer
{

struct VideoFrame;

class MovieDecoder
{
public:
    MovieDecoder(const std::string& filename, AVFormatContext* pavContext = nullptr);
    ~MovieDecoder();

    std::string getCodec();
    void seek(int timeInSeconds);
    void decodeVideoFrame();
    

    int getWidth();
    int getHeight();
	int getOrientation();
    int getDuration();
	void getScaledVideoFrame(int scaledSize, bool maintainAspectRatio, VideoFrame& videoFrame);
    void initialize(const std::string& filename);
    void destroy();
    
private:
    void initializeVideo();
    void checkRc(int ret, const std::string& message);
    bool decodeVideoPacket();
    bool getVideoPacket();
#if CONFIG_AVFILTER
    void initializeFilterGraph(const AVRational& timeBase, int size, bool maintainAspectRatio);
	std::string createScaleString(int size, bool maintainAspectRatio);
#else
	void convertAndScaleFrame(PixelFormat format, int scaledSize, bool maintainAspectRatio, int& scaledWidth, int& scaledHeight);
	 void calculateDimensions(int squareSize, bool maintainAspectRatio, int& destWidth, int& destHeight);
	 void createAVFrame(AVFrame** pAvFrame, uint8_t** pFrameBuffer, int width, int height, PixelFormat format);
#endif
    
   

	int32_t getStreamRotation();
private:
    int                     m_VideoStream;
    AVFormatContext*        m_pFormatContext;
    AVCodecContext*         m_pVideoCodecContext;
    AVCodec*                m_pVideoCodec;
#if CONFIG_AVFILTER
	AVFilterGraph*          m_pFilterGraph;
    AVFilterContext*        m_pFilterSource;
    AVFilterContext*        m_pFilterSink;
#endif
    AVStream*               m_pVideoStream;
    AVFrame*                m_pFrame;
    uint8_t*                m_pFrameBuffer;
    AVPacket*               m_pPacket;
    bool                    m_FormatContextWasGiven;
    bool                    m_AllowSeek;
};

}

#endif

