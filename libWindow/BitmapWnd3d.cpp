// ReSharper disable All
#include "header.h"
#include "BitmapWnd3d.h"
#include "BitmapWndRender.h"
#include <RenderOpenGL.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>


#include <opencv2/core/opengl.hpp>
#include <utility.h>

#ifdef __APPLE__
#include <OpenCL/cl_gl.h>
#include <OpenGL/OpenGL.h>
#include <OpenCL/cl_gl_ext.h>
#elif defined(__WXGTK__)
#if wxUSE_GLCANVAS_EGL == 1
#include <EGL/egl.h>
#endif
#endif

#if defined (__APPLE__) || defined(MACOSX)
static const char* CL_GL_SHARING_EXT = "cl_APPLE_gl_sharing";
#else
static const char* CL_GL_SHARING_EXT = "cl_khr_gl_sharing";
#endif


extern bool isOpenCLInitialized;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

CBitmapWnd3D::CBitmapWnd3D(wxWindow* parent, wxWindowID id)
	: CWindowOpenGLMain("CBitmapWnd3D", parent, id)
{
	Connect(wxEVT_PAINT, wxPaintEventHandler(CBitmapWnd3D::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CBitmapWnd3D::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CBitmapWnd3D::OnLButtonDown));
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CBitmapWnd3D::OnRButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CBitmapWnd3D::OnLButtonUp));
	Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(CBitmapWnd3D::OnLDoubleClick));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CBitmapWnd3D::OnMouseWheel));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CBitmapWnd3D::OnKeyDown));
	Connect(wxEVT_KEY_UP, wxKeyEventHandler(CBitmapWnd3D::OnKeyUp));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CBitmapWnd3D::OnMouseCaptureLost));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CBitmapWnd3D::OnIdle));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CBitmapWnd3D::OnMouseLeave));
}

void CBitmapWnd3D::OnMouseLeave(wxMouseEvent& event)
{
	if (HasCapture())
		ReleaseMouse();
}

void CBitmapWnd3D::SetBitmapRenderInterface(IBitmapRenderInterface* bitmapWndRender)
{
	this->bitmapWndRender = bitmapWndRender;
	this->bitmapWndRender->SetParent(this);
	bitmapWndRender->SetOpenCLOpenGLInterop(openclOpenGLInterop);
	vector<int> listTimer = bitmapWndRender->GetListTimer();
	for (int id : listTimer)
	{
		Connect(id, wxEVT_TIMER, wxTimerEventHandler(CBitmapWnd3D::OnTimer), nullptr, this);
	}

	vector<int> listCommand = bitmapWndRender->GetListCommand();
	for (int id : listCommand)
	{
		Connect(id, wxCommandEventHandler(CBitmapWnd3D::OnCommand));
	}
}

void CBitmapWnd3D::UpdateRenderInterface(IBitmapRenderInterface* bitmapWndRender)
{
	this->bitmapWndRender = bitmapWndRender;
	bitmapWndRender->SetOpenCLOpenGLInterop(openclOpenGLInterop);
}

bool CBitmapWnd3D::GetProcessEnd()
{
	return bitmapWndRender->GetProcessEnd();
}

void CBitmapWnd3D::OnIdle(wxIdleEvent& evt)
{
	bitmapWndRender->SetEndProgram(endProgram);
	bitmapWndRender->OnIdle(evt);
}

void CBitmapWnd3D::UpdateScreenRatio()
{
	bitmapWndRender->UpdateScreenRatio();
}

void CBitmapWnd3D::Resize()
{
	//updateFilter = true;
	bitmapWndRender->UpdateResized();
	this->Refresh();
}

IBitmapRenderInterface* CBitmapWnd3D::GetWndPt()
{
	return bitmapWndRender;
}

void CBitmapWnd3D::OnCommand(wxCommandEvent& event)
{
	bitmapWndRender->OnCommand(event);
}

void CBitmapWnd3D::OnTimer(wxTimerEvent& event)
{
	bitmapWndRender->OnTimer(event);
}

//-----------------------------------------------------------------
//Destructeur
//-----------------------------------------------------------------
CBitmapWnd3D::~CBitmapWnd3D(void)
{
}

void CBitmapWnd3D::OnRButtonDown(wxMouseEvent& event)
{
	bitmapWndRender->OnRButtonDown(event); //
	this->SetFocus();
}

