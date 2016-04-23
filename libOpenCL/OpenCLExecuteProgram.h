#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "OpenCLParameter.h"
#include <RegardsBitmap.h>
#include <string>
#include <vector>
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
		class COpenCLContext;


		class COpenCLExecuteProgram
		{
		public:
			COpenCLExecuteProgram(COpenCLContext * context, const cl_mem_flags &flag);
			~COpenCLExecuteProgram();

			void SetParameter(vector<COpenCLParameter *> * vecParam, CRegardsBitmap * bitmapOut);
			void SetParameter(vector<COpenCLParameter *> * vecParam, wxImage * imageOut);
			void ExecuteProgram(const cl_program &program, const wxString &kernelName);
			void ExecuteProgram1D(const cl_program &program, const wxString &kernelName);

		private:

			void ExecuteKernel2D();
			void ExecuteKernel1D();
			cl_kernel kernel;
			cl_mem_flags flag;
			//CRegardsBitmap * bitmapOut;

			uint8_t * data;
			int width;
			int height;
			int bitmapSize;
			COpenCLContext * context;
			vector<COpenCLParameter *> * vecParam;
		};
	}
}

