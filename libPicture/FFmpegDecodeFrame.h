#pragma once

extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <libavutil/timestamp.h>
}

#include <wx/mstream.h>
#include <VideoStabilization.h>

using namespace Regards::OpenCV;

class CRegardsBitmap;
class CompressVideo;

class CFFmpegDecodeFrame
{
public:
	struct buffer_data
	{
		uint8_t* ptr;
		uint8_t* data;
		size_t size; ///< size left in the buffer
	};

	using StreamContext = struct StreamContext
	{
		AVCodecContext* dec_ctx;
		AVCodecContext* enc_ctx;

		AVFrame* dec_frame;
	};

	void OpenFile(wxMemoryOutputStream* dataOutput, const wxString& filename);
	CFFmpegDecodeFrame();
	~CFFmpegDecodeFrame();
	void EndTreatment();
	int GetExifRotation();
	void OpenFile(const wxString& filename);
	bool IsOk();
	void GetVideoInfos(int& width, int& height, int& rotation);
	int GetFrameBitmapPosition(const long& time, const int& widthThumbnail = 0, const int& heightThumbnail = 0);
	CRegardsBitmap* GetBitmap(const bool& copy = true);

	int CalculVideoSecondStabilization(COpenCVStabilization* opencvStabilization, const int& nbFrame,
	                                   const bool& isBuffer);

	double GetTotalTime();
	int GetRotation();
private:
	void Release();
	int open_input_file(const wxString& filename);
	void hb_limit_rational(int* x, int* y, int num, int den, int limit);
	void hb_reduce(int* x, int* y, int num, int den);
	void FrameToBitmap(AVFrame* sw_frame, const int& widthThumbnail, const int& heightThumbnail);

	int videoFrameOutputWidth = 0;
	int videoFrameOutputHeight = 0;
	AVFormatContext* ifmt_ctx = nullptr;
	StreamContext* stream_ctx;
	AVPacket packet;
	bool cleanPacket = false;
	std::thread* bitmapShow = nullptr;
	//double pourcentage = 0;
	//bool processEnd = true;
	//int nbframePerSecond = 0;
	static int read_packet(void* opaque, uint8_t* buf, int buf_size);
	//AVBufferRef* hw_device_ctx = nullptr;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;

	bool first;
	AVFrame* dst = nullptr;
	SwsContext* scaleContext = nullptr;
	bool m_allowSeek = true;
	AVCodecContext* codec_ctx;
	int videoStreamIndex = 0;

	mutex muFrame;
	mutex muWriteData;
	mutex muEnding;
	AVFrame* copyFrameBuffer = nullptr;

	double duration_movie = 0.0;

	int widthVideo;
	int heightVideo;
	int rotation;
	int64_t startTime = 0;
	bool isOk = true;
	CRegardsBitmap* image;


	struct buffer_data bd = {nullptr};
	bool isBuffer = false;

	AVIOContext* avio_ctx = nullptr;
	uint8_t* avio_ctx_buffer = nullptr;
	size_t avio_ctx_buffer_size = 4096;
};
