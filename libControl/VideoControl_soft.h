#pragma once
#include "VideoControlInterface.h"
#include "WindowOpenGLMain.h"
#include <OpenCLContext.h>
#include <OpenCLEngine.h>
#include <OpenCLEffectVideoYUV.h>
#include <OpenCLEffectVideoNV12.h>
#include <VideoStabilization.h>

extern "C" {
#include <libswscale/swscale.h>
}

#include "WindowMain.h"
#include "EffectVideoParameter.h"
#include "VideoInterface.h"
#include <ParamInit.h>

#include "RenderVideoOpenGL.h"

using namespace Regards::Window;
using namespace Regards::Video;
using namespace Regards::OpenCL;
using namespace Regards::OpenCV;
using namespace Regards::OpenGL;
class CFFmfc;
class CThumbnailVideo;
class Chqdn3d;

class CVideoControlSoft : public CWindowOpenGLMain, public CVideoControlInterface
{
public:
	CVideoControlSoft(wxWindow* parent, wxWindowID id, CWindowMain* windowMain, IVideoInterface* eventPlayer);
	~CVideoControlSoft() override;
	COpenCLContext * GetOpenclContext();
	void ReloadResource();
	bool IsPause();
	void SetVideoDuration(const int64_t& duration, const int64_t& startTime) override;
	void SetCurrentclock(wxString message);
	void SetPos(int64_t pos) override;
	//void SetVideoPosition(const int64_t & pos);
	void VolumeUp() override;
	void VolumeDown() override;
	int GetVolume() override;
	void SetVolume(const int& pos);
	void ChangeVideoFormat();
	int GetZoomIndex();
	void SetZoomIndex(const int& pos);
	void ShrinkVideo();
	void RepeatVideo();
	void DiaporamaMode(const bool & value);
	vector<int> GetZoomValue();
	virtual CRegardsBitmap* SavePicture(bool& isFromBuffer);
	bool IsFFmpegDecode();
	void VideoStart(wxCommandEvent& event);
	void SetVideoPreviewEffect(CEffectParameter* effectParameter);
	void UpdateFiltre(CEffectParameter* effectParameter);
	CEffectParameter* GetParameter();
	void OnPlay();
	void OnStop(wxString photoName);
	void OnPause();
	int PlayMovie(const wxString& movie, const bool& play);
	int ChangeAudioStream(int newStreamAudio) override;
	int ChangeSubtitleStream(int newStreamSubtitle);
	int getWidth() override;
	int getHeight() override;

	wxWindow* GetWindow()
	{
		return this;
	}

	static CVideoControlSoft* CreateWindow(wxWindow* parent, wxWindowID id, CWindowMain* windowMain,
	                                       IVideoInterface* eventPlayer);

	void SetSubtitulePicture(CRegardsBitmap* picture) override;
	void DeleteSubtitulePicture() override;

	bool GetPausedValue()
	{
		return pause;
	};

	void RedrawFrame()
	{
		this->Refresh();
	}

	void SetRotation(const int& rotation) override;
	void SetData(void* data, const float& sample_aspect_ratio, void* WIN32Context) override;
	void UpdateScreenRatio() override;

