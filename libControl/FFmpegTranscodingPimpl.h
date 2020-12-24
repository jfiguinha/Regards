#pragma once
#include "VideoCompressOption.h"
extern "C"
{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavfilter/buffersink.h>
	#include <libavfilter/buffersrc.h>
	#include <libavutil/opt.h>
	#include <libavutil/pixdesc.h>
	#include <libavutil/imgutils.h>
	#include <libswscale/swscale.h>
	#include <libavutil/timestamp.h>
}

class CRegardsBitmap;
class CompressVideo;

class CFFmpegTranscodingPimpl
{
public:


	typedef struct FilteringContext {
		AVFilterContext *buffersink_ctx;
		AVFilterContext *buffersrc_ctx;
		AVFilterGraph *filter_graph;

		AVFrame *filtered_frame;
	} FilteringContext;


	typedef struct StreamContext {
		AVCodecContext *dec_ctx;
		AVCodecContext *enc_ctx;

		AVFrame *dec_frame;
	} StreamContext;


	CFFmpegTranscodingPimpl(const wxString &acceleratorHardware)
	{
		dst = av_frame_alloc();
		scaleContext = sws_alloc_context();
		packet.data = NULL;
		packet.size = 0;
		this->acceleratorHardware = acceleratorHardware;

	};
	~CFFmpegTranscodingPimpl()
	{
		if (cleanPacket)
		{
            if(packet.data != NULL)
                av_packet_unref(&packet);
			Release();
			if (bitmapShow != nullptr)
			{
				bitmapShow->join();
				delete bitmapShow;
			}

		}

		if (copyFrameBuffer != nullptr)
			av_frame_free(&copyFrameBuffer);
		copyFrameBuffer = nullptr;

		if (dst != nullptr)
			av_frame_free(&dst);

		if (scaleContext != nullptr)
			sws_freeContext(scaleContext);
	};

	static void DisplayPreview(void * data);

	void ProcessEnd()
	{
		processEnd = true;
	}

	int EncodeFile(const wxString & input, const wxString & output, CompressVideo * m_dlgProgress, CVideoOptionCompress * videoCompressOption);
	int OpenFile(const wxString & input, const wxString & output);
	int ProcessEncodeFile(AVFrame * dst, SwsContext * scaleContext);


private:
	wxString GetCodecName(AVCodecID vcodec, const wxString &encoderHardware);
	AVDictionary * setEncoderParam(const AVCodecID &codec_id, AVCodecContext * pCodecCtx, AVCodecContext * pSourceCodecCtx, const wxString &encoderName);
	bool openHardEncoder(const AVCodecID &codec_id, const wxString &encoderName, AVCodecContext * pSourceCodecCtx);
	bool openSoftEncoder(const AVCodecID &codec_id, AVCodecContext * pSourceCodecCtx);
	AVCodecID GetCodecID(AVMediaType codec_type);
	wxString GetCodecNameForEncoder(AVCodecID vcodec, const wxString &nameEncoder);
	int encode_write_frame(AVFrame *filt_frame, unsigned int stream_index);
	void CopyFrame(AVFrame * frame);
	int open_input_file(const wxString & filename);
	int open_output_file(const wxString & filename);
	int init_filter(FilteringContext* fctx, AVCodecContext *dec_ctx, AVCodecContext *enc_ctx, const char *filter_spec);
	int init_filters(void);
	int filter_encode_write_frame(AVFrame *frame, unsigned int stream_index, CompressVideo * m_dlgProgress, const int &isvideo);
	int flush_encoder(unsigned int stream_index);
	void Release();
	void SetFrameData(AVFrame * src_frame, CompressVideo * m_dlgProgress);

	int hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type);
	static enum AVPixelFormat get_hw_format(AVCodecContext *ctx, const enum AVPixelFormat *pix_fmts);

	AVFormatContext *ifmt_ctx = nullptr;
	AVFormatContext *ofmt_ctx = nullptr;
	FilteringContext *filter_ctx = nullptr;
	StreamContext *stream_ctx;
	AVPacket packet;
	bool cleanPacket = false;

	CRegardsBitmap * bitmapVideo = nullptr;
	std::thread * bitmapShow = nullptr;
	CompressVideo * m_dlgProgress;
	mutex muEnding;
	mutex muFrame;
	mutex muWriteData;
	bool isend = false;
	AVFrame * copyFrameBuffer = nullptr;
	CVideoOptionCompress * videoCompressOption;
	char timebase[255];
	char duration[255];
	double pos = 0;
	double pourcentage = 0;
	bool processEnd = true;
	int nbframe = 0;
	int nbframePerSecond = 0;
	AVBufferRef *hw_device_ctx = NULL;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
	wxString acceleratorHardware = "dxva2";
	double duration_movie = 0.0;
	AVFrame * dst = nullptr;
	SwsContext* scaleContext = nullptr;
	bool m_allowSeek = true;
	int videoStreamIndex = 0;
};