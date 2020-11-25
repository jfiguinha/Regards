#include "header.h"
#include "ShowVideo.h"
#include "SliderVideo.h"
#include "VideoControl.h"
#include "VideoControl_soft.h"
#include <OpenCLContext.h>
#include <window_id.h>
#include "SlideToolbar.h"
#include <ViewerParam.h>
#include <SavePicture.h>
#include <ImageLoadingFormat.h>
using namespace Regards::Video;
using namespace Regards::Window;
using namespace Regards::OpenCL;
using namespace Regards::Viewer;

#define WM_SETPOSITION SDL_USER+1

CShowVideo::CShowVideo(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, CThemeParam * config)
	: wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0)
{

	toolbarOutside = false;
	height = 0;
	width = 0;
	videoPosOld = 0;
	isDiaporama = false;

	CThemeSlider themeSlider;
	CTheme themeVideo;

		
	if (config != nullptr)
	{
		config->GetVideoControlTheme(&themeVideo);
	}
    
#ifdef WIN32

	int dxva2 = 0;
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		dxva2 = regardsParam->GetDxva2Actif();
	}
	if (dxva2)
	{
		OpenCLDevice * device = COpenCLEngine::GetDefaultDevice();
		if (device != nullptr)
		{
			if (device->deviceType == CL_DEVICE_TYPE_CPU)
				softRender = true;
			else if (!device->openGlSharing)
				softRender = true;
		}
	}
	else
	{
		softRender = true;
	}



	if (softRender)
		videoWindow = CVideoControlSoft::CreateWindow(this, VIDEOCONTROL, windowMain, this);
	else
		videoWindow = CVideoControl::CreateWindow(this, VIDEOCONTROL, windowMain, this);

	

#else
     videoWindow = CVideoControlSoft::CreateWindow(this, VIDEOCONTROL, windowMain, this);
#endif

    scrollbar = new CScrollbarWnd(this, videoWindow, wxID_ANY, "VideoScroll");

	if (config != nullptr)
	{
		CThemeSlider theme;
		config->GetVideoSliderTheme(&theme);
	}

	CThemeToolbar themeToolbar;
	if (config != nullptr)
	{
		config->GetBitmapToolbarTheme(&themeToolbar);
	}


	videoSlider = new CSliderVideo(this, wxID_ANY, this, themeSlider);
	slideToolbar = new CSlideToolbar(this, wxID_ANY, themeToolbar);
	
	for (int i = 0; i < 101; i++)
		value.push_back(i);
	slideToolbar->SetTabValue(value);
	play = false;


    Connect(wxEVENT_SAVE, wxCommandEventHandler(CShowVideo::OnSave));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CShowVideo::OnSize));
	Connect(wxEVENT_SHRINKPOS, wxCommandEventHandler(CShowVideo::OnValueShrinkChange));
	Connect(wxEVENT_ZOOMPOS, wxCommandEventHandler(CShowVideo::OnValueChange));
	Connect(wxEVENT_SHRINK, wxCommandEventHandler(CShowVideo::OnShrink));
	Connect(wxEVENT_CLOSE, wxCommandEventHandler(CShowVideo::OnClose));
	this->windowMain = windowMain;
}

void CShowVideo::OnValueShrinkChange(wxCommandEvent& event)
{
	slideToolbar->SetTrackBarPosition(videoWindow->GetZoomIndex());
}

void CShowVideo::OnClose(wxCommandEvent& event)
{
	slideToolbar->Show(false);
	showToolbarSup = false;
	this->Resize();
}

void CShowVideo::OnSave(wxCommandEvent& event)
{
    if (videoWindow != nullptr)
    {
        CRegardsBitmap * bitmap = videoWindow->SavePicture();
        CImageLoadingFormat * imageLoading = new CImageLoadingFormat();
        bitmap->SetFilename(this->filename);
        if (videoWindow->IsFFmpegDecode())
            bitmap->VertFlipBuf();
        imageLoading->SetPicture(bitmap);
        CSavePicture::SavePicture(nullptr, imageLoading, filename);
        if (imageLoading != nullptr)
            delete imageLoading;
    }

	
}

void CShowVideo::SavePicture()
{
    wxCommandEvent evt(wxEVENT_SAVE);
    this->GetEventHandler()->AddPendingEvent(evt);
}

