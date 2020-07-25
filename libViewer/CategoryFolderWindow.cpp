#include <header.h>
#include "CategoryFolderWindow.h"
#include <LibResource.h>
#include "ViewerParamInit.h"
#include <ListCriteriaPhoto.h>
#include <window_id.h>
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
//#include <PictureRecognition.h>
#include <libPicture.h>
#include <FileUtility.h>
#include <ImageLoadingFormat.h>
#include <RegardsBitmap.h>
#include <SqlPhotoGPS.h>
#include <ConvertUtility.h>
#include <ThumbnailMessage.h>
using namespace std;
using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::Internet;
#define NUMCATALOGID 1
//wxDEFINE_EVENT(EVENT_CRITERIAUPDATE, wxCommandEvent);
wxDEFINE_EVENT(EVENT_CRITERIAPHOTOUPDATE, wxCommandEvent);


class CFindPhotoCriteria
{
public:
	CFindPhotoCriteria()
	{
		mainWindow = nullptr;
		criteriaNew = false;
        hasGps = false;
        fromGps = false;
		phthread = nullptr;
	};

	wxString urlServer;
	CCategoryFolderWindow * mainWindow;
	bool criteriaNew;
	int numPhoto;
    int numFolderId;
	wxString photoPath;
	thread * phthread;
    bool hasGps;
    bool fromGps;
};

CCategoryFolderWindow::CCategoryFolderWindow(wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & theme)
	: CTreeWithScrollbar("CCategoryFolderWindow", parent, id, themeScroll, theme)
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
    //nbPhotos = 0;
	//nbProcesseur =  thread::hardware_concurrency();
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
		nbProcesseur = config->GetExifProcess();

	//this->statusBarViewer = statusBarViewer;
	catalogWndOld = nullptr;

    gpsLocalisationFinish = true;
   
    
    CRegardsConfigParam * param = CParamInit::getInstance();
    if (param != nullptr)
    {
        urlServer = param->GetUrlServer();
    }
    refreshTimer = new wxTimer(this, wxTIMER_REFRESH);
    Connect(wxEVENT_CRITERIAUPDATE, wxCommandEventHandler(CCategoryFolderWindow::RefreshCriteriaSearch));
	Connect(EVENT_CRITERIAPHOTOUPDATE, wxCommandEventHandler(CCategoryFolderWindow::CriteriaPhotoUpdate));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CCategoryFolderWindow::OnIdle));
	Connect(wxTIMER_REFRESH, wxEVT_TIMER, wxTimerEventHandler(CCategoryFolderWindow::OnTimerRefresh), nullptr, this);
	update = true;
	threadDataProcess = true;
	noCategoryMessage = false;
	categoryMessage = false;
	processIdle = true;

    nbGpsFile = 0;
    //Refresh all 1 minute
    refreshTimer->Start(60000, wxTIMER_CONTINUOUS);
    refreshTimer->Start();
}

void CCategoryFolderWindow::OnTimerRefresh(wxTimerEvent& event)
{
    printf(" CCategoryFolderWindow::OnTimerRefresh %d \n", nbGpsFile);
    nbGpsFile = 0;
    processIdle = true;
}

void CCategoryFolderWindow::RefreshCriteriaSearch()
{
    if(catalogWndOld != nullptr)
        catalogWndOld->RefreshCriteriaSearch();
}

void CCategoryFolderWindow::RefreshCriteriaSearch(wxCommandEvent& event)
{
    if(catalogWndOld != nullptr)
        catalogWndOld->RefreshCriteriaSearch();
}

CCategoryFolderWindow::~CCategoryFolderWindow()
{
	if (refreshTimer->IsRunning())
		refreshTimer->Stop();

	delete(refreshTimer);	
	delete(catalogWndOld);
}

void CCategoryFolderWindow::InitSaveParameter()
{
    CMainParam * config = (CMainParam *)CMainParamInit::getInstance();
    if(config != nullptr)
    {
        config->SetCatalogOpenTriangle("");
        config->SetCatalogCriteria("");
    }
}

void CCategoryFolderWindow::Init()
{
	UpdateCriteria(false);
	update = true;
    processIdle = true;
}

