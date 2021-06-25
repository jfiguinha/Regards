#pragma once

extern "C" {
#include "libavutil/avstring.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

enum HWAccelID
{
	HWACCEL_NONE = 0,
	HWACCEL_AUTO,
	HWACCEL_VDPAU,
	HWACCEL_DXVA2,
	HWACCEL_VDA,
	HWACCEL_VIDEOTOOLBOX,
	HWACCEL_QSV,
	HWACCEL_VAAPI,
	HWACCEL_CUVID,
};

using InputStream = struct InputStream
{
	int file_index;
	AVStream* st;
	int discard; //true if stream data should be discarded 
	int user_set_discard;
	int decoding_needed; // non zero if the packets must be decoded in 'raw_fifo', see DECODING_FOR_
#define DECODING_FOR_OST    1
#define DECODING_FOR_FILTER 2

	AVCodecContext* dec_ctx;
	AVCodec* dec;
	AVFrame* decoded_frame;
	AVFrame* filter_frame; // a ref of decoded_frame, to be sent to filters 

	int64_t start; /* time when read started */
	/* predicted dts of the next packet read for this stream or (when there are
        * several frames in a packet) of the next frame in current packet (in AV_TIME_BASE units) */
	int64_t next_dts;
	int64_t dts; ///< dts of the last packet read for this stream (in AV_TIME_BASE units)

	int64_t next_pts; ///< synthetic pts for the next decode frame (in AV_TIME_BASE units)
	int64_t pts; ///< current pts of the decoded frame  (in AV_TIME_BASE units)
	int wrap_correction_done;

	int64_t filter_in_rescale_delta_last;

	int64_t min_pts; /* pts with the smallest value in a current stream */
	int64_t max_pts; /* pts with the higher value in a current stream */
	int64_t nb_samples; /* number of samples in the last decoded audio frame before looping */

	double ts_scale;
	int saw_first_ts;
	int showed_multi_packet_warning;
	AVDictionary* decoder_opts;
	AVRational framerate; /* framerate forced with -r */
	int top_field_first;
	int guess_layout_max;

	int autorotate;
	int resample_height;
	int resample_width;
	int resample_pix_fmt;

	int resample_sample_fmt;
	int resample_sample_rate;
	int resample_channels;
	uint64_t resample_channel_layout;

	int fix_sub_duration;

	struct
	{
		/* previous decoded subtitle and related variables */
		int got_output;
		int ret;
		AVSubtitle subtitle;
	} prev_sub;

	struct sub2video
	{
		int64_t last_pts;
		int64_t end_pts;
		AVFrame* frame;
		int w, h;
	} sub2video;

	int dr1;

	/* decoded data from this stream goes into all those filters
        * currently video and audio only */
	//InputFilter **filters;
	//int        nb_filters;

	//int reinit_filters;

	/* hwaccel options */
	HWAccelID hwaccel_id;
	char* hwaccel_device;
	AVPixelFormat hwaccel_output_format;

	/* hwaccel context */
	void* hwaccel_ctx;
	void (*hwaccel_uninit)(AVCodecContext* s);
	int (*hwaccel_get_buffer)(AVCodecContext* s, AVFrame* frame, int flags);
	int (*hwaccel_retrieve_data)(AVCodecContext* s, AVFrame* frame);
	AVPixelFormat hwaccel_pix_fmt;
	AVPixelFormat hwaccel_retrieved_pix_fmt;
	AVBufferRef* hw_frames_ctx;

	/* stats */
	// combined size of all the packets read
	uint64_t data_size;
	/* number of packets successfully read for this stream */
	uint64_t nb_packets;
	// number of frames/samples retrieved from the decoder
	uint64_t frames_decoded;
	uint64_t samples_decoded;
};
