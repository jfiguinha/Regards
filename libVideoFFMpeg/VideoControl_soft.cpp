#include "header.h"
#include "VideoControl_soft.h"
#include <wx/dcbuffer.h>
#include <RegardsFloatBitmap.h>
#include <RegardsBitmap.h>
#include "ffplaycore.h"
#include <Interpolation.h>
#include <config_id.h>
#include "ffmpegToBitmap.h"

#ifdef __APPLE__
#include <OpenCL/cl.h>
#include <OpenCL/opencl.h>
#include <utility.h>
#else
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <utility.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <ConvertUtility.h>
//#include "LoadingResource.h"
wxDEFINE_EVENT(TIMER_FPS,  wxTimerEvent);
wxDEFINE_EVENT(EVENT_ENDVIDEOTHREAD, wxCommandEvent);

AVFrame * copyFrameBuffer = nullptr;

CVideoControlSoft::CVideoControlSoft(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer)
: CWindowOpenGLMain("CVideoControl",parent, id)
{
	renderBitmapOpenGL = new Regards::Video::CRenderBitmapInterfaceOpenGL(this);
    printf("CVideoControl initialisation \n");
    
#ifdef WIN32
    renderBitmapOpenGL->Init(this);
#endif  

#ifndef __APPLE__
    int argc = 1;
    char* argv[1] ={wxString((wxTheApp->argv)[0]).char_str()};
    glutInit(&argc, argv);  
#endif

	widthVideo = 0;
	heightVideo = 0;
	subtilteUpdate = false;
	threadVideo = nullptr;
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
    Connect(EVENT_ENDVIDEOTHREAD, wxCommandEventHandler(CVideoControlSoft::EndVideoThread));
    Connect(EVENT_VIDEOSTART, wxCommandEventHandler(CVideoControlSoft::VideoStart));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CVideoControlSoft::OnIdle));
	Connect(EVENT_VIDEOROTATION, wxCommandEventHandler(CVideoControlSoft::VideoRotation));
    Connect(wxEVENT_VIDEOREFRESH, wxCommandEventHandler(CVideoControlSoft::OnRefresh));
    fpsTimer = new wxTimer(this, TIMER_FPS);
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CVideoControlSoft::OnRButtonDown));
    Connect(TIMER_FPS, wxEVT_TIMER, wxTimerEventHandler(CVideoControlSoft::OnShowFPS), nullptr, this);
	pause = false;
	videoEnd = true;
	this->windowMain = windowMain;
	this->eventPlayer = eventPlayer;
	openCLEngine = nullptr;
	openclContext = nullptr;
	openclEffectYUV = nullptr;
	
}

void CVideoControlSoft::SetRotation(const int &rotation)
{
    wxCommandEvent event(EVENT_VIDEOROTATION);
    event.SetExtraLong(rotation);
    wxPostEvent(this, event);
}

void CVideoControlSoft::SetVideoStart()
{
    wxCommandEvent event(EVENT_VIDEOSTART);
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

	//this->FastRefresh(this);
}

CVideoControlInterface * CVideoControlSoft::CreateWindow(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer)
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

void CVideoControlSoft::OnRefresh(wxCommandEvent& event)
{
    this->Refresh();
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
			if(ffmfc_quit())
			{
				wxCommandEvent localevent;
				EndVideoThread(localevent);
				//videoEnd = true;
			}
		}
	}
    
#ifdef __APPLE__
   if (!videoRenderStart && !stopVideo)
        this->FastRefresh(this, true);
#endif
}

void CVideoControlSoft::OnShowFPS(wxTimerEvent& event)
{
    
	msgFrame = wxString::Format("FPS : %d", nbFrame);
	nbFrame = 0;
}

void CVideoControlSoft::PlayVideo(CVideoControlSoft * sdlWindow)
{
    //const char * fichier = CConvertUtility::ConvertFromwxString(sdlWindow->filename);
    ffmfc_play(sdlWindow, CConvertUtility::ConvertToStdString(sdlWindow->filename));
    wxCommandEvent event(EVENT_ENDVIDEOTHREAD);
    wxPostEvent(sdlWindow, event);
}


void CVideoControlSoft::EndVideoThread(wxCommandEvent& event)
{
    
    if(threadVideo != nullptr)
    {
        threadVideo->join();
        delete threadVideo;
        threadVideo = nullptr;
    }

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
		PlayMovie(standByMovie);
	}
}


CVideoControlSoft::~CVideoControlSoft()
{
    
	delete fpsTimer;
    
    if(renderBitmapOpenGL != nullptr)
        delete renderBitmapOpenGL;
    
	if(openclEffectYUV != nullptr)
		delete openclEffectYUV;
        
    if(ffmpegToBitmap != nullptr)           
    {
         delete ffmpegToBitmap;
        ffmpegToBitmap = nullptr;               
    }
    
	if (openCLEngine != nullptr)
		delete openCLEngine;
	openCLEngine = nullptr;

	if(pictureSubtitle != nullptr)
		delete pictureSubtitle;
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

int CVideoControlSoft::PlayMovie(const wxString &movie)
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
        videoEnd = false;
        filename = movie;
		standByMovie = "";
		threadVideo = new thread(PlayVideo, this);
	}
	else if(movie != filename)
	{
		OnStop();
		standByMovie = movie;
	}

	return 0;
}

void CVideoControlSoft::VideoStart(wxCommandEvent& event)
{
	eventPlayer->OnVideoStart();
    ffmfc_play();
	pause = false;
    videoStart =true;
    fpsTimer->Start(1000);
}

