#include <header.h>
#include "ToolbarPhoto.h"
#include <LibResource.h>
using namespace Regards::Window;
using namespace Regards::Viewer;


CToolbarPhoto::CToolbarPhoto(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme,
                             CToolbarInterface* toolbarInterface, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	folder = nullptr;
	criteria = nullptr;
	this->toolbarInterface = toolbarInterface;
	saveLastPush = true;
	folder = CreateTexte(L"LBLFOLDER", WM_FOLDER);
	criteria = CreateTexte(L"LBLCRITERIA", WM_CRITERIA);
}

void CToolbarPhoto::SetFolderPush()
{
	folder->SetPush(true);
}

void CToolbarPhoto::SetCriteriaPush()
{
	criteria->SetPush(true);
}

void CToolbarPhoto::SetFaceListPush()
{
	facelist->SetPush(true);
}

void CToolbarPhoto::Resize()
{
	int nbElement = static_cast<int>(navElement.size());
	themeToolbar.texte.SetTailleX(GetWindowWidth() / nbElement);

	for (auto& nav : navElement)
	{
		nav->Resize(themeToolbar.texte.GetTailleX(), themeToolbar.texte.GetTailleY());
	}
	needToRefresh = true;
}

void CToolbarPhoto::EventManager(const int& id)
{
	if (toolbarInterface != nullptr)
	{
		toolbarInterface->ClickShowButton(id);
	}
}
