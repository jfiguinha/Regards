#include <header.h>
#include "KeywordManagementWindow.h"
#include "KeywordWnd.h"
#include "TreeWindow.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Viewer;


CKeywordManagementWindow::CKeywordManagementWindow(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll,
                                                   const CThemeTree& theme)
	: CTreeWithScrollbar("CKeywordManagementWindow", parent, id, themeScroll, theme)
{
	keywordWndOld = nullptr;
	processIdle = false;
	Connect(wxEVENT_REFRESHDATA, wxCommandEventHandler(CKeywordManagementWindow::UpdateKeyword));
}

CKeywordManagementWindow::~CKeywordManagementWindow()
{
}

void CKeywordManagementWindow::UpdateKeyword(wxCommandEvent& event)
{
	Init(filename);
}

void CKeywordManagementWindow::Init(const wxString& filename)
{
	this->filename = filename;
	auto keyword = new CKeywordWnd(this, treeWindow->GetTheme(), treeWindow);
	keyword->Init(filename);
	treeWindow->SetTreeControl(keyword);
	delete(keywordWndOld);
	keywordWndOld = keyword;
}
