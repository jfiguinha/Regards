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
#include "utility_opencl.h"
#include <LibResource.h>
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
extern std::map<wxString, vector<char>> openclBinaryMapping;
extern string buildOption;

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
	{
		vkdev = ncnn::get_gpu_device(numIntel);
		printf("Vulkan Intel \n");
	}
	else if (platformName.find("NVIDIA") == 0 && findNvidia)
	{
		vkdev = ncnn::get_gpu_device(numNvidia);
		printf("Vulkan NVIDIA \n");
	}
	else if (findAmd)
	{
		vkdev = ncnn::get_gpu_device(numAmd);
		printf("Vulkan AMD \n");
	}
	else
		vkdev = nullptr;

}

cv::ocl::Program COpenCLContext::GetProgram(const wxString & programName)
{
	cv::ocl::Program program;
	cv::String module_name = "REGARDS";
	cv::ocl::Context context = clExecCtx.getContext();
	std::map<wxString, vector<char>>::iterator it;
	it = openclBinaryMapping.find(programName);
	if (it == openclBinaryMapping.end())
	{
		// Récupération du code source du kernel
		wxString kernelSource = CLibResource::GetOpenCLUcharProgram(programName);
		//cv::ocl::ProgramSource programSource(module_name, programName, kernelSource, "");
		// Compilation du kernel
		string errmsg;

		cv::ocl::ProgramSource src(module_name, programName.ToStdString(), kernelSource.ToStdString(), "");
		cv::ocl::Program program(src, buildOption, errmsg);
		if (program.ptr() == NULL)
		{
			std::cout << "Error: " << errmsg << std::endl;
			return program;
		}

		program.getBinary(openclBinaryMapping[programName]);
		std::cout << "Program binary size: " << openclBinaryMapping[programName].size() << " bytes" << std::endl;

	}
	cv::ocl::ProgramSource programSource;
	try
	{
		
		string errmsg;
		programSource = cv::ocl::ProgramSource::fromBinary(module_name, programName.ToStdString(), (uchar*)&openclBinaryMapping[programName][0], openclBinaryMapping[programName].size());
		if (programSource.empty())
		{
			std::cout << "Error: " << std::endl;
			return program;
		}
		program.create(programSource, buildOption, errmsg);
	}
	catch (cv::Exception& e)
	{
		std::cerr << "COpenCLFilter::Interpolation exception caught: " << e.what() << std::endl;
		std::cerr << "Invalid file format. Please input the name of an IMAGE file." << std::endl;

	}

	return program;
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

    //printf("initializeContextFromGL 1\n");
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
   //  printf("initializeContextFromGL 2\n");
	int found = -1;
	cl_device_id device = NULL;
	device = GetListOfDevice(platforms[0], CL_DEVICE_TYPE_GPU, found);

	if (found < 0)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for OpenGL interop");

	cl_context context = NULL;

	// get OpenGL share group
	CGLContextObj cgl_current_context = CGLGetCurrentContext();
	CGLShareGroupObj cgl_share_group = CGLGetShareGroup(cgl_current_context);
    
   //  printf("initializeContextFromGL 3\n");

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

    // printf("initializeContextFromGL 4\n");

	cl_platform_id platform = platforms[0];
	std::string platformName = cv::ocl::PlatformInfo(&platform).name();

	cv::ocl::OpenCLExecutionContext clExecCtx = cv::ocl::OpenCLExecutionContext::create(platformName, platform, context, device);

	cv::ocl::Device(cv::ocl::Device::fromHandle(device));

	clReleaseDevice(device);
	clReleaseContext(context);
	clExecCtx.bind();

    // printf("initializeContextFromGL 5\n");
#else

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
    
    printf("initializeContextFromGL 2\n");

	for (int j = 0; j < platformCompatible.size(); j++)
	{
		int i = platformCompatible[j];
		std::string platformName = cv::ocl::PlatformInfo(&platforms[i]).name();
        
        printf("platformName : %i - %s \n", i, platformName.c_str());

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

printf("initializeContextFromGL wayland\n");

		EGLContext eglContext = eglGetCurrentContext();
		EGLDisplay eglDisplay = eglGetCurrentDisplay();

		cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
		CL_GL_CONTEXT_KHR, (cl_context_properties)eglContext,
		CL_EGL_DISPLAY_KHR,  (cl_context_properties)eglDisplay, 0 };

#else
        
    printf("initializeContextFromGL x11\n");
    
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

        printf("initializeContextFromGL 4\n");

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
        
     printf("initializeContextFromGL 6\n");

	cl_platform_id platform = platforms[found];
	platformName = cv::ocl::PlatformInfo(&platform).name();

	clExecCtx = cv::ocl::OpenCLExecutionContext::create(
		platformName, platform, context, device);
        
    printf("platformName : %i - %s \n", found, platformName.c_str());
        
    printf("initializeContextFromGL 7\n");
	
	clRetainContext(context);

	cv::ocl::Device(cv::ocl::Device::fromHandle(device));
    
    printf("initializeContextFromGL 8\n");

#endif
}

