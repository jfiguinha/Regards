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

using namespace Regards::OpenCV;
using namespace Regards::OpenCL;
class CRegardsBitmap;
class CompressVideo;
class CFiltreEffet;
class CffmpegToBitmap;
extern COpenCLContext* openclContext;

namespace cv
{
	class VideoCapture;
}

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


	CFFmpegTranscodingPimpl();
	~CFFmpegTranscodingPimpl();

	int EncodeOneFrame(CompressVideo* m_dlgProgress, const wxString& input, const wxString& output, const long& time, CVideoOptionCompress* videoCompressOption);
	int EncodeFile(const wxString& input, const wxString& output, CompressVideo* m_dlgProgress, CVideoOptionCompress* videoCompressOption);
	
private:

	void EncodeOneFrame(AVCodecContext* enc_ctx, AVFrame* frame, FILE* outfile);
	int EncodeOneFrameFFmpeg(const char* filename, AVFrame* dst, const int64_t& timeInSeconds);

	cv::Mat ApplyProcess(CRegardsBitmap* bitmap);
	static void DisplayPreview(void* data);
	void EndTreatment();
	int IsSupportOpenCL();
	int OpenFile(const wxString& input, const wxString& output);
	static int write_packet(void* opaque, uint8_t* buf, int buf_size);
	int encode_write_frame_withoutpos(AVFrame* filt_frame, unsigned int stream_index);
	void VideoTreatment(AVFrame* & tmp_frame, StreamContext* stream);
	int ProcessEncodeFile(AVFrame* dst);
	int ProcessEncodeOneFrameFile(AVFrame* dst, const int64_t& timeInSeconds);
	void DecodeHardwareFrame(AVFrame* & tmp_frame, AVFrame* sw_frame, StreamContext* stream);
	wxString GetCodecName(AVCodecID vcodec, const wxString& encoderHardware);

	AVCodecContext * OpenFFmpegEncoder(AVCodecID codec_id, AVCodecContext* pCodecCtx, AVStream* streamVideo, wxString encoderName);
	void SetParamFromVideoCodec(AVCodecContext* pCodecCtx, AVCodecContext* pSourceCodecCtx);
	AVDictionary* setEncoderParam(const AVCodecID& codec_id, AVCodecContext* pCodecCtx,	const wxString& encoderName);
	AVCodecID GetCodecID(AVMediaType codec_type) const;
	wxString GetCodecNameForEncoder(AVCodecID vcodec, const wxString& nameEncoder);
	int encode_write_frame(AVFrame* filt_frame, unsigned int stream_index);

	int open_input_file(const wxString& filename, const wxString& decodeHardware);
	int open_output_file(const wxString& filename);

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
	StreamContext* stream_ctx;
	AVPacket packet;
	bool cleanPacket = false;
	int rotation = 0;
	std::thread* bitmapShow = nullptr;
	CompressVideo* m_dlgProgress;
	mutex muEnding;
	mutex muFrame;
	mutex muWriteData;
	bool isend = true;
	wxString input_file = "";
	int orientation = 0;
	CVideoOptionCompress* videoCompressOption;

	double duration = 0;
	int totalFrame = 0;
	int nbFrameEncoded = 0;
	int fps = 0;
	int rotate = 0;
	AVBufferRef* hw_device_ctx = nullptr;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;

	CRegardsBitmap* bmp = nullptr;
	int width = 0;
	int height = 0;
	AVFrame* dst = nullptr;
	AVFrame* dst_hardware = nullptr;
	SwsContext* scaleContext = nullptr;
	SwsContext* localContext = nullptr;
	int videoStreamIndex = 0;
	int64_t startTime = 0;
	bool showpreview = false;
	bool encodeOneFrame = false;
	bool first = true;
	bool first_frame = true;

	wxString outputFile;
	std::map<int, int> streamCorrespondant;

	Regards::OpenCV::COpenCVStabilization* openCVStabilization = nullptr;
	int nbFrame = 0;

	int framerate = 30;

	

	cv::VideoCapture * capture = nullptr;
	std::map<int, int> streamInNumberInOut;
};
