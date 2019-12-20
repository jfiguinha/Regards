#pragma once
#ifdef WIN32
#include "VideoControlInterface.h"
#ifdef RENDEROPENGL  
#include "WindowOpenGLMain.h"
#include "RenderBitmapInterfaceOpenGL.h"
#else
#include "WindowMain.h"
#endif
#include "EffectVideoParameter.h"
#include "VideoInterface.h"
#include <BitmapYUV.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include "ffmpeg_dxva2.h"
#include <OpenCLEffectVideoNV12.h>
#include <d3d9.h>

class CRegardsBitmap;

using namespace Regards::OpenCL;
using namespace std;
using namespace Regards::Window;
using namespace Regards::Video;

#ifdef RENDEROPENGL  
class CVideoControl : public CWindowOpenGLMain, public CVideoControlInterface
#else
class CVideoControl : public CWindowMain, public CVideoControlInterface
#endif
{
public:
	CVideoControl(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer);
	~CVideoControl();
    
    static CVideoControlInterface * CreateWindow(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer);

    wxWindow * GetWindow()
    {
        return this;
    }

    void RedrawFrame()
    {
        this->Refresh();
    }

	void SetVideoDuration(int64_t duration);
	void SetCurrentclock(wxString message);
	void SetPos(int64_t pos);
	void SetVideoPosition(int64_t pos);
	static void PlayVideo(CVideoControl * sdlWindow);
	void VolumeUp();
	void VolumeDown();
	int GetVolume();
	void VideoStart(wxCommandEvent& event);
	void SetVideoPreviewEffect(CEffectParameter * effectParameter);
	void UpdateFiltre(CEffectParameter * effectParameter);
	CEffectParameter * GetParameter();
	void OnPlay();
	void OnStop();
	void OnPause();
	int PlayMovie(const wxString &movie);
	int GetState(){ return 0; };
    void OnRefresh(wxCommandEvent& event);
	int ChangeAudioStream(int newStreamAudio);
	int ChangeSubtitleStream(int newStreamSubtitle);
    int getWidth();
    int getHeight();
	void SetSubtitulePicture(CRegardsBitmap * picture);
	void DeleteSubtitulePicture();
	bool GetPausedValue()
	{
		return pause;
	};
    
    void SetRotation(const int &rotation);
    void SetVideoStart();    

	void SetDXVA2Compatible(const bool &compatible);
	bool GetDXVA2Compatible();
#ifdef RENDEROPENGL  
	GLTexture * RenderFromOpenGLTexture();
#endif
	IDirect3D9Ex * GetDirectd3d9();
	HMODULE GetDXVA2Lib();
	void * GetDXVA2CreateDirect3DDeviceManager9();
	HRESULT CreateDevice(char * hwaccel_device);
	IDirect3DDevice9Ex * GetDirect3DDevice();
	HRESULT InitVideoDevice(char * hwaccel_device, DXVA2Context * ctx, const int &width, const int &height);
	
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
	thread * threadVideo;
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


	bool isDXVA2Compatible;

#ifdef RENDEROPENGL 
	bool UnbindTexture();
#endif
	COpenCLEffectVideoNV12 * openclEffectNV12;
	LPDIRECT3DSURFACE9 surface;
	DXVA2Context * dxva2;
	HANDLE hTexture;
	
    HMODULE d3dlib;
    HMODULE dxva2lib;
	IDirect3D9Ex *d3d9;
	IDirect3DDevice9Ex  *d3d9device;
	IDirect3DDeviceManager9     *d3d9devmgr;
	HANDLE  deviceHandle;
	IDirect3DSurface9*			m_pRenderTargetSurface;
	D3DSURFACE_DESC rtDesc;
	bool initDevice;
	int windowWidth;
	int windowHeight;
	bool dxva2ToOpenGLWorking;
	HANDLE hDevice;
};

#endif