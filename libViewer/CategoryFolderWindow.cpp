#include "CategoryFolderWindow.h"
#include <LibResource.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#include "ViewerParamInit.h"
#include <ListCriteriaPhoto.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <SqlInsertFile.h>
#include <SqlCriteria.h>
#include <SqlFindFolderCatalog.h>
#include <SqlPhotoCriteria.h>
#include <FileGeolocation.h>
#include <RegardsConfigParam.h>
#include "ParamInit.h"
#include <ConvertUtility.h>
#include <SQLRemoveData.h>
#include <SqlPhotos.h>
#include <FolderCatalog.h>
using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::Internet;
#define NUMCATALOGID 1
wxDEFINE_EVENT(EVENT_CRITERIAUPDATE, wxCommandEvent);
wxDEFINE_EVENT(EVENT_CRITERIAPHOTOUPDATE, wxCommandEvent);

class CFindPhotoCriteria
{
public:
	wxString urlServer;
	CCategoryFolderWindow * mainWindow = nullptr;
	bool criteriaNew = false;
	int numFolder = 0;
	CPhotos photo;
	std::thread * phthread = nullptr;
};

CCategoryFolderWindow::CCategoryFolderWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarViewer)
	: CWindowMain(parent, id)
{
	this->statusBarViewer = statusBarViewer;
	catalogWndOld = nullptr;
	treeWindow = nullptr;//new CTreeWindow();
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLCRITERIA", 1);
		CThemeScrollBar themeScroll;
		CThemeTree themeTree;

		viewerTheme->GetCategoryScrollTheme(themeScroll);
		catalogWndScroll = new CScrollbarWnd(this, wxID_ANY);
		
		viewerTheme->GetCategoryTreeTheme(themeTree);
		treeWindow = new CTreeWindow(catalogWndScroll, CATEGORYWINDOWID, themeTree);
		catalogWndScroll->SetCentralWindow(treeWindow, themeScroll);
	}

    Connect(EVENT_CRITERIAUPDATE, wxCommandEventHandler(CCategoryFolderWindow::RefreshCriteriaSearch));
	Connect(EVENT_CRITERIAPHOTOUPDATE, wxCommandEventHandler(CCategoryFolderWindow::CriteriaPhotoUpdate));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CCategoryFolderWindow::OnIdle));
	update = true;
}


void CCategoryFolderWindow::RefreshCriteriaSearch(wxCommandEvent& event)
{
    if(catalogWndOld != nullptr)
        catalogWndOld->RefreshCriteriaSearch();
}

void CCategoryFolderWindow::UpdateScreenRatio()
{
    catalogWndScroll->UpdateScreenRatio();
    catalogWndOld->UpdateScreenRatio();
    treeWindow->UpdateScreenRatio();
}


CCategoryFolderWindow::~CCategoryFolderWindow()
{
	delete(treeWindow);
	delete(catalogWndScroll);
	delete(catalogWndOld);
}

void CCategoryFolderWindow::InitSaveParameter()
{
    CViewerParam * config = (CViewerParam *)CViewerParamInit::getInstance();
    if(config != nullptr)
    {
        config->SetCatalogOpenTriangle("");
        config->SetCatalogCriteria("");
    }
}

void CCategoryFolderWindow::Init(const int64_t &numFolder)
{
	this->numFolder = numFolder;

	UpdateCriteria();
	update = true;
}

