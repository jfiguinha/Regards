#include <header.h>
#include "PanelPhotoWnd.h"
#include "ToolbarPhoto.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <window_id.h>
#include "MainWindow.h"
#include <SqlFindFolderCatalog.h>
#include <SqlPhotoCriteria.h>
#include <SqlInsertFile.h>
#include <LibResource.h>
#include <directoryctrl.h>
#include <wx/dir.h>
#include <wx/busyinfo.h>
#include "ConvertUtility.h"
#include "SqlFolderCatalog.h"
#include "SQLRemoveData.h"
#include <Tracing.h>
namespace Regards
{
	namespace Viewer
	{
		class CListFace;
	}
}

using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Viewer;
//using namespace Regards::Control;

CPanelPhotoWnd::CPanelPhotoWnd(wxWindow* parent, wxWindowID id)
	: CTabWindow("CPanelPhotoWnd", parent, id)
{
	categoryFolderWnd = nullptr;
	photoToolbar = nullptr;
	folderWnd = nullptr;


	//CRegardsConfigParam * config = CParamInit::getInstance();
	CMainTheme* viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeTree themeTree;
		folderWnd = new wxGenericDirCtrl(this, FOLDERWINDOWID, wxDirDialogDefaultFolderStr, wxDefaultPosition,
		                                 wxDefaultSize, wxDIRCTRL_DIR_ONLY);
		if (folderWnd->GetTreeCtrl() != nullptr)
		{
			folderWnd->GetTreeCtrl()->SetBackgroundColour(themeTree.bgColorOne);
			folderWnd->GetTreeCtrl()->SetForegroundColour(themeTree.bgColorBackground);
		}
		folderWnd->Show(true);

		FolderCatalogVector folderList;
		CSqlFindFolderCatalog folderCatalog;
		folderCatalog.GetFolderCatalog(&folderList, NUMCATALOGID);

		for (CFolderCatalog folder_catalog : folderList)
		{
			folderWnd->SetPath(folder_catalog.GetFolderPath());
			if (folderWnd->GetTreeCtrl() != nullptr)
			{
				wxTreeItemId treeitem;
				treeitem = folderWnd->GetTreeCtrl()->GetSelection();
				folderWnd->GetTreeCtrl()->Check(treeitem);
			}
		}


		auto tabInfosFile = new CTabWindowData();
		tabInfosFile->SetWindow(folderWnd);
		tabInfosFile->SetId(WM_FOLDER);
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

		auto tabInfosFile = new CTabWindowData();
		tabInfosFile->SetWindow(categoryFolderWnd);
		tabInfosFile->SetId(WM_CRITERIA);
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

	Connect(wxEVT_CHECKTREE_CHOICE, wxCommandEventHandler(CPanelPhotoWnd::OnSelChanged), nullptr, this);
	Connect(wxEVENT_SETFOLDER, wxCommandEventHandler(CPanelPhotoWnd::SetFolder));
	Connect(wxEVENT_SAVEPARAMETER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPanelPhotoWnd::SaveParameter));
	Connect(wxEVENT_SELCHANGED, wxCommandEventHandler(CPanelPhotoWnd::OnSelChanged));
	Connect(wxEVENT_UPDATECRITERIA, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CPanelPhotoWnd::UpdateCriteria));
	Connect(wxEVENT_REFRESHDATA, wxCommandEventHandler(CPanelPhotoWnd::OnRefreshData));

	categoryFolderWnd->UpdateCriteria(false);
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
		if (needToSendMessage < 2)
		{
			categoryFolderWnd->UpdateCriteria((needToSendMessage == 1) ? true : false);
		}
		else if (needToSendMessage == 2)
		{
			categoryFolderWnd->RefreshCriteriaSearch();
		}
		else if (needToSendMessage == 3)
		{
			wxString title = CLibResource::LoadStringFromResource(L"LBLSTOPALLPROCESS", 1);
			wxString message = CLibResource::LoadStringFromResource(L"LBLSTOPPROCESS", 1);
			StopAllProcess(title, message, this);

			//Suppression de l'ensemble des données
			CSqlPhotoCriteria sqlPhotoCriteria;
			sqlPhotoCriteria.DeletePhotoCriteria();
			CSqlInsertFile insertFile;
			insertFile.ReinitPhotosToProcess();

			SetStopProcess(false);
			//categoryFolderWnd->UpdateCriteria(true);
			categoryFolderWnd->RefreshCriteriaSearch();
		}
	}
}

