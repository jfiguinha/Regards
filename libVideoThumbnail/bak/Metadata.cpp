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

#include "Metadata.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

extern "C" {
#ifdef LATEST_GREATEST_FFMPEG	
#include <libavutil/opt.h>
#endif
#include <libswscale/swscale.h>
}

#include <libavutil/dict.h>
using namespace std;

namespace ffmpegthumbnailer
{

static const int SMART_FRAME_ATTEMPTS = 25;

Metadata::Metadata()
{
}



Metadata::~Metadata()
{
}

int Metadata::generateMetadata(const string &filename)
{
	AVFormatContext *fmt_ctx = NULL;
	AVDictionaryEntry *tag = NULL;
	int ret = 0;
	char message[1024];
	av_register_all();
	if ((ret = avformat_open_input(&fmt_ctx, filename.c_str(), NULL, NULL)))
		return ret;

#if DEBUG
	while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
	{
		printf("%s=%s\n", tag->key, tag->value);
	}
#endif
    
	avcodec_register_all();

	if (avformat_open_input(&fmt_ctx, filename.c_str(), nullptr, nullptr) == 0)
	{


	if (avformat_find_stream_info(fmt_ctx, nullptr) >= 0)
	{


	int numStream = 0;

	for (unsigned int i = 0; i < fmt_ctx->nb_streams; ++i)
	{
		if (fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			numStream = i;
			break;
		}
	}


	//Récupération des métadata du stream

	tag = NULL;

#if DEBUG
	while ((tag = av_dict_get(fmt_ctx->streams[numStream]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
	{
		//wxString test;
		printf("%s=%s\n", tag->key, tag->value);
		//wxString value = message;
		//test.assign(value.begin(), value.end());
		//OutputDebugString(test.c_str());
	}
#endif
    }
    }
end:

	avformat_close_input(&fmt_ctx);

	return ret;
}


}