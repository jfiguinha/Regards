#include <header.h>
#include "CategoryFolderWindow.h"
#include <LibResource.h>
#include "ViewerParamInit.h"
#include <ListCriteriaPhoto.h>
#include <SqlInsertFile.h>
#include "ViewerParam.h"
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
#include "CategoryWnd.h"
#include "CategoryTreeStatePersistence.h"
#include "CategorySelectionController.h"
#include "CategoryQueryService.h"
#include "CategoryRepository.h"
#include "CategoryHierarchyIndex.h"
#include <ThumbnailMessage.h>
#include <TreeWindow.h>
#include <GpsEngine.h>
#include <Photos.h>
#include <ScrollbarWnd.h>
using namespace std;
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::Internet;

#include <appcontext.h>
extern AppContext application_context;


#define NUMCATALOGID 1

#define TIMETOWAITINTERNET 3

wxDEFINE_EVENT(EVENT_CRITERIAPHOTOUPDATE, wxCommandEvent);


class CListToClean
{
public:
	std::unique_ptr<CCategoryWnd> catalogWndOld;
	std::time_t timeToAdd{};
};

class CCategoryFolderWindowPimpl
{
public:
    
    CCategoryFolderWindowPimpl()
    {
        catalogWnd = nullptr;
        explorerconfig = nullptr;
        oldPos = 0;
        update = false;
        numProcess = 0;
		numProcessGps = 0;
        nbProcesseur = 1;
        needToSendMessage = false;
        refreshTimer = nullptr;
		startUpdateCriteria = false;
    };
    
    ~CCategoryFolderWindowPimpl()
    {
		if (refreshTimer)
    		if (refreshTimer->IsRunning())
				refreshTimer->Stop();
		listToErase.clear();
    }
    
    CCategoryWnd* catalogWnd;
    CMainParam* explorerconfig;
	std::atomic<bool> traitementEnd;
	std::atomic<bool> startUpdateCriteria;

    int oldPos;
    bool update;

	std::atomic<int> numProcess;
	std::atomic<int> numProcessGps;
    int nbProcesseur;
    bool needToSendMessage;
    bool noCategoryMessage;
    bool categoryMessage;
    int nbPhotos = 0;

	deque<int> listCriteriaToGeolocalize;
    PhotosVector m_photosVector;
    wxString urlServer;
	wxString apiKey = "";
	std::unique_ptr<wxTimer> refreshTimer;
    std::mutex muVector;
	std::vector<std::unique_ptr<CListToClean>> listToErase;
};


class CFindPhotoCriteria
{
public:
	CFindPhotoCriteria();

	wxString urlServer;
	wxString apiKey;
	CCategoryFolderWindow* mainWindow;
	bool criteriaNew;
	int numPhoto{};
	int numFolderId{};
	bool fromGps = false;
	int numCriteria = 0;
	wxString photoPath;
	std::unique_ptr<thread> phthread;
	bool isOk = true;
};

CFindPhotoCriteria::CFindPhotoCriteria()
{
	mainWindow = nullptr;
	criteriaNew = false;
	phthread = nullptr;
}