void CShowVideo::OnShrink(wxCommandEvent& event)
{
	videoWindow->ShrinkVideo();
	slideToolbar->SetTrackBarPosition(videoWindow->GetZoomIndex());
	scrollbar->HideHorizontalScroll();
	scrollbar->HideVerticalScroll();
}


void CShowVideo::OnValueChange(wxCommandEvent& event)
{
	int value = event.GetInt();
	int mode = slideToolbar->GetMode();
	if (mode == SPEAKER_MODE)
	{
		videoWindow->SetVolume(value);
	}
	else
	{
		scrollbar->ShowHorizontalScroll();
		scrollbar->ShowVerticalScroll();
		videoWindow->SetZoomIndex(value);

	}
}

CVideoControlSoft * CShowVideo::GetVideoControl()
{
	return videoWindow;
}

void CShowVideo::SetVideoPreviewEffect(CEffectParameter * effectParameter)
{
	if(videoWindow != nullptr)
		videoWindow->SetVideoPreviewEffect(effectParameter);
}

CEffectParameter *  CShowVideo::GetParameter()
{
	if(videoWindow != nullptr)
		return videoWindow->GetParameter();
	return nullptr;
}

void  CShowVideo::UpdateFiltre(CEffectParameter * effectParameter)
{
	if(videoWindow != nullptr)
		videoWindow->UpdateFiltre(effectParameter);
}

void CShowVideo::ChangeAudio(const int &langue)
{
	videoWindow->ChangeAudioStream(langue);
}

void CShowVideo::ClickButton(const int &id)
{
	switch (id)
	{
	case PLAYBUTTONID:
		PlayVideo();
		break;
	case PAUSEBUTTONID:
		PauseVideo();
		break;
	case SPEAKERBUTTONID:
		slideToolbar->Show(true);
		showToolbarSup = true;
		slideToolbar->SetSpeakerMode();
		slideToolbar->SetTabValue(value);
		slideToolbar->SetTrackBarPosition(videoWindow->GetVolume());
		this->Resize();
		break;
	case ZOOMBUTTONID:
		slideToolbar->Show(true);
		showToolbarSup = true;
		slideToolbar->SetZoomMode();
		slideToolbar->SetTabValue(videoWindow->GetZoomValue());
		slideToolbar->SetTrackBarPosition(videoWindow->GetZoomIndex());
		this->Resize();
		break;
	case SCREENFORMATID:
		slideToolbar->Show(false);
		showToolbarSup = false;
		videoWindow->ChangeVideoFormat();
		this->Resize();
		break;
		/*
	case VOLUMEUPBUTTONID:
		videoWindow->VolumeUp();
		videoSlider->SetVolumePos(videoWindow->GetVolume());
		break;
	case VOLUMEDOWNBUTTONID:
		videoWindow->VolumeDown();
		videoSlider->SetVolumePos(videoWindow->GetVolume());
		break;
		*/
	}
    
#ifdef __APPLE__
    videoSlider->CallRefresh(videoSlider);
#else
	videoSlider->Refresh();
#endif
}

CShowVideo::~CShowVideo()
{
	delete(videoWindow);
	delete(scrollbar);
	
	delete(videoSlider);
	delete(slideToolbar);
}

void CShowVideo::SetDiaporamaMode()
{
	isDiaporama = true;
	this->Resize();
}

void CShowVideo::SetNormalMode()
{
	isDiaporama = false;
	this->Resize();
}

void CShowVideo::SetStreamInfo(vector<CStreamInfo> & listAudio, vector<CStreamInfo> & listVideo, vector<CStreamInfo> & listSubtitle)
{
	listStream.clear();
	for(int i = 0; i < listAudio.size();i++)
	{
		listStream.push_back(listAudio[i]);
	}

	for(int i = 0; i < listVideo.size();i++)
	{
		listStream.push_back(listVideo[i]);
	}

	for(int i = 0; i < listSubtitle.size();i++)
	{
		listStream.push_back(listSubtitle[i]);
	}

	if (windowMain != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ADDSTREAMINFO);
		evt.SetClientData(&listStream);
		this->windowMain->GetEventHandler()->AddPendingEvent(evt);
	}
}

