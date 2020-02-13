#pragma once
#ifdef WIN32
#include "ffmpeg_dxva2.h"
#endif
extern "C"
{
    #include "libavutil/frame.h"
}
#include "WindowMain.h"
#include "VideoInterface.h"
#include "EffectParameter.h"
#include <OpenCLContext.h>
#include <OpenCLEngine.h>
#include <OpenCLEffectVideoYUV.h>
#include "RenderBitmapInterfaceOpenGL.h"
using namespace Regards::Window;
using namespace Regards::Video;
using namespace Regards::OpenCL;
class CRegardsBitmap;
class CffmpegToBitmap;


#define PAUSE 1
#define PLAY 2
#define STOP 3
#define EVENT_VIDEOSTART 1001
#define EVENT_VIDEOSTOP 1003
#define EVENT_REFRESHSCREEN 1002

class CVideoControlInterface
{
public:
    CVideoControlInterface()
    {
        
    };
    virtual ~CVideoControlInterface()
    {
        
    };
    virtual void RedrawFrame() = 0;
    virtual void SetSubtitulePicture(CRegardsBitmap * picture) = 0;
    virtual void DeleteSubtitulePicture() = 0;
    virtual void SetPos(int64_t pos) = 0;
    virtual void SetData(void * data, const float & sample_aspect_ratio, void * WIN32Context) = 0;
    virtual void SetRotation(const int &rotation) = 0;
    virtual void SetVideoDuration(int64_t duration) = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;    
    virtual void SetVideoStart() = 0;
    virtual wxWindow * GetWindow() = 0;
    virtual void SetVideoPreviewEffect(CEffectParameter * effectParameter) = 0;
    virtual CEffectParameter * GetParameter() = 0;
    virtual void UpdateFiltre(CEffectParameter * effectParameter) = 0;
    virtual int ChangeAudioStream(int newStreamAudio) = 0;
	virtual void VolumeUp() = 0;
	virtual void VolumeDown() = 0;    
    virtual int GetVolume() = 0;
    virtual void SetVideoPosition(int64_t pos) = 0;
    
	virtual void OnPlay() = 0;
	virtual void OnStop(wxString photoName) = 0;
	virtual void OnPause() = 0;
	virtual int PlayMovie(const wxString &movie, const bool &play) = 0;    
    

    
    virtual void UpdateScreenRatio() = 0;
    
	int GetVideoWidth(){ return 0; };
	int GetVideoHeight(){ return 0; };
	int64_t GetDuration(){ return 0; };  
    int GetState(){ return 0; };
	void Rotate90();
	void Rotate270();
	void FlipVertical();
	void FlipHorizontal();
  	void OnBnClickedStop(){};
	void SetFormatProtocol(string message){};
	void SetCodecResolution(wxString message){};
	void SetDecoderName(string message){};
	void SetFrameRate(string message){};
	void SetPixelFormat(string message){};
	void SetMetadata(vector<string> meta){};
	void SetInputFormat(string message){};
	void SetCodecName(string message){};
	void SetCodecSampleRate(string message){};
	void SetCodecChannel(string message){};
	void SetFormatBitrate(string message){};
	void SetFormatDuration(string message){};
	void SetBitrate(string message){};
    void SystemClear(){}; 
    bool IsCPUContext();
#ifdef WIN32
    virtual void SetDXVA2Compatible(const bool &compatible){};
    virtual bool GetDXVA2Compatible(){return false;};
    virtual HRESULT InitVideoDevice(char * hwaccel_device, DXVA2Context * ctx, const int &width, const int &height){ return S_OK;};
#endif

protected:
	void calculate_display_rect(wxRect *rect, int scr_xleft, int scr_ytop, int scr_width, int scr_height);
#ifdef RENDEROPENGL
    GLTexture * RenderToTexture(COpenCLEffectVideo * openclEffect);
    GLTexture * RenderToTexture(CRegardsBitmap * bitmap);
    GLTexture * RenderFFmpegToTexture();
#else
	CRegardsBitmap * GenerateBitmap(COpenCLEffectVideo * openclEffect);
	CRegardsBitmap * RenderToBitmap();
	CRegardsBitmap * RenderToBitmap(COpenCLEffectVideo * openclEffect);
#endif
    void CopyFrame(AVFrame * frame);
    void SetFrameData(AVFrame * frame);
#ifdef RENDEROPENGL
    GLTexture * RenderToGLTexture();
#endif
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