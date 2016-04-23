#include "MainWindow.h"
#include <libResource.h>
#include "ListPicture.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include "PreviewWnd.h"
#include <libPicture.h>
#include "CategoryFolderWindow.h"
#include <SqlInsertFile.h>
#include <SqlPhotoCriteria.h>
#include <SqlFindFolderCatalog.h>
#include <SQLRemoveData.h>
#include <SqlFolderCatalog.h>
#include <Criteria.h>
#include "ExplorerParamInit.h"
#include "ExplorerThemeInit.h"
#include "PreviewThumbnail.h"
#include <FileGeolocation.h>
#include <ConvertUtility.h>

using namespace std;
using namespace Regards::Sqlite;
using namespace Regards::Explorer;

const int timeToRefresh = 5 * 60 * 1000;

enum
{
	TIMER_REFRESHFOLDER = 1,
    TIMER_REFRESHLIST = 2
};

#define EVENT_CRITERIAUPDATE 1

CMainWindow::CMainWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarViewer, IMainInterface * mainInterface)
	: CWindowMain(parent, id)
{
    
#ifdef __APPLE__
    //Test du nombre de répertoire
    CSqlFindFolderCatalog findFolder;
    FolderCatalogVector catalogfolderVector;
    int nbFolder = findFolder.GetNbFolderCatalog(NUMCATALOGID);

    if(nbFolder > 0 && findFolder.GetFolderCatalog(&catalogfolderVector, NUMCATALOGID))
    {
        
        for(CFolderCatalog folderCatalog : catalogfolderVector)
        {
            CAccessAppleFolder accessFolder;
            //Initialisation des accs sŽcurisŽs aux rŽpertoires
            accessFolder.selectFolder = new CAppleSelectFile(folderCatalog.GetFolderPath());
            CSqlFolderCatalog sqlfolderCatalog;
            accessFolder.appleSecurityScopeData = sqlfolderCatalog.GetDataSecurityScope(folderCatalog.GetFolderPath());
            if(accessFolder.appleSecurityScopeData != nullptr)
                accessFolder.selectFolder->EnableSecurityAccess(accessFolder.appleSecurityScopeData);
            
            appleFolderAccessVector.push_back(accessFolder);
        }
        
    }
#else
	CSqlFindFolderCatalog findFolder;
	int nbFolder = findFolder.GetNbFolderCatalog(NUMCATALOGID);
#endif
    
	this->mainInterface = mainInterface;
	statusBarExplorer = statusBarViewer;
	videoEffectParameter = new CVideoEffectParameter();
	explorerTheme = CExplorerThemeInit::getInstance();
	explorerParam = CExplorerParamInit::getInstance();

	statusBar = new wxStatusBar(this, wxID_ANY, wxSTB_DEFAULT_STYLE, "wxStatusBar");

	int tabWidth[] = { 100, 300, 200, 200 };
	statusBar->SetFieldsCount(4);
	statusBar->SetStatusWidths(4, tabWidth);

	progressBar = new wxGauge(statusBar, wxID_ANY, 200, wxPoint(800, 0), wxSize(200, statusBar->GetSize().y),
		wxGA_HORIZONTAL);
	progressBar->SetRange(100);
	progressBar->SetValue(50);

	if (explorerTheme != nullptr)
	{
		CThemeSplitter themeSplitter;
		explorerTheme->GetCentralWndTheme(themeSplitter);
		centralWnd = new CCentralWnd(this, CENTRALVIEWERWINDOWID, statusBarViewer, themeSplitter, videoEffectParameter);
		centralWnd->HideToolbar();
	}

	if (explorerTheme != nullptr)
	{
		CThemeToolbar themeToolbar;
		explorerTheme->GetExplorerToolbarTheme(themeToolbar);
		toolbar = new CMainToolbar(this, MAINTOOLBARID, themeToolbar);
	}

	it = photosVector.end();

	timerRefreshFolder = new wxTimer(this, TIMER_REFRESHFOLDER);
    timerRefreshList = new wxTimer(this, TIMER_REFRESHLIST);
	Connect(wxEVT_IDLE, wxIdleEventHandler(CMainWindow::OnIdle));
	Connect(TOOLBAR_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnShowToolbar));
	Connect(TIMER_REFRESHFOLDER, wxEVT_TIMER, wxTimerEventHandler(CMainWindow::OnTimerRefreshListFolder), nullptr, this);
    Connect(TIMER_REFRESHLIST, wxEVT_TIMER, wxTimerEventHandler(CMainWindow::OnTimerRefreshList), nullptr, this);
    Connect(EVENT_CRITERIAUPDATE, wxCommandEventHandler(CMainWindow::CriteriaUpdate));
    Connect(VIDEO_START, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnVideoStart));
    

    if (nbFolder == 0)
    {
        //Choix d'un répertoire
        AddFolder();
    }
    
	update = true;
	timerRefreshFolder->Start(10000, true);
}

