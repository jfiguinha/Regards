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
        wxStandardPathsBase& stdp = wxStandardPaths::Get();
        wxString documentPath = stdp.GetDocumentsDir();       

        
#ifdef WIN32
		
        documentPath.append("\\Regards\\Regards.viewer.config");
        _singleton = param;
        _singleton->OpenFile(documentPath);
#else


        documentPath.append("/Regards/Regards.viewer.config");
        _singleton = param;
        _singleton->OpenFile(documentPath);
#endif


	}
}
