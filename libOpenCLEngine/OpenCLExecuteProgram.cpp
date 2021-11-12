#include <header.h>
#include "OpenCLExecuteProgram.h"
#include "OpenCLContext.h"
#include <RegardsBitmap.h>
#include <RegardsFloatBitmap.h>
#include "utility.h"
using namespace Regards::OpenCL;

#define PAD_LINES 2

COpenCLExecuteProgram::COpenCLExecuteProgram(COpenCLContext* context, const cl_mem_flags& flag): kernel(nullptr),
	data(nullptr),
	width(0),
	height(0), bitmapSize(0),
	vecParam(nullptr)
{
	this->flag = flag;
	this->context = context;
	keepMemory = false;
	cl_output_buffer = nullptr;
}


COpenCLExecuteProgram::~COpenCLExecuteProgram()
{
}

void COpenCLExecuteProgram::ExecuteProgram1D(const compute::program & program, const wxString& kernelName,
                                             const size_t& global_size, const size_t& outputBufferSize)
{
	try
	{
		compute::kernel _kernel(program, kernelName.ToStdString());
		kernel = _kernel;
		ExecuteKernel1D(global_size, outputBufferSize);
	}
	catch (...)
	{

	}
}

void COpenCLExecuteProgram::ExecuteProgram1D(const compute::program & program, const wxString& kernelName)
{
	try
	{
		compute::kernel _kernel(program, kernelName.ToStdString());
		kernel = _kernel;
		size_t global_size = width * height;
		ExecuteKernel1D(global_size, bitmapSize);
	}
	catch (...)
	{

	}
	
}

void COpenCLExecuteProgram::ExecuteProgram(const compute::program & program, const wxString& kernelName)
{
	try
	{
		compute::kernel _kernel(program, kernelName.ToStdString());
		kernel = _kernel;
		ExecuteKernel2D(bitmapSize);
	}
	catch (...)
	{

	}
}

void COpenCLExecuteProgram::ExecuteProgram(const compute::program & program, const wxString& kernelName,
                                           const size_t& outputBufferSize)
{
	try
	{
		compute::kernel _kernel(program, kernelName.ToStdString());
		kernel = _kernel;
		ExecuteKernel2D(outputBufferSize);
	}
	catch (...)
	{

	}
}


