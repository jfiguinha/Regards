#pragma once


namespace Regards
{
	namespace OpenCL
	{
        class COpenCLContext
        {
        public:
            COpenCLContext(){};
            ~COpenCLContext(){};
            static void initializeContextFromGL();
            static void CreateDefaultOpenCLContext();
        private:
            static wxString GetDeviceInfo(cl_device_id device, cl_device_info param_name);
            static cl_device_id GetListOfDevice(cl_platform_id platform, cl_device_type device_type, int& found);
        };
    }
}