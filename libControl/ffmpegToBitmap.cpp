#include "header.h"
#include "ffmpegToBitmap.h"
#include <stdexcept>
#include <cassert>

extern "C" {
#include "libavutil/imgutils.h"
#include <libavutil/opt.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
#include <libavfilter/avfilter.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

AVPixelFormat pixelFormat = AV_PIX_FMT_RGB24;

CffmpegToBitmap::CffmpegToBitmap(const bool& rgba): videoFrameWidth(0), videoFrameHeight(0), videoFrameOutputWidth(0),
                                                    videoFrameOutputHeight(0)
{
	if (rgba)
		pixelFormat = AV_PIX_FMT_BGRA;
	else
		pixelFormat = AV_PIX_FMT_RGB24;
}

int CffmpegToBitmap::GetVideoWidth()
{
	return videoFrameWidth;
}

int CffmpegToBitmap::GetVideoHeight()
{
	return videoFrameHeight;
}

void CffmpegToBitmap::InitContext(AVFrame* src_frame, const bool& bicubic, const int& thumbnailWidth,
                                  const int& thumbnailHeight)
{
	DeleteData();
	// Enable this when it hits the released ffmpeg version
	scaleContext = sws_alloc_context();
	if (scaleContext == nullptr)
	{
		throw std::logic_error("Failed to allocate scale context");
	}

	videoFrameOutputWidth = thumbnailWidth;
	videoFrameOutputHeight = thumbnailHeight;

	av_opt_set_int(scaleContext, "srcw", src_frame->width, 0);
	av_opt_set_int(scaleContext, "srch", src_frame->height, 0);
	av_opt_set_int(scaleContext, "src_format", src_frame->format, 0);
	av_opt_set_int(scaleContext, "dstw", thumbnailWidth, 0);
	av_opt_set_int(scaleContext, "dsth", thumbnailHeight, 0);
	av_opt_set_int(scaleContext, "dst_format", pixelFormat, 0);
	if (bicubic)
		av_opt_set_int(scaleContext, "sws_flags", SWS_BICUBIC, 0);
	else
		av_opt_set_int(scaleContext, "sws_flags", SWS_FAST_BILINEAR, 0);

	videoFrameWidth = src_frame->width;
	videoFrameHeight = src_frame->height;

	const int* coeff = sws_getCoefficients(SWS_CS_DEFAULT);
	if (sws_setColorspaceDetails(scaleContext, coeff, src_frame->format, coeff, pixelFormat, 0, 1 << 16, 1 << 16) < 0)
	{
		sws_freeContext(scaleContext);
		throw std::logic_error("Failed to set colorspace details");
	}

	if (sws_init_context(scaleContext, nullptr, nullptr) < 0)
	{
		sws_freeContext(scaleContext);
		throw std::logic_error("Failed to initialise scale context");
	}

	if (nullptr == scaleContext)
	{
		throw logic_error("Failed to create resize context");
	}
}

void CffmpegToBitmap::DeleteData()
{
	if (nullptr != scaleContext)
		sws_freeContext(scaleContext);
    scaleContext = nullptr;
}

CffmpegToBitmap::~CffmpegToBitmap()
{
	if (nullptr != scaleContext)
		sws_freeContext(scaleContext);
}

int CffmpegToBitmap::GetThumbnailWidth()
{
	return videoFrameOutputWidth;
}

int CffmpegToBitmap::GetThumbnailHeight()
{
	return videoFrameOutputHeight;
}

int CffmpegToBitmap::GetConvert(cv::Mat& bitmap, AVFrame* src_frame, const int& thumbnailWidth,
                                const int& thumbnailHeight)
{
	if (!bitmap.empty())
	{
		int numBytes = av_image_get_buffer_size(pixelFormat, thumbnailWidth, thumbnailHeight, 1);
		uint8_t* convertedFrameBuffer = bitmap.data;
		int linesize = thumbnailWidth * 4;

		videoFrameOutputWidth = thumbnailWidth;
		videoFrameOutputHeight = thumbnailHeight;
		sws_scale(scaleContext, src_frame->data, src_frame->linesize, 0, src_frame->height,
		          &convertedFrameBuffer, &linesize);

		return 1;
	}
	return 0;
}

cv::Mat CffmpegToBitmap::GetConvert(AVFrame* src_frame, const int& thumbnailWidth, const int& thumbnailHeight)
{
	cv::Mat bitmap;
	if (thumbnailWidth != 0 && thumbnailHeight != 0)
		bitmap = cv::Mat(thumbnailHeight, thumbnailWidth, CV_8UC4);
	else
		bitmap = cv::Mat(src_frame->height, src_frame->width, CV_8UC4);
	//int numBytes = av_image_get_buffer_size(pixelFormat, thumbnailWidth, thumbnailHeight, 1);
	uint8_t* convertedFrameBuffer = bitmap.data;
	int linesize = thumbnailWidth * 4;

	videoFrameOutputWidth = thumbnailWidth;
	videoFrameOutputHeight = thumbnailHeight;
	sws_scale(scaleContext, src_frame->data, src_frame->linesize, 0, src_frame->height,
	          &convertedFrameBuffer, &linesize);
	return bitmap;
}

cv::Mat CffmpegToBitmap::GetConvert(AVFrame* src_frame)
{
	cv::Mat bitmap;
	bitmap = cv::Mat(src_frame->height, src_frame->width, CV_8UC4);
	if (scaleContext == nullptr)
	{
		InitContext(src_frame, true, src_frame->width, src_frame->height);
	}

	uint8_t* convertedFrameBuffer = bitmap.data;
	int linesize = src_frame->width * 4;

	sws_scale(scaleContext, src_frame->data, src_frame->linesize, 0, src_frame->height,
	          &convertedFrameBuffer, &linesize);
	return bitmap;
}
