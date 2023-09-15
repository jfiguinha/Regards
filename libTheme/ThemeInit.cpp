#include "header.h"
#include "ThemeInit.h"
#include "ThemeParam.h"
#include <FileUtility.h>
#include <wx/stdpaths.h>
#include <wx/settings.h>
CThemeParam* CThemeInit::_singleton = nullptr;


CThemeInit::CThemeInit()
{
}


CThemeInit::~CThemeInit()
{
}


CThemeParam* CThemeInit::getInstance()
{
	return _singleton;
}

void CThemeInit::Initialize(CThemeParam* param)
{
	if (nullptr == _singleton)
	{
        
        
        
		/*
		wxString filename = wxStandardPaths::Get().GetExecutablePath();
		filename.append(L".theme");
		_singleton = param;
		_singleton->OpenFile(filename);
		*/
        wxSystemAppearance systemApp = wxSystemSettings::GetAppearance();
		wxString filepath = wxStandardPaths::Get().GetExecutablePath();
        bool isDarkTheme =  systemApp.IsDark();

#ifdef WIN32
 
		filepath = filepath.SubString(0, filepath.size() - 4);
        if(isDarkTheme)
            filepath.append("theme.dark");
        else
            filepath.append("theme.light");
		_singleton = param;
		_singleton->OpenFile(filepath);
#else

        wxStandardPathsBase& stdp = wxStandardPaths::Get();
        wxString documentPath = stdp.GetDocumentsDir();
        if(isDarkTheme)
            documentPath.append("/Regards.theme.dark");
        else
            documentPath.append("/Regards.theme.light");
        _singleton = param;
        _singleton->OpenFile(documentPath);
#endif
	}
}