void CCategoryFolderWindow::UpdateCriteria()
{
	CWindowMain * windowMain = (CWindowMain *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (windowMain != nullptr && treeWindow != nullptr)
	{
		CCategoryWnd * catalogWnd = new CCategoryWnd(windowMain, treeWindow->GetTheme(), treeWindow);
		catalogWnd->Init(numFolder);
		treeWindow->SetTreeControl(catalogWnd);
		delete(catalogWndOld);
		catalogWndOld = catalogWnd;	
	}
	treeWindow->Refresh();
}

void CCategoryFolderWindow::Resize()
{
	catalogWndScroll->SetSize(width, height);
}

void CCategoryFolderWindow::RefreshFilter()
{
    numImage = 0;
    photosVector.clear();
    CSqlInsertFile sqlInsertFile;
    sqlInsertFile.GetAllPhotos(&photosVector);
    
    it = photosVector.begin();
    wxString message = "Image Processing 0 / " + to_string(photosVector.size());
    
    if (statusBarViewer != nullptr)
    {
        statusBarViewer->SetRangeProgressBar((int)photosVector.size());
        statusBarViewer->SetPosProgressBar(0);
        statusBarViewer->SetText(2, message);
    }
    update = false;
    traitementEnd = false;
}

void CCategoryFolderWindow::OnIdle(wxIdleEvent& evt)
{
	if (update)
	{
		numImage = 0;
		photosVector.clear();
		CSqlInsertFile sqlInsertFile;
		sqlInsertFile.GetPhotos(&photosVector, numFolder);

		it = photosVector.begin();
		wxString message = "Image Processing 0 / " + to_string(photosVector.size());

		if (statusBarViewer != nullptr)
		{
			statusBarViewer->SetRangeProgressBar((int)photosVector.size());
			statusBarViewer->SetPosProgressBar(0);
			statusBarViewer->SetText(2, message);
		}
		update = false;
		traitementEnd = false;
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
		findPhotoCriteria->numFolder = numFolder;
		findPhotoCriteria->photo = *it; it++;
		findPhotoCriteria->phthread = new std::thread(FindPhotoCriteria, findPhotoCriteria);
		numProcess++;
	}
	else if (!traitementEnd)
	{
        //Nettoyage des critres non utiliss
        CSqlCriteria criteria;
        criteria.DeleteCriteriaAlone();
        
		//UpdatePhotoList();
		CWindowMain * windowMain = (CWindowMain *)this->FindWindowById(MAINVIEWERWINDOWID);
		if (windowMain != nullptr)
		{
			wxCommandEvent * event = new wxCommandEvent(EVENT_UPDATEPHOTOLIST);
			wxQueueEvent(windowMain, event);
		}
		traitementEnd = true;
	}
	else if (refreshFolder)
	{
		wxString message;
		int counter = 0;
		CSqlFindFolderCatalog folder;
		FolderCatalogVector catalogfolderVector;
		folder.GetFolderCatalog(&catalogfolderVector, 1);

		if (statusBarViewer != nullptr)
		{
			statusBarViewer->SetRangeProgressBar((int)catalogfolderVector.size());
			statusBarViewer->SetPosProgressBar(0);
		}

		for (CFolderCatalog folder : catalogfolderVector)
		{
			counter++;
			message = "Folder Processing " + to_string(counter) + " / " + to_string(catalogfolderVector.size());
			if (statusBarViewer != nullptr)
			{
				statusBarViewer->SetText(2, message);
				statusBarViewer->SetPosProgressBar(counter);
			}
			RefreshThreadFolder(&folder);
		}

		update = true;
		refreshFolder = false;
	}
}


void CCategoryFolderWindow::RefreshThreadFolder(CFolderCatalog * folder)
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
		UpdateCriteria();
	}
}


//Thread
void CCategoryFolderWindow::FindPhotoCriteria(CFindPhotoCriteria * findPhotoCriteria)
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

	geoloc.SetFile(listCriteriaPhoto.photoPath, true);


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

	if (datetime.Length() > 10)
	{
		datetime = datetime.substr(0, 10);
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


	photoCriteria.InsertPhotoListCriteria(listCriteriaPhoto, findPhotoCriteria->criteriaNew, geoloc.HasGps(), findPhotoCriteria->numFolder);


	if (findPhotoCriteria->mainWindow != nullptr)
	{
		wxCommandEvent * event = new wxCommandEvent(EVENT_CRITERIAPHOTOUPDATE);
		event->SetClientData(findPhotoCriteria);
		wxQueueEvent(findPhotoCriteria->mainWindow, event);
	}
}

wxString CCategoryFolderWindow::GetSqlRequest()
{
    if(catalogWndOld != nullptr)
        return catalogWndOld->GetSqlRequest();
    return "";
}


void CCategoryFolderWindow::CriteriaPhotoUpdate(wxCommandEvent& event)
{
	CFindPhotoCriteria * findPhotoCriteria = (CFindPhotoCriteria *)event.GetClientData();

	if (findPhotoCriteria->criteriaNew)
	{
		UpdateCriteria();
	}

	numImage++;
	wxString message = "Image Processing " + to_string(numImage) + " / " + to_string(photosVector.size());
	if (statusBarViewer != nullptr)
		statusBarViewer->SetText(2, message);


	findPhotoCriteria->phthread->join();

	CSqlPhotos sqlPhoto;
	sqlPhoto.UpdatePhotoCriteria(findPhotoCriteria->photo.GetId());

	delete findPhotoCriteria->phthread;
	delete findPhotoCriteria;

	numProcess--;
}