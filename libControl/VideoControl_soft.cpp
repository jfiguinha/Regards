// ReSharper disable All
#include "header.h"
#include "VideoControl_soft.h"
#include <wx/dcbuffer.h>

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
#include <opencv2/xphoto.hpp>
#include <picture_utility.h>
#include "ScrollbarWnd.h"
#include "ClosedHandCursor.h"
#include <ConvertUtility.h>
#include <videothumb.h>
#include <hqdn3d.h>
#include <SqlPhotos.h>
#include <RegardsConfigParam.h>
#include <MediaInfo.h>
#include <VideoStabilization.h>
#include <FiltreEffetCPU.h>
#include <aspectratio.h>
using namespace Regards::OpenCV;
using namespace Regards::Sqlite;
//#include "LoadingResource.h"

#define TIMER_FPS 0x10001
#define TIMER_PLAYSTART 0x10002
#define TIMER_PLAYSTOP 0x10003

extern bool firstElementToShow;
AVFrame* copyFrameBuffer = nullptr;

class CDataAVFrame
{
public:


	CDataAVFrame()
	{
	}

	~CDataAVFrame()
	{
	}
	float sample_aspect_ratio = 0.0;
	cv::Mat matFrame;
	int width;
	int height;


};

extern float clamp(float val, float minval, float maxval);

class DataTimeDuration
{
public:
	int64_t duration;
	int64_t startTime;
};

CVideoControlSoft::CVideoControlSoft(CWindowMain* windowMain, wxWindow* window, IVideoInterface* eventPlayer)
{
	renderBitmapOpenGL = nullptr;
	this->window = window;
	hq3d = nullptr; // new Chqdn3d(videoEffectParameter.denoisingLevel);
	widthVideo = 0;
	heightVideo = 0;
	subtilteUpdate = false;
	startingTime = 0;
	old_width = 0;
	old_height = 0;

	pause = false;
	config = nullptr;
	angle = 0;
	flipV = false;
	newVideo = true;
	flipH = false;
	videoEnd = true;
	exit = false;
	quitWindow = false;
	videoStart = false;
	videoRenderStart = false;
	video_aspect_ratio = 0.0;
	config = CParamInit::getInstance();
	pause = false;
	this->windowMain = windowMain;
	this->eventPlayer = eventPlayer;

	openclEffectYUV = nullptr;
	hCursorHand = CResourceCursor::GetClosedHand();
    
}

vector<int> CVideoControlSoft::GetListTimer()
{
	vector<int> list;
	list.push_back(TIMER_PLAYSTOP);
	list.push_back(TIMER_FPS);
	list.push_back(TIMER_PLAYSTART);
	return list;
}

vector<int> CVideoControlSoft::GetListCommand()
{
	vector<int> list;
	list.push_back(wxEVENT_UPDATEPOSMOVIETIME);
	list.push_back(wxEVENT_SCROLLMOVE);
	list.push_back(wxEVENT_ENDVIDEOTHREAD);
	list.push_back(wxEVENT_STOPVIDEO);
	list.push_back(EVENT_VIDEOSTART);
	list.push_back(wxEVENT_LEFTPOSITION);
	list.push_back(wxEVENT_TOPPOSITION);
	list.push_back(wxEVENT_SETPOSITION);
	list.push_back(EVENT_VIDEOROTATION);
	list.push_back(wxEVENT_UPDATEMOVIETIME);
	list.push_back(wxEVENT_UPDATEFRAME);
	list.push_back(wxEVENT_PAUSEMOVIE);
	return list;
}

int CVideoControlSoft::UpdateResized()
{
	Resize();
	return 0;
}

void CVideoControlSoft::OnTimer(wxTimerEvent& event)
{
	switch (event.GetId())
	{
	case TIMER_PLAYSTOP:
		OnPlayStop(event);
		break;
	case TIMER_FPS:
		OnShowFPS(event);
		break;
	case TIMER_PLAYSTART:
		OnPlayStart(event);
		break;
	}
}

void CVideoControlSoft::OnCommand(wxCommandEvent& event)
{
	switch (event.GetEventType())
	{
	case wxEVENT_UPDATEPOSMOVIETIME:
		OnSetPos(event);
		break;
	case wxEVENT_UPDATEMOVIETIME:
		OnVideoDuration(event);
		break;
	case wxEVENT_SCROLLMOVE:
		OnScrollMove(event);
		break;
	case wxEVENT_ENDVIDEOTHREAD:
		EndVideoThread(event);
		break;
	case wxEVENT_STOPVIDEO:
		StopVideoThread(event);
		break;
	case EVENT_VIDEOSTART:
		VideoStart(event);
		break;
	case wxEVENT_LEFTPOSITION:
		OnLeftPosition(event);
		break;
	case wxEVENT_TOPPOSITION:
		OnTopPosition(event);
		break;
	case wxEVENT_SETPOSITION:
		OnSetPosition(event);
		break;
	case EVENT_VIDEOROTATION:
		VideoRotation(event);
		break;
	case wxEVENT_UPDATEFRAME:
		OnSetData(event);
		break;

	case wxEVENT_PAUSEMOVIE:
		OnPauseMovie(event);
		break;
	}
}

void CVideoControlSoft::OnPauseMovie(wxCommandEvent& event)
{
	OnPause();
}

void CVideoControlSoft::SetParent(wxWindow* parent)
{
	parentRender = parent;

	fpsTimer = new wxTimer(parentRender, TIMER_FPS);
	playStartTimer = new wxTimer(parentRender, TIMER_PLAYSTART);
	playStopTimer = new wxTimer(parentRender, TIMER_PLAYSTOP);
	ffmfc = new CFFmfc(parentRender, wxID_ANY);
}



void CVideoControlSoft::DiaporamaMode(const bool& value)
{
	isDiaporama = value;
}

void CVideoControlSoft::RepeatVideo()
{
	if (repeatVideo)
		repeatVideo = false;
	else
		repeatVideo = true;
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
	muVideoEffect.lock();
	const float ratioSelect = videoEffectParameter.tabRatio[videoEffectParameter.ratioSelect];
	muVideoEffect.unlock();
	return ratioSelect;
}

cv::Mat CVideoControlSoft::SavePicture(bool& isFromBuffer)
{
	cv::Mat bitmap;
	if (!isffmpegDecode)
	{
		muBitmap.lock();
		if (openclEffectYUV != nullptr && openclEffectYUV->IsOk())
		{
			bitmap = openclEffectYUV->GetMatrix(true);
		}
		muBitmap.unlock();
	}
	else
	{
		muBitmap.lock();
		pictureFrame.copyTo(bitmap);
		muBitmap.unlock();
	}


	if (!bitmap.empty())
	{
		CPictureUtility::ApplyRotation(bitmap, angle);
	}
	return bitmap;
}

bool CVideoControlSoft::IsFFmpegDecode()
{
	return isffmpegDecode;
}

