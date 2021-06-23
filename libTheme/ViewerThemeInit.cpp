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
