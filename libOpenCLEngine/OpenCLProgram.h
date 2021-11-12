#pragma once
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <boost/compute/core.hpp>
namespace compute = boost::compute;
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

			const compute::program& GetProgram()
			{
				return program;
			}

			const wxString GetProgramId();

			const int GetDefaultType();


		private:

			wxString GetOpenCLIncludeDir();

			int CreateAndBuildProgram(const wxString &programId, const wxString &programData);
            
            bool LoadProgramFromBinaries(const wxString &programId);
            
			wxString buildOption;
			wxString numProgramId;
			compute::program program;
			int typeData;
			COpenCLContext * context;
            
            bool loadFromDatabase = false;
		};

	}
}
