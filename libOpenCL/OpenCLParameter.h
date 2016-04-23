#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <stdint.h>
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
		typedef struct
		{
			int red;
			int green;
			int blue;
			int alpha;
		}COLORData;

		class COpenCLParameter
		{
		public:
			COpenCLParameter(){ dataType = 0; };
			virtual ~COpenCLParameter()
			{
				//if (queue)
				//	clReleaseCommandQueue(queue);
			};

			void SetCommandQueue(const cl_command_queue &queue)
			{
				this->queue = queue;
			};
			void SetLibelle(const wxString &libelle);
			virtual void Add(cl_kernel kernelHandle, int numArg){};
			virtual void Release(){};
			int GetType(){
				return  dataType;
			};

		protected:

			cl_command_queue queue;
			int dataType;
			wxString libelle;
		};

		class COpenCLParameterInt : public COpenCLParameter
		{
		public:
			COpenCLParameterInt(){
				dataType = 1;
			 };
			~COpenCLParameterInt(){};

			void SetValue(const cl_int &value)
			{
				this->value = value;
			};
			void Add(cl_kernel kernelHandle, int numArg);

		private:

			cl_int value;
		};

		class COpenCLParameterUInt : public COpenCLParameter
		{
		public:
			COpenCLParameterUInt(){
				dataType = 2;
			};
			~COpenCLParameterUInt(){};

			void SetValue(const cl_uint &value)
			{
				this->value = value;
			};

			void Add(cl_kernel kernelHandle, int numArg);

		private:

			cl_uint value;
		};


		class COpenCLParameterFloat : public COpenCLParameter
		{
		public:
			COpenCLParameterFloat(){
				dataType = 3;
			};
			~COpenCLParameterFloat(){};

			void SetValue(const cl_float &value)
			{
				this->value = value;
			};
			
			void Add(cl_kernel kernelHandle, int numArg);

		private:

			cl_float value;
		};
		
		class COpenCLParameterByteArray : public COpenCLParameter
		{
		public:
			COpenCLParameterByteArray(){
				dataType = 4;
			};
			~COpenCLParameterByteArray(){};

			void SetValue(cl_context context, uint8_t * value, int size, cl_mem_flags flag);
			void Release();
			void Add(cl_kernel kernelHandle, int numArg);

		private:
			cl_mem cl_buffer;
		};

		class COpenCLParameterIntArray : public COpenCLParameter
		{
		public:
			COpenCLParameterIntArray(){
				dataType = 5;
			};
			~COpenCLParameterIntArray(){};

			void SetValue(cl_context context, int * value, int size, cl_mem_flags flag);
			void Release();
			void Add(cl_kernel kernelHandle, int numArg);

		private:
			cl_mem cl_buffer;
		};

		class COpenCLParameterFloatArray : public COpenCLParameter
		{
		public:
			COpenCLParameterFloatArray(){
				dataType = 6;
			};
			~COpenCLParameterFloatArray(){};

			void SetValue(cl_context context, float * value, int size, cl_mem_flags flag);
			void Release();
			void Add(cl_kernel kernelHandle, int numArg);

		private:
			cl_mem cl_buffer;
		};

		class COpenCLParameterColorData : public COpenCLParameter
		{
		public:
			COpenCLParameterColorData(){
				dataType = 7;
			};
			~COpenCLParameterColorData(){};


			void SetValue(cl_context context, COLORData * value, cl_mem_flags flag);
			void Release();
			void Add(cl_kernel kernelHandle, int numArg);

		private:
			cl_mem cl_buffer;
		};

	}
}
