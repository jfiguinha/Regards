#include "CriteriaFolderSplitter.h"
#include <ConfigParam.h>
#include "ViewerParamInit.h"
#include <LibResource.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <Gps.h>

#include <SqlFindFolderCatalog.h>
#include <SqlPhotoCriteria.h>
#include <SqlInsertFile.h>
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Sqlite;


CCriteriaFolderSplitter::CCriteriaFolderSplitter(wxWindow* parent, wxWindowID id, 
	IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, const CThemeToolbar & themeInfosToolbar, const bool &horizontal)
	: CSplitterWithPanel(parent, id, theme, themeInfosToolbar, horizontal)
{

	posBarInfos = 0;
	categoryFolderWnd = nullptr;
	viewerconfig = nullptr;
	fullscreen = false;
	lastWindow = ALL_WINDOW;

	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

	//Criteria
	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLCRITERIA", 1);
		CThemePane themePane;
		CThemeScrollBar themeScroll;
		

		viewerTheme->GetPaneCategory(themePane);
		paneWindow2 = new CPane(this, wxID_ANY, this, PANEL2_FILTER, themePane);
		paneWindow2->SetTitle(libelle);
		paneWindow2->SetClosable(true);

		categoryFolderWnd = new CCategoryFolderWindow(paneWindow2, CATEGORYFOLDERWINDOWID, statusBarInterface);
		paneWindow2->SetOtherWindow(categoryFolderWnd);
	}

	//Folder
	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLFOLDER", 1);
		CThemePane themePane;
		CThemeScrollBar themeScroll;
		CThemeTree themeTree;

		viewerTheme->GetPaneCategory(themePane);
		paneWindow1 = new CPane(this, wxID_ANY, this, PANEL1_FILTER, themePane);
		paneWindow1->SetTitle(libelle);
		paneWindow1->SetClosable(true);

		folderWnd = new wxGenericDirCtrl(paneWindow1, FOLDERWINDOWID, wxDirDialogDefaultFolderStr, wxDefaultPosition, wxDefaultSize, wxDIRCTRL_DIR_ONLY);
		folderWnd->GetTreeCtrl()->SetBackgroundColour(themeTree.bgColorOne);
		folderWnd->GetTreeCtrl()->SetForegroundColour(themeTree.bgColorBackground);
		paneWindow1->SetOtherWindow(folderWnd);
	}


	this->SetWindow(paneWindow1, paneWindow2);


	bool showFilter = true;
	bool showFolder = true;
	CConfigParam * configParam = CViewerParamInit::getInstance();
	if (configParam != nullptr)
	{
		viewerconfig = (CViewerParam *)configParam;
		this->posBar = viewerconfig->GetPositionCriteriaFolder();
		viewerconfig->GetShowFilter(showFilter);
		viewerconfig->GetShowFolder(showFolder);
	}

	FolderCatalogVector folderList;
	CSqlFindFolderCatalog folderCatalog;
	folderCatalog.GetFolderCatalog(&folderList, NUMCATALOGID);

	for (CFolderCatalog folderCatalog : folderList)
	{
		folderWnd->SetPath(folderCatalog.GetFolderPath());
		wxTreeItemId treeitem = folderWnd->GetTreeCtrl()->GetSelection();
		folderWnd->GetTreeCtrl()->Check(treeitem);
	}

	Connect(wxEVT_CHECKTREE_CHOICE, wxCommandEventHandler(CCriteriaFolderSplitter::OnSelChanged), NULL, this);
	Connect(wxEVENT_SETFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CCriteriaFolderSplitter::SetFolder));
	Connect(wxEVENT_SAVEPARAMETER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CCriteriaFolderSplitter::SaveParameter));
	Connect(wxEVENT_PANELCRITERIA, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CCriteriaFolderSplitter::ShowPanelCriteria));
	Connect(wxEVENT_PANELFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CCriteriaFolderSplitter::ShowPanelFolder));
	Connect(wxEVENT_SELCHANGED, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CCriteriaFolderSplitter::OnSelChanged));
	Connect(wxEVENT_UPDATECRITERIA, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CCriteriaFolderSplitter::UpdateCriteria));
	/*
	if (categoryFolderWnd != nullptr)
	{
		categoryFolderWnd->UpdateCriteria(false);
	}
	*/
    if(!showFolder)
        ClosePane(PANEL1_FILTER);
    if(!showFilter)
        ClosePane(PANEL2_FILTER);
}