//-----------------------------------------------------------------
//Gestion du click de souris
//-----------------------------------------------------------------
void CVideoControlSoft::OnLButtonDown(wxMouseEvent& event)
{
	parentRender->SetFocus();
	int xPos = event.GetX();
	int yPos = event.GetY();
	mouseBlock = true;
	mouseScrollX = xPos;
	mouseScrollY = yPos;
	parentRender->CaptureMouse();
	wxSetCursor(hCursorHand);
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CVideoControlSoft::OnLButtonUp(wxMouseEvent& event)
{
	if (event.LeftDClick())
		return;

	int xPos = event.GetX();
	int yPos = event.GetY();

	mouseBlock = false;
	mouseScrollX = xPos;
	mouseScrollY = yPos;
	if (parentRender->HasCapture())
		parentRender->ReleaseMouse();
}

//------------------------------------------------------------------------------------
//Déplacement de la souris
//------------------------------------------------------------------------------------
void CVideoControlSoft::OnMouseMove(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();

	wxSetCursor(hCursorHand);

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
	float screenWidth = static_cast<float>(parentRender->GetSize().GetWidth());
	float posLargeurMax = bitmapRatioWidth - screenWidth;


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
	float screenHeight = static_cast<float>(parentRender->GetSize().GetHeight());
	float posHauteurMax = bitmapRatioHeight - screenHeight;


	int yValue = posHauteurMax;

	if (posHauteur >= yValue)
		posHauteur = yValue;

	if (posHauteur < 0)
		posHauteur = 0;
}


void CVideoControlSoft::OnSetPosition(wxCommandEvent& event)
{
	long pos = event.GetExtraLong();
	if (videoPosition == pos)
		return;
	if (stopVideo)
		OnPlay();
	videoPosition = pos;

	int64_t timeToSeek = videoPosition * 1000 * 1000;

	if (openCVStabilization != nullptr)
		openCVStabilization->Init();

	ffmfc->SetTimePosition(timeToSeek);

}

void CVideoControlSoft::OnLeftPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posLargeur = pos;
	muRefresh.lock();
	needToRefresh = true;
	muRefresh.unlock();
}

void CVideoControlSoft::OnTopPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
	muRefresh.lock();
	needToRefresh = true;
	muRefresh.unlock();
}

void CVideoControlSoft::OnScrollMove(wxCommandEvent& event)
{
	//int isMoving = event.GetInt();
	//printf("Is moving !!");
}

void CVideoControlSoft::OnKeyUp(wxKeyEvent& event)
{
	switch (event.GetKeyCode())
	{
	case WXK_CONTROL:
		controlKeyPush = false;
		break;
	default: ;
	}
}

void CVideoControlSoft::MoveTop()
{
	wxWindow* parent = parentRender->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVT_NULL, wxEVENT_MOVETOP);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CVideoControlSoft::MoveLeft()
{
	wxWindow* parent = parentRender->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVELEFT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CVideoControlSoft::MoveBottom()
{
	wxWindow* parent = parentRender->GetParent();

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
	for (int i = 0; i < videoEffectParameter.tabZoom.size(); i++)
		listValue.push_back(videoEffectParameter.tabZoom[i] * 100.0f);
	muVideoEffect.unlock();
	return listValue;
}

int CVideoControlSoft::GetZoomIndex()
{
	if (shrinkVideo)
	{
		CalculRatio(GetBitmapWidth(), GetBitmapHeight());
	}

	muVideoEffect.lock();
	const int zoomIndex = videoEffectParameter.zoomSelect;
	muVideoEffect.unlock();
	return zoomIndex;
}

void CVideoControlSoft::ChangeVideoFormat()
{
	//int zoomSelect = 0;
	muVideoEffect.lock();
	videoEffectParameter.ratioSelect++;
	if (videoEffectParameter.ratioSelect >= videoEffectParameter.tabRatio.size())
		videoEffectParameter.ratioSelect = 0;

	muVideoEffect.unlock();

	muRefresh.lock();
	needToRefresh = true;
	muRefresh.unlock();
}


float CVideoControlSoft::GetZoomRatio()
{
	float zoom;
	if (shrinkVideo)
	{
		zoom = CPictureUtility::CalculPictureRatio(GetSrcBitmapWidth(), GetSrcBitmapHeight(),
		                                           parentRender->GetSize().GetWidth(),
		                                           parentRender->GetSize().GetHeight());
	}
	else
	{
		muVideoEffect.lock();
		zoom = videoEffectParameter.tabZoom[videoEffectParameter.zoomSelect];
		muVideoEffect.unlock();
	}
	return zoom;
}

float CVideoControlSoft::CalculRatio(const int& pictureWidth, const int& pictureHeight)
{
	float newRatio = CPictureUtility::CalculPictureRatio(GetSrcBitmapWidth(), GetSrcBitmapHeight(),
	                                                     parentRender->GetSize().GetWidth(),
	                                                     parentRender->GetSize().GetHeight());
	int zoomSelect = 0;
	//Détermination du ration par rapport au tableau
	//printf("Ratio %f \n", newRatio);
	muVideoEffect.lock();

	//Calcul Zoom Index
	if (newRatio != 0.0)
	{
		for (int i = 0; i < videoEffectParameter.tabZoom.size(); i++)
		{
			//printf("Ratio %f \n", videoEffectParameter.tabZoom[i]);
			if (newRatio < videoEffectParameter.tabZoom[i])
			{
				//ratio = videoEffectParameter.tabZoom[i];
				zoomSelect = i;
				break;
			}
		}
	}

	//printf("Ratio index %d \n", zoomSelect);

	videoEffectParameter.zoomSelect = zoomSelect;

	muVideoEffect.unlock();

	return newRatio;
}

void CVideoControlSoft::ShrinkVideo()
{
	CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	//Calcul position largeur et hauteur initial
	posLargeur = 0;
	posHauteur = 0;
	centerX = static_cast<float>(GetBitmapWidth()) / 2.0f;
	centerY = static_cast<float>(GetBitmapHeight()) / 2.0f;

	UpdateScrollBar();

	shrinkVideo = true;
}

void CVideoControlSoft::CalculTextureSize(int& widthOut, int& heightOut)
{
	int width_local = GetSrcBitmapWidth();
	int height_local = GetSrcBitmapHeight();
	float zoom = GetZoomRatio();
	widthOut = width_local * zoom;
	heightOut = height_local * zoom;
}

void CVideoControlSoft::UpdateScrollBar()
{
	int widthOut = 0;
	int heightOut = 0;
	wxWindow* parent = parentRender->GetParent();
	CalculTextureSize(widthOut, heightOut);
	if (parent != nullptr)
	{
		auto controlSize = new CControlSize();
		wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
		controlSize->controlWidth = widthOut;
		controlSize->controlHeight = heightOut;
		evt.SetClientData(controlSize);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}

	if (parent != nullptr)
	{
		auto size = new wxSize();
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		size->x = posLargeur;
		size->y = posHauteur;
		evt.SetClientData(size);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
	muRefresh.lock();
	needToRefresh = true;
	muRefresh.unlock();
}

void CVideoControlSoft::MoveRight()
{
	wxWindow* parent = parentRender->GetParent();

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
	default: ;
	}
}

void CVideoControlSoft::SetRotation(const int& rotation)
{
	
	wxCommandEvent event(EVENT_VIDEOROTATION);
	event.SetExtraLong(rotation);
	wxPostEvent(parentRender, event);
	
	/*
	if (rotation == 90)
		angle = 270;
	else if (rotation == -90)
		angle = 270;
	else if (rotation == -180)
		angle = 180;
	else if (rotation == 180)
		angle = 180;
	else if (rotation == -270)
		angle = 90;
	else if (rotation == 270)
		angle = 90;

	CSqlPhotos sqlPhotos;
	int exif = sqlPhotos.GetPhotoExif(filename);
	if (exif != -1)
	{
		int _flipH = 0;
		int _flipV = 0;
		CSqlPhotos::GetAngleAndFlip(exif, angle, _flipH, _flipV);
		if (_flipH)
			flipH = true;
		if (_flipV)
			flipV = true;
	}
	*/
}

void CVideoControlSoft::VideoRotation(wxCommandEvent& event)
{
	long rotation = event.GetExtraLong();
	if (rotation == 90)
		angle = 270;
	else if (rotation == -90)
		angle = 270;
	else if (rotation == -180)
		angle = 180;
	else if (rotation == 180)
		angle = 180;
	else if (rotation == -270)
		angle = 90;
	else if (rotation == 270)
		angle = 90;

	CSqlPhotos sqlPhotos;
	int exif = sqlPhotos.GetPhotoExif(filename);
	if (exif != -1)
	{
		int _flipH = 0;
		int _flipV = 0;
		CSqlPhotos::GetAngleAndFlip(exif, angle, _flipH, _flipV);
		if (_flipH)
			flipH = true;
		if (_flipV)
			flipV = true;
	}

	parentRender->Refresh();
}

void CVideoControlSoft::UpdateFiltre(CEffectParameter* effectParameter)
{
	bool updateScroll = false;
	auto videoParameter = static_cast<CVideoEffectParameter*>(effectParameter);
	if (videoParameter->streamAudioUpdate)
	{
		ChangeAudioStream(videoParameter->streamAudioIndex);
		videoParameter->streamAudioUpdate = 0;
	}
	else if (videoParameter->streamVideoUpdate)
	{
		//ChangeAudioStream(videoParameter->streamAudioIndex);
		videoParameter->streamVideoUpdate = 0;
	}
	else if (videoParameter->streamSubtitleUpdate)
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
		{
			muRefresh.lock();
			needToRefresh = true;
			muRefresh.unlock();
		}
	}
	//Refresh();
}


