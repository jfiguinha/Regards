#include "header.h"
#include "ScannerThemeInit.h"
#include "ScannerTheme.h"
#include <FileUtility.h>
#include <ParamInit.h>
#include <wx/filename.h>
#include <RegardsConfigParam.h>
using namespace Regards::Scanner;

std::unique_ptr<CMainTheme> CMainThemeInit::_singleton = nullptr;
wxString CMainThemeInit::documentPath = "";

CMainTheme* CMainThemeInit::getInstance()
{
	if (nullptr == _singleton)
		Initialize();

    if (_singleton == nullptr)
        return nullptr;

    return _singleton.get();
}

void CMainThemeInit::Initialize()
{
    if (_singleton != nullptr)
        return;

    _singleton = std::make_unique<CMainTheme>();

    int skinMode = 0;

    if (auto* regardsParam = CParamInit::getInstance())
        skinMode = regardsParam->GetSkinWindowMode();

    const bool isDarkTheme =
        wxSystemSettings::GetAppearance().IsDark();

    wxString themeFile;
    wxString resourceFile;

    if (skinMode == 0)
    {
        themeFile = isDarkTheme
            ? "Regards.dark.theme"
            : "Regards.light.theme";

        resourceFile = isDarkTheme
            ? "Regards.viewer.dark.theme"
            : "Regards.viewer.light.theme";
    }
    else if (skinMode == 1)
    {
        themeFile = "Regards.light.theme";
        resourceFile = "Regards.viewer.light.theme";
    }
    else
    {
        themeFile = "Regards.dark.theme";
        resourceFile = "Regards.viewer.dark.theme";
    }

    wxFileName userTheme(
        CFileUtility::GetDocumentFolderPath(),
        themeFile);

    wxFileName resourceTheme(
        CFileUtility::GetResourcesFolderPath() +
        wxFILE_SEP_PATH +
        "theme",
        resourceFile);

    documentPath = userTheme.GetFullPath();

    if (!wxFileExists(documentPath))
    {
        wxCopyFile(
            resourceTheme.GetFullPath(),
            documentPath);
    }
    _singleton->OpenFile(documentPath);
}

void CMainThemeInit::SaveTheme()
{
	if (nullptr != _singleton)
	{
        if (!wxFileExists(documentPath))
		    _singleton->SaveFile();
	}
}
