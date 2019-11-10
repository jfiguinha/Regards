#include <header.h>
#include "MainWindow.h"
#include <LibResource.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <PrintEngine.h>
#include <libPicture.h>
#include "ThemeParam.h"
#include "MainThemeInit.h"
#include "MainTheme.h"
#include "SqlFindPhotos.h"
#include <ImageLoadingFormat.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include "ImageList.h"
#include <ConvertUtility.h>
#include <window_id.h>
#include <FilterData.h>
#include <SqlFindFolderCatalog.h>
#include <wxSVG/SVGDocument.h>
#include "VideoFilter.h"
#include <SQLRemoveData.h>
#include <SqlFolderCatalog.h>
#include <SqlInsertFile.h>
#include <ExportFile.h>
#include <SqlFindCriteria.h>
#include <ShowVideo.h>
#include <StatusText.h>
#include "PictureElement.h"
#include <ThumbnailMessage.h>
#include <SqlThumbnailVideo.h>
#include "FaceInfosUpdate.h"
#include "CheckVersion.h"
#include <wx/mimetype.h>
#include <PictureRecognition.h>
//#include <jpge.h>
//using namespace jpge;
using namespace Regards::Viewer;
//const long ONE_MSEC = 1000;
using namespace std;
using namespace Regards::Sqlite;


bool firstTime = true;

class CThreadMD5
{
public:

	CThreadMD5()
	{
		thread = nullptr;
		mainWindow = nullptr;
	};
	~CThreadMD5() {
	};

	wxString filename;
	std::thread * thread;
	wxWindow * mainWindow;
};


wxDEFINE_EVENT(wxEVENT_SETSCREEN, wxCommandEvent);

CMainWindow::CMainWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar)
	: CWindowMain("CMainWindow",parent, id)
{
	sendMessageVideoStop = false;
	loadPicture = true;
	fullscreen = false;
	startDiaporama = false;
	nbProcessMD5 = 0;
	numElement = 0;
	showToolbar = true;
    multithread = true;
    needToReload = false;
	typeAffichage = SHOW_ALL;
	updateCriteria = true;
	updateFolder = true;
	updatePicture = false;
	refreshFolder = false;
	numElementTraitement = 0;
	start = true;
	criteriaSendMessage = false;
	videoStart = false;
	checkVersion = true;
    imageList = new CImageList();
    PhotosVector pictures;
	CSqlFindPhotos sqlFindPhotos;
	sqlFindPhotos.SearchPhotos(&pictures);
    imageList->SetImageList(pictures);
    
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();
	viewerParam = (CMainParam *)CMainParamInit::getInstance();
	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetMainToolbarTheme(&theme);
		toolbar = new CToolbar(this, wxID_ANY, theme, false);
	}

	//refreshTimer = new wxTimer(this, wxTIMER_REFRESH);
	diaporamaTimer = new wxTimer(this, wxTIMER_DIAPORAMA);

	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetSplitterTheme(&theme);
		centralWnd = new CCentralWindow(this, CENTRALVIEWERWINDOWID, theme, imageList, false);
	}
	this->statusBarViewer = statusbar;


	Connect(wxEVENT_FACEINFOSUPDATESTATUSBAR, wxCommandEventHandler(CMainWindow::OnFaceInfosStatusBarUpdate));
	Connect(wxEVENT_FACEINFOSUPDATE, wxCommandEventHandler(CMainWindow::OnFaceInfosUpdate));
	Connect(wxTIMER_DIAPORAMA, wxEVT_TIMER, wxTimerEventHandler(CMainWindow::OnTimerDiaporama), nullptr, this);
	//Connect(wxTIMER_REFRESH, wxEVT_TIMER, wxTimerEventHandler(CMainWindow::OnTimerRefresh), nullptr, this);
	Connect(wxEVENT_SETSCREEN, wxCommandEventHandler(CMainWindow::SetScreenEvent));
	Connect(wxEVENT_INFOS, wxCommandEventHandler(CMainWindow::OnUpdateInfos));
	Connect(EVENT_ENDNEWPICTURETHREAD, wxCommandEventHandler(CMainWindow::OnEndPictureLoad));
	Connect(wxEVENT_CRITERIASHOWUPDATE, wxCommandEventHandler(CMainWindow::RefreshCriteriaPictureList));
	Connect(TOOLBAR_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnShowToolbar));
	Connect(VIDEO_END_ID, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnVideoEnd));
    Connect(VIDEO_START, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnVideoStart));
	Connect(wxEVENT_ADDFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnAddFolder));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CMainWindow::OnIdle));
	Connect(wxEVENT_REMOVEFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnRemoveFolder));
	Connect(wxEVENT_ONPICTURECLICK, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnPictureClick));
    Connect(wxEVT_CRITERIACHANGE, wxCommandEventHandler(CMainWindow::CriteriaChange));
	Connect(wxEVENT_PICTUREVIDEOCLICK, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::PictureVideoClick));
	Connect(wxEVENT_REFRESHFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::InitPictures));
    Connect(wxEVENT_REFRESHPICTURE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnRefreshPicture));
	Connect(wxEVENT_MD5CHECKING, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::Md5Checking));
	//Connect(wxTIMER_REFRESHTIMERSTART, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::RefreshTimer));
    Connect(wxTIMER_DIAPORAMATIMERSTART, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::StartDiaporamaMessage));
	Connect(wxEVENT_SETSTATUSTEXT, wxCommandEventHandler(CMainWindow::OnStatusSetText));
	Connect(wxEVT_EXIT, wxCommandEventHandler(CMainWindow::OnExit));
	Connect(wxEVENT_SETRANGEPROGRESSBAR, wxCommandEventHandler(CMainWindow::OnSetRangeProgressBar));
	Connect(wxEVENT_PRINTPICTURE, wxCommandEventHandler(CMainWindow::PrintPreview));
	Connect(wxEVENT_UPDATEMESSAGETHUMBNAIL, wxCommandEventHandler(CMainWindow::UpdateThumbnailMessage));
	Connect(wxEVENT_UPDATEMESSAGECRITERIA, wxCommandEventHandler(CMainWindow::UpdateCriteriaMessage));
	Connect(wxEVENT_PRINT, wxCommandEventHandler(CMainWindow::OnPrint));
	Connect(wxEVENT_SETVALUEPROGRESSBAR, wxCommandEventHandler(CMainWindow::OnSetValueProgressBar));
    Connect(wxEVT_ANIMATIONTIMERSTOP, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::StopAnimation));
	statusBar = new wxStatusBar(this, wxID_ANY, wxSTB_DEFAULT_STYLE, "wxStatusBar");

	int tabWidth[] = { 100, 300, 300, 300 };
	statusBar->SetFieldsCount(4);
	statusBar->SetStatusWidths(4, tabWidth);

	progressBar = new wxGauge(statusBar, wxID_ANY, 200, wxPoint(1000, 0), wxSize(200, statusBar->GetSize().y),
		wxGA_HORIZONTAL);
	progressBar->SetRange(100);
	progressBar->SetValue(50);

	//refreshTimer->Start(300000, wxTIMER_ONE_SHOT);
    //diaporamaTimer->Start(2000, wxTIMER_ONE_SHOT);
	updatePicture = true;

	refreshFolder = true;
    if(imageList->GetNbElement() > 0)
        processIdle = true;
    else
        processIdle = false;

	updateFolder = true;
	processIdle = true;


}

