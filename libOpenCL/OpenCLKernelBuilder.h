#pragma once

#include <memory>
#include <vector>


namespace Regards
{
    namespace OpenCL
    {
        class COpenCLParameter;
        class COpenCLParameterInt;
        class COpenCLParameterFloat;
        class COpenCLParameterClMem;
        class COpenCLParameterFloatArray;
        class COpenCLParameterIntArray;

        class OpenCLKernelBuilder
        {
        public:

            OpenCLKernelBuilder() = default;

            OpenCLKernelBuilder& Int(
                const wxString& name,
                int value);

            OpenCLKernelBuilder& Float(
                const wxString& name,
                float value);

            OpenCLKernelBuilder& Image(
                const wxString& name,
                cl_mem image);

            OpenCLKernelBuilder& FloatArray(
                const wxString& name,
                cl_context context,
                float* values,
                int size,
                cl_mem_flags flags);

            OpenCLKernelBuilder& ByteArray(
                    const wxString& name,
                    cl_context context,
                    uint8_t* values,
                    int size,
                    cl_mem_flags flags);

            OpenCLKernelBuilder& IntArray(
                const wxString& name,
                cl_context context,
                int* values,
                int size,
                cl_mem_flags flags);

            std::vector<COpenCLParameter*> GetParameters();

        private:

            std::vector<std::unique_ptr<COpenCLParameter>> parameters_;
        };
    }
}