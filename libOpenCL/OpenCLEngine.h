#pragma once
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <algorithm>
using namespace std;

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLContext;

		struct OpenCLPlatform
		{
			int platformIndex;
			wxString platformName;
			cl_platform_id platformId;
		};

		struct OpenCLDevice
		{
			int deviceIndex;
			wxString deviceName;
			cl_platform_id platformId;
			cl_device_id deviceId;
			cl_device_type deviceType;
            bool openGlSharing;
		};


		class COpenCLPlatformList
		{
		public:

			static OpenCLPlatform * SelectPlatform(const wxString &name)
			{
				if (listOfPlatform.size() == 0)
					GetListOfPlatform();

				vector<OpenCLPlatform *>::iterator i = std::find_if(listOfPlatform.begin(),
					listOfPlatform.end(),
					[&](const auto& val) { return val->platformName == name; });

				if (i != listOfPlatform.end())
				{
					return ((OpenCLPlatform *)*i);
				}

				/*
				for (OpenCLPlatform * openCLPlatform : listOfPlatform)
				{
					if (name == openCLPlatform->platformName)
						return openCLPlatform;
				}
				*/
				return nullptr;
			}

			static OpenCLPlatform * SelectPlatform(int index)
			{
                printf("SelectPlatform \n");
				if (listOfPlatform.size() == 0)
					GetListOfPlatform();

				vector<OpenCLPlatform *>::iterator i = std::find_if(listOfPlatform.begin(),
					listOfPlatform.end(),
					[&](const auto& val) { return val->platformIndex == index; });

				if (i != listOfPlatform.end())
				{
					return ((OpenCLPlatform *)*i);
				}
				/*
				for (OpenCLPlatform * openCLPlatform : listOfPlatform)
				{
					if (openCLPlatform->platformIndex == index)
						return openCLPlatform;
				}
				*/
				return nullptr;
			}

			static vector<OpenCLPlatform *> GetPlatform()
			{
				if (listOfPlatform.size() == 0)
					GetListOfPlatform();

				return listOfPlatform;
			}

		private:
	
			static void GetListOfPlatform();
			static vector<OpenCLPlatform *> listOfPlatform;
		};

		

		class COpenCLDeviceList
		{
		public:

			static OpenCLDevice * SelectDevice(const wxString &deviceName);
            
            static wxString GetDeviceInfo(cl_device_id device, cl_device_info param_name);
            
            static void GetAllDevice();
            
			static OpenCLDevice * SelectDevice(OpenCLPlatform * platform, const int &index);
			
			static vector<OpenCLDevice *> GetPlatformDevice(OpenCLPlatform * platform);
            
		private:
			static int IsExtensionSupported(const char* support_str, const char* ext_string, size_t ext_buffer_size);
			static cl_device_type ParseDeviceType(const wxString& device_type_name);
			static cl_platform_id SelectPlatform(const wxString& platform_name_or_index);
			static void GetListOfDevice(vector<OpenCLDevice *> & listOfDevice, cl_platform_id platform, cl_device_type device_type);
            static vector<OpenCLDevice *> listOfDevice;
		};

		class  COpenCLEngine
		{
		public:
			COpenCLEngine();
			~COpenCLEngine();
           static OpenCLDevice * GetDefaultDevice();
			COpenCLContext * GetInstance();
		private:
			COpenCLContext * _singleton;
		};

	}
}
