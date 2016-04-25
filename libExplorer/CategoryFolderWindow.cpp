#include "CategoryFolderWindow.h"
#include <LibResource.h>
#include "ExplorerTheme.h"
#include "ExplorerParamInit.h"
#include "ExplorerThemeInit.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Explorer;

#define FOLDER_PANE 1
#define CATALOG_PANE 2
#define CLICK_FOLDER 3
wxDEFINE_EVENT(EVENT_CRITERIAUPDATE, wxCommandEvent);

CCategoryFolderWindow::CCategoryFolderWindow(wxWindow* parent, wxWindowID id)
	: CWindowMain(parent, id)
{

	catalogWndOld = nullptr;
	paneCatalog = nullptr;//new CPane(this, CATALOG_PANE);
	treeWindow = nullptr;//new CTreeWindow();

	CExplorerTheme * explorerTheme = CExplorerThemeInit::getInstance();

	if (explorerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLCRITERIA", 1);
		CThemePane themePane;
		CThemeScrollBar themeScroll;
		CThemeTree themeTree;

		explorerTheme->GetPaneCategory(themePane);
		paneCatalog = new CPane(this, wxID_ANY, this, CATALOG_PANE, themePane);
		paneCatalog->SetTitle(libelle);
		paneCatalog->SetClosable(false);
		
		explorerTheme->GetCategoryScrollTheme(themeScroll);
		catalogWndScroll = new CScrollbarWnd(paneCatalog, wxID_ANY);
		
		explorerTheme->GetCategoryTreeTheme(themeTree);
		treeWindow = new CTreeWindow(catalogWndScroll, CATEGORYWINDOWID, themeTree);
		catalogWndScroll->SetCentralWindow(treeWindow, themeScroll);

		paneCatalog->SetOtherWindow(catalogWndScroll);
	}
    
    Connect(EVENT_CRITERIAUPDATE, wxCommandEventHandler(CCategoryFolderWindow::RefreshCriteriaSearch));

	Init();
}

void CCategoryFolderWindow::RefreshCriteriaSearch(wxCommandEvent& event)
{
    if(catalogWndOld != nullptr)
        catalogWndOld->RefreshCriteriaSearch();
}

void CCategoryFolderWindow::UpdatePhotoList()
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
	{
		catalogWndOld->RefreshListPhoto();
	}
}

void CCategoryFolderWindow::UpdateCriteria()
{
	//if (catalogWndOld != nullptr)
	//	catalogWndOld->SaveState();

	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
	{
		CCategoryWnd * catalogWnd = new CCategoryWnd(listPicture, treeWindow->GetTheme(), treeWindow);
		catalogWnd->Init();
		treeWindow->SetTreeControl(catalogWnd);
		delete(catalogWndOld);
		catalogWndOld = catalogWnd;
		wxCommandEvent * event = new wxCommandEvent(EVENT_CRITERIAUPDATE);
		wxQueueEvent(this, event);
	}
}

void CCategoryFolderWindow::UpdateScreenRatio()
{
    paneCatalog->UpdateScreenRatio();
    catalogWndScroll->UpdateScreenRatio();
    catalogWndOld->UpdateScreenRatio();
    treeWindow->UpdateScreenRatio();
}


CCategoryFolderWindow::~CCategoryFolderWindow()
{
	delete(treeWindow);
	delete(catalogWndScroll);
	delete(paneCatalog);

	//if (catalogWndOld != nullptr)
	//	catalogWndOld->SaveState();

	delete(catalogWndOld);
}

void CCategoryFolderWindow::Init()
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr && treeWindow != nullptr)
	{
		CCategoryWnd * catalogWnd = new CCategoryWnd(listPicture, treeWindow->GetTheme(), treeWindow);
		catalogWnd->Init();
		treeWindow->SetTreeControl(catalogWnd);
		delete(catalogWndOld);
		catalogWndOld = catalogWnd;
	}
}

void CCategoryFolderWindow::Resize()
{
	paneCatalog->SetSize(width, height);
}