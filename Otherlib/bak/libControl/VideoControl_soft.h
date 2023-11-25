#pragma once
#include "VideoControlInterface.h"
#include <OpenCLEffectVideo.h>
#include <VideoStabilization.h>

extern "C" {
#include <libswscale/swscale.h>
#include <ffplay_struct.h>
}

#include "WindowMain.h"
#include "EffectVideoParameter.h"
#include "VideoInterface.h"
#include <ParamInit.h>
#include "RenderVideoOpenGL.h"
#include <wx/glcanvas.h>
#include <IBitmapRenderInterface.h>

using namespace Regards::Window;
using namespace Regards::Video;
using namespace Regards::OpenCL;
using namespace Regards::OpenCV;
using namespace Regards::OpenGL;
class CFFmfc;

class Chqdn3d;

namespace Regards
{
	namespace Video
	{
		class CThumbnailVideo;
	}

	namespace OpenCV
	{
		class COpenCVStabilization;
	}
}

class CVideoControlSoft : public IBitmapRenderInterface
{
public:
	CVideoControlSoft(CWindowMain* windowMain, wxWindow * window, IVideoInterface* eventPlayer);
	~CVideoControlSoft() ;
	void SetOpenCLOpenGLInterop(const bool& openclOpenGLInterop) ;
	void SetParent(wxWindow* parent) ;
	void ReloadResource();
	bool IsPause();
	
	void SetCurrentclock(wxString message);

	//void SetVideoPosition(const int64_t & pos);
	//void VolumeUp() ;
	//void VolumeDown() ;
	int GetVolume() ;
	void SetVolume(const int& pos);
	void ChangeVideoFormat();
	int GetZoomIndex();
	void SetZoomIndex(const int& pos);
	void ShrinkVideo();
	void RepeatVideo();
	void DiaporamaMode(const bool& value);
	vector<int> GetZoomValue();
	virtual cv::Mat SavePicture(bool& isFromBuffer);
	bool IsFFmpegDecode();
	void OnVideoStart(wxCommandEvent& event);
	void SetVideoPreviewEffect(CEffectParameter* effectParameter);
	void UpdateFiltre(CEffectParameter* effectParameter);
	CEffectParameter* GetParameter();
	void OnPlay();
	void OnStop(wxString photoName);
	void OnPause();
	int PlayMovie(const wxString& movie, const bool& play);
	int ChangeAudioStream(int newStreamAudio) ;
	int ChangeSubtitleStream(int newStreamSubtitle);
	int getWidth() ;
	int getHeight() ;

	void SetSubtitulePicture(cv::Mat& picture) ;
	void DeleteSubtitulePicture() ;

	bool GetPausedValue();
	void RedrawFrame();
	void SetRotation(const int& rotation) ;
	
	void UpdateScreenRatio() ;

	//Function to export to ffplay

	static void SetData(void* data, void * parent, float sample_aspect_ratio);
	static void SetPos(void* parent, int64_t pos);
	static void SetVideoDuration(void* parent, int64_t duration, int64_t startTime);
	static void VideoStart(void* parent, int64_t duration, int64_t startTime);
	static void VideoEnd(void* parent);
	/// <summary>
	/// 
	/// </summary>

	void Rotate90();
	void Rotate270();
	void FlipVertical();
	void FlipHorizontal();
	void PlayFirstMovie(const bool& firstMovie);

	void OnIdle(wxIdleEvent& evt) ;
	void OnPaint3D(wxGLCanvas* canvas, CRenderOpenGL* renderOpenGL) ;
	void OnPaint2D(wxWindow* gdi) ;
	void OnMouseMove(wxMouseEvent& event) ;
	void OnLButtonDown(wxMouseEvent& event) ;
	void OnRButtonDown(wxMouseEvent& event) ;
	void OnLButtonUp(wxMouseEvent& event) ;

	void OnLDoubleClick(wxMouseEvent& event) 
	{
	};

	void OnMouseWheel(wxMouseEvent& event) 
	{
	};
	void OnKeyDown(wxKeyEvent& event) ;
	void OnKeyUp(wxKeyEvent& event) ;

	void OnTimer(wxTimerEvent& event) ;
	void OnCommand(wxCommandEvent& event) ;
	vector<int> GetListTimer() ;
	vector<int> GetListCommand() ;
	void SetEndProgram(const bool& endProgram) ;
	int UpdateResized() ;
    
