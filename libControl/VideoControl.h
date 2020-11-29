#pragma once
#ifdef WIN32
#include "VideoControl_soft.h"
#include "ffmpeg_dxva2.h"
#include "OpenCLEffectVideoNV12.h"
#include <d3d9.h>

class CVideoControl : public CVideoControlSoft
{
public:
	CVideoControl(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer);
	~CVideoControl();
    
    static CVideoControlSoft * CreateWindow(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer);
   
	bool GetDXVA2HardwareCompatible();
	void SetDXVA2Compatible(const bool &compatible);
	
#ifdef RENDEROPENGL  
	GLTexture * RenderFromOpenGLTexture();
#endif
	IDirect3D9Ex * GetDirectd3d9();
	HMODULE GetDXVA2Lib();
	void * GetDXVA2CreateDirect3DDeviceManager9();
	HRESULT CreateDevice(char * hwaccel_device);
	IDirect3DDevice9Ex * GetDirect3DDevice();
	HRESULT InitVideoDevice(char * hwaccel_device, DXVA2Context * ctx, const int &width, const int &height);
	
    virtual void SetData(void * data, const float & sample_aspect_ratio, void * WIN32Context);
	CRegardsBitmap * SavePicture();

protected:

    virtual void OnPaint(wxPaintEvent& event);

    

#ifdef RENDEROPENGL 
	bool UnbindTexture();
#endif

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
	COpenCLEffectVideoNV12 * openclEffectNV12;
};

#endif
