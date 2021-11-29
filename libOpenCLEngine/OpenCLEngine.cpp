#include <header.h>
#include "OpenCLEngine.h"
#include "utility.h"
#include "OpenCLContext.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <boost/compute/core.hpp>
#include <boost/compute/interop/opengl/context.hpp>
namespace compute = boost::compute;
#include <opencv2/core/ocl.hpp>
using namespace Regards::OpenCL;

COpenCLContext * COpenCLEngine::CreateInstance()
{
	COpenCLContext* contextLocal = nullptr;
	try
	{

		compute::context context = boost::compute::opengl_create_shared_context();
		contextLocal = new COpenCLContext(context,
			true);
	}
	catch (...)
	{
		compute::device gpu = compute::system::default_device();
		compute::context context(gpu);
		contextLocal = new COpenCLContext(context,
			false);
	}

	cv::ocl::setUseOpenCL(true);
	contextLocal->SetOpenCVContext(cv::ocl::OpenCLExecutionContext::create(
		contextLocal->GetContext().get_device().platform().name(), contextLocal->GetContext().get_device().platform().id(), contextLocal->GetContext(), contextLocal->GetContext().get_device().id()));

	return contextLocal;
}


COpenCLContext* COpenCLEngine::Create2DInstance()
{
	COpenCLContext* contextLocal = nullptr;
	try
	{

		compute::device gpu = compute::system::default_device();
		compute::context context(gpu);
		contextLocal = new COpenCLContext(context,
			false);
	}
	catch (...)
	{
		return nullptr;
	}

	cv::ocl::setUseOpenCL(true);
	contextLocal->SetOpenCVContext(cv::ocl::OpenCLExecutionContext::create(
		contextLocal->GetContext().get_device().platform().name(), contextLocal->GetContext().get_device().platform().id(), contextLocal->GetContext(), contextLocal->GetContext().get_device().id()));

	return contextLocal;
}

int COpenCLEngine::SupportOpenCL()
{
	int supportOpenCL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenCL = config->GetIsOpenCLSupport();
	return supportOpenCL;
}
