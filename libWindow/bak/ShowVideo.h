#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "VideoControl.h"
#include "SliderVideo.h"
#include "SliderInterface.h"
#include "VideoInterface.h"
#include <ThemeParam.h>
using namespace Regards::Window;
using namespace Regards::Video;
class CVideoEffectParameter;

#define WM_SETPOSITION SDL_USER+1


class CShowVideo : public wxWindow, public CSliderInterface, public IVideoInterface
{
public:
	CShowVideo(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, CVideoEffectParameter * videoEffectParameter, CThemeParam * config);
	~CShowVideo();
	bool SetVideo(const wxString &filename);
	int GetVideoWidth();
	int GetVideoHeight();

	void SetPosition(const int64_t &timePosition);
	void SetVideoDuration(const int64_t &position);
	void SetTimePosition(const int64_t &timePosition);
			
	void ShowToolbar();
	void HideToolbar();
			
	void Resize();

	void PositionVideo(const int64_t &position);
	void SlidePosChange(const int &position, const wxString &key){};
	void ZoomPos(const int &position){};
	void MoveSlider(const int64_t &position);
	void ClickButton(const int &id);
	void SetTrackBarPosition(const int &iPos){};
	void Rotate90();
	void Rotate270();
	void FlipVertical();
	void FlipHorizontal();

	void StopVideo();
	void PlayVideo();
	void PauseVideo();
			
	void ChangeAudio(const wxString &langue);
	bool IsToolbarMouseOver();

	void SetDiaporamaMode();
	void SetNormalMode();
    
    void UpdateScreenRatio();

protected:

	//Interface Video
	void OnVideoEnd();
	void OnVideoStart();
	void OnVideoStop();
	void OnVideoPause();
	void OnAfterOpenVideo();
	void OnPositionVideo(const int64_t &position);

private:

	void OnSize(wxSizeEvent& event);
	//bool MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result);

	void ShowSliderToolbar(const bool &show);
	void ShowSlider(const bool &show);
	void InitControl();

	CSliderVideo * videoSlider = nullptr;
	CVideoControl * videoWindow = nullptr;
	CVideoEffectParameter * videoEffectParameter = nullptr;
	CWindowMain * windowMain = nullptr;
	bool play;
	bool toolbarOutside = false;
	int height = 0;
	int width = 0;
	int videoPosOld = 0;
	bool isDiaporama = false;
};
