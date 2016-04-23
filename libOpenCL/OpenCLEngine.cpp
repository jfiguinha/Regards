#include "OpenCLEngine.h"
#include "utility.h"
#include "OpenCLContext.h"
#include "OpenCLProgram.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <IFiltreEffet.h>
#if defined(__WXMSW__)
#include "../include/config_id.h"
#else
#include <config_id.h>
#endif

#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
using namespace Regards::OpenCL;

// Initialisation du singleton à nullptr
COpenCLContext *COpenCLEngine::_singleton = nullptr;
vector<COpenCLProgram *> listProgram;

void COpenCLEngine::Init()
{
	vector<COpenCLDeviceList> listDevice = COpenCLEngine::GetListOfContext();

	CRegardsConfigParam * config = CParamInit::getInstance();
	//Initialise OpenCL
	if (config->GetEffectLibrary() == 0)
	{
		vector<COpenCLDeviceList> listDevice;
		listDevice = COpenCLEngine::GetListOfContext();
		if (listDevice.size() > 0)
		{
			COpenCLDeviceList clDevice = listDevice[0];
			config->SetEffectLibrary(LIBOPENCL);
			config->SetPreviewLibrary(LIBOPENCL);
			config->SetVideoLibrary(LIBOPENGL);
			config->SetOpenCLPlatformIndex(clDevice.platformIndex);
			config->SetOpenCLPlatformName(clDevice.platformName);
		}
		else
		{
			config->SetEffectLibrary(LIBCPU);
			config->SetPreviewLibrary(LIBCPU);
			config->SetVideoLibrary(LIBCPU);
		}
	}

	if (config->GetEffectLibrary() == LIBOPENCL)
		COpenCLEngine::Initialize(config->GetOpenCLPlatformName());

}

COpenCLContext * COpenCLEngine::getInstance()
{
	return _singleton;
}

vector<COpenCLDeviceList> COpenCLEngine::GetListOfContext()
{
    Error error("error");
	vector<COpenCLDeviceList> listDevice;


	cl_uint num_of_platforms = 0;
	// get total number of available platforms:
	cl_int err = clGetPlatformIDs(0, 0, &num_of_platforms);
	error.CheckError(err);

	// use vector for automatic memory management
	vector<cl_platform_id> platforms(num_of_platforms);
	// get IDs for all platforms:
	err = clGetPlatformIDs(num_of_platforms, &platforms[0], 0);
	error.CheckError(err);

	//cl_uint selected_platform_index = num_of_platforms;
	//bool by_index = false;

	//cout << "Platforms (" << num_of_platforms << "):\n";

	// TODO In case of empty platform name select the default platform or 0th platform?

	for (cl_uint i = 0; i < num_of_platforms; ++i)
	{
		// Get the length for the i-th platform name
		size_t platform_name_length = 0;
		err = clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			0,
			0,
			&platform_name_length
			);
		error.CheckError(err);

		

		// Get the name itself for the i-th platform
		// use vector for automatic memory management
		vector<char> platform_name(platform_name_length);
		err = clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			platform_name_length,
			&platform_name[0],
			0
			);
		error.CheckError(err);

		COpenCLDeviceList device;
		device.platformIndex = i;
		device.platformName = wxString(&platform_name[0]);
		listDevice.push_back(device);
		//cout << "    [" << i << "] " << &platform_name[0];
	}


	return listDevice;
}



bool COpenCLEngine::IsOpenCLCompatible()
{
	return (_singleton != nullptr) ? true : false;
}

int COpenCLEngine::Initialize(const wxString &platform)
{
	int rtnValue = 0;
	if (nullptr == _singleton)
	{
		_singleton = new COpenCLContext();
		rtnValue = _singleton->GenerateContext(platform);
		if (rtnValue == -1)
		{
			delete _singleton;
			_singleton = nullptr;
		}
	}
	else
	{
		delete _singleton;
		_singleton = new COpenCLContext();
		rtnValue = _singleton->GenerateContext(platform);
		if (rtnValue == -1)
		{
			delete _singleton;
			_singleton = nullptr;
		}

	}
	return rtnValue;
}

COpenCLProgram * COpenCLEngine::GetProgram(const wxString &numProgramId)
{
	bool findPreloadShader = false;
	COpenCLProgram * program = nullptr;
	for (COpenCLProgram * programLocal : listProgram)
	{
		program = programLocal;
		if (program->GetProgramId() == numProgramId)
		{
			findPreloadShader = true;
			break;
		}
	}

	//Ajout du program en pré chargement
	if (!findPreloadShader)
	{
		program = new COpenCLProgram(_singleton);
		program->SetProgramId(numProgramId);
		listProgram.push_back(program);
	}

	return program;
}

void COpenCLEngine::kill()
{
	if (nullptr != _singleton)
	{
		delete _singleton;
		_singleton = nullptr;
	}

	for (COpenCLProgram * program : listProgram)
	{
		delete program;
		program = nullptr;
	}

	listProgram.clear();
}
