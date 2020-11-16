#include "header.h"

#ifdef WIN32

#include "VideoControl.h"
#include <wx/dcbuffer.h>
#include "ffplaycore.h"
#include <d3d9.h>
#include <CL/cl_dx9_media_sharing.h>
#include <CL/cl.h>
#include <utility.h>
#include "ffmpegToBitmap.h"
#include <ViewerParam.h>


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
: CVideoControlSoft(parent, id, windowMain, eventPlayer)
{
	hDevice = nullptr;
	hTexture = nullptr;
	windowWidth = 0;
	windowHeight = 0;
	dxva2ToOpenGLWorking = true;
	openclEffectNV12 = nullptr;
	initDevice = true;
	dxva2 = nullptr;
    d3d9devmgr = nullptr;
	d3d9device = nullptr;
	m_pRenderTargetSurface = nullptr;
	isDXVA2Compatible = true;
	
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

}


CVideoControl::~CVideoControl()
{
	initDevice = true;

#ifdef RENDEROPENGL 
	// This is normally only necessary if there is more than one wxGLCanvas
// or more than one wxGLContext in the application.
	if (renderBitmapOpenGL != nullptr)
	{
		renderBitmapOpenGL->SetCurrent(*this);
		UnbindTexture();
        delete renderBitmapOpenGL;
        renderBitmapOpenGL = nullptr;
	}
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


	if (openclEffectNV12 != nullptr)
		delete openclEffectNV12;
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
	if (initDevice)
		renderBitmapOpenGL->DeleteVideoTexture();

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

		
	}

	return true;
}
#endif
CVideoControlSoft * CVideoControl::CreateWindow(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, IVideoInterface * eventPlayer)
{
   return new CVideoControl(parent, id, windowMain, eventPlayer); 
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
#ifdef RENDEROPENGL 

GLTexture * CVideoControl::RenderFromOpenGLTexture()
{
    GLTexture * glTexture = nullptr;
	GLTexture * glTextureCopy = nullptr;
	displaywithInterpolation = false;
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
				{
					wxMessageBox("DXVA2 not support by your Graphic Card. Please Restart Regards.", "Error", wxICON_ERROR);
					CRegardsConfigParam * regardsParam = CParamInit::getInstance();
					if(regardsParam != nullptr)
						regardsParam->SetDxva2Actif(0);
				}
			}

			
		}

		if (hDevice != nullptr)
		{
			muBitmap.lock();
			cl_mem cl_textureVideoCopy = 0;
			wglDXLockObjectsNV(hDevice, 1, &hTexture);
			cl_textureVideoCopy = renderBitmapOpenGL->GetCopyVideoTexture(openclContext->GetContext());
			wglDXUnlockObjectsNV(hDevice, 1, &hTexture);
			muBitmap.unlock();
			wxRect rect;
			if (cl_textureVideoCopy != nullptr)
			{
				cl_int err;
				err = clEnqueueAcquireGLObjects(openclContext->GetCommandQueue(), 1, &cl_textureVideoCopy, 0, 0, 0);
				Error::CheckError(err);

				float zoomRatio = 1.0f;
				muVideoEffect.lock();
				int bicubic = videoEffectParameter.BicubicEnable;
				zoomRatio = videoEffectParameter.tabZoom[videoEffectParameter.zoomSelect];
				muVideoEffect.unlock();
				int filterInterpolation = 0;
				CRegardsConfigParam * regardsParam = CParamInit::getInstance();

				if (regardsParam != nullptr)
					filterInterpolation = regardsParam->GetInterpolationType();

				if (zoomRatio == 1.0f)
				{
					if (angle == 90 || angle == 270)
					{
						calculate_display_rect(&rect, 0, 0, getHeight(), getWidth());
						openclEffectNV12->InterpolationBicubicOpenGLTexture(cl_textureVideoCopy, widthVideo, heightVideo, rect.height, rect.width, flipH, flipV, angle, filterInterpolation);
					}
					else
					{
						calculate_display_rect(&rect, 0, 0, getWidth(), getHeight());
						openclEffectNV12->InterpolationBicubicOpenGLTexture(cl_textureVideoCopy, widthVideo, heightVideo, rect.width, rect.height, flipH, flipV, angle, filterInterpolation);
					}

				}
				else
				{
					int widthOut = 0;
					int heightOut = 0;
					CalculTextureSize(widthOut, heightOut);

					wxRect posrect;
					posrect.x = posLargeur;
					posrect.y = posHauteur;
					posrect.width = widthOut;
					posrect.height = heightOut;

					if (angle == 90 || angle == 270)
					{
						rect.height = getHeight();
						rect.width = getWidth();
						if (rect.height > widthOut)
							rect.height = widthOut;
						if (rect.width > heightOut)
							rect.width = heightOut;
						openclEffectNV12->InterpolationBicubicZoneOpenGLTexture(cl_textureVideoCopy, widthVideo, heightVideo, rect.height, rect.width, posrect, flipH, flipV, angle, filterInterpolation);
					}
					else
					{
						rect.height = getHeight();
						rect.width = getWidth();

						if (rect.height > heightOut)
							rect.height = heightOut;
						if (rect.width > widthOut)
							rect.width = widthOut;

						openclEffectNV12->InterpolationBicubicZoneOpenGLTexture(cl_textureVideoCopy, widthVideo, heightVideo, rect.width, rect.height, posrect, flipH, flipV, angle, filterInterpolation);
					}
				}

				err = clEnqueueReleaseGLObjects(openclContext->GetCommandQueue(), 1, &cl_textureVideoCopy, 0, 0, 0);
				Error::CheckError(err);
				err = clFlush(openclContext->GetCommandQueue());
				Error::CheckError(err);
			}

			if (cl_textureVideoCopy != nullptr)
			{

				if (angle == 90 || angle == 270)
					glTexture = renderBitmapOpenGL->GetDisplayTexture(rect.height, rect.width, openclContext->GetContext());
				else
					glTexture = renderBitmapOpenGL->GetDisplayTexture(rect.width, rect.height, openclContext->GetContext());

				if (glTexture != nullptr)
				{
					try
					{
						cl_int err;
						cl_mem cl_image = renderBitmapOpenGL->GetOpenCLTexturePt();
						err = clEnqueueAcquireGLObjects(openclContext->GetCommandQueue(), 1, &cl_image, 0, 0, 0);
						Error::CheckError(err);
						if (!renderBitmapOpenGL->IsCopyDirect())
							openclEffectNV12->GetRgbaBitmap(cl_image, 1);
						else
							openclEffectNV12->GetRgbaBitmap(cl_image, 0);
						err = clEnqueueReleaseGLObjects(openclContext->GetCommandQueue(), 1, &cl_image, 0, 0, 0);
						Error::CheckError(err);
						err = clFlush(openclContext->GetCommandQueue());
						Error::CheckError(err);
					}
					catch (...)
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
	// This is a dummy, to avoid an endless succession of paint messages.
	// OnPaint handlers must always create a wxPaintDC.
	wxPaintDC dc(this);

#ifdef RENDEROPENGL 
    GLTexture * glTexture = nullptr;
	GLTexture * glTextureOutput = nullptr;
#endif


#ifdef RENDEROPENGL

	if (renderBitmapOpenGL == nullptr)
	{
		renderBitmapOpenGL = new CRenderVideoOpenGL(this);

		//Now we have a context, retrieve pointers to OGL functions
		renderBitmapOpenGL->Init(this);
	}

	// This is normally only necessary if there is more than one wxGLCanvas
	// or more than one wxGLContext in the application.
	renderBitmapOpenGL->SetCurrent(*this);

    if (openCLEngine == nullptr)
    {
        openCLEngine = new COpenCLEngine();
        if (openCLEngine != nullptr)
            openclContext = openCLEngine->GetInstance();
    }

#endif

	if (videoRenderStart && initStart)
	{
		if (!fpsTimer->IsRunning())
			fpsTimer->Start(1000);
#ifdef RENDEROPENGL 
		UnbindTexture();
#endif

		dxva2ToOpenGLWorking = true;
		initStart = false;
	}

    printf("OnPaint CVideoControl begin \n"); 
       
    std::clock_t start;
    start = std::clock();    
    
	int supportOpenCL = COpenCLEngine::SupportOpenCL();
    int width = GetWindowWidth();
    int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;
	
	if (quitWindow)
        return;

#ifdef RENDEROPENGL 
	renderBitmapOpenGL->SetCurrent(*this);

	if (openclEffectNV12 == nullptr)
	{
		openclEffectNV12 = new COpenCLEffectVideoNV12(openclContext);
		hDevice = nullptr;
		hTexture = nullptr;
		openclEffectYUV = new COpenCLEffectVideoYUV(openclContext);
	}
#else

	if (openclEffectNV12 == nullptr)
		openclEffectNV12 = new COpenCLEffectVideoNV12(openclContext);

	if (openclEffectYUV == nullptr)
		openclEffectYUV = new COpenCLEffectVideoYUV(openclContext);

#endif

#ifdef RENDEROPENGL 
	if (videoRenderStart)
	{
		if (dxva2ToOpenGLWorking)
			glTexture = RenderFromOpenGLTexture();
		else
			glTexture = RenderToGLTexture();
	}
	if (videoRenderStart && glTexture != nullptr)
	{
		renderBitmapOpenGL->CreateScreenRender(GetWindowWidth(), GetWindowHeight(), CRgbaquad(0, 0, 0, 0));
		
		glTextureOutput = DisplayTexture(glTexture);

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

	if (glTextureOutput != nullptr)
		delete glTextureOutput;
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
	if (localmemBitmap.IsOk())
		dc.DrawBitmap(localmemBitmap, 0, 0);

#endif

    double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    //printf("Nb Frame per Seconds : %d \n",nbFrame);
    //std::cout<<"Video OnPaint Time : "<< duration <<'\n';
    
    nbFrame++;
}

void CVideoControl::SetDXVA2Compatible(const bool &compatible)
{
	isDXVA2Compatible = compatible;
}

bool CVideoControl::GetDXVA2HardwareCompatible()
{
	int dxva2 = 0;
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		dxva2 = regardsParam->GetDxva2Actif();
	}
	return dxva2;
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

	if (isDXVA2Compatible)
	{
		bool glewExist = WGLEW_NV_DX_interop ? true : false;

		if (glewExist)
		{
			if (dxva2Context != nullptr)
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
				IDirect3DSurface9* pSharedSurface = dxva2->m_pSharedSurface;
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
			//else
			//	dxva2ToOpenGLWorking = false;

			muBitmap.unlock();


		}
		else
		{
			dxva2ToOpenGLWorking = false;
			printf("dxva2ToOpenGLWorking false \n");
		}
		if (!dxva2ToOpenGLWorking && openclEffectNV12 != nullptr)
		{
			printf("copy surface LPDIRECT3DSURFACE9 \n");

			LPDIRECT3DSURFACE9 surface = (LPDIRECT3DSURFACE9)src_frame->data[3];
			D3DSURFACE_DESC    surfaceDesc;
			D3DLOCKED_RECT     LockedRect;
			HRESULT            hr;

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
		if (dxva2Context == nullptr)
		{
			SetFrameData(src_frame);

			if (isffmpegDecode)
				CopyFrame(src_frame);
		}

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
    
    this->ForceRefresh();
}

#endif