//-----------------------------------------------------------------
//Gestion du click de souris
//-----------------------------------------------------------------
void CBitmapWnd3D::OnLButtonDown(wxMouseEvent& event)
{
	bitmapWndRender->OnLButtonDown(event);
	this->SetFocus();
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWnd3D::OnLButtonUp(wxMouseEvent& event)
{
	//
	if (event.LeftDClick())
		return;

	bitmapWndRender->OnLButtonUp(event);
}

void CBitmapWnd3D::OnKeyUp(wxKeyEvent& event)
{
	bitmapWndRender->OnKeyUp(event);
}

void CBitmapWnd3D::OnKeyDown(wxKeyEvent& event)
{
	bitmapWndRender->OnKeyDown(event);
}

//-----------------------------------------------------------------
//Gestion du volant de souris
//-----------------------------------------------------------------
void CBitmapWnd3D::OnMouseWheel(wxMouseEvent& event)
{
	bitmapWndRender->OnMouseWheel(event);
}

//------------------------------------------------------------------------------------
//Déplacement de la souris
//------------------------------------------------------------------------------------
void CBitmapWnd3D::OnMouseMove(wxMouseEvent& event)
{
	bitmapWndRender->OnMouseMove(event);
	//this->Refresh();
}

wxString GetDeviceInfo(cl_device_id device, cl_device_info param_name)
{
	try
	{
		// Get the length for the i-th device name
		size_t device_name_length = 0;
		cl_int err = clGetDeviceInfo(
			device,
			param_name,
			0,
			nullptr,
			&device_name_length
		);
		Error::CheckError(err);

		// Get the name itself for the i-th device
		// use vector for automatic memory management
		vector<char> device_name(device_name_length);
		err = clGetDeviceInfo(
			device,
			param_name,
			device_name_length,
			&device_name[0],
			nullptr
		);
		Error::CheckError(err);

		return wxString(&device_name[0]);
	}
	catch (...)
	{
	}

	return "";
}

cl_device_id GetListOfDevice(cl_platform_id platform, cl_device_type device_type, int& found)
{
	found = -1;

	cl_uint num_of_devices;

	cl_int err = clGetDeviceIDs(
		platform,
		device_type,
		0,
		nullptr,
		&num_of_devices
	);

	Error::CheckError(err);

	vector<cl_device_id> devices(num_of_devices);

	err = clGetDeviceIDs(
		platform,
		device_type,
		num_of_devices,
		&devices[0],
		nullptr
	);
	Error::CheckError(err);

	for (cl_uint i = 0; i < num_of_devices; ++i)
	{
		int supported = 0;
		cl_device_type type;
		clGetDeviceInfo(devices[i], CL_DEVICE_TYPE, sizeof(type), &type, nullptr);
		wxString deviceName = GetDeviceInfo(devices[i], CL_DEVICE_NAME);
		if (deviceName == "")
			continue;

		if (type == CL_DEVICE_TYPE_GPU)
		{
			wxString supportExt = GetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS);
			supported = supportExt.find(CL_GL_SHARING_EXT);
			if (supported > 0)
				supported = 1;
			else
				supported = 0;
		}

		if (!supported)
			continue;

		found = i;
		printf("Device found : %s \n", CConvertUtility::ConvertToUTF8(deviceName));
		break;
	}

	if (found == -1)
		return nullptr;
	return devices[found];
}

cv::ocl::Context& CBitmapWnd3D::initializeContextFromGL()
{
#if defined(__APPLE__) || defined(__MACOSX)


    cl_uint numPlatforms;
    cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    if (numPlatforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(numPlatforms);
    status = clGetPlatformIDs(numPlatforms, &platforms[0], NULL);
    if (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE

    int found = -1;
    cl_device_id device = NULL;
    device = GetListOfDevice(platforms[0], CL_DEVICE_TYPE_GPU, found);

    if (found < 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for OpenGL interop");

    cl_context context = NULL;

    // get OpenGL share group
    CGLContextObj cgl_current_context = CGLGetCurrentContext();
    CGLShareGroupObj cgl_share_group = CGLGetShareGroup(cgl_current_context);

    cl_context_properties properties[] = {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties)cgl_share_group,
        0
    };
    // create context
    context = clCreateContext(properties, 1, &device, NULL, NULL, &status);
    if (status != CL_SUCCESS)
    {
        clReleaseDevice(device);
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for OpenGL interop");
    }

    cl_platform_id platform = platforms[0];
    std::string platformName = cv::ocl::PlatformInfo(&platform).name();

    cv::ocl::OpenCLExecutionContext clExecCtx = cv::ocl::OpenCLExecutionContext::create(platformName, platform, context, device);
    clReleaseDevice(device);
    clReleaseContext(context);
    clExecCtx.bind();
    return const_cast<cv::ocl::Context&>(clExecCtx.getContext());

#else

	cl_uint numPlatforms;
	cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
	if (numPlatforms == 0)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

	std::vector<cl_platform_id> platforms(numPlatforms);
	status = clGetPlatformIDs(numPlatforms, &platforms[0], NULL);
	if (status != CL_SUCCESS)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");

	// TODO Filter platforms by name from OPENCV_OPENCL_DEVICE

	int found = -1;
	cl_device_id device = NULL;
	cl_context context = NULL;

	for (int i = 0; i < (int)numPlatforms; i++)
	{
		// query platform extension: presence of "cl_khr_gl_sharing" extension is required
		{
			cv::AutoBuffer<char> extensionStr;

			size_t extensionSize;
			status = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 0, NULL, &extensionSize);
			if (status == CL_SUCCESS)
			{
				extensionStr.allocate(extensionSize + 1);
				status = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, extensionSize,
				                           (char*)extensionStr.data(), NULL);
			}
			if (status != CL_SUCCESS)
				CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform extension string");

			if (!strstr((const char*)extensionStr.data(), CL_GL_SHARING_EXT))
				continue;
		}

		clGetGLContextInfoKHR_fn clGetGLContextInfoKHR = (clGetGLContextInfoKHR_fn)
			clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetGLContextInfoKHR");
		if (!clGetGLContextInfoKHR)
			continue;


#ifdef WIN32
		// Create CL context properties, add WGL context & handle to DC
		cl_context_properties properties[] = {
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), // WGL Context
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(), // WGL HDC
			CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i], // OpenCL platform
			0
		};
