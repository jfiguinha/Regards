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
	//Todo Modify to support change in parameter
	if (sharedContextCompatible)
		return OPENCL_FLOAT;
	return OPENCL_UCHAR;
}

COpenCLContext::COpenCLContext(compute::context& context, const bool& opengl): context(nullptr)
{
	this->isOpenGL = opengl;
	this->context = context;
	if (isOpenGL)
		sharedContextCompatible = true;
	else
		sharedContextCompatible = false;
	queue = compute::command_queue(context, context.get_device());

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
