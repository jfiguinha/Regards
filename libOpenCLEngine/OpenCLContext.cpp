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

int COpenCLContext::GetDefaultType()
{
	return OPENCL_UCHAR;
}

COpenCLContext::COpenCLContext(cl_platform_id platformId, const wxString& platformName, cl_device_id deviceId,
                               cl_device_type deviceType, const bool& opengl): context(nullptr)
{
	this->isOpenGL = opengl;
	platform = platformId;
	device = deviceId;
	this->platform_name = platformName;
	this->deviceType = deviceType;
	queue = nullptr;
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
		program = new COpenCLProgram(type);
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

void COpenCLContext::CreateContext()
{
	if (!platform)
	{
		throw Error("Platform is not selected");
	}

	if (!device)
	{
		throw Error("Device is not selected");
	}

	if (isOpenGL)
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

		// Find CL capable devices in the current GL context
		//cl_device_id devices[32]; size_t size;
		//clGetGLContextInfoKHR(properties, CL_DEVICES_FOR_GL_CONTEXT_KHR, 32 * sizeof(cl_device_id), devices, &size);

		cl_int err = 0;
		context = clCreateContext(properties, 1, &device, nullptr, nullptr, &err);
		//Error::CheckError(err);
		if (err == CL_SUCCESS)
		{
			sharedContextCompatible = true;
			return;
		}
		std::cerr << "Unable to find a compatible OpenCL device for openGL sharing." << std::endl;
		std::cerr << "Create a compatible OpenCL context." << std::endl;
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