void CCategoryFolderWindow::UpdateCriteria(const bool &needToSendMessage)
{
    printf("CCategoryFolderWindow::UpdateCriteria() \n");
	this->needToSendMessage = needToSendMessage;
	CWindowMain * windowMain = (CWindowMain *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (windowMain != nullptr && treeWindow != nullptr)
	{
		CCategoryWnd * catalogWnd = new CCategoryWnd(windowMain, treeWindow->GetTheme(), treeWindow);
		catalogWnd->Init();
		treeWindow->SetTreeControl(catalogWnd);
		delete(catalogWndOld);
		catalogWndOld = catalogWnd;	
		update = true;
	}
    processIdle = true;
}

bool CCategoryFolderWindow::GetProcessEnd()
{
	if(numProcess > 0  || !processEnd)
		return false;
	return true;
}

wxString CCategoryFolderWindow::GetWaitingMessage()
{
	return "Window CCategoryFolderWindow waiting : " + to_string(this->GetId()) + " - NbProcess Waiting : " + to_string(numProcess);
}

void CCategoryFolderWindow::ProcessIdle()
{
    TRACE();
    bool hasSomethingTodo = true;
    printf("CCategoryFolderWindow::ProcessIdle() \n");
	int nbPhotos;

    CSqlInsertFile sqlInsertFile;
    nbPhotos = sqlInsertFile.GetNbPhotosToProcess(); 

	CThumbnailMessage * thumbnailMessage = new CThumbnailMessage();
	thumbnailMessage->nbPhoto = nbPhotos;
	thumbnailMessage->typeMessage = 0;
	wxWindow * mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
	wxCommandEvent eventChange(wxEVENT_UPDATEMESSAGECRITERIA);
	eventChange.SetClientData(thumbnailMessage);

	if(mainWnd != nullptr)
		mainWnd->GetEventHandler()->AddPendingEvent(eventChange);
	else
		delete thumbnailMessage;
	/*
    wxString message = "Criteria Nb Image to Process :  " + to_string(nbPhotos);

    if (statusBarViewer != nullptr)
    {
        statusBarViewer->SetRangeProgressBar(nbPhotos);
        statusBarViewer->SetPosProgressBar(0);
        statusBarViewer->SetText(2, message);
    }
*/

	if (nbPhotos > 0 && numProcess < nbProcesseur && threadDataProcess)
	{
		//Put in a thread
		CSqlInsertFile sqlInsertFile;
		CFindPhotoCriteria * findPhotoCriteria = new CFindPhotoCriteria();
        CPhotos photo = sqlInsertFile.GetPhotoToProcess();
        
        printf("CCategoryFolderWindow::ProcessIdle : Nb Photo : %d Path : %s \n", nbPhotos, CConvertUtility::ConvertToUTF8(photo.GetPath()));
        
        if(photo.GetId() != -1)
        {
            findPhotoCriteria->numPhoto = photo.GetId();
            findPhotoCriteria->photoPath = photo.GetPath();  
            findPhotoCriteria->numFolderId = photo.GetFolderId();
            findPhotoCriteria->urlServer = urlServer;
            findPhotoCriteria->mainWindow = this;
            //findPhotoCriteria->numFolder = photo.GetFolderId();
            
            findPhotoCriteria->phthread = new thread(FindPhotoCriteria, findPhotoCriteria);
            numProcess++;  
            sqlInsertFile.UpdatePhotoProcess(photo.GetId());    
            traitementEnd = false;
        }
	}
	else if (!traitementEnd && threadDataProcess)
	{
		//Nettoyage des criteres non utilises
		CSqlCriteria criteria;
		criteria.DeleteCriteriaAlone();
		traitementEnd = true;
	}
	else if (refreshFolder)
	{
		wxString message;
		int counter = 0;
		CSqlFindFolderCatalog folder;
		FolderCatalogVector catalogfolderVector;
		folder.GetFolderCatalog(&catalogfolderVector, 1);

		CThumbnailMessage * thumbnailMessage = new CThumbnailMessage();
		thumbnailMessage->nbElement = catalogfolderVector.size();
		thumbnailMessage->typeMessage = 1;
		wxWindow * mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
		wxCommandEvent eventChange(wxEVENT_UPDATEMESSAGECRITERIA);
		eventChange.SetClientData(thumbnailMessage);

		if(mainWnd != nullptr)
			mainWnd->GetEventHandler()->AddPendingEvent(eventChange);
		else
			delete thumbnailMessage;
		/*
		if (statusBarViewer != nullptr)
		{
			statusBarViewer->SetRangeProgressBar((int)catalogfolderVector.size());
			statusBarViewer->SetPosProgressBar(0);
		}
		*/
		for (CFolderCatalog folder : catalogfolderVector)
		{
			counter++;
			CThumbnailMessage * thumbnailMessage = new CThumbnailMessage();
			thumbnailMessage->thumbnailPos = counter;
			thumbnailMessage->nbElement = catalogfolderVector.size();
			thumbnailMessage->typeMessage = 2;
			wxWindow * mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
			if (mainWnd != nullptr)
			{
				wxCommandEvent eventChange(wxEVENT_UPDATEMESSAGECRITERIA);
				eventChange.SetClientData(thumbnailMessage);
				mainWnd->GetEventHandler()->AddPendingEvent(eventChange);
			}
			else
				delete thumbnailMessage;
			/*
			message = "Folder Processing " + to_string(counter) + " / " + to_string(catalogfolderVector.size());
			if (statusBarViewer != nullptr)
			{
				statusBarViewer->SetText(2, message);
				statusBarViewer->SetPosProgressBar(counter);
			}
			*/
			RefreshThreadFolder(&folder);
		}

		update = true;
		refreshFolder = false;
		threadDataProcess = true;
	}
    else if (nbPhotos == 0)
        hasSomethingTodo = false;
        
	//nbProcesseur = 2;
      
    
    if(!hasSomethingTodo)
        processIdle = false;
        
    //---------------------------------------------------------------------------------------------------------------
    //GPS Traitement
    //Thread by Thread
    //---------------------------------------------------------------------------------------------------------------
    int numPhoto = 0;
    int numFolderId = 0;
    wxString photoPath = "";
    CSqlPhotoGPS photoGPS; 
    if(photoGPS.GetFirstPhoto(numPhoto, photoPath, numFolderId) > 0)
    {
        int nbGpsFileByMinute = 60;
        printf("Geolocalize File photoGPS.GetFirstPhoto nbGPSFile : %d \n", nbGpsFile);
        CRegardsConfigParam * param = CParamInit::getInstance();
        if (param != nullptr)
            nbGpsFileByMinute = param->GetNbGpsIterationByMinute();
         
        if(gpsLocalisationFinish && nbGpsFile < nbGpsFileByMinute)
        {
            
            CFindPhotoCriteria * findPhotoCriteria = new CFindPhotoCriteria();
            findPhotoCriteria->urlServer = urlServer;
            findPhotoCriteria->mainWindow = this;
            findPhotoCriteria->numPhoto = numPhoto;
            findPhotoCriteria->photoPath = photoPath;
            findPhotoCriteria->numFolderId = numFolderId;
            findPhotoCriteria->phthread = new thread(FindGPSPhotoCriteria, findPhotoCriteria);    
            gpsLocalisationFinish = false;    
            nbGpsFile++;
            processIdle = true; 
        }
        //processIdle = true; 
    } 


}


void CCategoryFolderWindow::OnIdle(wxIdleEvent& evt)
{
    //TRACE();
   //if (processIdle && !endProgram)
     //   ProcessIdle();
    
    if(endProgram)
    {
        if (refreshTimer->IsRunning())
            refreshTimer->Stop();
    }
     
	StartThread();
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
		UpdateCriteria(true);
        processIdle = true;
	}
    
    
}

