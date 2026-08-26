#include <header.h>
#include "TreeWithScrollbar.h"


using namespace Regards::Window;

CTreeWithScrollbar::CTreeWithScrollbar(const wxString& windowName, wxWindow* parent, wxWindowID id,
                                       const CThemeScrollBar& themeScroll, const CThemeTree& theme,
                                       const wxString& label, const bool& showTitle)
	: CWindowMain(windowName, parent, id)
{
	scrollWindow = nullptr;
	treeWindow = nullptr;
	this->showTitle = showTitle;
	treeWindow = new CTreeWindow(this, wxID_ANY, theme);
	if (showTitle)
	{
		titleBar = new CTitleBar(this, wxID_ANY, (CTitleBarInterface*)this);
		titleBar->SetTitle(label);
		titleBar->SetClosable(false);
		titleBar->SetRefresh(false);
		titleBar->SetCenter(true);
	}
	else
		titleBar = nullptr;
	scrollWindow = new CScrollbarWnd(this, treeWindow, wxID_ANY);
}

void CTreeWithScrollbar::SetLabel(const wxString& label)
{
	if (titleBar != nullptr)
		titleBar->SetTitle(label);
}

void CTreeWithScrollbar::UpdateScreenRatio()
{
	if (scrollWindow != nullptr)
		scrollWindow->UpdateScreenRatio();
}

void CTreeWithScrollbar::Resize()
{
	if (titleBar == nullptr)
	{
		scrollWindow->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
		return;
	}

	if (showTitle && !titleBar->IsShown())
		titleBar->Show(true);
	else if (!showTitle && titleBar->IsShown())
		titleBar->Show(false);


	if (scrollWindow != nullptr)
	{
		if (!showTitle)
		{		
			scrollWindow->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
		}
		else
		{
			titleBar->SetSize(0, 0, GetWindowWidth(), titleBar->GetHeight());
			scrollWindow->SetSize(0, titleBar->GetHeight(), GetWindowWidth(),
			                      GetWindowHeight() - titleBar->GetHeight());
		}
	}
}
