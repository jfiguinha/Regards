#pragma once
#ifdef RENDEROPENGL
#include "GLTexture.h"
using namespace Regards::OpenGL;
#endif

extern "C" {
#include <libavfilter/avfilter.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class CRegardsBitmap;

class CffmpegToBitmap
{
public:
   CffmpegToBitmap(const bool & rgba = false);
    ~CffmpegToBitmap();
    int GetVideoWidth();
    int GetVideoHeight();
    int GetThumbnailWidth();
    int GetThumbnailHeight();    
    void DeleteData();
    void InitContext(AVFrame * src_frame, const bool & bicubic, const int & thumbnailWidth, const int & thumbnailHeight);
    void Preconvert(AVFrame *src_frame, const int & thumbnailWidth, const int & thumbnailHeight);
	CRegardsBitmap * ConvertFrameToRgba32();
	/*
#ifdef RENDEROPENGL
    GLTexture * ConvertFrameToOpenGLTexutreWithInterpolation(const int &angle);
	
#else
	void ConvertFrameWithInterpolation(wxImage * imageToDisplay, AVFrame *src_frame, const int & thumbnailWidth, const int & thumbnailHeight);
	CRegardsBitmap * ConvertFrameToBitmapWithInterpolation(const int &angle);
#endif
    */
private:
 
    //void rotate90(uint8_t * buffer, const unsigned int width, const unsigned int height);
    void createAVFrame(AVFrame** pAvFrame, uint8_t** pFrameBuffer, int width, int height);
    AVFrame* convertedFrame = nullptr;
    uint8_t* convertedFrameBuffer = nullptr;
    SwsContext* scaleContext = nullptr;
    int videoFrameWidth;
    int videoFrameHeight;
    int videoFrameOutputWidth;
    int videoFrameOutputHeight;    
    int width;
    int height;
	CRegardsBitmap * bitmap;
	mutex mubmp;
#ifdef RENDEROPENGL
   // GLTexture * glTexture = nullptr;
#endif
};