#include <header.h>
#include "KeywordWndToolbar.h"
#include "KeywordManagementWindow.h"
#include "ToolbarKeyword.h"
#include "MainTheme.h"
#include "ViewerThemeInit.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Viewer;


CKeywordWndToolbar::CKeywordWndToolbar(wxWindow* parent, wxWindowID id)
	: CWindowMain("CKeywordManagementWindow", parent, id)
{
	CMainTheme* viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		CThemeTree theme;

		viewerTheme->GetScrollTheme(&themeScroll);
		viewerTheme->GetTreeTheme(&theme);

		keywordWnd = new CKeywordManagementWindow(this, KEYWORDCRITERIAWINDOWID, themeScroll, theme);
		keywordWnd->Show(true);
	}

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetThumbnailToolbarTheme(theme);
		toolbarKeyword = new CToolbarKeyword(this, wxID_ANY, theme, false);
	}
}

CKeywordWndToolbar::~CKeywordWndToolbar()
{
}

void CKeywordWndToolbar::Init(const wxString& filename)
{
	this->filename = filename;
	if (keywordWnd != nullptr)
		keywordWnd->Init(filename);
}

void CKeywordWndToolbar::UpdateScreenRatio()
{
	TRACE();
	toolbarKeyword->UpdateScreenRatio();
	keywordWnd->UpdateScreenRatio();
	this->Resize();
}

void CKeywordWndToolbar::Resize()
{
	TRACE();

	wxRect rcAffichageBitmap;
	rcAffichageBitmap.x = 0;
	rcAffichageBitmap.y = toolbarKeyword->GetNavigatorHeight();
	rcAffichageBitmap.width = GetWindowWidth();
	rcAffichageBitmap.height = GetWindowHeight() - toolbarKeyword->GetNavigatorHeight();

	keywordWnd->SetSize(0, 0, rcAffichageBitmap.width, rcAffichageBitmap.height);
	keywordWnd->Refresh();

	toolbarKeyword->SetSize(0, rcAffichageBitmap.height, rcAffichageBitmap.width, toolbarKeyword->GetNavigatorHeight());
	toolbarKeyword->Refresh();
}