void CCategoryFolderWindow::FindGPSPhotoCriteria(CFindPhotoCriteria * findPhotoCriteria)
{
	CFileGeolocation geoloc(findPhotoCriteria->urlServer);
	CSqlPhotoCriteria photoCriteria;
    

	CListCriteriaPhoto listCriteriaPhoto;

	listCriteriaPhoto.numCatalog = NUMCATALOGID;
	listCriteriaPhoto.numPhotoId = findPhotoCriteria->numPhoto;
	listCriteriaPhoto.photoPath = findPhotoCriteria->photoPath;

    printf("FindGPSPhotoCriteria %s \n ", CConvertUtility::ConvertToUTF8(listCriteriaPhoto.photoPath));

	geoloc.SetFile(listCriteriaPhoto.photoPath, true);


	if (geoloc.HasGps())
	{
        printf("Has GPS %s \n ", CConvertUtility::ConvertToUTF8((listCriteriaPhoto.photoPath)));
		geoloc.Geolocalisation(&listCriteriaPhoto);
        photoCriteria.InsertPhotoListCriteria(listCriteriaPhoto, findPhotoCriteria->criteriaNew, geoloc.HasGps(), findPhotoCriteria->numFolderId);
	}
    
    findPhotoCriteria->hasGps = geoloc.HasGps();
    findPhotoCriteria->fromGps = true;    

	if (findPhotoCriteria->mainWindow != nullptr)
	{
		wxCommandEvent * event = new wxCommandEvent(EVENT_CRITERIAPHOTOUPDATE);
		event->SetClientData(findPhotoCriteria);
		wxQueueEvent(findPhotoCriteria->mainWindow, event);
	}

	
}

