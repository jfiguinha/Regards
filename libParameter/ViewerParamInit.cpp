#include <header.h>
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <FileUtility.h>
#include <wx/filename.h>
using namespace Regards::Viewer;

std::unique_ptr<CMainParam> CMainParamInit::_singleton = nullptr;


CMainParam* CMainParamInit::getInstance()
{
	if (nullptr == _singleton)
	{
		Initialize();
		return _singleton.get();
	}
	return _singleton.get();
}

void CMainParamInit::Initialize()
{
	if (nullptr == _singleton)
	{
		_singleton = std::make_unique<CMainParam>();
		wxFileName configFile(CFileUtility::GetDocumentFolderPath(), "Regards.viewer.config");
		_singleton->OpenFile(configFile.GetFullPath());
	}
}
