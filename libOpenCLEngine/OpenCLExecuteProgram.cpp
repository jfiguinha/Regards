#include <header.h>
#include "OpenCLExecuteProgram.h"
#include "OpenCLContext.h"
#include <RegardsBitmap.h>
#include <RegardsFloatBitmap.h>
#include "utility.h"
#include <boost/compute/utility/dim.hpp>
using namespace Regards::OpenCL;
using compute::dim;
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

void COpenCLExecuteProgram::CreateValue(const size_t& outputBufferSize, void* data, const cl_mem_flags& flag)
{
	cl_int err = 0;
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

void COpenCLExecuteProgram::KeepMemory(const size_t& outputBufferSize)
{
	cl_int err = 0;
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
}

void COpenCLExecuteProgram::ExecuteKernel2D(const size_t& outputBufferSize)
{
	CreateValue(outputBufferSize, data, flag);
	kernel.set_arg(0, sizeof(cl_mem), &cl_output_buffer);

	int numArg = 1;
	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		parameter->Add(kernel, numArg++);
	}

	context->GetCommandQueue().enqueue_nd_range_kernel(
		kernel, dim(0, 0), dim(width, height), dim(1, 1)
	);


	KeepMemory(outputBufferSize);

	DeleteParam(vecParam);
}

void COpenCLExecuteProgram::DeleteParam(vector<COpenCLParameter*>* & vecParam)
{
	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		if (!parameter->GetNoDelete())
		{
			parameter->Release();
			delete parameter;
		}
	}
}

void COpenCLExecuteProgram::ExecuteProgram2D(const compute::program & program, const wxString& kernelName,
                                             vector<COpenCLParameter*>* vecParam, int width, int height)
{
	try
	{
		compute::kernel kernel(program, kernelName.ToStdString());

		int numArg = 0;
		for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
		{
			COpenCLParameter* parameter = *it;
			parameter->Add(kernel, numArg++);
		}

		context->GetCommandQueue().enqueue_nd_range_kernel(
			kernel, dim(0, 0), dim(width, height), dim(1, 1)
		);

		DeleteParam(vecParam);
	}
	catch (...)
	{

	}
}


void COpenCLExecuteProgram::ExecuteKernel2D(size_t* offset, size_t* gs_d, size_t* ls)
{
	int numArg = 0;
	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		parameter->Add(kernel, numArg++);
	}

	context->GetCommandQueue().enqueue_nd_range_kernel(
		kernel, 2, offset, gs_d, ls
	);

	DeleteParam(vecParam);
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
	CreateValue(outputBufferSize, data, flag);
	kernel.set_arg(0, sizeof(cl_mem), &cl_output_buffer);
	int numArg = 1;
	for (auto it = vecParam->begin(); it != vecParam->end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		parameter->Add(kernel, numArg++);
	}


	context->GetCommandQueue().enqueue_1d_range_kernel(
		kernel, 0, global_size, 0
	);

	KeepMemory(bitmapSize);

	DeleteParam(vecParam);
}
