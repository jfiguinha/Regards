#include <header.h>
#include "OpenCLInfos.h"
#include "utility.h"
using namespace Regards::OpenCL;

COpenCLInfos::COpenCLInfos()
{
}


COpenCLInfos::~COpenCLInfos()
{
}


// Minimal alignment in bytes for memory used in clCreateBuffer with CL_MEM_USE_HOST_PTR
cl_uint COpenCLInfos::requiredOpenCLAlignment(cl_device_id device)
{
	cl_uint result = 0;
	cl_int err = clGetDeviceInfo(
		device,
		CL_DEVICE_MEM_BASE_ADDR_ALIGN,
		sizeof(result),
		&result,
		0
		);
    
    Error::CheckError(err);
	//assert(result % 8 == 0);
	return result / 8;    // clGetDeviceInfo returns value in bits, convert it to bytes
}

// Maximum number of work-items in a workgroup
size_t COpenCLInfos::deviceMaxWorkGroupSize(cl_device_id device)
{
	size_t result = 0;
	cl_int err = clGetDeviceInfo(
		device,
		CL_DEVICE_MAX_WORK_GROUP_SIZE,
		sizeof(result),
		&result,
		0
		);
    
    Error::CheckError(err);
	return result;
}

// Maximum number of work-items that can be
// specified in each dimension of the workgroup
void COpenCLInfos::deviceMaxWorkItemSizes(cl_device_id device, size_t* sizes)
{
	cl_int err = clGetDeviceInfo(
		device,
		CL_DEVICE_MAX_WORK_ITEM_SIZES,
		sizeof(size_t[3]),
		sizes,
		0
		);
    
    Error::CheckError(err);
}

cl_ulong COpenCLInfos::devideMaxMemAllocSize(cl_device_id device)
{
    cl_ulong max_mem_alloc_size;
 	cl_int err = clGetDeviceInfo(
		device,
		CL_DEVICE_MAX_MEM_ALLOC_SIZE ,
		sizeof(cl_ulong),
		&max_mem_alloc_size,
		0
		);
    
    Error::CheckError(err);   
    return max_mem_alloc_size;
}

// Maximum work-group size that can be used to execute
// a kernel on a specific device
size_t COpenCLInfos::kernelMaxWorkGroupSize(cl_kernel kernel, cl_device_id device)
{
	size_t result = 0;
	cl_int err = clGetKernelWorkGroupInfo(
		kernel,
		device,
		CL_KERNEL_WORK_GROUP_SIZE,
		sizeof(result),
		&result,
		0
		);
    
    Error::CheckError(err);
	return result;
}
