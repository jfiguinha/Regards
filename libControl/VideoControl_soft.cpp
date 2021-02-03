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
#include <InterpolationFilters.h>
#include <SavePicture.h>
#include <ImageLoadingFormat.h>
#include "ScrollbarWnd.h"
#include "ClosedHandCursor.h"
#include <ConvertUtility.h>
#include <libPicture.h>
#include <InterpolationBicubic.h>
#include <videothumb.h>
#include <hqdn3d.h>
#include <OpenCVEffect.h>
using namespace Regards::OpenCV;
//#include "LoadingResource.h"
wxDEFINE_EVENT(TIMER_FPS,  wxTimerEvent);
wxDEFINE_EVENT(TIMER_PLAYSTART, wxTimerEvent);
wxDEFINE_EVENT(TIMER_PLAYSTOP, wxTimerEvent);
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
	hq3d = nullptr;// new Chqdn3d(videoEffectParameter.denoisingLevel);
	widthVideo = 0;
	heightVideo = 0;
	subtilteUpdate = false;
	startingTime = 0;
	old_width = 0;
	old_height = 0;
	thumbnailVideo = nullptr;
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
	Connect(wxEVENT_STOPVIDEO, wxCommandEventHandler(CVideoControlSoft::StopVideoThread));
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
	Connect(wxEVENT_SETPOSITION, wxCommandEventHandler(CVideoControlSoft::OnSetPosition));
    playStopTimer = new wxTimer(this, TIMER_PLAYSTOP);
    Connect(TIMER_PLAYSTOP, wxEVT_TIMER, wxTimerEventHandler(CVideoControlSoft::OnPlayStop), nullptr, this);
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
	pictureFrame = new CRegardsBitmap();
}

COpenCLEngine * CVideoControlSoft::GetOpenCLEngine()
{
	return openCLEngine;
}

void CVideoControlSoft::OnPlayStop(wxTimerEvent& event)
{
    OnStop(filename);
}

bool CVideoControlSoft::IsPause()
{
	return pause;
}

float CVideoControlSoft::GetMovieRatio()
{
	float ratioSelect = 0;
	muVideoEffect.lock();
	ratioSelect = videoEffectParameter.tabRatio[videoEffectParameter.ratioSelect];
	muVideoEffect.unlock();
	return ratioSelect;
}

CRegardsBitmap * CVideoControlSoft::SavePicture(bool & isFromBuffer)
{
	CRegardsBitmap * bitmap = nullptr;
	if (thumbnailFromBitmap)
	{
		bitmap = new CRegardsBitmap();
		muBitmap.lock();
		*bitmap = *pictureVideo;
		bitmap->VertFlipBuf();
		muBitmap.unlock();
		isFromBuffer = true;
	}
	else
	{
		isFromBuffer = false;
		if (isffmpegDecode)
		{
			bitmap = new CRegardsBitmap();
			muBitmap.lock();
			*bitmap = *pictureFrame;
			muBitmap.unlock();
		}
		else
		{
			muBitmap.lock();
			bitmap = openclEffectYUV->GetRgbaBitmap(true);
			muBitmap.unlock();
		}
	}


    return bitmap;
}

bool CVideoControlSoft::IsFFmpegDecode()
{
    return isffmpegDecode;
}

