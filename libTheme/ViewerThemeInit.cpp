#include <header.h>
#include "ViewerThemeInit.h"
#include "ViewerTheme.h"
#include <wx/stdpaths.h>
using namespace Regards::Viewer;

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
#ifdef WIN32

        if(isDarkTheme)
            documentPath.append("\\Regards\\Regards.viewer.dark.theme");
        else
            documentPath.append("\\Regards\\Regards.viewer.light.theme");
		_singleton = param;
		_singleton->OpenFile(documentPath);
#else

        if(isDarkTheme)
            documentPath.append("/Regards/Regards.viewer.dark.theme");
        else
            documentPath.append("/Regards/Regards.viewer.light.theme");
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
