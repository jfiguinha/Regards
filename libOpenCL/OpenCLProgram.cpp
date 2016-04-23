#include "OpenCLProgram.h"
#include "utility.h"
#include "OpenCLEngine.h"
#include "OpenCLContext.h"
#include <libResource.h>
using namespace Regards::OpenCL;

COpenCLProgram::COpenCLProgram(COpenCLContext * context)
{
	program = 0;
	buildOption = "";
	this->context = context;
}

COpenCLProgram::~COpenCLProgram()
{
	if (program)
	{
		clReleaseProgram(program);
	}
}


int COpenCLProgram::CreateAndBuildProgram(const wxString &programData, const wxString &buildOption)
{
    
	const char * raw_text = programData.c_str();
	cl_int err;
	// TODO Using prepared length and not terminating by 0 is better way?
	program = clCreateProgramWithSource(context->GetContext(), 1, &raw_text, 0, &err);
	Error::CheckError(err);

	err = clBuildProgram(program, context->GetNumberOfDevice(), context->GetSelectedDevice(), buildOption.c_str(), 0, 0);

	if (err == CL_BUILD_PROGRAM_FAILURE)
	{
		for (size_t i = 0; i < context->GetNumberOfDevice(); ++i)
		{
			size_t log_length = 0;
			err = clGetProgramBuildInfo(
				program,
				context->GetSelectedDevice()[i],
				CL_PROGRAM_BUILD_LOG,
				0,
				0,
				&log_length
				);
			Error::CheckError(err);

			vector<char> log(log_length);

			err = clGetProgramBuildInfo(
				program,
				context->GetSelectedDevice()[i],
				CL_PROGRAM_BUILD_LOG,
				log_length,
				&log[0],
				0
				);
			Error::CheckError(err);

			wxString error = wxString(&log[0]);

			throw Error(
				"Error happened during the build of OpenCL program.\n"
				"Build log:\n" +
				wxString(&log[0])
				);
		}
	}

	Error::CheckError(err);

	return 0;
}


void COpenCLProgram::SetProgramId(const wxString &programId)
{
	this->numProgramId = programId;
	wxString programData = CLibResource::GetShaderProgram(programId);
	CreateAndBuildProgram(programData, buildOption);
}

void COpenCLProgram::SetBuildOptions(const wxString &buildOption)
{
	this->buildOption = buildOption;
}

const cl_program COpenCLProgram::GetProgram()
{
	return program;
}

const wxString COpenCLProgram::GetProgramId()
{
	return this->numProgramId;
}