void CMainWindow::OnPrint(wxCommandEvent& event)
{
	CLibPicture libPicture;
	wxString filename = GetFilename();
	if (filename != "")
	{
		CImageLoadingFormat * image = libPicture.LoadPicture(filename);
		if (image != nullptr)
		{
			statusBarViewer->PrintPreview(image);
		}
	}
}

wxString CMainWindow::GetFilename()
{
	return filename;
}

void CMainWindow::UpdateThumbnailMessage(wxCommandEvent& event)
{
	TRACE();
	CThumbnailMessage * thumbnailMessage = (CThumbnailMessage *)event.GetClientData();
	if (thumbnailMessage != nullptr)
	{
		wxString picture = CLibResource::LoadStringFromResource(L"LBLPICTURERENDER", 1);
		wxString message = picture + to_string(thumbnailMessage->nbPhoto);
		if (statusBarViewer != nullptr)
		{
			statusBarViewer->SetRangeProgressBar(thumbnailMessage->nbElement);
			statusBarViewer->SetText(2, message);
			statusBarViewer->SetPosProgressBar(thumbnailMessage->thumbnailPos + 1);
		}

		delete thumbnailMessage;
	}
}

void CMainWindow::UpdateCriteriaMessage(wxCommandEvent& event)
{
	TRACE();
	CThumbnailMessage * thumbnailMessage = (CThumbnailMessage *)event.GetClientData();
	if (thumbnailMessage != nullptr)
	{
		switch (thumbnailMessage->typeMessage)
		{
			case 0:
			{
				wxString picture = CLibResource::LoadStringFromResource(L"LBLCRITERIANBIMAGE", 1);
				wxString message = picture + to_string(thumbnailMessage->nbPhoto);

				if (statusBarViewer != nullptr)
				{
					statusBarViewer->SetRangeProgressBar(thumbnailMessage->nbPhoto);
					statusBarViewer->SetPosProgressBar(0);
					statusBarViewer->SetText(2, message);
				}
			}
			break;

			case 1:
			{
				if (statusBarViewer != nullptr)
				{
					statusBarViewer->SetRangeProgressBar(thumbnailMessage->nbElement);
					statusBarViewer->SetPosProgressBar(0);
				}
			}
			break;

			case 2:
				{
					wxString picture = CLibResource::LoadStringFromResource(L"LBLFOLDERPROCESSING", 1);
					wxString message = picture + to_string(thumbnailMessage->thumbnailPos) + " / " + to_string(thumbnailMessage->nbPhoto);
					if (statusBarViewer != nullptr)
					{
						statusBarViewer->SetText(2, message);
						statusBarViewer->SetPosProgressBar(thumbnailMessage->thumbnailPos);
					}
				}
				break;
		}
		delete thumbnailMessage;
	}
}