/*
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
*/
//-----------------------------------------------------------------
//Gestion du click de souris
//-----------------------------------------------------------------
void CVideoControlSoft::OnLButtonDown(wxMouseEvent& event)
{
	TRACE();
	this->SetFocus();
	int xPos = event.GetX();
	int yPos = event.GetY();
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

void CVideoControlSoft::GenerateThumbnailVideo(void * data)
{
	CVideoControlSoft * videoSoft = (CVideoControlSoft *)data;
	videoSoft->muBitmap.lock();
	videoSoft->pictureVideo = videoSoft->thumbnailVideo->GetVideoFrame(videoSoft->videoPosition, 0, 0);
	videoSoft->muBitmap.unlock();
	videoSoft->threadVideoEnd = true;

	wxCommandEvent evt(wxEVENT_REFRESH);
	videoSoft->GetEventHandler()->AddPendingEvent(evt);
	//videoSoft->Refresh();
}

void CVideoControlSoft::OnSetPosition(wxCommandEvent& event)
{
	long pos = event.GetExtraLong();
	if (videoPosition == pos)
		return;
	if (stopVideo)
		OnPlay();
	videoPosition = pos;

	if (openCVStabilization != nullptr)
		openCVStabilization->Init();

	ffmfc->SetTimePosition(videoPosition * 1000 * 1000);
	if (pause && thumbnailVideo != nullptr)
	{
		thumbnailFromBitmap = true;
		if (threadVideoEnd)
		{
			if (_threadVideo != nullptr)
			{
				_threadVideo->join();
				delete _threadVideo;
			}
			_threadVideo = new thread(GenerateThumbnailVideo, this);
			threadVideoEnd = false;
		}
	}
	else
		thumbnailFromBitmap = false;
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
		zoom = CalculPictureRatio(GetSrcBitmapWidth(), GetSrcBitmapHeight());
	}
	else
	{
		
		muVideoEffect.lock();
		zoom = (float)videoEffectParameter.tabZoom[videoEffectParameter.zoomSelect];
		muVideoEffect.unlock();
	}
	return zoom;
}

float CVideoControlSoft::CalculRatio(const int &pictureWidth, const int &pictureHeight)
{
   TRACE();

	float newRatio = CalculPictureRatio(GetSrcBitmapWidth(), GetSrcBitmapHeight());
    int zoomSelect = 0;
	//Détermination du ration par rapport au tableau
	printf("Ratio %f \n",newRatio);
	muVideoEffect.lock();
	
	//Calcul Zoom Index
	if (newRatio != 0.0)
	{
		for (int i = 0; i < videoEffectParameter.tabZoom.size(); i++)
		{
            printf("Ratio %f \n",videoEffectParameter.tabZoom[i]);
			if (newRatio < videoEffectParameter.tabZoom[i])
			{
				//ratio = videoEffectParameter.tabZoom[i];
				zoomSelect = i;
				break;
			}
		}
	}

    printf("Ratio index %d \n",zoomSelect);

	videoEffectParameter.zoomSelect = zoomSelect;

	muVideoEffect.unlock();
    
	return newRatio;
}

void CVideoControlSoft::ShrinkVideo()
{
    TRACE();

	float ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	//Calcul position largeur et hauteur initial
	posLargeur = 0;
	posHauteur = 0;
	centerX = float(GetBitmapWidth()) / 2.0f;
	centerY = float(GetBitmapHeight()) / 2.0f;

	UpdateScrollBar();

	shrinkVideo = true;
}

void CVideoControlSoft::CalculTextureSize(int &widthOut, int &heightOut)
{
	int width_local = GetSrcBitmapWidth();
	int height_local = GetSrcBitmapHeight();
	float zoom = GetZoomRatio();
	float ratio = 1.0f;

	/*
	muVideoEffect.lock();
	ratio = (float)videoEffectParameter.tabRatio[videoEffectParameter.ratioSelect];
	muVideoEffect.unlock();

	if (ratio == 1.0f)	
		ratio = (float)GetSrcBitmapWidth() / (float)GetSrcBitmapHeight();
	else
	{
		width_local = (int)((float)height_local * ratio);
	}
	*/
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
	else
	{
		if (pause)
			Refresh();
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
	if(!videoEnd)
	{
		if (stopVideo && !processVideoEnd)
		{
			if (!videoEnd)
			{
				ffmfc->Quit();
				processVideoEnd = true;
			}
		}
	}


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
	ffmfc->SetFile(this, CConvertUtility::ConvertToStdString(filename), IsHardwareCompatible() ? acceleratorHardware : "", isOpenGLDecoding, GetSoundVolume());
}

void CVideoControlSoft::SetEncoderHardware(const wxString &encoderHardware, const bool &opengl)
{
	acceleratorHardware = encoderHardware;
	isOpenGLDecoding = opengl;
}

bool CVideoControlSoft::GetDXVA2Compatible()
{
	return isDXVA2Compatible;
}

void CVideoControlSoft::EndVideoThread(wxCommandEvent& event)
{
	if (!endProgram)
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
	}
	else
	{
		fpsTimer->Stop();
		videoRenderStart = false;
		stopVideo = true;
		videoEnd = true;
	}

}


