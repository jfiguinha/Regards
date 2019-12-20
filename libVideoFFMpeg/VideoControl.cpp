#include "header.h"

#ifdef WIN32

#include "VideoControl.h"
#include <wx/dcbuffer.h>
#include <RegardsFloatBitmap.h>
#include "ffplaycore.h"
#include <Interpolation.h>
#include "config_id.h"
#include <d3d9.h>
#include <CL/cl_dx9_media_sharing.h>
#include <CL/cl.h>
#include <utility.h>
#include "ffmpegToBitmap.h"
#include <RegardsBitmap.h>
#include <ImageLoadingFormat.h>
//#include "LoadingResource.h"
wxDEFINE_EVENT(TIMER_FPS,  wxTimerEvent);
wxDEFINE_EVENT(EVENT_ENDVIDEOTHREAD, wxCommandEvent);

#ifndef RENDEROPENGL
extern COpenCLEngine * openCLEngine;
extern COpenCLContext * openclContext;
#else
#ifdef GLUT
#include <GL/glut.h>
#endif
#include <CL/cl_gl.h>
#endif

CVideoControl::CVideoControl(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer)
#ifdef RENDEROPENGL  
: CWindowOpenGLMain("CVideoControl",parent, id)
#else
: CWindowMain("CVideoControl", parent, id)
#endif
{
#ifdef RENDEROPENGL 
	renderBitmapOpenGL = new Regards::Video::CRenderBitmapInterfaceOpenGL(this);
    renderBitmapOpenGL->Init(this);

#ifdef GLUT
	int argc = 1;
	char* argv[1] = { wxString((wxTheApp->argv)[0]).char_str() };
	glutInit(&argc, argv);
#endif

#endif
    printf("CVideoControl initialisation \n");


	hDevice = nullptr;
	hTexture = nullptr;
	windowWidth = 0;
	windowHeight = 0;
	dxva2ToOpenGLWorking = true;

	widthVideo = 0;
	heightVideo = 0;
	subtilteUpdate = false;
	threadVideo = nullptr;
	volumeStart = 64;
	old_width = 0;
	old_height = 0;
	pause = false;
	config = nullptr;
	angle = 0;
	flipV = false;
    newVideo = true;
	flipH = false;
	videoEnd = false;
	exit = false;
	quitWindow = false;
    videoStart = false;
    videoRenderStart = false;
	pictureSubtitle = nullptr;
	video_aspect_ratio = 0.0;
	config = CParamInit::getInstance();
    Connect(wxEVT_PAINT, wxPaintEventHandler(CVideoControl::OnPaint));
    Connect(wxEVT_SIZE, wxSizeEventHandler(CVideoControl::OnSize));
    Connect(EVENT_ENDVIDEOTHREAD, wxCommandEventHandler(CVideoControl::EndVideoThread));
    Connect(EVENT_VIDEOSTART, wxCommandEventHandler(CVideoControl::VideoStart));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CVideoControl::OnIdle));
	Connect(EVENT_VIDEOROTATION, wxCommandEventHandler(CVideoControl::VideoRotation));
    Connect(wxEVENT_REFRESH, wxCommandEventHandler(CVideoControl::OnRefresh));
    fpsTimer = new wxTimer(this, TIMER_FPS);
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CVideoControl::OnRButtonDown));
    Connect(TIMER_FPS, wxEVT_TIMER, wxTimerEventHandler(CVideoControl::OnShowFPS), nullptr, this);
	pause = false;
	videoEnd = true;
	this->windowMain = windowMain;
	this->eventPlayer = eventPlayer;

	initDevice = true;
	dxva2 = nullptr;
    d3d9devmgr = nullptr;
	d3d9device = nullptr;
	m_pRenderTargetSurface = nullptr;
	isDXVA2Compatible = true;
	openclEffectNV12 = nullptr;

    d3dlib = LoadLibrary(L"d3d9.dll");
    if (!d3dlib) {
        throw("Failed to load D3D9 library\n");
    }
    
	dxva2lib = LoadLibrary(L"dxva2.dll");
    if (!dxva2lib) {
        throw("Failed to load DXVA2 library\n");
    }

	Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d9);
    // ctx->d3d9 = createD3D(D3D_SDK_VERSION);
    if (!d3d9) {
        throw("Failed to create IDirect3D object\n");
    }

#ifdef RENDEROPENGL  
	openCLEngine = nullptr;
	openclContext = nullptr;
#endif
	openclEffectYUV = nullptr;
}

void CVideoControl::VideoRotation(wxCommandEvent& event)
{
	long rotation = event.GetExtraLong();
	if(rotation < 0)
		rotation = 360 + rotation;
	angle = rotation % 360;
}