void CMainWindow::OnVideoStart(wxCommandEvent& event)
{
    centralWnd->StopLoadingPicture();
}

void CMainWindow::UpdateScreenRatio()
{
    toolbar->UpdateScreenRatio();
    centralWnd->UpdateScreenRatio();
    this->Resize();
}

void CMainWindow::OnShowToolbar(wxCommandEvent& event)
{
	ShowToolbar();
}

CMainWindow::~CMainWindow()
{
#ifdef __APPLE__
    for(CAccessAppleFolder accessFolder : appleFolderAccessVector)
    {
        //Initialisation des accs sŽcurisŽs aux rŽpertoires
        if(accessFolder.appleSecurityScopeData != nullptr)
            accessFolder.selectFolder->StopSecurityAccess(accessFolder.appleSecurityScopeData);
        delete accessFolder.selectFolder;
        delete accessFolder.appleSecurityScopeData;
    }
#endif
	if (timerRefreshFolder->IsRunning())
		timerRefreshFolder->Stop();
	delete(timerRefreshFolder);
    
    if (timerRefreshList->IsRunning())
        timerRefreshList->Stop();
    delete(timerRefreshList);

	delete(centralWnd);
	delete(toolbar);
	delete(statusBar);
	delete(videoEffectParameter);
}

void CMainWindow::SetText(const int &numPos, const wxString &libelle)
{
	statusBar->SetStatusText(libelle, numPos);
}

void CMainWindow::SetRangeProgressBar(const int &range)
{
	if (progressBar != nullptr)
		progressBar->SetRange(range);
}

void CMainWindow::SetPosProgressBar(const int &position)
{
	if (progressBar != nullptr)
		progressBar->SetValue(position);
}


void CMainWindow::Resize()
{
	wxRect rcAffichageBitmap;
	wxSize sizeStatusBar = statusBar->GetSize();

	rcAffichageBitmap.x = 0;
	rcAffichageBitmap.y = toolbar->GetNavigatorHeight();
	rcAffichageBitmap.width = width;
	rcAffichageBitmap.height = height - toolbar->GetNavigatorHeight() - sizeStatusBar.y;

	toolbar->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, toolbar->GetNavigatorHeight());
	centralWnd->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.y, rcAffichageBitmap.width, rcAffichageBitmap.height);
	statusBar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.y + rcAffichageBitmap.height, rcAffichageBitmap.width, sizeStatusBar.y);
}



void CMainWindow::OnTimerRefreshListFolder(wxTimerEvent& event)
{
	this->refreshFolder = true;
	timerRefreshFolder->Start(timeToRefresh, true);
}

void CMainWindow::OnTimerRefreshList(wxTimerEvent& event)
{
	RefreshPhotoList();
}


//Thread
void CMainWindow::FindPhotoCriteria(CFindPhotoCriteria * findPhotoCriteria)
{
	CFileGeolocation geoloc(findPhotoCriteria->urlServer);
    CSqlPhotoCriteria photoCriteria;
    
    CListCriteriaPhoto listCriteriaPhoto;
    wxString latitude = "";
    wxString longitude = "";
    wxString latitudeRef = "";
    wxString longitudeRef = "";
    
    listCriteriaPhoto.numCatalog = NUMCATALOGID;
    listCriteriaPhoto.numPhotoId = findPhotoCriteria->photo.GetId();
    listCriteriaPhoto.photoPath = findPhotoCriteria->photo.GetPath();
    
	geoloc.SetFile(listCriteriaPhoto.photoPath);
	

	if (geoloc.HasGps())
	{
		geoloc.Geolocalisation(&listCriteriaPhoto);
	}
	else
	{
		CInsertCriteria * insertCriteria = new CInsertCriteria();
		insertCriteria->type = CATEGORIE_GEO;
		insertCriteria->value = CLibResource::LoadStringFromResource(L"LBLNOTGEO", 1);
		listCriteriaPhoto.listCriteria.push_back(insertCriteria);
	}
    
    
    wxString datetime = geoloc.GetDateTimeInfos();
    vector<wxString> vDateTime;
    
    if(datetime.Length() > 10)
    {
        datetime = datetime.substr(0,10);
        wxChar separator = datetime[4];
        vDateTime = CConvertUtility::split(datetime, separator);
    
        if (vDateTime.size() == 3)
        {
            datetime = "";
            
            for (int i = 0; i < vDateTime.size(); i++)
            {
                datetime.append(vDateTime.at(i));
                if ((i + 1) < vDateTime.size())
                    datetime.append(".");
                
            }
            
            CInsertCriteria * insertCriteria = new CInsertCriteria();
            insertCriteria->type = CATEGORIE_DATE;
            insertCriteria->value = datetime;
            listCriteriaPhoto.listCriteria.push_back(insertCriteria);
        }
    }


	photoCriteria.InsertPhotoListCriteria(listCriteriaPhoto, findPhotoCriteria->criteriaNew, geoloc.HasGps());
    

    if(findPhotoCriteria->mainWindow != nullptr)
    {
        wxCommandEvent event(EVENT_CRITERIAUPDATE);
        event.SetClientData(findPhotoCriteria);
        wxPostEvent(findPhotoCriteria->mainWindow, event);
    }
}

