#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
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
		class COpenCLContext
		{
		public:
			COpenCLContext();
			~COpenCLContext();
			int GenerateContext(const wxString &platform_name_or_index = "intel", const wxString &device_name_or_index = "0", const wxString &device_type = "all");

			cl_context GetContext()
			{
				return context;
			}

			const cl_device_id * GetSelectedDevice()
			{
				return &device;
			}

			cl_device_id GetDeviceId()
			{
				return device;
			}

			int GetNumberOfDevice()
			{
				return num_of_devices;
			}

			cl_command_queue GetCommandQueue()
			{
				return queue;
			}
		

		private:

			cl_device_type ParseDeviceType(const wxString& device_type_name);
			cl_platform_id SelectPlatform(const wxString& platform_name_or_index);
			int SelectDevice(const wxString& device_name_or_index);
			std::vector<cl_device_id> SelectDevices(cl_platform_id platform, const wxString& device_type_name);
			void CreateContext();
			void CreateQueue(cl_command_queue_properties queue_properties = 0);

			cl_platform_id platform;
			cl_device_id device;
			cl_context context;
			cl_command_queue queue;
			cl_uint num_of_devices;
		};
	}
}

