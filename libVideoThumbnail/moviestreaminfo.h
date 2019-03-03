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

#define CONFIG_AVFILTER 1

#include <inttypes.h>
#include <StreamInfo.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
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

	class MovieStreamInfo
	{
	public:
		MovieStreamInfo(const std::string& filename, AVFormatContext* pavContext = nullptr);
		~MovieStreamInfo();  
		vector<CStreamInfo>	GetListStreamInfo();
	private:
		void initializeVideo();
		void initialize(const std::string& filename);
		void destroy();
		int check_stream_specifier(AVFormatContext *s, AVStream *st, const char *spec);
		AVDictionary * filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
                                AVFormatContext *s, AVStream *st, AVCodec *codec);
		AVDictionary ** setup_find_stream_info_opts(AVFormatContext *s, AVDictionary *codec_opts);
		int                     m_VideoStream;
		AVFormatContext*        m_pFormatContext;
		AVCodecContext*         m_pVideoCodecContext;
		AVCodec*                m_pVideoCodec;
		AVStream*               m_pVideoStream;
		bool                    m_FormatContextWasGiven;
		vector<CStreamInfo>		listStream;
	};

}

#endif

