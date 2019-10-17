#include "header.h"
#include "ScannerThemeInit.h"
#include "ScannerTheme.h"
#include <wx/stdpaths.h>
using namespace Regards::Scanner;

CMainTheme * CMainThemeInit::_singleton = nullptr;


CMainThemeInit::CMainThemeInit()
{
}


CMainThemeInit::~CMainThemeInit()
{
}


CMainTheme * CMainThemeInit::getInstance()
{
	return _singleton;
}

void CMainThemeInit::Initialize(CMainTheme * param)
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

void CMainThemeInit::SaveTheme()
{
	if (nullptr != _singleton)
	{
		_singleton->SaveFile();
	}
}
