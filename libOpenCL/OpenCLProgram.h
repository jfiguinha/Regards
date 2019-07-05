#pragma once
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLContext;

		class COpenCLProgram
		{
		public:

			COpenCLProgram(COpenCLContext * context, const int& type);

			~COpenCLProgram();

			void SetProgramId(const wxString &programId);

			void SetBuildOptions(const wxString &buildOption);

			const cl_program GetProgram();

			const wxString GetProgramId();

			const int GetDefaultType();


		private:

			int CreateAndBuildProgram(const wxString &programId, const wxString &programData, const wxString &buildOption);
            
            bool LoadProgramFromBinaries(const wxString &programId);
            
			wxString buildOption;
			wxString numProgramId;
			cl_program program;
			int typeData;
			COpenCLContext * context;
            
            bool loadFromDatabase = false;
		};

	}
}
