#include "header.h"
#include "VideoControl_soft.h"
#include <wx/dcbuffer.h>
#include <RegardsBitmap.h>
#include "ffplaycore.h"
#include "ffmpegToBitmap.h"
#ifdef __APPLE__
#include <OpenCL/cl.h>
#include <OpenCL/opencl.h>
#include <utility.h>
#else
#include <CL/cl.h>
#include <utility.h>
#endif


#include <ConvertUtility.h>
//#include "LoadingResource.h"
wxDEFINE_EVENT(TIMER_FPS,  wxTimerEvent);
wxDEFINE_EVENT(TIMER_PLAYSTART, wxTimerEvent);

AVFrame * copyFrameBuffer = nullptr;

#ifdef RENDEROPENGL
#ifdef GLUT
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#endif
#include <CL/cl_gl.h>
#else
extern COpenCLEngine * openCLEngine;
extern COpenCLContext * openclContext;
#endif


CVideoControlSoft::CVideoControlSoft(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer)
#ifdef RENDEROPENGL  
: CWindowOpenGLMain("CVideoControl",parent, id)
#else
: CWindowMain("CVideoControl", parent, id)
#endif
{
#ifdef RENDEROPENGL  
	renderBitmapOpenGL = nullptr;
#ifdef GLUT
#ifndef __APPLE__
	int argc = 1;
	char* argv[1] = { wxString((wxTheApp->argv)[0]).char_str() };
	glutInit(&argc, argv);
#endif
#endif

#endif
	widthVideo = 0;
	heightVideo = 0;
	subtilteUpdate = false;
	volumeStart = 64;
	old_width = 0;
	old_height = 0;
	pause = false;
	config = nullptr;
	angle = 0;
	flipV = false;
    newVideo = true;
	flipH = false;
	videoEnd = false;
	exit = false;
	quitWindow = false;
    videoStart = false;
    videoRenderStart = false;
	pictureSubtitle = nullptr;
	video_aspect_ratio = 0.0;
	config = CParamInit::getInstance();
    Connect(wxEVT_PAINT, wxPaintEventHandler(CVideoControlSoft::OnPaint));
    Connect(wxEVT_SIZE, wxSizeEventHandler(CVideoControlSoft::OnSize));
    Connect(wxEVENT_ENDVIDEOTHREAD, wxCommandEventHandler(CVideoControlSoft::EndVideoThread));
    Connect(EVENT_VIDEOSTART, wxCommandEventHandler(CVideoControlSoft::VideoStart));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CVideoControlSoft::OnIdle));
	Connect(EVENT_VIDEOROTATION, wxCommandEventHandler(CVideoControlSoft::VideoRotation));
    fpsTimer = new wxTimer(this, TIMER_FPS);
	playStartTimer = new wxTimer(this, TIMER_PLAYSTART);
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CVideoControlSoft::OnRButtonDown));
    Connect(TIMER_FPS, wxEVT_TIMER, wxTimerEventHandler(CVideoControlSoft::OnShowFPS), nullptr, this);
	Connect(TIMER_PLAYSTART, wxEVT_TIMER, wxTimerEventHandler(CVideoControlSoft::OnPlayStart), nullptr, this);
    Connect(wxEVENT_REFRESH, wxCommandEventHandler(CVideoControlSoft::OnRefresh));
	pause = false;
	videoEnd = true;
	this->windowMain = windowMain;
	this->eventPlayer = eventPlayer;
#ifdef RENDEROPENGL 
	openCLEngine = nullptr;
	openclContext = nullptr;
#endif
	openclEffectYUV = nullptr;
	

	ffmfc = new CFFmfc(this, wxID_ANY);
}

void CVideoControlSoft::OnRefresh(wxCommandEvent& event)
{
    Refresh();
}

void CVideoControlSoft::SetRotation(const int &rotation)
{
    wxCommandEvent event(EVENT_VIDEOROTATION);
    event.SetExtraLong(rotation);
    wxPostEvent(this, event);
}