void CMainWindow::RefreshPhotoList()
{
	CCategoryFolderWindow * catalogWnd = (CCategoryFolderWindow *)this->FindWindowById(CATEGORYFOLDERWINDOWID);
	if (catalogWnd != nullptr)
		catalogWnd->UpdatePhotoList();
}

void CMainWindow::CriteriaUpdate(wxCommandEvent& event)
{
    CFindPhotoCriteria * findPhotoCriteria = (CFindPhotoCriteria *)event.GetClientData();
    
    if (findPhotoCriteria->criteriaNew)
    {
        CCategoryFolderWindow * catalogWnd = (CCategoryFolderWindow *)this->FindWindowById(CATEGORYFOLDERWINDOWID);
        if (catalogWnd != nullptr)
            catalogWnd->UpdateCriteria();
    }
    
    numImage++;
    wxString message = "Image Processing " + to_string(numImage) + " / " + to_string(photosVector.size());
    statusBarExplorer->SetText(3, message);
    
    
    findPhotoCriteria->phthread->join();
    
    delete findPhotoCriteria->phthread;
    delete findPhotoCriteria;
    
    numProcess--;
}

void CMainWindow::OnIdle(wxIdleEvent& evt)
{
	if (update)
	{
		numImage = 0;
		photosVector.clear();
		CSqlInsertFile sqlInsertFile;
		sqlInsertFile.GetPhotos(&photosVector);

		it = photosVector.begin();

		statusBarExplorer->SetRangeProgressBar((int)photosVector.size());
		statusBarExplorer->SetPosProgressBar(0);
		wxString message = "Image Processing 0 / " + to_string(photosVector.size());
		statusBarExplorer->SetText(3, message);
		update = false;
        traitementEnd = false;
        
        timerRefreshList->Start(3000);
	}

	if (it != photosVector.end() && numProcess < 4)
	{
        //Put in a thread
		wxString urlServer;
		CRegardsConfigParam * param = CParamInit::getInstance();
		if (param != nullptr)
		{
			urlServer = param->GetUrlServer();
		}
        
        CFindPhotoCriteria * findPhotoCriteria = new CFindPhotoCriteria();
        findPhotoCriteria->urlServer = urlServer;
        findPhotoCriteria->mainWindow = this;
        findPhotoCriteria->photo = *it;it++;
        findPhotoCriteria->phthread = new std::thread(FindPhotoCriteria, findPhotoCriteria);

        numProcess++;


	}
    else if(!traitementEnd)
    {
        timerRefreshList->Stop();
		RefreshPhotoList();
        traitementEnd = true;
    }
	else if (refreshFolder)
	{
		wxString message;
		int counter = 0;
		CSqlFindFolderCatalog folder;
		FolderCatalogVector catalogfolderVector;
		folder.GetFolderCatalog(&catalogfolderVector, 1);

		statusBarExplorer->SetRangeProgressBar((int)catalogfolderVector.size());
		statusBarExplorer->SetPosProgressBar(0);

		for (CFolderCatalog folder : catalogfolderVector)
		{
			counter++;
			message = "Folder Processing " + to_string(counter) + " / " + to_string(catalogfolderVector.size());
			statusBarExplorer->SetText(2, message);
			RefreshThreadFolder(&folder);
			statusBarExplorer->SetPosProgressBar(counter);
		}

		update = true;
		refreshFolder = false;

		
	}
}


