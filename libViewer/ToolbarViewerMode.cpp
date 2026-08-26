#include <header.h>
#include "ToolbarViewerMode.h"
#include "window_mode_id.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include "ViewerParam.h"
#include "ViewerParamInit.h"
using namespace Regards::Window;
using namespace Regards::Viewer;


CToolbarViewerMode::CToolbarViewerMode(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme,
	CToolbarInterface* toolbarInterface, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	CMainParam* config = CMainParamInit::getInstance();
	int windowMode = 1;

	if (config != nullptr)
	{
		windowMode = config->GetViewerMode();
	}

	showLine = true;

	this->toolbarInterface = toolbarInterface;
	saveLastPush = true;   
	int height = this->themeToolbar.GetHeight();
	int faceDetection = 1;

	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		faceDetection = regardsParam->GetFaceDetection();
	}

	if (faceDetection)
	{
		facemode = CreateTexte(L"LBLFACELIST", WINDOW_FACE);
	}

	explorermode = CreateTexte(L"LBLEXPLORERMODE", WINDOW_EXPLORER);
	viewermode = CreateTexte(L"LBLVIEWERMODE", WINDOW_VIEWER);
	picturemode = CreateTexte(L"LBLPICTUREMODE", WINDOW_PICTURE);
    imageNewVersion = CreateTexte(L"LBLUPDATE", IDM_NEWVERSION);
    
    if (!faceDetection && windowMode == WINDOW_FACE)
    {
        windowMode = WINDOW_VIEWER;
    }

	switch (windowMode)
	{
	case WINDOW_FACE:
        facemode->SetPush(true);
        break;
	case WINDOW_VIEWER:
		viewermode->SetPush(true);
		break;
	case WINDOW_EXPLORER:
		explorermode->SetPush(true);
		break;
	case WINDOW_PICTURE:
		picturemode->SetPush(true);
		break;
	}
}

void CToolbarViewerMode::SetUpdateVisible(const bool& isVisible)
{
	imageNewVersion->SetVisible(isVisible);
	this->Refresh();
}

void CToolbarViewerMode::DisableAll()
{
	if(facemode != nullptr)
		facemode->SetPush(false);
	viewermode->SetPush(false);
	explorermode->SetPush(false);
	picturemode->SetPush(false);
	imageNewVersion->SetPush(false);
}

void CToolbarViewerMode::SetFaceWindowPush()
{
	DisableAll();
	facemode->SetPush(true);
}

void CToolbarViewerMode::SetViewerWindowPush()
{
	DisableAll();
	viewermode->SetPush(true);
}

void CToolbarViewerMode::SetExplorerWindowPush()
{
	DisableAll();
	explorermode->SetPush(true);
}

void CToolbarViewerMode::SetPictureWindowPush()
{
	DisableAll();
	picturemode->SetPush(true);
}

void CToolbarViewerMode::EventManager(const int& id)
{
	if (toolbarInterface != nullptr)
	{
		toolbarInterface->ClickShowButton(id);
	}
}
