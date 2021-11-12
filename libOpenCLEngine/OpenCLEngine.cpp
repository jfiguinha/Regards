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


bool COpenCLPlatformList::SelectPlatform(const wxString& name, compute::platform & platformSelect)
{
	bool findPlatform = false;
	std::vector<compute::platform> platforms = compute::system::platforms();

	vector<compute::platform>::iterator i = std::find_if(platforms.begin(),
		platforms.end(),
		[&](const auto& val) { return val.name() == name.ToStdString(); });

	if (i != platforms.end())
	{
		findPlatform = true;
		platformSelect = *i;
	}

	return findPlatform;
}

bool COpenCLPlatformList::SelectPlatform(const int & index, compute::platform& platformSelect)
{
	bool findPlatform = false;
	std::vector<compute::platform> platforms = compute::system::platforms();
	if (index < platforms.size())
	{
		findPlatform = true;
		platformSelect = platforms[index];
	}

	return findPlatform;
}

COpenCLEngine::COpenCLEngine()
{
	//_singleton = CreateInstance(attachOpenCV);
}

COpenCLContext * COpenCLEngine::CreateInstance(const bool& attachOpenCV)
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

#ifdef OPENCV_OPENCL
	if (attachOpenCV)
	{
		cv::ocl::setUseOpenCL(true);
		contextLocal->SetOpenCVContext(cv::ocl::OpenCLExecutionContext::create(
			contextLocal->GetContext().get_device().platform().name(), contextLocal->GetContext().get_device().platform().id(), contextLocal->GetContext(), contextLocal->GetContext().get_device().id()));

		//Set OpenCV to use OpenCL context
		//
		//cv::ocl::attachContext(platform->platformName.ToStdString(), platform->platformId, _singleton->GetContext(), device->deviceId);
	}
#endif

	return contextLocal;
}


COpenCLEngine::~COpenCLEngine()
{

}

int COpenCLDeviceList::SelectDevice(const wxString& platformName, const wxString& deviceName, compute::device& deviceSelect)
{
	std::vector<compute::platform> platforms = compute::system::platforms();
	bool findPlatform = false;
	bool findDevice = false;
	int index_platform = 0;
	int index_device = -1;
	for (compute::platform platform : platforms)
	{
		if (platformName == platform.name())
		{
			findPlatform = true;
			break;
		}
		index_platform++;
	}

	if (findPlatform)
	{
		for (int i = 0; i < platforms[index_platform].devices().size(); i++)
		{
			if (platforms[index_platform].devices()[i].name() >= deviceName)
			{
				deviceSelect = platforms[index_platform].devices()[i];
				findDevice = true;
				index_device = i;
			}
		}
			
	}

	return index_device;
}

int COpenCLDeviceList::SelectDevice(const wxString& platformName, const wxString& deviceName)
{
	std::vector<compute::platform> platforms = compute::system::platforms();
	bool findPlatform = false;
	bool findDevice = false;
	int index_platform = 0;
	int index_device = 0;
	for (compute::platform platform : platforms)
	{
		if (platformName == platform.name())
		{
			findPlatform = true;
			break;
		}
		index_platform++;
	}

	if (findPlatform)
	{
		for (int i = 0; i < platforms[index_platform].devices().size(); i++)
		{
			if (platforms[index_platform].devices()[i].name() >= deviceName)
			{
				findDevice = true;
				index_device = i;
			}
		}

	}

	return index_device;
}

int COpenCLEngine::SupportOpenCL()
{
	int supportOpenCL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenCL = config->GetIsOpenCLSupport();
	return supportOpenCL;
}

int COpenCLEngine::GetNbPlatform()
{
	std::vector<compute::platform> platforms = compute::system::platforms();
	return platforms.size();
}

int COpenCLEngine::GetDefaultGpuDeviceInformation()
{
	std::vector<compute::platform> platforms = compute::system::platforms();
	int deviceIndex = 0;
	int platformIndex = 0;
	compute::device gpu = compute::system::default_device();

	for (int i = 0; i < platforms.size(); i++)
	{
		compute::platform platform = platforms[i];
		if (platform.id() == gpu.platform().id())
		{
			platformIndex = i;
			break;
		}
	}


	for (int j = 0; j < platforms[platformIndex].devices().size(); j++)
	{
		compute::device device = platforms[platformIndex].devices()[j];
		if (gpu.id() == device.id())
		{
			deviceIndex = j;
			break;
		}
	}
	
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
	{
		config->SetOpenCLPlatformIndex(deviceIndex);
		config->SetOpenCLPlatformName(gpu.platform().name());
	}

	return 0;
}