void CVideoControl::UpdateFiltre(CEffectParameter * effectParameter)
{
	CVideoEffectParameter * videoParameter = (CVideoEffectParameter *)effectParameter;
	if(videoParameter->streamAudioUpdate)
	{
		ChangeAudioStream(videoParameter->streamAudioIndex);
		videoParameter->streamAudioUpdate = 0;
	}
	else if(videoParameter->streamVideoUpdate)
	{
		//ChangeAudioStream(videoParameter->streamAudioIndex);
		videoParameter->streamVideoUpdate = 0;
	}
	else if(videoParameter->streamSubtitleUpdate)
	{
		//ChangeAudioStream(videoParameter->streamAudioIndex);
		videoParameter->streamSubtitleUpdate = 0;
	}

	muVideoEffect.lock();
	videoEffectParameter = *videoParameter;
	muVideoEffect.unlock();

	//Refresh();
}

void CVideoControl::SetVideoPreviewEffect(CEffectParameter * effectParameter)
{
	CVideoEffectParameter * videoParameter = (CVideoEffectParameter *)effectParameter;
	muVideoEffect.lock();
	videoEffectParameter = *videoParameter;
	muVideoEffect.unlock();
}

CEffectParameter * CVideoControl::GetParameter()
{
	CVideoEffectParameter * videoParameter = new CVideoEffectParameter();
	muVideoEffect.lock();
	*videoParameter = videoEffectParameter;
	muVideoEffect.unlock();
	return videoParameter;
}


bool CVideoControl::GetProcessEnd()
{
	return videoEnd;
}

void CVideoControl::OnRefresh(wxCommandEvent& event)
{
    Refresh();
}

void CVideoControl::OnIdle(wxIdleEvent& evt)
{
    //TRACE();
	if(endProgram && videoRenderStart && !quitWindow)
	{
		fpsTimer->Stop();
		quitWindow = true;
		exit = true;
		if (!videoEnd)
		{
			if(ffmfc_quit())
			{
				wxCommandEvent localevent;
				EndVideoThread(localevent);
				//videoEnd = true;
			}
		}
	}
}

void CVideoControl::OnShowFPS(wxTimerEvent& event)
{
	msgFrame = wxString::Format("FPS : %d", nbFrame);
	nbFrame = 0;
}

void CVideoControl::PlayVideo(CVideoControl * sdlWindow)
{
    ffmfc_play(sdlWindow, CConvertUtility::ConvertToStdString(sdlWindow->filename));
    wxCommandEvent event(EVENT_ENDVIDEOTHREAD);
    wxPostEvent(sdlWindow, event);
}


void CVideoControl::EndVideoThread(wxCommandEvent& event)
{
    
    if(threadVideo != nullptr)
    {
        threadVideo->join();
        delete threadVideo;
        threadVideo = nullptr;
    }

    videoEnd = true;
    if (eventPlayer != nullptr)
    {
        eventPlayer->OnPositionVideo(0);
        eventPlayer->OnVideoEnd();
    }
	fpsTimer->Stop();
	videoRenderStart = false;
	stopVideo = true;

	if(standByMovie != "")
	{
		PlayMovie(standByMovie);
	}
}


CVideoControl::~CVideoControl()
{
	initDevice = true;

#ifdef RENDEROPENGL 
	UnbindTexture();
#endif
	if(m_pRenderTargetSurface)
		IDirect3DSurface9_Release(m_pRenderTargetSurface);

    if (d3d9devmgr && deviceHandle != INVALID_HANDLE_VALUE)
        d3d9devmgr->CloseDeviceHandle(deviceHandle);

    if (d3d9devmgr)
        d3d9devmgr->Release();

    if (d3d9device)
        IDirect3DDevice9_Release(d3d9device);

    if (d3d9)
        IDirect3D9_Release(d3d9);

    if (d3dlib)
        FreeLibrary(d3dlib);

    if (dxva2lib)
        FreeLibrary(dxva2lib);
        
#ifdef RENDEROPENGL 
	delete renderBitmapOpenGL;
#endif
	delete fpsTimer;

	if(openclEffectYUV != nullptr)
		delete openclEffectYUV;

	if(openclEffectNV12 != nullptr)
		delete openclEffectNV12;

#ifdef RENDEROPENGL  
	if (openCLEngine != nullptr)
		delete openCLEngine;
#endif
	openCLEngine = nullptr;

	if(pictureSubtitle != nullptr)
		delete pictureSubtitle;
}

void CVideoControl::SetSubtitulePicture(CRegardsBitmap * picture)
{
	muSubtitle.lock();

	if(pictureSubtitle != nullptr)
		delete pictureSubtitle;

	pictureSubtitle = picture;

	subtilteUpdate = true;

	muSubtitle.unlock();
}

void CVideoControl::DeleteSubtitulePicture()
{
	muSubtitle.lock();

	if(pictureSubtitle != nullptr)
		delete pictureSubtitle;

	pictureSubtitle = nullptr;

	subtilteUpdate = true;

	muSubtitle.unlock();
}