bool CVideoControlSoft::GetPausedValue()
{
	return pause;
};

void CVideoControlSoft::RedrawFrame()
{
	muRefresh.lock();
	needToRefresh = true;
	muRefresh.unlock();
}

void CVideoControlSoft::SetVideoPreviewEffect(CEffectParameter* effectParameter)
{
	auto videoParameter = static_cast<CVideoEffectParameter*>(effectParameter);
	muVideoEffect.lock();
	videoEffectParameter = *videoParameter;
	muVideoEffect.unlock();
}

CEffectParameter* CVideoControlSoft::GetParameter()
{
	auto videoParameter = new CVideoEffectParameter();
	muVideoEffect.lock();
	*videoParameter = videoEffectParameter;
	muVideoEffect.unlock();
	return videoParameter;
}


bool CVideoControlSoft::GetProcessEnd()
{
	if (!videoEnd)
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
	//
	if (endProgram && videoRenderStart && !quitWindow)
	{
		fpsTimer->Stop();
		quitWindow = true;
		exit = true;
		if (!videoEnd)
		{
			if (ffmfc->Quit())
			{
				wxCommandEvent localevent;
				EndVideoThread(localevent);
				//videoEnd = true;
			}
		}
	}

	bool localRefresh = false;
	muRefresh.lock();
	localRefresh = needToRefresh;
	needToRefresh = false;

	muRefresh.unlock();
	if (localRefresh)
	{
		parentRender->Refresh();
	}
}


void CVideoControlSoft::SetEndProgram(const bool& endProgram)
{
	this->endProgram = endProgram;
}

void CVideoControlSoft::OnShowFPS(wxTimerEvent& event)
{
	msgFrame = wxString::Format("FPS : %d", nbFrame);
	nbFrame = 0;
}

void CVideoControlSoft::OnPlayStart(wxTimerEvent& event)
{
    wxString hardwareDecoder = "";
    CRegardsConfigParam* config = CParamInit::getInstance();
    if (config != nullptr)
        hardwareDecoder = config->GetHardwareDecoder();
    
	ffmfc->SetFile(this, filename,
                   isHardwareDecoder ? hardwareDecoder : "none", isOpenGLDecoding, GetSoundVolume());
}

wxString CVideoControlSoft::GetAcceleratorHardware()
{
    wxString hardwareDecoder = "";
    CRegardsConfigParam* config = CParamInit::getInstance();
    if (config != nullptr)
        hardwareDecoder = config->GetHardwareDecoder();
    
    return isHardwareDecoder ? hardwareDecoder : "none";
}

void CVideoControlSoft::EndVideoThread(wxCommandEvent& event)
{
	if (!endProgram)
	{
		videoEnd = true;
		//if (videoRenderStart)
		//{
		if (eventPlayer != nullptr)
		{
			eventPlayer->OnPositionVideo(0);
			eventPlayer->OnVideoEnd();
		}
		fpsTimer->Stop();
		videoRenderStart = false;
		stopVideo = true;
		//}
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
			eventPlayer->OnVideoStop();
		}
		fpsTimer->Stop();
		videoRenderStart = false;
		stopVideo = true;


		if (repeatVideo && !endProgram && !isDiaporama && filename == ffmfc->Getfilename())
		{
			PlayMovie(filename, true);
		}
        
        if(startVideoAfterProblem)
        {
            PlayMovie(filename, true);
            startVideoAfterProblem = false;
        }
	}
}


CVideoControlSoft::~CVideoControlSoft()
{
    /*
	if (_threadVideo != nullptr)
	{
		_threadVideo->join();
		delete _threadVideo;
	}
    */

	if (playStartTimer->IsRunning())
		playStartTimer->Stop();

	if (playStopTimer->IsRunning())
		playStopTimer->Stop();

	if (hq3d != nullptr)
		delete hq3d;

	if (openCVStabilization != nullptr)
		delete openCVStabilization;

	delete playStartTimer;
	delete fpsTimer;

	if (renderBitmapOpenGL != nullptr)
	{
		delete renderBitmapOpenGL;
	}

	if (openclEffectYUV != nullptr)
		delete openclEffectYUV;

	if (ffmfc)
		delete ffmfc;

	//if (thumbnailVideo != nullptr)
	//	delete thumbnailVideo;

	if (localContext != nullptr)
		sws_freeContext(localContext);
	localContext = nullptr;
}

