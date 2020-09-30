#pragma once
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLInfos
		{
		public:
			COpenCLInfos();
			~COpenCLInfos();

			// Minimal alignment in bytes for memory used in clCreateBuffer with CL_MEM_USE_HOST_PTR
			static cl_uint requiredOpenCLAlignment(cl_device_id device);

			// Maximum number of work-items in a workgroup
			static size_t deviceMaxWorkGroupSize(cl_device_id device);

			// Maximum number of work-items that can be
			// specified in each dimension of the workgroup
			static void deviceMaxWorkItemSizes(cl_device_id device, size_t* sizes);
            
            static cl_ulong devideMaxMemAllocSize(cl_device_id device);

			// Maximum work-group size that can be used to execute
			// a kernel on a specific device
			static size_t kernelMaxWorkGroupSize(cl_kernel kernel, cl_device_id device);
		};
	}
}