//------------------------------------------------------------
//Interface Video
//------------------------------------------------------------

void CShowVideo::OnVideoEnd()
{
	videoSlider->SetPause();
	if (windowMain != nullptr)
	{
		wxCommandEvent evt(VIDEO_END_ID);
		this->windowMain->GetEventHandler()->AddPendingEvent(evt);
	}
#ifdef __APPLE__
    videoSlider->CallRefresh(videoSlider);
#else
	videoSlider->Refresh();
#endif
}

void CShowVideo::SetVideoDuration(const int64_t &position)
{
	if (videoWindow != nullptr && videoSlider != nullptr)
	{
		videoTotalTime = position;
		videoSlider->SetTotalSecondTime(position / 1000);
	}
#ifdef __APPLE__
    videoSlider->CallRefresh(videoSlider);
#else
	videoSlider->Refresh();
#endif
}

void CShowVideo::OnVideoStart()
{
	if (videoWindow != nullptr && videoSlider != nullptr)
	{
		this->play = true;
		videoSlider->SetPlay();
	}
    
    if (windowMain != nullptr)
    {
        wxCommandEvent evt(VIDEO_START);
        this->windowMain->GetEventHandler()->AddPendingEvent(evt);
    }
#ifdef __APPLE__
    videoSlider->CallRefresh(videoSlider);
#else
	videoSlider->Refresh();
#endif
}

void CShowVideo::OnVideoStop()
{

}

void CShowVideo::OnVideoPause()
{

}

void CShowVideo::OnAfterOpenVideo()
{
	videoSlider->SetTotalSecondTime(0);
	videoSlider->SetPastSecondTime(0);
	ShowSlider(false);
	PlayVideo();
	//videoSlider->SetVolumePos(videoWindow->GetVolume());
#ifdef __APPLE__
    videoSlider->CallRefresh(videoSlider);
#else
	videoSlider->Refresh();
#endif
}

void CShowVideo::OnPositionVideo(const int64_t &position)
{
	if (position >= 0 && position <= videoTotalTime)
	{

		if (videoSlider != nullptr)
			videoSlider->SetPastSecondTime(position);

		int videoPos = position / 1000;
		if (videoPos != videoPosOld)
		{
			wxWindow * viewerWindow = this->FindWindowById(CENTRALVIEWERWINDOWID);
			if (viewerWindow != nullptr)
			{
				wxCommandEvent event(VIDEO_UPDATE_ID);
				event.SetExtraLong(videoPos);
				viewerWindow->GetEventHandler()->AddPendingEvent(event);
			}
			videoPosOld = videoPos;
		}

#ifdef __APPLE__
		videoSlider->CallRefresh(videoSlider);
#else
		videoSlider->Refresh();
#endif
	}
	//wxPostEvent(main, event);

	//CLocalWindow * mainWindow = (CLocalWindow *)CWindowManagerEngine::getInstance()->GetWindow(L"MainWindow");
	//if (mainWindow != nullptr)
	//	mainWindow->OnCommand(WM_SETPOSITION, 0, position / 1000);
}

//------------------------------------------------------------

void CShowVideo::SetPosition(const int64_t &timePosition)
{
	if (videoWindow != nullptr)
	{
		videoWindow->SetVideoPosition(timePosition);
	}
}

void CShowVideo::SetTimePosition(const int64_t &timePosition)
{
	if (videoSlider != nullptr)
	{
		videoSlider->SetPastSecondTime(timePosition / 1000);
		videoSlider->UpdatePositionEvent();
#ifdef __APPLE__
    videoSlider->CallRefresh(videoSlider);
#else
	videoSlider->Refresh();
#endif
	}
}

void CShowVideo::MoveSlider(const int64_t &position)
{
	if (videoWindow != nullptr)
		videoWindow->SetVideoPosition(position);
}

void CShowVideo::InitControl()
{
	if (videoWindow != nullptr)
	{
		Resize();
	}
}

bool CShowVideo::SetVideo(const wxString &filename, const int &rotation, const bool &play)
{
	videoTotalTime = 0;
	videoPosOld = 0;
	bool value = false;
    this->filename = filename;
	if (videoWindow != nullptr && videoSlider != nullptr)
	{
		value = videoWindow->PlayMovie(filename, true);
	}
	return value;
}

