#include "header.h"
#include "ParamInit.h"
#include "RegardsConfigParam.h"
#include <wx/stdpaths.h>

CRegardsConfigParam * CParamInit::_singleton = nullptr;

CParamInit::CParamInit()
{
}


CParamInit::~CParamInit()
{
}


CRegardsConfigParam * CParamInit::getInstance()
{
	return _singleton;
}

void CParamInit::Initialize(CRegardsConfigParam * param)
{
	if (nullptr == _singleton)
	{
        
        wxStandardPathsBase& stdp = wxStandardPaths::Get();
        wxString documentPath = stdp.GetDocumentsDir();
        
#ifdef WIN32
		
        documentPath.append("\\Regards\\Regards.config");
        _singleton = param;
        _singleton->OpenFile(documentPath);
#else


        documentPath.append("/Regards/Regards.config");
        _singleton = param;
        _singleton->OpenFile(documentPath);
#endif


	}
}
