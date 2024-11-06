#include "header.h"
#include "OpenCLContext.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <OpenGL/OpenGL.h>
#else
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <CL/cl_gl_ext.h>
#endif

#include <epoxy/gl.h>

#ifdef WIN32
#include <epoxy/wgl.h>
#endif

#ifdef __WXGTK__
#include <epoxy/glx.h>
#if wxUSE_GLCANVAS_EGL == 1
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif
#endif



#include <utility.h>
#include <ncnn/gpu.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>

#if defined (__APPLE__) || defined(MACOSX)
static const char* CL_GL_SHARING_EXT = "cl_APPLE_gl_sharing";
#else
static const char* CL_GL_SHARING_EXT = "cl_khr_gl_sharing";
#endif

extern string platformName;
extern cv::ocl::OpenCLExecutionContext clExecCtx;
extern bool isOpenCLInitialized;
extern ncnn::VulkanDevice* vkdev;
using namespace Regards::OpenCL;

void COpenCLContext::AssociateToVulkan()
{

	bool findNvidia = false;
	bool findIntel = false;
	bool findAmd = false;
	int numNvidia = 0;
	int numAmd = 0;
	int numIntel = 0;
	int select = 0;
	cl_uint numPlatforms = ncnn::get_gpu_count();
	for (int i = 0; i < numPlatforms; i++)
	{
		const ncnn::GpuInfo& pguInfo = ncnn::get_gpu_info(i);
		string deviceName = pguInfo.device_name();
		for (auto& c : deviceName)
		{
			c = tolower(c);
		}
		if (deviceName.find("nvidia") != std::string::npos)
		{
			findNvidia = true;
			numNvidia = i;
		}

		if (deviceName.find("amd") != std::string::npos)
		{
			findAmd = true;
			numAmd = i;
		}

		if (deviceName.find("intel") != std::string::npos)
		{
			findIntel = true;
			numIntel = i;
		}
	}

	if (platformName.find("Intel") == 0 && findIntel)
		vkdev = ncnn::get_gpu_device(numIntel);
	else if(platformName.find("NVIDIA") == 0 && findNvidia)
		vkdev = ncnn::get_gpu_device(numNvidia);
	else if (findAmd)
		vkdev = ncnn::get_gpu_device(numAmd);

}

wxString COpenCLContext::GetDeviceInfo(cl_device_id device, cl_device_info param_name)
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

cl_device_id COpenCLContext::GetListOfDevice(cl_platform_id platform, cl_device_type device_type, int& found)
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
		//printf("Device found : %s \n", CConvertUtility::ConvertToUTF8(deviceName));
		break;
	}

	if (found == -1)
		return nullptr;
	return devices[found];
}

void COpenCLContext::initializeContextFromGL()
{
#if defined(__APPLE__) || defined(__MACOSX)

    printf("initializeContextFromGL 1\n");
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
     printf("initializeContextFromGL 2\n");
	int found = -1;
	cl_device_id device = NULL;
	device = GetListOfDevice(platforms[0], CL_DEVICE_TYPE_GPU, found);

	if (found < 0)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for OpenGL interop");

	cl_context context = NULL;

	// get OpenGL share group
	CGLContextObj cgl_current_context = CGLGetCurrentContext();
	CGLShareGroupObj cgl_share_group = CGLGetShareGroup(cgl_current_context);
    
     printf("initializeContextFromGL 3\n");

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

     printf("initializeContextFromGL 4\n");

	cl_platform_id platform = platforms[0];
	std::string platformName = cv::ocl::PlatformInfo(&platform).name();

	cv::ocl::OpenCLExecutionContext clExecCtx = cv::ocl::OpenCLExecutionContext::create(platformName, platform, context, device);

	cv::ocl::Device(cv::ocl::Device::fromHandle(device));

	AssociateToVulkan();

	clReleaseDevice(device);
	clReleaseContext(context);
	clExecCtx.bind();

     printf("initializeContextFromGL 5\n");
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

	vector<int> platformCompatible;

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

		platformCompatible.push_back(i);
	}

	for (int j = 0; j < platformCompatible.size(); j++)
	{
		int i = platformCompatible[j];
		std::string platformName = cv::ocl::PlatformInfo(&platforms[i]).name();

		clGetGLContextInfoKHR_fn clGetGLContextInfoKHR = (clGetGLContextInfoKHR_fn)
			clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetGLContextInfoKHR");

#ifdef WIN32
		// Create CL context properties, add WGL context & handle to DC
		cl_context_properties properties[] = {
			CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i], // OpenCL platform
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), // WGL Context
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(), // WGL HDC
			0
		};

		/*
		// create context properties listing the platform and current OpenGL display
		cl_context_properties properties[] = {
			CL_CONTEXT_PLATFORM, (cl_context_properties)platform.id(),
		#if defined(__linux__)
			CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
			CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
		#elif defined(_WIN32)
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		#endif
			0
		};
		*/

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
			&device, 0);
		if (status != CL_SUCCESS)
			continue;

		// create context
		context = clCreateContext(properties, 1, &device, NULL, NULL, &status);
		if (status != CL_SUCCESS)
		{
			clReleaseDevice(device);
			found = -1;
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
	platformName = cv::ocl::PlatformInfo(&platform).name();

	clExecCtx = cv::ocl::OpenCLExecutionContext::create(
		platformName, platform, context, device);

	clRetainContext(context);

	cv::ocl::Device(cv::ocl::Device::fromHandle(device));

	AssociateToVulkan();

#endif
}

void COpenCLContext::CreateDefaultOpenCLContext()
{
	
	cv::ocl::Context context;
	if (!context.create(cv::ocl::Device::TYPE_GPU))
		isOpenCLInitialized = false;
	else
		isOpenCLInitialized = true;


	if (isOpenCLInitialized)
	{
		cv::ocl::Device(context.device(0));
		clExecCtx = cv::ocl::OpenCLExecutionContext::getCurrent();
		platformName = clExecCtx.getDevice().vendorName();
		AssociateToVulkan();
	}



}
