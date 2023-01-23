#include <header.h>
#include "ToolbarViewerMode.h"
#include "window_mode_id.h"
#include <LibResource.h>
using namespace Regards::Window;
using namespace Regards::Viewer;


CToolbarViewerMode::CToolbarViewerMode(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme,
	CToolbarInterface* toolbarInterface, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{

	/*
	 *		CToolbarTexte* facemode = nullptr;
		CToolbarTexte* viewermode = nullptr;
		CToolbarTexte* explorermode = nullptr;
		CToolbarTexte* picturemode = nullptr;
	 *
	 */

	this->toolbarInterface = toolbarInterface;
	saveLastPush = true;
	wxString facemode_label = CLibResource::LoadStringFromResource(L"labelWindowFace", 1); // L"Folder";
	wxString viewermode_label = CLibResource::LoadStringFromResource(L"labelWindowViewer", 1); //L"Criteria";
	wxString explorermode_label = CLibResource::LoadStringFromResource(L"labelWindowFolder", 1); //L"Criteria";
	wxString picturemode_label = CLibResource::LoadStringFromResource(L"labelWindowPicture", 1); //L"Criteria";

	facemode = new CToolbarTexte(themeToolbar.texte);
	facemode->SetCommandId(WINDOW_FACE);
	facemode->SetLibelle(facemode_label);
	navElement.push_back(facemode);

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

void CToolbarViewerMode::Resize()
{
	int nbElement = static_cast<int>(navElement.size());
	themeToolbar.texte.SetTailleX(GetWindowWidth() / nbElement);

	for (CToolbarElement* nav : navElement)
	{
		nav->Resize(themeToolbar.texte.GetTailleX(), themeToolbar.texte.GetTailleY());
	}
	needToRefresh = true;
}

void CToolbarViewerMode::EventManager(const int& id)
{
	if (toolbarInterface != nullptr)
	{
		toolbarInterface->ClickShowButton(id);
	}
}
