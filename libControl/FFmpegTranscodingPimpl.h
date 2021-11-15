#pragma once
#include "VideoCompressOption.h"
#include <map>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
#include <libavutil/timestamp.h>
}

#include <wx/mstream.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <OpenCLContext.h>
#include <OpenCLEffectVideoYUV.h>
#include <OpenCLEngine.h>
#include <OpenCLParameter.h>
#include <RegardsBitmap.h>
#include <VideoStabilization.h>
#include <FFmpegDecodeFrame.h>
using namespace Regards::OpenCV;
using namespace Regards::OpenCL;
class CRegardsBitmap;
class CompressVideo;
class CFiltreEffet;

class CFFmpegTranscodingPimpl
{
public:
	using FilteringContext = struct FilteringContext
	{
		AVFilterContext* buffersink_ctx;
		AVFilterContext* buffersrc_ctx;
		AVFilterGraph* filter_graph;

		AVFrame* filtered_frame;
	};


	using StreamContext = struct StreamContext
	{
		AVCodecContext* dec_ctx;
		AVCodecContext* enc_ctx;

		AVFrame* dec_frame;
	};

	using buffer_data = struct buffer_data
	{
		uint8_t* buf;
		size_t size;
		uint8_t* ptr;
		size_t room; ///< size left in the buffer
	};


	CFFmpegTranscodingPimpl(const wxString& acceleratorHardware, COpenCLContext* openclContext): stream_ctx(nullptr),
		m_dlgProgress(nullptr),
		videoCompressOption(nullptr), duration{}
	{
		dst = av_frame_alloc();
		scaleContext = sws_alloc_context();
		packet.data = nullptr;
		packet.size = 0;
		this->acceleratorHardware = acceleratorHardware;
		bitmapCopy = new CRegardsBitmap();
		int supportOpenCL = 0;
		CRegardsConfigParam* config = CParamInit::getInstance();
		if (config != nullptr)
			supportOpenCL = config->GetIsOpenCLSupport();
		ffmpegDecodeFrame = new CFFmpegDecodeFrame();
		if (supportOpenCL)
		{
			this->openclContext = openclContext;
			if (openclContext != nullptr)
				openclEffectYUV = new COpenCLEffectVideoYUV(openclContext);
		}
	}
	;

	void EndTreatment()
	{
		if (cleanPacket)
		{
			if (packet.data != nullptr)
				av_packet_unref(&packet);
			Release();
			if (bitmapShow != nullptr)
			{
				bitmapShow->join();
				delete bitmapShow;
			}
			cleanPacket = false;
		}
	}

	~CFFmpegTranscodingPimpl()
	{
		EndTreatment();

		if (copyFrameBuffer != nullptr)
		{
			av_freep(&copyFrameBuffer->data[0]);
			av_frame_free(&copyFrameBuffer);
		}

		copyFrameBuffer = nullptr;

		if (ffmpegDecodeFrame != nullptr)
			delete ffmpegDecodeFrame;
		ffmpegDecodeFrame = nullptr;

		if (dst != nullptr)
		{
			av_freep(&dst->data[0]);
			av_frame_free(&dst);
		}

		if (dst_hardware != nullptr)
		{
			av_freep(&dst_hardware->data[0]);
			av_frame_free(&dst_hardware);
		}


		if (scaleContext != nullptr)
			sws_freeContext(scaleContext);
#ifdef USE_FILTER
		if(filterContext != nullptr)
			sws_freeContext(filterContext);
#endif
		if (localContext != nullptr)
			sws_freeContext(localContext);

		if (openclEffectYUV != nullptr)
			delete openclEffectYUV;

		if (bitmapCopy != nullptr)
			delete bitmapCopy;

		if (bitmapData != nullptr)
			delete bitmapData;
	};

	static void DisplayPreview(void* data);

	void ProcessEnd()
	{
		processEnd = true;
	}

	int EncodeOneFrame(CompressVideo* m_dlgProgress, wxMemoryOutputStream* dataOutput, const wxString& input, const wxString& output,
	                   const long& time, CVideoOptionCompress* videoCompressOption);
	int EncodeFile(const wxString& input, const wxString& output, CompressVideo* m_dlgProgress,
	               CVideoOptionCompress* videoCompressOption);
	int OpenFile(const wxString& input, const wxString& output);

	int IsSupportOpenCL();

private:


	static int write_packet(void* opaque, uint8_t* buf, int buf_size);