void ShowInfos()
{
	cl_int errNum;
	cl_uint numPlatforms;
	cl_platform_id firstPlatformId;
	cl_context context = NULL;
	cl_device_id device;
	//get Platform and choose first one
	errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
	if (errNum != CL_SUCCESS || numPlatforms <= 0) {
		cerr << "No OpenCL platforum found!" << endl;
		return;
	}

	char buffer[10240];
	printf("=====  Platform 0 =====\n");
	clGetPlatformInfo(firstPlatformId, CL_PLATFORM_PROFILE, 10240, buffer, NULL);
	printf("  PROFILE = %s\n", buffer);
	clGetPlatformInfo(firstPlatformId, CL_PLATFORM_VERSION, 10240, buffer, NULL);
	printf("  VERSION = %s\n", buffer);
	clGetPlatformInfo(firstPlatformId, CL_PLATFORM_NAME, 10240, buffer, NULL);
	printf("  NAME = %s\n", buffer);
	clGetPlatformInfo(firstPlatformId, CL_PLATFORM_VENDOR, 10240, buffer, NULL);
	printf("  VENDOR = %s\n", buffer);
	clGetPlatformInfo(firstPlatformId, CL_PLATFORM_EXTENSIONS, 10240, buffer, NULL);
	printf("  VENDOR = %s\n", buffer);
	//  clGetPlatformInfo(platforms[i],CL_PLATFORM_EXTENSIONS,10240,buffer,NULL);
	//  printf("  EXTENSIONS = %s\n", buffer);

	cl_uint devices_n;

	// get the GPU-devices of platform i, print details of the device
	errNum = clGetDeviceIDs(firstPlatformId, CL_DEVICE_TYPE_CPU, 1, &device,
		&devices_n);
	if (errNum != CL_SUCCESS)
		printf("error getting device IDS\n");
	printf("  === %d OpenCL device(s) found on platform: 0\n\n", devices_n);
	for (unsigned int d = 0; d < devices_n; d++)
	{
		char buffer[10240];
		cl_uint buf_uint;
		cl_ulong buf_ulong;
		cl_bool buf_bool;
		printf("  === --- Device -- %d \n", d);
		(clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(buffer),
			buffer, NULL));
		printf("    DEVICE_NAME = %s\n", buffer);
		(clGetDeviceInfo(device, CL_DEVICE_VENDOR, sizeof(buffer),
			buffer, NULL));
		printf("    DEVICE_VENDOR = %s\n", buffer);
		(clGetDeviceInfo(device, CL_DEVICE_VERSION, sizeof(buffer),
			buffer, NULL));
		printf("    DEVICE_VERSION = %s\n", buffer);
		(clGetDeviceInfo(device, CL_DRIVER_VERSION, sizeof(buffer),
			buffer, NULL));
		printf("    DRIVER_VERSION = %s\n", buffer);
		(clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS,
			sizeof(buf_uint), &buf_uint, NULL));
		printf("    DEVICE_MAX_COMPUTE_UNITS = %u\n", (unsigned int)buf_uint);
		(clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY,
			sizeof(buf_uint), &buf_uint, NULL));
		printf("    DEVICE_MAX_CLOCK_FREQUENCY = %u\n", (unsigned int)buf_uint);
		(clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE,
			sizeof(buf_ulong), &buf_ulong, NULL));
		printf("    DEVICE_GLOBAL_MEM_SIZE = %u\n\n", (unsigned int)buf_ulong);
		(clGetDeviceInfo(device, CL_DEVICE_AVAILABLE,
			sizeof(buf_bool), &buf_bool, NULL));
		printf("    DEVICE_AVAILABLE = %s\n\n", buf_bool ? "Yes" : "No");
	}
	if (devices_n == 0)
	{
		printf("error, on platform 0, there is no GPU device\n");
	}
}

