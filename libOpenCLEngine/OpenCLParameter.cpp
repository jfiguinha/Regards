#include <header.h>
#include "OpenCLParameter.h"
#include "utility.h"
#include <boost/compute/core.hpp>
namespace compute = boost::compute;
using namespace Regards::OpenCL;


void COpenCLParameter::SetLibelle(const wxString& libelle)
{
	this->libelle = libelle;
}

void COpenCLParameterInt::Add(compute::kernel & kernelHandle, int numArg)
{
	kernelHandle.set_arg(numArg, sizeof(cl_int), &value);
}

void COpenCLParameterUShort::Add(compute::kernel & kernelHandle, int numArg)
{
	kernelHandle.set_arg(numArg, sizeof(cl_ushort), &value);
}

void COpenCLParameterUInt::Add(compute::kernel & kernelHandle, int numArg)
{
	kernelHandle.set_arg(numArg, sizeof(cl_uint), &value);
}

void COpenCLParameterFloat::Add(compute::kernel & kernelHandle, int numArg)
{
	kernelHandle.set_arg(numArg, sizeof(cl_float), &value);
}

void COpenCLParameterByteArray::SetValue(cl_context context, uint8_t* value, int size, cl_mem_flags flag)
{
	Release();
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size, value, &err);
	Error::CheckError(err);
	if (cl_buffer == static_cast<cl_mem>(nullptr))
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterByteArray::Add(compute::kernel & kernelHandle, int numArg)
{
	kernelHandle.set_arg(numArg, sizeof(cl_mem), &cl_buffer);
}

void COpenCLParameterIntArray::SetValue(cl_context context, int* value, int size, cl_mem_flags flag)
{
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size * sizeof(cl_int), value, &err);
	Error::CheckError(err);
	if (cl_buffer == static_cast<cl_mem>(nullptr))
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterIntArray::Add(compute::kernel & kernelHandle, int numArg)
{
	kernelHandle.set_arg(numArg, sizeof(cl_mem), &cl_buffer);
}


void COpenCLParameterFloatArray::SetValue(cl_context context, float* value, int size, cl_mem_flags flag)
{
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size * sizeof(cl_float), value, &err);
	Error::CheckError(err);
	if (cl_buffer == static_cast<cl_mem>(nullptr))
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterFloatArray::Add(compute::kernel & kernelHandle, int numArg)
{
	kernelHandle.set_arg(numArg, sizeof(cl_mem), &cl_buffer);
}

void COpenCLParameterUShortArray::SetValue(cl_context context, unsigned short* value, int size, cl_mem_flags flag)
{
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size * sizeof(cl_ushort), value, &err);
	Error::CheckError(err);
	if (cl_buffer == static_cast<cl_mem>(nullptr))
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterUShortArray::Add(compute::kernel & kernelHandle, int numArg)
{
	kernelHandle.set_arg(numArg, sizeof(cl_mem), &cl_buffer);
}


void COpenCLParameterColorData::SetValue(cl_context context, COLORData* value, cl_mem_flags flag)
{
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, sizeof(COLORData), value, &err);
	Error::CheckError(err);
	if (cl_buffer == static_cast<cl_mem>(nullptr))
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameter::Release()
{
	if (cl_buffer != nullptr)
	{
		cl_int err;
		err = clReleaseMemObject(cl_buffer);
		Error::CheckError(err);
		cl_buffer = nullptr;
	}
}

void COpenCLParameterColorData::Add(compute::kernel & kernelHandle, int numArg)
{
	cl_int err;
	err = clSetKernelArg(kernelHandle, numArg, sizeof(cl_mem), &cl_buffer);
	Error::CheckError(err);
}


void COpenCLParameterClMem::SetValue(cl_mem memValue)
{
	if (cl_buffer != nullptr)
		Release();
	cl_buffer = nullptr;
	cl_buffer = memValue;
}

void COpenCLParameterClMem::Add(compute::kernel & kernelHandle, int numArg)
{
	kernelHandle.set_arg(numArg, sizeof(cl_mem), &cl_buffer);
}

void COpenCLParameterUCharArray::SetValue(cl_context context, unsigned char* value, int size, cl_mem_flags flag)
{
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size * sizeof(cl_uchar), value, &err);
	Error::CheckError(err);
	if (cl_buffer == static_cast<cl_mem>(nullptr))
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterUCharArray::Add(compute::kernel & kernelHandle, int numArg)
{
	kernelHandle.set_arg(numArg, sizeof(cl_mem), &cl_buffer);
}

void COpenCLParameterShortArray::SetValue(cl_context context, short* value, int size, cl_mem_flags flag)
{
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size * sizeof(cl_short), value, &err);
	Error::CheckError(err);
	if (cl_buffer == static_cast<cl_mem>(nullptr))
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterShortArray::Add(compute::kernel & kernelHandle, int numArg)
{
	kernelHandle.set_arg(numArg, sizeof(cl_mem), &cl_buffer);
}

void COpenCLParameterUintArray::SetValue(cl_context context, unsigned int* value, int size, cl_mem_flags flag)
{
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size * sizeof(cl_uint), value, &err);
	Error::CheckError(err);
	if (cl_buffer == static_cast<cl_mem>(nullptr))
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterUintArray::Add(compute::kernel & kernelHandle, int numArg)
{
	kernelHandle.set_arg(numArg, sizeof(cl_mem), &cl_buffer);
}
