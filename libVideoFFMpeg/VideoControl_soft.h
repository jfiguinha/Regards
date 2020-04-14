#pragma once
#include "VideoControlInterface.h"
#ifdef RENDEROPENGL  
#include "WindowOpenGLMain.h"
#endif
#include "WindowMain.h"
#include "EffectVideoParameter.h"
#include "VideoInterface.h"
#include <BitmapYUV.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <OpenCLContext.h>
#include <OpenCLEngine.h>
#include <OpenCLEffectVideoYUV.h>
#include "RenderBitmapInterfaceOpenGL.h"
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
	int PlayMovie(const wxString &movie, const bool &play);

	int ChangeAudioStream(int newStreamAudio);
	int ChangeSubtitleStream(int newStreamSubtitle);
    int getWidth();
    int getHeight();

#ifdef WIN32
	virtual void SetDXVA2Compatible(const bool &compatible) {};
	virtual bool GetDXVA2Compatible() {
		return false;
	};
	virtual HRESULT InitVideoDevice(char * hwaccel_device, DXVA2Context * ctx, const int &width, const int &height)
	{
		return S_OK;
	};
#endif

    wxWindow * GetWindow()
    {
        return this;
    }
    
    static  CVideoControlSoft * CreateWindow(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer);

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
	virtual void SetData(void * data, const float & sample_aspect_ratio, void * WIN32Context);
    void UpdateScreenRatio();

	void Rotate90();
	void Rotate270();
	void FlipVertical();
	void FlipHorizontal();

protected:

    void OnRefresh(wxCommandEvent& event);
	bool GetProcessEnd();
	void EndVideoThread(wxCommandEvent& event);
	virtual void OnPaint(wxPaintEvent &event);
	void OnRButtonDown(wxMouseEvent& event);
	void VideoRotation(wxCommandEvent& event);
	void OnIdle(wxIdleEvent& evt);
	void OnShowFPS(wxTimerEvent& event);
    void Resize();
	void calculate_display_rect(wxRect *rect, int scr_xleft, int scr_ytop, int scr_width, int scr_height);
	GLTexture * RenderToGLTexture();
	GLTexture * RenderToTexture(CRegardsBitmap * bitmap);
	GLTexture * RenderToTexture(COpenCLEffectVideo * openclEffect);
	GLTexture * RenderFFmpegToTexture();

	bool IsCPUContext();
	void SetFrameData(AVFrame * src_frame);
	void CopyFrame(AVFrame * frame);

	bool subtilteUpdate;
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

	mutex muBitmap;
	mutex muVideoEffect;
	mutex muSubtitle;


	CRegardsBitmap * bitmap = nullptr;
	COpenCLEffectVideoYUV * openclEffectYUV = nullptr;
	CffmpegToBitmap * ffmpegToBitmap = nullptr;
#ifdef RENDEROPENGL
	COpenCLEngine * openCLEngine = nullptr;
	COpenCLContext * openclContext = nullptr;
	CRenderBitmapInterfaceOpenGL * renderBitmapOpenGL;
#endif
	CVideoEffectParameter videoEffectParameter;
	float video_aspect_ratio;
	int widthVideo;
	int heightVideo;
	int angle;
	bool flipV;
	bool flipH;
	bool oldBicubic = 0;
	AVFrame * copyFrameBuffer = nullptr;
	int isCPU = -1;
	bool isffmpegDecode = false;
	bool deleteTexture = false;
	int nbFrame;
	wxString message;
	CRegardsBitmap * pictureSubtitle;
	int videoPosition;
	bool updateContext = true;
};
