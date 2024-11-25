#include "header.h"
#include "ParamInit.h"
#include "RegardsConfigParam.h"
#include <wx/stdpaths.h>

CRegardsConfigParam* CParamInit::_singleton = nullptr;

CParamInit::CParamInit()
{
}


CParamInit::~CParamInit()
{
}


CRegardsConfigParam* CParamInit::getInstance()
{
	return _singleton;
}

bool CParamInit::IsConfigFileExist()
{
	wxString documentPath = ConfigPath();
	return wxFileExists(documentPath);
}

wxString CParamInit::ConfigPath()
{
    wxStandardPathsBase& stdp = wxStandardPaths::Get();
    wxString documentPath = stdp.GetDocumentsDir();

#ifdef WIN32
    documentPath.append("\\Regards\\Regards.config");
#else
    documentPath.append("/Regards/Regards.config");   
#endif
    return documentPath;
}

void CParamInit::Initialize(CRegardsConfigParam* param)
{
	if (nullptr == _singleton)
	{
		wxString documentPath = ConfigPath();
        _singleton = param;
        _singleton->OpenFile(documentPath);
	}
}
