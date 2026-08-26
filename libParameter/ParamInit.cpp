#include "header.h"
#include "ParamInit.h"
#include "RegardsConfigParam.h"
#include <FileUtility.h>
#include <wx/filename.h>
std::unique_ptr<CRegardsConfigParam> CParamInit::_singleton = nullptr;

CRegardsConfigParam* CParamInit::getInstance()
{
    if (nullptr == _singleton)
    {
        wxString documentPath = ConfigPath();
        _singleton = std::make_unique<CRegardsConfigParam>();
        _singleton->OpenFile(documentPath);
    }
	return _singleton.get();
}

bool CParamInit::IsConfigFileExist()
{
	wxString documentPath = ConfigPath();
	return wxFileExists(documentPath);
}

wxString CParamInit::ConfigPath()
{
    return wxFileName(CFileUtility::GetDocumentFolderPath(), "Regards.config").GetFullPath();
}