CCategoryFolderWindow::CCategoryFolderWindow(wxWindow* parent, const wxWindowID idCTreeWithScrollbarViewer, const CThemeScrollBar& themeScroll,
                                             const CThemeTree& theme)
	: CTreeWithScrollbar("CCategoryFolderWindow", parent, idCTreeWithScrollbarViewer, themeScroll, theme)
{
	CListOfWindow* fileGeolocalisation = CGpsEngine::getInstance();
	fileGeolocalisation->AddWindow(this);
    
    pimpl = std::make_unique<CCategoryFolderWindowPimpl>();

	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		pimpl->nbProcesseur = config->GetExifProcess();

	time(&start);

	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
	{
		pimpl->urlServer = param->GetGeoLocUrlServer();
		pimpl->apiKey = param->GetApiKey();
		if(pimpl->apiKey == "")
			application_context.isGPsAvailable = false;
		else
			application_context.isGPsAvailable = true;
	}
	pimpl->refreshTimer = std::make_unique<wxTimer>(this, wxTIMER_REFRESH);
	Connect(EVENT_CRITERIAPHOTOUPDATE, wxCommandEventHandler(CCategoryFolderWindow::CriteriaPhotoUpdate));
	Connect(wxTIMER_REFRESH, wxEVT_TIMER, wxTimerEventHandler(CCategoryFolderWindow::OnTimerRefresh), nullptr, this);
	Connect(wxEVENT_UPDATEGPSINFOS, wxCommandEventHandler(CCategoryFolderWindow::OnUpdateGpsInfos));
	Connect(wxEVENT_REFRESHFOLDER, wxCommandEventHandler(CCategoryFolderWindow::OnRefreshFolder));

	pimpl->update = true;
	pimpl->noCategoryMessage = false;
	pimpl->categoryMessage = false;
	processIdle = true;

	if (application_context.isGPsAvailable)
		pimpl->refreshTimer->Start(60000, wxTIMER_CONTINUOUS);
	
	if (param != nullptr)
		nbGpsFileByMinute = param->GetNbGpsIterationByMinute();

	init();

	listProcessWindow.push_back(this);
}

void CCategoryFolderWindow::OnRefreshFolder(wxCommandEvent& event)
{
	init();
}

void CCategoryFolderWindow::OnTimerRefresh(wxTimerEvent& event)
{
	nbGpsRequest = 0;
	processIdle = true;
}

void CCategoryFolderWindow::RefreshCriteriaSearch()
{
	if (pimpl->catalogWnd != nullptr)
		pimpl->catalogWnd->RefreshCriteriaSearch();

	init();
}

void CCategoryFolderWindow::OnUpdateGpsInfos(wxCommandEvent& event)
{
	auto filename = static_cast<wxString*>(event.GetClientData());
	if (filename != nullptr)
		delete filename;

	UpdateCriteria(false);
}

void CCategoryFolderWindow::RefreshCriteriaSearch(wxCommandEvent& event)
{
	if (pimpl->catalogWnd != nullptr)
		pimpl->catalogWnd->RefreshCriteriaSearch();
}

void CCategoryFolderWindow::InitSaveParameter()
{
	auto config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
		config->SetCatalogOpenTriangle("");
		config->SetCatalogCriteria("");
	}
}

void CCategoryFolderWindow::init()
{
	UpdateCriteria(false);
	pimpl->update = true;
	processIdle = true;

	{
		std::lock_guard<std::mutex> lock(pimpl->muVector);
		//Get List of Photo to process
		CSqlInsertFile sql_insert_file;
		pimpl->m_photosVector.clear();
		sql_insert_file.GetPhotoToProcessList(&pimpl->m_photosVector);

		nbPhotoToProcess = pimpl->m_photosVector.size();

		if (application_context.isGPsAvailable)
		{
			CSqlCriteria sqlCriteria;
			pimpl->listCriteriaToGeolocalize = sqlCriteria.GetListCriteriaToGeolocalize();
		}
	}


}

void CCategoryFolderWindow::UpdateCriteria(const bool& need_to_send_message)
{
	//printf("CCategoryFolderWindow::UpdateCriteria() \n");
	pimpl->needToSendMessage = need_to_send_message;
	auto windowMain = static_cast<CWindowMain*>(this->FindWindowById(MAINVIEWERWINDOWID));
	if (windowMain != nullptr && treeWindow != nullptr)
	{
		auto catalogWnd = new CCategoryWnd(windowMain, treeWindow->GetTheme(), treeWindow);
		catalogWnd->Init();
		treeWindow->SetTreeControl(catalogWnd);
        
		auto listToAdd = std::make_unique<CListToClean>();
		listToAdd->catalogWndOld.reset(pimpl->catalogWnd);

		pimpl->catalogWnd = catalogWnd;
		pimpl->update = true;
        
       
		time(&listToAdd->timeToAdd);
		
		pimpl->listToErase.push_back(std::move(listToAdd));


	}

	processIdle = true;
}

