#include "OpenCLExecuteProgram.h"
#include "OpenCLContext.h"
#include "utility.h"
using namespace Regards::OpenCL;

#define PAD_LINES 2

COpenCLExecuteProgram::COpenCLExecuteProgram(COpenCLContext * context, const cl_mem_flags &flag)
{
	this->flag = flag;
	this->context = context;
}


COpenCLExecuteProgram::~COpenCLExecuteProgram()
{
	try
	{
		if (kernel)
		{
			clReleaseKernel(kernel);
		}
	}
	catch (...)
	{
		//destructorException();
	}
}

void COpenCLExecuteProgram::ExecuteProgram1D(const cl_program &program, const wxString &kernelName)
{
    
	cl_int err = 0;
	kernel = clCreateKernel(program, kernelName.c_str(), &err);
	Error::CheckError(err);
	ExecuteKernel1D();
}

void COpenCLExecuteProgram::ExecuteProgram(const cl_program &program, const wxString &kernelName)
{
    
	cl_int err = 0;
	kernel = clCreateKernel(program, kernelName.c_str(), &err);
	Error::CheckError(err);
	ExecuteKernel2D();
}

void COpenCLExecuteProgram::SetParameter(vector<COpenCLParameter *> * vecParam, CRegardsBitmap * bitmapOut)
{
	this->vecParam = vecParam;
	data = bitmapOut->GetPtBitmap();
	width = bitmapOut->GetBitmapWidth();
	height = bitmapOut->GetBitmapHeight();
	bitmapSize = (int)bitmapOut->GetBitmapSize();
	//this->bitmapOut = bitmapOut;
}

void COpenCLExecuteProgram::SetParameter(vector<COpenCLParameter *> * vecParam, wxImage * imageOut)
{
	this->vecParam = vecParam;
	data = imageOut->GetData();
	width = imageOut->GetWidth();
	height = imageOut->GetHeight();
	bitmapSize = width * height * 3;
}

void COpenCLExecuteProgram::ExecuteKernel2D()
{
    
	//cl_event        cl_perf_event = nullptr;
	cl_int          err;
	size_t bitmap_size = bitmapSize;
	cl_uchar * p_output = data;
	size_t global_work_size[2] = { static_cast<size_t>(width), static_cast<size_t>(height) };
	//size_t offset[2] = { 0, PAD_LINES };


	cl_mem cl_output_buffer = clCreateBuffer(context->GetContext(), CL_MEM_WRITE_ONLY | flag, bitmap_size, p_output, &err);
	Error::CheckError(err);
	if (cl_output_buffer == (cl_mem)0)
		throw Error("Failed to create Output Buffer!");

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cl_output_buffer);
	Error::CheckError(err);
	

	int numArg = 1;
	for (vector<COpenCLParameter *>::iterator it = vecParam->begin(); it != vecParam->end(); it++)
	{
		COpenCLParameter * parameter = *it;
		parameter->Add(kernel, numArg++);
	}


	err = clEnqueueNDRangeKernel(context->GetCommandQueue(), kernel, sizeof(global_work_size) / sizeof(size_t), nullptr, global_work_size, nullptr, 0, nullptr, nullptr);
	Error::CheckError(err);
	//err = clWaitForEvents(1, &cl_perf_event);
	//Error::CheckError(err);
	err = clFinish(context->GetCommandQueue());
	Error::CheckError(err);

	if (flag == CL_MEM_USE_HOST_PTR)
	{
		void* tmp_ptr = clEnqueueMapBuffer(context->GetCommandQueue(), cl_output_buffer, true, CL_MAP_READ, 0, bitmap_size, 0, nullptr, nullptr, &err);
		Error::CheckError(err);
		if (tmp_ptr != data)
		{// the pointer have to be same because CL_MEM_USE_HOST_PTR option was used in clCreateBuffer
			throw Error("clEnqueueMapBuffer failed to return original pointer");
		}
		//err = clFinish(context->GetCommandQueue());
		//Error::CheckError(err);

		err = clEnqueueUnmapMemObject(context->GetCommandQueue(), cl_output_buffer, tmp_ptr, 0, nullptr, nullptr);
		Error::CheckError(err);
	}
	else
	{
		err = clEnqueueReadBuffer(context->GetCommandQueue(), cl_output_buffer, CL_TRUE, 0, bitmap_size, p_output, 0, nullptr, nullptr);
		Error::CheckError(err);
		err = clFinish(context->GetCommandQueue());
		Error::CheckError(err);
	}

	//if (context->GetCommandQueue())
	//	clReleaseCommandQueue(context->GetCommandQueue());

	err = clReleaseMemObject(cl_output_buffer);
	Error::CheckError(err);

	for (vector<COpenCLParameter *>::iterator it = vecParam->begin(); it != vecParam->end(); it++)
	{
		COpenCLParameter * parameter = *it;
		parameter->Release();
	}

	//delete[] p_output;
}


