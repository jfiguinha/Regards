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
#include <SqlPhotoGPS.h>
#include <ThumbnailMessage.h>
#include <TreeWindow.h>
#include <GpsEngine.h>
#include <DateValidation.hpp>
#include <Photos.h>
#include <SqlPhotoGPS.h>
using namespace std;
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::Internet;
#define NUMCATALOGID 1

wxDEFINE_EVENT(EVENT_CRITERIAPHOTOUPDATE, wxCommandEvent);


class CCategoryFolderWindowPimpl
{
public:
    
    CCategoryFolderWindowPimpl()
    {
        catalogWndOld = nullptr;
        explorerconfig = nullptr;
        oldPos = 0;
        update = false;
        traitementEnd = true;
        numProcess = 0;
        nbProcesseur = 1;
        refreshFolder = false;
        needToSendMessage = false;
        gpsLocalisationFinish = true;
        refreshTimer = nullptr;

    };
    
    ~CCategoryFolderWindowPimpl()
    {
    	if (refreshTimer->IsRunning())
            refreshTimer->Stop();

        delete(refreshTimer);
        delete(catalogWndOld);

    }
    
    CCategoryWnd* catalogWndOld;
    CMainParam* explorerconfig;
    int oldPos;
    bool update;
    //int numImageFace;
    bool traitementEnd;
    int numProcess;
    int nbProcesseur;
    bool refreshFolder;

    bool needToSendMessage;
    bool noCategoryMessage;
    bool categoryMessage;
    bool startUpdateCriteria = false;
    int nbPhotos = 0;

    PhotosVector m_photosVector;
    wxString urlServer;
    bool gpsLocalisationFinish;
    int nbGpsFile;
    wxTimer* refreshTimer;
    std::mutex muVector;
};

class CFindPhotoCriteria
{
public:
	CFindPhotoCriteria();

	wxString urlServer;
	CCategoryFolderWindow* mainWindow;
	bool criteriaNew;
	int numPhoto{};
	int numFolderId{};
	wxString photoPath;
	thread* phthread;
	bool hasGps;
	bool fromGps;
};

CFindPhotoCriteria::CFindPhotoCriteria()
{
	mainWindow = nullptr;
	criteriaNew = false;
	hasGps = false;
	fromGps = false;
	phthread = nullptr;
}

CCategoryFolderWindow::CCategoryFolderWindow(wxWindow* parent, const wxWindowID idCTreeWithScrollbarViewer, const CThemeScrollBar& themeScroll,
                                             const CThemeTree& theme)
	: CTreeWithScrollbar("CCategoryFolderWindow", parent, idCTreeWithScrollbarViewer, themeScroll, theme)
{
	CListOfWindow* fileGeolocalisation = CGpsEngine::getInstance();
	fileGeolocalisation->AddWindow(this);
    
    pimpl = new CCategoryFolderWindowPimpl();

	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		pimpl->nbProcesseur = config->GetExifProcess();



	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
	{
		pimpl->urlServer = param->GetUrlServer();
	}
	pimpl->refreshTimer = new wxTimer(this, wxTIMER_REFRESH);
	Connect(EVENT_CRITERIAPHOTOUPDATE, wxCommandEventHandler(CCategoryFolderWindow::CriteriaPhotoUpdate));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CCategoryFolderWindow::OnIdle));
	Connect(wxTIMER_REFRESH, wxEVT_TIMER, wxTimerEventHandler(CCategoryFolderWindow::OnTimerRefresh), nullptr, this);
	Connect(wxEVENT_UPDATEGPSINFOS, wxCommandEventHandler(CCategoryFolderWindow::OnUpdateGpsInfos));

	pimpl->update = true;
	pimpl->noCategoryMessage = false;
	pimpl->categoryMessage = false;
	processIdle = true;

	pimpl->nbGpsFile = 0;
	//Refresh all 1 minute
	pimpl->refreshTimer->Start(60000, wxTIMER_CONTINUOUS);
	pimpl->refreshTimer->Start();

	init();

	listProcessWindow.push_back(this);
}

void CCategoryFolderWindow::OnTimerRefresh(wxTimerEvent& event)
{
	printf(" CCategoryFolderWindow::OnTimerRefresh %d \n", pimpl->nbGpsFile);
	pimpl->nbGpsFile = 0;
	processIdle = true;
}

void CCategoryFolderWindow::RefreshCriteriaSearch()
{
	if (pimpl->catalogWndOld != nullptr)
		pimpl->catalogWndOld->RefreshCriteriaSearch();

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
	if (pimpl->catalogWndOld != nullptr)
		pimpl->catalogWndOld->RefreshCriteriaSearch();
}

