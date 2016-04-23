#pragma once
#include <SDL.h>
#include "WindowMain.h"
#include <RegardsBitmap.h>
#include <mutex>
#include <thread>
#include <vector>
#include <string>
#include <GLSLShader.h>
#include "EffectVideoParameter.h"
#include "VideoInterface.h"
#include <wx/glcanvas.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include "VideoWindowContext.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Video;
using namespace Regards::OpenGL;
#define PAUSE 1
#define PLAY 2
#define STOP 3
#define EVENT_VIDEOSTART 1001
#define EVENT_REFRESHSCREEN 1002

namespace Regards
{
    namespace Window
    {
class CVideoControl : public wxGLCanvas
{
public:
	CVideoControl(wxWindow* parent, wxWindowID id, CVideoEffectParameter * videoEffectParameter, IVideoInterface * eventPlayer);
	~CVideoControl();
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
	void SetVideoDuration(int64_t duration);
	void SystemClear(){};
	void SetPos(int64_t pos);
	void SetVideoPosition(int64_t pos);
	static void PlayVideo(CVideoControl * sdlWindow);
	void VolumeUp();
	void VolumeDown();
	int GetVolume();
	void Rotate90();
	void Rotate270();
	void FlipVertical();
	void FlipHorizontal();
	void VideoStart(wxCommandEvent& event);

	void OnPlay();
	void OnStop();
	void OnPause();
	int PlayMovie(const wxString &movie);
	int ChangeAudio(const wxString &langue){ return 0; };

	int GetVideoWidth(){ return 0; };
	int GetVideoHeight(){ return 0; };
	int64_t GetDuration(){ return 0; };
	int GetState(){ return 0; };

    int getWidth();
    int getHeight();
    

	bool GetPausedValue()
	{
		return pause;
	};

	void RefreshScreen(wxCommandEvent& event);
    
    void UpdateScreenRatio();
	//SDL_Surface *screen;
private:

	SDL_Rect calculate_display_rect(float aspect_ratio, int angle);

	CRegardsBitmap * Convert(wxImage imageToDisplay);
	wxImage RenderOpenGL();
	void EndVideoThread(wxCommandEvent& event);
	void OnPaint(wxPaintEvent &event);
    void OnSize(wxSizeEvent& event);
    CVideoWindowContext& GetContext(wxGLCanvas *canvas);
    void CreateSourceTexture();
	void DrawBitmap();
	
    std::mutex muVideo;
	std::mutex muBitmap;
	std::thread * threadVideo = nullptr;
	
	int volumeStart = 64;
	wxImage bmp;
	CRegardsBitmap * bitmap = nullptr;
	int old_width = 0;
	int old_height = 0;
	bool pause = false;
	wxString filename;
	IVideoInterface * eventPlayer = nullptr;
	CVideoEffectParameter * videoEffectParameter = nullptr;
	//CRenderVideoInterfaceOpenGL * videoRenderOpenGL = nullptr;
	CRegardsConfigParam * config = nullptr;
	int angle = 0;
	bool flipV = false;
    bool newVideo = true;
	bool flipH = false;
	bool videoEnd = false;
	bool exit = false;
	bool openGLMode = false;
	bool openCLMode = false;
	bool quitWindow = false;
    bool videoStart = false;
    bool startVideoThread = false;
	float aspectRatio = 0.0;
    int textureWidth = 0;
    int textureHeight = 0;
    //OpenGL Texture
    GLuint m_nTextureID = 0;
    GLuint m_nTextureVideoID = 0;
    GLuint pboIds[1];                   // IDs of PBO
    GLSLShader * m_pShader = nullptr;
    CVideoWindowContext * m_glContext = nullptr;
};
    }
}
