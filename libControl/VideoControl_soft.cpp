// ReSharper disable All
#include "header.h"
#include "VideoControl_soft.h"
#include "ffplaycore.h"
#ifdef __APPLE__
#include <OpenCL/cl.h>
#include <OpenCL/opencl.h>
#include <utility.h>
#else
#include <CL/cl.h>
#endif
#include <picture_utility.h>
#include "ScrollbarWnd.h"
#include "ClosedHandCursor.h"
#include <ConvertUtility.h>
#include <hqdn3d.h>
#include <SqlPhotos.h>
#include <RegardsConfigParam.h>
#include <MediaInfo.h>
#include <VideoStabilization.h>
#include <FiltreEffetCPU.h>
#include <aspectratio.h>
#include <ParamInit.h>
#include <OpenCLEffectVideo.h>
#include "DataAVFrame.h"
#ifdef USE_CUDA
#include <CudaEffectVideo.h>
#endif
using namespace Regards::OpenCV;
#ifdef USE_CUDA
using namespace Regards::Cuda;
#endif
using namespace Regards::OpenCL;
using namespace Regards::Sqlite;

//#include "LoadingResource.h"

#define TIMER_FPS 0x10001
#define TIMER_PLAYSTART 0x10002
#define TIMER_PLAYSTOP 0x10003
#define TIMER_SUBTITLE 0x10004

extern bool firstElementToShow;
AVFrame* copyFrameBuffer = nullptr;
extern cv::ocl::OpenCLExecutionContext clExecCtx;



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
    
    CRegardsConfigParam* config = CParamInit::getInstance();
    if (config != nullptr)
	{
		CVideoEffectParameter * parameter = config->GetVideoEffectParameter();
		videoEffectParameter = *parameter;
    }

    
   
    
}

vector<int> CVideoControlSoft::GetListTimer()
{
	return { TIMER_PLAYSTOP, TIMER_FPS, TIMER_PLAYSTART, TIMER_SUBTITLE };
}


vector<int> CVideoControlSoft::GetListCommand()
{
	return {
		wxEVENT_UPDATEPOSMOVIETIME, wxEVENT_SCROLLMOVE, wxEVENT_ENDVIDEOTHREAD,
		wxEVENT_STOPVIDEO, EVENT_VIDEOSTART, wxEVENT_LEFTPOSITION,
		wxEVENT_TOPPOSITION, wxEVENT_SETPOSITION, EVENT_VIDEOROTATION,
		wxEVENT_UPDATEMOVIETIME, wxEVENT_UPDATEFRAME, wxEVENT_PAUSEMOVIE
	};
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
	case TIMER_SUBTITLE:
	{
		subtilteUpdate = false;
		break;
	}
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
	assSubtitleTimer = new wxTimer(parentRender, TIMER_SUBTITLE);
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
	std::lock_guard<std::mutex> lock(muVideoEffect);
	return videoEffectParameter.tabRatio[videoEffectParameter.ratioSelect];
}

cv::Mat CVideoControlSoft::SavePicture(bool& isFromBuffer)
{
	cv::Mat bitmap;
	if (openclEffectYUV != nullptr && openclEffectYUV->IsOk())
	{
		bitmap = openclEffectYUV->GetMatrix(true).getMat();
	}
	else if(pictureFrame != nullptr)
	{
		if(pictureFrame->matFrame != nullptr)
			pictureFrame->matFrame->copyTo(bitmap);
		//muBitmap.unlock();
	}


	if (!bitmap.empty())
	{
		CPictureUtility::ApplyRotation(bitmap, angle);
	}
	return bitmap;
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
	//muRefresh.lock();
	needToRefresh = true;
	//muRefresh.unlock();
}

void CVideoControlSoft::OnTopPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
	//muRefresh.lock();
	needToRefresh = true;
	//muRefresh.unlock();
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

	//muRefresh.lock();
	needToRefresh = true;
	//muRefresh.unlock();
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
	//muRefresh.lock();
	needToRefresh = true;
	//muRefresh.unlock();
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
	int inverseRotation = 0;
	long rotation = event.GetExtraLong();
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		inverseRotation = config->GetInverseVideoRotation();
	if (inverseRotation == 0)
	{
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
	}
	else
	{
		angle = rotation;
	}

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
		ChangeSubtitleStream(videoParameter->streamSubtitleIndex);
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
			//muRefresh.lock();
			needToRefresh = true;
			//muRefresh.unlock();
		}
	}


	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
	{
		config->SetVideoEffectParameter(videoParameter);
	}
}


