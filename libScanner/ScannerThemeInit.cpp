#include <header.h>
#include "ScannerThemeInit.h"
#include "ScannerTheme.h"
#include <wx/stdpaths.h>
using namespace Regards::Viewer;

CScannerTheme * CScannerThemeInit::_singleton = nullptr;


CScannerThemeInit::CScannerThemeInit()
{
}


CScannerThemeInit::~CScannerThemeInit()
{
}


CScannerTheme * CScannerThemeInit::getInstance()
{
	return _singleton;
}

void CScannerThemeInit::Initialize(CScannerTheme * param)
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

void CScannerThemeInit::SaveTheme()
{
	if (nullptr != _singleton)
	{
		_singleton->SaveFile();
	}
}
