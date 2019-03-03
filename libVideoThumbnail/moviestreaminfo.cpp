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

#include "moviestreaminfo.h"


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
using namespace ffmpegthumbnailer;

MovieStreamInfo::MovieStreamInfo(const string& filename, AVFormatContext* pavContext)
: m_VideoStream(-1)
, m_pFormatContext(pavContext)
, m_pVideoCodecContext(nullptr)
, m_pVideoCodec(nullptr)
, m_pVideoStream(nullptr)
, m_FormatContextWasGiven(pavContext != nullptr)
{
    initialize(filename);
}

MovieStreamInfo::~MovieStreamInfo()
{
    destroy();
}

int MovieStreamInfo::check_stream_specifier(AVFormatContext *s, AVStream *st, const char *spec)
{
    int ret = avformat_match_stream_specifier(s, st, spec);
    if (ret < 0)
        av_log(s, AV_LOG_ERROR, "Invalid stream specifier: %s.\n", spec);
    return ret;
}

AVDictionary * MovieStreamInfo::filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
                                AVFormatContext *s, AVStream *st, AVCodec *codec)
{
    AVDictionary    *ret = nullptr;
    AVDictionaryEntry *t = nullptr;
    int            flags = s->oformat ? AV_OPT_FLAG_ENCODING_PARAM
                                      : AV_OPT_FLAG_DECODING_PARAM;
    char          prefix = 0;
    const AVClass    *cc = avcodec_get_class();

    if (!codec)
        codec            = s->oformat ? avcodec_find_encoder(codec_id)
                                      : avcodec_find_decoder(codec_id);
    if (!codec)
        return nullptr;

    switch (codec->type) {
    case AVMEDIA_TYPE_VIDEO:
        prefix  = 'v';
        flags  |= AV_OPT_FLAG_VIDEO_PARAM;
        break;
    case AVMEDIA_TYPE_AUDIO:
        prefix  = 'a';
        flags  |= AV_OPT_FLAG_AUDIO_PARAM;
        break;
    case AVMEDIA_TYPE_SUBTITLE:
        prefix  = 's';
        flags  |= AV_OPT_FLAG_SUBTITLE_PARAM;
        break;
    }

    while (t = av_dict_get(opts, "", t, AV_DICT_IGNORE_SUFFIX)) {
        char *p = strchr(t->key, ':');

        /* check stream specification in opt name */
        if (p)
            switch (check_stream_specifier(s, st, p + 1)) {
            case  1: *p = 0; break;
            case  0:         continue;
            default:         return nullptr;
            }

        if (av_opt_find(&cc, t->key, nullptr, flags, AV_OPT_SEARCH_FAKE_OBJ) ||
            (codec && codec->priv_class &&
             av_opt_find(&codec->priv_class, t->key, nullptr, flags,
                         AV_OPT_SEARCH_FAKE_OBJ)))
            av_dict_set(&ret, t->key, t->value, 0);
        else if (t->key[0] == prefix &&
                 av_opt_find(&cc, t->key + 1, nullptr, flags,
                             AV_OPT_SEARCH_FAKE_OBJ))
            av_dict_set(&ret, t->key + 1, t->value, 0);

        if (p)
            *p = ':';
    }
    return ret;
}

AVDictionary ** MovieStreamInfo::setup_find_stream_info_opts(AVFormatContext *s, AVDictionary *codec_opts)
{
    int i;
    AVDictionary **opts;

    if (!s->nb_streams)
        return nullptr;
    opts = (AVDictionary **)av_mallocz(s->nb_streams * sizeof(*opts));
    if (!opts) {
        av_log(nullptr, AV_LOG_ERROR,
               "Could not alloc memory for stream options.\n");
        return nullptr;
    }
    for (i = 0; i < s->nb_streams; i++)
        opts[i] = filter_codec_opts(codec_opts, s->streams[i]->codec->codec_id,
                                    s, s->streams[i], nullptr);
    return opts;
}