//Thread
void CCategoryFolderWindow::FindPhotoCriteria(CFindPhotoCriteria * findPhotoCriteria)
{
    
	CFileGeolocation geoloc(findPhotoCriteria->urlServer);
	CSqlPhotoCriteria photoCriteria;

	CListCriteriaPhoto listCriteriaPhoto;
	listCriteriaPhoto.numCatalog = NUMCATALOGID;
	listCriteriaPhoto.numPhotoId = findPhotoCriteria->numPhoto;
	listCriteriaPhoto.photoPath = findPhotoCriteria->photoPath;

    printf("FindPhotoCriteria %s \n ", CConvertUtility::ConvertToUTF8((listCriteriaPhoto.photoPath)));

	geoloc.SetFile(listCriteriaPhoto.photoPath, true);

	if (!geoloc.HasGps())
	{
        printf("Has not GPS %s \n ", CConvertUtility::ConvertToUTF8((listCriteriaPhoto.photoPath)));
		CInsertCriteria * insertCriteria = new CInsertCriteria();
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
	vector<wxString> vDateTime;
    
    printf("FindPhotoCriteria %s datetime %s \n ", CConvertUtility::ConvertToUTF8(listCriteriaPhoto.photoPath), CConvertUtility::ConvertToUTF8(datetime));

	if (datetime.Length() > 10)
	{
		datetime = datetime.substr(0, 10);
		wxChar separator = datetime[4];
		vDateTime = CConvertUtility::split(datetime, separator);

		if (vDateTime.size() == 3)
		{
			datetime = "";

			for (auto i = 0; i < vDateTime.size(); i++)
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


	photoCriteria.InsertPhotoListCriteria(listCriteriaPhoto, findPhotoCriteria->criteriaNew, geoloc.HasGps(), findPhotoCriteria->numFolderId);


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
		UpdateCriteria(true);
       
	}




    if(findPhotoCriteria->phthread != nullptr)
        findPhotoCriteria->phthread->join();

    if((findPhotoCriteria->hasGps && findPhotoCriteria->fromGps) || !findPhotoCriteria->hasGps)
    {
        CSqlPhotos sqlPhoto;
        sqlPhoto.UpdatePhotoCriteria(findPhotoCriteria->numPhoto);
    }
    
    if(findPhotoCriteria->fromGps)
    {
        CSqlPhotoGPS photoGPS; 
        photoGPS.DeletePhoto(findPhotoCriteria->numPhoto);
        gpsLocalisationFinish = true;
    }

    if(findPhotoCriteria->phthread != nullptr)
        delete findPhotoCriteria->phthread;
	delete findPhotoCriteria;

	numProcess--;
    processIdle = true;
    
}
