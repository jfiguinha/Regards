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
            static void AssociateToVulkan();
            static void CreateDefaultOpenCLContext();
            static void GetOutputData(cl_mem cl_output_buffer, void* dataOut, const int& sizeOutput, const int& flag);
            static cv::ocl::Program GetProgram(const wxString& programName);
        private:

            static cl_command_queue CreateCommandQueue(cl_command_queue_properties queue_properties = 0);
            static wxString GetDeviceInfo(cl_device_id device, cl_device_info param_name);
            static cl_device_id GetListOfDevice(cl_platform_id platform, cl_device_type device_type, int& found);
        };
    }
}