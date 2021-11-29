// ReSharper disable All
#include <header.h>
#include "OpenCLProgram.h"
#include "utility.h"
#include "OpenCLContext.h"
#include <LibResource.h>
#include <SqlOpenCLKernel.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
using namespace Regards::OpenCL;
using namespace Regards::Sqlite;


COpenCLProgram::COpenCLProgram(COpenCLContext* context, const int& type)
{
	buildOption = "-cl-mad-enable -cl-unsafe-math-optimizations";
	this->context = context;
	this->typeData = type; // context->GetDefaultType();
}

COpenCLProgram::~COpenCLProgram()
{
}

int COpenCLProgram::CreateAndBuildProgram(const wxString& programId, const wxString& programData)
{
	CSqlOpenCLKernel sqlOpenCLKernel;
	wxString platformName = "";
	int indexDevice = -1;

	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
	{
		platformName = config->GetOpenCLPlatformName();
		indexDevice = config->GetOpenCLPlatformIndex();
		loadFromDatabase = config->GetOpenCLLoadFromBinaries();
	}

	compute::context context_local = context->GetContext();
	try
	{
		program = compute::program::build_with_source(programData.ToStdString(), context_local, buildOption.ToStdString());
	}
	catch(boost::compute::opencl_error error_cl)
	{
#if defined(WIN32) && defined(_DEBUG)
		//OutputDebugString(error_cl.error_string().c_str());
#else
		printf(CConvertUtility::ConvertToUTF8(error_cl.error_string().c_str()));
#endif
	}
	
	return 0;
}

const int COpenCLProgram::GetDefaultType()
{
	return typeData;
}


void COpenCLProgram::SetProgramId(const wxString& programId)
{
	this->numProgramId = programId;
	wxString programData = "";
	if (typeData == OPENCL_FLOAT)
		programData = CLibResource::GetOpenCLFloatProgram(programId);
	else if (typeData == OPENCL_UCHAR)
		programData = CLibResource::GetOpenCLUcharProgram(programId);
	CreateAndBuildProgram(programId, programData);

}

void COpenCLProgram::SetBuildOptions(const wxString& buildOption)
{
	this->buildOption = buildOption;
}

const wxString COpenCLProgram::GetProgramId()
{
	return this->numProgramId;
}
