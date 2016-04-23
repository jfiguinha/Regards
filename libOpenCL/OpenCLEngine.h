#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <vector>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
using namespace std;

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLContext;
		class COpenCLProgram;

		class COpenCLDeviceList
		{
		public:
			COpenCLDeviceList(){};
			~COpenCLDeviceList(){};

			int platformIndex;
			wxString platformName;
		};

		class  COpenCLEngine
		{
		public:
			COpenCLEngine(){};
			~COpenCLEngine(){};

			static void Init();
			static vector<COpenCLDeviceList> GetListOfContext();
			static bool IsOpenCLCompatible();
			static COpenCLContext * getInstance();
			static int Initialize(const wxString &platform);
			static void kill();
			static COpenCLProgram * GetProgram(const wxString &numProgramId);
		private:
			
			static COpenCLContext * _singleton;
			//static 
		};

	}
}