int CVideoControl::PlayMovie(const wxString &movie)
{
	if (videoEnd)
	{
		stopVideo = false;
		angle = 0;
		flipV = false;
		flipH = false;
        videoStart = false;
        newVideo = true;
		initStart = true;
		videoRenderStart = false;
        videoEnd = false;
        filename = movie;
		standByMovie = "";
		threadVideo = new thread(PlayVideo, this);
	}
	else if(movie != filename)
	{
		OnStop();
		standByMovie = movie;
	}

	return 0;
}

void CVideoControl::VideoStart(wxCommandEvent& event)
{
	eventPlayer->OnVideoStart();
    ffmfc_play();
	pause = false;
    videoStart =true;
}

int CVideoControl::getWidth()
{
     return GetSize().x;
}
int CVideoControl::getHeight()
{
    return GetSize().y;
}

void CVideoControl::UpdateScreenRatio()
{
    Refresh();
}

IDirect3D9Ex * CVideoControl::GetDirectd3d9()
{
	return d3d9;
}

void * CVideoControl::GetDXVA2CreateDirect3DDeviceManager9()
{
	return GetProcAddress(dxva2lib, "DXVA2CreateDirect3DDeviceManager9");;
}

HMODULE CVideoControl::GetDXVA2Lib()
{
	return dxva2lib;
}

typedef HRESULT WINAPI pCreateDeviceManager9(UINT *, IDirect3DDeviceManager9 **);

HRESULT CVideoControl::InitVideoDevice(char * hwaccel_device, DXVA2Context * ctx, const int &width, const int &height)
{
	HRESULT hr = S_OK;
	bool init = false;

	HDC screen = GetDC(0);
	RECT rcClip;
	GetClipBox(screen, &rcClip);
	ReleaseDC(0, screen);

	int widthSize = (width % 128);
	if (widthSize == 0)
		widthSize = width;
	else
		widthSize = (128 - widthSize) + width;

	int heightSize = height;


	//int heightSize = (((height)+31) & ~31);
	//size_t u2 = 1; while (u2 < width) u2 *= 2;
	//size_t v2 = 1; while (v2 < height) v2 *= 2;

	if(rcClip.right != windowWidth && rcClip.bottom != windowHeight)
	{
		windowHeight = rcClip.bottom;
		windowWidth = rcClip.right;

		if(d3d9device != nullptr)
		{
			if(m_pRenderTargetSurface)
				IDirect3DSurface9_Release(m_pRenderTargetSurface);

			if (d3d9devmgr && deviceHandle != INVALID_HANDLE_VALUE)
			{
				d3d9devmgr->CloseDeviceHandle(deviceHandle);
				deviceHandle = nullptr;
			}

			if (d3d9devmgr)
			{
				d3d9devmgr->Release();
				d3d9devmgr = nullptr;
			}

			if (d3d9device)
			{
				IDirect3DDevice9_Release(d3d9device);
				d3d9device = nullptr;
			}
		}

		CreateDevice(hwaccel_device);

		if(d3d9device != nullptr)
		{
			UINT adapter = D3DADAPTER_DEFAULT;
			unsigned resetToken = 0;	
			pCreateDeviceManager9 *createDeviceManager = NULL;

			createDeviceManager = (pCreateDeviceManager9 *)GetProcAddress(dxva2lib, "DXVA2CreateDirect3DDeviceManager9");
			if (!createDeviceManager) {
				throw("Failed to locate DXVA2CreateDirect3DDeviceManager9\n");
			}

			if (hwaccel_device) {
				adapter = atoi(hwaccel_device);
				//av_log(NULL, AV_LOG_INFO, "Using HWAccel device %d\n", adapter);
			}


			D3DDISPLAYMODE        d3ddm;
			IDirect3D9_GetAdapterDisplayMode(d3d9, adapter, &d3ddm);

			D3DPRESENT_PARAMETERS d3dpp = { 0 };
			d3dpp.Windowed = TRUE;
			d3dpp.BackBufferWidth = windowWidth;
			d3dpp.BackBufferHeight = windowHeight;
			d3dpp.BackBufferCount = 0;
			d3dpp.BackBufferFormat = d3ddm.Format;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.Flags = D3DPRESENTFLAG_VIDEO;

			hr = d3d9device->ResetEx(&d3dpp, NULL);
			if (FAILED(hr)) return hr;    

			hr = d3d9device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
			if (FAILED(hr)) return hr;    


			hr = d3d9device->GetRenderTarget(0, & m_pRenderTargetSurface);
    
			
			m_pRenderTargetSurface->GetDesc(&rtDesc);
    
			// g_pSharedSurface should be able to be opened in OGL via the WGL_NV_DX_interop extension
			// Vendor support for various textures/surfaces may vary
			hr = d3d9device->CreateOffscreenPlainSurface(widthSize,
														heightSize,
														rtDesc.Format, 
														D3DPOOL_DEFAULT, 
														&ctx->m_pSharedSurface, 
														& ctx->m_hSharedSurface);	

			// Since this demo only shows RGB->RGB conversion, verify that the hardware can do NV12->RGB conversion
			hr = d3d9->CheckDeviceFormatConversion(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (D3DFORMAT) MAKEFOURCC('N', 'V', '1', '2'), D3DFMT_X8R8G8B8);
			hr = d3d9device->SetRenderState(D3DRS_LIGHTING, FALSE);
			hr = d3d9device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
			hr = createDeviceManager(&resetToken, &d3d9devmgr);
			if (FAILED(hr)) {
				throw("Failed to create Direct3D device manager\n");
			}

			//hr = IDirect3DDeviceManager9_ResetDevice(ctx->d3d9devmgr, ctx->d3d9device, resetToken);
			hr = d3d9devmgr->ResetDevice(d3d9device, resetToken);
			if (FAILED(hr)) {
				throw("Failed to bind Direct3D device to device manager\n");
			}

			//hr = IDirect3DDeviceManager9_OpenDeviceHandle(ctx->d3d9devmgr, &ctx->deviceHandle);
			hr = d3d9devmgr->OpenDeviceHandle(&deviceHandle);
			if (FAILED(hr)) {
				throw("Failed to open device handle\n");
			}

			//hr = IDirect3DDeviceManager9_GetVideoService(ctx->d3d9devmgr, ctx->deviceHandle, &IID_IDirectXVideoDecoderService, (void **)&ctx->decoder_service);
			hr = d3d9devmgr->GetVideoService(deviceHandle, IID_IDirectXVideoDecoderService, (void **)&ctx->decoder_service);
			if (FAILED(hr)) {
				throw("Failed to create IDirectXVideoDecoderService\n");
			}
		}
	}
	else
	{
  
		// g_pSharedSurface should be able to be opened in OGL via the WGL_NV_DX_interop extension
		// Vendor support for various textures/surfaces may vary
		hr = d3d9device->CreateOffscreenPlainSurface(widthSize,
													heightSize, 
													rtDesc.Format, 
													D3DPOOL_DEFAULT, 
													&ctx->m_pSharedSurface, 
													& ctx->m_hSharedSurface);

		//hr = IDirect3DDeviceManager9_GetVideoService(ctx->d3d9devmgr, ctx->deviceHandle, &IID_IDirectXVideoDecoderService, (void **)&ctx->decoder_service);
		hr = d3d9devmgr->GetVideoService(deviceHandle, IID_IDirectXVideoDecoderService, (void **)&ctx->decoder_service);
		if (FAILED(hr)) {
			throw("Failed to create IDirectXVideoDecoderService\n");
		}
	}
	return hr;
}

