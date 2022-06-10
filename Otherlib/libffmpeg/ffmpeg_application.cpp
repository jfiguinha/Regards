#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/progdlg.h>
#include <stdexcept>
#include "ffmpeg_application.h"

extern "C" {
#include "ffmpeg.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libavutil/avassert.h>
#include <libavutil/imgutils.h>
}

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

#include <stdio.h>


extern "C"
{
	static AVBufferRef* hw_device_ctx = NULL;
	static enum AVPixelFormat hw_pix_fmt;
	static FILE* output_file = NULL;

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

	static enum AVPixelFormat get_hw_format(AVCodecContext* ctx,
		const enum AVPixelFormat* pix_fmts)
	{
		const enum AVPixelFormat* p;

		for (p = pix_fmts; *p != -1; p++) {
			if (*p == hw_pix_fmt)
				return *p;
		}

		fprintf(stderr, "Failed to get HW surface format.\n");
		return AV_PIX_FMT_NONE;
	}

	static int decode_write(AVCodecContext* avctx, AVPacket* packet)
	{
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

			if (frame->format == hw_pix_fmt) {
				/* retrieve data from GPU to CPU */
				if ((ret = av_hwframe_transfer_data(sw_frame, frame, 0)) < 0) {
					fprintf(stderr, "Error transferring the data to system memory\n");
					goto fail;
				}
				tmp_frame = sw_frame;
			}
			else
				tmp_frame = frame;

			size = av_image_get_buffer_size((AVPixelFormat)tmp_frame->format, tmp_frame->width, tmp_frame->height, 1);
			buffer = (uint8_t *)av_malloc(size);
			if (!buffer) {
				fprintf(stderr, "Can not alloc buffer\n");
				ret = AVERROR(ENOMEM);
				goto fail;
			}
			ret = av_image_copy_to_buffer(buffer, size,
				(const uint8_t* const*)tmp_frame->data,
				(const int*)tmp_frame->linesize, (AVPixelFormat)tmp_frame->format,
				tmp_frame->width, tmp_frame->height, 1);
			if (ret < 0) {
				fprintf(stderr, "Can not copy image to buffer\n");
				goto fail;
			}

			if ((ret = fwrite(buffer, 1, size, output_file)) < 0) {
				fprintf(stderr, "Failed to dump raw data.\n");
				goto fail;
			}

		fail:
			av_frame_free(&frame);
			av_frame_free(&sw_frame);
			av_freep(&buffer);
			if (ret < 0)
				return ret;
		}
	}
}