void CMainWindow::StopAnimation(wxCommandEvent& event)
{
    TRACE();
    if (startDiaporama)
    {
        int timeDelai = viewerParam->GetDelaiDiaporamaOption();
        diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
    }     
}

void CMainWindow::PrintPreview(wxCommandEvent& event)
{
	
	CRegardsBitmap * bitmap = (CRegardsBitmap *)event.GetClientData();
	
	if (bitmap != nullptr)
	{
		statusBarViewer->PrintImagePreview(bitmap);
	}
}

void CMainWindow::RefreshCriteriaPictureList(wxCommandEvent& event)
{
    TRACE();
	updateFolder = true;
    processIdle = true;
}

void CMainWindow::OnFacePertinence()
{
    TRACE();
	updateFolder = true;
    processIdle = true;

}
/*
void CMainWindow::OnTimerRefresh(wxTimerEvent& event)
{
    TRACE();
	refreshFolder = true;
    processIdle = true;
	refreshTimer->Stop();
}
*/
void CMainWindow::OnEndPictureLoad(wxCommandEvent& event)
{
	wxString * threadFilename = (wxString *)event.GetClientData();
	if (startDiaporama)
	{
		int timeDelai = viewerParam->GetDelaiDiaporamaOption();
		diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
	}

	if (*threadFilename != firstFileToShow)
	{
		updatePicture = true;
		processIdle = true;
	}

	if (threadFilename != nullptr)
		delete threadFilename;
}

void CMainWindow::CriteriaChange(wxCommandEvent& event)
{
    TRACE();
    //Refresh Criteria
	wxWindow * window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
	if (window)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_UPDATECRITERIA);
		evt.SetExtraLong(1);
		window->GetEventHandler()->AddPendingEvent(evt);
	}

	//Update Picture Info
	window = this->FindWindowById(PANELINFOSWNDID);
	if (window)
	{
		CPanelInfosWnd * panelInfo = (CPanelInfosWnd *)window;
		panelInfo->UpdateData();
	}
    processIdle = true;
}


void CMainWindow::OnVideoStart(wxCommandEvent& event)
{
    TRACE();
    //centralWnd->StopLoadingPicture();
	videoStart = true;
}

void CMainWindow::OnShowToolbar(wxCommandEvent& event)
{
    TRACE();
	ShowToolbar();
}

void CMainWindow::UpdateScreenRatio()
{
    TRACE();
    toolbar->UpdateScreenRatio();
    centralWnd->UpdateScreenRatio();
    this->Resize();
}

void CMainWindow::OnStatusSetText(wxCommandEvent& event)
{
    TRACE();
	CStatusText * statusText = (CStatusText *)event.GetClientData();
	if(statusText != nullptr)
	{
		statusBar->SetStatusText(statusText->text, statusText->position);
		delete statusText;
	}
}


void CMainWindow::OnSetValueProgressBar(wxCommandEvent& event)
{
    TRACE();
    
	int position = event.GetInt();
    //cout << "OnSetValueProgressBar Pos : " << position << endl;
	if (progressBar != nullptr)
    {
        if(progressBar->GetRange() > 0)
        {
            if(position >= progressBar->GetRange())
                progressBar->SetValue(progressBar->GetRange() - 1);
            else
                progressBar->SetValue(position);
        
            progressBar->Refresh();
        }
        
    }
}

void CMainWindow::OnSetRangeProgressBar(wxCommandEvent& event)
{
    TRACE();
	int range = event.GetInt();
   // cout << "OnSetRangeProgressBar Pos : " << range << endl;
	if (progressBar != nullptr)
		progressBar->SetRange(range);
}

void CMainWindow::SetText(const int &numPos, const wxString &libelle)
{
    TRACE();
    wxCommandEvent * event = new wxCommandEvent(wxEVENT_SETSTATUSTEXT);
	CStatusText * statusText = new CStatusText();
	statusText->position = numPos;
	statusText->text = libelle;
    event->SetClientData(statusText);
    wxQueueEvent(this, event);

	//statusBar->SetStatusText(libelle, numPos);
}

void CMainWindow::SetRangeProgressBar(const int &range)
{
    TRACE();
    wxCommandEvent * event = new wxCommandEvent(wxEVENT_SETRANGEPROGRESSBAR);
    event->SetInt(range);
    wxQueueEvent(this, event);
}

