#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "OpenCLParameter.h"
#include <RegardsFloatBitmap.h>
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
			void SetParameter(vector<COpenCLParameter*>* vecParam, CRegardsFloatBitmap* bitmapOut);
			void SetParameter(vector<COpenCLParameter *> * vecParam, CRegardsBitmap * bitmapOut);
			void SetParameter(vector<COpenCLParameter *> * vecParam, int width, int height, int sizeOutput);
			void SetParameter(vector<COpenCLParameter *> * vecParam, int width, int height, void * dataPt = nullptr);
			void SetParameter(vector<COpenCLParameter *> * vecParam, wxImage * imageOut);
			void SetParameter(vector<COpenCLParameter *> * vecParam, int sizeOutput, int width, int height, void * dataPt);
			void SetParameter(vector<COpenCLParameter *> * vecParam, int width, int height, cl_mem output);
			void ExecuteProgram(const cl_program &program, const wxString &kernelName);
			void ExecuteProgram(const cl_program &program, const wxString &kernelName, const size_t & outputBufferSize);
			void ExecuteProgram1D(const cl_program &program, const wxString &kernelName);
			void ExecuteProgram1D(const cl_program &program, const wxString &kernelName, const size_t & global_size, const size_t & outputBufferSize);
			void ExecuteProgram2D(const cl_program &program, const wxString &kernelName, vector<COpenCLParameter *> * vecParam, size_t * offset, size_t * gs_d, size_t * ls);
			void ExecuteProgram2D(const cl_program &program, const wxString &kernelName, vector<COpenCLParameter *> * vecParam, int width, int height);
			
			cl_mem GetOutput();
			void SetKeepOutput(const bool &keepMemory);
		private:

			void ExecuteKernel2D(size_t * offset, size_t * gs_d, size_t * ls);
			void ExecuteKernel2D(const size_t & outputBufferSize);
			void ExecuteKernel1D(const size_t & global_size, const size_t & outputBufferSize);
			cl_kernel kernel;
			cl_mem_flags flag;
			void * data;
			int width;
			int height;
			int bitmapSize;
			size_t global_size;
			COpenCLContext * context;
			vector<COpenCLParameter *> * vecParam;
			cl_mem cl_output_buffer;
			bool keepMemory;
		};
	}
}

