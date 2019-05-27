#include <header.h>
#include "PanelPhotoWnd.h"
#include "ToolbarPhoto.h"
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#include <window_id.h>
#include <config_id.h>
#include "MainWindow.h"
#include <SqlFindFolderCatalog.h>
#include <SqlPhotoCriteria.h>
#include <SqlInsertFile.h>
using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Control;

CPanelPhotoWnd::CPanelPhotoWnd(wxWindow* parent, wxWindowID id)
	: CTabWindow("CPanelPhotoWnd",parent, id)
{
    categoryFolderWnd = nullptr;
	photoToolbar = nullptr;
    folderWnd = nullptr;
 

	//CRegardsConfigParam * config = CParamInit::getInstance();
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
        CThemeTree themeTree;
		folderWnd = new wxGenericDirCtrl(this, FOLDERWINDOWID, wxDirDialogDefaultFolderStr, wxDefaultPosition, wxDefaultSize, wxDIRCTRL_DIR_ONLY);
		folderWnd->GetTreeCtrl()->SetBackgroundColour(themeTree.bgColorOne);
		folderWnd->GetTreeCtrl()->SetForegroundColour(themeTree.bgColorBackground);
        folderWnd->Show(true);
        
        FolderCatalogVector folderList;
        CSqlFindFolderCatalog folderCatalog;
        folderCatalog.GetFolderCatalog(&folderList, NUMCATALOGID);

        for (CFolderCatalog folderCatalog : folderList)
        {
            folderWnd->SetPath(folderCatalog.GetFolderPath());
            wxTreeItemId treeitem = folderWnd->GetTreeCtrl()->GetSelection();
            folderWnd->GetTreeCtrl()->Check(treeitem);
        }

        
        CTabWindowData * tabInfosFile = new CTabWindowData();
        tabInfosFile->window = folderWnd;
        tabInfosFile->windowMain = nullptr;
        tabInfosFile->windowName = WM_FOLDER;
        listWindow.push_back(tabInfosFile);
	}

	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		CThemeTree theme;

		viewerTheme->GetScrollTheme(&themeScroll);
		viewerTheme->GetTreeTheme(&theme);

		categoryFolderWnd = new CCategoryFolderWindow(this, CATEGORYFOLDERWINDOWID, themeScroll, theme);
		categoryFolderWnd->Show(false);
        
        CTabWindowData * tabInfosFile = new CTabWindowData();
        tabInfosFile->window = categoryFolderWnd;
        tabInfosFile->windowMain = categoryFolderWnd;
        tabInfosFile->windowName = WM_CRITERIA;
        listWindow.push_back(tabInfosFile);
	}
    
	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetInfosToolbarTheme(&theme);
		photoToolbar = new CToolbarPhoto(this, wxID_ANY, theme, this, false);
	}

    toolbarWindow = photoToolbar;
    photoToolbar->SetFolderPush();
	windowVisible = WM_FOLDER;
    
	Connect(wxEVT_CHECKTREE_CHOICE, wxCommandEventHandler(CPanelPhotoWnd::OnSelChanged), NULL, this);
	Connect(wxEVENT_SETFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPanelPhotoWnd::SetFolder));
	Connect(wxEVENT_SAVEPARAMETER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPanelPhotoWnd::SaveParameter));
	Connect(wxEVENT_SELCHANGED, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPanelPhotoWnd::OnSelChanged));
	Connect(wxEVENT_UPDATECRITERIA, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPanelPhotoWnd::UpdateCriteria));
}

CPanelPhotoWnd::~CPanelPhotoWnd()
{
	delete(categoryFolderWnd);  
	delete(folderWnd);
    delete(photoToolbar);
}

void CPanelPhotoWnd::SaveParameter(wxCommandEvent& event)
{
	if (categoryFolderWnd != nullptr)
		categoryFolderWnd->InitSaveParameter();
}

void CPanelPhotoWnd::UpdateCriteria(wxCommandEvent& event)
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

void CPanelPhotoWnd::SetFolder(wxCommandEvent& folderEvent)
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

void CPanelPhotoWnd::OnSelChanged(wxCommandEvent& aEvent)
{
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

void CPanelPhotoWnd::RefreshData()
{
	switch (windowVisible)
	{
	case WM_FOLDER:
		{
			wxWindow* window = this->FindWindowById(MAINVIEWERWINDOWID);
			if (window != nullptr)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REFRESHFOLDER);
				window->GetEventHandler()->AddPendingEvent(evt);
			}
		}
		break;
	case WM_CRITERIA:
		{
			wxWindow* window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
			if (window != nullptr)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_UPDATECRITERIA);
				evt.SetExtraLong(3);
				window->GetEventHandler()->AddPendingEvent(evt);
			}
		}
		break;
	}
}

void CPanelPhotoWnd::LoadInfo()
{
	if (this->IsShown())
	{
		switch (windowVisible)
		{
		case WM_FOLDER:
            photoToolbar->SetFolderPush();
			break;
        case WM_CRITERIA:
            photoToolbar->SetCriteriaPush();
            break;
		}
	}
}