void CVideoControlSoft::VideoRotation(wxCommandEvent& event)
{
	long rotation = event.GetExtraLong();
	if(rotation < 0)
		rotation = 360 + rotation;
	angle = rotation % 360;
    muVideoEffect.lock();
    videoEffectParameter.rotation = rotation;
    muVideoEffect.unlock();
}

void CVideoControlSoft::UpdateFiltre(CEffectParameter * effectParameter)
{
	CVideoEffectParameter * videoParameter = (CVideoEffectParameter *)effectParameter;
	if(videoParameter->streamAudioUpdate)
	{
		ChangeAudioStream(videoParameter->streamAudioIndex);
		videoParameter->streamAudioUpdate = 0;
	}
	else if(videoParameter->streamVideoUpdate)
	{
		//ChangeAudioStream(videoParameter->streamAudioIndex);
		videoParameter->streamVideoUpdate = 0;
	}
	else if(videoParameter->streamSubtitleUpdate)
	{
		//ChangeAudioStream(videoParameter->streamAudioIndex);
		videoParameter->streamSubtitleUpdate = 0;
	}

	muVideoEffect.lock();
	videoEffectParameter = *videoParameter;
	muVideoEffect.unlock();

	//Refresh();
}

CVideoControlSoft * CVideoControlSoft::CreateWindow(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer)
{
   return new CVideoControlSoft(parent, id, windowMain, eventPlayer); 
}

void CVideoControlSoft::SetVideoPreviewEffect(CEffectParameter * effectParameter)
{
	CVideoEffectParameter * videoParameter = (CVideoEffectParameter *)effectParameter;
	muVideoEffect.lock();
	videoEffectParameter = *videoParameter;
	muVideoEffect.unlock();
}

CEffectParameter * CVideoControlSoft::GetParameter()
{
	CVideoEffectParameter * videoParameter = new CVideoEffectParameter();
	muVideoEffect.lock();
	*videoParameter = videoEffectParameter;
	muVideoEffect.unlock();
	return videoParameter;
}


bool CVideoControlSoft::GetProcessEnd()
{
	return videoEnd;
}

void CVideoControlSoft::OnIdle(wxIdleEvent& evt)
{
    //TRACE();
	if(endProgram && videoRenderStart && !quitWindow)
	{
		fpsTimer->Stop();
		quitWindow = true;
		exit = true;
		if (!videoEnd)
		{
			if(ffmfc->Quit())
			{
				wxCommandEvent localevent;
				EndVideoThread(localevent);
				//videoEnd = true;
			}
		}
	}
    
    
#ifdef __APPLE__
   //if (!videoRenderStart && !stopVideo)
   //     this->FastRefresh(this, true);
#endif
}

void CVideoControlSoft::OnShowFPS(wxTimerEvent& event)
{
    
	msgFrame = wxString::Format("FPS : %d", nbFrame);
	nbFrame = 0;
}

void CVideoControlSoft::OnPlayStart(wxTimerEvent& event)
{
	ffmfc->SetFile(this, CConvertUtility::ConvertToStdString(filename));
}

void CVideoControlSoft::EndVideoThread(wxCommandEvent& event)
{
    videoEnd = true;
    if (eventPlayer != nullptr)
    {
        eventPlayer->OnPositionVideo(0);
        eventPlayer->OnVideoEnd();
    }
	fpsTimer->Stop();
	videoRenderStart = false;
	stopVideo = true;

	if(standByMovie != "")
	{
		PlayMovie(standByMovie, true);
	}
}


