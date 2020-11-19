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
#include "RenderVideoOpenGL.h"
using namespace Regards::Window;
using namespace Regards::Video;
using namespace Regards::OpenCL;
using namespace Regards::OpenGL;
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
	void SetVolume(const int &pos);
	void ChangeVideoFormat();
	int GetZoomIndex();
	void SetZoomIndex(const int &pos);
	void ShrinkVideo();
	vector<int> GetZoomValue();
	virtual CRegardsBitmap * SavePicture();
    bool IsFFmpegDecode();
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
	virtual bool GetDXVA2HardwareCompatible()
	{
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

	float GetHauteurMax();
	float GetLargeurMax();
	void ExportPicture(CRegardsBitmap * bitmap);

	void CalculTextureSize(int &widthOut, int &heightOut);
	void UpdateScrollBar();
	void MoveTop();
	void MoveLeft();
	void MoveBottom();
	void MoveRight();
	void OnKeyUp(wxKeyEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnLeftPosition(wxCommandEvent& event);
	void OnTopPosition(wxCommandEvent& event);
	void CalculPositionPicture(const float &x, const float &y);

    int IsSupportOpenCL();
    void OnRefresh(wxCommandEvent& event);
	bool GetProcessEnd();
	void EndVideoThread(wxCommandEvent& event);
	virtual void OnPaint(wxPaintEvent &event);
	void OnRButtonDown(wxMouseEvent& event);
	void VideoRotation(wxCommandEvent& event);
	void OnIdle(wxIdleEvent& evt);
	void OnShowFPS(wxTimerEvent& event);
	void OnPlayStart(wxTimerEvent& event);
    void Resize();
	void calculate_display_rect(wxRect *rect, int scr_xleft, int scr_ytop, int scr_width, int scr_height);
	GLTexture * RenderToGLTexture();
	GLTexture * RenderToTexture(CRegardsBitmap * bitmap);
	GLTexture * RenderToTexture(COpenCLEffectVideo * openclEffect);
	GLTexture * RenderFFmpegToTexture();
	void OnScrollMove(wxCommandEvent& event);
	void ZoomOn();
	void ZoomOut();
	void CalculCenterPicture();
	bool IsCPUContext();
	void SetFrameData(AVFrame * src_frame);
	void CopyFrame(AVFrame * frame);
	int GetBitmapWidth();
	int GetBitmapHeight();
	void MouseClick(const int &xPos, const int &yPos);
	void MouseRelease(const int &xPos, const int &yPos);
	void OnMouseMove(wxMouseEvent& event);
	void OnLButtonDown(wxMouseEvent& event);
	void OnLButtonUp(wxMouseEvent& event);
	void TestMaxX();
	void TestMaxY();
	GLTexture * DisplayTexture(GLTexture * glTexture);
	void StopVideoThread(wxCommandEvent& event);

	float CalculPictureRatio(const int &pictureWidth, const int &pictureHeight);
	void SetSoundVolume(const int &soundVolume);
	int GetSoundVolume();
	float GetZoomRatio();

	int mouseScrollX = 0;
	int mouseScrollY = 0;
	bool mouseBlock = false;
	float centerX = 0;
	float centerY = 0;
	bool subtilteUpdate;
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
	wxTimer * playStartTimer;
	bool initStart;
	bool videoRenderStart;
	wxString standByMovie;
	CFFmfc * ffmfc;
	wxCursor hCursorHand;
	mutex muBitmap;
	mutex muVideoEffect;
	mutex muSubtitle;

	GLTexture * glTextureSrc = nullptr;
	CRegardsBitmap * bitmap = nullptr;
	COpenCLEffectVideoYUV * openclEffectYUV = nullptr;
	CffmpegToBitmap * ffmpegToBitmap = nullptr;
#ifdef RENDEROPENGL
	COpenCLEngine * openCLEngine = nullptr;
	COpenCLContext * openclContext = nullptr;
	CRenderVideoOpenGL * renderBitmapOpenGL;
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
	bool controlKeyPush = false;
	float ratioVideo = 1.0f;
	int posLargeur = 0;
	int posHauteur = 0;
	bool displaywithInterpolation = false;
	bool shrinkVideo = false;
	//bool inverted = true;
	int oldWidth = 0;
	int oldHeight = 0;
	bool startVideo = false;
	bool processVideoEnd = false;
};
