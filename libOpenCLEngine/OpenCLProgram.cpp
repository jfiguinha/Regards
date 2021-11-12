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
		bool err = false;
		COpenCLKernelData* outputData = sqlOpenCLKernel.GetOpenCLKernel(programId, platformName, indexDevice, typeData);
		if (outputData != nullptr)
		{
			try
			{
				program = compute::program::create_with_binary(outputData->program_file, outputData->program_size, context->GetContext());
			}
			catch (...)
			{
				err = true;
			}
			delete outputData;
			outputData = nullptr;

			if (err == false)
				return true;
		}
	}

	return false;
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

	bool error = false;
	compute::context context_local = context->GetContext();
	try
	{
		program = compute::program::build_with_source(programData.ToStdString(), context_local, buildOption.ToStdString());
	}
	catch(...)
	{
		error = true;
		for (size_t i = 0; i < context_local.get_devices().size(); i++)
		{
			size_t log_length = 0;
			cl_int err = clGetProgramBuildInfo(
				program,
				context_local.get_devices()[i].id(),
				CL_PROGRAM_BUILD_LOG,
				0,
				nullptr,
				&log_length
			);
			Error::CheckError(err);

			vector<char> log(log_length);

			err = clGetProgramBuildInfo(
				program,
				context_local.get_devices()[i].id(),
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
	
	if (!error)
	{
		if (!sqlOpenCLKernel.TestOpenCLKernel(programId, platformName, indexDevice))
		{
			unsigned i;
			cl_int err = CL_SUCCESS;
			size_t* binaries_size = nullptr;
			unsigned char** binaries_ptr = nullptr;

			try
			{
				// Read the binaries size
				const size_t binaries_size_alloc_size = sizeof(size_t) * context->GetNumberOfDevice();
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
				const size_t binaries_ptr_alloc_size = sizeof(unsigned char*) * context->GetNumberOfDevice();
				binaries_ptr = static_cast<unsigned char**>(malloc(binaries_ptr_alloc_size));
				if (!binaries_ptr)
				{
					err = CL_OUT_OF_HOST_MEMORY;
					throw std::invalid_argument("Error");
				}

				memset(binaries_ptr, 0, binaries_ptr_alloc_size);
				for (i = 0; i < context->GetNumberOfDevice(); ++i)
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

				if (indexDevice < context->GetNumberOfDevice())
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
				for (i = 0; i < context->GetNumberOfDevice(); ++i)
				{
					free(binaries_ptr[i]);
				}
				free(binaries_ptr);
			}
			free(binaries_size);
		}
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
		CreateAndBuildProgram(programId, programData);
	}
}

void COpenCLProgram::SetBuildOptions(const wxString& buildOption)
{
	this->buildOption = buildOption;
}

const wxString COpenCLProgram::GetProgramId()
{
	return this->numProgramId;
}
