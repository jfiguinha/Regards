#include "ExplorerParamInit.h"
#include "ExplorerParam.h"
#include <wx/stdpaths.h>
using namespace Regards::Explorer;

CExplorerParam * CExplorerParamInit::_singleton = nullptr;

CExplorerParamInit::CExplorerParamInit()
{
}


CExplorerParamInit::~CExplorerParamInit()
{
}


CExplorerParam * CExplorerParamInit::getInstance()
{
	return _singleton;
}

void CExplorerParamInit::Initialize(CExplorerParam * param)
{
	if (nullptr == _singleton)
	{

#ifdef WIN32
        wxString filepath = wxStandardPaths::Get().GetExecutablePath();
        filepath = filepath.SubString(0, filepath.size() - 4);
        filepath.append("explorer.config");
        _singleton = param;
        _singleton->OpenFile(filepath);
        
#else
        wxStandardPathsBase& stdp = wxStandardPaths::Get();
        wxString documentPath = stdp.GetDocumentsDir();
        documentPath.append("/RegardsExplorer.config");
        _singleton = param;
        _singleton->OpenFile(documentPath);
#endif


	}
}