void CShowVideo::UpdateScreenRatio()
{
	scrollbar->UpdateScreenRatio();
    videoSlider->UpdateScreenRatio();
    videoWindow->UpdateScreenRatio();
    this->Resize();
}

void CShowVideo::StopVideo(wxString photoName)
{
	play = false;
	ShowSliderToolbar(true);
	videoSlider->SetPastSecondTime(0);
	videoWindow->OnStop(photoName);
#ifdef __APPLE__
    videoSlider->CallRefresh(videoSlider);
#else
	videoSlider->Refresh();
#endif
}

void CShowVideo::ShowSliderToolbar(const bool &show)
{
	if (show && !videoSlider->IsShown())
	{
		videoSlider->Show(true);
		Resize();
	}
	else if (!show && videoSlider->IsShown())
	{
		videoSlider->Show(false);
		Resize();
	}
}


void CShowVideo::ShowSlider(const bool &show)
{
	if (play)
		ShowSliderToolbar(true);

}

void CShowVideo::Rotate90()
{
	if (videoWindow != nullptr)
		videoWindow->Rotate90();
}

void CShowVideo::Rotate270()
{
	if (videoWindow != nullptr)
		videoWindow->Rotate270();
}

void CShowVideo::FlipVertical()
{
	if (videoWindow != nullptr)
		videoWindow->FlipVertical();
}

void CShowVideo::FlipHorizontal()
{
	if (videoWindow != nullptr)
		videoWindow->FlipHorizontal();
}

void CShowVideo::PlayVideo()
{
	videoWindow->OnPlay();
	videoSlider->SetPlay();
}

void CShowVideo::PauseVideo()
{
	play = false;
	videoWindow->OnPause();
	ShowSliderToolbar(true);
	videoSlider->SetPause();
#ifdef __APPLE__
    videoSlider->CallRefresh(videoSlider);
#else
	videoSlider->Refresh();
#endif
}

void CShowVideo::ShowToolbar()
{
	if (videoSlider != nullptr)
	{
		if (!videoSlider->IsShown())
			videoSlider->Show(true);
		if (!slideToolbar->IsShown() && showToolbarSup)
			slideToolbar->Show(true);
	}
	this->Resize();
}

void CShowVideo::HideToolbar()
{
	if (videoSlider != nullptr)
	{
		if (!videoSlider->IsMouseOver())
		{
			if (videoSlider->IsShown())
				videoSlider->Show(false);
			if (slideToolbar->IsShown())
				slideToolbar->Show(false);
		}
	}
	this->Resize();
}

bool CShowVideo::IsToolbarMouseOver()
{
	if (videoSlider != nullptr)
		return videoSlider->IsMouseOver();

	return false;
}

void CShowVideo::Resize()
{
	if (isDiaporama && videoSlider != nullptr && videoWindow != nullptr)
	{
		slideToolbar->SetSize(0, 0, 0, 0);
		slideToolbar->Refresh();
		videoSlider->SetSize(0, 0, 0, 0);
		videoSlider->Refresh();
		scrollbar->SetSize(0, 0, width, height);
		scrollbar->Refresh();
	}
	else if(videoSlider != nullptr)
	{
		//CDeferPos deferpos;
		int posHeight = 0;
		int posHeightSup = 0;
		if (videoSlider->IsShown())
			posHeight += videoSlider->GetHeight();

		if(showToolbarSup && slideToolbar->IsShown())
			posHeightSup = posHeight;

		int positionHeight = height - posHeight - posHeightSup;

		scrollbar->SetSize(0, 0, width, positionHeight);
		scrollbar->Resize();

		if (videoSlider->IsShown())
		{
			if (showToolbarSup)
			{
				slideToolbar->SetSize(0, positionHeight, width, videoSlider->GetHeight(), TRUE);
				slideToolbar->Refresh();
			}

			videoSlider->SetSize(0, positionHeight + posHeightSup, width, videoSlider->GetHeight(), TRUE);
			videoSlider->Refresh();
		}
	}
    else
    {
		scrollbar->SetSize(0, 0, width, height);
		scrollbar->Refresh();
    }

	
}

void CShowVideo::OnSize(wxSizeEvent& event)
{
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	Resize();
}