void CMainWindow::SetPosProgressBar(const int &position)
{
    TRACE();
    wxCommandEvent * event = new wxCommandEvent(wxEVENT_SETVALUEPROGRESSBAR);
    event->SetInt(position);
    wxQueueEvent(this, event);
}
void CMainWindow::StartDiaporamaMessage(wxCommandEvent& event)
{
    TRACE();
    int timeDelai = viewerParam->GetDelaiDiaporamaOption();
    diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
}

wxString GetFileName(const wxString &nameFile)
{
    TRACE();
	wxString folder = CFileUtility::GetDocumentFolderPath();

	#ifdef WIN32
		folder.append("\\face");
	#else
		folder.append("/face");
	#endif

	if (!wxDir::Exists(folder))
	{
		wxFileName::Mkdir(folder);
	}

	#ifdef WIN32
		folder.append("\\" + nameFile);
	#else
		folder.append("/"  + nameFile);
	#endif

	return folder;
}

/*
void CMainWindow::RefreshTimer(wxCommandEvent& event)
{
    TRACE();
    refreshTimer->Start();
}
*/

bool CMainWindow::IsVideoStart()
{
    return videoStart;
}

void CMainWindow::ProcessIdle()
{
    TRACE();
	bool hasDoneOneThings = false;
	int nbProcesseur = 1;

	/*
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
		nbProcesseur = config->GetFaceProcess();

	wxWindow * window = this->FindWindowById(LISTFACEID);
	if (window)
	{
		wxCommandEvent evt(wxEVENT_REFRESHDATA);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
	*/
    
    printf("ProcessIdle : updateCriteria : %d refreshFolder : %d updateFolder : %d updatePicture %d\n", updateCriteria, refreshFolder, updateFolder, updatePicture);
    
    if(checkVersion)
    {
        wxString localVersion = CLibResource::LoadStringFromResource("REGARDSVERSION",1);
        wxString serverURL = CLibResource::LoadStringFromResource("ADRESSEWEBVERSION",1);
        CCheckVersion _checkVersion(serverURL);
        wxString serverVersion = _checkVersion.GetLastVersion();
        serverVersion = serverVersion.SubString(0, serverVersion.length() - 2);
        if(serverVersion != "error" && serverVersion != "")
        {
            if(localVersion != serverVersion )
            {
                wxString information = CLibResource::LoadStringFromResource("LBLINFORMATIONS",1);
                wxString newVersionAvailable = CLibResource::LoadStringFromResource("LBLNEWVERSIONAVAILABLE",1);
                
                int answer = wxMessageBox(newVersionAvailable, information, wxYES_NO | wxCANCEL, this);
                if (answer == wxYES)
                {
                    wxString siteweb = CLibResource::LoadStringFromResource("SITEWEB",1);
                    wxMimeTypesManager manager;
                    wxFileType *filetype=manager.GetFileTypeFromExtension("html");
                    wxString command=filetype->GetOpenCommand(siteweb);
                    wxExecute(command);
                }
            }
        }
        checkVersion = false;
    }
    
	if (updateCriteria)
	{
		wxWindow * window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
		if (window)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_UPDATECRITERIA);
			evt.SetExtraLong((criteriaSendMessage == true) ? 1 : 0);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
		criteriaSendMessage = false;
		updateCriteria = false;
        hasDoneOneThings = true;
	}
	else if (refreshFolder)
	{
		//refreshTimer->Stop();
		FolderCatalogVector folderList;
		CSqlFindFolderCatalog folderCatalog;
		folderCatalog.GetFolderCatalog(&folderList, NUMCATALOGID);

		bool folderChange = false;

		//Test de la validité des répertoires
		for (CFolderCatalog folderlocal : folderList)
		{
			if (!wxDirExists(folderlocal.GetFolderPath()))
			{
				//Remove Folder
				CSQLRemoveData::DeleteFolder(folderlocal.GetNumFolder());
				folderChange = true;
			}
		}

		//Test de la validité des fichiers
		//PhotosVector photoList;
		//CSqlFindPhotos findphotos;
		//findphotos.GetAllPhotos(&photoList);
        for(int i = 0;i < imageList->GetNbElement();i++)
        {
            bool isValid = true;
            CPhotos photo = imageList->GetElement(i, isValid);
            if(isValid)
            {
                if (!wxFileExists(photo.GetPath()))
                {
                    //Remove Folder
                    CSQLRemoveData::DeletePhoto(photo.GetId());
                    folderChange = true;
                }
            }
        }
		

		int nbFile = 0;
		//Test de la validité des répertoires

		for (CFolderCatalog folderlocal : folderList)
        {
			wxString fichier;
			CSqlInsertFile sqlInsertFile;
			nbFile += sqlInsertFile.ImportFileFromFolder(folderlocal.GetFolderPath(), folderlocal.GetNumFolder(), fichier);
		}

		if (folderChange || nbFile > 0)
		{
			CMainParam * viewerParam = (CMainParam *)CMainParamInit::getInstance();
			wxString sqlRequest = viewerParam->GetLastSqlRequest();

			CSqlFindPhotos sqlFindPhotos;
			sqlFindPhotos.SearchPhotos(sqlRequest);

			updateFolder = true;
			updateCriteria = true;
		}
		refreshFolder = false;
        //wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxTIMER_REFRESHTIMERSTART);
		//this->GetEventHandler()->AddPendingEvent(evt);
        hasDoneOneThings = true;
        numElementTraitement = 0;
	}
	else if (updateFolder)
	{
        wxString requestSql = "";
        PhotosVector pictures;
		CSqlFindPhotos sqlFindPhotos;
		bool isValid = true;
		//wxString photoName = imageList->GetFilePath(numElement, isValid);
        CCategoryFolderWindow * categoryFolder = (CCategoryFolderWindow *)this->FindWindowById(CATEGORYFOLDERWINDOWID);
        if(categoryFolder != nullptr)
            requestSql = categoryFolder->GetSqlRequest();
            
		if (requestSql != "")
		{
			sqlFindPhotos.SearchPhotos(requestSql);
			sqlFindPhotos.SearchPhotosByTypeAffichage(&pictures, typeAffichage, NUMCATALOGID);

			//if (imageList->GetNbElement() == 0)
			//	sqlFindPhotos.SearchPhotos(&pictures);
		}
		else
			sqlFindPhotos.SearchPhotos(&pictures);
            
        imageList->SetImageList(pictures);
		numElement = 0;
		if (isValid)
		{
			int position = 0;
			bool isFound = false;
			//Recherche du numElement en cours
			for (CPhotos photo : pictures)
			{
				if (filename == photo.GetPath())
				{
					isFound = true;
					break;
				}
				position++;
			}

			if (isFound)
				numElement = position;
		}

		wxWindow * window = this->FindWindowById(CENTRALVIEWERWINDOWID);
		if (window)
		{
			wxCommandEvent evt(wxEVENT_SETLISTPICTURE);
			evt.SetClientData(imageList);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		updateFolder = false;
		updatePicture = true;

        hasDoneOneThings = true;
	}
	else if (updatePicture)
	{
		if (imageList->GetNbElement() > 0)
		{
            bool isValid = true;
			if (filename != L"")
			{
				imageList->FindFileIndex(filename);
				firstFileToShow = filename;
			}
			else
				firstFileToShow = imageList->GetFilePath(numElement, isValid);



			if (isValid)
			{
				if (firstFileToShow != L"")
				{
					wxWindow * viewerWindow = this->FindWindowById(VIEWERPICTUREWND);
					wxCommandEvent evt(wxEVENT_LOADPICTURE);
					CPictureElement * pictureElement = new CPictureElement();
					pictureElement->filename = firstFileToShow;
					pictureElement->numElement = numElement;
					evt.SetClientData(pictureElement);
					viewerWindow->GetEventHandler()->AddPendingEvent(evt);
					filename = firstFileToShow;
				}
			}
			//numElementTraitement = 0;
		}
		updatePicture = false;
        hasDoneOneThings = true;
	}
	else if(loadPicture)
	{
        if(!videoStart)
        {
            bool isValid = false;
            wxString photoName = imageList->GetFilePath(numElement, isValid);
			if (isValid)
			{
				if (firstFileToShow != photoName)
				{
					firstFileToShow = photoName;
					this->filename = photoName;
					wxWindow * viewerWindow = this->FindWindowById(VIEWERPICTUREWND);
					wxCommandEvent evt(wxEVENT_LOADPICTURE);
					CPictureElement * pictureElement = new CPictureElement();
					pictureElement->filename = firstFileToShow;
					pictureElement->numElement = numElement;
					evt.SetClientData(pictureElement);
					viewerWindow->GetEventHandler()->AddPendingEvent(evt);
				}
				this->filename = photoName;
				loadPicture = false;
				sendMessageVideoStop = false;
			}
        }
        else if(videoStart && !sendMessageVideoStop)
        {
            CShowVideo * showVideoWindow = (CShowVideo *)this->FindWindowById(SHOWVIDEOVIEWERID);
            if (showVideoWindow != nullptr)
                showVideoWindow->StopVideo();

            sendMessageVideoStop = true;
        }
        hasDoneOneThings = true;
    }
	else if (numElementTraitement < imageList->GetNbElement())
	{
        bool isValid = false;
        CPhotos photo = imageList->GetElement(numElementTraitement,isValid);//pictures.at(numElementTraitement);
        if(isValid)
        {
            if (wxFileName::FileExists(photo.GetPath()))
            {
                //Test si thumbnail valide
                CMainParam * config = CMainParamInit::getInstance();
                if (config != nullptr)
                {
                    if (config->GetCheckThumbnailValidity() && nbProcessMD5 < nbProcesseur)
                    {
                        CThreadMD5 * path = new CThreadMD5();
                        path->filename = photo.GetPath();
                        path->mainWindow = this;
                        path->thread = new thread(CheckMD5, path);
                        nbProcessMD5++;
                    }
                    else
                        numElementTraitement++;
                }

            }
            else
            {
                //Remove file
                CSQLRemoveData::DeletePhoto(photo.GetId());
                updateCriteria = true;
                updateFolder = true;
                numElementTraitement++;
            }            
        }


        printf("ProcessIdle : %d \n", numElementTraitement);
		wxString label = CLibResource::LoadStringFromResource(L"LBLFILECHECKING", 1);
        wxString message = label+ to_string(numElementTraitement) + L"/" + to_string(imageList->GetNbElement());
        if (statusBarViewer != nullptr)
        {
            statusBarViewer->SetText(3, message);
        }   

        hasDoneOneThings = true;
	}
    
    
        
        
	//muPictureList.unlock();

    if(!hasDoneOneThings)
        processIdle = false;
        
 
}

