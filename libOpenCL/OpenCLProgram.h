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

		class COpenCLProgram
		{
		public:

			COpenCLProgram(COpenCLContext * context);

			~COpenCLProgram();

			void SetProgramId(const wxString &programId);

			void SetBuildOptions(const wxString &buildOption);

			const cl_program GetProgram();

			const wxString GetProgramId();

		private:

			int CreateAndBuildProgram(const wxString &programData, const wxString &buildOption);
			wxString buildOption;
			wxString numProgramId;
			cl_program program;
			COpenCLContext * context;
		};

	}
}
