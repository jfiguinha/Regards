#include "header.h"
#include "ffmpegToBitmap.h"
#include <stdexcept>
#include <cassert>
#include <array>
#include <RotateByShearRGB.h>
extern "C" {
#include <libavutil/opt.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
}

#include <RegardsBitmap.h>

AVPixelFormat pixelFormat = AV_PIX_FMT_RGB24;

 CffmpegToBitmap::CffmpegToBitmap(const bool & rgba)
 {
#ifdef RENDEROPENGL  
    // glTexture = new GLTexture();
#endif
     if(rgba)
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

void CffmpegToBitmap::InitContext(AVFrame * src_frame, const bool & bicubic, const int & thumbnailWidth, const int & thumbnailHeight)
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
    
	av_opt_set_int(scaleContext, "srcw", src_frame->width,0);
    av_opt_set_int(scaleContext, "srch", src_frame->height,0);
    av_opt_set_int(scaleContext, "src_format", src_frame->format,0);
    av_opt_set_int(scaleContext, "dstw", thumbnailWidth,0);
    av_opt_set_int(scaleContext, "dsth", thumbnailHeight,0);
    av_opt_set_int(scaleContext, "dst_format", pixelFormat,0);
    av_opt_set_int(scaleContext, "dst_format", pixelFormat,0);
    if(bicubic)
        av_opt_set_int(scaleContext, "sws_flags", SWS_BICUBIC,0);
    else
        av_opt_set_int(scaleContext, "sws_flags", SWS_FAST_BILINEAR,0);
    
    videoFrameWidth = src_frame->width;
    videoFrameHeight = src_frame->height;
	
	const int* coeff = sws_getCoefficients(SWS_CS_DEFAULT);
    if (sws_setColorspaceDetails(scaleContext, coeff, src_frame->format, coeff, pixelFormat, 0, 1<<16, 1<<16) < 0)
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
}

CffmpegToBitmap::~CffmpegToBitmap()
{

}

int CffmpegToBitmap::GetThumbnailWidth()
{
     return videoFrameOutputWidth;
      
}

int CffmpegToBitmap::GetThumbnailHeight()
{
    
     return videoFrameOutputHeight;
}

int CffmpegToBitmap::GetConvert(CRegardsBitmap * bitmap, AVFrame *src_frame, const int & thumbnailWidth, const int & thumbnailHeight)
{
	if(bitmap != nullptr)
	{
		int numBytes = avpicture_get_size(pixelFormat, thumbnailWidth, thumbnailHeight);
		if (numBytes == bitmap->GetBitmapSize())
		{
			uint8_t * convertedFrameBuffer = bitmap->GetPtBitmap();
			int linesize = thumbnailWidth * 4;

			videoFrameOutputWidth = thumbnailWidth;
			videoFrameOutputHeight = thumbnailHeight;
			sws_scale(scaleContext, src_frame->data, src_frame->linesize, 0, src_frame->height,
				&convertedFrameBuffer, &linesize);

			return 1;
		}
	}
	return 0;
}

CRegardsBitmap * CffmpegToBitmap::GetConvert(AVFrame *src_frame, const int & thumbnailWidth, const int & thumbnailHeight)
{
	CRegardsBitmap * bitmap = new CRegardsBitmap(thumbnailWidth, thumbnailHeight);
	int numBytes = avpicture_get_size(pixelFormat, thumbnailWidth, thumbnailHeight);
	uint8_t * convertedFrameBuffer = bitmap->GetPtBitmap();
	int linesize = thumbnailWidth * 4;

	videoFrameOutputWidth = thumbnailWidth;
	videoFrameOutputHeight = thumbnailHeight;
	sws_scale(scaleContext, src_frame->data, src_frame->linesize, 0, src_frame->height,
		&convertedFrameBuffer, &linesize);
	return bitmap;
}
