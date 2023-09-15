#include "header.h"
#include "ThemeInit.h"
#include "ThemeParam.h"
#include <FileUtility.h>
#include <wx/stdpaths.h>
#include <wx/settings.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include "ViewerParamInit.h"
#include "ViewerParam.h"
using namespace Regards::Viewer;
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
        int skinMode = 0;
        CRegardsConfigParam* regardsParam = CParamInit::getInstance();
        if (regardsParam != nullptr)
            skinMode = regardsParam->GetSkinWindowMode();
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
        if (skinMode == 0)
        {
            if (isDarkTheme)
                filepath.append("theme.dark");
            else
                filepath.append("theme.light");
        }
        else if (skinMode == 1)
        {
            filepath.append("theme.light");
        }
        else
        {
            filepath.append("theme.dark");
        }
		_singleton = param;
		_singleton->OpenFile(filepath);
#else

        wxStandardPathsBase& stdp = wxStandardPaths::Get();
        wxString documentPath = stdp.GetDocumentsDir();
        if (skinMode == 0)
        {
            if (isDarkTheme)
                documentPath.append("/Regards.theme.dark");
            else
                documentPath.append("/Regards.theme.light");
        }
        else if (skinMode == 1)
        {
            documentPath.append("/Regards.theme.light");
        }
        else
        {
            documentPath.append("/Regards.theme.dark");
        }


        _singleton = param;
        _singleton->OpenFile(documentPath);
#endif
	}
}
