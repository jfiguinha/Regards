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
			COpenCLContext(const bool &opengl);
			~COpenCLContext();

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
			COpenCLProgram * GetProgram(const wxString& numProgramId, const int &type);

			int GetDefaultType();

			void SetPlatformName(const wxString & platform_name);

			wxString GetPlatformName()
			{
				return platform_name;
			}

			cv::ocl::OpenCLExecutionContext GetContextForOpenCV()
			{
				return opencvContext;
			}

			void SetOpenCVContext(cv::ocl::OpenCLExecutionContext opencvContext)
			{
				this->opencvContext = opencvContext;
			}

		private:

			int GenerateContext();
			cl_device_id GetDevice(cl_platform_id platform, cl_device_type device_type, const bool& openglCompatible, bool& findDevice);
			wxString GetDeviceInfo(cl_device_id device, cl_device_info param_name);
			vector<cl_platform_id> GetListOfPlatform();
			void opengl_create_shared_context();
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
			cv::ocl::OpenCLExecutionContext opencvContext;
			vector<COpenCLProgram *> listProgram;
		};
	}
}