void CMainWindow::RefreshThreadFolder(CFolderCatalog * folder)
{
	vector<wxString> listFilePath;
	vector<int> listNumPhoto;
	CriteriaVector criteriaVector;
	CSqlInsertFile insertFile;
	insertFile.InsertPhotoFolderToRefresh(folder->GetFolderPath());
	insertFile.GetPhotoToAdd(&listFilePath);
	insertFile.GetPhotoToRemove(&listNumPhoto, folder->GetNumFolder());
	if (listFilePath.size() > 0)
		insertFile.ImportFileFromFolder(listFilePath, folder->GetNumFolder());

	if (listNumPhoto.size() > 0)
		CSQLRemoveData::DeleteListPhoto(listNumPhoto, &criteriaVector);

	if (listFilePath.size() > 0 || listNumPhoto.size() > 0)
	{
		//Refresh Criteria 
		//Mise à jour de l'affichage de l'arborescence
		CCategoryFolderWindow * catalogWnd = (CCategoryFolderWindow *)this->FindWindowById(CATEGORYFOLDERWINDOWID);
		if (catalogWnd != nullptr)
			catalogWnd->UpdateCriteria();
	}
}

void CMainWindow::RefreshViewer()
{
	if (mainInterface != nullptr)
		mainInterface->UpdateViewer();
}

void CMainWindow::ShowRegardsViewer()
{
	if (statusBarExplorer != nullptr)
		statusBarExplorer->ShowViewer();
}


//--------------------------------------------------------------------
//Ajout d'un répertoire
//--------------------------------------------------------------------
int CMainWindow::AddFolder()
{
	wxDirDialog dlg(nullptr, "Choose a folder", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() != wxID_OK)
		return 0;

	wxString folder = dlg.GetPath();

	//Indication d'imporation des critères 
	CSqlFolderCatalog sqlFolderCatalog;

	if (sqlFolderCatalog.GetFolderCatalogId(NUMCATALOGID, folder) == -1)
	{
		int idFolder = (int)sqlFolderCatalog.GetOrInsertFolderCatalog(NUMCATALOGID, folder);

		//Insert la liste des photos dans la base de données.
		CSqlInsertFile sqlInsertFile;
		sqlInsertFile.ImportFileFromFolder(folder, idFolder);
        update = true;
        
        CCategoryFolderWindow * catalogWnd = (CCategoryFolderWindow *)this->FindWindowById(CATEGORYFOLDERWINDOWID);
        if (catalogWnd != nullptr)
            catalogWnd->UpdateCriteria();
	}
    else
    {
        wxMessageBox(L"This Folder is already imported", "Informations",wxICON_WARNING);
    }

	return 1;
}


//--------------------------------------------------------------------
//Suppression d'un répertoire
//--------------------------------------------------------------------
void CMainWindow::RemoveFolder(const int &idFolder)
{
	//Message box
	if (wxMessageBox(L"Are you sure to remove this folder ?", "Informations", wxYES_NO | wxICON_WARNING) == wxYES)
	{
		CSqlInsertFile sqlInsertFile;
		sqlInsertFile.RemovePhotos(idFolder);

		CriteriaVector criteriaVector;

		if (idFolder != -1)
		{
			CSQLRemoveData sqlRemoveData;
			sqlRemoveData.DeleteFolder(idFolder);
		}

		//Test du nombre de répertoire
		CSqlFindFolderCatalog findFolder;
		int nbFolder = findFolder.GetNbFolderCatalog(NUMCATALOGID);
		if (nbFolder == 0)
		{
			//Choix d'un répertoire
			if (AddFolder() == 0)
                return;
		}
		else
		{
			//Mise à jour de l'affichage de l'arborescence
			CCategoryFolderWindow * catalogWnd = (CCategoryFolderWindow *)this->FindWindowById(CATEGORYFOLDERWINDOWID);
			if (catalogWnd != nullptr)
				catalogWnd->UpdateCriteria();
		}
	}
}

void CMainWindow::RefreshFolder()
{
	refreshFolder = true;
}


void CMainWindow::ShowToolbar()
{
	showToolbar = !showToolbar;

	if (!showToolbar)
		centralWnd->HideToolbar();
	else
		centralWnd->ShowToolbar();
}

void CMainWindow::ShowPreview()
{
	CPreviewThumbnail * previewThumbnail = (CPreviewThumbnail *)this->FindWindowById(PREVIEWTHUMBNAILWINDOWID);
	if (previewThumbnail != nullptr)
		previewThumbnail->ShowPreview();
}

void CMainWindow::ShowPreviewInfos()
{
	CPreviewWnd * preview = (CPreviewWnd *)this->FindWindowById(PREVIEWEXPLORERID);
	if (preview != nullptr)
		preview->ShowInfos();
}

void CMainWindow::Exit()
{
	if (statusBarExplorer != nullptr)
		statusBarExplorer->Exit();
}