	void Rotate90();
	void Rotate270();
	void FlipVertical();
	void FlipHorizontal();
	void PlayFirstMovie(const bool& firstMovie);
	void SetEncoderHardware(const wxString& encoderHardware, const bool& opengl);

protected:
	bool ApplyOpenCVEffect(CRegardsBitmap* pictureFrame);
	bool IsHardwareCompatible();
	float GetHauteurMax();
	float GetLargeurMax();
	void CalculTextureSize(int& widthOut, int& heightOut);
	void UpdateScrollBar();
	void MoveTop();
	void MoveLeft();
	void MoveBottom();
	void MoveRight();
	void OnSetPosition(wxCommandEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnLeftPosition(wxCommandEvent& event);
	void OnTopPosition(wxCommandEvent& event);
	void CalculPositionPicture(const float& x, const float& y);
	static void GenerateThumbnailVideo(void* data);
	int IsSupportOpenCL();
	void OnRefresh(wxCommandEvent& event) ;
	bool GetProcessEnd() override;
	void EndVideoThread(wxCommandEvent& event);
	virtual void on_paint(wxPaintEvent& event);
	void OnRButtonDown(wxMouseEvent& event);
	void VideoRotation(wxCommandEvent& event);
	void OnIdle(wxIdleEvent& evt);
	void OnShowFPS(wxTimerEvent& event);
	void OnPlayStart(wxTimerEvent& event);
	void Resize() override;
	void calculate_display_rect(wxRect* rect, int scr_xleft, int scr_ytop, int scr_width, int scr_height);
	GLTexture* RenderToGLTexture();
	GLTexture* RenderToTexture(CRegardsBitmap* bitmap);
	GLTexture* RenderToTexture(COpenCLEffectVideo* openclEffect);
	GLTexture* RenderFFmpegToTexture();
	void OnScrollMove(wxCommandEvent& event);
	void ZoomOn();
	void ZoomOut();
	void CalculCenterPicture();
	bool IsCPUContext();
	void SetFrameData(AVFrame* src_frame);

	int GetBitmapWidth();
	int GetBitmapHeight();
	void MouseClick(const int& xPos, const int& yPos);
	void MouseRelease(const int& xPos, const int& yPos);
	void OnMouseMove(wxMouseEvent& event);
	void OnLButtonDown(wxMouseEvent& event);
	void OnLButtonUp(wxMouseEvent& event);
	void TestMaxX();
	void TestMaxY();
	GLTexture* RenderFFmpegToTexture(CRegardsBitmap* source);
	GLTexture* DisplayTexture(GLTexture* glTexture);
	void StopVideoThread(wxCommandEvent& event);
	float CalculRatio(const int& pictureWidth, const int& pictureHeight);
	float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight);
	void SetSoundVolume(const int& soundVolume);
	int GetSoundVolume();
	float GetZoomRatio();
	void CalculRectPictureInterpolation(wxRect& rc, int& widthInterpolationSize, int& heightInterpolationSize,
	                                    int& left, int& top, const bool& invert);
	void CalculPositionVideo(int& widthOutput, int& heightOutput, wxRect& rc);
	int IsOpenGLDecoding();
	int GetSrcBitmapWidth();
	int GetSrcBitmapHeight();
	float GetMovieRatio();
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
	IVideoInterface* eventPlayer;
	CRegardsConfigParam* config;
	bool newVideo;
	bool videoEnd;
	bool stopVideo;
	bool isDiaporama = false;
	bool exit;
	bool quitWindow;
	bool videoStart;
	wxString msgFrame;
	CWindowMain* windowMain;
	wxTimer* fpsTimer;
	wxTimer* playStartTimer;
	bool initStart;
	bool videoRenderStart;
	wxString standByMovie;
	CFFmfc* ffmfc;
	wxCursor hCursorHand;
	mutex muBitmap;
	mutex muVideoEffect;
	mutex muSubtitle;
	thread* _threadVideo = nullptr;
	bool threadVideoEnd = true;
	GLTexture* glTextureSrc = nullptr;
	CRegardsBitmap* bitmap = nullptr;
	COpenCLEffectVideoYUV* openclEffectYUV = nullptr;
	COpenCLContext* openclContext = nullptr;
	CRenderVideoOpenGL* renderBitmapOpenGL;
	CVideoEffectParameter videoEffectParameter;
	float video_aspect_ratio;
	int widthVideo;
	int heightVideo;
	int angle;
	bool flipV;
	bool flipH;
	bool oldBicubic = false;
	bool repeatVideo = false;
	int isCPU = -1;
	bool isffmpegDecode = false;
	bool deleteTexture = false;
	int nbFrame;
	wxString message;
	CRegardsBitmap* pictureSubtitle;
	CRegardsBitmap* pictureFrame = nullptr;
	CRegardsBitmap* pictureVideo = nullptr;
	int64_t videoPosition = 0;
	int64_t oldvideoPosition = 0;
	bool updateContext = true;
	bool controlKeyPush = false;
	float ratioVideo = 1.0f;
	int posLargeur = 0;
	int posHauteur = 0;
	bool shrinkVideo = false;
	bool inverted = true;
	int oldWidth = 0;
	int oldHeight = 0;
	bool startVideo = false;
	bool processVideoEnd = false;
	bool openGLDecoding = true;
	wxString acceleratorHardware = "";
	bool isOpenGLDecoding = false;
	float startingTime = 0;
	CThumbnailVideo* thumbnailVideo;
	bool thumbnailFromBitmap = false;
	Chqdn3d* hq3d = nullptr;
	int oldLevelDenoise = 4;
	int oldwidthDenoise = 0;
	int oldheightDenoise = 0;
	void GetDenoiserPt(const int& width, const int& height);
	CRegardsBitmap* GetBitmapRGBA(AVFrame* tmp_frame);
	bool firstMovie = true;
	wxTimer* playStopTimer;
	void OnPlayStop(wxTimerEvent& event);
	CRegardsBitmap* previousFrame = nullptr;
	CRegardsBitmap* bitmapData = nullptr;
	COpenCVStabilization* openCVStabilization = nullptr;
	SwsContext* localContext = nullptr;
	//bool reloadResource = false;
};