CCategoryFolderWindow::~CCategoryFolderWindow()
{
    delete(pimpl);
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

	pimpl->muVector.lock();
	//Get List of Photo to process
	CSqlInsertFile sql_insert_file;
	pimpl->m_photosVector.clear();
	sql_insert_file.GetPhotoToProcessList(&pimpl->m_photosVector);
	pimpl->muVector.unlock();
}

void CCategoryFolderWindow::UpdateCriteria(const bool& need_to_send_message)
{
	printf("CCategoryFolderWindow::UpdateCriteria() \n");
	pimpl->needToSendMessage = need_to_send_message;
	auto windowMain = static_cast<CWindowMain*>(this->FindWindowById(MAINVIEWERWINDOWID));
	if (windowMain != nullptr && treeWindow != nullptr)
	{
		auto catalogWnd = new CCategoryWnd(windowMain, treeWindow->GetTheme(), treeWindow);
		catalogWnd->Init();
		treeWindow->SetTreeControl(catalogWnd);
		delete(pimpl->catalogWndOld);
		pimpl->catalogWndOld = catalogWnd;
		pimpl->update = true;


	}





	processIdle = true;
}

bool CCategoryFolderWindow::GetProcessEnd()
{
	if (pimpl->numProcess > 0)
		return false;
	return true;
}

wxString CCategoryFolderWindow::GetWaitingMessage()
{
	return "Window CCategoryFolderWindow waiting : " + to_string(this->GetId()) + " - NbProcess Waiting : " +
		to_string(pimpl->numProcess);
}

void CCategoryFolderWindow::ProcessIdle()
{
    pimpl->muVector.lock();
	bool hasSomethingTodo = true;
	printf("CCategoryFolderWindow::ProcessIdle() \n");
    int nbPhotos = pimpl->m_photosVector.size();
	pimpl->muVector.unlock();

	if (nbPhotos > 0 && pimpl->numProcess < pimpl->nbProcesseur)
	{

		pimpl->muVector.lock();
        
		pimpl->startUpdateCriteria = true;
		//Put in a thread
		//CSqlInsertFile sql_insert_file;
		auto findPhotoCriteria = new CFindPhotoCriteria();
		//CPhotos photo = sql_insert_file.GetPhotoToProcess();
        CPhotos photo = pimpl->m_photosVector[0];
        
		printf("CCategoryFolderWindow::ProcessIdle : Nb Photo : %d Path : %s \n", nbPhotos,
		       CConvertUtility::ConvertToUTF8(photo.GetPath()));

		if (photo.GetId() != -1)
		{
			findPhotoCriteria->numPhoto = photo.GetId();
			findPhotoCriteria->photoPath = photo.GetPath();
			findPhotoCriteria->numFolderId = photo.GetFolderId();
			findPhotoCriteria->urlServer = pimpl->urlServer;
			findPhotoCriteria->mainWindow = this;
			//findPhotoCriteria->numFolder = photo.GetFolderId();

			findPhotoCriteria->phthread = new thread(FindPhotoCriteria, findPhotoCriteria);
			pimpl->numProcess++;
            CSqlInsertFile sql_insert_file;
			sql_insert_file.UpdatePhotoProcess(photo.GetId());
			pimpl->traitementEnd = false;
            pimpl->m_photosVector.erase(pimpl->m_photosVector.begin());
		}

		pimpl->muVector.unlock();
	}
	else if (!pimpl->traitementEnd)
	{
		//Nettoyage des criteres non utilises
		CSqlCriteria criteria;
		criteria.DeleteCriteriaAlone();
		pimpl->traitementEnd = true;
	}
	else if (pimpl->refreshFolder)
	{
		wxString message;
		int counter = 0;
		CSqlFindFolderCatalog folder;
		FolderCatalogVector catalogfolderVector;
		folder.GetFolderCatalog(&catalogfolderVector, 1);

		{
			auto thumbnailMessage = new CThumbnailMessage();
			thumbnailMessage->nbElement = catalogfolderVector.size();
			thumbnailMessage->typeMessage = 1;
			//wxWindow* mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);  // NOLINT(clang-diagnostic-shorten-64-to-32)
			wxCommandEvent eventChange(wxEVENT_UPDATESTATUSBARMESSAGE);
			eventChange.SetClientData(thumbnailMessage);
		}

		for (CFolderCatalog folder_catalog : catalogfolderVector)
		{
			counter++;
			auto thumbnailMessage = new CThumbnailMessage();
			thumbnailMessage->thumbnailPos = counter;
			thumbnailMessage->nbElement = catalogfolderVector.size();
			thumbnailMessage->typeMessage = 2;
			wxWindow* mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
			if (mainWnd != nullptr)
			{
				wxCommandEvent eventChange(wxEVENT_UPDATESTATUSBARMESSAGE);
				eventChange.SetClientData(thumbnailMessage);
				mainWnd->GetEventHandler()->AddPendingEvent(eventChange);
			}
			else
				delete thumbnailMessage;

			RefreshThreadFolder(&folder_catalog);
		}

		pimpl->update = true;
		pimpl->refreshFolder = false;

	}
	else if (nbPhotos == 0)
	{
		hasSomethingTodo = false;
	}
	if (!hasSomethingTodo)
		processIdle = false;

	//---------------------------------------------------------------------------------------------------------------
	//GPS Traitement
	//Thread by Thread
	//---------------------------------------------------------------------------------------------------------------
	int numPhoto = 0;
	int numFolderId = 0;
	wxString photoPath = "";
	CSqlPhotoGPS photoGPS;
	if (photoGPS.GetFirstPhoto(numPhoto, photoPath, numFolderId) > 0)
	{
		int nbGpsFileByMinute = 60;
		printf("Geolocalize File photoGPS.GetFirstPhoto nbGPSFile : %d \n", pimpl->nbGpsFile);
		CRegardsConfigParam* param = CParamInit::getInstance();
		if (param != nullptr)
			nbGpsFileByMinute = param->GetNbGpsIterationByMinute();

		if (pimpl->gpsLocalisationFinish && pimpl->nbGpsFile < nbGpsFileByMinute)
		{
			auto findPhotoCriteria = new CFindPhotoCriteria();
			findPhotoCriteria->urlServer = pimpl->urlServer;
			findPhotoCriteria->mainWindow = this;
			findPhotoCriteria->numPhoto = numPhoto;
			findPhotoCriteria->photoPath = photoPath;
			findPhotoCriteria->numFolderId = numFolderId;
			findPhotoCriteria->phthread = new thread(FindGPSPhotoCriteria, findPhotoCriteria);
			pimpl->gpsLocalisationFinish = false;
			pimpl->nbGpsFile++;
			processIdle = true;
		}
		
		//
	}
}