void CMainWindow::OnIdle(wxIdleEvent& evt)
{
    //TRACE();
    if(endProgram)
    {
        if (diaporamaTimer->IsRunning())
            diaporamaTimer->Stop();

        //if (refreshTimer->IsRunning())
       //     refreshTimer->Stop();
    }    
    

        StartThread();
  // if (processIdle && !endProgram)
  //  ProcessIdle();
}

void CMainWindow::Md5Checking(wxCommandEvent& event)
{
    TRACE();
	CThreadMD5 * path = (CThreadMD5 *)event.GetClientData();
	if (path != nullptr)
	{
		if (path->thread != nullptr)
		{
			path->thread->join();
			delete(path->thread);
			path->thread = nullptr;
		}
		delete path;
	}
	nbProcessMD5--;
	numElementTraitement++;
    printf("Md5Checking \n");
	wxString label = CLibResource::LoadStringFromResource(L"LBLFILECHECKING", 1);
	wxString message = label + to_string(numElementTraitement) + L"/" + to_string(imageList->GetNbElement());
	if (statusBarViewer != nullptr)
	{
		statusBarViewer->SetText(3, message);
	}
}

void CMainWindow::CheckMD5(void * param)
{
    TRACE();
	CThreadMD5 * path = (CThreadMD5 *)param;
	if (path != nullptr)
	{
		CSqlThumbnail sqlThumbnail;
		CSqlThumbnailVideo sqlThumbnailVideo;
		//wxString md5file = md5.GetFileMD5(path->filename);
		wxFileName file(path->filename);
		wxULongLong sizeFile = file.GetSize();
		wxString md5file = sizeFile.ToString();

		bool result = sqlThumbnail.TestThumbnail(path->filename, md5file);
		if (!result)
		{
			//Remove thumbnail
			sqlThumbnail.DeleteThumbnail(path->filename);
			sqlThumbnailVideo.DeleteThumbnail(path->filename);
		}

		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_MD5CHECKING);
		evt.SetClientData(path);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}


}

