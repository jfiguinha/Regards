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
#include <SavePicture.h>
#include <ImageLoadingFormat.h>
#include "ScrollbarWnd.h"
#include "ClosedHandCursor.h"
#include <ConvertUtility.h>
#include <InterpolationBicubic.h>
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
	Connect(wxEVENT_SCROLLMOVE, wxCommandEventHandler(CVideoControlSoft::OnScrollMove));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CVideoControlSoft::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CVideoControlSoft::OnLButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CVideoControlSoft::OnLButtonUp));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CVideoControlSoft::OnKeyDown));
	Connect(wxEVT_KEY_UP, wxKeyEventHandler(CVideoControlSoft::OnKeyUp));
	Connect(wxEVENT_LEFTPOSITION, wxCommandEventHandler(CVideoControlSoft::OnLeftPosition));
	Connect(wxEVENT_TOPPOSITION, wxCommandEventHandler(CVideoControlSoft::OnTopPosition));
	pause = false;
	videoEnd = true;
	this->windowMain = windowMain;
	this->eventPlayer = eventPlayer;
#ifdef RENDEROPENGL 
	openCLEngine = nullptr;
	openclContext = nullptr;
#endif
	openclEffectYUV = nullptr;
	
	hCursorHand = CResourceCursor::GetClosedHand();
	ffmfc = new CFFmfc(this, wxID_ANY);
	
}

CRegardsBitmap * CVideoControlSoft::SavePicture()
{
    CRegardsBitmap * bitmap = nullptr;
	if (isffmpegDecode)
	{
		muBitmap.lock();
		bitmap = ffmpegToBitmap->ConvertFrameToRgba32();
		muBitmap.unlock();
	}
	else
	{
		muBitmap.lock();
		bitmap = openclEffectYUV->GetRgbaBitmap(true);
		muBitmap.unlock();
	}
    return bitmap;
}

bool CVideoControlSoft::IsFFmpegDecode()
{
    return isffmpegDecode;
}

void CVideoControlSoft::ExportPicture(CRegardsBitmap * bitmap)
{
	CImageLoadingFormat * imageLoading = new CImageLoadingFormat();
	bitmap->SetFilename(this->filename);
	if (isffmpegDecode)
		bitmap->VertFlipBuf();
	imageLoading->SetPicture(bitmap);
	CSavePicture::SavePicture(nullptr, imageLoading, filename);
	if (imageLoading != nullptr)
		delete imageLoading;
}

