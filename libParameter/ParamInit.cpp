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
        wxString filepath = wxStandardPaths::Get().GetExecutablePath();

        
#ifdef WIN32
		
		filepath = filepath.SubString(0, filepath.size() - 4);
        filepath.append("config");
        _singleton = param;
        _singleton->OpenFile(filepath);
#else

        wxStandardPathsBase& stdp = wxStandardPaths::Get();
        wxString documentPath = stdp.GetDocumentsDir();
        documentPath.append("/Regards.config");
        _singleton = param;
        _singleton->OpenFile(documentPath);
#endif


	}
}
