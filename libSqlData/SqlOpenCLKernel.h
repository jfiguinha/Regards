#pragma once
#include <SqlExecuteRequest.h>
#include "SqlResult.h"

class CRegardsBitmap;

class COpenCLKernelData
{
public:
    COpenCLKernelData()
    {
        
    }
    
    ~COpenCLKernelData()
    {
        if(program_file != nullptr)
            delete[] program_file;
        program_file = nullptr;
    }

    unsigned char* program_file = nullptr;
    size_t program_size = 0;
};

namespace Regards
{
	namespace Sqlite
	{
		class CSqlOpenCLKernel : public CSqlExecuteRequest
		{
		public:
			CSqlOpenCLKernel();
			~CSqlOpenCLKernel();
            bool InsertOpenCLKernel(const unsigned char * zBlob, const size_t &nBlob, const wxString &numProgram, const wxString &platformName, const int &numDevice);
            COpenCLKernelData * GetOpenCLKernel(const wxString & numProgram, const wxString &platformName, const int &numDevice);
            bool TestOpenCLKernel(const wxString & numProgram, const wxString &platformName, const int &numDevice);
            void EraseOpenCLKernelTable();

		private:
			int TraitementResult(CSqlResult * sqlResult);
			int type;
			bool find;
			COpenCLKernelData * memData = nullptr;
		};
	}
}
