#pragma once
#include "OpenCLContext.h"
#include "OpenCLProgram.h"
#include "OpenCLParameter.h"
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

using namespace std;
using namespace Regards::OpenCL;

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLBm3D
		{
		public:

			COpenCLBm3D(COpenCLContext * context);

			void SetFlag(const bool &useMemory)
			{
				flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
			};

			~COpenCLBm3D();

			void ExecuteFilter(const float &sigma);
			void InitData(cl_mem dataInput, int sizeInput, int width, int height);
			cl_mem GetBasicImage();
			cl_mem GetWienerImage();
		protected:

			cl_mem_flags  flag;
			void CalculDistanceStep1(COpenCLParameterShortArray * similar_coords, COpenCLParameterUCharArray * block_counts, int i, int j);
			void CalculDistanceStep2(const cl_mem & basicPicture, COpenCLParameterShortArray * similar_coords, COpenCLParameterUCharArray * block_counts, int i, int j);
			cl_mem ExecuteBasicFilter(const float &sigma, COpenCLParameterShortArray * similar_coords, COpenCLParameterUCharArray * block_counts);
			cl_mem ExecuteWienerFilter(const float &sigma, const cl_mem & basicPicture, COpenCLParameterShortArray * similar_coords, COpenCLParameterUCharArray * block_counts);
			void StepOne(const float &sigma);
			void StepTwo(const float &sigma);

			COpenCLProgram * openCLProgram;
			COpenCLContext * context;
			int width;
			int height;
			int sizeoutput;
			COpenCLParameter * paramInput;
			cl_mem basicPicture;
			cl_mem wienerPicture;
			int opencl_type;
		};

	}
}