void CVideoControlSoft::SetSubtitulePicture(cv::Mat& picture)
{
	muSubtitle.lock();
	picture.copyTo(pictureSubtitle);
	subtilteUpdate = true;

	muSubtitle.unlock();
}

void CVideoControlSoft::DeleteSubtitulePicture()
{
	muSubtitle.lock();

	subtilteUpdate = true;
	pictureSubtitle.release();
	muSubtitle.unlock();
}

bool CVideoControlSoft::IsHardwareCompatible()
{
	return isHardwareDecoder;
}

void CVideoControlSoft::PlayFirstMovie(const bool& firstMovie)
{
	this->firstMovie = firstMovie;
}

void CVideoControlSoft::SetOpenCLOpenGLInterop(const bool& openclOpenGLInterop)
{
	this->openclOpenGLInterop = openclOpenGLInterop;
}

int CVideoControlSoft::PlayMovie(const wxString& movie, const bool& play)
{
    
    if(movie != filename)
    {
        isHardwareDecoder = true;
    }
    return Play(movie);
}

int CVideoControlSoft::Play(const wxString& movie)
{
    if (videoEnd || stopVideo)
    {
        //if (thumbnailVideo != nullptr)
        //    delete thumbnailVideo;

        if (localContext != nullptr)
            sws_freeContext(localContext);
        localContext = nullptr;

        //thumbnailVideo = new CThumbnailVideo(movie, true);

        if (openCVStabilization != nullptr)
            delete openCVStabilization;

        openCVStabilization = nullptr;

        if (playStopTimer->IsRunning())
            playStopTimer->Stop();

        if (playStartTimer->IsRunning())
            playStartTimer->Stop();
        
        /*
        CVideoThumb videoThumb(movie, true);
        AspectRatio aspectRatio = videoThumb.GetAspectRatio();
        
            
        printf("video_aspect_ratio %d %d \n",aspectRatio.num, aspectRatio.den);
        /*
        for (int i = 0; i < videoEffectParameter.tabZoom.size(); i++)
        {
             printf("video_aspect_ratio %f \n",videoEffectParameter.tabZoom[i]);
            if(video_aspect_ratio < videoEffectParameter.tabZoom[i])
            {
                videoEffectParameter.zoomSelect = i - 1;
                break;
            }
        }
        */
        startVideo = true;
        stopVideo = false;
        videoStartRender = false;
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
        firstMovie = true;

        colorRange = CMediaInfo::GetColorRange(movie);
        colorSpace = CMediaInfo::GetColorSpace(movie);

        muVideoEffect.lock();
        videoEffectParameter.ratioSelect = 0;
        muVideoEffect.unlock();

        firstMovie = false;
        parentRender->Refresh();
    }
    else if (movie != filename)
    {
        OnStop(movie);
    }
    return 0;
}


void CVideoControlSoft::VideoStart(wxCommandEvent& event)
{
	if(eventPlayer != nullptr)
		eventPlayer->OnVideoStart();
	if (startVideo)
	{
		ffmfc->Play();
        
		pause = false;
		videoEnd = false;
		videoStart = true;
		fpsTimer->Start(1000);
		ShrinkVideo();

		wxWindow* window = wxWindow::FindWindowById(PREVIEWVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_HIDESAVEBUTTON);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
	}
}

int CVideoControlSoft::getWidth()
{
	return parentRender->GetSize().x;
}

int CVideoControlSoft::getHeight()
{
	return parentRender->GetSize().y;
}

void CVideoControlSoft::UpdateScreenRatio()
{
	muRefresh.lock();
	needToRefresh = true;
	muRefresh.unlock();
}

void CVideoControlSoft::ReloadResource()
{
	muRefresh.lock();
	needToRefresh = true;
	muRefresh.unlock();
}

void CVideoControlSoft::OnPaint2D(wxWindow* gdi)
{
}


void CVideoControlSoft::OnPaint3D(wxGLCanvas* canvas, CRenderOpenGL* renderOpenGL)
{
	if (renderBitmapOpenGL == nullptr)
	{
		this->renderOpenGL = renderOpenGL;
		renderBitmapOpenGL = new CRenderVideoOpenGL(renderOpenGL);
	}


	if (IsSupportOpenCL())
	{
		if (openclEffectYUV == nullptr)
		{
			openclEffectYUV = new COpenCLEffectVideo();
		}
	}

	isInit = true;
	inverted = true;

	// This is a dummy, to avoid an endless succession of paint messages.
	// OnPaint handlers must always create a wxPaintDC.
	//wxPaintDC dc(this);
	//printf("CVideoControlSoft::OnPaint \n");

#ifndef WIN32
	double scale_factor = parentRender->GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif

	std::clock_t start;
	start = std::clock();


	int width = parentRender->GetSize().GetWidth() * scale_factor;
	int height = parentRender->GetSize().GetHeight() * scale_factor;
	if (width == 0 || height == 0)
		return;

	if (quitWindow)
		return;

	nbFrame++;

	if (videoRenderStart && initStart)
	{
		//nbFrame = 0;
		if (!fpsTimer->IsRunning())
			fpsTimer->Start(1000);
	}
    
    
    renderOpenGL->CreateScreenRender(width, height, CRgbaquad(0, 0, 0, 0));

	if (videoRenderStart)
	{
        
        
		if (!pictureFrame.empty() && !IsSupportOpenCL())
		{
			muBitmap.lock();
			RenderFFmpegToTexture(pictureFrame);
			muBitmap.unlock();
		}
		else
			RenderToGLTexture();

		//printf("DisplayTexture not openGLDecoding \n");
		muVideoEffect.lock();
		wxFloatRect floatRect;
		floatRect.left = 0;
		floatRect.right = 1.0f;
		floatRect.top = 0;
		floatRect.bottom = 1.0f;
		renderBitmapOpenGL->RenderWithEffect(&videoEffectParameter, floatRect, videoPosition / 100,	inverted);
		muVideoEffect.unlock();

		muVideoEffect.lock();
		if (videoEffectParameter.showFPS)
		{
			renderOpenGL->Print(0, 1, CConvertUtility::ConvertToUTF8(msgFrame));
		}
		muVideoEffect.unlock();

		muSubtitle.lock();

		if (subtilteUpdate && !pictureSubtitle.empty())
		{
			renderBitmapOpenGL->SetSubtitle(pictureSubtitle);
			subtilteUpdate = false;
		}
		else if (subtilteUpdate)
		{
			//renderBitmapOpenGL->DeleteSubtitle();
			subtilteUpdate = false;
		}

		muSubtitle.unlock();

		if (videoEffectParameter.enableSubtitle)
		{
			renderBitmapOpenGL->ShowSubtitle();
		}

	}


	canvas->SwapBuffers();

	if (!videoStartRender)
    {
        if(firstElementToShow)
            playStartTimer->Start(1000, true);
        else
            playStartTimer->Start(100, true);
    }
		
	videoStartRender = true;

}