void CMainWindow::SetSelectFile(const wxString &filename)
{
    TRACE();
	this->filename = filename;
    //centralWnd->HidePanel();
}

void CMainWindow::OnTimerDiaporama(wxTimerEvent& event)
{
    TRACE();
	ImageSuivante();
}

void CMainWindow::OnVideoEnd(wxCommandEvent& event)
{
    TRACE();
	videoStart = false;
	if(startDiaporama)
		ImageSuivante();

}

CMainWindow::~CMainWindow()
{
    TRACE();
	bool showInfos;
	bool showThumbnail;

	CViewerWindow * viewerWindow = (CViewerWindow *)this->FindWindowById(VIEWERPICTUREWND);
	if (viewerWindow != nullptr)
	{
		showInfos = viewerWindow->IsPanelInfosVisible();
		showThumbnail = viewerWindow->IsPanelThumbnailVisible();
	}

	if (viewerParam != nullptr)
	{
		viewerParam->SetShowInfos(showInfos);
		viewerParam->SetShowThumbnail(showThumbnail);
        wxRect rc = this->GetWindowRect();
		viewerParam->SetPositionWindow(rc);
	}

	if (diaporamaTimer->IsRunning())
		diaporamaTimer->Stop();

	delete(diaporamaTimer);

	//if (refreshTimer->IsRunning())
	//	refreshTimer->Stop();

	//delete(refreshTimer);
	delete(centralWnd);
	delete(toolbar);
}

void CMainWindow::Resize()
{
    TRACE();
	if (!fullscreen)
	{
		wxRect rcAffichageBitmap;
		wxSize sizeStatusBar = statusBar->GetSize();

		rcAffichageBitmap.x = 0;
		rcAffichageBitmap.y = toolbar->GetNavigatorHeight();
		rcAffichageBitmap.width = GetWindowWidth();
		rcAffichageBitmap.height = GetWindowHeight() - toolbar->GetNavigatorHeight() - sizeStatusBar.y;

		toolbar->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, toolbar->GetNavigatorHeight());
		centralWnd->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.y, rcAffichageBitmap.width, rcAffichageBitmap.height);
		statusBar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.y + rcAffichageBitmap.height, rcAffichageBitmap.width, sizeStatusBar.y);
	}
	else
	{
		centralWnd->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
	}
}


