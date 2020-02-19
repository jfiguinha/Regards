#include "header.h"
#include "VideoControlInterface.h"
#include "ffmpegToBitmap.h"
#include <RegardsBitmap.h>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#include <OpenCL/opencl.h>
#include <utility.h>
#else
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <utility.h>
#endif

void CVideoControlInterface::calculate_display_rect(wxRect *rect, int scr_xleft, int scr_ytop, int scr_width, int scr_height)
{
    
 
    
	float aspect_ratio = video_aspect_ratio;
	int width, height, x, y;
	

	if (aspect_ratio <= 0.0)
		aspect_ratio = 1.0;
	aspect_ratio *= (float)widthVideo / (float)heightVideo;

	/* XXX: we suppose the screen has a 1.0 pixel ratio */
	height = scr_height;
	width = ((int)rint(height * aspect_ratio)) & ~1;
	if (width > scr_width) {
		width = scr_width;
		height = ((int)rint(width / aspect_ratio)) & ~1;
	}
	x = (scr_width - width) / 2;
	y = (scr_height - height) / 2;
	rect->x = scr_xleft + x;
	rect->y = scr_ytop  + y;
	rect->width = FFMAX(width,  1);
	rect->height = FFMAX(height, 1);
}

#ifdef RENDEROPENGL

GLTexture * CVideoControlInterface::RenderToTexture(CRegardsBitmap * bitmap)
{
    GLTexture * glTexture = new GLTexture();
    glTexture->Create(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap());
    return glTexture;
}