void CVideoControlSoft::StopVideoThread(wxCommandEvent& event)
{
	//OnStop(filename);
	if (!stopVideo)
	{
		//this->OnPause();
		if (eventPlayer != nullptr)
		{
			eventPlayer->OnPositionVideo(0);
			eventPlayer->OnVideoEnd();
		}
		fpsTimer->Stop();
		videoRenderStart = false;
		stopVideo = true;
	}

}


CVideoControlSoft::~CVideoControlSoft()
{
	if (_threadVideo != nullptr)
	{
		_threadVideo->join();
		delete _threadVideo;
	}

	if(playStartTimer->IsRunning())
		playStartTimer->Stop();

    if(playStopTimer->IsRunning())
		playStopTimer->Stop();

	if(hq3d != nullptr)
		delete hq3d;

	if (openCVStabilization != nullptr)
		delete openCVStabilization;

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
    
	if(pictureSubtitle != nullptr)
		delete pictureSubtitle;

	if (ffmfc)
		delete ffmfc;

	if(pictureFrame != nullptr)
		delete pictureFrame;

	if (thumbnailVideo != nullptr)
		delete thumbnailVideo;

	if (localContext != nullptr)
		sws_freeContext(localContext);
	localContext = nullptr;
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

bool CVideoControlSoft::IsHardwareCompatible()
{
	if (acceleratorHardware != "")
	{
		CThumbnailVideo video(filename);
		return video.IsHardwareDecoderCompatible();
	}
	return true;
}

void CVideoControlSoft::PlayFirstMovie(const bool &firstMovie)
{
	this->firstMovie = firstMovie;
}

int CVideoControlSoft::PlayMovie(const wxString &movie, const bool &play)
{
	if (videoEnd || stopVideo)
	{
		if (thumbnailVideo != nullptr)
			delete thumbnailVideo;

		if (localContext != nullptr)
			sws_freeContext(localContext);
		localContext = nullptr;

		thumbnailVideo = new CThumbnailVideo(movie);

		if (openCVStabilization != nullptr)
			delete openCVStabilization;

		openCVStabilization = nullptr;

		if(playStopTimer->IsRunning())
			playStopTimer->Stop();

		if(playStartTimer->IsRunning())
			playStartTimer->Stop();
		startVideo = play;
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
        ffmfc->SetFile(this, CConvertUtility::ConvertToStdString(filename), IsHardwareCompatible() ? acceleratorHardware : "", isOpenGLDecoding, firstMovie ? 0 : GetSoundVolume());
        muVideoEffect.lock();
		videoEffectParameter.ratioSelect = 0;
		muVideoEffect.unlock();
        if(firstMovie)
            playStopTimer->Start(100,true);
        firstMovie = false;
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
	if (startVideo)
	{
		ffmfc->Play();
		pause = false;
		videoEnd = false;
		videoStart = true;
		fpsTimer->Start(1000);


		wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_HIDESAVEBUTTON);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
	}



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
    double scale_factor = 1.0f;//GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
     return GetSize().x * scale_factor;
}
int CVideoControlSoft::getHeight()
{
#ifndef WIN32
    double scale_factor = 1.0f;//GetContentScaleFactor();
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
    printf("CVideoControlSoft::OnPaint \n");
	deleteTexture = false;
	inverted = true;
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
            openCLEngine = new COpenCLEngine(false);
            if (openCLEngine != nullptr)
                openclContext = openCLEngine->GetInstance();
        }
    }
#endif 

    std::clock_t start;
    start = std::clock();    
    

    int width = GetWindowWidth() * scale_factor;
    int height = GetWindowHeight()* scale_factor;
    if(width == 0 || height == 0)
        return;
	
	if (quitWindow)
        return;

    if(IsSupportOpenCL())
    {
		if (openclEffectYUV == nullptr)
        {
            openclEffectYUV = new COpenCLEffectVideoYUV(openclContext);
        }  
    }

    nbFrame++;

    if (videoRenderStart && initStart)
    {
		//nbFrame = 0;
        if(!fpsTimer->IsRunning())
            fpsTimer->Start(1000);
	}

#ifdef RENDEROPENGL 
	if (videoRenderStart)
	{
		if (thumbnailVideo != nullptr && pause && pictureVideo != nullptr)
		{
			muBitmap.lock();
			glTexture = RenderFFmpegToTexture(pictureVideo);
			muBitmap.unlock();
		}
		else
			glTexture = RenderToGLTexture();
	}

    
