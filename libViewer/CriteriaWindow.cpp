#include <header.h>
#include "CriteriaWindow.h"
#include "KeywordWndToolbar.h"
#include <CriteriaTreeWnd.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <window_id.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Control;


CCriteriaWindow::CCriteriaWindow(wxWindow* parent, wxWindowID id,
	const CThemeSplitter & theme, const bool &horizontal)
	: CSplitter(parent, id, theme)
{
	//CRegardsConfigParam * config = CParamInit::getInstance();
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();
    
    if (viewerTheme != nullptr)
    {
        CThemeScrollBar themeScroll;
        viewerTheme->GetScrollTheme(&themeScroll);
        
        CThemeTree themeTree;
        viewerTheme->GetTreeTheme(&themeTree);
        criteriaTreeWnd = new CCriteriaTreeWnd(this, wxID_ANY, MAINVIEWERWINDOWID, themeTree, themeScroll);
    }
    
	if (viewerTheme != nullptr)
	{
		keywordWnd = new CKeywordWndToolbar(this, KEYWORDTOOLBARWINDOWID);
		keywordWnd->Show(true);
	}
    
	SetHorizontal(horizontal);
	this->SetWindow1FixPosition(true, 160);
	this->SetWindow(criteriaTreeWnd, keywordWnd);

}


void CCriteriaWindow::SetFile(const wxString &filename)
{
    if(criteriaTreeWnd != nullptr)
        criteriaTreeWnd->SetFile(filename);
        
    this->filename = filename;
	if (keywordWnd != nullptr)
		keywordWnd->Init(filename);
    
    
}

CCriteriaWindow::~CCriteriaWindow()
{
 	if (criteriaTreeWnd != nullptr)
		delete(criteriaTreeWnd);

    if(keywordWnd != nullptr)
        delete(keywordWnd);
}


void CCriteriaWindow::UpdateScreenRatio()
{
 	if (criteriaTreeWnd != nullptr)
		criteriaTreeWnd->UpdateScreenRatio();
	if (keywordWnd != nullptr)
		keywordWnd->UpdateScreenRatio();
}