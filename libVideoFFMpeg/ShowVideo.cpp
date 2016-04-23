#include "ShowVideo.h"
#include "SliderVideo.h"
#include "VideoControl.h"
#include <ThemeInit.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Video;
using namespace Regards::Window;

#define WM_SETPOSITION SDL_USER+1

CShowVideo::CShowVideo(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, CVideoEffectParameter * videoEffectParameter, CThemeParam * config)
	: wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0)
{
	CThemeSlider themeSlider;
	this->videoEffectParameter = videoEffectParameter;
	CTheme themeVideo;
		
	if (config != nullptr)
	{
		config->GetVideoControlTheme(&themeVideo);
	}

	videoWindow = new CVideoControl(this, wxID_ANY, videoEffectParameter, this);
	
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

void CShowVideo::ChangeAudio(const wxString &langue)
{
	videoWindow->ChangeAudio(langue);
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
}

void CShowVideo::SetVideoDuration(const int64_t &position)
{
	if (videoWindow != nullptr && videoSlider != nullptr)
	{
		videoSlider->SetTotalSecondTime(position);
	}
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
}

void CShowVideo::OnPositionVideo(const int64_t &position)
{
	if (videoSlider != nullptr)
		videoSlider->SetPastSecondTime(position);

	int videoPos = position / 1000;
	if (videoPos != videoPosOld)
	{
		wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, VIDEO_UPDATE_ID);
		event.SetInt(videoPos);
		windowMain->GetEventHandler()->AddPendingEvent(event);
		videoPosOld = videoPos;
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
		videoSlider->SetPastSecondTime(timePosition);
		videoSlider->UpdatePositionEvent();
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

bool CShowVideo::SetVideo(const wxString &filename)
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
		videoWindow->SetSize(0, 0, width, height);
	}
	else if(videoSlider != nullptr)
	{
		//CDeferPos deferpos;
		int posHeight = 0;
		if (videoSlider->IsShown())
			posHeight += videoSlider->GetHeight();

		int positionHeight = height - posHeight;

		videoWindow->SetSize(0, 0, width, positionHeight);

		if (videoSlider->IsShown())
		{
			videoSlider->SetSize(0, positionHeight, width, videoSlider->GetHeight(), TRUE);
			//positionHeight += videoSlider->GetHeight();
		}
	}
}

void CShowVideo::OnSize(wxSizeEvent& event)
{
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	Resize();
}

