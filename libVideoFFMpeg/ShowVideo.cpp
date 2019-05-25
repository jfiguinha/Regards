#include "header.h"
#include "ShowVideo.h"
#include "SliderVideo.h"
#include "VideoControl.h"
#include "VideoControl_soft.h"
#include <ThemeInit.h>
#include <OpenCLContext.h>
#include <window_id.h>

using namespace Regards::Video;
using namespace Regards::Window;
using namespace Regards::OpenCL;

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
    OpenCLDevice * device = COpenCLEngine::GetDefaultDevice();
    if(device != nullptr)
    {
        if(device->deviceType == CL_DEVICE_TYPE_CPU)
            softRender = true;
        else if(!device->openGlSharing)
            softRender = true;
    }

    if(softRender)
        videoWindow = CVideoControlSoft::CreateWindow(this, VIDEOCONTROL, windowMain, this);
    else
        videoWindow = CVideoControl::CreateWindow(this, VIDEOCONTROL, windowMain, this);
#else
     videoWindow = CVideoControlSoft::CreateWindow(this, VIDEOCONTROL, windowMain, this);
#endif
	if (config != nullptr)
	{
		CThemeSlider theme;
		config->GetVideoSliderTheme(&theme);
	}
	videoSlider = new CSliderVideo(this, wxID_ANY, this, themeSlider);
	play = false;

	Connect(wxEVT_SIZE, wxSizeEventHandler(CShowVideo::OnSize));

	this->windowMain = windowMain;
}

CVideoControlInterface * CShowVideo::GetVideoControl()
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
	case VOLUMEUPBUTTONID:
		videoWindow->VolumeUp();
		videoSlider->SetVolumePos(videoWindow->GetVolume());
		break;
	case VOLUMEDOWNBUTTONID:
		videoWindow->VolumeDown();
		videoSlider->SetVolumePos(videoWindow->GetVolume());
		break;
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
	delete(videoSlider);
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
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, VIDEO_END_ID);
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
        wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, VIDEO_START);
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
	videoSlider->SetTotalSecondTime(videoWindow->GetDuration());
	videoSlider->SetPastSecondTime(0);
	ShowSlider(false);
	PlayVideo();
	videoSlider->SetVolumePos(videoWindow->GetVolume());
#ifdef __APPLE__
    videoSlider->CallRefresh(videoSlider);
#else
	videoSlider->Refresh();
#endif
}

void CShowVideo::OnPositionVideo(const int64_t &position)
{
	if (videoSlider != nullptr)
		videoSlider->SetPastSecondTime(position);

	int videoPos = position / 1000;
	if (videoPos != videoPosOld)
	{
		wxWindow * viewerWindow = this->FindWindowById(VIEWERPICTUREWND);
		if (viewerWindow != nullptr)
		{
			wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, VIDEO_UPDATE_ID);
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

	//CLocalWindow * mainWindow = (CLocalWindow *)CWindowManagerEngine::getInstance()->GetWindow(L"MainWindow");
	//if (mainWindow != nullptr)
	//	mainWindow->OnCommand(WM_SETPOSITION, 0, position / 1000);
}

int CShowVideo::GetVideoWidth()
{
	int width = 0;
	if (videoWindow != nullptr)
		width = videoWindow->GetVideoWidth();
	return width;
}

int CShowVideo::GetVideoHeight()
{
	int height = 0;
	if (videoWindow != nullptr)
		height = videoWindow->GetVideoHeight();
	return height;
}

void CShowVideo::InitControl()
{
	if (videoWindow != nullptr)
	{
		Resize();
	}
}

bool CShowVideo::SetVideo(const wxString &filename, const int &rotation)
{
	videoPosOld = 0;
	bool value = false;
	if (videoWindow != nullptr && videoSlider != nullptr)
	{
		if (play)
			PauseVideo();

		value = videoWindow->PlayMovie(filename);

	}
	return value;
}

void CShowVideo::UpdateScreenRatio()
{
    videoSlider->UpdateScreenRatio();
    videoWindow->UpdateScreenRatio();
    this->Resize();
}

void CShowVideo::StopVideo()
{
	play = false;
	ShowSliderToolbar(true);
	videoSlider->SetPastSecondTime(0);
	videoWindow->OnStop();
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
	}
}

void CShowVideo::HideToolbar()
{
	if (videoSlider != nullptr)
	{
		if (!videoSlider->IsMouseOver())
		{
			if (videoSlider->IsShown())
				videoSlider->Show(false);
		}
	}
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
		videoSlider->SetSize(0, 0, 0, 0);
		//videoSlider->SendSizeEvent();
		videoWindow->GetWindow()->SetSize(0, 0, width, height);
		//videoWindow->SendSizeEvent();
	}
	else if(videoSlider != nullptr)
	{
		//CDeferPos deferpos;
		int posHeight = 0;
		if (videoSlider->IsShown())
			posHeight += videoSlider->GetHeight();

		int positionHeight = height - posHeight;

		videoWindow->GetWindow()->SetSize(0, 0, width, positionHeight);
		//videoWindow->SendSizeEvent();

		if (videoSlider->IsShown())
		{
			videoSlider->SetSize(0, positionHeight, width, videoSlider->GetHeight(), TRUE);
			//videoSlider->SendSizeEvent();
		}
	}
    else
    {
		videoWindow->GetWindow()->SetSize(0, 0, width, height);
		//videoWindow->SendSizeEvent();        
    }
}

void CShowVideo::OnSize(wxSizeEvent& event)
{
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	Resize();
}