void COpenCLExecuteProgram::ExecuteProgram2D(const compute::program & program, const wxString& kernelName,
                                             vector<COpenCLParameter*>* vecParam, size_t* offset, size_t* gs_d,
                                             size_t* ls)
{
	this->vecParam = vecParam;
	try
	{
		compute::kernel _kernel(program, kernelName.ToStdString());
		kernel = _kernel;
		ExecuteKernel2D(offset, gs_d, ls);
	}
	catch (...)
	{

	}
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

	if (cl_output_buffer == static_cast<cl_mem>(nullptr))
	{
		if (data == nullptr)
			cl_output_buffer = clCreateBuffer(context->GetContext(), CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR,
			                                  outputBufferSize, nullptr, &err);
		else
			cl_output_buffer = clCreateBuffer(context->GetContext(), CL_MEM_READ_WRITE | flag, outputBufferSize, data,
			                                  &err);
		Error::CheckError(err);
	}

	if (cl_output_buffer == static_cast<cl_mem>(nullptr))
		throw Error("Failed to create Output Buffer!");

	//err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_output_buffer);
	//Error::CheckError(err);
	kernel.set_arg(0, sizeof(cl_mem), &cl_output_buffer);


	int numArg = 1;
	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		parameter->Add(kernel, numArg++);
	}


	err = clEnqueueNDRangeKernel(context->GetCommandQueue(), kernel, sizeof(global_work_size) / sizeof(size_t), nullptr,
	                             global_work_size, nullptr, 0, nullptr, nullptr);
	Error::CheckError(err);

	err = clFinish(context->GetCommandQueue());
	Error::CheckError(err);

	if (!keepMemory)
	{
		if (flag == CL_MEM_USE_HOST_PTR)
		{
			void* tmp_ptr = clEnqueueMapBuffer(context->GetCommandQueue(), cl_output_buffer, true, CL_MAP_READ, 0,
			                                   outputBufferSize, 0, nullptr, nullptr, &err);
			Error::CheckError(err);
			if (tmp_ptr != data)
			{
				// the pointer have to be same because CL_MEM_USE_HOST_PTR option was used in clCreateBuffer
				throw Error("clEnqueueMapBuffer failed to return original pointer");
			}

			err = clFinish(context->GetCommandQueue());
			Error::CheckError(err);

			err = clEnqueueUnmapMemObject(context->GetCommandQueue(), cl_output_buffer, tmp_ptr, 0, nullptr, nullptr);
			Error::CheckError(err);
		}
		else
		{
			err = clEnqueueReadBuffer(context->GetCommandQueue(), cl_output_buffer, CL_TRUE, 0, outputBufferSize, data,
			                          0, nullptr, nullptr);
			Error::CheckError(err);
			err = clFinish(context->GetCommandQueue());
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

void COpenCLExecuteProgram::ExecuteProgram2D(const compute::program & program, const wxString& kernelName,
                                             vector<COpenCLParameter*>* vecParam, int width, int height)
{
	try
	{
		compute::kernel _kernel(program, kernelName.ToStdString());
		kernel = _kernel;
		cl_int err = 0;

		size_t global_work_size[2] = {static_cast<size_t>(width), static_cast<size_t>(height)};

		int numArg = 0;
		for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
		{
			COpenCLParameter* parameter = *it;
			parameter->Add(kernel, numArg++);
		}

		err = clEnqueueNDRangeKernel(context->GetCommandQueue(), kernel, sizeof(global_work_size) / sizeof(size_t), nullptr,
									 global_work_size, nullptr, 0, nullptr, nullptr);
		Error::CheckError(err);

		err = clFinish(context->GetCommandQueue());
		Error::CheckError(err);

		for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
		{
			COpenCLParameter* parameter = *it;
			if (!parameter->GetNoDelete())
				parameter->Release();
		}
	}
	catch (...)
	{

	}
}


void COpenCLExecuteProgram::ExecuteKernel2D(size_t* offset, size_t* gs_d, size_t* ls)
{
	cl_int err;

	int numArg = 0;
	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		parameter->Add(kernel, numArg++);
	}

	cl_device_id deviceId = context->GetContext().get_device().id();

	size_t local_size_max;
	err = clGetKernelWorkGroupInfo(kernel, deviceId, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t),
	                               &local_size_max, nullptr);
	Error::CheckError(err);

	cl_event event;

	//clEnqueueNDRangeKernel(queue, dist_kernel, 2, offset, gs_d, ls, 0, NULL, &event)
	err = clEnqueueNDRangeKernel(context->GetCommandQueue(), kernel, 2, offset, gs_d, ls, 0, nullptr, &event);
	Error::CheckError(err);


	err = clFinish(context->GetCommandQueue());
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

	if (cl_output_buffer == static_cast<cl_mem>(nullptr))
	{
		if (data == nullptr)
			cl_output_buffer = clCreateBuffer(context->GetContext(), CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR,
			                                  outputBufferSize, data, &err);
		else
			cl_output_buffer = clCreateBuffer(context->GetContext(), CL_MEM_READ_WRITE | flag, outputBufferSize, data,
			                                  &err);
		Error::CheckError(err);
	}

	if (cl_output_buffer == static_cast<cl_mem>(nullptr))
		throw Error("Failed to create Output Buffer!");

	kernel.set_arg(0, sizeof(cl_mem), &cl_output_buffer);
	/*
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_output_buffer);
	Error::CheckError(err);
	*/

	int numArg = 1;
	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		parameter->Add(kernel, numArg++);
	}

	cl_device_id deviceId = context->GetContext().get_device().id();
	size_t local_size = 0;

	// get maximum workgroup size
	size_t local_size_max;
	err = clGetKernelWorkGroupInfo(kernel, deviceId, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t),
	                               &local_size_max, nullptr);
	Error::CheckError(err);

	err = clEnqueueNDRangeKernel(context->GetCommandQueue(), kernel, 1, nullptr, &global_size,
	                             local_size ? &local_size : nullptr, 0, nullptr, &cl_perf_event);
	err = clFinish(context->GetCommandQueue());
	Error::CheckError(err);

	if (!keepMemory)
	{
		if (flag == CL_MEM_USE_HOST_PTR)
		{
			void* tmp_ptr = clEnqueueMapBuffer(context->GetCommandQueue(), cl_output_buffer, true, CL_MAP_READ, 0,
			                                   bitmapSize, 0, nullptr, nullptr, &err);
			Error::CheckError(err);
			if (tmp_ptr != data)
			{
				// the pointer have to be same because CL_MEM_USE_HOST_PTR option was used in clCreateBuffer
				throw Error("clEnqueueMapBuffer failed to return original pointer");
			}
			err = clFinish(context->GetCommandQueue());
			Error::CheckError(err);

			err = clEnqueueUnmapMemObject(context->GetCommandQueue(), cl_output_buffer, tmp_ptr, 0, nullptr, nullptr);
			Error::CheckError(err);
		}
		else
		{
			err = clEnqueueReadBuffer(context->GetCommandQueue(), cl_output_buffer, CL_TRUE, 0, bitmapSize, data, 0,
			                          nullptr, nullptr);
			Error::CheckError(err);
			err = clFinish(context->GetCommandQueue());
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