CVideoControlSoft::~CVideoControlSoft()
{
	if(playStartTimer->IsRunning())
		playStartTimer->Stop();

	delete playStartTimer;
	delete fpsTimer;
#ifdef RENDEROPENGL   
	if (renderBitmapOpenGL != nullptr)
	{
		renderBitmapOpenGL->SetCurrent(*this);
		delete renderBitmapOpenGL;
	}

	if (openCLEngine != nullptr)
		delete openCLEngine;
	openCLEngine = nullptr;

#endif
	if(openclEffectYUV != nullptr)
		delete openclEffectYUV;
        
    if(ffmpegToBitmap != nullptr)           
    {
         delete ffmpegToBitmap;
        ffmpegToBitmap = nullptr;               
    }
    
	if(pictureSubtitle != nullptr)
		delete pictureSubtitle;

	if (ffmfc)
		delete ffmfc;

}

void CVideoControlSoft::SetSubtitulePicture(CRegardsBitmap * picture)
{
	muSubtitle.lock();

	if(pictureSubtitle != nullptr)
		delete pictureSubtitle;

	pictureSubtitle = picture;

	subtilteUpdate = true;

	muSubtitle.unlock();
}

void CVideoControlSoft::DeleteSubtitulePicture()
{
	muSubtitle.lock();

	if(pictureSubtitle != nullptr)
		delete pictureSubtitle;

	pictureSubtitle = nullptr;

	subtilteUpdate = true;

	muSubtitle.unlock();
}

int CVideoControlSoft::PlayMovie(const wxString &movie, const bool &play)
{
	if (videoEnd)
	{
		stopVideo = false;
		angle = 0;
		flipV = false;
		flipH = false;
        videoStart = false;
        newVideo = true;
		initStart = true;
		videoRenderStart = false;
        filename = movie;
		standByMovie = "";
        pause = false;
		//playStartTimer->Start(100, true);
		ffmfc->SetFile(this, CConvertUtility::ConvertToStdString(filename));
	}
	else if(movie != filename)
	{
		OnStop(movie);
		
	}

	return 0;
}

void CVideoControlSoft::VideoStart(wxCommandEvent& event)
{
	eventPlayer->OnVideoStart();
	ffmfc->Play();
	pause = false;
    videoEnd = false;
    videoStart =true;
    fpsTimer->Start(1000);
}

int CVideoControlSoft::getWidth()
{
#ifndef WIN32
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
     return GetSize().x * scale_factor;
}
int CVideoControlSoft::getHeight()
{
#ifndef WIN32
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
    return GetSize().y * scale_factor;
}

void CVideoControlSoft::UpdateScreenRatio()
{
    this->Refresh();
}

