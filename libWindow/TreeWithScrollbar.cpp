#include <header.h>
#include "TreeWithScrollbar.h"

using namespace Regards::Window;

CTreeWithScrollbar::CTreeWithScrollbar(const wxString &windowName, wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & theme)
	: CWindowMain(windowName, parent, id)
{
	scrollWindow = nullptr;
	treeWindow = nullptr;
	scrollWindow = new CScrollbarWnd(this, wxID_ANY);
	treeWindow = new CTreeWindow(scrollWindow, wxID_ANY, theme);
	scrollWindow->SetCentralWindow(treeWindow, themeScroll);
}
CTreeWithScrollbar::~CTreeWithScrollbar(void)
{
	if (treeWindow != nullptr)
		delete(treeWindow);

	if (scrollWindow != nullptr)
		delete(scrollWindow);
}

void CTreeWithScrollbar::UpdateScreenRatio()
{
	if (scrollWindow != nullptr)
		scrollWindow->UpdateScreenRatio();
}

void CTreeWithScrollbar::Resize()
{
	if (scrollWindow != nullptr)
		scrollWindow->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
}
