#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <stdint.h>
#include <string>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <boost/compute/core.hpp>
namespace compute = boost::compute;
using namespace std;

namespace Regards
{
	namespace OpenCL
	{
		struct COLORData
		{
			int red;
			int green;
			int blue;
			int alpha;
		};

		class COpenCLParameter
		{
		public:
			COpenCLParameter(): queue(nullptr)
			{
				dataType = 0;
				noDelete = false;
				cl_buffer = nullptr;
				noDeleteClMem = false;
			}

			virtual ~COpenCLParameter()
			{
				if (!noDeleteClMem)
					COpenCLParameter::Release();
			}

			void SetCommandQueue(const compute::command_queue& queue)
			{
				this->queue = queue;
			}

			void SetLibelle(const wxString& libelle);

			virtual void Add(compute::kernel & kernelHandle, int numArg)
			{
			};
			virtual void Release();

			int GetType()
			{
				return dataType;
			};

			void SetNoDelete(const bool& value)
			{
				noDelete = value;
			}

			bool GetNoDelete()
			{
				return noDelete;
			}

			virtual cl_mem GetValue()
			{
				return cl_buffer;
			}

		protected:
			cl_mem cl_buffer = nullptr;
			compute::command_queue queue;
			int dataType;
			bool noDelete;
			bool noDeleteClMem;
			wxString libelle;
			cl_mem_flags memory_flags = CL_MEM_READ_ONLY;
		};

		class COpenCLParameterInt : public COpenCLParameter
		{
		public:
			explicit COpenCLParameterInt(const bool& noDeleteMem = false): value(0)
			{
				dataType = 1;
				noDeleteClMem = noDeleteMem;
			};

			~COpenCLParameterInt() override
			{
			};

			void SetValue(const cl_int& value)
			{
				this->value = value;
			};
			void Add(compute::kernel & kernelHandle, int numArg) override;

		private:
			cl_int value;
		};

		class COpenCLParameterUInt : public COpenCLParameter
		{
		public:
			explicit COpenCLParameterUInt(const bool& noDeleteMem = false): value(0)
			{
				dataType = 2;
				noDeleteClMem = noDeleteMem;
			};

			~COpenCLParameterUInt() override
			{
			};

			void SetValue(const cl_uint& value)
			{
				this->value = value;
			};

			void Add(compute::kernel & kernelHandle, int numArg) override;

		private:
			cl_uint value;
		};


		class COpenCLParameterFloat : public COpenCLParameter
		{
		public:
			explicit COpenCLParameterFloat(const bool& noDeleteMem = false): value(0)
			{
				dataType = 3;
				noDeleteClMem = noDeleteMem;
			}

			~COpenCLParameterFloat() override
			{
			};

			void SetValue(const cl_float& value)
			{
				this->value = value;
			};

			void Add(compute::kernel & kernelHandle, int numArg) override;

		private:
			cl_float value;
		};

		class COpenCLParameterByteArray : public COpenCLParameter
		{
		public:
			COpenCLParameterByteArray(const bool& noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY)
			{
				dataType = 4;
				noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};

			void SetValue(cl_context context, uint8_t* value, int size, cl_mem_flags flag);
			void Add(compute::kernel & kernelHandle, int numArg) override;
		};

		class COpenCLParameterIntArray : public COpenCLParameter
		{
		public:
			COpenCLParameterIntArray(const bool& noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY)
			{
				dataType = 5;
				noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};


			void SetValue(cl_context context, int* value, int size, cl_mem_flags flag);
			void Add(compute::kernel & kernelHandle, int numArg) override;
		};

		class COpenCLParameterFloatArray : public COpenCLParameter
		{
		public:
			COpenCLParameterFloatArray(const bool& noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY)
			{
				dataType = 6;
				noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};

			void SetValue(cl_context context, float* value, int size, cl_mem_flags flag);
			void Add(compute::kernel & kernelHandle, int numArg) override;
		};

		class COpenCLParameterColorData : public COpenCLParameter
		{
		public:
			COpenCLParameterColorData(const bool& noDeleteMem = false)
			{
				dataType = 7;
				noDeleteClMem = noDeleteMem;
			};


			void SetValue(cl_context context, COLORData* value, cl_mem_flags flag);
			void Add(compute::kernel & kernelHandle, int numArg) override;
		};

		class COpenCLParameterUShortArray : public COpenCLParameter
		{
		public:
			COpenCLParameterUShortArray(const bool& noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY)
			{
				dataType = 9;
				noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};


			void SetValue(cl_context context, unsigned short* value, int size, cl_mem_flags flag);
			void Add(compute::kernel & kernelHandle, int numArg) override;
		};

		class COpenCLParameterClMem : public COpenCLParameter
		{
		public:
			COpenCLParameterClMem(const bool& noDeleteMem = false)
			{
				dataType = 8;
				noDeleteClMem = noDeleteMem;
			};


			void SetValue(cl_mem memValue);
			void Add(compute::kernel & kernelHandle, int numArg) override;
			cl_mem GetValue() override { return cl_buffer; };
		};


		class COpenCLParameterUShort : public COpenCLParameter
		{
		public:
			explicit COpenCLParameterUShort(const bool& noDeleteMem = false): value(0)
			{
				dataType = 10;
				noDeleteClMem = noDeleteMem;
			}
			;

			~COpenCLParameterUShort() override
			{
			};

			void SetValue(const cl_ushort& value)
			{
				this->value = value;
			};
			void Add(compute::kernel & kernelHandle, int numArg) override;

		private:
			cl_ushort value;
		};

		class COpenCLParameterUCharArray : public COpenCLParameter
		{
		public:
			COpenCLParameterUCharArray(const bool& noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY)
			{
				dataType = 11;
				noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};

			~COpenCLParameterUCharArray() override
			{
			};

			void SetValue(cl_context context, unsigned char* value, int size, cl_mem_flags flag);
			void Add(compute::kernel & kernelHandle, int numArg) override;
		};

		class COpenCLParameterShortArray : public COpenCLParameter
		{
		public:
			COpenCLParameterShortArray(const bool& noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY)
			{
				dataType = 12;
				noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};

			~COpenCLParameterShortArray() override
			{
			};

			void SetValue(cl_context context, short* value, int size, cl_mem_flags flag);
			void Add(compute::kernel & kernelHandle, int numArg) override;
		};

		class COpenCLParameterUintArray : public COpenCLParameter
		{
		public:
			COpenCLParameterUintArray(const bool& noDeleteMem = false, cl_mem_flags memory_flags = CL_MEM_READ_ONLY)
			{
				dataType = 13;
				noDeleteClMem = noDeleteMem;
				this->memory_flags = memory_flags;
			};

			~COpenCLParameterUintArray() override
			{
			};

			void SetValue(cl_context context, unsigned int* value, int size, cl_mem_flags flag);
			void Add(compute::kernel & kernelHandle, int numArg) override;
		};
	}
}
