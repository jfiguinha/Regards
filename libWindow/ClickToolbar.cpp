#include <header.h>
#include "ClickToolbar.h"
#include "ToolbarInterface.h"
using namespace Regards::Window;


CClickToolbar::CClickToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme,
                             CToolbarInterface* interfaceToolbar, const int& idMessage, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	screen = nullptr;
	this->interfaceToolbar = interfaceToolbar;
	idObject = idMessage;
	isVertical = vertical;

	screen = new CToolbarButton(themeToolbar.button);
	screen->ReplaceColor(wxColor(0, 0, 0), wxColor(0, 0, 0), themeToolbar.replaceColor);
	if (vertical)
	{
		screen->Resize(themeToolbar.button.GetRealTailleY(), themeToolbar.button.GetRealTailleX());
		screen->SetButtonResourceId("IDB_BUTTON_CLICK_VERTICAL");
	}
	else
	{
		screen->Resize(themeToolbar.button.GetRealTailleX(), themeToolbar.button.GetRealTailleY());
		screen->SetButtonResourceId("IDB_BUTTON_CLICK");
	}
	screen->SetCommandId(wxEVENT_WM_CLICK);
	screen->SetLibelle("");
	screen->SetBorder(0);
	screen->IsPushActif(false);
	navElement.push_back(screen);
}

void CClickToolbar::SetVertical(const bool& vertical)
{
	isVertical = vertical;
	if (isVertical)
	{
		screen->Resize(themeToolbar.button.GetRealTailleY(), themeToolbar.button.GetRealTailleX());
		screen->SetButtonResourceId("IDB_BUTTON_CLICK_VERTICAL");
		//width = themeToolbar.GetWidth();
	}
	else
	{
		screen->Resize(themeToolbar.button.GetRealTailleX(), themeToolbar.button.GetRealTailleY());
		screen->SetButtonResourceId("IDB_BUTTON_CLICK");
		//height = themeToolbar.GetHeight();
	}


	needToRefresh = true;
}

CClickToolbar::~CClickToolbar()
{
}

int CClickToolbar::GetWidth()
{
	if (isVertical)
		return themeToolbar.button.GetRealTailleY();

	return themeToolbar.button.GetRealTailleX();
}

int CClickToolbar::GetHeight()
{
	if (isVertical)
		return themeToolbar.button.GetRealTailleX();

	return themeToolbar.button.GetRealTailleY();
}

void CClickToolbar::EventManager(const int& id)
{
	switch (id)
	{
	case wxEVENT_WM_CLICK:
		interfaceToolbar->ClickShowButton(idObject);
		break;
	default: ;
	}
}