bool CCategoryFolderWindow::GetProcessEnd()
{
     //printf("CCategoryFolderWindow::GetProcessEnd() \n");
     
	if (pimpl->numProcess > 0)
		return false;
	return true;
}

wxString CCategoryFolderWindow::GetWaitingMessage()
{
	return "Window CCategoryFolderWindow waiting : " + to_string(this->GetId()) + " - NbProcess Waiting : " +
		to_string(pimpl->numProcess);
}

void CCategoryFolderWindow::ProcessPhotoQueue()
{

	std::lock_guard<std::mutex> lock(pimpl->muVector);
	pimpl->startUpdateCriteria = true;
	auto findPhotoCriteria = new CFindPhotoCriteria();
	CPhotos photo = pimpl->m_photosVector[0];

	if (photo.GetId() != -1)
	{
		findPhotoCriteria->numPhoto = photo.GetId();
		findPhotoCriteria->photoPath = photo.GetPath();
		findPhotoCriteria->numFolderId = photo.GetFolderId();
		findPhotoCriteria->urlServer = pimpl->urlServer;
		findPhotoCriteria->apiKey = pimpl->apiKey;
		findPhotoCriteria->mainWindow = this;
		//findPhotoCriteria->numFolder = photo.GetFolderId();

		findPhotoCriteria->phthread = std::make_unique<thread>(FindPhotoCriteria, findPhotoCriteria);
		pimpl->numProcess++;
		CSqlInsertFile sql_insert_file;
		sql_insert_file.UpdatePhotoProcess(photo.GetId());
		pimpl->traitementEnd = false;
		pimpl->m_photosVector.erase(pimpl->m_photosVector.begin());

		{
			auto thumbnailMessage = new CThumbnailMessage();
			thumbnailMessage->thumbnailPos = nbPhotoToProcess - pimpl->m_photosVector.size();
			thumbnailMessage->nbElement = nbPhotoToProcess;
			thumbnailMessage->nbPhoto = pimpl->m_photosVector.size();
			thumbnailMessage->typeMessage = 0;
			SendStatusMessage(thumbnailMessage);
		}
	}
}

void CCategoryFolderWindow::ProcessGpsQueue()
{
	auto findPhotoCriteria = new CFindPhotoCriteria();
	findPhotoCriteria->urlServer = pimpl->urlServer;
	findPhotoCriteria->mainWindow = this;
	findPhotoCriteria->numCriteria = pimpl->listCriteriaToGeolocalize[0];
	findPhotoCriteria->phthread = std::make_unique<thread>(FindGPSPhotoCriteria, findPhotoCriteria);

	pimpl->listCriteriaToGeolocalize.pop_front();
	pimpl->numProcessGps++;
	nbGpsRequest++;
	processIdle = true;
	time(&start);

	{
		auto thumbnailMessage = new CThumbnailMessage();
		thumbnailMessage->thumbnailPos = 1;
		thumbnailMessage->nbPhoto = nbPhotoGpsToProcess;
		thumbnailMessage->nbElement = nbPhotoGpsToProcess;
		thumbnailMessage->typeMessage = 6;
		SendStatusMessage(thumbnailMessage);

	}
}

void CCategoryFolderWindow::SendStatusMessage(CThumbnailMessage * thumbnailMessage)
{
	wxWindow* mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWnd != nullptr)
	{
		wxCommandEvent eventChange(wxEVENT_UPDATESTATUSBARMESSAGE);
		eventChange.SetClientData(thumbnailMessage);
		mainWnd->GetEventHandler()->AddPendingEvent(eventChange);
	}
	else
		delete thumbnailMessage;
}


void CCategoryFolderWindow::CleanupOldCatalogs()
{
	if (!pimpl->listToErase.empty())
	{
		time_t ending;
		time(&ending);
		for (auto it = pimpl->listToErase.begin();
			it != pimpl->listToErase.end(); )
		{
			if (difftime(ending, (*it)->timeToAdd) > 5)
				it = pimpl->listToErase.erase(it);
			else
				++it;
		}
	}
}

