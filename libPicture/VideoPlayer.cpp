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

extern "C" {
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


	int video_stream, ret;

	AVCodecContext* decoder_ctx = nullptr;
	//AVCodec* decoder = nullptr;
	AVPacket* packet = nullptr;
	SwsContext* localContext = nullptr;
	AVFormatContext* input_ctx = nullptr;
    
    
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

	void GetAspectRatio(int& ascpectNominator, int& ascpectDenominator)
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

		int ret = av_seek_frame(input_ctx, -1, timestamp, AVSEEK_FLAG_FRAME);

        
		if (ret >= 0)
		{
			avcodec_flush_buffers(decoder_ctx);
		}
		else
		{
			throw logic_error("Seeking in video failed");
		}
    
        return -1;

	}

	CVideoPlayerPimpl()
	{
	}

	~CVideoPlayerPimpl()
	{
		av_packet_free(&packet);
		avcodec_free_context(&decoder_ctx);
		avformat_close_input(&input_ctx);
		if (localContext != nullptr)
			sws_freeContext(localContext);
		localContext = nullptr;
	}


	int decode_write(AVCodecContext* avctx, AVPacket* packet, bool& decode_video)
	{
		decode_video = false;
		AVFrame *frame = nullptr;

		ret = avcodec_send_packet(avctx, packet);
		if (ret < 0)
		{
			fprintf(stderr, "Error during decoding\n");
			return ret;
		}

		while (true)
		{
			if (!(frame = av_frame_alloc()))
			{
				fprintf(stderr, "Can not alloc frame\n");
				ret = AVERROR(ENOMEM);
				goto fail;
			}

			ret = avcodec_receive_frame(avctx, frame);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			{
				av_frame_free(&frame);
				return 0;
			}
			if (ret < 0)
			{
				fprintf(stderr, "Error while decoding\n");
				goto fail;
			}

			GetBitmapRGBA(frame);
			decode_video = true;

		fail:
			av_frame_free(&frame);
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
			av_opt_set_int(localContext, "dst_format", AV_PIX_FMT_BGRA, 0);
			av_opt_set_int(localContext, "sws_flags", SWS_FAST_BILINEAR, 0);

			if (sws_init_context(localContext, nullptr, nullptr) < 0)
			{
				sws_freeContext(localContext);
				throw std::logic_error("Failed to initialise scale context");
			}
		}

		int numBytes = av_image_get_buffer_size(AV_PIX_FMT_BGRA, tmp_frame->width, tmp_frame->height, 16);
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
		                                                 AV_PKT_DATA_DISPLAYMATRIX, nullptr);
		double theta = 0;
		if (displaymatrix)
			theta = -av_display_rotation_get((int32_t*)displaymatrix);

		theta -= 360 * floor(theta / 360 + 0.9 / 360);

		if (fabs(theta - 90 * round(theta / 90)) > 2)
			av_log(nullptr, AV_LOG_WARNING, "Odd rotation angle.\n"
			       "If you want to help, upload a sample "
			       "of this file to https://streams.videolan.org/upload/ "
			       "and contact the ffmpeg-devel mailing list. (ffmpeg-devel@ffmpeg.org)");

		return theta;
	}


	int OpenVideoFile(const char* videoFilename)
	{
        AVCodec* decoder = nullptr;
		printf("Filename OpenVideoFile : %s \n", videoFilename);

        packet = av_packet_alloc();
        if (!packet)
        {
            fprintf(stderr, "Failed to allocate AVPacket\n");
            return -1;
        }

        // open the input file 
        if (avformat_open_input(&input_ctx, videoFilename, nullptr, nullptr) != 0)
        {
            fprintf(stderr, "Cannot open input file '%s'\n", videoFilename);
            return -1;
        }

        if (avformat_find_stream_info(input_ctx, nullptr) < 0)
        {
            fprintf(stderr, "Cannot find input stream information.\n");
            return -1;
        }

        ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, (const AVCodec**)&decoder, 0);

        if (ret < 0)
        {
            fprintf(stderr, "Cannot find a video stream in the input file\n");
            return -1;
        }
        video_stream = ret;


        if (!(decoder_ctx = avcodec_alloc_context3(decoder)))
            return AVERROR(ENOMEM);


        AVStream * video = input_ctx->streams[video_stream];
        if (avcodec_parameters_to_context(decoder_ctx, video->codecpar) < 0)
            return -1;

        if ((ret = avcodec_open2(decoder_ctx, decoder, nullptr)) < 0)
        {
            fprintf(stderr, "Failed to open codec for stream #%u\n", video_stream);
            return -1;
        }

		rotation = get_rotation(video);
		widthVideo = decoder_ctx->width;
		heightVideo = decoder_ctx->height;
		duration = input_ctx->duration;
		nbFrames = input_ctx->streams[video_stream]->nb_frames;

		int durationInSec = (duration / AV_TIME_BASE);
		if (durationInSec > 0)
			nbFps = static_cast<double>(nbFrames) / durationInSec;
		else
			nbFps = 30;
		
        videoFrame = cv::Mat(cv::Size(widthVideo, heightVideo), CV_8UC4);

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

	cv::Mat GetVideoFrame(const bool& applyOrientation, const bool& invertRotation)
	{
		if (input_ctx == nullptr)
			return cv::Mat();

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
		}
		while (!exit_white || !decode_video);

		if (!decode_video)
			return cv::Mat();

		if (applyOrientation && rotation != 0)
		{
			cv::Mat src = videoFrame;
			cv::Mat dst;
			//Rotate Frame
			if (rotation == 270)
			{
				if(invertRotation)
					cv::rotate(src, dst, cv::ROTATE_90_CLOCKWISE);
				else
					cv::rotate(src, dst, cv::ROTATE_90_COUNTERCLOCKWISE);
				// Rotate clockwise 270 degrees
				// transpose(src, dst);
				// flip(dst, src, 0);
			}
			else if (rotation == 180)
			{
				// Rotate clockwise 180 degrees
				flip(src, src, -1);
			}
			else if (rotation == 90)
			{
				if (invertRotation)
					cv::rotate(src, dst, cv::ROTATE_90_COUNTERCLOCKWISE);
				else
					cv::rotate(src, dst, cv::ROTATE_90_CLOCKWISE);
				
				// Rotate clockwise 90 degrees
				// transpose(src, dst);
				// flip(dst, src, 1);
			}
            else 
                return videoFrame;
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

void CVideoPlayer::SeekToBegin()
{
	if (IsOk())
		pimpl->SeekToBegin();
	/*
	if (pimpl != nullptr)
		delete pimpl;
	pimpl = new CVideoPlayerPimpl();
	int ret = pimpl->OpenVideoFile(CConvertUtility::ConvertToUTF8(filename));
	*/

}

void CVideoPlayer::SkipFrame(const int& nbFrame)
{
	if (IsOk())
		pimpl->SkipFrame(nbFrame);
}

bool CVideoPlayer::IsOk()
{
	return pimpl->isOpened();
}

int CVideoPlayer::GetFps()
{
	if (IsOk())
		return pimpl->nbFps;
	return -1;
}

int CVideoPlayer::GetTotalFrame()
{
	if (IsOk())
		return pimpl->nbFrames;
	return -1;
}

int CVideoPlayer::GetWidth()
{
	if (IsOk())
		return pimpl->widthVideo;
	return -1;
}

int CVideoPlayer::GetHeight()
{
	if (IsOk())
		return pimpl->heightVideo;
	return -1;
}

void CVideoPlayer::GetAspectRatio(int& ascpectNominator, int& ascpectDenominator)
{
	if (IsOk())
		pimpl->GetAspectRatio(ascpectNominator, ascpectDenominator);
}

int CVideoPlayer::GetDuration()
{
	if (IsOk())
		return pimpl->GetDuration();
	return -1;
}

int CVideoPlayer::GetOrientation()
{
	if (IsOk())
		return pimpl->GetOrientation();
	return -1;
}

bool CVideoPlayer::isOpened()
{
	return pimpl->isOpened();
}

int CVideoPlayer::SeekToPos(const int& sec)
{
	if (IsOk())
		return pimpl->SeekToPos(sec);
	return -1;
}

CVideoPlayer::CVideoPlayer(const wxString& filename) : IVideoPlayer(filename)
{
	int ret = 0;
	pimpl = new CVideoPlayerPimpl();
	ret = pimpl->OpenVideoFile(CConvertUtility::ConvertToUTF8(filename));
	this->filename = filename;
}

cv::Mat CVideoPlayer::GetVideoFrame(const bool& applyOrientation, const bool& invertRotation)
{
	if (IsOk())
	{
		cv::Mat mat = pimpl->GetVideoFrame(applyOrientation, invertRotation);
		return mat;
	}
	return cv::Mat();
}

CVideoPlayer::~CVideoPlayer()
{
	if(pimpl != nullptr)
		delete pimpl;
}