int CVideoControlSoft::ChangeSubtitleStream(int newStreamSubtitle)
{
	ffmfc->Change_subtitle_stream(newStreamSubtitle);
	//SetVideoPosition(videoPosition / 1000);
	wxCommandEvent evt(wxEVENT_SETPOSITION);
	evt.SetExtraLong(videoPosition / 1000);
	parentRender->GetEventHandler()->AddPendingEvent(evt);
	return 0;
}


int CVideoControlSoft::ChangeAudioStream(int newStreamAudio)
{
	ffmfc->Change_audio_stream(newStreamAudio);
	//SetVideoPosition(videoPosition / 1000);
	wxCommandEvent evt(wxEVENT_SETPOSITION);
	evt.SetExtraLong(videoPosition / 1000);
	parentRender->GetEventHandler()->AddPendingEvent(evt);
	return 0;
}


void CVideoControlSoft::ErrorDecodingFrame()
{
    if(!startVideoAfterProblem)
    {
        isHardwareDecoder = false;
        startVideoAfterProblem = true;
        ffmfc->Quit(); 
    }

    //Play(filename);
    //wxCommandEvent evt(wxEVENT_STOPVIDEO);
    //parentRender->GetEventHandler()->AddPendingEvent(evt);
}


void CVideoControlSoft::OnPlay()
{
	if (videoStart)
	{
		bool _videoEnd = videoEnd;
		if (!_videoEnd)
			if (stopVideo)
				_videoEnd = true;

		if (pause && !_videoEnd)
		{
			ffmfc->Pause();
			wxWindow* window = wxWindow::FindWindowById(PREVIEWVIEWERID);
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
		PlayMovie(filename, false);
	}

	pause = false;
}

void CVideoControlSoft::OnStop(wxString photoName)
{

	standByMovie = photoName;

	QuitMovie();
}


void CVideoControlSoft::QuitMovie()
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


	wxWindow* window = wxWindow::FindWindowById(PREVIEWVIEWERID);
	if (window != nullptr)
	{
		wxCommandEvent evt(wxEVENT_HIDESAVEBUTTON);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
}


float CVideoControlSoft::GetLargeurMax()
{
	float bitmapRatioWidth = GetBitmapWidth();
	float screenWidth = static_cast<float>(parentRender->GetSize().GetWidth());
	return bitmapRatioWidth - screenWidth;
}

float CVideoControlSoft::GetHauteurMax()
{
	float bitmapRatioHeight = GetBitmapHeight();
	float screenHeight = static_cast<float>(parentRender->GetSize().GetHeight());
	return bitmapRatioHeight - screenHeight;
}

void CVideoControlSoft::CalculPositionPicture(const float& x, const float& y)
{
	float bitmapRatioWidth = GetBitmapWidth();
	float bitmapRatioHeight = GetBitmapHeight();
	float screenWidth = static_cast<float>(parentRender->GetSize().GetWidth());
	float screenHeight = static_cast<float>(parentRender->GetSize().GetHeight());

	float posLargeurMax = bitmapRatioWidth - screenWidth;
	float posHauteurMax = bitmapRatioHeight - screenHeight;

	//float ratio = GetZoomRatio();

	//float middleScreenWidth = screenWidth / 2.0f;
	//float middleScreenHeight = screenHeight / 2.0f;

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
	//int localAngle = angle;
	int widthOut = 0;
	int heightOut = 0;
	CalculTextureSize(widthOut, heightOut);
	return widthOut;
}

int CVideoControlSoft::GetBitmapHeight()
{
	//int localAngle = angle;
	int widthOut = 0;
	int heightOut = 0;
	CalculTextureSize(widthOut, heightOut);
	return heightOut;
}

int CVideoControlSoft::GetSrcBitmapWidth()
{
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
	return _widthVideo;
	return 0;
}

int CVideoControlSoft::GetSrcBitmapHeight()
{
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
	return _heightVideo;
	return 0;
}


//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CVideoControlSoft::MouseClick(const int& xPos, const int& yPos)
{
	mouseScrollX = xPos;
	mouseScrollY = yPos;
	mouseBlock = true;
	parentRender->CaptureMouse();
	parentRender->SetFocus();
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CVideoControlSoft::MouseRelease(const int& xPos, const int& yPos)
{
	mouseBlock = false;
	wxSetCursor(wxCursor(wxCURSOR_ARROW));
	if (parentRender->HasCapture())
		parentRender->ReleaseMouse();
}

void CVideoControlSoft::CalculCenterPicture()
{
	float bitmapRatioWidth = GetBitmapWidth();
	float bitmapRatioHeight = GetBitmapHeight();
	float screenWidth = static_cast<float>(parentRender->GetSize().GetWidth());
	float screenHeight = static_cast<float>(parentRender->GetSize().GetHeight());


	//float middleScreenWidth = screenWidth / 2.0f;
	//float middleScreenHeight = screenHeight / 2.0f;

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
			centerX = static_cast<float>(posLargeur) / posLargeurMax;
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
			centerY = static_cast<float>(posHauteur) / posHauteurMax;
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
			wxWindow* window = wxWindow::FindWindowById(PREVIEWVIEWERID);
			if (window != nullptr)
			{
				wxCommandEvent evt(wxEVENT_SHOWSAVEBUTTON);
				window->GetEventHandler()->AddPendingEvent(evt);
			}
		}
		pause = true;
	}
}

void CVideoControlSoft::OnVideoDuration(wxCommandEvent& event)
{
	DataTimeDuration * dtTime =  (DataTimeDuration *)event.GetClientData();
	if(dtTime != nullptr)
	{
		startingTime = dtTime->startTime;
		
		if (eventPlayer != nullptr)
			eventPlayer->SetVideoDuration(dtTime->duration);	
			
		delete dtTime;	
	}

}

void CVideoControlSoft::OnSetPos(wxCommandEvent& event)
{
	if (!videoEnd)
	{
		videoPosition = event.GetExtraLong();
		if (eventPlayer != nullptr)
			eventPlayer->OnPositionVideo(videoPosition);
	}
}


void CVideoControlSoft::SetPos(int64_t pos)
{
	int64_t videoPosition = (pos * 1000) - startingTime;
	videoPosition = videoPosition / 1000;
	wxCommandEvent evt(wxEVENT_UPDATEPOSMOVIETIME);
	evt.SetExtraLong(videoPosition);
	parentRender->GetEventHandler()->AddPendingEvent(evt);
}

void CVideoControlSoft::SetVideoDuration(const int64_t& duration, const int64_t& startTime)
{
	DataTimeDuration * dtTime = new DataTimeDuration();
	dtTime->duration = duration;
	dtTime->startTime = startTime;
	wxCommandEvent evt(wxEVENT_UPDATEMOVIETIME);
	evt.SetClientData(dtTime);
	parentRender->GetEventHandler()->AddPendingEvent(evt);
}


void CVideoControlSoft::SetCurrentclock(wxString message)
{
	this->message = message;
}