bool CVideoControlSoft::GetPausedValue()
{
	return pause;
};

void CVideoControlSoft::RedrawFrame()
{
	//muRefresh.lock();
	needToRefresh = true;
	//muRefresh.unlock();
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
	//muRefresh.lock();
	localRefresh = needToRefresh;
	needToRefresh = false;

	//muRefresh.unlock();
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


	if (startVideoAfterProblem)
	{
		PlayMovie(filename, true);
		startVideoAfterProblem = false;
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

	if (assSubtitleTimer->IsRunning())
		assSubtitleTimer->Stop();

	if (playStartTimer->IsRunning())
		playStartTimer->Stop();

	if (playStopTimer->IsRunning())
		playStopTimer->Stop();



	if (hq3d != nullptr)
		delete hq3d;

	if (openCVStabilization != nullptr)
		delete openCVStabilization;

	delete playStartTimer;
	delete assSubtitleTimer;
	delete fpsTimer;

	if (renderBitmapOpenGL != nullptr)
	{
		delete renderBitmapOpenGL;
	}

	if (openclEffectYUV != nullptr)
		delete openclEffectYUV;

	if (ffmfc)
		delete ffmfc;

	if (pictureFrame != nullptr)
		delete pictureFrame;

}

void CVideoControlSoft::SetSubtituleText(const char* textSub, int timing)
{
	muSubtitle.lock();
	subtitleText = textSub;
	std::vector<wxString> listString = CConvertUtility::split(subtitleText, ',');
	int timeShow = atoi(listString.at(0));
	if (listString.size() > 9)
	{
		subtitleText = "";
		for (int i = 8; i < listString.size(); i++)
		{
			subtitleText.append(listString.at(i) + ",");
		}
		subtitleText = subtitleText.SubString(0,subtitleText.size() - 1);
	}
	else
		subtitleText = listString.at(listString.size() - 1);
	
	
	subtilteUpdate = true;
	typeSubtitle = 1;
	if (assSubtitleTimer->IsRunning())
		assSubtitleTimer->Stop();
	assSubtitleTimer->StartOnce(10);
	muSubtitle.unlock();
}


void CVideoControlSoft::SetSubtitulePicture(cv::Mat& picture)
{
	muSubtitle.lock();
	picture.copyTo(pictureSubtitle);
	subtilteUpdate = true;
	typeSubtitle = 0;
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



int CVideoControlSoft::PlayMovie(const wxString& movie, const bool& play)
{
	errorDecoding = false;
    
    if(movie != filename)
    {
        isHardwareDecoder = true;
    }
	if (standByMovie != "")
	{
		return Play(standByMovie);
	}

    return Play(movie);
}

int CVideoControlSoft::Play(const wxString& movie)
{
    if (videoEnd || stopVideo)
	{
		if (movie != filename)
		{
			if (openCVStabilization != nullptr)
				delete openCVStabilization;

			openCVStabilization = nullptr;

			if (playStartTimer->IsRunning())
				playStartTimer->Stop();


			muVideoEffect.lock();
			videoEffectParameter.ratioSelect = 0;
			muVideoEffect.unlock();

			AspectRatio aspectRatio = CMediaInfo::GetVideoAspectRatio(movie);
			if (aspectRatio.den != 0 && aspectRatio.num != 0)
			{
				float video_aspect_ratio = (float)aspectRatio.num / (float)aspectRatio.den;
				printf("video_aspect_ratio %d %d \n", aspectRatio.num, aspectRatio.den);
				for (int i = 0; i < videoEffectParameter.tabRatio.size(); i++)
				{
					printf("video_aspect_ratio %f \n", videoEffectParameter.tabRatio[i]);
					if (video_aspect_ratio < videoEffectParameter.tabRatio[i])
					{
						muVideoEffect.lock();
						videoEffectParameter.ratioSelect = i;
						muVideoEffect.unlock();
						break;
					}
				}
			}

			colorRange = CMediaInfo::GetColorRange(movie);
			colorSpace = CMediaInfo::GetColorSpace(movie);

			angle = 0;
			flipV = false;
			flipH = false;
		}


		startVideo = true;
		videoStartRender = false;
		videoStart = false;
		newVideo = true;
		initStart = true;
		videoRenderStart = false;
		filename = movie;
		standByMovie = "";
		pause = false;
		firstMovie = false;
		stopVideo = false;
		parentRender->Refresh();
	}
	else if (movie != filename)
	{
		OnStop(movie);
		//filename = movie;
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
	needToRefresh = true;
}

void CVideoControlSoft::ReloadResource()
{
	needToRefresh = true;
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

#ifdef USE_CUDA

	if (IsSupportCuda())
	{
		if (openclEffectYUV == nullptr)
		{
			openclEffectYUV = new CCudaEffectVideo();
		}
	}

#endif

   // printf("CVideoControlSoft::OnPaint3D 1 \n");

	if (IsSupportOpenCL() && openclEffectYUV == nullptr)
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


	int widthOutput = 0;
	int heightOutput = 0;
	wxRect rc(0, 0, 0, 0);
	CalculPositionVideo(widthOutput, heightOutput, rc);

	if (videoRenderStart)
	{
		if (IsSupportOpenCL() && openclEffectYUV != nullptr)
		{
			openclEffectYUV->SetMatrix(pictureFrame->matFrame);
		}

		if (IsSupportOpenCL() && openclEffectYUV != nullptr && openclEffectYUV->IsOk())
			RenderToTexture();
		else
			RenderFFmpegToTexture();


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
        #ifndef WIN32
            double scale_factor = parentRender->GetContentScaleFactor();
        #else
            double scale_factor = 1.0f;
        #endif
			renderOpenGL->Print(0, 1, scale_factor, CConvertUtility::ConvertToUTF8(msgFrame));
		}

		if (videoEffectParameter.enableSubtitle)
		{
			muSubtitle.lock();

			if (subtilteUpdate)
			{
				if (typeSubtitle == 0 && !pictureSubtitle.empty())
				{
					renderBitmapOpenGL->SetSubtitle(pictureSubtitle);
					subtilteUpdate = false;
					renderBitmapOpenGL->ShowSubtitle();
				}
				else if(typeSubtitle == 1)
				{

	#ifndef WIN32
					double scale_factor = parentRender->GetContentScaleFactor() * ((float)videoEffectParameter.subtitleSize);
	#else
					double scale_factor = 1.0f* ((float)videoEffectParameter.subtitleSize);
	#endif

					renderOpenGL->PrintSubtitle(width / 2, height / 4, scale_factor, videoEffectParameter.subtitleRedColor
                        , videoEffectParameter.subtitleGreenColor, videoEffectParameter.subtitleBlueColor, subtitleText);

				}
			
			}
			else if (subtilteUpdate)
			{
				subtilteUpdate = false;
			}

			muSubtitle.unlock();

			
		}
		muVideoEffect.unlock();
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


	if (errorDecoding)
	{
		ErrorDecodingFrame();
	}

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
    isHardwareDecoder = false;
    startVideoAfterProblem = true;
	QuitMovie();
	//ffmfc->Quit();
	//PlayMovie(filename, true);

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



void CVideoControlSoft::OnSetData(wxCommandEvent& event)
{
	CDataAVFrame* old = pictureFrame;
	CDataAVFrame* dataFrame = static_cast<CDataAVFrame*>(event.GetClientData());
	if (dataFrame != nullptr)
	{
		videoRenderStart = true;

		widthVideo = dataFrame->width;
		heightVideo = dataFrame->height;
		ratioVideo = static_cast<float>(dataFrame->width) / static_cast<float>(dataFrame->height);
		video_aspect_ratio = dataFrame->sample_aspect_ratio;
		pictureFrame = dataFrame;

		if (old != nullptr)
			delete old;

		needToRefresh = false;
		parentRender->Refresh();
	}
}


void CVideoControlSoft::SetData(CDataAVFrame* dataFrame)
{
	wxCommandEvent event(wxEVENT_UPDATEFRAME);
	event.SetClientData(dataFrame);
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

		//muRefresh.lock();
		needToRefresh = true;
		//muRefresh.unlock();
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

void CVideoControlSoft::RenderToTexture()
{
    
	if (openclEffectYUV == nullptr)
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

        if (videoEffectParameter.stabilizeVideo)
        {
            if (openCVStabilization == nullptr)
                openCVStabilization = new Regards::OpenCV::COpenCVStabilization(videoEffectParameter.stabilizeImageBuffere, openclEffectYUV->GetType());
			openclEffectYUV->ApplyStabilization(&videoEffectParameter, openCVStabilization);
        }
		openclEffectYUV->InterpolationZoomBicubic(widthOutput, heightOutput, rc, flipH, flipV, angle, filterInterpolation,
                                               (int)GetZoomRatio() * 100);

		if ((videoEffectParameter.autoConstrast || videoEffectParameter.filmEnhance || videoEffectParameter.filmcolorisation) && videoEffectParameter.
			effectEnable)
		{
			openclEffectYUV->ApplyOpenCVEffect(&videoEffectParameter);
		}

        Regards::Picture::CPictureArray pictureArray = Regards::Picture::CPictureArray(openclEffectYUV->GetMatrix(false));
		renderOpenGL->SetData(pictureArray);

        int nError = glGetError();

    }
}

bool CVideoControlSoft::ApplyOpenCVEffect(cv::Mat& image)
{
	bool frameStabilized = false;
	Regards::Picture::CPictureArray pictureArray(image);
	if (videoEffectParameter.stabilizeVideo)
	{
		if (openCVStabilization == nullptr)
			openCVStabilization = new Regards::OpenCV::COpenCVStabilization(videoEffectParameter.stabilizeImageBuffere, TYPE_CPU);

		openCVStabilization->SetNbFrameBuffer(videoEffectParameter.stabilizeImageBuffere);

		if (openCVStabilization->GetNbFrameBuffer() == 0)
		{
			openCVStabilization->BufferFrame(pictureArray);
		}
		else
		{
			frameStabilized = true;
			openCVStabilization->AddFrame(pictureArray);
		}

		if (frameStabilized)
		{
			Regards::Picture::CPictureArray output = openCVStabilization->CorrectFrame(pictureArray);
			output.copyTo(image);
		}
	}

	if (videoEffectParameter.autoConstrast)
	{
		frameStabilized = true;
		CFiltreEffetCPU::BrightnessAndContrastAuto(image, 1.0);
	}


	return frameStabilized;
}


void CVideoControlSoft::RenderFFmpegToTexture()
{
    //printf("RenderFFmpegToTexture \n");
	int widthOutput = 0;
	int heightOutput = 0;
	inverted = true;
	//if (angle == 90 || angle == 270)
	//	inverted = false;

	wxRect rc(0, 0, 0, 0);
	CalculPositionVideo(widthOutput, heightOutput, rc);

	if(pictureFrame != nullptr)
		if (pictureFrame->matFrame->empty())
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
		if (pictureFrame != nullptr)
		{
			if (pictureFrame->matFrame != nullptr)
			{
				pictureFrame->matFrame->copyTo(cvImage);

				cv::cvtColor(cvImage, cvImage, cv::COLOR_BGRA2BGR);

				if (videoEffectParameter.denoiseEnable && videoEffectParameter.effectEnable && !IsSupportOpenCL())
				{
					if (videoEffectParameter.denoiseEnable && videoEffectParameter.effectEnable)
					{
						if (hq3d == nullptr)
							hq3d = new Chqdn3d(widthVideo, heightVideo, videoEffectParameter.denoisingLevel, videoEffectParameter.templateWindowSize, videoEffectParameter.searchWindowSize);
						else
							hq3d->UpdateParameter(widthVideo, heightVideo, videoEffectParameter.denoisingLevel, videoEffectParameter.templateWindowSize, videoEffectParameter.searchWindowSize);
						hq3d->ApplyDenoise3D(cvImage);
					}
				}

				cv::Mat bitmapOut = CFiltreEffetCPU::Interpolation(cvImage, widthOutput, heightOutput, rc, filterInterpolation,
					flipH, flipV, angle, (int)GetZoomRatio() * 100);


				if ((videoEffectParameter.stabilizeVideo || videoEffectParameter.autoConstrast || videoEffectParameter.filmcolorisation || videoEffectParameter.filmEnhance) && videoEffectParameter.
					effectEnable)
				{
					ApplyOpenCVEffect(bitmapOut);
				}

				Regards::Picture::CPictureArray pictureArray = Regards::Picture::CPictureArray(bitmapOut);
				renderOpenGL->SetData(pictureArray);
			}
		}

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
		//muRefresh.lock();
		needToRefresh = true;
		//muRefresh.unlock();
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
		//muRefresh.lock();
		needToRefresh = true;
		//muRefresh.unlock();
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

	//if (cv::ocl::Context::getDefault(false).empty())
	//	return 0;

	return supportOpenCL;
}

int CVideoControlSoft::IsSupportCuda()
{
	int supportCuda = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportCuda = config->GetIsCudaSupport();

	//if (cv::ocl::Context::getDefault(false).empty())
	//	return 0;

	return supportCuda;
}

