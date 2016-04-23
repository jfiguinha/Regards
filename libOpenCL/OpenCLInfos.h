#pragma once
#include <string>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
using namespace std;

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
			cl_uint requiredOpenCLAlignment(cl_device_id device);

			// Maximum number of work-items in a workgroup
			size_t deviceMaxWorkGroupSize(cl_device_id device);

			// Maximum number of work-items that can be
			// specified in each dimension of the workgroup
			void deviceMaxWorkItemSizes(cl_device_id device, size_t* sizes);

			// Maximum work-group size that can be used to execute
			// a kernel on a specific device
			size_t kernelMaxWorkGroupSize(cl_kernel kernel, cl_device_id device);
		};
	}
}

