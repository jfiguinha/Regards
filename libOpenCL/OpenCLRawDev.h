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
		class COpenCLRawDev
		{
		public:

			COpenCLRawDev(COpenCLContext * context);

			void SetFlag(const bool &useMemory)
			{
				flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
			};

			~COpenCLRawDev();

			void InitData(void * dataInput, int sizeInput, int sizeOutput, int width, int height, int scale);
			void WaveletDenoiseNormalize(int size, int hpass, int lpass, float thold);
			void WaveletDenoiseChannel(int size, float threshold, float * noise);
			void WaveletDenoiseCol(int hpass, int lpass, int sc);
			void WaveletDenoiseRow(int lpass, int sc);
			void * GetTempData();
			void * GetOutputData(void * dataOutput, int sizeOutput, int lpass);
			void * GetAlphaChannel(void * dataOutput, int sizeOutput);

		protected:
			
			cl_mem GetAlphaChannel(int sizeOutput);
			cl_mem OutputData(int sizeOutput, int lpass);
			cl_mem_flags  flag;
			cl_mem WaveletDenoiseNormalizeValue(int size, int hpass, int lpass, float thold);
			cl_mem WaveletDenoiseByColSetValue(int hpass, int lpass, int sc);
			cl_mem WaveletDenoiseByRowSetValue(int lpass, int sc);
			cl_mem InitData(int scale);
			COpenCLProgram * openCLProgram;
			COpenCLContext * context;
			int width;
			int height;
			int sizeoutput;
			//float * img;
			//float * output;
			COpenCLParameter * paramOutput;
		};

	}
}
