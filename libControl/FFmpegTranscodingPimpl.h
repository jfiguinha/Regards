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
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <OpenCLContext.h>
#include <OpenCLEffectVideoYUV.h>
#include <OpenCLEngine.h>
#include <EffectVideoParameter.h>
#include <OpenCLParameter.h>
using namespace Regards::OpenCL;

class CRegardsBitmap;
class CompressVideo;
class CVideoEffectParameter;

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
		dst_hardware = av_frame_alloc();
		scaleContext = sws_alloc_context();
		packet.data = NULL;
		packet.size = 0;
		this->acceleratorHardware = acceleratorHardware;

		int supportOpenCL = 0;
		CRegardsConfigParam* config = CParamInit::getInstance();
		if (config != nullptr)
			supportOpenCL = config->GetIsOpenCLSupport();

		if (supportOpenCL)
		{
			if (openCLEngine == nullptr)
			{
				openCLEngine = new COpenCLEngine(true);
				if (openCLEngine != nullptr)
					openclContext = openCLEngine->GetInstance();

				if(openclContext != nullptr)
					openclEffectYUV = new COpenCLEffectVideoYUV(openclContext);

				memOutput = new COpenCLParameterClMem();
			}

		}

		videoEffectParameter = new CVideoEffectParameter();
		videoEffectParameter->effectEnable = true;
		videoEffectParameter->grayEnable = true;

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

		if(dst_hardware != nullptr)
			av_frame_free(&dst);

		if (scaleContext != nullptr)
			sws_freeContext(scaleContext);

		if (openCLEngine != nullptr)
			delete openCLEngine;

		if (videoEffectParameter != nullptr)
			delete videoEffectParameter;

		if (openclEffectYUV != nullptr)
			delete openclEffectYUV;

		if(memOutput != nullptr)
			delete memOutput;
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
	int encode_write_frame(AVFrame *filt_frame, unsigned int stream_index, const int &isvideo);
	void CopyFrame(AVFrame * frame);
	int open_input_file(const wxString & filename);
	int open_output_file(const wxString & filename);
	int init_filter(FilteringContext* fctx, AVCodecContext *dec_ctx, AVCodecContext *enc_ctx, const char *filter_spec);
	int init_filters(void);
	int filter_encode_write_frame(AVFrame *frame, unsigned int stream_index, CompressVideo * m_dlgProgress, const int &isvideo);
	int flush_encoder(unsigned int stream_index);
	void Release();
	void SetFrameData(AVFrame * src_frame, CompressVideo * m_dlgProgress);
	int GenerateFrameFromDecoder(bool & first, AVFrame * & tmp_frame, StreamContext *stream);
	int hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type);
	static enum AVPixelFormat get_hw_format(AVCodecContext *ctx, const enum AVPixelFormat *pix_fmts);
	void ApplyFilter(AVFrame * sw_frame);
	void RgbToYuv(uint8_t *rgb, int width, int height, AVFrame * & src_frame);

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
	bool isend = true;
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
	AVFrame * dst_hardware = nullptr;
	SwsContext* scaleContext = nullptr;
	SwsContext * filterContext = nullptr;
	SwsContext * localContext = nullptr;
	bool m_allowSeek = true;
	int videoStreamIndex = 0;
	int64_t startTime = 0;
	CVideoEffectParameter * videoEffectParameter = nullptr;
	COpenCLEngine * openCLEngine = nullptr;
	COpenCLContext * openclContext = nullptr;
	COpenCLEffectVideoYUV * openclEffectYUV = nullptr;
	COpenCLParameterClMem * memOutput = nullptr;
	CRegardsBitmap * bitmap = nullptr;
	CRegardsBitmap * bitmapCopy = nullptr;
	bool hardwareVideoDecoding = false;
};