void CPanelPhotoWnd::SetFolder(wxCommandEvent& folderEvent)
{
	if (folderEvent.GetClientData() != nullptr)
	{
		wxTreeItemId treeitem;
		auto folder = static_cast<wxString*>(folderEvent.GetClientData());
		folderWnd->SetPath(*folder);
		if (folderWnd->GetTreeCtrl() != nullptr)
		{
			treeitem = folderWnd->GetTreeCtrl()->GetSelection();
			folderWnd->GetTreeCtrl()->Check(treeitem);
		}
		
		delete folder;
	}
}

void CPanelPhotoWnd::OnSelChanged(wxCommandEvent& aEvent)
{
	auto windowMain = static_cast<CWindowMain*>(this->FindWindowById(MAINVIEWERWINDOWID));
	if (windowMain != nullptr)
	{
		wxString getSelectPath = folderWnd->GetPath();
		int isChecked = aEvent.GetExtraLong();
		int isShowDialog = aEvent.GetInt();
		wxCommandEvent evt(wxEVENT_UPDATEFOLDER);

		if (isChecked)
		{
			folderWnd->AddPath(getSelectPath);
			const wxString firstFile = AddFolder(getSelectPath, !isShowDialog);
			evt.SetInt(0);
			auto newPath = new wxString(firstFile);
			evt.SetClientData(newPath);
		}
		else
		{
			evt.SetInt(1);
			folderWnd->RemovePath(getSelectPath);
			RemoveFolder(getSelectPath);
		}


		windowMain->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CPanelPhotoWnd::OnRefreshData(wxCommandEvent& event)
{
	switch (windowVisible)
	{
	case WM_FOLDER:
		{
			wxWindow* window = this->FindWindowById(MAINVIEWERWINDOWID);
			if (window != nullptr)
			{
				const wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REFRESHFOLDER);
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
	default: ;
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
		default: ;
		}
	}
}


//Add and Remove Folder Management

wxString CPanelPhotoWnd::AddFolder(const wxString& folder, const bool &showDialog)
{
	TRACE();
	auto windowMain = static_cast<CWindowMain*>(this->FindWindowById(MAINVIEWERWINDOWID));
	wxString localFilename = "";
	wxString msg = "In progress ...";

	wxArrayString files;
	wxDir::GetAllFiles(folder, &files, wxEmptyString, wxDIR_FILES);
	if (files.size() > 0)
		sort(files.begin(), files.end());

	wxBusyInfo wait("Please wait, working...", windowMain);
	{
		//Indication d'imporation des critères 
		CSqlFolderCatalog sqlFolderCatalog;
		int64_t idFolder = sqlFolderCatalog.GetFolderCatalogId(NUMCATALOGID, folder);

		printf("AddFolder : %s \n", CConvertUtility::ConvertToUTF8(folder));

		if (idFolder == -1)
		{
			idFolder = sqlFolderCatalog.GetOrInsertFolderCatalog(NUMCATALOGID, folder);
			//Insert la liste des photos dans la base de données.
			CSqlInsertFile sqlInsertFile;
			sqlInsertFile.AddFileFromFolder(this, nullptr, files, folder, idFolder, localFilename);
			//printf("CMainWindow::AddFolder : %s \n", CConvertUtility::ConvertToUTF8(localFilename));
		}
	}


	wxWindow* window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
	if (window)
	{
		auto newFolder = new wxString(folder);
		wxCommandEvent evt(wxEVENT_SETFOLDER);
		evt.SetClientData(newFolder);
		window->GetEventHandler()->AddPendingEvent(evt);
	}

	window = this->FindWindowById(LISTFACEID);
	if (window)
	{
		wxCommandEvent evt(wxEVENT_THUMBNAILFOLDERADD);
		window->GetEventHandler()->AddPendingEvent(evt);
	}

	

	return localFilename;
}


void CPanelPhotoWnd::RemoveFolder(const wxString& folder)
{
	auto windowMain = static_cast<CWindowMain*>(this->FindWindowById(MAINVIEWERWINDOWID));
	TRACE();
	wxBusyInfo wait("Please wait, working...", windowMain);
	if (!folder.IsEmpty())
	{
		wxString title = CLibResource::LoadStringFromResource(L"LBLSTOPALLPROCESS", 1);
		wxString message = CLibResource::LoadStringFromResource(L"LBLSTOPPROCESS", 1);
		StopAllProcess(title, message, this);

		//Indication d'imporation des critères 
		CSqlFolderCatalog sqlFolderCatalog;
		int64_t idFolder = sqlFolderCatalog.GetFolderCatalogId(NUMCATALOGID, folder);
		if (idFolder != -1)
		{
			CSQLRemoveData sqlRemoveData;
			sqlRemoveData.DeleteFolder(idFolder);
		}

		SetStopProcess(false);

		wxWindow* listFace = this->FindWindowById(LISTFACEID);
		if (listFace != nullptr)
		{
			wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
			listFace->GetEventHandler()->AddPendingEvent(evt);
		}
	}
}
