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

		class COpenCLEngine
		{
		public:          
			static COpenCLContext* CreateInstance();
			static COpenCLContext* Create2DInstance();
			static int SupportOpenCL();
		};

	}
}
