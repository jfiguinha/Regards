// ReSharper disable All
#include <header.h>
#include "OpenCLContext.h"
#include "utility.h"
#include "OpenCLProgram.h"
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <OpenGL/OpenGL.h>
#elif defined(__WXGTK__)
#include <GL/glx.h>
#include <CL/cl_gl.h>
#else
#include <CL/cl_gl.h>
#endif
#include <LibResource.h>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
using namespace Regards::OpenCL;


#if defined (__APPLE__) || defined(MACOSX)
static const char* CL_GL_SHARING_EXT = "cl_APPLE_gl_sharing";
#else
static const char* CL_GL_SHARING_EXT = "cl_khr_gl_sharing";
#endif

int COpenCLContext::GetDefaultType()
{
	//Todo Modify to support change in parameter
	if (sharedContextCompatible)
		return OPENCL_FLOAT;
	return OPENCL_UCHAR;
}

COpenCLContext::COpenCLContext(const bool& opengl): context(nullptr)
{
	this->isOpenGL = opengl;
	queue = nullptr;
	context = nullptr;
	GenerateContext();
}

void COpenCLContext::SetPlatformName(const wxString& platform_name)
{
	this->platform_name = platform_name;
}

void COpenCLContext::GetOutputData(cl_mem cl_output_buffer, void* dataOut, const int& sizeOutput, const int& flag)
{
	cl_int err = 0;
	try
	{
		if (flag == CL_MEM_USE_HOST_PTR)
		{
			void* tmp_ptr = clEnqueueMapBuffer(GetCommandQueue(), cl_output_buffer, true, CL_MAP_READ, 0, sizeOutput, 0,
			                                   nullptr, nullptr, &err);
			Error::CheckError(err);
			if (tmp_ptr != dataOut)
			{
				// the pointer have to be same because CL_MEM_USE_HOST_PTR option was used in clCreateBuffer
				throw Error("clEnqueueMapBuffer failed to return original pointer");
			}

			err = clFinish(GetCommandQueue());
			Error::CheckError(err);

			err = clEnqueueUnmapMemObject(GetCommandQueue(), cl_output_buffer, tmp_ptr, 0, nullptr, nullptr);
			Error::CheckError(err);
		}
		else
		{
			err = clEnqueueReadBuffer(GetCommandQueue(), cl_output_buffer, CL_TRUE, 0, sizeOutput, dataOut, 0, nullptr,
			                          nullptr);
			Error::CheckError(err);
			err = clFinish(GetCommandQueue());
			Error::CheckError(err);
		}
	}
	catch (...)
	{
		wxString copyvideoData = CLibResource::LoadStringFromResource("LBLERRORCOPYVIDEODATA", 1);
		wxString infos = CLibResource::LoadStringFromResource("informationserror", 1);
		wxMessageBox(copyvideoData, infos);
	}
}

COpenCLProgram* COpenCLContext::GetProgram(const wxString& numProgramId, const int& type)
{
	bool findPreloadShader = false;
	COpenCLProgram* program = nullptr;
	for (COpenCLProgram* programLocal : listProgram)
	{
		program = programLocal;
		if (program->GetProgramId() == numProgramId && program->GetDefaultType() == type)
		{
			findPreloadShader = true;
			break;
		}
	}

	//Ajout du program en pré chargement
	if (!findPreloadShader)
	{
		program = new COpenCLProgram(this, type);
		program->SetProgramId(numProgramId);
		listProgram.push_back(program);
	}

	return program;
}

COpenCLProgram* COpenCLContext::GetProgram(const wxString& numProgramId)
{
	return GetProgram(numProgramId, GetDefaultType());
}

COpenCLContext::~COpenCLContext()
{
	try
	{
		// Release objects in the opposite order of creation

		if (queue)
		{
			cl_int err = clReleaseCommandQueue(queue);

			Error::CheckError(err);
		}

		if (context)
		{
			cl_int err = clReleaseContext(context);

			Error::CheckError(err);
		}
	}
	catch (...)
	{
		//destructorException();
	}


	for (COpenCLProgram* program : listProgram)
	{
		delete program;
		program = nullptr;
	}

	listProgram.clear();
}


int COpenCLContext::GenerateContext()
{
	try
	{
		CreateContext();
		CreateQueue();
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}


void COpenCLContext::CreateQueue(cl_command_queue_properties queue_properties)
{
	if (!device)
	{
		throw Error("Device is not selected");
	}

	cl_int err = 0;
	queue = clCreateCommandQueue(context, device, queue_properties, &err);
	Error::CheckError(err);
}

vector<cl_platform_id> COpenCLContext::GetListOfPlatform()
{
	printf("GetListOfPlatform \n");
	Error error("error");
	cl_uint num_of_platforms = 0;
	// get total number of available platforms:
	cl_int err = clGetPlatformIDs(0, nullptr, &num_of_platforms);
	error.CheckError(err);

	// use vector for automatic memory management
	vector<cl_platform_id> platforms(num_of_platforms);
	// get IDs for all platforms:
	err = clGetPlatformIDs(num_of_platforms, &platforms[0], nullptr);
	error.CheckError(err);

	//cl_uint selected_platform_index = num_of_platforms;
	//bool by_index = false;

	//cout << "Platforms (" << num_of_platforms << "):\n";

	// TODO In case of empty platform name select the default platform or 0th platform?

	for (cl_uint i = 0; i < num_of_platforms; ++i)
	{
		// Get the length for the i-th platform name
		size_t platform_name_length = 0;
		err = clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			0,
			nullptr,
			&platform_name_length
		);
		error.CheckError(err);


		// Get the name itself for the i-th platform
		// use vector for automatic memory management
		vector<char> platform_name(platform_name_length);
		err = clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			platform_name_length,
			&platform_name[0],
			nullptr
		);
		error.CheckError(err);
	}

	return platforms;
}

