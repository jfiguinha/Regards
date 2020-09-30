#include <header.h>
#include "OpenCLParameter.h"
#include "utility.h"
using namespace Regards::OpenCL;



void COpenCLParameter::SetLibelle(const wxString &libelle)
{
	this->libelle = libelle;
}

void COpenCLParameterInt::Add(cl_kernel kernelHandle, int numArg)
{
    
	cl_int err;
    err  = clSetKernelArg(kernelHandle, numArg, sizeof(cl_int), (void *) &value);
    Error::CheckError(err);;
}

void COpenCLParameterUShort::Add(cl_kernel kernelHandle, int numArg)
{
    
	cl_int err;
    err  = clSetKernelArg(kernelHandle, numArg, sizeof(cl_ushort), (void *) &value);
    Error::CheckError(err);;
}

void COpenCLParameterUInt::Add(cl_kernel kernelHandle, int numArg)
{
    
	cl_int err;
    err  = clSetKernelArg(kernelHandle, numArg, sizeof(cl_uint), (void *) &value);
    Error::CheckError(err);;
}

void COpenCLParameterFloat::Add(cl_kernel kernelHandle, int numArg)
{
    
	cl_int err;
    err  = clSetKernelArg(kernelHandle, numArg, sizeof(cl_float), (void *) &value);
    Error::CheckError(err);;
}

void COpenCLParameterByteArray::SetValue(cl_context context, uint8_t * value, int size, cl_mem_flags flag)
{
    
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size, value, &err);
	Error::CheckError(err);;
	if (cl_buffer == (cl_mem)0)
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterByteArray::Add(cl_kernel kernelHandle, int numArg)
{
    
	cl_int err;
	err = clSetKernelArg(kernelHandle, numArg, sizeof(cl_mem), (void *)&cl_buffer);
    Error::CheckError(err);;
}

void COpenCLParameterIntArray::SetValue(cl_context context, int * value, int size, cl_mem_flags flag)
{
    
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size * sizeof(cl_int), value, &err);
	Error::CheckError(err);;
	if (cl_buffer == (cl_mem)0)
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterIntArray::Add(cl_kernel kernelHandle, int numArg)
{
    
	cl_int err;
	err = clSetKernelArg(kernelHandle, numArg, sizeof(cl_mem), (void *)&cl_buffer);
	Error::CheckError(err);;
}


void COpenCLParameterFloatArray::SetValue(cl_context context, float * value, int size, cl_mem_flags flag)
{
    
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size * sizeof(cl_float), value, &err);
	Error::CheckError(err);;
	if (cl_buffer == (cl_mem)0)
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterFloatArray::Add(cl_kernel kernelHandle, int numArg)
{
    
	cl_int err;
	err = clSetKernelArg(kernelHandle, numArg, sizeof(cl_mem), (void *)&cl_buffer);
	Error::CheckError(err);;
}

void COpenCLParameterUShortArray::SetValue(cl_context context, unsigned short * value, int size, cl_mem_flags flag)
{
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size * sizeof(cl_ushort), value, &err);
	Error::CheckError(err);;
	if (cl_buffer == (cl_mem)0)
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterUShortArray::Add(cl_kernel kernelHandle, int numArg)
{
	cl_int err;
	err = clSetKernelArg(kernelHandle, numArg, sizeof(cl_mem), (void *)&cl_buffer);
	Error::CheckError(err);;
}


void COpenCLParameterColorData::SetValue(cl_context context, COLORData * value, cl_mem_flags flag)
{
    
	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, sizeof(COLORData), value, &err);
	Error::CheckError(err);;
	if (cl_buffer == (cl_mem)0)
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameter::Release()
{
	if(cl_buffer != nullptr)
	{
		cl_int err;
		err = clReleaseMemObject(cl_buffer);
		Error::CheckError(err);
		cl_buffer = nullptr;
	}
}

void COpenCLParameterColorData::Add(cl_kernel kernelHandle, int numArg)
{
    
	cl_int err;
	err = clSetKernelArg(kernelHandle, numArg, sizeof(cl_mem), (void *)&cl_buffer);
	Error::CheckError(err);;
}


void COpenCLParameterClMem::SetValue(cl_mem memValue)
{
	cl_buffer = memValue;
}

void COpenCLParameterClMem::Add(cl_kernel kernelHandle, int numArg)
{
	cl_int err;
	err = clSetKernelArg(kernelHandle, numArg, sizeof(cl_mem), (void *)&cl_buffer);
	Error::CheckError(err);;
}

void COpenCLParameterUCharArray::SetValue(cl_context context, unsigned char * value, int size, cl_mem_flags flag)
{

	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size * sizeof(cl_uchar), value, &err);
	Error::CheckError(err);;
	if (cl_buffer == (cl_mem)0)
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterUCharArray::Add(cl_kernel kernelHandle, int numArg)
{

	cl_int err;
	err = clSetKernelArg(kernelHandle, numArg, sizeof(cl_mem), (void *)&cl_buffer);
	Error::CheckError(err);;
}

void COpenCLParameterShortArray::SetValue(cl_context context, short * value, int size, cl_mem_flags flag)
{

	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size * sizeof(cl_short), value, &err);
	Error::CheckError(err);;
	if (cl_buffer == (cl_mem)0)
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterShortArray::Add(cl_kernel kernelHandle, int numArg)
{

	cl_int err;
	err = clSetKernelArg(kernelHandle, numArg, sizeof(cl_mem), (void *)&cl_buffer);
	Error::CheckError(err);;
}

void COpenCLParameterUintArray::SetValue(cl_context context, unsigned int * value, int size, cl_mem_flags flag)
{

	cl_int err;
	cl_buffer = clCreateBuffer(context, memory_flags | flag, size * sizeof(cl_uint), value, &err);
	Error::CheckError(err);;
	if (cl_buffer == (cl_mem)0)
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterUintArray::Add(cl_kernel kernelHandle, int numArg)
{

	cl_int err;
	err = clSetKernelArg(kernelHandle, numArg, sizeof(cl_mem), (void*)& cl_buffer);
	Error::CheckError(err);;
}