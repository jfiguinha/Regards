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
	cl_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | flag, size, value, &err);
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

void COpenCLParameterByteArray::Release()
{
    
	cl_int err;
	err = clReleaseMemObject(cl_buffer);
	Error::CheckError(err);;
};


void COpenCLParameterIntArray::SetValue(cl_context context, int * value, int size, cl_mem_flags flag)
{
    
	cl_int err;
	cl_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | flag, size * sizeof(cl_int), value, &err);
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

void COpenCLParameterIntArray::Release()
{
    
	cl_int err;
	err = clReleaseMemObject(cl_buffer);
	Error::CheckError(err);;
};

void COpenCLParameterFloatArray::SetValue(cl_context context, float * value, int size, cl_mem_flags flag)
{
    
	cl_int err;
	cl_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | flag, size * sizeof(cl_float), value, &err);
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

void COpenCLParameterFloatArray::Release()
{
   
	cl_int err;
	err = clReleaseMemObject(cl_buffer);
	Error::CheckError(err);;
};


void COpenCLParameterColorData::SetValue(cl_context context, COLORData * value, cl_mem_flags flag)
{
    
	cl_int err;
	cl_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | flag, sizeof(COLORData), value, &err);
	Error::CheckError(err);;
	if (cl_buffer == (cl_mem)0)
		throw Error("Failed to create Input Buffer!");
}

void COpenCLParameterColorData::Release()
{
    
	cl_int err;
	err = clReleaseMemObject(cl_buffer);
	Error::CheckError(err);;
}

void COpenCLParameterColorData::Add(cl_kernel kernelHandle, int numArg)
{
    
	cl_int err;
	err = clSetKernelArg(kernelHandle, numArg, sizeof(cl_mem), (void *)&cl_buffer);
	Error::CheckError(err);;
}