void CCategoryFolderWindow::ProcessIdle()
{
	bool hasSomethingTodo = true;
	int nbPhotos = 0;

	{
		std::lock_guard<std::mutex> lock(pimpl->muVector);
		nbPhotos = pimpl->m_photosVector.size();
	}

	if (nbPhotos > 0 && pimpl->numProcess < pimpl->nbProcesseur)
	{
		ProcessPhotoQueue();
	}
	else if (!pimpl->traitementEnd)
	{
		//Nettoyage des criteres non utilises
		CSqlCriteria criteria;
		criteria.DeleteCriteriaAlone();
		pimpl->traitementEnd = true;
	}
	else if (nbPhotos == 0)
	{
		hasSomethingTodo = false;
	}
	if (!hasSomethingTodo)
		processIdle = false;

	if (application_context.isGPsAvailable)
	{
		time_t ending;
		time(&ending);

		int diff = difftime(ending, start);

		if (pimpl->listCriteriaToGeolocalize.size() > 0 && nbGpsRequest < nbGpsFileByMinute && pimpl->numProcessGps < pimpl->nbProcesseur && diff >= TIMETOWAITINTERNET)
		{
			ProcessGpsQueue();
		}

		if (pimpl->listCriteriaToGeolocalize.size() > 0)
			processIdle = true;
	}

	CleanupOldCatalogs();

	if (pimpl->startUpdateCriteria && pimpl->numProcess <= 0 && nbPhotos == 0)
	{
		pimpl->startUpdateCriteria = false;
		//Send Update Folder
		wxCommandEvent evt(wxEVENT_CRITERIAPHOTOUPDATE);
		evt.SetExtraLong(-1);
		this->GetEventHandler()->AddPendingEvent(evt);
	}

}


void CCategoryFolderWindow::IdleFunction()
{
	if (endProgram)
	{
		if (pimpl->refreshTimer->IsRunning())
			pimpl->refreshTimer->Stop();
	}
}


void CCategoryFolderWindow::RefreshThreadFolder(CFolderCatalog* folder)
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
		UpdateCriteria(true);
		processIdle = true;
	}
}


void CCategoryFolderWindow::FindGPSPhotoCriteria(CFindPhotoCriteria * findPhotoCriteria)
{
	CListOfWindow* geoloc = CGpsEngine::getInstance();
	CSqlCriteria criteria;
	wxString urlServer = "";
	wxString apiKey = "";
	int numCriteria = findPhotoCriteria->numCriteria;
	//Géolocalisation
	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetGeoLocUrlServer();
		apiKey = param->GetApiKey();
	}

	CFileGeolocation fileGeoloc(findPhotoCriteria->urlServer, findPhotoCriteria->apiKey);
	wxString geoinfo = criteria.GetCriteriaLibelle(numCriteria);
	if (geoinfo != "")
	{
		fileGeoloc.SetInfosGPS(geoinfo);


		if (fileGeoloc.HasGps())
		{
			wxString geolocalisation = fileGeoloc.Geolocalize();
			if (geolocalisation != "")
			{
				criteria.UpdateCriteria(1, numCriteria, geolocalisation);
				findPhotoCriteria->isOk = true;
				findPhotoCriteria->criteriaNew = true;
			}
			else
			{
				findPhotoCriteria->isOk = false;
				//printf("Error gps informations \n");
			}
		}
	}
	findPhotoCriteria->fromGps = true;
	//if (geoloc != nullptr)
	//	geoloc->SendMessageToWindow(listCriteriaPhoto.photoPath, 1);

	if (findPhotoCriteria->mainWindow != nullptr)
	{
		auto event = new wxCommandEvent(EVENT_CRITERIAPHOTOUPDATE);
		event->SetClientData(findPhotoCriteria);
		wxQueueEvent(findPhotoCriteria->mainWindow, event);
	}
}

//Thread