void COpenCLExecuteProgram::ExecuteKernel1D()
{
    
	cl_event        cl_perf_event = nullptr;
	cl_int          err;

	cl_mem cl_output_buffer = clCreateBuffer(context->GetContext(), CL_MEM_WRITE_ONLY | flag, bitmapSize, data, &err);
	Error::CheckError(err);
	if (cl_output_buffer == (cl_mem)0)
		throw Error("Failed to create Output Buffer!");

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cl_output_buffer);
	Error::CheckError(err);


	int numArg = 1;
	for (vector<COpenCLParameter *>::iterator it = vecParam->begin(); it != vecParam->end(); it++)
	{
		COpenCLParameter * parameter = *it;
		parameter->Add(kernel, numArg++);
	}

	size_t global_size = width * height;
	size_t local_size = 0;// cmd.local_size.getValue();
	cl_uchar * p_output = data;
	// get maximum workgroup size
	size_t local_size_max;
	err = clGetKernelWorkGroupInfo(kernel, context->GetDeviceId(), CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), (void *)&local_size_max, nullptr);
	Error::CheckError(err);

	err = clEnqueueNDRangeKernel(context->GetCommandQueue(), kernel, 1, nullptr, &global_size, local_size ? &local_size : nullptr, 0, nullptr, &cl_perf_event);
	Error::CheckError(err);
	err = clWaitForEvents(1, &cl_perf_event);

	if (flag == CL_MEM_USE_HOST_PTR)
	{
		void* tmp_ptr = clEnqueueMapBuffer(context->GetCommandQueue(), cl_output_buffer, true, CL_MAP_READ, 0, bitmapSize, 0, nullptr, nullptr, &err);
		Error::CheckError(err);
		if (tmp_ptr != data)
		{// the pointer have to be same because CL_MEM_USE_HOST_PTR option was used in clCreateBuffer
			throw Error("clEnqueueMapBuffer failed to return original pointer");
		}
		err = clFinish(context->GetCommandQueue());
		Error::CheckError(err);

		err = clEnqueueUnmapMemObject(context->GetCommandQueue(), cl_output_buffer, tmp_ptr, 0, nullptr, nullptr);
		Error::CheckError(err);
	}
	else
	{
		err = clEnqueueReadBuffer(context->GetCommandQueue(), cl_output_buffer, CL_TRUE, 0, bitmapSize, p_output, 0, nullptr, nullptr);
		Error::CheckError(err);
		err = clFinish(context->GetCommandQueue());
		Error::CheckError(err);
	}

	//if (context->GetCommandQueue())
	//	clReleaseCommandQueue(context->GetCommandQueue());

	err = clReleaseMemObject(cl_output_buffer);
	Error::CheckError(err);

	for (vector<COpenCLParameter *>::iterator it = vecParam->begin(); it != vecParam->end(); it++)
	{
		COpenCLParameter * parameter = *it;
		parameter->Release();
	}
}
