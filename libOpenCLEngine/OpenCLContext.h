#pragma once
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include "OpenCLInfos.h"
#include <opencv2/core/ocl.hpp>
#include <boost/compute/core.hpp>
namespace compute = boost::compute;
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
			COpenCLContext(compute::context& context, const bool &opengl);
			~COpenCLContext();

			//int GenerateContext();

			compute::context & GetContext()
			{
				return context;
			}

			/*
			cl_device_id * GetSelectedDevice()
			{
				cl_device_id deviceId = context.get_device().id();
				return &deviceId;
			}
			*/

			compute::command_queue GetCommandQueue()
			{
				return queue;
			}
            
            bool IsSharedContextCompatible()
            {
				return sharedContextCompatible;
            }
            
            uint64_t GetMaxMemoryAllocable()
            {
                return COpenCLInfos::devideMaxMemAllocSize(context.get_device().id());
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

			const cl_uint GetNumberOfDevice()
			{
				return 1;
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

			compute::context context;
			bool sharedContextCompatible = false;
            bool isOpenGL = false;
			wxString platform_name;
			compute::command_queue queue;
			cv::ocl::OpenCLExecutionContext opencvContext;
			vector<COpenCLProgram *> listProgram;
		};
	}
}

