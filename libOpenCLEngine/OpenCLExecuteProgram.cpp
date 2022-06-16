#include <header.h>
#include "OpenCLExecuteProgram.h"
#include "OpenCLContext.h"
#include <RegardsBitmap.h>
#include <RegardsFloatBitmap.h>
#include "utility.h"
using namespace Regards::OpenCL;
extern COpenCLContext* openclContext;
#define PAD_LINES 2

COpenCLExecuteProgram::COpenCLExecuteProgram(const cl_mem_flags& flag): kernel(nullptr),
	data(nullptr),
	width(0),
	height(0), bitmapSize(0),
	vecParam(nullptr)
{
	this->flag = flag;
	keepMemory = false;
	cl_output_buffer = nullptr;
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

void COpenCLExecuteProgram::ExecuteProgram1D(const cl_program& program, const wxString& kernelName,
                                             const size_t& global_size, const size_t& outputBufferSize)
{
	cl_int err = 0;
	kernel = clCreateKernel(program, kernelName.c_str(), &err);
	Error::CheckError(err);
	ExecuteKernel1D(global_size, outputBufferSize);
}

void COpenCLExecuteProgram::ExecuteProgram1D(const cl_program& program, const wxString& kernelName)
{
	cl_int err = 0;
	kernel = clCreateKernel(program, kernelName.c_str(), &err);
	Error::CheckError(err);
	size_t global_size = width * height;
	ExecuteKernel1D(global_size, bitmapSize);
}

void COpenCLExecuteProgram::ExecuteProgram(const cl_program& program, const wxString& kernelName)
{
	cl_int err = 0;
	kernel = clCreateKernel(program, kernelName.c_str(), &err);
	Error::CheckError(err);
	ExecuteKernel2D(bitmapSize);
}

void COpenCLExecuteProgram::ExecuteProgram(const cl_program& program, const wxString& kernelName,
                                           const size_t& outputBufferSize)
{
	cl_int err = 0;
	kernel = clCreateKernel(program, kernelName.c_str(), &err);
	Error::CheckError(err);
	ExecuteKernel2D(outputBufferSize);
}


void COpenCLExecuteProgram::ExecuteProgram2D(const cl_program& program, const wxString& kernelName,
                                             vector<COpenCLParameter*>* vecParam, size_t* offset, size_t* gs_d,
                                             size_t* ls)
{
	this->vecParam = vecParam;
	cl_int err = 0;
	kernel = clCreateKernel(program, kernelName.c_str(), &err);
	Error::CheckError(err);
	ExecuteKernel2D(offset, gs_d, ls);
}

void COpenCLExecuteProgram::SetParameter(vector<COpenCLParameter*>* vecParam, CRegardsBitmap* bitmapOut)
{
	this->vecParam = vecParam;
	data = bitmapOut->GetPtBitmap();
	width = bitmapOut->GetBitmapWidth();
	height = bitmapOut->GetBitmapHeight();
	bitmapSize = static_cast<int>(bitmapOut->GetBitmapSize());
	//this->bitmapOut = bitmapOut;
}

void COpenCLExecuteProgram::SetParameter(vector<COpenCLParameter*>* vecParam, CRegardsFloatBitmap* bitmapOut)
{
	this->vecParam = vecParam;
	data = bitmapOut->GetData();
	width = bitmapOut->GetWidth();
	height = bitmapOut->GetHeight();
	bitmapSize = static_cast<int>(bitmapOut->GetSize());
	//this->bitmapOut = bitmapOut;
}

void COpenCLExecuteProgram::SetParameter(vector<COpenCLParameter*>* vecParam, wxImage* imageOut)
{
	this->vecParam = vecParam;
	data = imageOut->GetData();
	width = imageOut->GetWidth();
	height = imageOut->GetHeight();
	bitmapSize = width * height * 3;
}

void COpenCLExecuteProgram::SetParameter(vector<COpenCLParameter*>* vecParam, int width, int height, int sizeOutput)
{
	this->vecParam = vecParam;
	this->width = width;
	this->height = height;
	data = nullptr;
	bitmapSize = sizeOutput;
}

void COpenCLExecuteProgram::SetParameter(vector<COpenCLParameter*>* vecParam, int width, int height, cl_mem output)
{
	this->vecParam = vecParam;
	cl_output_buffer = output;
	this->width = width;
	this->height = height;
}

void COpenCLExecuteProgram::SetParameter(vector<COpenCLParameter*>* vecParam, int sizeOutput, int width, int height,
                                         void* dataPt)
{
	this->vecParam = vecParam;
	data = dataPt;
	this->width = width;
	this->height = height;
	bitmapSize = sizeOutput;
}

void COpenCLExecuteProgram::SetParameter(vector<COpenCLParameter*>* vecParam, int width, int height, void* dataPt)
{
	this->vecParam = vecParam;
	data = dataPt;
	this->width = width;
	this->height = height;
	bitmapSize = width * height * 4;
}


void COpenCLExecuteProgram::ExecuteKernel2D(const size_t& outputBufferSize)
{
	//cl_event        cl_perf_event = nullptr;
	cl_int err = 0;
	size_t global_work_size[2] = {static_cast<size_t>(width), static_cast<size_t>(height)};
	//size_t offset[2] = { 0, PAD_LINES };
	cl_command_queue q = openclContext->GetCommandQueue();

	if (cl_output_buffer == static_cast<cl_mem>(nullptr))
	{
		if (data == nullptr)
			cl_output_buffer = clCreateBuffer(openclContext->GetContext(), CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR,
			                                  outputBufferSize, nullptr, &err);
		else
			cl_output_buffer = clCreateBuffer(openclContext->GetContext(), CL_MEM_READ_WRITE | flag, outputBufferSize, data,
			                                  &err);
		Error::CheckError(err);
	}

	if (cl_output_buffer == static_cast<cl_mem>(nullptr))
		throw Error("Failed to create Output Buffer!");

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_output_buffer);
	Error::CheckError(err);


	int numArg = 1;
	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		parameter->Add(kernel, numArg++);
	}

	cl_event _event;
	err = clEnqueueNDRangeKernel(q, kernel, sizeof(global_work_size) / sizeof(size_t), nullptr,
	                             global_work_size, nullptr, 0, nullptr, &_event);
	Error::CheckError(err);

	clWaitForEvents(1, &_event);

	err = clFlush(q);
	Error::CheckError(err);

	err = clFinish(q);
	Error::CheckError(err);

	//wxMilliSleep(1000);

	if (!keepMemory)
	{
		if (flag == CL_MEM_USE_HOST_PTR)
		{
			void* tmp_ptr = clEnqueueMapBuffer(q, cl_output_buffer, true, CL_MAP_READ, 0,
			                                   outputBufferSize, 0, nullptr, nullptr, &err);
			Error::CheckError(err);
			if (tmp_ptr != data)
			{
				// the pointer have to be same because CL_MEM_USE_HOST_PTR option was used in clCreateBuffer
				throw Error("clEnqueueMapBuffer failed to return original pointer");
			}

			err = clFinish(q);
			Error::CheckError(err);

			err = clEnqueueUnmapMemObject(q, cl_output_buffer, tmp_ptr, 0, nullptr, nullptr);
			Error::CheckError(err);
		}
		else
		{
			err = clEnqueueReadBuffer(q, cl_output_buffer, CL_TRUE, 0, outputBufferSize, data,
			                          0, nullptr, nullptr);
			Error::CheckError(err);
			err = clFinish(q);
			Error::CheckError(err);
		}

		err = clReleaseMemObject(cl_output_buffer);
		Error::CheckError(err);
	}
	

	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		if (!parameter->GetNoDelete())
			parameter->Release();
	}
}

