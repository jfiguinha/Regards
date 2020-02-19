#include "header.h"
#include "VideoControl_soft.h"
#include <wx/dcbuffer.h>
#include <RegardsFloatBitmap.h>
#include <RegardsBitmap.h>
#include "ffplaycore.h"
#include <Interpolation.h>
#include <config_id.h>
#include "ffmpegToBitmap.h"
#include <ImageLoadingFormat.h>
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
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CVideoControlSoft::OnRButtonDown));
    Connect(TIMER_FPS, wxEVT_TIMER, wxTimerEventHandler(CVideoControlSoft::OnShowFPS), nullptr, this);
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
        if(play)
            ffmfc->Play(this, CConvertUtility::ConvertToStdString(filename));

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

int CVideoControlSoft::GetWidth()
{
	return GetWindowWidth();
}

int CVideoControlSoft::GetHeight()
{
	return GetWindowHeight();
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


#ifdef RENDEROPENGL 
    GLTexture * glTexture = nullptr;

	if (renderBitmapOpenGL == nullptr)
	{
		renderBitmapOpenGL = new CRenderBitmapInterfaceOpenGL(this);

		//Now we have a context, retrieve pointers to OGL functions
		renderBitmapOpenGL->Init(this);
	}

	renderBitmapOpenGL->SetCurrent(*this);
    

    if (openCLEngine == nullptr)
    {
        openCLEngine = new COpenCLEngine();
        if (openCLEngine != nullptr)
            openclContext = openCLEngine->GetInstance();
    }
#endif 

    std::clock_t start;
    start = std::clock();    
    

	int width = GetWidth();
	int height = GetHeight();
    if(width == 0 || height == 0)
        return;
	
	if (quitWindow)
        return;

	int supportOpenCL = 0;
	muVideoEffect.lock();
	supportOpenCL = videoEffectParameter.enableOpenCL;
	muVideoEffect.unlock();

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


    nbFrame++;
    //printf("Nb Frame per Seconds : %d \n",nbFrame);


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
        //if(glTexture != nullptr)
        //    printf("glTexture id : %d \n",glTexture->GetTextureID());
    }
    
    if(videoRenderStart && glTexture != nullptr)
    {       
        renderBitmapOpenGL->CreateScreenRender(width, height, CRgbaquad(0,0,0,0));

        if(glTexture != nullptr)
        {          
            int inverted = 1;
            int x = (width - glTexture->GetWidth()) / 2;
            int y = (height  - glTexture->GetHeight()) / 2;
            if(openclContext->IsSharedContextCompatible())
                inverted = 0;
               
            if(!supportOpenCL)
                inverted = 1;
                
            if(isffmpegDecode)
                inverted = 1;
                
            //printf("Inverted %d Enable OpenCL %d \n",inverted, enableopenCL);
            //printf("flipH : %d flipV : %d inverted : %d \n", flipH, flipV, inverted);
            muVideoEffect.lock();
            renderBitmapOpenGL->RenderWithEffect(x,y, glTexture, &videoEffectParameter, flipH, flipV, inverted);
           // printf("Rotation : %d \n",videoEffectParameter.rotation);
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
        dc.DrawBitmap(resized, 0, 0);
    }
    else
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
	ffmfc->Quit();
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