void COpenCLContext::CreateDefaultOpenCLContext()
{
	//ShowInfos();
	
	cv::ocl::Context context;
	if (!context.create(cv::ocl::Device::TYPE_GPU))
		isOpenCLInitialized = false;
	else
		isOpenCLInitialized = true;

	if (!isOpenCLInitialized) {
		cl_int errNum;
		cl_uint numPlatforms;
		cl_platform_id firstPlatformId;
		cl_context _context = NULL;
		cl_device_id device;
		//get Platform and choose first one
		errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
		if (errNum != CL_SUCCESS || numPlatforms <= 0) {
			cerr << "No OpenCL platforum found!" << endl;
			return;
		}

		cl_context_properties contextProperties[3] = {
			  CL_CONTEXT_PLATFORM,
			  (cl_context_properties)firstPlatformId,
			  0
		};
		_context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_ALL, NULL, NULL, &errNum);
		if (errNum != CL_SUCCESS) {
			cerr << "Unable to create GPU or CPU context" << endl;
			//check_error(errNum);
			return;
		}
		else
		{
			context.fromHandle(_context);
			isOpenCLInitialized = true;
		}
			

		cout << "Created CPU context" << endl;
	}

	if (isOpenCLInitialized)
	{
		//cv::ocl::Device(context.device(0));
		clExecCtx = cv::ocl::OpenCLExecutionContext::getCurrent();
		platformName = clExecCtx.getDevice().vendorName();
	}
}

cl_command_queue COpenCLContext::CreateCommandQueue(cl_command_queue_properties queue_properties)
{
	cl_int err = 0;
	cl_command_queue queue = clCreateCommandQueue((cl_context)clExecCtx.getContext().ptr(), (cl_device_id)clExecCtx.getDevice().ptr(), queue_properties, &err);
	Error::CheckError(err);
	return queue;
}


void COpenCLContext::GetOutputData(cl_mem cl_output_buffer, void* dataOut, const int& sizeOutput, const int& flag)
{
	cl_int err = 0;
	cl_command_queue queue = CreateCommandQueue();

	if (flag == CL_MEM_USE_HOST_PTR)
	{
		

		void* tmp_ptr = clEnqueueMapBuffer(queue, cl_output_buffer, true, CL_MAP_READ, 0, sizeOutput, 0, nullptr, nullptr, &err);
		ErrorOpenCL::CheckError(err);
		if (tmp_ptr != dataOut)
		{// the pointer have to be same because CL_MEM_USE_HOST_PTR option was used in clCreateBuffer
			throw ErrorOpenCL("clEnqueueMapBuffer failed to return original pointer");
		}

		err = clFinish(queue);
		ErrorOpenCL::CheckError(err);

		err = clEnqueueUnmapMemObject(queue, cl_output_buffer, tmp_ptr, 0, nullptr, nullptr);
		ErrorOpenCL::CheckError(err);
	}
	else
	{
		err = clEnqueueReadBuffer(queue, cl_output_buffer, CL_TRUE, 0, sizeOutput, dataOut, 0, nullptr, nullptr);
		ErrorOpenCL::CheckError(err);
		err = clFinish(queue);
		ErrorOpenCL::CheckError(err);
	}


	if (queue)
	{
		cl_int err = clReleaseCommandQueue(queue);
		ErrorOpenCL::CheckError(err);
	}
}
