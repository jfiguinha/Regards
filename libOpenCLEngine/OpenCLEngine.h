#pragma once
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <algorithm>
#include <boost/compute/core.hpp>
namespace compute = boost::compute;
using namespace std;

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLContext;

		class COpenCLPlatformList
		{
		public:
			static bool SelectPlatform(const wxString& name, compute::platform& platformSelect);
			static bool SelectPlatform(const int & index, compute::platform& platformSelect);
		};

		

		class COpenCLDeviceList
		{
		public:
			static int SelectDevice(const wxString& platformName, const wxString& deviceName);
			static int SelectDevice(const wxString& platformName, const wxString& deviceName, compute::device& deviceSelect);
			static vector<compute::device> GetPlatformDevice(const compute::platform & platform);
            		
		};

		class COpenCLEngine
		{
		public:
			COpenCLEngine();
			~COpenCLEngine();
            
			static COpenCLContext* CreateInstance(const bool& attachOpenCV);
			static int GetNbPlatform();
			static int GetDefaultGpuDeviceInformation();
			static int SupportOpenCL();
		};

	}
}
