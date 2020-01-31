#include <header.h>
#include "TreeWithScrollbar.h"

using namespace Regards::Window;

CTreeWithScrollbar::CTreeWithScrollbar(const wxString &windowName, wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & theme)
	: CWindowMain(windowName, parent, id)
{
	scrollWindow = nullptr;
	treeWindow = nullptr;
	treeWindow = new CTreeWindow(this, wxID_ANY, theme);
	scrollWindow = new CScrollbarWnd(this, treeWindow, wxID_ANY);

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