int CVideoControlSoft::getWidth()
{
#ifdef __WXGTK__
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
     return GetSize().x * scale_factor;
}
int CVideoControlSoft::getHeight()
{
#ifdef __WXGTK__
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
    return GetSize().y * scale_factor;
}

void CVideoControlSoft::UpdateScreenRatio()
{
#ifdef __APPLE__
         this->FastRefresh(this,true);
#else
         this->Refresh();
#endif
}

void CVideoControlSoft::OnPaint(wxPaintEvent& event)
{
#ifdef __WXGTK__
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
    GLTexture * glTexture = nullptr;
    //printf("OnPaint Soft Render begin \n"); 
#ifndef WIN32
    if(!renderBitmapOpenGL->IsInit())
    {
        renderBitmapOpenGL->Init(this);
        
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

#ifdef WIN32
    renderBitmapOpenGL->SetCurrent(*this);
#else
    if(updateContext)
    {
        renderBitmapOpenGL->SetCurrent(*this);
        updateContext = false;
    }   
#endif
    

    if (openCLEngine == nullptr)
    {
		openCLEngine = new COpenCLEngine();

		if (openCLEngine != nullptr)
			openclContext = openCLEngine->GetInstance();

		openclEffectYUV = new COpenCLEffectVideoYUV(openclContext);
    }

    nbFrame++;
    printf("Nb Frame per Seconds : %d \n",nbFrame);


    if (videoRenderStart && initStart)
    {
		//nbFrame = 0;
        if(!fpsTimer->IsRunning())
            fpsTimer->Start(1000);
	}

	if(videoRenderStart)
	{
        glTexture = RenderToGLTexture();
        if(glTexture != nullptr)
            printf("glTexture id : %d \n",glTexture->GetTextureID());
    }
    
    if(videoRenderStart && glTexture != nullptr)
    {       
        renderBitmapOpenGL->CreateScreenRender(width, height, CRgbaquad(0,0,0,0));

        if(glTexture != nullptr)
        {
            
            muVideoEffect.lock();
            int enableopenCL = videoEffectParameter.enableOpenCL;
            muVideoEffect.unlock();
            
            int inverted = 1;
            int x = (width - glTexture->GetWidth()) / 2;
            int y = (height  - glTexture->GetHeight()) / 2;
            if(openclContext->IsSharedContextCompatible())
                inverted = 0;
               
            if(!enableopenCL)
                inverted = 1;
                
            if(isffmpegDecode)
                inverted = 1;
                
            //printf("Inverted %d Enable OpenCL %d \n",inverted, enableopenCL);
            printf("flipH : %d flipV : %d inverted : %d \n", flipH, flipV, inverted);
            muVideoEffect.lock();
            renderBitmapOpenGL->RenderWithEffect(x,y, glTexture, &videoEffectParameter, flipH, flipV, inverted);
            printf("Rotation : %d \n",videoEffectParameter.rotation);
            muVideoEffect.unlock();
            
             
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
		renderBitmapOpenGL->CreateScreenRender(width, height, CRgbaquad(0,0,0,0));
	}

	this->SwapBuffers();

    if(deleteTexture && glTexture != nullptr)
        delete glTexture;
    

    double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;


    std::cout<<"Video OnPaint Time : "<< duration <<'\n';
    

   // printf("OnPaint end \n");
}

int CVideoControlSoft::ChangeSubtitleStream(int newStreamSubtitle)
{
	ffmfc_change_subtitle_stream(newStreamSubtitle);
	SetVideoPosition(videoPosition / 1000);
	return 0;
}


int CVideoControlSoft::ChangeAudioStream(int newStreamAudio)
{
	ffmfc_change_audio_stream(newStreamAudio);
	SetVideoPosition(videoPosition / 1000);
	return 0;
}

void CVideoControlSoft::OnPlay()
{
	if (pause && !videoEnd)
	{
		ffmfc_play_pause();
	}
	else if(videoEnd)
    {
		PlayMovie(filename);
    }

	pause = false;
}

void CVideoControlSoft::OnStop()
{
	exit = true;
	stopVideo = true;
    if(ffmfc_quit())
	{
		wxCommandEvent localevent;
		EndVideoThread(localevent);
		//videoEnd = true;
	}
}

void CVideoControlSoft::OnPause()
{
	if (!pause)
	{
		ffmfc_play_pause();
	}
	pause = true;
}

void CVideoControlSoft::SetVideoDuration(int64_t duration)
{
	if (eventPlayer != nullptr)
		eventPlayer->SetVideoDuration(duration);
}

void CVideoControlSoft::SetVideoPosition(int64_t pos)
{
	ffmfc_SetTimePosition(pos * 1000 * 1000);
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
	//this->FastRefresh(this);
}

void CVideoControlSoft::VolumeUp()
{
	ffmfc_VolumeUp();
}

void CVideoControlSoft::VolumeDown()
{
	ffmfc_VolumeDown();
}

int CVideoControlSoft::GetVolume()
{
	return ffmfc_GetVolume();
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
    bool isCPU = IsCPUContext();
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

    std::cout<<"CVideoControlSoft::SetData : "<< duration <<'\n';
    
    this->FastRefresh(this,true);
 
}

void CVideoControlSoft::Resize()
{
    updateContext = true;

    
    if(videoStart)
        ffmfc_videoDisplaySize(GetWindowWidth(),GetWindowHeight() );
  
    if(pause && isffmpegDecode  && copyFrameBuffer != nullptr)
    {
         SetFrameData(copyFrameBuffer);
    } 
    
#ifdef __APPLE__
         this->FastRefresh(this,true);
#else
         this->Refresh();
#endif
}