void CCriteriaFolderSplitter::HidePanel()
{
    ClosePane(PANEL1_FILTER);
    ClosePane(PANEL2_FILTER);
}


void CCriteriaFolderSplitter::ScreenMode()
{
	this->Resize(this);
}

void CCriteriaFolderSplitter::OnSelChanged(wxCommandEvent& aEvent)
{
	printf("OnSelChanged \n");

	CWindowMain * windowMain = (CWindowMain *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (windowMain != nullptr)
	{
        
        
 		wxString getSelectPath = folderWnd->GetPath();
        

        
		int isChecked = aEvent.GetExtraLong();
		wxString * path = new wxString();
		*path = getSelectPath;
		if (isChecked)
		{
			folderWnd->AddPath(getSelectPath);
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ADDFOLDER);
			evt.SetClientData(path);
			windowMain->GetEventHandler()->AddPendingEvent(evt);
		}
		else
		{
			folderWnd->RemovePath(getSelectPath);
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REMOVEFOLDER);
			evt.SetClientData(path);
			windowMain->GetEventHandler()->AddPendingEvent(evt);
		}
	}
}

CCriteriaFolderSplitter::~CCriteriaFolderSplitter()
{
	if (viewerconfig != nullptr)
	{
		viewerconfig->SetPositionCriteriaFolder(this->posBar);
		viewerconfig->SetShowFilter(this->paneWindow2->IsShown());
		viewerconfig->SetShowFolder(this->paneWindow1->IsShown());
	}

	if (folderWnd != nullptr)
		delete(folderWnd);

	if (categoryFolderWnd != nullptr)
		delete(categoryFolderWnd);
}


void CCriteriaFolderSplitter::SetFolder(wxCommandEvent& folderEvent)
{
	if (folderEvent.GetClientData() != nullptr)
	{
		wxString * folder = (wxString *)folderEvent.GetClientData();
		folderWnd->SetPath(*folder);
		wxTreeItemId treeitem = folderWnd->GetTreeCtrl()->GetSelection();
		folderWnd->GetTreeCtrl()->Check(treeitem);
		delete folder;
	}
}

wxString CCriteriaFolderSplitter::GetSqlRequest()
{
	if (categoryFolderWnd != nullptr)
		return categoryFolderWnd->GetSqlRequest();
	return "";
}

void CCriteriaFolderSplitter::ShowPanelCriteria(wxCommandEvent& event)
{
	ClickShowButton(PANEL2_FILTER);
}

void CCriteriaFolderSplitter::ShowPanelFolder(wxCommandEvent& event)
{
	ClickShowButton(PANEL1_FILTER);
}

void CCriteriaFolderSplitter::SaveParameter(wxCommandEvent& event)
{
	if (categoryFolderWnd != nullptr)
		categoryFolderWnd->InitSaveParameter();
}

void CCriteriaFolderSplitter::UpdateCriteria(wxCommandEvent& event)
{
	if (categoryFolderWnd != nullptr)
	{
		long needToSendMessage = event.GetExtraLong();
		if(needToSendMessage < 2)
		{
			categoryFolderWnd->UpdateCriteria((needToSendMessage == 1) ? true : false);
		}
		else if(needToSendMessage == 2)
		{
			categoryFolderWnd->RefreshCriteriaSearch();
		}
		else if (needToSendMessage == 3)
		{
			//Suppression de l'ensemble des données
			CSqlPhotoCriteria sqlPhotoCriteria;
			sqlPhotoCriteria.DeletePhotoCriteria();
			CSqlInsertFile insertFile;
			insertFile.ReinitPhotosToProcess();


			//categoryFolderWnd->UpdateCriteria(true);
			categoryFolderWnd->RefreshCriteriaSearch();
		}
	}
}