void CCategoryFolderWindow::OnIdle(wxIdleEvent& evt)
{
    pimpl->muVector.lock();
    int nbPhotos = pimpl->m_photosVector.size();
    pimpl->muVector.unlock();
	if (needToRefresh)
	{
		this->Refresh();
		needToRefresh = false;
	}

	if (pimpl->startUpdateCriteria&& pimpl->numProcess <= 0 && nbPhotos == 0)
	{
		pimpl->startUpdateCriteria = false;
		//Send Update Folder
		wxCommandEvent evt(wxEVENT_CRITERIAPHOTOUPDATE);
		evt.SetExtraLong(-1);
		this->GetEventHandler()->AddPendingEvent(evt);
	}

	if (endProgram)
	{
		if (pimpl->refreshTimer->IsRunning())
			pimpl->refreshTimer->Stop();
	}

	StartThread();
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


void CCategoryFolderWindow::FindGPSPhotoCriteria(CFindPhotoCriteria* findPhotoCriteria)
{
	CListOfWindow* geoloc = CGpsEngine::getInstance();
	CSqlPhotoCriteria photoCriteria;

	wxString urlServer = "";
	//Géolocalisation
	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetUrlServer();
	}

	wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
	CListCriteriaPhoto listCriteriaPhoto;
	CFileGeolocation fileGeolocalisation(urlServer);
	listCriteriaPhoto.numCatalog = NUMCATALOGID;
	listCriteriaPhoto.numPhotoId = findPhotoCriteria->numPhoto;
	listCriteriaPhoto.photoPath = findPhotoCriteria->photoPath;

	printf("FindGPSPhotoCriteria %s \n ", CConvertUtility::ConvertToUTF8(listCriteriaPhoto.photoPath));

	fileGeolocalisation.SetFile(listCriteriaPhoto.photoPath, notGeo);


	if (fileGeolocalisation.HasGps())
	{
		printf("Has GPS %s \n ", CConvertUtility::ConvertToUTF8((listCriteriaPhoto.photoPath)));
		fileGeolocalisation.Geolocalisation(&listCriteriaPhoto);
		photoCriteria.InsertPhotoListCriteria(listCriteriaPhoto, findPhotoCriteria->criteriaNew,
		                                      fileGeolocalisation.HasGps(),
		                                      findPhotoCriteria->numFolderId);
	}

	findPhotoCriteria->hasGps = fileGeolocalisation.HasGps();
	findPhotoCriteria->fromGps = true;

	if (geoloc != nullptr)
		geoloc->SendMessageToWindow(listCriteriaPhoto.photoPath, 1);

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
	CFileGeolocation geoloc(findPhotoCriteria->urlServer);
	CSqlPhotoCriteria photoCriteria;

	CListCriteriaPhoto listCriteriaPhoto;
	listCriteriaPhoto.numCatalog = NUMCATALOGID;
	listCriteriaPhoto.numPhotoId = findPhotoCriteria->numPhoto;
	listCriteriaPhoto.photoPath = findPhotoCriteria->photoPath;

	printf("FindPhotoCriteria %s \n ", CConvertUtility::ConvertToUTF8((listCriteriaPhoto.photoPath)));

	geoloc.SetFile(listCriteriaPhoto.photoPath, notGeo);

	if (!geoloc.HasGps())
	{
		printf("Has not GPS %s \n ", CConvertUtility::ConvertToUTF8((listCriteriaPhoto.photoPath)));
		auto insertCriteria = new CInsertCriteria();
		insertCriteria->type = CATEGORIE_GEO;
		insertCriteria->value = CLibResource::LoadStringFromResource(L"LBLNOTGEO", 1);
		listCriteriaPhoto.listCriteria.push_back(insertCriteria);
	}
	else
	{
		//Insert GPS info into GPS table
		CSqlPhotoGPS photoGPS;
		photoGPS.InsertPhoto(listCriteriaPhoto.numPhotoId, listCriteriaPhoto.photoPath, findPhotoCriteria->numFolderId);
	}
	findPhotoCriteria->hasGps = geoloc.HasGps();
	findPhotoCriteria->fromGps = false;

	wxString datetime = geoloc.GetDateTimeInfos();

	printf("FindPhotoCriteria %s datetime %s \n ", CConvertUtility::ConvertToUTF8(listCriteriaPhoto.photoPath),
	       CConvertUtility::ConvertToUTF8(datetime));

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

			auto insertCriteria = new CInsertCriteria();
			insertCriteria->type = CATEGORIE_DATE;
			insertCriteria->value = datetime;
			listCriteriaPhoto.listCriteria.push_back(insertCriteria);
			CListOfWindow* list_of_window = CGpsEngine::getInstance();
			if (list_of_window != nullptr)
				list_of_window->SendMessageToWindow(listCriteriaPhoto.photoPath, 2);
		}
	}


	photoCriteria.InsertPhotoListCriteria(listCriteriaPhoto, findPhotoCriteria->criteriaNew, geoloc.HasGps(),
	                                      findPhotoCriteria->numFolderId);

	for (CInsertCriteria* criteria : listCriteriaPhoto.listCriteria)
	{
		if (criteria != nullptr)
		{
			delete criteria;
			criteria = nullptr;
		}
	}

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
	//auto viewerParam = CMainParamInit::getInstance();
	//if(viewerParam != nullptr)
	//	sqlRequest = viewerParam->GetLastSqlRequest();

	if (pimpl->catalogWndOld != nullptr)
		return pimpl->catalogWndOld->GetSqlRequest();
	return sqlRequest;
}

void CCategoryFolderWindow::CriteriaPhotoUpdate(wxCommandEvent& event)
{
    
	auto findPhotoCriteria = static_cast<CFindPhotoCriteria*>(event.GetClientData());

	if (findPhotoCriteria->criteriaNew)
	{
		UpdateCriteria(true);
	}


	if (findPhotoCriteria->phthread != nullptr)
		findPhotoCriteria->phthread->join();

	if ((findPhotoCriteria->hasGps && findPhotoCriteria->fromGps) || !findPhotoCriteria->hasGps)
	{
		CSqlPhotos sqlPhoto;
		sqlPhoto.UpdatePhotoCriteria(findPhotoCriteria->numPhoto);
	}

	if (findPhotoCriteria->fromGps)
	{
		CSqlPhotoGPS photoGPS;
		photoGPS.DeletePhoto(findPhotoCriteria->numPhoto);
		pimpl->gpsLocalisationFinish = true;
	}

	if (findPhotoCriteria->phthread != nullptr)
		delete findPhotoCriteria->phthread;
	delete findPhotoCriteria;
    

    
	pimpl->numProcess--;
	pimpl->numProcess = max(pimpl->numProcess, 0);
	processIdle = true;
}