void CCategoryFolderWindow::FindPhotoCriteria(CFindPhotoCriteria* findPhotoCriteria)
{
	wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
	CFileGeolocation geoloc(findPhotoCriteria->urlServer, findPhotoCriteria->apiKey);
	CSqlPhotoCriteria photoCriteria;

	wxString urlServer = "";
	wxString apiKey = "";
	//Géolocalisation
	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetGeoLocUrlServer();
		apiKey = param->GetApiKey();
	}

	CListCriteriaPhoto listCriteriaPhoto;
	listCriteriaPhoto.numCatalog = NUMCATALOGID;
	listCriteriaPhoto.numPhotoId = findPhotoCriteria->numPhoto;
	listCriteriaPhoto.photoPath = findPhotoCriteria->photoPath;

	geoloc.SetFile(listCriteriaPhoto.photoPath, notGeo);


	CInsertCriteria insertCriteria;
	insertCriteria.type = CATEGORIE_GEO;
	if (!geoloc.HasGps())
		insertCriteria.value = notGeo;
	else
		insertCriteria.value = application_context.special_key + "/" + geoloc.GetLatitude() + "/" + geoloc.GetLongitude();
	listCriteriaPhoto.listCriteria.push_back(insertCriteria);


	wxString datetime = geoloc.GetDateTimeInfos();

	if (datetime.Length() >= 10)
	{
		vector<wxString> vDateTime;
		datetime = datetime.substr(0, 10);

		wxChar separator = datetime[4];
		vDateTime = CConvertUtility::split(datetime, separator);

		if (vDateTime.size() == 3)
		{
			datetime = "";

			for (auto i = 0; i < vDateTime.size(); i++)
			{
				datetime.append(vDateTime.at(i));
				if (i + 1 < vDateTime.size())
					datetime.append(".");
			}

			CInsertCriteria insertCriteria;
			insertCriteria.type = CATEGORIE_DATE;
			insertCriteria.value = datetime;
			listCriteriaPhoto.listCriteria.push_back(insertCriteria);
			CListOfWindow* list_of_window = CGpsEngine::getInstance();
			if (list_of_window != nullptr)
				list_of_window->SendMessageToWindow(listCriteriaPhoto.photoPath, 2);
		}
	}


	photoCriteria.InsertPhotoListCriteria(listCriteriaPhoto, findPhotoCriteria->criteriaNew, geoloc.HasGps(),
	                                      findPhotoCriteria->numFolderId);


	listCriteriaPhoto.listCriteria.clear();

	if (findPhotoCriteria->mainWindow != nullptr)
	{
		auto event = new wxCommandEvent(EVENT_CRITERIAPHOTOUPDATE);
		event->SetClientData(findPhotoCriteria);
		wxQueueEvent(findPhotoCriteria->mainWindow, event);
	}
}

wxString CCategoryFolderWindow::GetSqlRequest()
{
	wxString sqlRequest = "";
	if (pimpl->catalogWnd != nullptr)
		return pimpl->catalogWnd->GetSqlRequest();
	return sqlRequest;
}

void CCategoryFolderWindow::CriteriaPhotoUpdate(wxCommandEvent& event)
{
	auto findPhotoCriteria = static_cast<CFindPhotoCriteria*>(event.GetClientData());

	if (findPhotoCriteria->isOk)
	{
		if (findPhotoCriteria->criteriaNew)
		{
			UpdateCriteria(true);
		}
	}

	if (findPhotoCriteria->phthread != nullptr)
		findPhotoCriteria->phthread->join();

	if (findPhotoCriteria->fromGps)
	{
		pimpl->numProcessGps--;
		if (pimpl->numProcessGps < 0)
			pimpl->numProcessGps = 0;
		time(&start);
	}
	else
	{
		pimpl->numProcess--;
		if (pimpl->numProcess < 0)
			pimpl->numProcess = 0;

		if (application_context.isGPsAvailable)
		{
			CSqlCriteria sqlCriteria;
			pimpl->listCriteriaToGeolocalize = sqlCriteria.GetListCriteriaToGeolocalize();
		}
	}

	delete findPhotoCriteria;

	processIdle = true;
	
}