void CVideoControlSoft::OnPaint(wxPaintEvent& event)
{
	// This is a dummy, to avoid an endless succession of paint messages.
	// OnPaint handlers must always create a wxPaintDC.
	wxPaintDC dc(this);

#ifndef WIN32
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif

#ifdef RENDEROPENGL 
    GLTexture * glTexture = nullptr;

	if (renderBitmapOpenGL == nullptr)
	{
		renderBitmapOpenGL = new CRenderVideoOpenGL(this);

		//Now we have a context, retrieve pointers to OGL functions
		renderBitmapOpenGL->Init(this);
	}

	renderBitmapOpenGL->SetCurrent(*this);
    
    if(IsSupportOpenCL())
    {
        if (openCLEngine == nullptr)
        {
            openCLEngine = new COpenCLEngine();
            if (openCLEngine != nullptr)
                openclContext = openCLEngine->GetInstance();
        }
    }
#endif 

    std::clock_t start;
    start = std::clock();    
    

    int width = GetWindowWidth() * scale_factor;
    int height = GetWindowHeight() * scale_factor;
    if(width == 0 || height == 0)
        return;
	
	if (quitWindow)
        return;

	int supportOpenCL = COpenCLEngine::SupportOpenCL();
    if(IsSupportOpenCL())
    {
       // muVideoEffect.lock();
       
       // muVideoEffect.unlock();

    #ifdef RENDEROPENGL 
        if (openclEffectYUV == nullptr)
        {
            openclEffectYUV = new COpenCLEffectVideoYUV(openclContext);
        }
    #else

        if (openclEffectYUV == nullptr)
        {
            openclEffectYUV = new COpenCLEffectVideoYUV(openclContext);
        }
    #endif        
    }

    nbFrame++;

    if (videoRenderStart && initStart)
    {
		//nbFrame = 0;
        if(!fpsTimer->IsRunning())
            fpsTimer->Start(1000);
	}

#ifdef RENDEROPENGL 
	if(videoRenderStart)
	{
        glTexture = RenderToGLTexture();
    }
    
    if(videoRenderStart && glTexture != nullptr)
    {       
        renderBitmapOpenGL->CreateScreenRender(width, height, CRgbaquad(0,0,0,0));

        if(glTexture != nullptr)
        {          
            int inverted = 1;
            int x = (width - glTexture->GetWidth()) / 2;
            int y = (height  - glTexture->GetHeight()) / 2;
            
            if(IsSupportOpenCL())
            {
                if(openclContext->IsSharedContextCompatible())
                    inverted = 0;
            }
   
            if(!supportOpenCL)
                inverted = 1;
                
            if(isffmpegDecode)
                inverted = 1;
            
			muVideoEffect.lock();
			renderBitmapOpenGL->RenderWithEffect(x, y, glTexture, &videoEffectParameter, flipH, flipV, inverted);
			muVideoEffect.unlock();
			/*
			if (IsSupportOpenCL() && videoEffectParameter.enableOpenCL)
				renderBitmapOpenGL->RenderWithoutEffect(x, y, glTexture, flipH, flipV, inverted);
			else
			{
				muVideoEffect.lock();
				renderBitmapOpenGL->RenderWithEffect(x, y, glTexture, &videoEffectParameter, flipH, flipV, inverted);
				muVideoEffect.unlock();
			}
			*/
        }

        muVideoEffect.lock();
        if(videoEffectParameter.showFPS)
        {
            renderBitmapOpenGL->Print(0, 1, CConvertUtility::ConvertToUTF8(msgFrame));
        }
        muVideoEffect.unlock();

        muSubtitle.lock();

        if(subtilteUpdate && pictureSubtitle != nullptr)
        {
            renderBitmapOpenGL->SetSubtitle(pictureSubtitle);

            delete pictureSubtitle;
            pictureSubtitle = nullptr;

            subtilteUpdate = false;
        }
        else if(subtilteUpdate)
        {
            renderBitmapOpenGL->DeleteSubtitle();
            subtilteUpdate = false;
        }

        muSubtitle.unlock();

        if(videoEffectParameter.enableSubtitle)
        {
            renderBitmapOpenGL->ShowSubtitle();
        }    
	}
	else
	{
		renderBitmapOpenGL->CreateScreenRender(width * scale_factor, height * scale_factor, CRgbaquad(0,0,0,0));
	}

	this->SwapBuffers();

    if(deleteTexture && glTexture != nullptr)
        delete glTexture;
#else

	wxPaintDC dc(this);
    
	wxBitmap localmemBitmap(width,height);
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = width;
	rc.height = height;
	wxMemoryDC memDC(localmemBitmap);    
	FillRect(&memDC, rc, wxColor(0, 0, 0));
	if (videoRenderStart)
	{
		CRegardsBitmap * bitmap = RenderToBitmap();
		if(bitmap != nullptr)
        {
            CImageLoadingFormat image;
            image.SetPicture(bitmap);
            wxImage * picture = image.GetwxImage();

            int x = (width - picture->GetWidth()) / 2;
            int y = (height - picture->GetHeight()) / 2;

			if (picture->IsOk())
				memDC.DrawBitmap(picture->Mirror(false), x, y);
            //memDC.DrawBitmap(*picture, x, y);

            delete picture;
        }
	}
    
	memDC.SelectObject(wxNullBitmap);

    if(scale_factor != 1.0)
    {
        wxImage image = localmemBitmap.ConvertToImage();
        wxBitmap resized(image, wxBITMAP_SCREEN_DEPTH, scale_factor);
		if (resized.IsOk())
			dc.DrawBitmap(resized, 0, 0);
    }
    else if(localmemBitmap.IsOk())
        dc.DrawBitmap(localmemBitmap, 0, 0);

#endif

    double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;


    //std::cout<<"Video OnPaint Time : "<< duration <<'\n';
    

   // printf("OnPaint end \n");
}

