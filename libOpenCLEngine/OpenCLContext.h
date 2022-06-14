#pragma once
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include "OpenCLInfos.h"
#include <opencv2/core/ocl.hpp>

using namespace std;

#define OPENCL_UCHAR 1
#define OPENCL_FLOAT 2

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLProgram;

		class COpenCLContext
		{
		public:
			COpenCLContext(cl_platform_id platformId, const wxString & platformName, cl_device_id deviceId, cl_device_type deviceType, const bool &opengl);
			~COpenCLContext();
			int GenerateContext();

			cl_context GetContext()
			{
				return (cl_context)cv::ocl::Context::getDefault().ptr();
			}
            
            const cl_device_type GetDeviceType()
            {
                return deviceType;
            }

			const cl_platform_id GetPlatformId()
			{
				return platform;
			}

			const cl_device_id * GetSelectedDevice()
			{
				return &device;
			}

			const cl_uint GetNumberOfDevice()
			{
				return 1;
			}

			cl_device_id GetDeviceId()
			{
				return device;
			}

			cl_command_queue GetCommandQueue()
			{
				return (cl_command_queue)cv::ocl::Queue::getDefault().ptr();;
			}
            
            bool IsSharedContextCompatible()
            {
				return sharedContextCompatible;
            }
            
            uint64_t GetMaxMemoryAllocable()
            {
                return COpenCLInfos::devideMaxMemAllocSize(device);
            }

			void GetOutputData(cl_mem cl_output_buffer, void * dataOut, const int &sizeOutput, const int &flag);
		
			COpenCLProgram * GetProgram(const wxString &numProgramId);
			COpenCLProgram * GetProgram(const wxString& numProgramId, const int &type);

			int GetDefaultType();

			void SetPlatformName(const wxString & platform_name);

			wxString GetPlatformName()
			{
				return platform_name;
			}



		private:


			void CreateContext();
			void CreateQueue(cl_command_queue_properties queue_properties = 0);

			bool sharedContextCompatible = false;
            bool isOpenGL = false;
			cl_platform_id platform;
			wxString platform_name;
			cl_device_id device;
			cl_context context;
            cl_device_type deviceType;
			cl_command_queue queue;
			vector<COpenCLProgram *> listProgram;
		};
	}
}

