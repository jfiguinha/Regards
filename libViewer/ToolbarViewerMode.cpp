#include <header.h>
#include "ToolbarViewerMode.h"
#include "window_mode_id.h"
#include <LibResource.h>
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
	wxString facemode_label = CLibResource::LoadStringFromResource(L"LBLFACELIST", 1); // L"Folder";
	wxString viewermode_label = CLibResource::LoadStringFromResource(L"LBLVIEWERMODE", 1); //L"Criteria";
	wxString explorermode_label = CLibResource::LoadStringFromResource(L"LBLEXPLORERMODE", 1); //L"Criteria";
	wxString picturemode_label = CLibResource::LoadStringFromResource(L"LBLPICTUREMODE", 1); //L"Criteria";

	int height = this->themeToolbar.GetHeight();
	int faceDetection = 1;

	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		faceDetection = regardsParam->GetFaceDetection();
	}

	if (faceDetection)
	{
		facemode = new CToolbarTexte(themeToolbar.texte);
		facemode->SetCommandId(WINDOW_FACE);
		facemode->SetLibelle(facemode_label);
		navElement.push_back(facemode);
	}

	explorermode = new CToolbarTexte(themeToolbar.texte);
	explorermode->SetCommandId(WINDOW_EXPLORER);
	explorermode->SetLibelle(explorermode_label);
	navElement.push_back(explorermode);

	viewermode = new CToolbarTexte(themeToolbar.texte);
	viewermode->SetCommandId(WINDOW_VIEWER);
	viewermode->SetLibelle(viewermode_label);
	navElement.push_back(viewermode);

	picturemode = new CToolbarTexte(themeToolbar.texte);
	picturemode->SetCommandId(WINDOW_PICTURE);
	picturemode->SetLibelle(picturemode_label);
	navElement.push_back(picturemode);

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

CToolbarViewerMode::~CToolbarViewerMode()
{
}

void CToolbarViewerMode::SetFaceWindowPush()
{
	facemode->SetPush(true);
}

void CToolbarViewerMode::SetViewerWindowPush()
{
	viewermode->SetPush(true);
}

void CToolbarViewerMode::SetExplorerWindowPush()
{
	explorermode->SetPush(true);
}

void CToolbarViewerMode::SetPictureWindowPush()
{
	picturemode->SetPush(true);
}

void CToolbarViewerMode::EventManager(const int& id)
{
	if (toolbarInterface != nullptr)
	{
		toolbarInterface->ClickShowButton(id);
	}
}
