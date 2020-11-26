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

    createAVFrame(&convertedFrame, &convertedFrameBuffer, thumbnailWidth, thumbnailHeight);
}

void CffmpegToBitmap::DeleteData()
{
    if (nullptr == scaleContext)
        sws_freeContext(scaleContext);
    

    if (convertedFrame)
    {
        av_frame_free(&convertedFrame);
        convertedFrame = nullptr;
    }    
    
    if (convertedFrameBuffer)
    {
        av_free(convertedFrameBuffer);
        convertedFrameBuffer = nullptr;
    }
}

CffmpegToBitmap::~CffmpegToBitmap()
{
#ifdef RENDEROPENGL  
  //  if(glTexture != nullptr)
   //     delete glTexture;    
#endif
}

int CffmpegToBitmap::GetThumbnailWidth()
{
     return videoFrameOutputWidth;
      
}

int CffmpegToBitmap::GetThumbnailHeight()
{
    
     return videoFrameOutputHeight;
}

void CffmpegToBitmap::createAVFrame(AVFrame** pAvFrame, uint8_t** pFrameBuffer, int width, int height)
{
    *pAvFrame = av_frame_alloc();

    int numBytes = avpicture_get_size(pixelFormat, width, height);
    *pFrameBuffer = reinterpret_cast<uint8_t*>(av_malloc(numBytes));
    avpicture_fill((AVPicture*) *pAvFrame, *pFrameBuffer, pixelFormat, width, height);
}




void CffmpegToBitmap::Preconvert(AVFrame *src_frame, const int & thumbnailWidth, const int & thumbnailHeight)
{
    //printf("thumbnail bitmap width %d height %d \n", thumbnailWidth, thumbnailHeight);
    videoFrameOutputWidth = thumbnailWidth;
    videoFrameOutputHeight = thumbnailHeight;    
    sws_scale(scaleContext, src_frame->data, src_frame->linesize, 0, src_frame->height,
          convertedFrame->data, convertedFrame->linesize);  
          

}
CRegardsBitmap * CffmpegToBitmap::ConvertFrameToRgba32()
{
	CRegardsBitmap * bitmap = nullptr;
	if (convertedFrame != nullptr)
	{
		uint8_t * data = convertedFrame->data[0];
		bitmap = new CRegardsBitmap();
		bitmap->SetBitmap(data, videoFrameOutputWidth, videoFrameOutputHeight);
        bitmap->VertFlipBuf();
	}
	return bitmap;
}

/*
#ifdef RENDEROPENGL  


GLTexture * CffmpegToBitmap::ConvertFrameToOpenGLTexutreWithInterpolation(const int &angle)
{
    int localAngle = angle % 360;
    if(glTexture != nullptr && glTexture->GetTextureID() == 0)
    {
        glTexture->Create(videoFrameOutputWidth, videoFrameOutputHeight, 0);
    }

    if(convertedFrame != nullptr)
    {
        //int lineSize = convertedFrame->linesize[0];
        uint8_t * data = convertedFrame->data[0];
        
        if(localAngle != 0)
        {
            printf("Rotate Picture : %d \n",localAngle );
            CRgbaquad * m_OriginalBitmapBits = (CRgbaquad *)data;
            wxSize m_size = wxSize(videoFrameOutputWidth, videoFrameOutputHeight);
            wxSize m_sizedst;
            CRgbaquad clrBack;
            CRotateByShearRGB Rot(CRotateByShearRGB::Progress);
            CRgbaquad * m_ScaledBitmapBits = Rot.AllocAndRotate (
                                                m_OriginalBitmapBits,
                                                m_size,
                                                angle,
                                                &m_sizedst,
                                                clrBack
                                                );
            glTexture->SetData((uint8_t *)m_ScaledBitmapBits, m_sizedst.GetWidth(), m_sizedst.GetHeight());
            delete[] m_ScaledBitmapBits;
        }
        else
            glTexture->SetData(data, videoFrameOutputWidth, videoFrameOutputHeight);      
    }

    
    return glTexture;
}
#else

void CffmpegToBitmap::ConvertFrameWithInterpolation(wxImage * imageToDisplay, AVFrame *src_frame, const int & thumbnailWidth, const int & thumbnailHeight)
{
     
    if(imageToDisplay != nullptr && src_frame != nullptr)
    {
        printf("thumbnail bitmap width %d height %d \n", thumbnailWidth, thumbnailHeight);
        
        sws_scale(scaleContext, src_frame->data, src_frame->linesize, 0, src_frame->height,
              convertedFrame->data, convertedFrame->linesize);

        int width = thumbnailWidth;
        int height = thumbnailHeight;
        int lineSize = convertedFrame->linesize[0];

        uint8_t *tmp_ptr = imageToDisplay->GetData();

        for(int y=0; y<height; y++) 
        {
            memcpy(tmp_ptr+y*width*3, 
                convertedFrame->data[0]+y*convertedFrame->linesize[0], 
                width*3);
        }        
    }
}

CRegardsBitmap * CffmpegToBitmap::ConvertFrameToBitmapWithInterpolation(const int &angle)
{
	CRegardsBitmap * picture = nullptr;
	int localAngle = angle % 360;

	if (convertedFrame != nullptr)
	{
		//int lineSize = convertedFrame->linesize[0];
		uint8_t * data = convertedFrame->data[0];

		if (localAngle != 0)
		{
			printf("Rotate Picture : %d \n", localAngle);
			CRgbaquad * m_OriginalBitmapBits = (CRgbaquad *)data;
			wxSize m_size = wxSize(videoFrameOutputWidth, videoFrameOutputHeight);
			wxSize m_sizedst;
			CRgbaquad clrBack;
			CRotateByShearRGB Rot(CRotateByShearRGB::Progress);
			CRgbaquad * m_ScaledBitmapBits = Rot.AllocAndRotate(
				m_OriginalBitmapBits,
				m_size,
				angle,
				&m_sizedst,
				clrBack
			);
			picture = new CRegardsBitmap();
			picture->SetBitmap((uint8_t *)m_ScaledBitmapBits, m_sizedst.GetWidth(), m_sizedst.GetHeight());
			delete[] m_ScaledBitmapBits;
		}
		else
		{
			picture = new CRegardsBitmap();
			picture->SetBitmap((uint8_t *)data, videoFrameOutputWidth, videoFrameOutputHeight);
		}
	}


	return picture;
}
*/
//#endif