void CVideoControlSoft::SetVolume(const int& pos)
{
	ffmfc->SetVolume(pos);
	SetSoundVolume(pos);
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


cv::Mat CVideoControlSoft::GetBitmapRGBA(AVFrame* tmp_frame)
{
	cv::Mat bitmapData = cv::Mat(tmp_frame->height, tmp_frame->width, CV_8UC4);

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

	int numBytes = av_image_get_buffer_size(AV_PIX_FMT_BGRA, tmp_frame->width, tmp_frame->height, 16);
	uint8_t* convertedFrameBuffer = bitmapData.data;
	int linesize = tmp_frame->width * 4;

	sws_scale(localContext, tmp_frame->data, tmp_frame->linesize, 0, tmp_frame->height,
	          &convertedFrameBuffer, &linesize);


	//cv::flip(bitmapData, bitmapData, 0);
	return bitmapData;
}

void CVideoControlSoft::OnSetData(wxCommandEvent& event)
{

	CDataAVFrame* avFrameData = (CDataAVFrame *)event.GetClientData();
	if (avFrameData != nullptr)
	{
		//isffmpegDecode = true;
		//enableopenCL = 0;

		video_aspect_ratio = avFrameData->sample_aspect_ratio;
		widthVideo = avFrameData->width;
		heightVideo = avFrameData->height;
		ratioVideo = static_cast<float>(avFrameData->width) / static_cast<float>(avFrameData->height);

		muRefresh.lock();
		needToRefresh = false;
		muRefresh.unlock();
        
        /*
        printf("video_aspect_ratio %f \n",video_aspect_ratio);
        for (int i = 0; i < videoEffectParameter.tabZoom.size(); i++)
        {
             printf("video_aspect_ratio %f \n",videoEffectParameter.tabZoom[i]);
            if(video_aspect_ratio == videoEffectParameter.tabZoom[i])
            {
                videoEffectParameter.zoomSelect = i;
                break;
            }
        }
        */

		if (!isffmpegDecode)
		{
			//muBitmap.lock();
			openclEffectYUV->TransfertData();
			//muBitmap.unlock();
		}

		delete avFrameData;
		parentRender->Refresh();
	}


}


void CVideoControlSoft::SetData(void* data, const float& sample_aspect_ratio, void* dxva2Context)
{
	bool sendMessage = false;
	bool isCPU = true;
	if (IsSupportOpenCL())
		isCPU = IsCPUContext();

	CDataAVFrame* avFrameData = new CDataAVFrame();

	//printf("Set Data Begin \n");

	videoRenderStart = true;
	auto src_frame = static_cast<AVFrame*>(data);
	avFrameData->sample_aspect_ratio = sample_aspect_ratio;

	SetFrameData(src_frame);
	//avFrameData->matFrame = GetBitmapRGBA(src_frame);
	avFrameData->width = src_frame->width;
	avFrameData->height = src_frame->height;
	//ratioVideo = static_cast<float>(src_frame->width) / static_cast<float>(src_frame->height);


	wxCommandEvent event(wxEVENT_UPDATEFRAME);
	event.SetClientData(avFrameData);
	wxPostEvent(parentRender, event);
}

void CVideoControlSoft::Resize()
{
	float screenWidth = static_cast<float>(parentRender->GetSize().GetWidth());
	float screenHeight = static_cast<float>(parentRender->GetSize().GetHeight());

	if (!stopVideo)
	{
		updateContext = true;

		if (videoStart)
			ffmfc->VideoDisplaySize(parentRender->GetSize().GetWidth(), parentRender->GetSize().GetHeight());

		if (pause && isffmpegDecode && copyFrameBuffer != nullptr)
		{
			SetFrameData(copyFrameBuffer);
		}

		if (shrinkVideo)
		{
			ShrinkVideo();
		}
		else if (oldWidth != 0 && oldHeight != 0)
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

		muRefresh.lock();
		needToRefresh = true;
		muRefresh.unlock();
	}
	oldWidth = screenWidth;
	oldHeight = screenHeight;
}

void CVideoControlSoft::calculate_display_rect(wxRect* rect, int scr_xleft, int scr_ytop, int scr_width, int scr_height)
{
	float aspect_ratio = video_aspect_ratio;
	int width, height, x, y;
	//float ratio = 1.0f;
	float zoom = GetZoomRatio();

	muVideoEffect.lock();
	aspect_ratio = videoEffectParameter.tabRatio[videoEffectParameter.ratioSelect];
	muVideoEffect.unlock();

	if (aspect_ratio == 1.0)
		aspect_ratio = static_cast<float>(GetSrcBitmapWidth()) / static_cast<float>(GetSrcBitmapHeight());

	/* XXX: we suppose the screen has a 1.0 pixel ratio */
	height = scr_height * zoom;
	width = static_cast<int>(rint(height * aspect_ratio)) & ~1;
	if (width > scr_width)
	{
		width = scr_width;
		height = static_cast<int>(rint(width / aspect_ratio)) & ~1;
	}
	x = (scr_width - width) / 2;
	y = (scr_height - height) / 2;
	rect->x = scr_xleft + x;
	rect->y = scr_ytop + y;
	rect->width = FFMAX(width, 1);
	rect->height = FFMAX(height, 1);
}



void CVideoControlSoft::SetZoomIndex(const int& pos)
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


void CVideoControlSoft::CalculRectPictureInterpolation(wxRect& rc, int& widthInterpolationSize,
                                                       int& heightInterpolationSize, int& left, int& top,
                                                       const bool& invertY, const bool& invertX)
{
#ifndef WIN32
	double scale_factor = parentRender->GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif

	int widthOutput = static_cast<int>(GetBitmapWidth()) * scale_factor;
	int heightOutput = static_cast<int>(GetBitmapHeight()) * scale_factor;
	int xValue = 0;
	int yValue = 0;


	if (widthOutput > parentRender->GetSize().GetWidth() * scale_factor)
	{
		left = 0;
		xValue = posLargeur * scale_factor;
	}
	else
	{
		xValue = 0;
		left = (parentRender->GetSize().GetWidth() * scale_factor - widthOutput) / 2;
	}

	widthInterpolationSize = parentRender->GetSize().GetWidth() * scale_factor - (left * 2);


	if (heightOutput > parentRender->GetSize().GetHeight() * scale_factor)
	{
		top = 0;
		yValue = posHauteur * scale_factor;
	}
	else
	{
		yValue = 0;
		top = (parentRender->GetSize().GetHeight() * scale_factor - heightOutput) / 2;
	}

	heightInterpolationSize = parentRender->GetSize().GetHeight() * scale_factor - (top * 2);


	if (!invertX)
	{
		int widthmax = widthOutput - (parentRender->GetSize().GetWidth() * scale_factor) - xValue;
		rc.x = max(widthmax, 0);
	}
	else
		rc.x = max(xValue, 0);

	if (!invertY)
	{
		int heightmax = heightOutput - (parentRender->GetSize().GetHeight() * scale_factor) - yValue;
		rc.y = max(heightmax, 0);
	}
	else
		rc.y = max(yValue, 0);

	rc.width = widthOutput;
	rc.height = heightOutput;
}

void CVideoControlSoft::CalculPositionVideo(int& widthOutput, int& heightOutput, wxRect& rc)
{

#ifndef WIN32
	double scale_factor = parentRender->GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif

	widthOutput = static_cast<int>(GetBitmapWidth());
	heightOutput = static_cast<int>(GetBitmapHeight());
    
    

	int left = 0, top = 0;
	int tailleAffichageWidth = widthOutput;
	int tailleAffichageHeight = heightOutput;

	if (parentRender->GetSize().GetWidth() * scale_factor > tailleAffichageWidth)
		left = ((parentRender->GetSize().GetWidth() * scale_factor - tailleAffichageWidth) / 2);
	else
		left = 0;

	if (parentRender->GetSize().GetHeight() * scale_factor > tailleAffichageHeight)
		top = ((parentRender->GetSize().GetHeight() * scale_factor - tailleAffichageHeight) / 2);
	else
		top = 0;

	CalculRectPictureInterpolation(rc, widthOutput, heightOutput, left, top, true, true);

}

void CVideoControlSoft::RenderToTexture(COpenCLEffectVideo* openclEffect)
{
    
	if (openclEffect == nullptr)
		return;


	int widthOutput = 0;
	int heightOutput = 0;
	wxRect rc(0, 0, 0, 0);
	CalculPositionVideo(widthOutput, heightOutput, rc);

    if(widthOutput > 0 && heightOutput > 0)
    {
        wxRect rect;
        int filterInterpolation = 0;
        inverted = true;

        CRegardsConfigParam* regardsParam = CParamInit::getInstance();

        if (regardsParam != nullptr)
            filterInterpolation = regardsParam->GetInterpolationType();


        if ((videoEffectParameter.stabilizeVideo || videoEffectParameter.autoConstrast) && videoEffectParameter.
            effectEnable)
        {
            if (openCVStabilization == nullptr)
                openCVStabilization = new Regards::OpenCV::COpenCVStabilization(videoEffectParameter.stabilizeImageBuffere);
            openclEffect->ApplyOpenCVEffect(&videoEffectParameter, openCVStabilization);
        }

        openclEffect->InterpolationZoomBicubic(widthOutput, heightOutput, rc, flipH, flipV, angle, filterInterpolation,
                                               (int)GetZoomRatio() * 100);
                                            
        cv::UMat data = openclEffect->GetUMat(false);
        if (!renderOpenGL->SetData(data))
            openclOpenGLInterop = false;

        int nError = glGetError();

    }
}

bool CVideoControlSoft::ApplyOpenCVEffect(cv::Mat& image)
{
	bool frameStabilized = false;
	//cv::Mat & image = pictureFrame->GetMatrix();
	if (videoEffectParameter.stabilizeVideo)
	{
		if (openCVStabilization == nullptr)
			openCVStabilization = new Regards::OpenCV::COpenCVStabilization(videoEffectParameter.stabilizeImageBuffere);

		openCVStabilization->SetNbFrameBuffer(videoEffectParameter.stabilizeImageBuffere);

		if (openCVStabilization->GetNbFrameBuffer() == 0)
		{
			openCVStabilization->BufferFrame(image);
		}
		else
		{
			frameStabilized = true;
			openCVStabilization->AddFrame(image);
		}

		if (frameStabilized)
			openCVStabilization->CorrectFrame(image);
	}

	if (videoEffectParameter.autoConstrast)
	{
		frameStabilized = true;
		CFiltreEffetCPU::BrightnessAndContrastAuto(image, 1.0);
	}
	//pictureFrame->SetBitmap(image.data, pictureFrame->GetBitmapWidth(), pictureFrame->GetBitmapHeight());
	return frameStabilized;
}


void CVideoControlSoft::RenderFFmpegToTexture(cv::Mat& pictureFrame)
{
    //printf("RenderFFmpegToTexture \n");
	int widthOutput = 0;
	int heightOutput = 0;
	inverted = true;
	//if (angle == 90 || angle == 270)
	//	inverted = false;

	wxRect rc(0, 0, 0, 0);
	CalculPositionVideo(widthOutput, heightOutput, rc);

	if (pictureFrame.empty())
		return;

    if(widthOutput > 0 && heightOutput > 0)
    {
            //GLTexture* glTexture = nullptr;
        CRgbaquad backColor;

        int filterInterpolation = 0;
        CRegardsConfigParam* regardsParam = CParamInit::getInstance();
        if (regardsParam != nullptr)
            filterInterpolation = regardsParam->GetInterpolationType();


        cv::Mat cvImage;

        cv::cvtColor(pictureFrame, cvImage, cv::COLOR_BGRA2BGR);

        cv::Mat bitmapOut = CFiltreEffetCPU::Interpolation(cvImage, widthOutput, heightOutput, rc, filterInterpolation,
                                                           flipH, flipV, angle, (int)GetZoomRatio() * 100);

        if ((videoEffectParameter.stabilizeVideo || videoEffectParameter.autoConstrast) && videoEffectParameter.
            effectEnable)
        {
            ApplyOpenCVEffect(bitmapOut);
        }

        renderOpenGL->SetData(bitmapOut);   
    }

}

void CVideoControlSoft::Rotate90()
{
	angle += 90;
	angle = angle % 360;
	CalculPositionPicture(centerX, centerY);
	UpdateScrollBar();

	CSqlPhotos sqlPhotos;
	int exif = CSqlPhotos::GetExifFromAngleAndFlip(angle, flipH ? 1 : 0, flipV ? 1 : 0);
	sqlPhotos.UpdatePhotoExif(filename, exif);
}

void CVideoControlSoft::Rotate270()
{
	angle += 270;
	angle = angle % 360;
	CalculPositionPicture(centerX, centerY);
	UpdateScrollBar();

	CSqlPhotos sqlPhotos;
	int exif = CSqlPhotos::GetExifFromAngleAndFlip(angle, flipH ? 1 : 0, flipV ? 1 : 0);
	sqlPhotos.UpdatePhotoExif(filename, exif);
}

void CVideoControlSoft::FlipVertical()
{
	flipV = !flipV;
	if (pause)
	{
		muRefresh.lock();
		needToRefresh = true;
		muRefresh.unlock();
	}

	CSqlPhotos sqlPhotos;
	int exif = CSqlPhotos::GetExifFromAngleAndFlip(angle, flipH ? 1 : 0, flipV ? 1 : 0);
	sqlPhotos.UpdatePhotoExif(filename, exif);
}

void CVideoControlSoft::FlipHorizontal()
{
	flipH = !flipH;
	if (pause)
	{
		muRefresh.lock();
		needToRefresh = true;
		muRefresh.unlock();
	}

	CSqlPhotos sqlPhotos;
	int exif = CSqlPhotos::GetExifFromAngleAndFlip(angle, flipH ? 1 : 0, flipV ? 1 : 0);
	sqlPhotos.UpdatePhotoExif(filename, exif);
}

bool CVideoControlSoft::IsCPUContext()
{
	if (isCPU == -1)
	{
		isCPU = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_CPU ? 1 : 0);
	}

	//printf("IsCPUContext CPU : %d \n", isCPU);

	return (isCPU == 1 ? true : false);
}

