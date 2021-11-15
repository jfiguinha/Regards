#include <header.h>
#include "OpenCLEngine.h"
#include "utility.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
using namespace Regards::OpenCL;

int COpenCLEngine::SupportOpenCL()
{
	int supportOpenCL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenCL = config->GetIsOpenCLSupport();
	return supportOpenCL;
}