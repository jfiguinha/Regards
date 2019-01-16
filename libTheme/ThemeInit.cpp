#include "ThemeInit.h"
#include "ThemeParam.h"
#include <wx/stdpaths.h>

CThemeParam * CThemeInit::_singleton = nullptr;


CThemeInit::CThemeInit()
{
}


CThemeInit::~CThemeInit()
{
}


CThemeParam * CThemeInit::getInstance()
{
	return _singleton;
}

void CThemeInit::Initialize(CThemeParam * param)
{
	if (nullptr == _singleton)
	{
        /*
		wxString filename = wxStandardPaths::Get().GetExecutablePath();
		filename.append(L".theme");
		_singleton = param;
		_singleton->OpenFile(filename);
        */
        wxString filepath = wxStandardPaths::Get().GetExecutablePath();

        
#ifdef WIN32
		
		filepath = filepath.SubString(0, filepath.size() - 4);
        filepath.append("theme");
        _singleton = param;
        _singleton->OpenFile(filepath);
#else

        wxStandardPathsBase& stdp = wxStandardPaths::Get();
        wxString documentPath = stdp.GetDocumentsDir();
        documentPath.append("/Regards.theme");
        _singleton = param;
        _singleton->OpenFile(documentPath);
#endif

	}
}
