#pragma once
#include "VideoControlInterface.h"
#ifdef RENDEROPENGL  
#include "WindowOpenGLMain.h"
#else
#include "WindowMain.h"
#endif
#include "EffectVideoParameter.h"
#include "VideoInterface.h"
#include <BitmapYUV.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
using namespace Regards::Window;
using namespace Regards::Video;
using namespace Regards::OpenCL;

class CFFmfc;

#ifdef RENDEROPENGL  
class CVideoControlSoft : public CWindowOpenGLMain, public CVideoControlInterface
#else
class CVideoControlSoft : public CWindowMain, public CVideoControlInterface
#endif
{
public:
	CVideoControlSoft(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer);
	~CVideoControlSoft();

	void SetVideoDuration(int64_t duration);
	void SetCurrentclock(wxString message);
	void SetPos(int64_t pos);
	void SetVideoPosition(int64_t pos);
	//static void PlayVideo(CVideoControlSoft * sdlWindow);
	void VolumeUp();
	void VolumeDown();
	int GetVolume();

	void VideoStart(wxCommandEvent& event);
	void SetVideoPreviewEffect(CEffectParameter * effectParameter);
	void UpdateFiltre(CEffectParameter * effectParameter);
	CEffectParameter * GetParameter();
	void OnPlay();
	void OnStop(wxString photoName);
	void OnPause();
	int PlayMovie(const wxString &movie);

	int ChangeAudioStream(int newStreamAudio);
	int ChangeSubtitleStream(int newStreamSubtitle);
    int getWidth();
    int getHeight();

    wxWindow * GetWindow()
    {
        return this;
    }
    
    static  CVideoControlInterface * CreateWindow(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer);

	void SetSubtitulePicture(CRegardsBitmap * picture);
	void DeleteSubtitulePicture();
	bool GetPausedValue()
	{
		return pause;
	};

    void RedrawFrame()
    {
        this->Refresh();
    }
    void SetRotation(const int &rotation);
    void SetVideoStart();
	void SetData(void * data, const float & sample_aspect_ratio, void * WIN32Context);
    void UpdateScreenRatio();

private:
	bool GetProcessEnd();
	void EndVideoThread(wxCommandEvent& event);
	void OnPaint(wxPaintEvent &event);
	void OnRButtonDown(wxMouseEvent& event);
	void VideoRotation(wxCommandEvent& event);
	void OnIdle(wxIdleEvent& evt);
	void OnShowFPS(wxTimerEvent& event);
    void Resize();
    
	bool subtilteUpdate;
	//thread * threadVideo;
	int volumeStart;
	int old_width;
	int old_height;
	bool pause;
	wxString filename;
	IVideoInterface * eventPlayer;
	CRegardsConfigParam * config;	
    bool newVideo ;
	bool videoEnd ;
	bool stopVideo;
	bool exit ;
	bool quitWindow;
    bool videoStart;
	wxString msgFrame;
	CWindowMain * windowMain;
	wxTimer * fpsTimer;
	bool initStart;
	bool videoRenderStart;
	wxString standByMovie;
	CFFmfc * ffmfc;
};
