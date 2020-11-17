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
			COpenCLParameter(){ dataType = 0; noDelete = false; cl_buffer = nullptr;noDeleteClMem=false;};
			virtual ~COpenCLParameter()
			{
				if(!noDeleteClMem)
					Release();
			};

			void SetCommandQueue(const cl_command_queue &queue)
			{
				this->queue = queue;
			};
			void SetLibelle(const wxString &libelle);
			virtual void Add(cl_kernel kernelHandle, int numArg){};
			virtual void Release();
			int GetType(){
				return  dataType;
			};
			void SetNoDelete(const bool &value)
			{
				noDelete = value;
			}

			bool GetNoDelete()
			{
				return noDelete;
			}

			cl_mem GetValue()
			{
				return cl_buffer;
			}

		protected:
			cl_mem cl_buffer = nullptr;
			cl_command_queue queue;
			int dataType;
			bool noDelete;
			bool noDeleteClMem;
			wxString libelle;
			cl_mem_flags memory_flags = CL_MEM_READ_ONLY;
		};

		class COpenCLParameterInt : public COpenCLParameter
		{
		public:
			COpenCLParameterInt(const bool &noDeleteMem = false){
				dataType = 1;
                noDeleteClMem = noDeleteMem;
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
			COpenCLParameterUInt(const bool &noDeleteMem = false){
				dataType = 2;
                noDeleteClMem = noDeleteMem;
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
			COpenCLParameterFloat(const bool &noDeleteMem = false){
				dataType = 3;
                noDeleteClMem = noDeleteMem;
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
			COpenCLParameterByteArray(const bool &noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY){
				dataType = 4;
                noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};
			~COpenCLParameterByteArray(){};

			void SetValue(cl_context context, uint8_t * value, int size, cl_mem_flags flag);
			void Add(cl_kernel kernelHandle, int numArg);
		};

		class COpenCLParameterIntArray : public COpenCLParameter
		{
		public:
			COpenCLParameterIntArray(const bool &noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY){
				dataType = 5;
                noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};
			~COpenCLParameterIntArray(){};

			void SetValue(cl_context context, int * value, int size, cl_mem_flags flag);
			void Add(cl_kernel kernelHandle, int numArg);


		};

		class COpenCLParameterFloatArray : public COpenCLParameter
		{
		public:
			COpenCLParameterFloatArray(const bool &noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY){
				dataType = 6;
                noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};
			~COpenCLParameterFloatArray(){};

			void SetValue(cl_context context, float * value, int size, cl_mem_flags flag);
			void Add(cl_kernel kernelHandle, int numArg);
		};

		class COpenCLParameterColorData : public COpenCLParameter
		{
		public:
			COpenCLParameterColorData(const bool &noDeleteMem = false){
				dataType = 7;
                noDeleteClMem = noDeleteMem;
			};
			~COpenCLParameterColorData(){};


			void SetValue(cl_context context, COLORData * value, cl_mem_flags flag);
			void Add(cl_kernel kernelHandle, int numArg);

		};

		class COpenCLParameterUShortArray : public COpenCLParameter
		{
		public:
			COpenCLParameterUShortArray(const bool &noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY){
				dataType = 9;
                noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};
			~COpenCLParameterUShortArray(){};

			void SetValue(cl_context context, unsigned short * value, int size, cl_mem_flags flag);
			void Add(cl_kernel kernelHandle, int numArg);
		};

		class COpenCLParameterClMem : public COpenCLParameter
		{
		public:
			COpenCLParameterClMem(const bool &noDeleteMem = false){
				dataType = 8;
				noDeleteClMem = noDeleteMem;
			};
			~COpenCLParameterClMem()
			{

			};

			void SetValue(cl_mem memValue);
			void Add(cl_kernel kernelHandle, int numArg);
			cl_mem GetValue(){return cl_buffer;};
		};


		class COpenCLParameterUShort : public COpenCLParameter
		{
		public:
			COpenCLParameterUShort(const bool &noDeleteMem = false)
			{
				dataType = 10;
                noDeleteClMem = noDeleteMem;
			 };
			~COpenCLParameterUShort(){};

			void SetValue(const cl_ushort &value)
			{
				this->value = value;
			};
			void Add(cl_kernel kernelHandle, int numArg);

		private:

			cl_ushort value;
		};

		class COpenCLParameterUCharArray : public COpenCLParameter
		{
		public:
			COpenCLParameterUCharArray(const bool &noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY) {
				dataType = 11;
				noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};
			~COpenCLParameterUCharArray() {};

			void SetValue(cl_context context, unsigned char * value, int size, cl_mem_flags flag);
			void Add(cl_kernel kernelHandle, int numArg);
		};

		class COpenCLParameterShortArray : public COpenCLParameter
		{
		public:
			COpenCLParameterShortArray(const bool &noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY) {
				dataType = 12;
				noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};
			~COpenCLParameterShortArray() {};

			void SetValue(cl_context context, short * value, int size, cl_mem_flags flag);
			void Add(cl_kernel kernelHandle, int numArg);
		};

		class COpenCLParameterUintArray : public COpenCLParameter
		{
		public:
			COpenCLParameterUintArray(const bool& noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY) {
				dataType = 13;
				noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};
			~COpenCLParameterUintArray() {};

			void SetValue(cl_context context, unsigned int * value, int size, cl_mem_flags flag);
			void Add(cl_kernel kernelHandle, int numArg);
		};

	}
}