wxString COpenCLContext::GetDeviceInfo(cl_device_id device, cl_device_info param_name)
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

cl_device_id COpenCLContext::GetDevice(cl_platform_id platform, cl_device_type device_type, const bool &openglCompatible, bool &findDevice)
{
	findDevice = false;
	cl_device_id deviceId;
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
		deviceId = devices[i];
		int supported = 0;
		cl_device_type type;
		clGetDeviceInfo(devices[i], CL_DEVICE_TYPE, sizeof(type), &type, nullptr);
		wxString deviceName = GetDeviceInfo(deviceId, CL_DEVICE_NAME);

		if (type == CL_DEVICE_TYPE_GPU && !openglCompatible)
		{
			findDevice = true;
			return deviceId;
		}
		else if (type == CL_DEVICE_TYPE_GPU && openglCompatible)
		{
			wxString supportExt = GetDeviceInfo(deviceId, CL_DEVICE_EXTENSIONS);
			supported = supportExt.find(CL_GL_SHARING_EXT);
			if (supported > 0)
			{
				findDevice = true;
				return deviceId;
			}
		}
		else
		{
			findDevice = true;
			return deviceId;
		}
	}

	return deviceId;
}

void COpenCLContext::CreateContext()
{
	vector<cl_platform_id> platforms = GetListOfPlatform();

	if (isOpenGL)
	{
		bool findDevice = false;

		for (cl_platform_id _platform : platforms)
		{

			platform = _platform;
			device = GetDevice(_platform, CL_DEVICE_TYPE_GPU, true, findDevice);
			if (findDevice)
				break;
		}

		if (findDevice)
		{

#ifdef WIN32
			// Create CL context properties, add WGL context & handle to DC
			cl_context_properties properties[] = {
				CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), // WGL Context
				CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(), // WGL HDC
				CL_CONTEXT_PLATFORM, (cl_context_properties)platform, // OpenCL platform
				0
			};
#elif defined(__WXGTK__)

			// Create CL context properties, add GLX context & handle to DC
			cl_context_properties properties[] = {
			 CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(), // GLX Context
			 CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(), // GLX Display
			 CL_CONTEXT_PLATFORM, (cl_context_properties)platform, // OpenCL platform
			 0
			};
#elif defined(__APPLE__)

			// Get current CGL Context and CGL Share group
			CGLContextObj kCGLContext = CGLGetCurrentContext();
			CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
			// Create CL context properties, add handle & share-group enum
			cl_context_properties properties[] = {
				CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
			(cl_context_properties)kCGLShareGroup, 0
			};

#endif

			cl_int err = 0;
			context = clCreateContext(properties, 1, &device, nullptr, nullptr, &err);
			//Error::CheckError(err);
			if (err == CL_SUCCESS)
			{
				sharedContextCompatible = true;
				return;
			}
			else
			{
				sharedContextCompatible = false;

				//Generate context without shared compatibility
				cl_context_properties context_props[] = {
					CL_CONTEXT_PLATFORM,
					cl_context_properties(platform),
					0
				};

				cl_int err = 0;
				context = clCreateContext(context_props, 1, &device, nullptr, nullptr, &err);
				Error::CheckError(err);
			}

			std::cerr << "Unable to find a compatible OpenCL device for openGL sharing." << std::endl;
			std::cerr << "Create a compatible OpenCL context." << std::endl;
		}
		else
		{
			//Try to find GPU device
			bool findDevice = false;

			for (cl_platform_id _platform : platforms)
			{

				platform = _platform;
				device = GetDevice(_platform, CL_DEVICE_TYPE_GPU, false, findDevice);
				if (findDevice)
					break;
			}


			//Try to find CPU device if not found GPU device
			if (!findDevice)
			{
				for (cl_platform_id _platform : platforms)
				{

					platform = _platform;
					device = GetDevice(_platform, CL_DEVICE_TYPE_CPU, false, findDevice);
					if (findDevice)
						break;
				}

			}

			sharedContextCompatible = false;

			//Generate context without shared compatibility
			cl_context_properties context_props[] = {
				CL_CONTEXT_PLATFORM,
				cl_context_properties(platform),
				0
			};

			cl_int err = 0;
			context = clCreateContext(context_props, 1, &device, nullptr, nullptr, &err);
			Error::CheckError(err);
		}
	}

}
