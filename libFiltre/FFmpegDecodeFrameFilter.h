#pragma once

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
	#include <libavutil/display.h>
}
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <OpenCLContext.h>
#include <OpenCLEffectVideoYUV.h>
#include <OpenCLEngine.h>
#include <EffectVideoParameter.h>
#include <OpenCLParameter.h>
#include <RegardsBitmap.h>
using namespace Regards::OpenCL;
class CRegardsBitmap;
class CompressVideo;

class CFFmpegDecodeFrameFilter
{
public:

	typedef struct StreamContext {
		AVCodecContext *dec_ctx;
		AVCodecContext *enc_ctx;

		AVFrame *dec_frame;
	} StreamContext;

	CFFmpegDecodeFrameFilter(COpenCLEngine * openCLEngine, const wxString & filename, const wxString &acceleratorHardware);
	~CFFmpegDecodeFrameFilter();
	bool IsOk();
	void GetVideoInfos(int & width, int & height, int &rotation);
	int GetFrameBitmapPosition(CVideoEffectParameter * videoEffectParameter, const long &time);
	CRegardsBitmap * GetBitmap();

	double GetTotalTime();
	int GetRotation();
private:
	
	int GenerateFrameFromDecoder(CVideoEffectParameter * videoEffectParameter, bool & first, StreamContext *stream, const bool & decodeOpenCL);
	void ApplyFilter(CVideoEffectParameter * videoEffectParameter, AVFrame * sw_frame);
	void Release();
	int open_input_file(const wxString & filename);
	int hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type);
	static enum AVPixelFormat get_hw_format(AVCodecContext *ctx, const enum AVPixelFormat *pix_fmts);

	void hb_limit_rational(int *x, int *y, int num, int den, int limit);
	void hb_reduce(int *x, int *y, int num, int den);
	void GenerateBitmapFromFrame(AVFrame * sw_frame);

	AVFormatContext *ifmt_ctx = nullptr;
	StreamContext *stream_ctx;
	AVPacket packet;
	bool cleanPacket = false;
	std::thread * bitmapShow = nullptr;
	double pourcentage = 0;
	bool processEnd = true;
	int nbframePerSecond = 0;
	AVBufferRef *hw_device_ctx = NULL;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
	wxString acceleratorHardware = "dxva2";
	bool first;
	AVFrame * dst = nullptr;
	SwsContext* scaleContext = nullptr;
	bool m_allowSeek = true;
	int videoStreamIndex = 0;


	mutex muFrame;
	mutex muWriteData;
	mutex muEnding;
	AVFrame * copyFrameBuffer = nullptr;

	double duration_movie = 0.0;
	char timebase[255];
	char duration[255];
	double pos = 0;
	int nbframe = 0;
	bool isend = false;
	int widthVideo;
	int heightVideo;
	int rotation;
	int64_t startTime = 0;
	bool isOk = true;
	COpenCLEngine * openCLEngine = nullptr;
	COpenCLContext * openclContext = nullptr;
	COpenCLEffectVideoYUV * openclEffectYUV = nullptr;
	bool hardwareDecode = false;
	SwsContext* localContext = nullptr;
	CRegardsBitmap * bitmap = nullptr;

};