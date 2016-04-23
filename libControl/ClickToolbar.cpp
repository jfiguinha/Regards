#include "ClickToolbar.h"
using namespace Regards::Control;


CClickToolbar::CClickToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * interfaceToolbar, const int &idMessage)
	: CToolbarWindow(parent, id, theme)
{
	this->interfaceToolbar = interfaceToolbar;
	idObject = idMessage;

	CToolbarButton * screen = new CToolbarButton(themeToolbar.button);
    screen->ReplaceColor(wxColor(0,0,0), wxColor(0,0,0), wxColor(255,255,255));
	if (themeToolbar.isVertical)
	{
		screen->Resize(themeToolbar.button.GetRealTailleY(), themeToolbar.button.GetRealTailleX());
       
		screen->SetButtonResourceId("IDB_BUTTON_CLICK_VERTICAL");
	}
	else
	{
		screen->Resize(themeToolbar.button.GetRealTailleX(), themeToolbar.button.GetRealTailleY());
		screen->SetButtonResourceId("IDB_BUTTON_CLICK");
	}
	screen->SetCommandId(WM_CLICK);
	screen->SetLibelle("");

    screen->SetBorder(0);
	screen->IsPushActif(false);
	navElement.push_back(screen);
}

void CClickToolbar::SetVertical(const bool &vertical)
{
	themeToolbar.isVertical = vertical;
}

CClickToolbar::~CClickToolbar()
{
}

void CClickToolbar::EventManager(const int &id)
{
	switch (id)
	{
	case WM_CLICK:
		interfaceToolbar->ClickShowButton(idObject);
		break;
	}
}
