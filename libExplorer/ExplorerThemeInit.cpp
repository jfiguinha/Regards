#include "ExplorerThemeInit.h"
#include "ExplorerTheme.h"
#include <wx/stdpaths.h>
using namespace Regards::Explorer;

CExplorerTheme * CExplorerThemeInit::_singleton = nullptr;


CExplorerThemeInit::CExplorerThemeInit()
{
}


CExplorerThemeInit::~CExplorerThemeInit()
{
}


CExplorerTheme * CExplorerThemeInit::getInstance()
{
	return _singleton;
}

void CExplorerThemeInit::Initialize(CExplorerTheme * param)
{
	if (nullptr == _singleton)
	{
#ifdef WIN32
        wxString filepath = wxStandardPaths::Get().GetExecutablePath();
        //filepath = filepath.SubString(0, filepath.size() - 3);
        filepath.append(".viewer.theme");
        _singleton = param;
        _singleton->OpenFile(filepath);
        
#else
        wxStandardPathsBase& stdp = wxStandardPaths::Get();
        wxString documentPath = stdp.GetDocumentsDir();
        documentPath.append("/RegardsExplorer.theme");
        _singleton = param;
        _singleton->OpenFile(documentPath);
#endif
        
        /*
		wxString filename = wxStandardPaths::Get().GetExecutablePath();
		filename.append(L".viewer.theme");
		_singleton = param;
		_singleton->OpenFile(filename);
        */

	}
}
