#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <wx/stdpaths.h>
using namespace Regards::Viewer;

CViewerParam * CViewerParamInit::_singleton = nullptr;

CViewerParamInit::CViewerParamInit()
{
}


CViewerParamInit::~CViewerParamInit()
{
}


CViewerParam * CViewerParamInit::getInstance()
{
	return _singleton;
}

void CViewerParamInit::Initialize(CViewerParam * param)
{
	if (nullptr == _singleton)
	{
		wxString filepath = wxStandardPaths::Get().GetExecutablePath();
#ifdef WIN32
        
        filepath = filepath.SubString(0, filepath.size() - 4);
        filepath.append("viewer.config");
        _singleton = param;
        _singleton->OpenFile(filepath);
        
#else
        wxStandardPathsBase& stdp = wxStandardPaths::Get();
        wxString documentPath = stdp.GetDocumentsDir();
        documentPath.append("/RegardsViewer.config");
        _singleton = param;
        _singleton->OpenFile(documentPath);
#endif


	}
}
