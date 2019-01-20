#include "SqlOpenCLKernel.h"
#include "SqlLib.h"
#include "SqlEngine.h"
using namespace Regards::Sqlite;

CSqlOpenCLKernel::CSqlOpenCLKernel()
	: CSqlExecuteRequest(L"RegardsDB")
{
	type = 0;
	find = false;
}


CSqlOpenCLKernel::~CSqlOpenCLKernel()
{
}

COpenCLKernelData * CSqlOpenCLKernel::GetOpenCLKernel(const wxString & numProgram, const wxString &platformName, const int &numDevice)
{
	type = 1;
	ExecuteRequest("SELECT openCLKernel FROM OPENCLKERNEL WHERE numProgram = '" + numProgram + "'  and platformName = '" + platformName + "' and numDevice = " + to_string(numDevice));
	return memData;
}

bool CSqlOpenCLKernel::TestOpenCLKernel(const wxString & numProgram, const wxString &platformName, const int &numDevice)
{
	type = 2;
	ExecuteRequest("SELECT numProgram FROM OPENCLKERNEL WHERE numProgram = '" + numProgram + "' and platformName = '" + platformName + "' and numDevice = " + to_string(numDevice));
	return find;
}

void CSqlOpenCLKernel::EraseOpenCLKernelTable()
{
    ExecuteRequest("DELETE FROM OPENCLKERNEL");
}

bool CSqlOpenCLKernel::InsertOpenCLKernel(const unsigned char * zBlob, const size_t &nBlob, const wxString & numProgram, const wxString &platformName, const int &numDevice)
{
	return ExecuteInsertBlobData("INSERT INTO OPENCLKERNEL (numProgram, platformName, numDevice, openCLKernel) VALUES('" + numProgram + "', '" + platformName + "'," + to_string(numDevice) + ",? )", 4, zBlob, nBlob);
}

int CSqlOpenCLKernel::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (type)
		{

		case 1:
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{

				switch (i)
				{
				case 0:
				{
					int size = sqlResult->ColumnDataBlobSize(i);
					if (size > 0)
					{	
                        memData = new COpenCLKernelData();
                        memData->program_size = size;
						memData->program_file = new unsigned char[size];
						sqlResult->ColumnDataBlob(i, (void * &)memData->program_file, memData->program_size);
					}
				}
				break;
				}
			}
			break;

		case 2:
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					find = true;
					break;
				}
			}
			break;

		}

		nbResult++;
	}
	return nbResult;
};