HRESULT CVideoControl::CreateDevice(char * hwaccel_device)
{
	HRESULT hr = S_OK;
	if(d3d9device == nullptr)
	{
		deviceHandle = INVALID_HANDLE_VALUE;
		UINT adapter = D3DADAPTER_DEFAULT;
		D3DDISPLAYMODE        d3ddm;
		D3DPRESENT_PARAMETERS d3dpp = { 0 };
		pCreateDeviceManager9 *createDeviceManager = NULL;

		createDeviceManager = (pCreateDeviceManager9 *)GetProcAddress(dxva2lib, "DXVA2CreateDirect3DDeviceManager9");
		if (!createDeviceManager) {
			throw("Failed to locate DXVA2CreateDirect3DDeviceManager9\n");
		}

		if (hwaccel_device) {
			adapter = atoi(hwaccel_device);
			//av_log(NULL, AV_LOG_INFO, "Using HWAccel device %d\n", adapter);
		}

		IDirect3D9_GetAdapterDisplayMode(d3d9, adapter, &d3ddm);
		d3dpp.Windowed = TRUE;
		d3dpp.BackBufferWidth = 640;
		d3dpp.BackBufferHeight = 480;
		d3dpp.BackBufferCount = 0;
		d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.Flags = D3DPRESENTFLAG_VIDEO;
		unsigned int          g_iAdapter;
		D3DADAPTER_IDENTIFIER9 g_adapter_id;
		// Find the first CL capable device
		for(g_iAdapter = 0; g_iAdapter < d3d9->GetAdapterCount(); g_iAdapter++)
		{
			D3DCAPS9 caps;
			if (FAILED(d3d9->GetDeviceCaps(g_iAdapter, D3DDEVTYPE_HAL, &caps)))
				// Adapter doesn't support Direct3D
				continue;

			if(FAILED(d3d9->GetAdapterIdentifier(g_iAdapter, 0, &g_adapter_id)))
				return E_FAIL;
			break;
		}
		// we check to make sure we have found a OpenCL-compatible D3D device to work on
		if(g_iAdapter == d3d9->GetAdapterCount() ) 
		{
			//shrLog("No OpenCL-compatible Direct3D9 device available\n");
			// destroy the D3D device
			d3d9->Release();
			//Cleanup(EXIT_SUCCESS);
		}

	
		// The interop definition states D3DCREATE_MULTITHREADED is required, but it may vary by vendor
		hr = d3d9->CreateDeviceEx(g_iAdapter,
							D3DDEVTYPE_HAL,
							this->GetHWND(),
							D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
							&d3dpp,
							NULL,
							&d3d9device);

		if (FAILED(hr)) {
			throw("Failed to create Direct3D device\n");
			//goto fail;

		}

		initDevice = true;
	}
	return hr;
}