GLTexture * CVideoControlInterface::RenderToTexture(COpenCLEffectVideo * openclEffect)
{
    muVideoEffect.lock();
    int bicubic = videoEffectParameter.BicubicEnable;
    muVideoEffect.unlock();
    GLTexture * glTexture = nullptr;
    wxRect rect;

    if(angle == 90 || angle == 270)
    {
        calculate_display_rect(&rect, 0, 0, GetHeight(), GetWidth());
        openclEffect->InterpolationBicubic(rect.height, rect.width, angle, bicubic);
    }
    else
    {
        calculate_display_rect(&rect, 0, 0, GetWidth(), GetHeight());
        openclEffect->InterpolationBicubic(rect.width, rect.height, angle, bicubic);
    }            
    
    bool isOpenGLOpenCL = false;
     if(openclContext->IsSharedContextCompatible())
     { 
        if(angle == 90 || angle == 270)
            glTexture = renderBitmapOpenGL->GetDisplayTexture(rect.height, rect.width, openclContext->GetContext());
        else
            glTexture = renderBitmapOpenGL->GetDisplayTexture(rect.width, rect.height, openclContext->GetContext());

        
        if(glTexture != nullptr)
        {
            
            try
            {
                cl_int err ;
                cl_mem cl_image = renderBitmapOpenGL->GetOpenCLTexturePt();
                if(cl_image != nullptr)
                {
                    err = clEnqueueAcquireGLObjects(openclContext->GetCommandQueue(), 1, &cl_image, 0, 0, 0);
                    Error::CheckError(err);
                    openclEffect->GetRgbaBitmap(cl_image);
                    err = clEnqueueReleaseGLObjects(openclContext->GetCommandQueue(), 1, &cl_image, 0, 0, 0);
                    Error::CheckError(err);
                    err = clFlush(openclContext->GetCommandQueue());
                    Error::CheckError(err);
                    isOpenGLOpenCL = true;
                }

            }
            catch(...)
            {

            }
        }
    }
    if(!isOpenGLOpenCL)
    {
        if(angle == 90 || angle == 270)
            glTexture = renderBitmapOpenGL->GetDisplayTexture(rect.height, rect.width);
        else
            glTexture = renderBitmapOpenGL->GetDisplayTexture(rect.width, rect.height);                  
        
        if(glTexture != nullptr)
        {
            CRegardsBitmap * bitmap = openclEffect->GetRgbaBitmap();
            glTexture->SetData(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
            delete bitmap;
        }
        else
            printf("CVideoControl glTexture Error \n");                
    }        

    return glTexture;
}

GLTexture * CVideoControlInterface::RenderFFmpegToTexture()
{
    //printf("CVideoControlSoft decode by ffmpeg 1 \n");   

    GLTexture * glTexture = ffmpegToBitmap->ConvertFrameToOpenGLTexutreWithInterpolation(angle);  

    deleteTexture = true;

    return glTexture;
}
#endif

void CVideoControlInterface::Rotate90()
{
	angle += 90;
	angle = angle % 360;
	//ffmfc_SetVideoParameter(angle, flipV, flipH);
}

void CVideoControlInterface::Rotate270()
{
	angle += 270;
	angle = angle % 360;
	//ffmfc_SetVideoParameter(angle, flipV, flipH);
}
void CVideoControlInterface::FlipVertical()
{
	flipV = !flipV;
	//ffmfc_SetVideoParameter(angle, flipV, flipH);
}

void CVideoControlInterface::FlipHorizontal()
{
	flipH = !flipH;
	//ffmfc_SetVideoParameter(angle, flipV, flipH);
}

bool CVideoControlInterface::IsCPUContext()
{
    if(isCPU == -1)
    {
        OpenCLDevice * device = COpenCLEngine::GetDefaultDevice();
        if(device != nullptr)
            isCPU = (device->deviceType == CL_DEVICE_TYPE_CPU ? 1 : 0);
    
    }
    
    //printf("IsCPUContext CPU : %d \n", isCPU);
        
    return (isCPU == 1 ? true : false);
}


void CVideoControlInterface::SetFrameData(AVFrame * src_frame)
{
	int enableopenCL = 0;
    muVideoEffect.lock();
    enableopenCL = videoEffectParameter.enableOpenCL;
    muVideoEffect.unlock();    
    
    bool isCPU = IsCPUContext();
         
    if(!enableopenCL || isCPU || src_frame->format != 0)
    {
        bool deleteData = false;
        isffmpegDecode = true;
        muVideoEffect.lock();
        int bicubic = videoEffectParameter.BicubicEnable;
        muVideoEffect.unlock();
          
        muBitmap.lock();
        
        wxRect rect;
        if(angle == 90 || angle == 270)
        {
            calculate_display_rect(&rect, 0, 0, GetHeight(), GetWidth());
        }
        else
        {
            calculate_display_rect(&rect, 0, 0, GetWidth(), GetHeight());            
        }
        
        if(ffmpegToBitmap != nullptr)
        {
            if(ffmpegToBitmap->GetVideoWidth() != src_frame->width || ffmpegToBitmap->GetVideoHeight() != src_frame->height)
            {
                deleteData = true;
            }
        }
        
        if(ffmpegToBitmap != nullptr)
        {
            if(ffmpegToBitmap->GetThumbnailWidth() != rect.width || ffmpegToBitmap->GetThumbnailHeight() != rect.height)
            {
                deleteData = true;
            }         
        }
        
        if(oldBicubic != bicubic)
        {
            if(ffmpegToBitmap != nullptr)
            {
                deleteData = true;                  
            }
             oldBicubic = bicubic;
        }
        
        //deleteData = true;
        
        if(ffmpegToBitmap == nullptr)
        {
            ffmpegToBitmap = new CffmpegToBitmap(true);
            ffmpegToBitmap->InitContext(src_frame, bicubic, rect.width, rect.height);
        }
        else if(deleteData)
        {
            ffmpegToBitmap->DeleteData();
            ffmpegToBitmap->InitContext(src_frame, bicubic, rect.width, rect.height);
        }
           
        ffmpegToBitmap->Preconvert(src_frame, rect.width, rect.height);
        
        muBitmap.unlock(); 
    }
    else
    {    
        isffmpegDecode = false;
        
        if(openclEffectYUV != nullptr)
        {
            //printf("OpenCL openclEffectYUV \n");
            int ysize = 0;
            int usize = 0;
            int vsize = 0;

            ysize = src_frame->linesize[0] * src_frame->height;
            usize = src_frame->linesize[1] * (src_frame->height / 2);
            vsize  =src_frame->linesize[2] * (src_frame->height / 2);
            muBitmap.lock();
            
            openclEffectYUV->SetMemoryData(src_frame->data[0], ysize, src_frame->data[1], usize, src_frame->data[2], vsize, src_frame->width, src_frame->height, src_frame->linesize[0], src_frame->format);

            muBitmap.unlock();
        }         
    }     
}

void CVideoControlInterface::CopyFrame(AVFrame * frame)
{
    AVFrame * oldFrame = nullptr;
    AVFrame * copyFrame = av_frame_alloc();
    copyFrame->format = frame->format;
    copyFrame->width = frame->width;
    copyFrame->height = frame->height;
    copyFrame->channels = frame->channels;
    copyFrame->channel_layout = frame->channel_layout;
    copyFrame->nb_samples = frame->nb_samples;        
    
    av_frame_get_buffer(copyFrame, 32);
    av_frame_copy(copyFrame, frame);
    av_frame_copy_props(copyFrame, frame);         
    
    oldFrame = copyFrameBuffer;
    copyFrameBuffer = copyFrame;   

    if(oldFrame != nullptr)
        av_frame_free(&oldFrame); 
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
#ifdef RENDEROPENGL
GLTexture * CVideoControlInterface::RenderToGLTexture()
{
    // printf("RenderToBitmap  \n"); 
    std::clock_t start;
    start = std::clock();  
    double duration;
    GLTexture * glTexture = nullptr;

    if(!isffmpegDecode)
    {
        
      //  printf("VideoControl Is use_opencl \n");               
        if(openclEffectYUV != nullptr && openclEffectYUV->IsOk())
        {
            muBitmap.lock();
            glTexture = RenderToTexture(openclEffectYUV);
            muBitmap.unlock();            
        }

        deleteTexture = false;
    }
    else
    {
        muBitmap.lock();
        glTexture = RenderFFmpegToTexture();
        muBitmap.unlock();  
        deleteTexture = false;       
    }
    /*
    if(bitmap != nullptr)
    {
        printf("VideoControl Is use_opencl \n");               
        muBitmap.lock();
        glTexture = RenderToTexture(bitmap);
        muBitmap.unlock();
    }
    else
    {
        muBitmap.lock();
        glTexture = RenderFFmpegToTexture();
        muBitmap.unlock();         
    }
    */
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    //std::cout<<"RenderToBitmap Time Execution: "<< duration <<'\n';
    
    return glTexture;
}
#else

CRegardsBitmap * CVideoControlInterface::GenerateBitmap(COpenCLEffectVideo * openclEffect)
{
	muVideoEffect.lock();
	int bicubic = videoEffectParameter.BicubicEnable;
	muVideoEffect.unlock();
	wxRect rect;

	if (angle == 90 || angle == 270)
	{
		calculate_display_rect(&rect, 0, 0, getHeight(), getWidth());
		openclEffect->InterpolationBicubic(rect.height, rect.width, angle, bicubic);
	}
	else
	{
		calculate_display_rect(&rect, 0, 0, getWidth(), getHeight());
		openclEffect->InterpolationBicubic(rect.width, rect.height, angle, bicubic);
	}
	muVideoEffect.lock();
	openclEffect->ApplyVideoEffect(&videoEffectParameter);
	muVideoEffect.unlock();
	CRegardsBitmap * bitmap = openclEffect->GetRgbaBitmap();
	return bitmap;
}

CRegardsBitmap * CVideoControlInterface::RenderToBitmap(COpenCLEffectVideo * openclEffect)
{
	// printf("RenderToBitmap  \n"); 
	std::clock_t start;
	start = std::clock();
	double duration;
	CRegardsBitmap * picture = nullptr;
	if (!isffmpegDecode)
	{

		//printf("VideoControl Is use_opencl \n");
		if (openclEffect != nullptr && openclEffect->IsOk())
		{
			muBitmap.lock();
			picture = GenerateBitmap(openclEffect);
			muBitmap.unlock();
		}

		deleteTexture = false;
	}
	else
	{
		muBitmap.lock();
		picture = ffmpegToBitmap->ConvertFrameToBitmapWithInterpolation(angle);
		muBitmap.unlock();
		deleteTexture = false;
	}

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	//std::cout<<"RenderToBitmap Time Execution: "<< duration <<'\n';

	return picture;
}


CRegardsBitmap * CVideoControlInterface::RenderToBitmap()
{
	// printf("RenderToBitmap  \n"); 
	std::clock_t start;
	start = std::clock();
	double duration;
	CRegardsBitmap * picture = nullptr;
	if (!isffmpegDecode)
	{

		//printf("VideoControl Is use_opencl \n");
		if (openclEffectYUV != nullptr && openclEffectYUV->IsOk())
		{
			muBitmap.lock();
			picture = GenerateBitmap(openclEffectYUV);
			muBitmap.unlock();
		}

		deleteTexture = false;
	}
	else
	{
		muBitmap.lock();
		picture = ffmpegToBitmap->ConvertFrameToBitmapWithInterpolation(angle);
		muBitmap.unlock();
		deleteTexture = false;
	}

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	//std::cout<<"RenderToBitmap Time Execution: "<< duration <<'\n';

	return picture;
}
#endif