void MovieStreamInfo::initialize(const string& filename)
{
	AVDictionary * codec_opts = nullptr;
    //av_register_all();
    //avcodec_register_all();
    //avformat_network_init();
	AVDictionary **opts;
    string inputFile = filename == "-" ? "pipe:" : filename;
    //m_AllowSeek = (filename != "-") && (filename.find("rtsp://") != 0) && (filename.find("udp://") != 0);
    

    if ((!m_FormatContextWasGiven) && avformat_open_input(&m_pFormatContext, inputFile.c_str(), nullptr, nullptr) != 0)
    {
        destroy();
        throw logic_error(string("Could not open input file: ") + filename);
    }


	opts = setup_find_stream_info_opts(m_pFormatContext, codec_opts);
	//orig_nb_streams = m_pFormatContext->nb_streams;
	// »ñÈ¡Á÷ÐÅÏ¢£¬¼ÓÈëAVFormatContextÖÐ
	//This is useful for file formats with no headers such as MPEG. 
	int err = avformat_find_stream_info(m_pFormatContext, opts);
	if (err < 0) {
        destroy();
        throw logic_error(string("Could not find stream information"));
	}

	/*
	if (avformat_find_stream_info(m_pFormatContext, nullptr) < 0)
    {
        destroy();
        throw logic_error(string("Could not find stream information"));
    }
	*/

    initializeVideo();

	 av_dict_free(&codec_opts);
}


void MovieStreamInfo::destroy()
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

    m_VideoStream = -1;
    
    //avformat_network_deinit();
}

vector<CStreamInfo>	MovieStreamInfo::GetListStreamInfo()
{
	return listStream;
}

void MovieStreamInfo::initializeVideo()
{
	int wanted_stream[AVMEDIA_TYPE_NB] = {-1,-1,0,-1,0};

		//m_pFormatContext->streams[i]->discard = AVDISCARD_ALL;
	int indexVideo = av_find_best_stream(m_pFormatContext, AVMEDIA_TYPE_VIDEO, wanted_stream[AVMEDIA_TYPE_VIDEO], -1, nullptr, 0);
	int indexAudio = av_find_best_stream(m_pFormatContext, AVMEDIA_TYPE_AUDIO, wanted_stream[AVMEDIA_TYPE_AUDIO], -1, nullptr, 0);
	int indexSubtitle = av_find_best_stream(m_pFormatContext, AVMEDIA_TYPE_SUBTITLE, wanted_stream[AVMEDIA_TYPE_SUBTITLE], -1, nullptr, 0);

	for (auto i = 0; i < m_pFormatContext->nb_streams; i++)
	{
		//const char * stream_type = av_get_media_type_string(m_pFormatContext->streams[i]->codec->codec_type);


		if(m_pFormatContext->streams[i]->codec->codec_type == 	AVMEDIA_TYPE_AUDIO)
		{
			AVDictionaryEntry * tag = nullptr;
			CStreamInfo streamInfo;
			streamInfo.streamLibelle = "Track Audio : " + to_string(i);
			while ((tag = av_dict_get(m_pFormatContext->streams[i]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
			{
				string key = tag->key;
				if(key == "language")
				{
					streamInfo.streamLibelle = tag->value;
					break;
				}
				//printf("STREAM_%s_%s=%s\n", stream_type,tag->key, tag->value);
			}
			if(indexAudio == i)
				streamInfo.selected = true;
			
			streamInfo.indexStream = i;	
			streamInfo.typeStream = 1;
			listStream.push_back(streamInfo);
		}
		else if(m_pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			AVDictionaryEntry * tag = nullptr;
			CStreamInfo streamInfo;
			streamInfo.streamLibelle = "Track Video : " + to_string(i);
			while ((tag = av_dict_get(m_pFormatContext->streams[i]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
			{
				string key = tag->key;
				if(key == "language")
				{
					streamInfo.streamLibelle = tag->value;
					break;
				}
			}

			streamInfo.indexStream = i;
			if(indexVideo == i)
				streamInfo.selected = true;
			streamInfo.typeStream = 2;
			listStream.push_back(streamInfo);
		}
		else if(m_pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_SUBTITLE)
		{
			AVDictionaryEntry * tag = nullptr;
			CStreamInfo streamInfo;
			streamInfo.streamLibelle = "Track Subtitle : " + to_string(i);
			while ((tag = av_dict_get(m_pFormatContext->streams[i]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
			{
				string key = tag->key;
				if(key == "language")
				{
					streamInfo.streamLibelle = tag->value;
					break;
				}
				//printf("STREAM_%s_%s=%s\n", stream_type,tag->key, tag->value);
			}

			if(indexSubtitle == i)
				streamInfo.selected = true;

			streamInfo.indexStream = i;
			streamInfo.typeStream = 3;
			listStream.push_back(streamInfo);
		}	
	}
}