void COpenCLExecuteProgram::ExecuteProgram2D(const cl_program& program, const wxString& kernelName,
                                             vector<COpenCLParameter*>* vecParam, int width, int height)
{
	cl_command_queue q = openclContext->GetCommandQueue();
	cl_int err = 0;
	kernel = clCreateKernel(program, kernelName.c_str(), &err);
	Error::CheckError(err);

	size_t global_work_size[2] = {static_cast<size_t>(width), static_cast<size_t>(height)};

	int numArg = 0;
	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		parameter->Add(kernel, numArg++);
	}
	cl_event _event;
	err = clEnqueueNDRangeKernel(q, kernel, sizeof(global_work_size) / sizeof(size_t), nullptr,
	                             global_work_size, nullptr, 0, nullptr, &_event);
	Error::CheckError(err);

	clWaitForEvents(1, &_event);

	err = clFlush(q);
	Error::CheckError(err);

	err = clFinish(q);
	Error::CheckError(err);

	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		if (!parameter->GetNoDelete())
			parameter->Release();
	}
}


void COpenCLExecuteProgram::ExecuteKernel2D(size_t* offset, size_t* gs_d, size_t* ls)
{
	cl_int err;
	cl_command_queue q = openclContext->GetCommandQueue();
	int numArg = 0;
	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		parameter->Add(kernel, numArg++);
	}

	size_t local_size_max;
	err = clGetKernelWorkGroupInfo(kernel, openclContext->GetDeviceId(), CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t),
	                               &local_size_max, nullptr);
	Error::CheckError(err);

	cl_event event;

	//clEnqueueNDRangeKernel(queue, dist_kernel, 2, offset, gs_d, ls, 0, NULL, &event)
	err = clEnqueueNDRangeKernel(q, kernel, 2, offset, gs_d, ls, 0, nullptr, &event);
	Error::CheckError(err);
	clWaitForEvents(1, &event);
	err = clFlush(q);
	Error::CheckError(err);

	err = clFinish(q);
	Error::CheckError(err);

	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		if (!parameter->GetNoDelete())
			parameter->Release();
	}
}

