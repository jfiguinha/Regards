#include "header.h"
#include "ScannerThemeInit.h"
#include "ScannerTheme.h"
#include <FileUtility.h>
#include <wx/stdpaths.h>
using namespace Regards::Scanner;

CMainTheme* CMainThemeInit::_singleton = nullptr;


CMainThemeInit::CMainThemeInit()
{
}


CMainThemeInit::~CMainThemeInit()
{
}


CMainTheme* CMainThemeInit::getInstance()
{
	if (nullptr == _singleton)
	{
		auto viewerTheme = new CMainTheme();
		Initialize(viewerTheme);
		return viewerTheme;
	}
	return _singleton;
}

void CMainThemeInit::Initialize(CMainTheme* param)
{
	if (nullptr == _singleton)
	{
		/*
		wxString filename = wxStandardPaths::Get().GetExecutablePath();
		filename.append(L".viewer.theme");
		_singleton = param;
		_singleton->OpenFile(filename);
		*/
        
        
         wxSystemAppearance systemApp = wxSystemSettings::GetAppearance();
		wxStandardPathsBase& stdp = wxStandardPaths::Get();
		wxString documentPath = stdp.GetDocumentsDir();
         bool isDarkTheme =  systemApp.IsDark();
         wxString resourceTheme = CFileUtility::GetResourcesFolderPath();
         
#ifdef WIN32

        if(isDarkTheme)
            documentPath.append("\\Regards\\Regards.viewer.dark.theme");
        else
            documentPath.append("\\Regards\\Regards.viewer.light.theme");
            
        if(isDarkTheme)
            resourceTheme.append("\\theme\\Regards.viewer.dark.theme");
        else
            resourceTheme.append("\\theme\\Regards.viewer.light.theme");
            

#else

        if(isDarkTheme)
            documentPath.append("/Regards/Regards.viewer.dark.theme");
        else
            documentPath.append("/Regards/Regards.viewer.light.theme");
            
        if(isDarkTheme)
            resourceTheme.append("/theme/Regards.viewer.dark.theme");
        else
            resourceTheme.append("/theme/Regards.viewer.light.theme");

#endif

        if(!wxFileExists(documentPath))
            wxCopyFile(resourceTheme, documentPath);

		_singleton = param;
        _singleton->OpenFile(documentPath);

	}
}

void CMainThemeInit::SaveTheme()
{
	if (nullptr != _singleton)
	{
		_singleton->SaveFile();
	}
}
