#pragma once

namespace Regards
{
    namespace OpenCL
    {
        class COpenCLContext
        {
        public:
            COpenCLContext() = default;
            ~COpenCLContext();

            COpenCLContext(const COpenCLContext&) = delete;
            COpenCLContext& operator=(const COpenCLContext&) = delete;

            void Bind();

            void initializeContextFromGL();
            void AssociateToVulkan();
            void CreateDefaultOpenCLContext();

            void GetOutputData(
                cl_mem cl_output_buffer,
                void* dataOut,
                const int& sizeOutput,
                const int& flag);

            cv::ocl::Program GetProgram(const wxString& programName);

            cv::ocl::OpenCLExecutionContext& GetExecutionContext()
            {
                return clExecCtx;
            }

            const cv::ocl::OpenCLExecutionContext& GetExecutionContext() const
            {
                return clExecCtx;
            }

            cl_command_queue GetCommandQueue()
            {
                if (commandQueue == nullptr)
                    CreateCommandQueue();
                return commandQueue;
            }

            cl_context GetContext()
            {
                return static_cast<cl_context>(clExecCtx.getContext().ptr());
            }

        private:
            cl_command_queue commandQueue = nullptr;
            std::map<wxString, cv::ocl::Program> openclBinaryMapping;
            cv::ocl::OpenCLExecutionContext clExecCtx;

            void CreateCommandQueue(
                cl_command_queue_properties queue_properties = 0);

            wxString GetDeviceInfo(
                cl_device_id device,
                cl_device_info param_name);

            cl_device_id GetListOfDevice(
                cl_platform_id platform,
                cl_device_type device_type,
                int& found);
        };
    }
}