cl_mem COpenCLExecuteProgram::GetOutput()
{
	return cl_output_buffer;
}

void COpenCLExecuteProgram::SetKeepOutput(const bool& keepMemory)
{
	this->keepMemory = keepMemory;
}

void COpenCLExecuteProgram::ExecuteKernel1D(const size_t& global_size, const size_t& outputBufferSize)
{
	cl_event cl_perf_event = nullptr;
	cl_int err;
	cl_command_queue q = openclContext->GetCommandQueue();

	if (cl_output_buffer == static_cast<cl_mem>(nullptr))
	{
		if (data == nullptr)
			cl_output_buffer = clCreateBuffer(openclContext->GetContext(), CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR,
			                                  outputBufferSize, data, &err);
		else
			cl_output_buffer = clCreateBuffer(openclContext->GetContext(), CL_MEM_READ_WRITE | flag, outputBufferSize, data,
			                                  &err);
		Error::CheckError(err);
	}

	if (cl_output_buffer == static_cast<cl_mem>(nullptr))
		throw Error("Failed to create Output Buffer!");

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_output_buffer);
	Error::CheckError(err);


	int numArg = 1;
	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		parameter->Add(kernel, numArg++);
	}


	size_t local_size = 0;

	// get maximum workgroup size
	size_t local_size_max;
	err = clGetKernelWorkGroupInfo(kernel, openclContext->GetDeviceId(), CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t),
	                               &local_size_max, nullptr);
	Error::CheckError(err);

	err = clEnqueueNDRangeKernel(q, kernel, 1, nullptr, &global_size,
	                             local_size ? &local_size : nullptr, 0, nullptr, &cl_perf_event);
	clWaitForEvents(1, &cl_perf_event);
	err = clFlush(q);
	Error::CheckError(err);

	err = clFinish(q);
	Error::CheckError(err);

	if (!keepMemory)
	{
		if (flag == CL_MEM_USE_HOST_PTR)
		{
			void* tmp_ptr = clEnqueueMapBuffer(q, cl_output_buffer, true, CL_MAP_READ, 0,
			                                   bitmapSize, 0, nullptr, nullptr, &err);
			Error::CheckError(err);
			if (tmp_ptr != data)
			{
				// the pointer have to be same because CL_MEM_USE_HOST_PTR option was used in clCreateBuffer
				throw Error("clEnqueueMapBuffer failed to return original pointer");
			}
			err = clFinish(q);
			Error::CheckError(err);

			err = clEnqueueUnmapMemObject(q, cl_output_buffer, tmp_ptr, 0, nullptr, nullptr);
			Error::CheckError(err);
		}
		else
		{
			err = clEnqueueReadBuffer(q, cl_output_buffer, CL_TRUE, 0, bitmapSize, data, 0,
			                          nullptr, nullptr);
			Error::CheckError(err);
			err = clFinish(q);
			Error::CheckError(err);
		}

		//if (context->GetCommandQueue())
		//	clReleaseCommandQueue(context->GetCommandQueue());

		err = clReleaseMemObject(cl_output_buffer);
		Error::CheckError(err);
	}
	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		if (!parameter->GetNoDelete())
			parameter->Release();
	}
}