#elif defined(__WXGTK__)

#if wxUSE_GLCANVAS_EGL == 1
        EGLContext eglContext = eglGetCurrentContext();
        EGLDisplay eglDisplay = eglGetCurrentDisplay();

        cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
        CL_GL_CONTEXT_KHR, (cl_context_properties)eglContext,
        CL_EGL_DISPLAY_KHR,  (cl_context_properties)eglDisplay, 0 };

#else
        // Create CL context properties, add GLX context & handle to DC
        GLXContext glxcontext = glXGetCurrentContext();
        Display* display = glXGetCurrentDisplay();

        cl_context_properties properties[] = {
         CL_GL_CONTEXT_KHR, (cl_context_properties)glxcontext, // GLX Context
         CL_GLX_DISPLAY_KHR, (cl_context_properties)display, // GLX Display
         CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i], // OpenCL platform
         0
        };

#endif
#endif

		// query device
		device = NULL;
		status = clGetGLContextInfoKHR(properties, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, sizeof(cl_device_id),
		                               (void*)&device, NULL);
		if (status != CL_SUCCESS)
			continue;

		// create context
		context = clCreateContext(properties, 1, &device, NULL, NULL, &status);
		if (status != CL_SUCCESS)
		{
			clReleaseDevice(device);
			found = -1;
			break;
		}
		else
		{
			found = i;
			break;
		}
	}

	if (found < 0)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for OpenGL interop");

	cl_platform_id platform = platforms[found];
	std::string platformName = cv::ocl::PlatformInfo(&platform).name();

	cv::ocl::OpenCLExecutionContext clExecCtx = cv::ocl::OpenCLExecutionContext::create(
		platformName, platform, context, device);
	clReleaseDevice(device);
	clReleaseContext(context);
	clExecCtx.bind();
	return const_cast<cv::ocl::Context&>(clExecCtx.getContext());
#endif
}


//-----------------------------------------------------------------
//Dessin de l'image
//-----------------------------------------------------------------
void CBitmapWnd3D::OnPaint(wxPaintEvent& event)
{
	if (GetWidth() == 0 || GetHeight() == 0)
		return;

	if (renderOpenGL == nullptr)
	{
		renderOpenGL = new CRenderOpenGL(this);
		renderOpenGL->Init(this);

		CRegardsConfigParam* regardsParam = CParamInit::getInstance();
		if (regardsParam != nullptr)
		{
			if (regardsParam->GetIsOpenCLSupport() && regardsParam->GetIsOpenCLOpenGLInteropSupport())
			{
				if (cv::ocl::haveOpenCL() && !isOpenCLInitialized)
				{
					try
					{
						initializeContextFromGL();
						isOpenCLInitialized = true;
						openclOpenGLInterop = true;
						regardsParam->SetIsOpenCLOpenGLInteropSupport(true);
					}
					catch (cv::Exception& e)
					{
						const char* err_msg = e.what();
						std::cout << "exception caught: " << err_msg << std::endl;
						std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
						openclOpenGLInterop = false;
						cv::ocl::Context context;
						if (!context.create(cv::ocl::Device::TYPE_GPU))
							isOpenCLInitialized = false;
						else
							isOpenCLInitialized = true;

						if (!isOpenCLInitialized)
						{
							if (!context.create(cv::ocl::Device::TYPE_CPU))
								isOpenCLInitialized = false;
							else
								isOpenCLInitialized = true;
						}

						if (isOpenCLInitialized)
						{
							cv::ocl::Device(context.device(0));
						}
					}
				}
			}
		}
		regardsParam->SetIsOpenCLOpenGLInteropSupport(openclOpenGLInterop);
		bitmapWndRender->SetOpenCLOpenGLInterop(openclOpenGLInterop);
	}

	renderOpenGL->SetCurrent(*this);

	bitmapWndRender->OnPaint3D(this, renderOpenGL);
}


//-----------------------------------------------------------------
//Gestion du double clique
//-----------------------------------------------------------------
void CBitmapWnd3D::OnLDoubleClick(wxMouseEvent& event)
{
	bitmapWndRender->OnLDoubleClick(event);
}