//#ifdef __APPLE__
/*
    if(videoRenderStart && glTexture != nullptr)
    {  
        renderBitmapOpenGL->CreateScreenRender(width, height, CRgbaquad(0,0,0,0));
        renderBitmapOpenGL->RenderWithEffect(glTexture, &videoEffectParameter, false);
    }
*/
//#else
    
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
        printf("renderBitmapOpenGL->CreateScreenRender \n");
		renderBitmapOpenGL->CreateScreenRender(width, height, CRgbaquad(0,0,0,0));
	}
//#endif

	this->SwapBuffers();

    if(deleteTexture && glTexture != nullptr)
        delete glTexture;

	if (glTextureOutput != nullptr)
		delete glTextureOutput;
        
    //if(deleteTexture)
    //renderBitmapOpenGL->DeleteTexture();

#endif
    double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;


    //std::cout<<"Video OnPaint Time : "<< duration <<'\n';
    

   // printf("OnPaint end \n");
}

int CVideoControlSoft::ChangeSubtitleStream(int newStreamSubtitle)
{
	ffmfc->Change_subtitle_stream(newStreamSubtitle);
	//SetVideoPosition(videoPosition / 1000);
	wxCommandEvent evt(wxEVENT_SETPOSITION);
	evt.SetExtraLong(videoPosition / 1000);
	this->GetEventHandler()->AddPendingEvent(evt);
	return 0;
}


int CVideoControlSoft::ChangeAudioStream(int newStreamAudio)
{
	ffmfc->Change_audio_stream(newStreamAudio);
	//SetVideoPosition(videoPosition / 1000);
	wxCommandEvent evt(wxEVENT_SETPOSITION);
	evt.SetExtraLong(videoPosition / 1000);
	this->GetEventHandler()->AddPendingEvent(evt);
	return 0;
}

