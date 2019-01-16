#include "ViewerThemeInit.h"
#include "ViewerTheme.h"
#include <wx/stdpaths.h>
using namespace Regards::Viewer;

CViewerTheme * CViewerThemeInit::_singleton = nullptr;


CViewerThemeInit::CViewerThemeInit()
{
}


CViewerThemeInit::~CViewerThemeInit()
{
}


CViewerTheme * CViewerThemeInit::getInstance()
{
	return _singleton;
}

void CViewerThemeInit::Initialize(CViewerTheme * param)
{
	if (nullptr == _singleton)
	{
        /*
		wxString filename = wxStandardPaths::Get().GetExecutablePath();
		filename.append(L".viewer.theme");
		_singleton = param;
		_singleton->OpenFile(filename);
        */
 
        wxStandardPathsBase& stdp = wxStandardPaths::Get();
        wxString documentPath = stdp.GetDocumentsDir();       

        
#ifdef WIN32
		
        documentPath.append("\\Regards\\Regards.viewer.theme");
        _singleton = param;
        _singleton->OpenFile(documentPath);
#else


        documentPath.append("/Regards/Regards.viewer.theme");
        _singleton = param;
        _singleton->OpenFile(documentPath);
#endif
	}
}

void CViewerThemeInit::SaveTheme()
{
	if (nullptr != _singleton)
	{
		_singleton->SaveFile();
	}
}