int CVideoControlSoft::ChangeSubtitleStream(int newStreamSubtitle)
{
	ffmfc->Change_subtitle_stream(newStreamSubtitle);
	SetVideoPosition(videoPosition / 1000);
	return 0;
}


int CVideoControlSoft::ChangeAudioStream(int newStreamAudio)
{
	ffmfc->Change_audio_stream(newStreamAudio);
	SetVideoPosition(videoPosition / 1000);
	return 0;
}

void CVideoControlSoft::OnPlay()
{
	if (videoStart)
	{
		if (pause && !videoEnd)
		{
			ffmfc->Pause();
		}
		else if (videoEnd)
		{
			PlayMovie(filename, true);
		}
	}
	else
	{
		PlayMovie(filename, true);
	}

	pause = false;
}

void CVideoControlSoft::OnStop(wxString photoName)
{
	exit = true;
	stopVideo = true;
	if (videoStart)
	{
		if (!videoEnd)
		{
            ffmfc->Quit();
        }
    }
	standByMovie = photoName;
}

void CVideoControlSoft::OnPause()
{
	if (videoStart)
	{
		if (!pause)
		{
			ffmfc->Pause();
		}
		pause = true;
	}
}

void CVideoControlSoft::SetVideoDuration(int64_t duration)
{
	if (eventPlayer != nullptr)
		eventPlayer->SetVideoDuration(duration);
}

void CVideoControlSoft::SetVideoPosition(int64_t pos)
{
	ffmfc->SetTimePosition(pos * 1000 * 1000);
}

void CVideoControlSoft::SetCurrentclock(wxString message)
{
	this->message = message;
}

void CVideoControlSoft::SetPos(int64_t pos)
{
	videoPosition = pos;
	if (eventPlayer != nullptr)
		eventPlayer->OnPositionVideo(pos);
	//Refresh();
}

void CVideoControlSoft::VolumeUp()
{
	ffmfc->VolumeUp();
}

void CVideoControlSoft::VolumeDown()
{
	ffmfc->VolumeDown();
}

int CVideoControlSoft::GetVolume()
{
	return ffmfc->GetVolume();
}

void CVideoControlSoft::OnRButtonDown(wxMouseEvent& event)
{
	//wxWindow * window = this->FindWindowById(idWindowMain);
	if (windowMain != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, TOOLBAR_UPDATE_ID);
		windowMain->GetEventHandler()->AddPendingEvent(evt);
	}
}




void CVideoControlSoft::SetData(void * data, const float & sample_aspect_ratio, void * dxva2Context)
{
    std::clock_t start = std::clock();
    
    bool isCPU = 1;
    if(IsSupportOpenCL())
       isCPU = IsCPUContext();
   // printf("Set Data Begin \n");
     
	videoRenderStart = true; 

	AVFrame *src_frame = (AVFrame *)data;

    video_aspect_ratio = sample_aspect_ratio;
    
    SetFrameData(src_frame);
    
    if(isffmpegDecode || isCPU)
        CopyFrame(src_frame);

    widthVideo = src_frame->width;
    heightVideo = src_frame->height;  
    double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    //std::cout<<"CVideoControlSoft::SetData : "<< duration <<'\n';
    
    //Refresh();
#if defined(__WXGTK__)
    wxCommandEvent event(wxEVENT_REFRESH);
    wxPostEvent(this, event);  
#else
   this->Refresh();
#endif 
}