void CVideoControlSoft::OnPlay()
{
	thumbnailFromBitmap = false;
	if (videoStart)
	{
		bool _videoEnd = videoEnd;
		if(!_videoEnd)
			if (stopVideo)
				_videoEnd = true;

		if (pause && !_videoEnd)
		{
			ffmfc->Pause();
			wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
			if (window != nullptr)
			{
				wxCommandEvent evt(wxEVENT_HIDESAVEBUTTON);
				window->GetEventHandler()->AddPendingEvent(evt);
			}
		}
		else if (videoEnd || stopVideo)
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
	if (playStartTimer->IsRunning())
		playStartTimer->Stop();

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
	return widthOut;

}

int CVideoControlSoft::GetBitmapHeight()
{
	int localAngle = angle;
	int widthOut = 0;
	int heightOut = 0;
	CalculTextureSize(widthOut, heightOut);
	return heightOut;
}

int CVideoControlSoft::GetSrcBitmapWidth()
{
	TRACE();
	int localAngle = angle;
	float ratioSelect = GetMovieRatio();
	float _widthVideo = widthVideo;
	float _heightVideo = heightVideo;
	if (ratioSelect != 1.0f)
	{
		_widthVideo = _heightVideo * ratioSelect;
	}

	if (localAngle == 90 || localAngle == 270)
		return _heightVideo;
	else
		return _widthVideo;
	return 0;
}

int CVideoControlSoft::GetSrcBitmapHeight()
{
	TRACE();
	float ratioSelect = GetMovieRatio();
	float _widthVideo = widthVideo;
	float _heightVideo = heightVideo;
	if (ratioSelect != 1.0f)
	{
		_widthVideo = _heightVideo * ratioSelect;
	}
	int localAngle = angle;
	if (localAngle == 90 || localAngle == 270)
		return _widthVideo;
	else
		return _heightVideo;
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

void CVideoControlSoft::SetVideoDuration(const int64_t & duration, const int64_t & startTime)
{
	startingTime = startTime;

	if (eventPlayer != nullptr)
		eventPlayer->SetVideoDuration(duration);
}

/*
void CVideoControlSoft::SetVideoPosition(const int64_t &  pos)
{
	ffmfc->SetTimePosition(pos * 1000 * 1000);
	if (pause && thumbnailVideo != nullptr)
	{
		muBitmap.lock();

		pictureVideo = thumbnailVideo->GetVideoFrame(pos,0,0);

		muBitmap.unlock();
		this->Refresh();
	}
}
*/
void CVideoControlSoft::SetCurrentclock(wxString message)
{
	this->message = message;
}

void CVideoControlSoft::SetPos(int64_t pos)
{
	if (!videoEnd)
	{
		videoPosition = (pos * 1000) - startingTime;
		videoPosition = videoPosition / 1000;
		if (eventPlayer != nullptr)
			eventPlayer->OnPositionVideo(videoPosition);
	}

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


CRegardsBitmap * CVideoControlSoft::GetBitmapRGBA(AVFrame * tmp_frame)
{
	if (bitmapData == nullptr)
		bitmapData = new CRegardsBitmap(tmp_frame->width, tmp_frame->height);

	if (localContext == nullptr)
	{
		localContext = sws_alloc_context();

		av_opt_set_int(localContext, "srcw", tmp_frame->width, 0);
		av_opt_set_int(localContext, "srch", tmp_frame->height, 0);
		av_opt_set_int(localContext, "src_format", tmp_frame->format, 0);
		av_opt_set_int(localContext, "dstw", tmp_frame->width, 0);
		av_opt_set_int(localContext, "dsth", tmp_frame->height, 0);
		av_opt_set_int(localContext, "dst_format", AV_PIX_FMT_BGRA, 0);
		av_opt_set_int(localContext, "sws_flags", SWS_FAST_BILINEAR, 0);

		if (sws_init_context(localContext, nullptr, nullptr) < 0)
		{
			sws_freeContext(localContext);
			throw std::logic_error("Failed to initialise scale context");
		}
	}

	int numBytes = avpicture_get_size(AV_PIX_FMT_BGRA, tmp_frame->width, tmp_frame->height);
	if (numBytes != bitmapData->GetBitmapSize())
	{
		bitmapData->SetBitmap(tmp_frame->width, tmp_frame->height);
	}

	uint8_t * convertedFrameBuffer = bitmapData->GetPtBitmap();
	int linesize = tmp_frame->width * 4;

	sws_scale(localContext, tmp_frame->data, tmp_frame->linesize, 0, tmp_frame->height,
		&convertedFrameBuffer, &linesize);

	bitmapData->VertFlipBuf();

	return bitmapData;
}


void CVideoControlSoft::SetData(void * data, const float & sample_aspect_ratio, void * dxva2Context)
{
    std::clock_t start = std::clock();
    
    bool isCPU = 1;
    if(IsSupportOpenCL())
       isCPU = IsCPUContext();
    
    printf("Set Data Begin \n");
     
	videoRenderStart = true; 

	AVFrame *src_frame = (AVFrame *)data;

    video_aspect_ratio = sample_aspect_ratio;

	bool frameStabilized = false;

	if (videoEffectParameter.stabilizeVideo && videoEffectParameter.effectEnable)
	{
		if (previousFrame == nullptr)
			previousFrame = new CRegardsBitmap();

		GetBitmapRGBA(src_frame);
		*previousFrame = *bitmapData;

		if (openCVStabilization == nullptr)
			openCVStabilization = new COpenCVStabilization(videoEffectParameter.stabilizeImageBuffere);

		openCVStabilization->SetNbFrameBuffer(videoEffectParameter.stabilizeImageBuffere);

		if (openCVStabilization->GetNbFrameBuffer() == 0)
		{
			openCVStabilization->BufferFrame(previousFrame);
		}
		else
		{
			frameStabilized = true;
			openCVStabilization->AddFrame(previousFrame);
		}

		if(frameStabilized)
			openCVStabilization->CorrectFrame(bitmapData);
	}
	

	if(!frameStabilized)
	{
		SetFrameData(src_frame);
	}
	else
	{
		muBitmap.lock();		
		*pictureFrame = *bitmapData;
		muBitmap.unlock();
		
	}

    
    widthVideo = src_frame->width;
    heightVideo = src_frame->height;  
	ratioVideo =(float) src_frame->width / (float)src_frame->height;

    double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    wxCommandEvent event(wxEVENT_REFRESH);
    wxPostEvent(this, event);  
    //std::cout<<"CVideoControlSoft::SetData : "<< duration <<'\n';
}

int CVideoControlSoft::IsOpenGLDecoding()
{
  	int supportOpenGL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenGL = config->GetVideoLibrary();

	return supportOpenGL;  
}

GLTexture * CVideoControlSoft::DisplayTexture(GLTexture * glTexture)
{
	GLTexture * glTextureOutput = nullptr;
	if (glTexture != nullptr)
	{
		if (!IsSupportOpenCL() && IsOpenGLDecoding())
		{
            printf("DisplayTexture openGLDecoding \n");
            
			float zoomRatio = GetZoomRatio();
			int filterInterpolation = 0;
			int widthOutput = 0;
			int heightOutput = 0;
			int local_angle = (360 - angle) % 360;
			wxRect rc(0, 0, 0, 0);
			CalculPositionVideo(widthOutput, heightOutput, rc);
			glTextureOutput = new GLTexture(widthOutput, heightOutput);

			CRegardsConfigParam * regardsParam = CParamInit::getInstance();
			if (regardsParam != nullptr)
				filterInterpolation = regardsParam->GetInterpolationType();

			renderBitmapOpenGL->RenderWithEffectInterpolation(glTexture, glTextureOutput, rc, &videoEffectParameter, flipH, !flipV, local_angle, filterInterpolation, true);
		}
		else
		{
			printf("DisplayTexture not openGLDecoding \n");
			muVideoEffect.lock();
			wxFloatRect floatRect;
			floatRect.left = 0;
			floatRect.right = 1.0f;
			floatRect.top = 0;
			floatRect.bottom = 1.0f;
			renderBitmapOpenGL->RenderWithEffect(glTexture, &videoEffectParameter, floatRect, inverted);
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
		aspect_ratio = (float)GetSrcBitmapWidth() / (float)GetSrcBitmapHeight();

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


void CVideoControlSoft::CalculRectPictureInterpolation(wxRect &rc, int &widthInterpolationSize, int &heightInterpolationSize, int &left, int &top, const bool &invert)
{
   TRACE();
#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif 
   
	int widthOutput = int(GetBitmapWidth()) * scale_factor;
	int heightOutput = int(GetBitmapHeight()) * scale_factor;
	int xValue = 0;
	int yValue = 0;


	if (widthOutput > GetWidth()* scale_factor)
	{
		left = 0;
		xValue = posLargeur * scale_factor;
	}
	else
	{
		xValue = 0;
		left = (GetWidth()* scale_factor - widthOutput) / 2;
	}

	widthInterpolationSize = GetWidth()* scale_factor - (left * 2);


	if (heightOutput > GetHeight()* scale_factor)
	{
		top = 0;
		yValue = posHauteur* scale_factor;
	}
	else
	{
		yValue = 0;
		top = (GetHeight()* scale_factor - heightOutput) / 2;
	}

	heightInterpolationSize = GetHeight()* scale_factor - (top * 2);

	rc.x = max(xValue,0);
	if (invert)
    {
        int heightmax = heightOutput - (GetHeight() * scale_factor) - yValue;
        rc.y = max(heightmax, 0);
    }
	else
		rc.y = max(yValue,0);
	rc.width = widthOutput;
	rc.height = heightOutput;
}

void CVideoControlSoft::CalculPositionVideo(int & widthOutput, int & heightOutput, wxRect & rc)
{
#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif 
    
	widthOutput = int(GetBitmapWidth());
	heightOutput = int(GetBitmapHeight());

	int left = 0, top = 0;
	int tailleAffichageWidth = widthOutput;
	int tailleAffichageHeight = heightOutput;

    if (GetWidth() * scale_factor > tailleAffichageWidth)
        left = ((GetWidth() * scale_factor - tailleAffichageWidth) / 2);
    else
        left = 0;

    if (GetHeight() * scale_factor > tailleAffichageHeight)
        top = ((GetHeight() * scale_factor - tailleAffichageHeight) / 2);
    else
        top = 0;

	//wxRect rc(0, 0, 0, 0);
	CalculRectPictureInterpolation(rc, widthOutput, heightOutput, left, top, true);
}

void CVideoControlSoft::GetDenoiserPt(const int &width, const int &height)
{
	if (hq3d == nullptr)
	{
		hq3d = new Chqdn3d(width, height, videoEffectParameter.denoisingLevel);
	}
	else if (oldLevelDenoise != videoEffectParameter.denoisingLevel || width != oldwidthDenoise || height != oldheightDenoise)
	{
		delete hq3d;
		hq3d = new Chqdn3d(width, height, videoEffectParameter.denoisingLevel);
	}

	oldLevelDenoise = videoEffectParameter.denoisingLevel;
	oldwidthDenoise = width;
	oldheightDenoise = height;

}

GLTexture * CVideoControlSoft::RenderToTexture(COpenCLEffectVideo * openclEffect)
{
    printf("RenderToTexture 1\n");
    
#ifndef WIN32
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
    
    if(openclEffect == nullptr)
        return nullptr;

	float zoomRatio = GetZoomRatio();

	GLTexture * glTexture = nullptr;
	wxRect rect;
	int filterInterpolation = 0;
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();

    printf("RenderToTexture 2\n");

	if (regardsParam != nullptr)
		filterInterpolation = regardsParam->GetInterpolationType();

	if (videoEffectParameter.stabilizeVideo && videoEffectParameter.effectEnable && pictureFrame != nullptr)
		openclEffect->LoadRegardsBitmap(pictureFrame);
	else
		openclEffect->TranscodePicture(widthVideo, heightVideo);

	int widthOutput = 0;
	int heightOutput = 0;
	wxRect rc(0, 0, 0, 0);
	CalculPositionVideo(widthOutput, heightOutput, rc);
	openclEffect->InterpolationZoomBicubic(widthOutput, heightOutput, rc, flipH, flipV, angle, filterInterpolation);

	//Test if denoising Effect
	if (videoEffectParameter.denoiseEnable && videoEffectParameter.effectEnable)
	{
		GetDenoiserPt(widthOutput, heightOutput);
		openclEffect->HQDn3D(hq3d, videoEffectParameter.denoisingLevel);
	}

	if (videoEffectParameter.autoConstrast && videoEffectParameter.effectEnable)
	{
		openclEffect->AutoContrast();
	}

	bool isOpenGLOpenCL = false;
    openGLDecoding = false;
    
	if (openclContext->IsSharedContextCompatible())
	{
        
        printf("RenderToTexture IsSharedContextCompatible 3\n");
		glTexture = renderBitmapOpenGL->GetDisplayTexture(widthOutput, heightOutput, openclContext->GetContext());

		if (glTexture != nullptr)
		{
            printf("RenderToTexture glTexture is not null 3\n");
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
        printf("RenderToTexture !isOpenGLOpenCL toto\n");
        
		glTexture = renderBitmapOpenGL->GetDisplayTexture(widthOutput, heightOutput);

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
    
    printf("RenderToTexture End\n");
	//inverted = true;
	return glTexture;
}

GLTexture * CVideoControlSoft::RenderFFmpegToTexture(CRegardsBitmap * pictureFrame)
{
#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif

	GLTexture * glTexture = new GLTexture(GetSrcBitmapWidth(), GetSrcBitmapHeight());

	int filterInterpolation = 0;
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
		filterInterpolation = regardsParam->GetInterpolationType();

	int widthOutput = 0;
	int heightOutput = 0;
	wxRect rc(0, 0, 0, 0);
	CalculPositionVideo(widthOutput, heightOutput, rc);
	inverted = false;
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOutput, heightOutput);
	if (angle == 90 || angle == 270)
	{
		ApplyInterpolationFilters(pictureFrame, bitmapOut, rc, !flipH, flipV, angle, filterInterpolation);
	}
	else
		ApplyInterpolationFilters(pictureFrame, bitmapOut, rc, flipH, !flipV, angle, filterInterpolation);

	//Test if denoising Effect
	if (videoEffectParameter.denoiseEnable && videoEffectParameter.effectEnable)
	{
		GetDenoiserPt(widthOutput, heightOutput);
		hq3d->ApplyDenoise3D(bitmapOut);
	}

	glTexture->Create(bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight(), bitmapOut->GetPtBitmap());
	delete bitmapOut;

	deleteTexture = true;
	//delete bitmap;
	return glTexture;
}

GLTexture * CVideoControlSoft::RenderFFmpegToTexture()
{
#ifndef WIN32
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif

	GLTexture * glTexture = new GLTexture(GetSrcBitmapWidth(), GetSrcBitmapHeight());
	if (!IsOpenGLDecoding())
	{
		int filterInterpolation = 0;
		CRegardsConfigParam * regardsParam = CParamInit::getInstance();
		if (regardsParam != nullptr)
			filterInterpolation = regardsParam->GetInterpolationType();

		int widthOutput = 0;
		int heightOutput = 0;
		wxRect rc(0, 0, 0, 0);
		CalculPositionVideo(widthOutput, heightOutput, rc);
        inverted = false;
		CRegardsBitmap * bitmapOut = new CRegardsBitmap(widthOutput, heightOutput);
		if(angle == 90 || angle == 270)
        {
            ApplyInterpolationFilters(pictureFrame, bitmapOut, rc, !flipH, flipV, angle, filterInterpolation);
        }
        else
            ApplyInterpolationFilters(pictureFrame, bitmapOut, rc, flipH, !flipV, angle, filterInterpolation);

		//Test if denoising Effect
		if (videoEffectParameter.denoiseEnable && videoEffectParameter.effectEnable)
		{
			GetDenoiserPt(widthOutput, heightOutput);
			hq3d->ApplyDenoise3D(bitmapOut);
		}

		if (videoEffectParameter.autoConstrast && videoEffectParameter.effectEnable)
		{
			COpenCVEffect::BrightnessAndContrastAuto(bitmapOut);
		}

		glTexture->Create(bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight(), bitmapOut->GetPtBitmap());
		delete bitmapOut;
	}
	else
    {
		glTexture->Create(pictureFrame->GetBitmapWidth(), pictureFrame->GetBitmapHeight(), pictureFrame->GetPtBitmap());
    }
		

	deleteTexture = true;
	//delete bitmap;
	return glTexture;
}
#endif

void CVideoControlSoft::Rotate90()
{
	angle += 90;
	angle = angle % 360;
	CalculPositionPicture(centerX, centerY);
	UpdateScrollBar();
}

void CVideoControlSoft::Rotate270()
{
	angle += 270;
	angle = angle % 360;
	CalculPositionPicture(centerX, centerY);
	UpdateScrollBar();
}
void CVideoControlSoft::FlipVertical()
{
	flipV = !flipV;
	if (pause)
		this->Refresh();
}

void CVideoControlSoft::FlipHorizontal()
{
	flipH = !flipH;
	if (pause)
		this->Refresh();
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

	if (!enableopenCL || isCPU || (src_frame->format != 0 && src_frame->format != 23))
	{
		GetBitmapRGBA(src_frame);
		muBitmap.lock();
		*pictureFrame = *bitmapData;
		muBitmap.unlock();
	}
	else
	{
		isffmpegDecode = false;
		if (openclEffectYUV != nullptr)
		{
			if (src_frame->format == 23)
			{
				//printf("OpenCL openclEffectYUV \n");
				int ysize = 0;
				int uvsize = 0;

				ysize = src_frame->linesize[0] * src_frame->height;
				uvsize = src_frame->linesize[1] * (src_frame->height / 2);
				muBitmap.lock();
				openclEffectYUV->SetMemoryDataNV12(src_frame->data[0], ysize, src_frame->data[1], uvsize, src_frame->width, src_frame->height, src_frame->linesize[0]);
				muBitmap.unlock();
			}
			else if (src_frame->format == 0)
			{
				//printf("OpenCL openclEffectYUV \n");
				int ysize = 0;
				int usize = 0;
				int vsize = 0;

				ysize = src_frame->linesize[0] * src_frame->height;
				usize = src_frame->linesize[1] * (src_frame->height / 2);
				vsize = src_frame->linesize[2] * (src_frame->height / 2);
				muBitmap.lock();
				openclEffectYUV->SetMemoryData(src_frame->data[0], ysize, src_frame->data[1], usize, src_frame->data[2], vsize, src_frame->width, src_frame->height, src_frame->linesize[0]);
				muBitmap.unlock();
			}
			
		}
	}
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
		printf("VideoControl Is use_opencl 1\n");
		if (openclEffectYUV != nullptr && openclEffectYUV->IsOk())
		{
			printf("VideoControl Is use_opencl 2\n");
			muBitmap.lock();
			glTexture = RenderToTexture(openclEffectYUV);
			muBitmap.unlock();
		}
		printf("VideoControl Is use_opencl 3\n");

		deleteTexture = false;
	}
	else
	{
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