IDirect3DDevice9Ex * CVideoControl::GetDirect3DDevice()
{
	return d3d9device;
}

#ifdef RENDEROPENGL 
bool CVideoControl::UnbindTexture()
{
	if (WGLEW_NV_DX_interop)
	{
		if (hTexture != nullptr)
		{
			wglDXUnregisterObjectNV(hDevice, hTexture);
			hTexture = nullptr;
		}
		if(initDevice)
		{
			if (hDevice != nullptr)
			{
				wglDXCloseDeviceNV(hDevice);
				hDevice = nullptr;
			}
			initDevice = false;
		}

		renderBitmapOpenGL->DeleteVideoTexture();
	}

	return true;
}
#endif
CVideoControlInterface * CVideoControl::CreateWindow(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer)
{
   return new CVideoControl(parent, id, windowMain, eventPlayer); 
}

void CVideoControl::SetRotation(const int &rotation)
{
    wxCommandEvent event(EVENT_VIDEOROTATION);
    event.SetExtraLong(rotation);
    wxPostEvent(this, event);
}

void CVideoControl::SetVideoStart()
{
    wxCommandEvent event(EVENT_VIDEOSTART);
    wxPostEvent(this, event);
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
#ifdef RENDEROPENGL 

GLTexture * CVideoControl::RenderFromOpenGLTexture()
{
    GLTexture * glTexture = nullptr;
    muVideoEffect.lock();
    int bicubic = videoEffectParameter.BicubicEnable;
    muVideoEffect.unlock();    
    if(bicubic)
        printf("Bicubic Enable \n");
    //printf("RenderFromOpenGLTexture \n");
    
	if (WGLEW_NV_DX_interop)
	{
		if(dxva2 != nullptr && hDevice == nullptr)
		{
			hDevice = wglDXOpenDeviceNV(d3d9device);
		}

		if(hTexture == nullptr && hDevice != nullptr)
		{
			glTexture = renderBitmapOpenGL->GetVideoTexture(widthVideo, heightVideo);
			if(wglDXSetResourceShareHandleNV(dxva2->m_pSharedSurface, dxva2->m_hSharedSurface))
			{
				hTexture = wglDXRegisterObjectNV(hDevice, dxva2->m_pSharedSurface,glTexture->GetTextureID(), GL_TEXTURE_2D, WGL_ACCESS_READ_ONLY_NV);
			
				if (hTexture == NULL)
					return 0;
			}
		}

		if(hDevice != nullptr)
		{
			muBitmap.lock();
			cl_mem cl_textureVideoCopy = 0;
			wglDXLockObjectsNV(hDevice, 1, &hTexture);
			cl_textureVideoCopy = renderBitmapOpenGL->GetCopyVideoTexture(openclContext->GetContext());
			wglDXUnlockObjectsNV(hDevice, 1, &hTexture);	
			muBitmap.unlock();

			
			wxRect rect;
			if(cl_textureVideoCopy != nullptr)
			{
				cl_int err ;
				err = clEnqueueAcquireGLObjects(openclContext->GetCommandQueue(), 1, &cl_textureVideoCopy, 0, 0, 0);
				Error::CheckError(err);

				if(angle == 90 || angle == 270)
				{
					calculate_display_rect(&rect, 0, 0, getHeight(), getWidth());
					openclEffectNV12->InterpolationBicubicOpenGLTexture(cl_textureVideoCopy, widthVideo, heightVideo, rect.height, rect.width, angle, bicubic);
				}
				else
				{
					calculate_display_rect(&rect, 0, 0, getWidth(), getHeight());
					openclEffectNV12->InterpolationBicubicOpenGLTexture(cl_textureVideoCopy, widthVideo, heightVideo, rect.width, rect.height, angle, bicubic);
				}

				err = clEnqueueReleaseGLObjects(openclContext->GetCommandQueue(), 1, &cl_textureVideoCopy, 0, 0, 0);
				Error::CheckError(err);
				err = clFlush(openclContext->GetCommandQueue());
				Error::CheckError(err);
			}

			if(cl_textureVideoCopy != nullptr)
			{
				
				if(angle == 90 || angle == 270)
					glTexture = renderBitmapOpenGL->GetDisplayTexture(rect.height, rect.width, openclContext->GetContext());
				else
					glTexture = renderBitmapOpenGL->GetDisplayTexture(rect.width, rect.height, openclContext->GetContext());
				
				if(glTexture != nullptr)
				{
					try
					{
						cl_int err;
						cl_mem cl_image = renderBitmapOpenGL->GetOpenCLTexturePt();
						err = clEnqueueAcquireGLObjects(openclContext->GetCommandQueue(), 1, &cl_image, 0, 0, 0);
						Error::CheckError(err);
                        if(!renderBitmapOpenGL->IsCopyDirect())
                            openclEffectNV12->GetRgbaBitmap(cl_image,1);
                        else
                            openclEffectNV12->GetRgbaBitmap(cl_image,0);
						err = clEnqueueReleaseGLObjects(openclContext->GetCommandQueue(), 1, &cl_image, 0, 0, 0);
						Error::CheckError(err);
						err = clFlush(openclContext->GetCommandQueue());
						Error::CheckError(err);
					}
					catch(...)
					{

					}
				}


			}
		}
	}
    return glTexture;
}
#else


#endif
void CVideoControl::OnPaint(wxPaintEvent& event)
{
#ifdef RENDEROPENGL 
    GLTexture * glTexture = nullptr;
#endif
    printf("OnPaint CVideoControl begin \n"); 
       
    std::clock_t start;
    start = std::clock();    
    
	int supportOpenCL = 0;
	muVideoEffect.lock();
	supportOpenCL = videoEffectParameter.enableOpenCL;
	muVideoEffect.unlock();


    int width = GetWindowWidth();
    int height = GetWindowHeight();
    if(width == 0 || height == 0)
        return;
	
	if (quitWindow)
        return;

#ifdef RENDEROPENGL 
    renderBitmapOpenGL->SetCurrent(*this);

	if (openCLEngine == nullptr)
    {
		openCLEngine = new COpenCLEngine();

		if (openCLEngine != nullptr)
			openclContext = openCLEngine->GetInstance();

		openclEffectNV12 = new COpenCLEffectVideoNV12(openclContext);
		hDevice = nullptr;
		hTexture = nullptr;
		openclEffectYUV = new COpenCLEffectVideoYUV(openclContext);
    }
#else

	if(openclEffectNV12 == nullptr)
		openclEffectNV12 = new COpenCLEffectVideoNV12(openclContext);

	if(openclEffectYUV == nullptr)
		openclEffectYUV = new COpenCLEffectVideoYUV(openclContext);

#endif

    if (videoRenderStart && initStart)
    {
        if(!fpsTimer->IsRunning())
            fpsTimer->Start(1000);
#ifdef RENDEROPENGL 
		UnbindTexture();
#endif
		dxva2ToOpenGLWorking = true;
		initStart = false;
	}

#ifdef RENDEROPENGL 
	if (videoRenderStart)
	{
		if (isDXVA2Compatible && dxva2ToOpenGLWorking && supportOpenCL)
			glTexture = RenderFromOpenGLTexture();
		else
		{
			if (isDXVA2Compatible && supportOpenCL)
			{
				if (openclEffectNV12->IsOk())
				{
					muBitmap.lock();
					glTexture = RenderToTexture(openclEffectNV12);
					muBitmap.unlock();
				}
			}
			else
			{
				glTexture = RenderToGLTexture();
			}
		}
	}
	if (videoRenderStart && glTexture != nullptr)
	{
		renderBitmapOpenGL->CreateScreenRender(GetWindowWidth(), GetWindowHeight(), CRgbaquad(0, 0, 0, 0));

		if (glTexture != nullptr)
		{
			int inverted = 1;
			int x = (GetWindowWidth() - glTexture->GetWidth()) / 2;
			int y = (GetWindowHeight() - glTexture->GetHeight()) / 2;
			if (openclContext->IsSharedContextCompatible())
				inverted = 0;

			if (!supportOpenCL)
				inverted = 1;

			if (isDXVA2Compatible)
				inverted = 0;

			printf("glTexture info id : %d width %d height %d \n", glTexture->GetTextureID(), glTexture->GetWidth(), glTexture->GetHeight());
			renderBitmapOpenGL->RenderWithEffect(x, y, glTexture, &videoEffectParameter, flipH, flipV, inverted);

		}

		printf("Nb Frame per Seconds : %s \n", CConvertUtility::ConvertToUTF8(msgFrame));

		muVideoEffect.lock();
		if (videoEffectParameter.showFPS)
		{
			renderBitmapOpenGL->Print(0, 1, CConvertUtility::ConvertToUTF8(msgFrame));
		}
		muVideoEffect.unlock();

		muSubtitle.lock();

		if (subtilteUpdate && pictureSubtitle != nullptr)
		{
			renderBitmapOpenGL->SetSubtitle(pictureSubtitle);

			delete pictureSubtitle;
			pictureSubtitle = nullptr;

			subtilteUpdate = false;
		}
		else if (subtilteUpdate)
		{
			renderBitmapOpenGL->DeleteSubtitle();
			subtilteUpdate = false;
		}

		muSubtitle.unlock();

		if (videoEffectParameter.enableSubtitle)
		{
			renderBitmapOpenGL->ShowSubtitle();
		}

	}
	else
	{
		renderBitmapOpenGL->CreateScreenRender(GetWindowWidth(), GetWindowHeight(), CRgbaquad(0, 0, 0, 0));
	}

	this->SwapBuffers();

	if (deleteTexture)
		if (glTexture != nullptr)
			delete glTexture;
#else
	wxPaintDC dc(this);
	wxBitmap localmemBitmap(width, height);
	wxMemoryDC memDC(localmemBitmap);
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = width;
	rc.height = height;
	FillRect(&memDC, rc, wxColor(0, 0, 0));
	if (videoRenderStart)
	{
		int inverted = 1;
		if (!supportOpenCL)
			inverted = 1;

		if (isDXVA2Compatible)
			inverted = 0;

		CRegardsBitmap * bitmap = nullptr;
		if (isDXVA2Compatible)
			bitmap = RenderToBitmap(openclEffectNV12);
		else
			bitmap = RenderToBitmap(openclEffectYUV);

		if (bitmap != nullptr)
		{
			CImageLoadingFormat image;
			image.SetPicture(bitmap);
			wxImage * picture = image.GetwxImage();

			int x = (width - picture->GetWidth()) / 2;
			int y = (height - picture->GetHeight()) / 2;

			if(inverted)
				memDC.DrawBitmap(picture->Mirror(false), x, y);
			else
				memDC.DrawBitmap(*picture, x, y);
		}
	}


	memDC.SelectObject(wxNullBitmap);

	dc.DrawBitmap(localmemBitmap, 0, 0);

#endif

    double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    printf("Nb Frame per Seconds : %d \n",nbFrame);
    std::cout<<"Video OnPaint Time : "<< duration <<'\n';
    
    nbFrame++;
}

int CVideoControl::ChangeSubtitleStream(int newStreamSubtitle)
{
	ffmfc_change_subtitle_stream(newStreamSubtitle);
	SetVideoPosition(videoPosition / 1000);
	return 0;
}


int CVideoControl::ChangeAudioStream(int newStreamAudio)
{
	ffmfc_change_audio_stream(newStreamAudio);
	SetVideoPosition(videoPosition / 1000);
	return 0;
}

void CVideoControl::OnPlay()
{
	if (pause && !videoEnd)
	{
		ffmfc_play_pause();
	}
	else if(videoEnd)
    {
		PlayMovie(filename);
    }

	pause = false;
}

void CVideoControl::OnStop()
{
	exit = true;
	stopVideo = true;
    if(ffmfc_quit())
	{
		wxCommandEvent localevent;
		EndVideoThread(localevent);
		//videoEnd = true;
	}
}

void CVideoControl::OnPause()
{
	if (!pause)
	{
		ffmfc_play_pause();
	}
	pause = true;
}

void CVideoControl::SetVideoDuration(int64_t duration)
{
	if (eventPlayer != nullptr)
		eventPlayer->SetVideoDuration(duration);
}

void CVideoControl::SetVideoPosition(int64_t pos)
{
	ffmfc_SetTimePosition(pos * 1000 * 1000);
}

void CVideoControl::SetCurrentclock(wxString message)
{
	this->message = message;
}

void CVideoControl::SetPos(int64_t pos)
{
	videoPosition = pos;
	if (eventPlayer != nullptr)
		eventPlayer->OnPositionVideo(pos);
	//Refresh();
}

void CVideoControl::VolumeUp()
{
	ffmfc_VolumeUp();
}

void CVideoControl::VolumeDown()
{
	ffmfc_VolumeDown();
}

int CVideoControl::GetVolume()
{
	return ffmfc_GetVolume();
}

void CVideoControl::OnRButtonDown(wxMouseEvent& event)
{
	//wxWindow * window = this->FindWindowById(idWindowMain);
	if (windowMain != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, TOOLBAR_UPDATE_ID);
		windowMain->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CVideoControl::SetDXVA2Compatible(const bool &compatible)
{
	isDXVA2Compatible = compatible;
}

bool CVideoControl::GetDXVA2Compatible()
{
	return isDXVA2Compatible;
}

void CVideoControl::SetData(void * data, const float & sample_aspect_ratio, void * dxva2Context)
{
    std::clock_t start = std::clock();
     
	videoRenderStart = true;  

	AVFrame *src_frame = (AVFrame *)data;

    video_aspect_ratio = sample_aspect_ratio;

#ifdef RENDEROPENGL  

    if(isDXVA2Compatible)
    {
        bool glewExist = WGLEW_NV_DX_interop ? true : false;
        
        if (glewExist)
        {            
            if(dxva2Context != nullptr)	
                this->dxva2 = (DXVA2Context *)dxva2Context;

            muBitmap.lock();
            LPDIRECT3DSURFACE9 surface = (LPDIRECT3DSURFACE9)src_frame->data[3];
            widthVideo = src_frame->width;
            heightVideo = src_frame->height;

            int widthSize = (widthVideo % 128);
            if (widthSize == 0)
                widthSize = widthVideo;
            else
                widthSize = (128 - widthSize) + widthVideo;

            int heightSize = heightVideo;

            if (hTexture != nullptr && hDevice != nullptr)
            {
                // Copy the result to the shared surface
                IDirect3DSurface9* pSharedSurface =dxva2->m_pSharedSurface;
                RECT rc = { 0, 0, widthVideo, heightVideo };
                //RECT rcWindow = { 0, 0, widthVideo, heightVideo };

                HRESULT hr = d3d9device->BeginScene();	
                if (FAILED(hr))
                    return;
                //hr = d3d9device->StretchRect(surface, &rc, pSharedSurface, &rc, D3DTEXF_NONE);
                hr = d3d9device->StretchRect(surface, &rc, pSharedSurface, &rc, D3DTEXF_NONE);
                if (FAILED(hr))
                    dxva2ToOpenGLWorking = false;
                else
                    dxva2ToOpenGLWorking = true;

                hr = d3d9device->EndScene();
                if (FAILED(hr))
                    return;


            }

            muBitmap.unlock();	


        }
        else
        {
            dxva2ToOpenGLWorking = false;
            printf("dxva2ToOpenGLWorking false \n");
        }	
        if (!dxva2ToOpenGLWorking)
        {
             printf("copy surface LPDIRECT3DSURFACE9 \n");
             
            LPDIRECT3DSURFACE9 surface = (LPDIRECT3DSURFACE9)src_frame->data[3];
            D3DSURFACE_DESC    surfaceDesc;
            D3DLOCKED_RECT     LockedRect;
            HRESULT            hr;
            int                ret;

            IDirect3DSurface9_GetDesc(surface, &surfaceDesc);
            muBitmap.lock();
            hr = IDirect3DSurface9_LockRect(surface, &LockedRect, NULL, D3DLOCK_READONLY);
            if (FAILED(hr)) {
                av_log(NULL, AV_LOG_ERROR, "Unable to lock DXVA2 surface\n");
                return;
            }

            int size = LockedRect.Pitch * surfaceDesc.Height + (LockedRect.Pitch * (surfaceDesc.Height / 2));

            openclEffectNV12->SetMemoryData((uint8_t *)LockedRect.pBits, size, src_frame->width, src_frame->height, LockedRect.Pitch, surfaceDesc.Height, src_frame->format);

            IDirect3DSurface9_UnlockRect(surface);
            muBitmap.unlock();
            widthVideo = src_frame->width;
            heightVideo = src_frame->height;
        }

    }
    else
    {

        SetFrameData(src_frame);
        
        if(isffmpegDecode)
            CopyFrame(src_frame);        

    }

#else
	if (isDXVA2Compatible)
	{
		printf("copy surface LPDIRECT3DSURFACE9 \n");

		LPDIRECT3DSURFACE9 surface = (LPDIRECT3DSURFACE9)src_frame->data[3];
		D3DSURFACE_DESC    surfaceDesc;
		D3DLOCKED_RECT     LockedRect;
		HRESULT            hr;
		int                ret;

		IDirect3DSurface9_GetDesc(surface, &surfaceDesc);
		muBitmap.lock();
		hr = IDirect3DSurface9_LockRect(surface, &LockedRect, NULL, D3DLOCK_READONLY);
		if (FAILED(hr)) {
			av_log(NULL, AV_LOG_ERROR, "Unable to lock DXVA2 surface\n");
			return;
		}

		int size = LockedRect.Pitch * surfaceDesc.Height + (LockedRect.Pitch * (surfaceDesc.Height / 2));

		if(openclEffectNV12 != nullptr)
			openclEffectNV12->SetMemoryData((uint8_t *)LockedRect.pBits, size, src_frame->width, src_frame->height, LockedRect.Pitch, surfaceDesc.Height, src_frame->format);

		IDirect3DSurface9_UnlockRect(surface);
		muBitmap.unlock();
		widthVideo = src_frame->width;
		heightVideo = src_frame->height;
	}
	else
	{
		SetFrameData(src_frame);
		if (isffmpegDecode)
			CopyFrame(src_frame);
	}

#endif

    widthVideo = src_frame->width;
    heightVideo = src_frame->height;  
    double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"Set Data time execution: "<< duration <<'\n';
    
    this->Refresh();
}

void CVideoControl::Resize()
{
     updateContext = true;
     
    if(videoStart)
        ffmfc_videoDisplaySize(GetWindowWidth(),GetWindowHeight() );
        
    if(pause && isffmpegDecode && copyFrameBuffer != nullptr)
    {
         SetFrameData(copyFrameBuffer);
         Refresh();
    } 
}

#endif