//-----------------------------------------------------------------
//Gestion du click de souris
//-----------------------------------------------------------------
void CVideoControlSoft::OnLButtonDown(wxMouseEvent& event)
{
	TRACE();
	this->SetFocus();
	int xPos = event.GetX();
	int yPos = event.GetY();
#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif

	mouseBlock = true;
	mouseScrollX = xPos;
	mouseScrollY = yPos;
	CaptureMouse();
	wxSetCursor(hCursorHand);
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CVideoControlSoft::OnLButtonUp(wxMouseEvent& event)
{
	TRACE();
	if (event.LeftDClick())
		return;

	int xPos = event.GetX();
	int yPos = event.GetY();

	mouseBlock = false;
	mouseScrollX = xPos;
	mouseScrollY = yPos;
	if (HasCapture())
		ReleaseMouse();
}

//------------------------------------------------------------------------------------
//Déplacement de la souris
//------------------------------------------------------------------------------------
void CVideoControlSoft::OnMouseMove(wxMouseEvent& event)
{
	TRACE();
#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif
	int xPos = event.GetX();
	int yPos = event.GetY();

	::wxSetCursor(hCursorHand);

	if (mouseBlock)
	{
		int diffX = mouseScrollX - xPos;
		int diffY = mouseScrollY - yPos;
		posLargeur += diffX;
		posHauteur += diffY;

		mouseScrollX = xPos;
		mouseScrollY = yPos;

		TestMaxX();
		TestMaxY();

		UpdateScrollBar();
	}
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CVideoControlSoft::TestMaxX()
{
	float bitmapRatioWidth = GetBitmapWidth();
	float screenWidth = float(GetWidth());
	float posLargeurMax = bitmapRatioWidth - screenWidth;

	TRACE();
	int xValue = posLargeurMax;

	if (posLargeur >= xValue)
		posLargeur = xValue;

	if (posLargeur < 0)
		posLargeur = 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CVideoControlSoft::TestMaxY()
{
	float bitmapRatioHeight = GetBitmapHeight();
	float screenHeight = float(GetHeight());
	float posHauteurMax = bitmapRatioHeight - screenHeight;

	TRACE();
	int yValue = posHauteurMax;

	if (posHauteur >= yValue)
		posHauteur = yValue;

	if (posHauteur < 0)
		posHauteur = 0;
}

void CVideoControlSoft::OnLeftPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posLargeur = pos;
	this->Refresh();
}

void CVideoControlSoft::OnTopPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
	this->Refresh();
}

void CVideoControlSoft::OnScrollMove(wxCommandEvent& event)
{
	int isMoving = event.GetInt();
	printf("Is moving !!");
}

void CVideoControlSoft::OnKeyUp(wxKeyEvent& event)
{
	switch (event.GetKeyCode())
	{
	case WXK_CONTROL:
		controlKeyPush = false;
		break;
	}
}

void CVideoControlSoft::MoveTop()
{
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVETOP);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CVideoControlSoft::MoveLeft()
{
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVELEFT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CVideoControlSoft::MoveBottom()
{
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVEBOTTOM);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

vector<int> CVideoControlSoft::GetZoomValue()
{
	vector<int> listValue;
	muVideoEffect.lock();
	for(int i = 0;i < videoEffectParameter.tabZoom.size();i++)
		listValue.push_back(videoEffectParameter.tabZoom[i] * 100.0f);
	muVideoEffect.unlock();
	return listValue;
}

int CVideoControlSoft::GetZoomIndex()
{
	int zoomIndex = 0;
	muVideoEffect.lock();
	zoomIndex = videoEffectParameter.zoomSelect;
	muVideoEffect.unlock();
	return zoomIndex;
}

void CVideoControlSoft::ChangeVideoFormat()
{
	int zoomSelect = 0;
	muVideoEffect.lock();
	videoEffectParameter.ratioSelect++;
	if (videoEffectParameter.ratioSelect >= videoEffectParameter.tabRatio.size())
		videoEffectParameter.ratioSelect = 0;

	muVideoEffect.unlock();

	this->Refresh();

}


float CVideoControlSoft::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight)
{
	TRACE();
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float newRatio = 1;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		newRatio = (float)GetWidth() / (float)(pictureWidth);
	else
		newRatio = (float)GetHeight() / (float)(pictureHeight);

	if ((pictureHeight * newRatio) > GetHeight())
	{
		newRatio = (float)GetHeight() / (float)(pictureHeight);
	}
	if ((pictureWidth * newRatio) > GetWidth())
	{
		newRatio = (float)GetWidth() / (float)(pictureWidth);
	}

	return newRatio;
}

float CVideoControlSoft::GetZoomRatio()
{
	float zoom = 1.0f;
	if (shrinkVideo)
	{
		zoom = CalculPictureRatio(widthVideo, heightVideo);
	}
	else
	{
		
		muVideoEffect.lock();
		zoom = (float)videoEffectParameter.tabZoom[videoEffectParameter.zoomSelect];
		muVideoEffect.unlock();
	}
	return zoom;
}

void CVideoControlSoft::ShrinkVideo()
{
	//CalculCenterPicture();

	int zoomSelect = 0;
	posLargeur = 0;
	posHauteur = 0;
	muVideoEffect.lock();
	float ratio = 1.0f;

	float newRatio = CalculPictureRatio(widthVideo, heightVideo);
	//Calcul Zoom Index
	if (newRatio != 0.0)
	{
		for (int i = 0; i < videoEffectParameter.tabZoom.size(); i++)
		{
			if (newRatio < videoEffectParameter.tabZoom[i])
			{
				ratio = videoEffectParameter.tabZoom[i];
				zoomSelect = i;
				break;
			}
		}
	}

	videoEffectParameter.zoomSelect = zoomSelect;

	muVideoEffect.unlock();

	shrinkVideo = true;

	//CalculPositionPicture(centerX, centerY);

	UpdateScrollBar();


}

void CVideoControlSoft::CalculTextureSize(int &widthOut, int &heightOut)
{

	int width_local = widthVideo;
	int height_local = heightVideo;
	int width = GetWidth();
	int height = GetHeight();
	float zoom = GetZoomRatio();
	float ratio = 1.0f;

	muVideoEffect.lock();
	ratio = (float)videoEffectParameter.tabRatio[videoEffectParameter.ratioSelect];
	muVideoEffect.unlock();

	if (ratio == 1.0f)	
		ratio = (float)widthVideo / (float)heightVideo;
	else
	{
		width_local = (int)((float)height_local * ratio);
	}
	widthOut = width_local * zoom;
	heightOut = height_local * zoom;
}

void CVideoControlSoft::UpdateScrollBar()
{
	int widthOut = 0;
	int heightOut = 0;
	wxWindow * parent = this->GetParent();
	CalculTextureSize(widthOut, heightOut);
	if (parent != nullptr)
	{
		CControlSize * controlSize = new CControlSize();
		wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
		controlSize->controlWidth = widthOut;
		controlSize->controlHeight = heightOut;
		evt.SetClientData(controlSize);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}

	if (parent != nullptr)
	{
		wxSize * size = new wxSize();
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		size->x = posLargeur;
		size->y = posHauteur;
		evt.SetClientData(size);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
	this->Refresh();
}

void CVideoControlSoft::MoveRight()
{
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVERIGHT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CVideoControlSoft::ZoomOn()
{
	CalculCenterPicture();

	muVideoEffect.lock();
	videoEffectParameter.zoomSelect++;
	muVideoEffect.unlock();

	CalculPositionPicture(centerX, centerY);

	UpdateScrollBar();
}

void CVideoControlSoft::ZoomOut()
{
	CalculCenterPicture();

	muVideoEffect.lock();
	videoEffectParameter.zoomSelect--;
	muVideoEffect.unlock();

	CalculPositionPicture(centerX, centerY);

	UpdateScrollBar();
}

void CVideoControlSoft::OnKeyDown(wxKeyEvent& event)
{
	switch (event.GetKeyCode())
	{
	case WXK_UP:
		MoveTop();
		break;
	case WXK_LEFT:
		MoveLeft();
		break;
	case WXK_DOWN:
		MoveBottom();
		break;
	case WXK_RIGHT:
		MoveRight();
		break;

	case WXK_CONTROL:
		controlKeyPush = true;
		break;

	case WXK_NUMPAD_ADD:
	case WXK_ADD:
	{
		this->ZoomOn();
	}
	break;

	case WXK_NUMPAD_SUBTRACT:
	case WXK_SUBTRACT:
	{
		this->ZoomOut();
	}
	break;

	}
	

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
	bool updateScroll = false;
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

	if (videoParameter->zoomUpdate)
	{
		videoParameter->zoomUpdate = false;
		updateScroll = true;
	}

	if (updateScroll)
	{
		CalculCenterPicture();
	}

	muVideoEffect.lock();
	videoEffectParameter = *videoParameter;
	muVideoEffect.unlock();

	if (updateScroll)
	{
		CalculPositionPicture(centerX, centerY);
		UpdateScrollBar();
	}
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
		ffmfc->SetVolume(GetSoundVolume());

		wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_HIDESAVEBUTTON);
			window->GetEventHandler()->AddPendingEvent(evt);
		}


		
		muVideoEffect.lock();
		videoEffectParameter.ratioSelect = 0;
		muVideoEffect.unlock();


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

	wxWindow * window = this->FindWindowById(SHOWVIDEOVIEWERID);
	if (window != nullptr)
	{
		wxCommandEvent evt(wxEVENT_SHRINK);
		window->GetEventHandler()->AddPendingEvent(evt);
	}

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
	GLTexture * glTextureOutput = nullptr;

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

    if(IsSupportOpenCL())
    {
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
              
		glTextureOutput = DisplayTexture(glTexture);

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

	if (glTextureOutput != nullptr)
		delete glTextureOutput;
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
			wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
			if (window != nullptr)
			{
				wxCommandEvent evt(wxEVENT_HIDESAVEBUTTON);
				window->GetEventHandler()->AddPendingEvent(evt);
			}
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

	wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
	if (window != nullptr)
	{
		wxCommandEvent evt(wxEVENT_HIDESAVEBUTTON);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
}

float CVideoControlSoft::GetLargeurMax()
{
	float bitmapRatioWidth = GetBitmapWidth();
	float screenWidth = float(GetWidth());
	return bitmapRatioWidth - screenWidth;
}

float CVideoControlSoft::GetHauteurMax()
{
	float bitmapRatioHeight = GetBitmapHeight();
	float screenHeight = float(GetHeight());
	return bitmapRatioHeight - screenHeight;
}

void CVideoControlSoft::CalculPositionPicture(const float &x, const float &y)
{
	TRACE();

	float bitmapRatioWidth = GetBitmapWidth();
	float bitmapRatioHeight = GetBitmapHeight();
	float screenWidth = float(GetWidth());
	float screenHeight = float(GetHeight());

	float posLargeurMax = bitmapRatioWidth - screenWidth;
	float posHauteurMax = bitmapRatioHeight - screenHeight;

	float ratio = GetZoomRatio();

	float middleScreenWidth = screenWidth / 2.0f;
	float middleScreenHeight = screenHeight / 2.0f;

	if (screenWidth > bitmapRatioWidth)
	{
		posLargeur = 0;
	}
	else
	{
		posLargeur = x * posLargeurMax;
	}

	if (screenHeight > bitmapRatioHeight)
	{
		posHauteur = 0;
	}
	else
	{
		posHauteur = y * posHauteurMax;
	}

	TestMaxY();
	TestMaxX();
}


int CVideoControlSoft::GetBitmapWidth()
{
	TRACE();
	int localAngle = angle;
	int widthOut = 0;
	int heightOut = 0;
	CalculTextureSize(widthOut, heightOut);

	if (localAngle == 90 || localAngle == 270)
		return heightOut;
	else
		return widthOut;

	return 0;
}

int CVideoControlSoft::GetBitmapHeight()
{
	TRACE();
	int localAngle = angle;
	int widthOut = 0;
	int heightOut = 0;
	CalculTextureSize(widthOut, heightOut);

	if (localAngle == 90 || localAngle == 270)
		return widthOut;
	else
		return heightOut;

	return 0;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CVideoControlSoft::MouseClick(const int &xPos, const int &yPos)
{
	TRACE();
	mouseScrollX = xPos;
	mouseScrollY = yPos;
	mouseBlock = true;
	CaptureMouse();
	SetFocus();
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CVideoControlSoft::MouseRelease(const int &xPos, const int &yPos)
{
	TRACE();
	mouseBlock = false;
	wxSetCursor(wxCursor(wxCURSOR_ARROW));
	if (HasCapture())
		ReleaseMouse();
}

void CVideoControlSoft::CalculCenterPicture()
{
	TRACE();

	float bitmapRatioWidth = GetBitmapWidth();
	float bitmapRatioHeight = GetBitmapHeight();
	float screenWidth = float(GetWidth());
	float screenHeight = float(GetHeight());


	float middleScreenWidth = screenWidth / 2.0f;
	float middleScreenHeight = screenHeight / 2.0f;

	float posLargeurMax = bitmapRatioWidth - screenWidth;
	float posHauteurMax = bitmapRatioHeight - screenHeight;

	float ratio = GetZoomRatio();

	if (ratio == 1.0)
	{
		centerX = 0;
		centerY = 0;
	}

	if (screenWidth > bitmapRatioWidth)
	{
		centerX = 0;
	}
	else
	{
		if (centerX == 0)
			centerX = 0.5f;
		else
			centerX = ((float)posLargeur ) / posLargeurMax;
	}

	if (screenHeight > bitmapRatioHeight)
	{
		centerY = 0;
	}
	else
	{
		if (centerY == 0)
			centerY = 0.5f;
		else
			centerY =((float)posHauteur) / posHauteurMax;
	}

	centerX = min(max(centerX, 0.0f), 1.0f);
	centerY = min(max(centerY, 0.0f), 1.0f);
}

void CVideoControlSoft::OnPause()
{
	if (videoStart)
	{
		if (!pause)
		{
			ffmfc->Pause();

			wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
			if (window != nullptr)
			{
				wxCommandEvent evt(wxEVENT_SHOWSAVEBUTTON);
				window->GetEventHandler()->AddPendingEvent(evt);
			}
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

void CVideoControlSoft::SetVolume(const int &pos)
{
	ffmfc->SetVolume(pos);
	SetSoundVolume(pos);
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
		wxCommandEvent evt(TOOLBAR_UPDATE_ID);
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
	ratioVideo =(float) src_frame->width / (float)src_frame->height;

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

GLTexture * CVideoControlSoft::DisplayTexture(GLTexture * glTexture)
{
	GLTexture * glTextureOutput = nullptr;

	if (displaywithInterpolation)
	{
		if (glTexture != nullptr)
		{

			float zoomRatio = GetZoomRatio();
			muVideoEffect.lock();
			int bicubic = videoEffectParameter.BicubicEnable;
			muVideoEffect.unlock();
			wxRect posrect;
			wxRect rect;
			int filterInterpolation = 0;
			CRegardsConfigParam * regardsParam = CParamInit::getInstance();

			if (regardsParam != nullptr)
				filterInterpolation = regardsParam->GetInterpolationType();

				int widthOut = 0;
				int heightOut = 0;
				CalculTextureSize(widthOut, heightOut);


				posrect.x = std::max((double)posLargeur, (double)0);
				posrect.y = std::max((double)posHauteur, (double)0);
				posrect.width = widthOut;
				posrect.height = heightOut;

				if (angle == 90 || angle == 270)
				{
					rect.height = getHeight();
					rect.width = getWidth();
					if (rect.height > widthOut)
						rect.height = widthOut;
					if (rect.width > heightOut)
						rect.width = heightOut;

					glTextureOutput = new GLTexture(rect.height, rect.width);
				}
				else
				{
					rect.height = getHeight();
					rect.width = getWidth();

					if (rect.height > heightOut)
						rect.height = heightOut;
					if (rect.width > widthOut)
						rect.width = widthOut;

					glTextureOutput = new GLTexture(rect.width, rect.height);
				}
            renderBitmapOpenGL->RenderWithEffectInterpolation(glTexture, glTextureOutput, posrect, &videoEffectParameter, flipH, flipV, angle, filterInterpolation, zoomRatio, true);
 		}
	}
	else
	{
		if (glTexture != nullptr)
		{
			muVideoEffect.lock();
            renderBitmapOpenGL->RenderWithEffect(glTexture, &videoEffectParameter, true);
			muVideoEffect.unlock();
		}
	}


	return glTextureOutput;
}

void CVideoControlSoft::Resize()
{
	float screenWidth = float(GetWidth());
	float screenHeight = float(GetHeight());

	if (!stopVideo)
	{
		updateContext = true;

		if (videoStart)
			ffmfc->VideoDisplaySize(GetWindowWidth(), GetWindowHeight());

		if (pause && isffmpegDecode  && copyFrameBuffer != nullptr)
		{
			SetFrameData(copyFrameBuffer);
		}

		if (shrinkVideo)
		{
			ShrinkVideo();
		}
		else if(oldWidth != 0 && oldHeight != 0)
		{
			float bitmapRatioWidth = GetBitmapWidth();
			float bitmapRatioHeight = GetBitmapHeight();

			float posLargeurMax = bitmapRatioWidth - oldWidth;
			float posHauteurMax = bitmapRatioHeight - oldHeight;

			float posX = posLargeur / posLargeurMax;
			float posY = posHauteur / posHauteurMax;

			posLargeurMax = bitmapRatioWidth - screenWidth;
			posHauteurMax = bitmapRatioHeight - screenHeight;

			posLargeur = posX * posLargeurMax;
			posHauteur = posY * posHauteurMax;

			TestMaxY();
			TestMaxX();

			UpdateScrollBar();
		}
			
		Refresh();
	}
	oldWidth = screenWidth;
	oldHeight = screenHeight;
	
}

void CVideoControlSoft::calculate_display_rect(wxRect *rect, int scr_xleft, int scr_ytop, int scr_width, int scr_height)
{

	float aspect_ratio = video_aspect_ratio;
	int width, height, x, y;
	float ratio = 1.0f;
	float zoom = GetZoomRatio();

	muVideoEffect.lock();
	aspect_ratio = (float)videoEffectParameter.tabRatio[videoEffectParameter.ratioSelect];
	muVideoEffect.unlock();

	if (aspect_ratio == 1.0)
		aspect_ratio = (float)widthVideo / (float)heightVideo;

	/* XXX: we suppose the screen has a 1.0 pixel ratio */
	height = scr_height * zoom;
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

void CVideoControlSoft::SetZoomIndex(const int &pos)
{
	CalculCenterPicture();
	shrinkVideo = false;
	float zoomRatio = 1.0f;
	muVideoEffect.lock();
	zoomRatio = videoEffectParameter.tabZoom[pos];
	videoEffectParameter.zoomSelect = pos;
	muVideoEffect.unlock();

	if (zoomRatio != 1.0f)
	{
		CalculPositionPicture(centerX, centerY);
		UpdateScrollBar();
	}
}


GLTexture * CVideoControlSoft::RenderToTexture(COpenCLEffectVideo * openclEffect)
{
    if(openclEffect == nullptr)
        return nullptr;

	float zoomRatio = GetZoomRatio();

	GLTexture * glTexture = nullptr;
	wxRect rect;
	int filterInterpolation = 0;
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();

	if (regardsParam != nullptr)
		filterInterpolation = regardsParam->GetInterpolationType();

	openclEffect->TranscodePicture(widthVideo, heightVideo);
	//CRegardsBitmap * data = openclEffect->GetRgbaBitmap();
	//data->SaveToBmp("d:\\test.bmp");

		int widthOut = 0;
		int heightOut = 0;
		CalculTextureSize(widthOut, heightOut);

		wxRect posrect;
		posrect.x = std::max((double)posLargeur, (double)0);
		posrect.y = std::max((double)GetHauteurMax() - posHauteur - 1,(double)0);
		posrect.width = widthOut;
		posrect.height = heightOut;

		if (angle == 90 || angle == 270)
		{
			rect.height = getHeight();
			rect.width = getWidth();
			if (rect.height > widthOut)
				rect.height = widthOut;
			if (rect.width > heightOut)
				rect.width = heightOut;
			openclEffect->InterpolationZoomBicubic(rect.height, rect.width, posrect, flipH, flipV, angle, filterInterpolation);
		}
		else
		{
			rect.height = getHeight();
			rect.width = getWidth();

			if (rect.height > heightOut)
				rect.height = heightOut;
			if (rect.width > widthOut)
				rect.width = widthOut;

			openclEffect->InterpolationZoomBicubic(rect.width, rect.height, posrect, flipH, flipV, angle, filterInterpolation);
		}

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
			openclEffect->FlipVertical();
			CRegardsBitmap * bitmap = openclEffect->GetRgbaBitmap();
			//bitmap->VertFlipBuf();
			glTexture->SetData(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
			delete bitmap;
		}
		else
			printf("CVideoControl glTexture Error \n");
	}
	//inverted = true;
	return glTexture;
}

GLTexture * CVideoControlSoft::RenderFFmpegToTexture()
{
	GLTexture * glTexture = new GLTexture(widthVideo, heightVideo);
	CRegardsBitmap * bitmap = ffmpegToBitmap->ConvertFrameToRgba32();
	glTexture->Create(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap());
	deleteTexture = true;
	delete bitmap;
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

void CVideoControlSoft::SetSoundVolume(const int &soundVolume)
{
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		config->SetSoundVolume(soundVolume);
}

int CVideoControlSoft::GetSoundVolume()
{
	int soundVolume = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		soundVolume = config->GetSoundVolume();

	return soundVolume;
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
		SwsContext* scaleContext = nullptr;
		bool deleteData = false;
		isffmpegDecode = true;

		//deleteData = true;
		if (widthVideo != 0 && heightVideo != 0)
		{
			muBitmap.lock();

			if (ffmpegToBitmap == nullptr)
			{
				ffmpegToBitmap = new CffmpegToBitmap(true);
				ffmpegToBitmap->InitContext(src_frame, 0, widthVideo, heightVideo);
			}

			if (ffmpegToBitmap->GetThumbnailWidth() != widthVideo || ffmpegToBitmap->GetThumbnailHeight() != heightVideo)
				deleteData = true;

			if (deleteData)
			{
				ffmpegToBitmap->DeleteData();
				ffmpegToBitmap->InitContext(src_frame, 0, widthVideo, heightVideo);
			}
			ffmpegToBitmap->Preconvert(src_frame, widthVideo, heightVideo);
			muBitmap.unlock();
		}


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
			displaywithInterpolation = false;
			muBitmap.lock();
			glTexture = RenderToTexture(openclEffectYUV);
			muBitmap.unlock();
		}

		deleteTexture = false;
	}
	else
	{
		displaywithInterpolation = true;
		muBitmap.lock();
		glTexture = RenderFFmpegToTexture();
		muBitmap.unlock();

	}

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

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