	void VideoTreatment(AVFrame* & tmp_frame, StreamContext* stream);
	int ProcessEncodeFile(AVFrame* dst);
	int ProcessEncodeOneFrameFile(AVFrame* dst, const int64_t& timeInSeconds);
	void DecodeHardwareFrame(AVFrame* & tmp_frame, AVFrame* sw_frame, StreamContext* stream);
	wxString GetCodecName(AVCodecID vcodec, const wxString& encoderHardware);
	AVDictionary* setEncoderParam(const AVCodecID& codec_id, AVCodecContext* pCodecCtx, AVCodecContext* pSourceCodecCtx,
	                              const wxString& encoderName);
	bool openHardEncoder(const AVCodecID& codec_id, const wxString& encoderName, AVCodecContext* pSourceCodecCtx,
	                     AVStream* stream);
	bool openSoftEncoder(const AVCodecID& codec_id, AVCodecContext* pSourceCodecCtx, AVStream* stream);
	AVCodecID GetCodecID(AVMediaType codec_type) const;
	wxString GetCodecNameForEncoder(AVCodecID vcodec, const wxString& nameEncoder);
	int encode_write_frame(AVFrame* filt_frame, unsigned int stream_index);
	void CopyFrame(AVFrame* frame);
	int open_input_file(const wxString& filename);
	int open_output_file(const wxString& filename);
#ifdef USE_FILTER
	int init_filter(FilteringContext* fctx, AVCodecContext *dec_ctx, AVCodecContext *enc_ctx, const char *filter_spec);
	int init_filters(void);
#endif
	int filter_encode_write_frame(AVFrame* frame, unsigned int stream_index, CompressVideo* m_dlgProgress,
	                              const int& isvideo);
	int flush_encoder(unsigned int stream_index);
	void Release();
	void SetFrameData(AVFrame* src_frame, CompressVideo* m_dlgProgress);
	void VideoInfos(StreamContext* stream);
	int hw_decoder_init(AVCodecContext* ctx, enum AVHWDeviceType type);
	static enum AVPixelFormat get_hw_format(AVCodecContext* ctx, const enum AVPixelFormat* pix_fmts);
	CRegardsBitmap* GetBitmapRGBA(AVFrame* tmp_frame);

	AVFormatContext* ifmt_ctx = nullptr;
	AVFormatContext* ofmt_ctx = nullptr;
#ifdef USE_FILTER
	FilteringContext *filter_ctx = nullptr;
#endif
	StreamContext* stream_ctx;
	AVPacket packet;
	bool cleanPacket = false;

	COpenCLContext* openclContext = nullptr;
	CRegardsBitmap* bitmapVideo = nullptr;
	std::thread* bitmapShow = nullptr;
	CompressVideo* m_dlgProgress;
	mutex muEnding;
	mutex muFrame;
	mutex muWriteData;
	bool isend = true;
	AVFrame* copyFrameBuffer = nullptr;
	CVideoOptionCompress* videoCompressOption;
	//char timebase[255];
	char duration[255];
	double pos = 0;
	double pourcentage = 0;
	bool processEnd = true;
	int nbframe = 0;
	int nbframePerSecond = 0;
	AVBufferRef* hw_device_ctx = nullptr;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
	wxString acceleratorHardware = "dxva2";
	double duration_movie = 0.0;
	AVFrame* dst = nullptr;
	AVFrame* dst_hardware = nullptr;
	SwsContext* scaleContext = nullptr;
#ifdef USE_FILTER
	SwsContext* filterContext = nullptr;
#endif
	SwsContext* localContext = nullptr;
	//bool m_allowSeek = true;
	int videoStreamIndex = 0;
	int64_t startTime = 0;
	//CRegardsBitmap * bitmap = nullptr;
	CRegardsBitmap* bitmapCopy = nullptr;
	CRegardsBitmap* bitmapData = nullptr;
	COpenCLEffectVideoYUV* openclEffectYUV = nullptr;
	//bool hardwareDecode = false;
	//bool yuvDecodeInit = false;
	bool showpreview = false;

	//int oldPos = 0;
	bool encodeOneFrame = false;
	bool first = true;
	bool deleteFrame = false;
	bool first_frame = true;
	//bool first_yuv = true;

	bool isBuffer = false;

	AVIOContext* avio_ctx = nullptr;
	wxMemoryOutputStream* dataOutput = nullptr;
	uint8_t* avio_ctx_buffer = nullptr;
	size_t avio_ctx_buffer_size = 4096;
	wxString outputFile;
	std::map<int, int> streamCorrespondant;

	COpenCVStabilization* openCVStabilization = nullptr;
	int nbFrame = 0;

	int framerate = 30;

	CFFmpegDecodeFrame* ffmpegDecodeFrame = nullptr;
	std::map<int, int> streamInNumberInOut;
};