void CVideoControlSoft::SetSoundVolume(const int& soundVolume)
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

	if (cv::ocl::Context::getDefault(false).empty())
		return 0;

	return supportOpenCL;
}


void CVideoControlSoft::SetFrameData(AVFrame* src_frame)
{
	int enableopenCL = 0;
	bool isCPU = true;
	if (IsSupportOpenCL())
	{
		enableopenCL = 1;
		isCPU = IsCPUContext();
	}
	else
	{
		isffmpegDecode = true;
		enableopenCL = 0;
	}

	if (!enableopenCL || (src_frame->format != AV_PIX_FMT_YUV420P && src_frame->format != AV_PIX_FMT_NV12))
	{
		isffmpegDecode = true;
		int nWidth = src_frame->width;
		int nHeight = src_frame->height;
		if (videoEffectParameter.denoiseEnable && videoEffectParameter.effectEnable)
		{
			if (hq3d == nullptr)
				hq3d = new Chqdn3d(nWidth, nHeight, videoEffectParameter.denoisingLevel, videoEffectParameter.templateWindowSize, videoEffectParameter.searchWindowSize);
			else
				hq3d->UpdateParameter(nWidth, nHeight, videoEffectParameter.denoisingLevel, videoEffectParameter.templateWindowSize, videoEffectParameter.searchWindowSize);
			uint8_t* outData = hq3d->ApplyDenoise3D(src_frame->data[0], src_frame->linesize[0], nHeight);
			memcpy(src_frame->data[0], outData, src_frame->linesize[0] * nHeight);
		}
		cv::Mat bitmapData = GetBitmapRGBA(src_frame);
		muBitmap.lock();
		bitmapData.copyTo(pictureFrame);
		muBitmap.unlock();
	}
	else
	{
		isffmpegDecode = false;
		if (openclEffectYUV != nullptr)
		{
			int _colorSpace = 0;
			int isLimited = 0;
			if (colorRange == "Limited")
				isLimited = 1;

			if (colorSpace == "BT.601")
			{
				_colorSpace = 1;
			}
			else if (colorSpace == "BT.709")
			{
				_colorSpace = 2;
			}
			else if (colorSpace == "BT.2020")
			{
				_colorSpace = 3;
			}

			//cv::ocl::OpenCLExecutionContext::getCurrent().bind();
			int nWidth = src_frame->width;
			int nHeight = src_frame->height;
			AVFrame* tmp_frame = src_frame;
			if (tmp_frame->format == AV_PIX_FMT_NV12)
			{
				muBitmap.lock();
				//Test if denoising Effect
				if (videoEffectParameter.denoiseEnable && videoEffectParameter.effectEnable)
				{
					/*
					cv::Mat yChannel(nHeight, tmp_frame->linesize[0], CV_8UC1, tmp_frame->data[0]);
					cv::UMat yChannelOut;
					yChannel.copyTo(yChannelOut);
					fastNlMeansDenoising(out, out, videoEffectParameter.denoisingLevel, videoEffectParameter.templateWindowSize, videoEffectParameter.searchWindowSize);

					openclEffectYUV->SetNV12(yChannelOut, tmp_frame->data[1],
						tmp_frame->linesize[1] * (nHeight / 2), tmp_frame->linesize[0], nHeight,
						tmp_frame->linesize[0], nWidth, nHeight, isLimited, _colorSpace);
					*/

					uint8_t * outData = openclEffectYUV->HQDn3D(tmp_frame->data[0], tmp_frame->linesize[0], nHeight, videoEffectParameter.denoisingLevel, videoEffectParameter.templateWindowSize, videoEffectParameter.searchWindowSize);
					openclEffectYUV->SetNV12(outData, tmp_frame->linesize[0] * nHeight, tmp_frame->data[1],
						tmp_frame->linesize[1] * (nHeight / 2), tmp_frame->linesize[0], nHeight,
						tmp_frame->linesize[0], nWidth, nHeight, isLimited, _colorSpace);

				}
				else
					openclEffectYUV->SetNV12(tmp_frame->data[0], tmp_frame->linesize[0] * nHeight, tmp_frame->data[1],
											 tmp_frame->linesize[1] * (nHeight / 2), tmp_frame->linesize[0], nHeight,
											 tmp_frame->linesize[0], nWidth, nHeight, isLimited, _colorSpace);
				muBitmap.unlock();
			}
			else if (tmp_frame->format == AV_PIX_FMT_YUV420P)
			{
				muBitmap.lock();
				if (videoEffectParameter.denoiseEnable && videoEffectParameter.effectEnable)
				{
					uint8_t* outData = openclEffectYUV->HQDn3D(tmp_frame->data[0], tmp_frame->linesize[0], nHeight, videoEffectParameter.denoisingLevel, videoEffectParameter.templateWindowSize, videoEffectParameter.searchWindowSize);
					openclEffectYUV->SetYUV420P(outData, tmp_frame->linesize[0] * nHeight, tmp_frame->data[1],
						tmp_frame->linesize[1] * (nHeight / 2), tmp_frame->data[2],
						tmp_frame->linesize[2] * (nHeight / 2), tmp_frame->linesize[0], nHeight,
						tmp_frame->linesize[0], nWidth, nHeight, isLimited, _colorSpace);
				}
				else
				{
					openclEffectYUV->SetYUV420P(tmp_frame->data[0], tmp_frame->linesize[0] * nHeight, tmp_frame->data[1],
						tmp_frame->linesize[1] * (nHeight / 2), tmp_frame->data[2],
						tmp_frame->linesize[2] * (nHeight / 2), tmp_frame->linesize[0], nHeight,
						tmp_frame->linesize[0], nWidth, nHeight, isLimited, _colorSpace);
				}

				muBitmap.unlock();
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

void CVideoControlSoft::RenderToGLTexture()
{
    //printf("RenderToGLTexture  \n"); 
	std::clock_t start;
	start = std::clock();
	double duration;

	if (!isffmpegDecode)
	{
       //  printf("RenderToGLTexture RenderToTexture  \n"); 
		muBitmap.lock();
		if (openclEffectYUV != nullptr && openclEffectYUV->IsOk())
		{
			RenderToTexture(openclEffectYUV);
		}
		muBitmap.unlock();

		deleteTexture = false;
	}
	else
	{
        //printf("RenderToGLTexture RenderFFmpegToTexture  \n"); 
		cv::Mat bitmap;
		muBitmap.lock();
		pictureFrame.copyTo(bitmap);
		muBitmap.unlock();
		RenderFFmpegToTexture(bitmap);
	}

	duration = (std::clock() - start) / static_cast<double>(CLOCKS_PER_SEC);

}
