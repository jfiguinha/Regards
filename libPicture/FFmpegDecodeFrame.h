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

class CRegardsBitmap;
class CompressVideo;

class CFFmpegDecodeFrame
{
public:

	typedef struct StreamContext {
		AVCodecContext *dec_ctx;
		AVCodecContext *enc_ctx;

		AVFrame *dec_frame;
	} StreamContext;

	CFFmpegDecodeFrame(const wxString & filename, const wxString &acceleratorHardware);
	~CFFmpegDecodeFrame();
	bool IsOk();
	void GetVideoInfos(int & width, int & height, int &rotation);
	int GetFrameBitmapPosition(const long &time, const int &widthThumbnail = 0, const int &heightThumbnail = 0);
	CRegardsBitmap * GetBitmap(const bool &copy = true);

	double GetTotalTime();
	int GetRotation();
private:
	
	void Release();
	int open_input_file(const wxString & filename);
	int hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type);
	static enum AVPixelFormat get_hw_format(AVCodecContext *ctx, const enum AVPixelFormat *pix_fmts);

	void hb_limit_rational(int *x, int *y, int num, int den, int limit);
	void hb_reduce(int *x, int *y, int num, int den);

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
	CRegardsBitmap * image;
};