void CMainWindow::AddFolder(const wxString &folder)
{
    TRACE();
	//Indication d'imporation des critères 
	CSqlFolderCatalog sqlFolderCatalog;
	int64_t idFolder = sqlFolderCatalog.GetFolderCatalogId(NUMCATALOGID, folder);
    
    printf("AddFolder : %s \n",CConvertUtility::ConvertToUTF8(folder));

	if (idFolder == -1)
	{
		idFolder = sqlFolderCatalog.GetOrInsertFolderCatalog(NUMCATALOGID, folder);
		//Insert la liste des photos dans la base de données.
		CSqlInsertFile sqlInsertFile;
		sqlInsertFile.AddFileFromFolder(folder, idFolder, filename);
	}
    
    statusBarViewer->AddFSEntry(folder);

	wxWindow * window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
	if (window)
	{
		wxString * newFolder = new wxString(folder);
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SETFOLDER);
		evt.SetClientData(newFolder);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CMainWindow::OnAddFolder(wxCommandEvent& event)
{
    TRACE();
 	wxString * info = (wxString *)event.GetClientData();
    
    printf("OnAddFolder : %s \n",CConvertUtility::ConvertToUTF8(*info));
    
	if (*info != "")
	{
		AddFolder(*info);
		updateCriteria = true;
		updateFolder = true;
		criteriaSendMessage = true;
	}
	delete info;
    
    processIdle = true;
}


void CMainWindow::OnRemoveFolder(wxCommandEvent& event)
{
    TRACE();
	wxString * info = (wxString *)event.GetClientData();
	if (*info != "")
	{
		//Indication d'imporation des critères 
		CSqlFolderCatalog sqlFolderCatalog;
		int64_t idFolder = sqlFolderCatalog.GetFolderCatalogId(NUMCATALOGID, *info);
		if (idFolder != -1)
		{
			CSqlInsertFile sqlInsertFile;
			sqlInsertFile.RemovePhotos(idFolder);

			CSQLRemoveData sqlRemoveData;
			sqlRemoveData.DeleteFolder(idFolder);

			updateCriteria = true;
			updateFolder = true;
			criteriaSendMessage = true;
            
            wxString dir = wxString(*info);
            statusBarViewer->RemoveFSEntry(dir);
		}
	}
	delete info;
    
    processIdle = true;
}

void CMainWindow::StartDiaporama()
{
    TRACE();
	
	if (viewerParam != nullptr)
	{
		bool isValid = false;
        wxString fileToLoad = imageList->GetFilePath(numElement,isValid);
		if (isValid)
		{
			//Test is video

			CLibPicture libPicture;
			if (!libPicture.TestIsVideo(fileToLoad))
			{
				int timeDelai = viewerParam->GetDelaiDiaporamaOption();
				diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
			}
		}
	}

	startDiaporama = true;
}


void CMainWindow::PictureVideoClick(wxCommandEvent& event)
{
    TRACE();
	long timePosition = event.GetExtraLong();
	CViewerWindow * viewerWindow = (CViewerWindow *)this->FindWindowById(VIEWERPICTUREWND);
	if (viewerWindow != nullptr)
	{
		viewerWindow->SetPosition(timePosition);
	}
}

void CMainWindow::OnPictureClick(wxCommandEvent& event)
{
    TRACE();
	int photoId = event.GetExtraLong();
	numElement = imageList->FindFileIndex(photoId);
	loadPicture = true;
	processIdle = true;
}
void CMainWindow::TransitionEnd()
{
    TRACE();
	if (startDiaporama)
	{
		int timeDelai = viewerParam->GetDelaiDiaporamaOption();
		diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
	}
	else
	{
		CShowBitmap * showBitmap = (CShowBitmap *)this->FindWindowById(SHOWBITMAPVIEWERID);
		if (showBitmap != nullptr)
		{
			showBitmap->TransitionEnd();
		}
	}
}

void CMainWindow::VideoEnd()
{
    TRACE();
	//Fin de la video
	if (startDiaporama)
		StartDiaporama();
}

void CMainWindow::StopDiaporama()
{
    TRACE();
	if (startDiaporama)
	{
		startDiaporama = false;
		if (diaporamaTimer->IsRunning())
			diaporamaTimer->Stop();
	}
}

void CMainWindow::OnFaceInfosStatusBarUpdate(wxCommandEvent& event)
{
	CFaceInfosUpdate * infoUpdate = (CFaceInfosUpdate *)event.GetClientData();
	if (infoUpdate != nullptr)
	{
		//statusBarViewer->SetText(2, infoUpdate->message_2);
		statusBarViewer->SetText(2, infoUpdate->message_3);
		statusBarViewer->SetRangeProgressBar(infoUpdate->photolistSize);
		statusBarViewer->SetPosProgressBar(infoUpdate->photolistSize - infoUpdate->listPhotoSize);
		delete infoUpdate;
	}
}

void CMainWindow::OnUpdateInfos(wxCommandEvent& event)
{
	TRACE();
	CPictureInfosMessage * pictureInfos = (CPictureInfosMessage *)event.GetClientData();
	if (pictureInfos != nullptr)
	{
		filename = pictureInfos->filename;
		if (filename[0] != '\0')
		{
			statusBarViewer->SetText(1, filename);
		}

		statusBarViewer->SetText(0, pictureInfos->infos);

		wxString label = CLibResource::LoadStringFromResource(L"LBLUPDATEINFOS", 1);
		wxString infos = label + CFileUtility::GetFileName(filename);
		statusBarViewer->SetWindowTitle(infos);

		delete pictureInfos;
	}
}

bool CMainWindow::GetProcessEnd()
{
    TRACE();
	if(nbProcessMD5 > 0)
		return false;

	return true;
}


void CMainWindow::OnExit(wxCommandEvent& event)
{
    TRACE();
	statusBarViewer->Exit();
}

////////////////////////////////////////////////////////////////////////////////////////////////
///Gestion des événements du menu
////////////////////////////////////////////////////////////////////////////////////////////////
bool CMainWindow::OpenFolder()
{
    TRACE();
	wxDirDialog dlg(nullptr, "Choose image directory", "",	wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dlg.ShowModal() == wxID_OK)
	{
		wxString path = dlg.GetPath();
		if (viewerParam != nullptr)
			viewerParam->SetCatalogCriteria("");

		AddFolder(path);

		updateCriteria = true;
		updateFolder = true;
        
        processIdle = true;
	}
	return false;
}

bool CMainWindow::IsFullscreen()
{
    TRACE();
	return fullscreen;
}

void CMainWindow::InitPictures(wxCommandEvent& event)
{
    TRACE();
	refreshFolder = true;
    processIdle = true;
	//updateCriteria = true;
}


void CMainWindow::OnFaceInfosUpdate(wxCommandEvent& event)
{
    TRACE();
	updateCriteria = true;
    processIdle = true;
}

void CMainWindow::ImageSuivante()
{
    TRACE();
    //clock_t tStart = clock();
    
	numElement++;
	if (numElement >= imageList->GetNbElement())
		numElement = 0;

	loadPicture = true;
    processIdle = true;
}

void CMainWindow::ImagePrecedente()
{
    TRACE();
	numElement--;
	if (numElement < 0)
		numElement = (int)imageList->GetNbElement() - 1;

	loadPicture = true;
    processIdle = true;
}


void CMainWindow::OnRefreshPicture(wxCommandEvent& event)
{
	wxWindow * viewerWindow = this->FindWindowById(VIEWERPICTUREWND);
	wxCommandEvent evt(wxEVENT_LOADPICTURE);
	CPictureElement * pictureElement = new CPictureElement();
	pictureElement->filename = filename;
	pictureElement->numElement = numElement;
	evt.SetClientData(pictureElement);
	viewerWindow->GetEventHandler()->AddPendingEvent(evt);
}


void CMainWindow::ImageFin()
{
    TRACE();
	this->numElement = (int)imageList->GetNbElement() - 1;
	loadPicture = true;
    processIdle = true;
}

void CMainWindow::ImageDebut()
{
    TRACE();
	this->numElement = 0;
	loadPicture = true;
    processIdle = true;
}

void CMainWindow::ShowToolbar()
{
    TRACE();
	showToolbar = !showToolbar;

	CViewerWindow * viewerWindow = (CViewerWindow *)this->FindWindowById(VIEWERPICTUREWND);
	if (viewerWindow != nullptr)
	{
		if (!showToolbar)
			viewerWindow->HideToolbar();
		else
			viewerWindow->ShowToolbar();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
///Affichage en mode plein écran
////////////////////////////////////////////////////////////////////////////////////////////////
void CMainWindow::SetFullscreen()
{
    TRACE();
	statusBarViewer->SetFullscreen();
}

void CMainWindow::SetScreenEvent(wxCommandEvent& event)
{
    TRACE();
    this->Resize();

}

void CMainWindow::SetFullscreenMode()
{
    TRACE();
	if (!fullscreen)
	{
		fullscreen = true;
		centralWnd->FullscreenMode();
		toolbar->Show(false);
		statusBar->Show(false);


        wxCommandEvent event(wxEVENT_SETSCREEN);
        wxPostEvent(this, event);  

	}
}

void CMainWindow::SetScreen()
{ 
    TRACE();
	if (fullscreen)
	{
		statusBarViewer->SetScreen();
		fullscreen = false;
		centralWnd->ScreenMode();
		toolbar->Show(true);
		statusBar->Show(true);

        wxCommandEvent event(wxEVENT_SETSCREEN);
        wxPostEvent(this, event);         
    }
}