int TestHWDecode(std::vector<std::string> & arrayOfStrings)
{
	AVFormatContext* input_ctx = NULL;
	int video_stream, ret;
	AVStream* video = NULL;
	AVCodecContext* decoder_ctx = NULL;
	AVCodec* decoder = NULL;
	AVPacket* packet = NULL;
	enum AVHWDeviceType type;
	int i;

	if (arrayOfStrings.size() < 4) {
		fprintf(stderr, "Usage: %s <device type> <input file> <output file>\n", arrayOfStrings[0].c_str());
		return -1;
	}

	type = av_hwdevice_find_type_by_name(arrayOfStrings[1].c_str());
	if (type == AV_HWDEVICE_TYPE_NONE) {
		fprintf(stderr, "Device type %s is not supported.\n", arrayOfStrings[1].c_str());
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

	/* open the input file */
	if (avformat_open_input(&input_ctx, arrayOfStrings[2].c_str(), NULL, NULL) != 0) {
		fprintf(stderr, "Cannot open input file '%s'\n", arrayOfStrings[2].c_str());
		return -1;
	}

	if (avformat_find_stream_info(input_ctx, NULL) < 0) {
		fprintf(stderr, "Cannot find input stream information.\n");
		return -1;
	}

	/* find the video stream information */
	ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
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

	/* open the file to dump raw data */
	output_file = fopen(arrayOfStrings[3].c_str(), "w+b");

	/* actual decoding and dump the raw data */
	while (ret >= 0) {
		if ((ret = av_read_frame(input_ctx, packet)) < 0)
			break;

		if (video_stream == packet->stream_index)
			ret = decode_write(decoder_ctx, packet);

		av_packet_unref(packet);
	}

	/* flush the decoder */
	ret = decode_write(decoder_ctx, NULL);

	if (output_file)
		fclose(output_file);
	av_packet_free(&packet);
	avcodec_free_context(&decoder_ctx);
	avformat_close_input(&input_ctx);
	av_buffer_unref(&hw_device_ctx);

	return 0;
}

/*
std::string convertSecondsToHHMMSS(int value)
{
	std::string result;
	// compute h, m, s
	std::string h = std::to_string(value / 3600);
	std::string m = std::to_string((value % 3600) / 60);
	std::string s = std::to_string(value % 60);
	// add leading zero if needed
	std::string hh = std::string(2 - h.length(), '0') + h;
	std::string mm = std::string(2 - m.length(), '0') + m;
	std::string ss = std::string(2 - s.length(), '0') + s;
	// return mm:ss if hh is 00
	if (hh.compare("00") != 0)
	{
		result = hh + ':' + mm + ":" + ss;
	}
	else
	{
		result = mm + ":" + ss;
	}
	return result;
}
*/
void CFFmpegApp::ExitFunction(int x)
{
	throw x;
}

int CFFmpegApp::ProgressBarFunction(int x, void* progressWnd)
{
	if (progressWnd != nullptr)
	{
		auto dialog = static_cast<wxProgressDialog*>(progressWnd);
		if (false == dialog->Update(x, "Video Progress ..."))
			return 1;
	}

	return 0;
}

void CFFmpegApp::ExecuteFFmpeg()
{
	arrayOfCstrings = new char*[arrayOfStrings.size()];

	for (int i = 0; i < arrayOfStrings.size(); ++i)
		arrayOfCstrings[i] = (char*)arrayOfStrings[i].c_str();


	wxProgressDialog* dialog = nullptr;

	if (showProgressWindow)
	{
		dialog = new wxProgressDialog("FFmpeg Process", "Checking...", 100, nullptr,
			wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
	}

	try
	{
		void (*foo)(int);
		foo = &ExitFunction;

		int (*progress)(int, void*);
		progress = &ProgressBarFunction;

		ret = ExecuteFFMpegProgram(arrayOfStrings.size(), arrayOfCstrings, foo, progress, dialog);
	}
	catch (int e)
	{
		ret = e;
	}
	catch (...)
	{
	}

	if (dialog != nullptr)
		delete dialog;

	Cleanup(ret);
}

int CFFmpegApp::TestFFmpeg(const wxString& commandline)
{
	//ffmpeg -hwaccel_output_format cuda -i F:\music_video\test.mp4 -frames:v 1 -q:v 2 test.png
	/*
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-hwaccel_output_format");
	arrayOfStrings.push_back("cuda");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back("F:\\music_video\\test.mp4");
	arrayOfStrings.push_back("-frames:v");
	arrayOfStrings.push_back("1");
	arrayOfStrings.push_back("-q:v");
	arrayOfStrings.push_back("2");
	arrayOfStrings.push_back("d:\\test.png");
	*/
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("cuda");
	arrayOfStrings.push_back("d:\\video\\20200509_132206.mp4");
	arrayOfStrings.push_back("F:\\music_video\\20200509_132206.yuv");
	TestHWDecode(arrayOfStrings);

	return 0;
}

void CFFmpegApp::Cleanup(int ret)
{
	ffmpeg_cleanup(ret);
	delete[] arrayOfCstrings;
}


int CFFmpegApp::CropAudio(const wxString& inputAudioFile, const wxString& timeVideo, const wxString& extension,
                          const wxString& outputFile)
{
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-stream_loop");
	arrayOfStrings.push_back("-1");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputAudioFile.ToStdString());
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back(extension.ToStdString());
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(timeVideo.ToStdString());
	arrayOfStrings.push_back("-vn");
	arrayOfStrings.push_back("-y");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}

int CFFmpegApp::ExecuteFFmpegApp(const wxString& inputVideoFile, const wxString& inputAudioFile,
                                 const wxString& timeVideo, const wxString& outputFile)
{
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back("00:00:00");
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(timeVideo.ToStdString());
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back("00:00:00");
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(timeVideo.ToStdString());
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputAudioFile.ToStdString());
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-map");
	arrayOfStrings.push_back("0:v:0");
	arrayOfStrings.push_back("-map");
	arrayOfStrings.push_back("1:a:0");
	arrayOfStrings.push_back("-y");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}

int CFFmpegApp::ExecuteFFmpegCutVideo(const wxString& inputVideoFile, const wxString& timeVideoIn, const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back(timeVideoIn.ToStdString());
	arrayOfStrings.push_back("-to");
	arrayOfStrings.push_back(timeVideoOut.ToStdString());
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}


int CFFmpegApp::ExecuteFFmpegExtractVideo(const wxString& inputVideoFile, const wxString& timeVideoIn, const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	if (timeVideoIn != "00:00:00" || timeVideoOut != "00:00:00")
	{
		arrayOfStrings.push_back("-ss");
		arrayOfStrings.push_back(timeVideoIn.ToStdString());
		arrayOfStrings.push_back("-to");
		arrayOfStrings.push_back(timeVideoOut.ToStdString());
	}
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-an");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}


int CFFmpegApp::ExecuteFFmpegExtractAudio(const wxString& inputVideoFile, const wxString& timeVideoIn, const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	if (timeVideoIn != "00:00:00" || timeVideoOut != "00:00:00")
	{
		arrayOfStrings.push_back("-ss");
		arrayOfStrings.push_back(timeVideoIn.ToStdString());
		arrayOfStrings.push_back("-to");
		arrayOfStrings.push_back(timeVideoOut.ToStdString());
	}
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-vn");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}



int CFFmpegApp::ExecuteFFmpegMuxVideoAudio(const wxString& inputVideoFile, const wxString& inputAudioFile, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputAudioFile.ToStdString());
	arrayOfStrings.push_back("-c");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}

/*
int CFFmpegApp::ExecuteFFmpegAddRotateInfo(const wxString& inputVideoFile, const wxString& outputFile, const int& rotate)
{
	//ffmpeg -i 20200509_132328.mp4 -map_metadata 0 -metadata:s:v rotate=-90 -codec copy output.mp4
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-map_metadata");
	arrayOfStrings.push_back("0");
	arrayOfStrings.push_back("-metadata:s:v");
	arrayOfStrings.push_back("rotate=" + std::to_string(rotate));
	arrayOfStrings.push_back("-codec");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();
	return 0;
}
*/