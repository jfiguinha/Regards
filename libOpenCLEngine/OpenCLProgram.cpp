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

extern COpenCLContext* openclContext;

COpenCLProgram::COpenCLProgram(const int& type)
{
	program = nullptr;
	buildOption = "-cl-mad-enable -cl-unsafe-math-optimizations";
	this->typeData = type; // context->GetDefaultType();
}

COpenCLProgram::~COpenCLProgram()
{
	if (program)
	{
		clReleaseProgram(program);
	}
}

bool COpenCLProgram::LoadProgramFromBinaries(const wxString& programId)
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

	if (sqlOpenCLKernel.TestOpenCLKernel(programId, platformName, indexDevice) && loadFromDatabase)
	{
		cl_int err;
		COpenCLKernelData* outputData = sqlOpenCLKernel.GetOpenCLKernel(programId, platformName, indexDevice, typeData);
		if (outputData != nullptr)
		{
			program = clCreateProgramWithBinary(openclContext->GetContext(), openclContext->GetNumberOfDevice(),
				openclContext->GetSelectedDevice(), &outputData->program_size,
			                                    const_cast<const unsigned char**>(&outputData->program_file),
			                                    nullptr, &err);

			err = clBuildProgram(program, openclContext->GetNumberOfDevice(), openclContext->GetSelectedDevice(), nullptr, nullptr,
			                     nullptr);

			delete outputData;
			outputData = nullptr;

			if (err == CL_SUCCESS)
				return true;
		}
	}

	return false;
}

int COpenCLProgram::CreateAndBuildProgram(const wxString& programId, const wxString& programData,
                                          const wxString& buildOption)
{
	wxString platformName = "";
	int indexDevice = -1;

	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
	{
		platformName = config->GetOpenCLPlatformName();
		indexDevice = config->GetOpenCLPlatformIndex();
	}

	//Save the precompiled data
	CSqlOpenCLKernel sqlOpenCLKernel;

	const char* raw_text = programData.c_str();
#if not defined(NDEBUG)
    printf("OpenCL code : \n %s \n", raw_text);
#endif
	cl_int err;
	// TODO Using prepared length and not terminating by 0 is better way?

	program = clCreateProgramWithSource(openclContext->GetContext(), 1, &raw_text, nullptr, &err);
	Error::CheckError(err);

	err = clBuildProgram(program, openclContext->GetNumberOfDevice(), openclContext->GetSelectedDevice(), buildOption.c_str(),
	                     nullptr, nullptr);
	Error::CheckError(err);

	if (err == CL_BUILD_PROGRAM_FAILURE)
	{
		for (size_t i = 0; i < openclContext->GetNumberOfDevice(); i++)
		{
			size_t log_length = 0;
			err = clGetProgramBuildInfo(
				program,
				openclContext->GetSelectedDevice()[i],
				CL_PROGRAM_BUILD_LOG,
				0,
				nullptr,
				&log_length
			);
			Error::CheckError(err);

			vector<char> log(log_length);

			err = clGetProgramBuildInfo(
				program,
				openclContext->GetSelectedDevice()[i],
				CL_PROGRAM_BUILD_LOG,
				log_length,
				&log[0],
				nullptr
			);
			Error::CheckError(err);

			auto error = wxString(&log[0]);
#if defined(WIN32) && defined(_DEBUG)
			OutputDebugString(error);
#else
			printf(CConvertUtility::ConvertToUTF8(error));
#endif
			throw Error(
				"Error happened during the build of OpenCL program.\n"
				"Build log:\n" +
				wxString(&log[0])
			);
		}
	}
	else if (!sqlOpenCLKernel.TestOpenCLKernel(programId, platformName, indexDevice))
	{
		unsigned i;
		cl_int err = CL_SUCCESS;
		size_t* binaries_size = nullptr;
		unsigned char** binaries_ptr = nullptr;

		try
		{
			// Read the binaries size
			const size_t binaries_size_alloc_size = sizeof(size_t) * openclContext->GetNumberOfDevice();
			binaries_size = static_cast<size_t*>(malloc(binaries_size_alloc_size));
			if (!binaries_size)
			{
				err = CL_OUT_OF_HOST_MEMORY;
				throw std::invalid_argument("Error");
			}

			err = clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, binaries_size_alloc_size, binaries_size, nullptr);
			if (err != CL_SUCCESS)
			{
				throw std::invalid_argument("Error");
			}

			// Read the binaries
			const size_t binaries_ptr_alloc_size = sizeof(unsigned char*) * openclContext->GetNumberOfDevice();
			binaries_ptr = static_cast<unsigned char**>(malloc(binaries_ptr_alloc_size));
			if (!binaries_ptr)
			{
				err = CL_OUT_OF_HOST_MEMORY;
				throw std::invalid_argument("Error");
			}

			memset(binaries_ptr, 0, binaries_ptr_alloc_size);
			for (i = 0; i < openclContext->GetNumberOfDevice(); ++i)
			{
				binaries_ptr[i] = static_cast<unsigned char*>(malloc(binaries_size[i]));
				if (!binaries_ptr[i])
				{
					err = CL_OUT_OF_HOST_MEMORY;
					throw std::invalid_argument("Error");
				}
			}

			err = clGetProgramInfo(program, CL_PROGRAM_BINARIES,
			                       binaries_ptr_alloc_size,
			                       binaries_ptr, nullptr);
			if (err != CL_SUCCESS)
			{
				throw std::invalid_argument("Error");
			}

			if (indexDevice < openclContext->GetNumberOfDevice())
			{
				sqlOpenCLKernel.InsertOpenCLKernel(binaries_ptr[indexDevice], binaries_size[indexDevice], programId,
				                                   platformName, indexDevice, typeData);
			}
		}
		catch (...)
		{
		}
		// Free the return value buffer
		if (binaries_ptr)
		{
			for (i = 0; i < openclContext->GetNumberOfDevice(); ++i)
			{
				free(binaries_ptr[i]);
			}
			free(binaries_ptr);
		}
		free(binaries_size);
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
	if (!LoadProgramFromBinaries(programId))
	{
		wxString programData = "";
		if (typeData == OPENCL_FLOAT)
			programData = CLibResource::GetOpenCLFloatProgram(programId);
		else if (typeData == OPENCL_UCHAR)
			programData = CLibResource::GetOpenCLUcharProgram(programId);
		CreateAndBuildProgram(programId, programData, buildOption);
	}
}

void COpenCLProgram::SetBuildOptions(const wxString& buildOption)
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