    wxString GetAcceleratorHardware();

protected:
	void OnSetPosition(wxCommandEvent& event);
	void OnLeftPosition(wxCommandEvent& event);
	void OnTopPosition(wxCommandEvent& event);
	void EndVideoThread(wxCommandEvent& event);
	void VideoRotation(wxCommandEvent& event);
	void OnScrollMove(wxCommandEvent& event);
	void OnPauseMovie(wxCommandEvent& event);
	void OnVideoDuration(wxCommandEvent& event);
	void OnShowFPS(wxTimerEvent& event);
	void OnPlayStart(wxTimerEvent& event);
	void OnPlayStop(wxTimerEvent& event);
	void OnSetPos(wxCommandEvent& event);
	void OnSetData(wxCommandEvent& event);

    void ErrorDecodingFrame();
    
	void CalculPositionPicture(const float& x, const float& y);
	static void GenerateThumbnailVideo(void* data);
	int IsSupportOpenCL();

	bool ApplyOpenCVEffect(cv::Mat& image);
	bool IsHardwareCompatible();
	float GetHauteurMax();
	float GetLargeurMax();
	void CalculTextureSize(int& widthOut, int& heightOut);
	void UpdateScrollBar();
	void MoveTop();
	void MoveLeft();
	void MoveBottom();
	void MoveRight();


	bool GetProcessEnd() ;

	void Resize() ;
	void calculate_display_rect(wxRect* rect, int scr_xleft, int scr_ytop, int scr_width, int scr_height);
	void RenderToGLTexture();
	void RenderToTexture(COpenCLEffectVideo* openclEffect);
	//GLTexture* RenderFFmpegToTexture();

	void ZoomOn();
	void ZoomOut();
	void CalculCenterPicture();
	bool IsCPUContext();
	void SetFrameData(AVFrame* src_frame);

	int GetBitmapWidth();
	int GetBitmapHeight();
	void MouseClick(const int& xPos, const int& yPos);
	void MouseRelease(const int& xPos, const int& yPos);

	void TestMaxX();
	void TestMaxY();
	void RenderFFmpegToTexture(cv::Mat& source);

	void StopVideoThread(wxCommandEvent& event);
	float CalculRatio(const int& pictureWidth, const int& pictureHeight);
	void SetSoundVolume(const int& soundVolume);
	int GetSoundVolume();
	float GetZoomRatio();
	void CalculRectPictureInterpolation(wxRect& rc, int& widthInterpolationSize, int& heightInterpolationSize,
	                                    int& left, int& top, const bool& invertY, const bool& invertX);
	void CalculPositionVideo(int& widthOutput, int& heightOutput, wxRect& rc);

	int GetSrcBitmapWidth();
	int GetSrcBitmapHeight();
	float GetMovieRatio();
	Chqdn3d* hq3d = nullptr;
	cv::Mat GetBitmapRGBA(AVFrame* tmp_frame);
    int Play(const wxString& movie);

	bool openclOpenGLInterop = false;
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
	mutex muVideoRender;
	mutex muSubtitle;
    
    bool isHardwareDecoder = true;

	bool videoRender = false;;
	bool videoStartRender = false;
	thread* _threadVideo = nullptr;
	bool threadVideoEnd = true;
	GLTexture* glTextureSrc = nullptr;
	cv::Mat bitmap;
	COpenCLEffectVideo* openclEffectYUV = nullptr;
	CRenderVideoOpenGL* renderBitmapOpenGL;
	CRenderOpenGL* renderOpenGL = nullptr;
	wxWindow * window;
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
	cv::Mat pictureSubtitle;
	cv::Mat pictureFrame;
	//cv::Mat pictureVideo;
	int64_t videoPosition = 0;
	int64_t oldvideoPosition = 0;
	bool updateContext = true;
	bool controlKeyPush = false;
	float ratioVideo = 1.0f;
	int posLargeur = 0;
	int posHauteur = 0;
	bool shrinkVideo = false;
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
	int oldLevelDenoise = 4;
	int oldwidthDenoise = 0;
	int oldheightDenoise = 0;
	bool isInit = false;
	bool firstMovie = true;
	wxTimer* playStopTimer;
	
	bool needToRefresh = false;
	std::mutex muRefresh;

	bool inverted = true;
	cv::Mat previousFrame;
	COpenCVStabilization* openCVStabilization = nullptr;
	SwsContext* localContext = nullptr;
	wxWindow* parentRender = nullptr;
	bool endProgram = false;
	wxString colorRange = "";
	wxString colorSpace = "";
	uint8_t* src = nullptr;
	int sizesrc = 0;
	ListFunction functionList;
    bool startVideoAfterProblem = false;
};