void CVideoControlSoft::Resize()
{
	if (!stopVideo)
	{
		updateContext = true;

		if (videoStart)
			ffmfc->VideoDisplaySize(GetWindowWidth(), GetWindowHeight());

		if (pause && isffmpegDecode  && copyFrameBuffer != nullptr)
		{
			SetFrameData(copyFrameBuffer);
		}

		Refresh();
	}
}

void CVideoControlSoft::calculate_display_rect(wxRect *rect, int scr_xleft, int scr_ytop, int scr_width, int scr_height)
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
	rect->y = scr_ytop + y;
	rect->width = FFMAX(width, 1);
	rect->height = FFMAX(height, 1);
}

#ifdef RENDEROPENGL

GLTexture * CVideoControlSoft::RenderToTexture(CRegardsBitmap * bitmap)
{
	GLTexture * glTexture = new GLTexture();
	glTexture->Create(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap());
	return glTexture;
}



GLTexture * CVideoControlSoft::RenderToTexture(COpenCLEffectVideo * openclEffect)
{
    if(openclEffect == nullptr)
        return nullptr;
    
	muVideoEffect.lock();
	int bicubic = videoEffectParameter.BicubicEnable;
	muVideoEffect.unlock();
	GLTexture * glTexture = nullptr;
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

	/*
	if (IsSupportOpenCL() && videoEffectParameter.enableOpenCL)
	{
		muVideoEffect.lock();
		openclEffect->ApplyVideoEffect(&videoEffectParameter);
		muVideoEffect.unlock();
	}
	*/
	bool isOpenGLOpenCL = false;
	if (openclContext->IsSharedContextCompatible())
	{
		if (angle == 90 || angle == 270)
			glTexture = renderBitmapOpenGL->GetDisplayTexture(rect.height, rect.width, openclContext->GetContext());
		else
			glTexture = renderBitmapOpenGL->GetDisplayTexture(rect.width, rect.height, openclContext->GetContext());


		if (glTexture != nullptr)
		{

			try
			{
				cl_int err;
				cl_mem cl_image = renderBitmapOpenGL->GetOpenCLTexturePt();
				if (cl_image != nullptr)
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
			catch (...)
			{

			}
		}
	}
	if (!isOpenGLOpenCL)
	{
		if (angle == 90 || angle == 270)
			glTexture = renderBitmapOpenGL->GetDisplayTexture(rect.height, rect.width);
		else
			glTexture = renderBitmapOpenGL->GetDisplayTexture(rect.width, rect.height);

		if (glTexture != nullptr)
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

GLTexture * CVideoControlSoft::RenderFFmpegToTexture()
{
	//printf("CVideoControlSoft decode by ffmpeg 1 \n");   

	GLTexture * glTexture = ffmpegToBitmap->ConvertFrameToOpenGLTexutreWithInterpolation(angle);

	deleteTexture = true;

	return glTexture;
}
#endif

void CVideoControlSoft::Rotate90()
{
	angle += 90;
	angle = angle % 360;
	//ffmfc_SetVideoParameter(angle, flipV, flipH);
}

void CVideoControlSoft::Rotate270()
{
	angle += 270;
	angle = angle % 360;
	//ffmfc_SetVideoParameter(angle, flipV, flipH);
}
void CVideoControlSoft::FlipVertical()
{
	flipV = !flipV;
	//ffmfc_SetVideoParameter(angle, flipV, flipH);
}

void CVideoControlSoft::FlipHorizontal()
{
	flipH = !flipH;
	//ffmfc_SetVideoParameter(angle, flipV, flipH);
}

bool CVideoControlSoft::IsCPUContext()
{
	if (isCPU == -1)
	{

            OpenCLDevice * device = COpenCLEngine::GetDefaultDevice();
            if (device != nullptr)
                isCPU = (device->deviceType == CL_DEVICE_TYPE_CPU ? 1 : 0);
	}

	//printf("IsCPUContext CPU : %d \n", isCPU);

	return (isCPU == 1 ? true : false);
}

int CVideoControlSoft::IsSupportOpenCL()
{
	int supportOpenCL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenCL = config->GetIsOpenCLSupport();

	return supportOpenCL;
}

void CVideoControlSoft::SetFrameData(AVFrame * src_frame)
{
	int enableopenCL = 0;
    bool isCPU = true;
    if(IsSupportOpenCL())
    {    
		enableopenCL = COpenCLEngine::SupportOpenCL();
        isCPU = IsCPUContext();
    }
    else
        enableopenCL = 0;

	if (!enableopenCL || isCPU || src_frame->format != 0)
	{
		bool deleteData = false;
		isffmpegDecode = true;
		muVideoEffect.lock();
		int bicubic = videoEffectParameter.BicubicEnable;
		muVideoEffect.unlock();

		muBitmap.lock();

		wxRect rect;
		if (angle == 90 || angle == 270)
		{
			calculate_display_rect(&rect, 0, 0, getHeight(), getWidth());
		}
		else
		{
			calculate_display_rect(&rect, 0, 0, getWidth(), getHeight());
		}

		if (ffmpegToBitmap != nullptr)
		{
			if (ffmpegToBitmap->GetVideoWidth() != src_frame->width || ffmpegToBitmap->GetVideoHeight() != src_frame->height)
			{
				deleteData = true;
			}
		}

		if (ffmpegToBitmap != nullptr)
		{
			if (ffmpegToBitmap->GetThumbnailWidth() != rect.width || ffmpegToBitmap->GetThumbnailHeight() != rect.height)
			{
				deleteData = true;
			}
		}

		if (oldBicubic != bicubic)
		{
			if (ffmpegToBitmap != nullptr)
			{
				deleteData = true;
			}
			oldBicubic = bicubic;
		}

		//deleteData = true;

		if (ffmpegToBitmap == nullptr)
		{
			ffmpegToBitmap = new CffmpegToBitmap(true);
			ffmpegToBitmap->InitContext(src_frame, bicubic, rect.width, rect.height);
		}
		else if (deleteData)
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

		if (openclEffectYUV != nullptr)
		{
			//printf("OpenCL openclEffectYUV \n");
			int ysize = 0;
			int usize = 0;
			int vsize = 0;

			ysize = src_frame->linesize[0] * src_frame->height;
			usize = src_frame->linesize[1] * (src_frame->height / 2);
			vsize = src_frame->linesize[2] * (src_frame->height / 2);
			muBitmap.lock();

			openclEffectYUV->SetMemoryData(src_frame->data[0], ysize, src_frame->data[1], usize, src_frame->data[2], vsize, src_frame->width, src_frame->height, src_frame->linesize[0], src_frame->format);

			muBitmap.unlock();
		}
	}
}

void CVideoControlSoft::CopyFrame(AVFrame * frame)
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

	if (oldFrame != nullptr)
		av_frame_free(&oldFrame);
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
#ifdef RENDEROPENGL
GLTexture * CVideoControlSoft::RenderToGLTexture()
{
	// printf("RenderToBitmap  \n"); 
	std::clock_t start;
	start = std::clock();
	double duration;
	GLTexture * glTexture = nullptr;

	if (!isffmpegDecode)
	{

		//  printf("VideoControl Is use_opencl \n");               
		if (openclEffectYUV != nullptr && openclEffectYUV->IsOk())
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
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	//std::cout<<"RenderToBitmap Time Execution: "<< duration <<'\n';

	return glTexture;
}
#else

CRegardsBitmap * CVideoControlSoft::GenerateBitmap(COpenCLEffectVideo * openclEffect)
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

CRegardsBitmap * CVideoControlSoft::RenderToBitmap(COpenCLEffectVideo * openclEffect)
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


CRegardsBitmap * CVideoControlSoft::RenderToBitmap()
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