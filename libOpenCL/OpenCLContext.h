#pragma once
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include "OpenCLInfos.h"
using namespace std;

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLProgram;

		class COpenCLContext
		{
		public:
			COpenCLContext(cl_platform_id platformId, cl_device_id deviceId, cl_device_type deviceType, const bool &opengl);
			~COpenCLContext();
			int GenerateContext();

			cl_context GetContext()
			{
				return context;
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
				return queue;
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

		private:


			void CreateContext();
			void CreateQueue(cl_command_queue_properties queue_properties = 0);

			bool sharedContextCompatible = false;
            bool isOpenGL = false;
			cl_platform_id platform;
			cl_device_id device;
			cl_context context;
            cl_device_type deviceType;
			cl_command_queue queue;
			vector<COpenCLProgram *> listProgram;
		};
	}
}

