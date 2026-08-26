#include <header.h>
#include "OpenCLParameter.h"
#include "OpenCLKernelBuilder.h"
using namespace Regards::OpenCL;

OpenCLKernelBuilder&
OpenCLKernelBuilder::Int(
    const wxString& name,
    int value)
{
    auto p = std::make_unique<COpenCLParameterInt>();

    p->SetLibelle(name);
    p->SetValue(value);

    parameters_.push_back(std::move(p));

    return *this;
}

OpenCLKernelBuilder&
OpenCLKernelBuilder::Float(
    const wxString& name,
    float value)
{
    auto p = std::make_unique<COpenCLParameterFloat>();

    p->SetLibelle(name);
    p->SetValue(value);

    parameters_.push_back(std::move(p));

    return *this;
}

OpenCLKernelBuilder&
OpenCLKernelBuilder::Image(
    const wxString& name,
    cl_mem image)
{
    auto p = std::make_unique<COpenCLParameterClMem>(true);

    p->SetLibelle(name);
    p->SetValue(image);
    p->SetNoDelete(true);

    parameters_.push_back(std::move(p));

    return *this;
}

OpenCLKernelBuilder&
OpenCLKernelBuilder::FloatArray(
    const wxString& name,
    cl_context context,
    float* values,
    int size,
    cl_mem_flags flags)
{
    auto p = std::make_unique<COpenCLParameterFloatArray>();

    p->SetLibelle(name);
    p->SetValue(context, values, size, flags);

    parameters_.push_back(std::move(p));

    return *this;
}

OpenCLKernelBuilder&
OpenCLKernelBuilder::IntArray(
    const wxString& name,
    cl_context context,
    int* values,
    int size,
    cl_mem_flags flags)
{
    auto p = std::make_unique<COpenCLParameterIntArray>();

    p->SetLibelle(name);
    p->SetValue(context, values, size, flags);

    parameters_.push_back(std::move(p));

    return *this;
}

OpenCLKernelBuilder&
OpenCLKernelBuilder::ByteArray(
    const wxString& name,
    cl_context context,
    uint8_t * values,
    int size,
    cl_mem_flags flags)
{
    auto p = std::make_unique<COpenCLParameterByteArray>();

    p->SetLibelle(name);
    p->SetValue(context, values, size, flags);

    parameters_.push_back(std::move(p));

    return *this;
}

std::vector<COpenCLParameter*>
OpenCLKernelBuilder::GetParameters()
{
    std::vector<COpenCLParameter*> result;

    result.reserve(parameters_.size());

    for (auto& p : parameters_)
        result.push_back(p.get());

    return result;
}