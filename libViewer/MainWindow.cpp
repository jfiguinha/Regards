 #include "MainWindow.h"
#include <libResource.h>
#include "CentralWnd.h"
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <PrintEngine.h>
#include <libPicture.h>
#include "ThemeParam.h"
#include "ViewerThemeInit.h"
#include "ViewerTheme.h"
#include "SqlFindPhotos.h"
#include "FileUtility.h"
#include <thread>
#include <wx/dir.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif

#include <wxSVG/SVGDocument.h>

#ifdef __APPLE__
#include <AppleSelectFile.h>
#include <SqlPhotos.h>
#include <SqlFolderCatalog.h>
#endif
#include <SQLRemoveData.h>
#include <SqlFolderCatalog.h>
#include <SqlInsertFile.h>

using namespace Regards::Viewer;
const long ONE_MSEC = 1000;
using namespace std;
using namespace Regards::Control;
using namespace Regards::Sqlite;
#define TIMER_DIAPORAMA 1
#define TIMER_LOADPICTURE 2
wxDEFINE_EVENT(EVENT_SHOWPICTURE, wxCommandEvent);
wxDEFINE_EVENT(EVENT_ENDNEWPICTURETHREAD, wxCommandEvent);
wxDEFINE_EVENT(EVENT_ENDTHUMBNAILTHREAD, wxCommandEvent);
wxDEFINE_EVENT(EVENT_POSTHUMBNAILPICTURE, wxCommandEvent);
wxDEFINE_EVENT(EVENT_ENDVIDEOTHREAD, wxCommandEvent);
//wxDEFINE_EVENT(VIDEO_END_ID, wxCommandEvent);

CMainWindow::CMainWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarViewer)
	: CWindowMain(parent, id)
{
    
    //wxSVGDocument* svgDoc = new wxSVGDocument;
    //svgDoc->Load("/Users/jacques/Pictures/test.svg");
    //wxImage img = svgDoc->Render(40,40,NULL,true,true);
    
	videoEffectParameter = new CVideoEffectParameter();
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();
	viewerParam = (CViewerParam *)CViewerParamInit::getInstance();
	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetMainToolbarTheme(&theme);
		toolbar = new CToolbar(this, wxID_ANY, theme);
	}

    loadPictureTimer = new wxTimer(this, TIMER_LOADPICTURE);
	diaporamaTimer = new wxTimer(this, TIMER_DIAPORAMA);
	centralWnd = new CCentralWnd(this, CENTRALEXPLORERWINDOWID, statusBarViewer, videoEffectParameter);
	this->statusBarViewer = statusBarViewer;
	Connect(TIMER_DIAPORAMA, wxEVT_TIMER, wxTimerEventHandler(CMainWindow::OnTimerDiaporama), nullptr, this);
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CMainWindow::OnKeyDown));
	Connect(EVENT_SHOWPICTURE, wxCommandEventHandler(CMainWindow::ShowPicture));
	Connect(EVENT_ENDNEWPICTURETHREAD, wxCommandEventHandler(CMainWindow::EndPictureThread));
    Connect(EVENT_ENDTHUMBNAILTHREAD, wxCommandEventHandler(CMainWindow::EndThumbnailThread));
    Connect(EVENT_ENDVIDEOTHREAD, wxCommandEventHandler(CMainWindow::EndVideoThread));
    Connect(EVENT_POSTHUMBNAILPICTURE, wxCommandEventHandler(CMainWindow::ThumbnailPosPicture));
	Connect(EVENT_REFRESHLIST, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::RefreshPictureList));
	Connect(TOOLBAR_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnShowToolbar));
	Connect(VIDEO_END_ID, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnVideoEnd));
	Connect(VIDEO_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::SetVideoPos));
    Connect(VIDEO_START, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnVideoStart));
    Connect(TIMER_LOADPICTURE, wxEVT_TIMER, wxTimerEventHandler(CMainWindow::OnTimerLoadPicture), nullptr, this);
    
#ifdef VIEWER
    Connect(wxEVT_CRITERIACHANGE, wxCommandEventHandler(CMainWindow::CriteriaChange));
#endif
	statusBar = new wxStatusBar(this, wxID_ANY, wxSTB_DEFAULT_STYLE, "wxStatusBar");

	int tabWidth[] = { 100, 300, 200 };
	statusBar->SetFieldsCount(3);
	statusBar->SetStatusWidths(3, tabWidth);

	progressBar = new wxGauge(statusBar, wxID_ANY, 200, wxPoint(600, 0), wxSize(200, statusBar->GetSize().y),
		wxGA_HORIZONTAL);
	progressBar->SetRange(100);
	progressBar->SetValue(50);
}

void CMainWindow::OnTimerLoadPicture(wxTimerEvent& event)
{
    CThreadPictureData * pictureData = new CThreadPictureData();
    pictureData->mainWindow = this;
    pictureData->picture = filenameTimer;
    pictureData->isVisible = true;
    std::thread * threadloadPicture = new thread(LoadingNewPicture, pictureData);
    pictureData->myThread = threadloadPicture;
    
    loadPictureTimer->Stop();
}

#ifdef VIEWER
void CMainWindow::CriteriaChange(wxCommandEvent& event)
{
    //Refresh Criteria
    //Mise ‡ jour de l'affichage de l'arborescence
    CCategoryFolderWindow * catalogWnd = (CCategoryFolderWindow *)this->FindWindowById(CATEGORYFOLDERWINDOWID);
    if (catalogWnd != nullptr)
        catalogWnd->UpdateCriteria();
    
    //Update List Picture
    if (centralWnd != nullptr)
        centralWnd->UpdateCriteria();
}
#endif
void CMainWindow::RefreshPictureList(wxCommandEvent& event)
{
	PhotosVector photoVector;
	CSqlFindPhotos sqlFindPhotos;
	sqlFindPhotos.SearchPhotos(&photoVector);

	if (photoVector.size() > 0)
	{
		pictureFiles.clear();

		for (CPhotos photo : photoVector)
		{
			AddPicturesFiles(photo.GetPath());
			//pictureFiles.push_back(photo.GetPath());
		}

		centralWnd->SetListeFile(pictureFiles);

		numElement = 0;

		firstFileToShow = GetFilePath(0);

		if (firstFileToShow != L"")
			LoadPictureInThread(firstFileToShow);
        
        centralWnd->SetNumElement(numElement);
	}
}

void CMainWindow::ThumbnailPosPicture(wxCommandEvent& event)
{
    centralWnd->SetNumElement(numElement);
    
    if(threadPosPicture != nullptr)
    {
        threadPosPicture->join();
        delete threadPosPicture;
        threadPosPicture = nullptr;
    }
}

void CMainWindow::OnVideoStart(wxCommandEvent& event)
{
    centralWnd->StopLoadingPicture();

}

void CMainWindow::OnShowToolbar(wxCommandEvent& event)
{
	ShowToolbar();
}

void CMainWindow::UpdateScreenRatio()
{
    toolbar->UpdateScreenRatio();
    centralWnd->UpdateScreenRatio();
    this->Resize();
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

void CMainWindow::EndPictureThread(wxCommandEvent& event)
{
    if(!multithread)
    {
        muPicture.lock();
        threadloadPicture->join();
        delete(threadloadPicture);
        threadloadPicture = nullptr;
        muPicture.unlock();
    }
    
    if(needToReload && !multithread)
    {
        LoadingPicture(firstFileToShow);
    }
}

void CMainWindow::EndVideoThread(wxCommandEvent& event)
{
    CThreadVideoData * videoData = (CThreadVideoData *)event.GetClientData();
    if(videoData != nullptr)
    {
        if(videoData->video != firstFileToShow)
        {
            CLibPicture libPicture;
            if (libPicture.TestIsVideo(firstFileToShow))
                needToReload = true;
        }
           
        
        delete videoData;
        videoData = nullptr;
    }
    
    if(!multithread)
    {
        muVideo.lock();
        threadloadVideo->join();
        delete(threadloadVideo);
        threadloadVideo = nullptr;
        muVideo.unlock();
    }
    
    
    if(needToReload)
    {
        LoadPictureInThread(firstFileToShow);
    }
    
}

void CMainWindow::EndThumbnailThread(wxCommandEvent& event)
{
    if(!multithread)
    {
        muThumbnail.lock();
        threadloadThumbnail->join();
        delete(threadloadThumbnail);
        threadloadThumbnail = nullptr;
        muThumbnail.unlock();
    }
}

void CMainWindow::OnTimerDiaporama(wxTimerEvent& event)
{
	ImageSuivante();
}

void CMainWindow::OnVideoEnd(wxCommandEvent& event)
{
	if (startDiaporama)
	{
		ImageSuivante();
	}
}

CMainWindow::~CMainWindow()
{
	bool showInfos;
	bool showThumbnail;

	showInfos = centralWnd->IsPanelInfosVisible();
	showThumbnail = centralWnd->IsPanelThumbnailVisible();

	if (viewerParam != nullptr)
	{
		viewerParam->SetShowInfos(showInfos);
		viewerParam->SetShowThumbnail(showThumbnail);
        wxRect rc = this->GetWindowRect();
		viewerParam->SetPositionWindow(rc);
	}
#ifdef __APPLE__
    if(selectFolder != nullptr)
    {
        if(appleSecurityScopeData != nullptr)
        {
            selectFolder->StopSecurityAccess(appleSecurityScopeData);
            delete appleSecurityScopeData;
        }
        delete selectFolder;
        
    }
#endif

	if (diaporamaTimer->IsRunning())
		diaporamaTimer->Stop();

	delete(diaporamaTimer);

	if (loadPictureTimer->IsRunning())
		loadPictureTimer->Stop();

	delete(loadPictureTimer);


	delete(videoEffectParameter);
	delete(centralWnd);
	delete(toolbar);
}

void CMainWindow::Resize()
{
	if (!fullscreen)
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
	else
	{
		centralWnd->SetSize(0, 0, width, height);
	}
}


void CMainWindow::CleanPictureFiles()
{
    pictureFiles.clear();
	//Clear filtre
	//CSQLRemoveData::DeleteCatalog(1);
    
}

void CMainWindow::AddPicturesFiles(const wxString &file)
{
    pictureFiles.push_back(file);
}

wxString CMainWindow::GetFilePath(const int &numElement)
{
    return pictureFiles.at(numElement);
}

void CMainWindow::FindFileIndex(const wxString & filename)
{
    for (wxString file : pictureFiles)
    {
        if (file == filename)
        {
            return;
        }
        else
            numElement++;
    }
    numElement = 0;
}

void CMainWindow::GetFolderCredential(const wxString & folderPath)
{
#ifdef __APPLE__
    
    if(selectFolder != nullptr)
    {
        if(appleSecurityScopeData != nullptr)
            selectFolder->StopSecurityAccess(appleSecurityScopeData);
        delete selectFolder;
        delete appleSecurityScopeData;
    }
    
    if(folderPath != "")
    {
        //Enable security
        selectFolder = new CAppleSelectFile(folderPath);
        CSqlFolderCatalog folderCatalog;
        appleSecurityScopeData = folderCatalog.GetDataSecurityScope(folderPath);
        if(appleSecurityScopeData != nullptr)
        {
            selectFolder->EnableSecurityAccess(appleSecurityScopeData);
            idFolder = folderCatalog.GetFolderCatalogId(1, folderPath);
        }
        else
        {
            appleSecurityScopeData = selectFolder->GetFileSecurityScope();
            folderCatalog.InsertFolderCatalog(1, folderPath, appleSecurityScopeData);
            idFolder = folderCatalog.GetLastId();
            selectFolder->EnableSecurityAccess(appleSecurityScopeData);
        }
        
    }
#endif
}


void CMainWindow::SetFolder(const wxString & folderPath, const wxString &filename)
{
    wxArrayString files;
    
    GetFolderCredential(folderPath);
    
	//Test if valid Folder
	if(!wxDir::Exists(folderPath))
	{
		OpenFolder();
	}
	else
	{
		this->folderPath = folderPath;
    
		CLibPicture libPicture;
		if (viewerParam != nullptr)
		{
			viewerParam->SetLastFolder(folderPath);
#ifdef VIEWER
            centralWnd->InitSaveParameter();
#endif
		}

		if(!RefreshFolder())
		{
			wxString title = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);
			wxString libelle = CLibResource::LoadStringFromResource(L"LBLPICTUREFOLDER", 1);
			wxMessageBox(libelle, title, wxICON_INFORMATION);
			
			OpenFolder();
		}
	}
}

void CMainWindow::InitSaveParameter()
{
#ifdef VIEWER
    if(centralWnd != nullptr)
        centralWnd->InitSaveParameter();
#endif
}

bool CMainWindow::RefreshFolder(const wxString &sqlRequest)
{
    CLibPicture libPicture;

    CleanPictureFiles();
    
    wxArrayString files;
    
    size_t nbFile = wxDir::GetAllFiles(folderPath, &files, wxEmptyString, wxDIR_FILES);
    
    if(nbFile > 0)
    {
        for (wxString file : files)
        {
            if (libPicture.TestImageFormat(file) != 0)
            {
                AddPicturesFiles(file);
            }
        }
        
        if (pictureFiles.size() == 0)
            return false;

        //Add Picture to DataBase
    #ifdef VIEWER
        CSqlInsertFile insertFile;
        CSqlFolderCatalog folderCatalog;
        int64_t idFolder = folderCatalog.GetOrInsertFolderCatalog(1, folderPath);
        insertFile.UpdateFolder(pictureFiles, idFolder);
        centralWnd->UpdateCriteria(idFolder);
        
        //Récupération de la recherch
        wxString requestSql = sqlRequest;
        
        if(requestSql == "")
            requestSql = centralWnd->GetSqlRequest();
        
        if(requestSql != "")
        {
            PhotosVector photoVector;
            CSqlFindPhotos sqlFindPhotos;
            sqlFindPhotos.SearchPhotos(requestSql);
            sqlFindPhotos.SearchPhotos(&photoVector);
            
            if (photoVector.size() > 0)
            {
                pictureFiles.clear();
                
                for (CPhotos photo : photoVector)
                {
                    AddPicturesFiles(photo.GetPath());
                }
            }
        }
        
        
    #endif
        
        //Execute filter search
        

        numElement = 0;
        centralWnd->SetListeFile(pictureFiles);
        
        if (filename != L"")
        {
            FindFileIndex(filename);
        }
        
        centralWnd->SetNumElement(numElement);
        
        return true;
    }
    
    return false;
}

void CMainWindow::StartDiaporama()
{
	
	if (viewerParam != nullptr)
	{
		centralWnd->SetDiaporamaMode();

		if (viewerParam->GetFullscreenDiaporamaOption())
			SetFullscreen();
		
        wxString fileToLoad = GetFilePath(numElement);
        
        //Test is video
        
        CLibPicture libPicture;
        if(!libPicture.TestIsVideo(fileToLoad))
        {
            int timeDelai = viewerParam->GetDelaiDiaporamaOption();
            diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
        }
	}

	startDiaporama = true;
}


void CMainWindow::PictureVideoClick(const int &timePosition)
{
	CShowVideo * showVideoWindow = (CShowVideo *)this->FindWindowById(SHOWVIDEOVIEWERID);
	if (showVideoWindow != nullptr)
		showVideoWindow->SetPosition(timePosition * ONE_MSEC);
}

void CMainWindow::SetVideoPos(wxCommandEvent& event)
{
	if (centralWnd != nullptr)
	{
		int64_t pos = event.GetExtraLong();
		centralWnd->SetVideoPosition(pos);
	}
}

void CMainWindow::PictureClick(const int &numPhoto)
{
	numElement = numPhoto;
	wxString photoName = GetFilePath(numElement);
	if (firstFileToShow != photoName)
    {
        firstFileToShow = photoName;
		LoadPictureInThread(firstFileToShow);
    }
}

void CMainWindow::TransitionEnd()
{
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
			showBitmap->Refresh();
		}
	}
}

void CMainWindow::VideoEnd()
{
	//Fin de la video
	if (startDiaporama)
		StartDiaporama();
}

void CMainWindow::StopDiaporama()
{
	if (startDiaporama)
	{
		startDiaporama = false;
		if (diaporamaTimer->IsRunning())
			diaporamaTimer->Stop();

		centralWnd->SetNormalMode();

		centralWnd->SetNumElement(numElement);
		


	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
///Mise à jour des informations sur les fichiers
////////////////////////////////////////////////////////////////////////////////////////////////
void CMainWindow::UpdateInfos(CRegardsBitmap * bitmap)
{
	if (bitmap != nullptr)
	{
		filename = bitmap->GetFilename();
		if (filename[0] != '\0')
		{
			//wxString folder = CFileUtility::GetFolder(filename);
			wxString fichier = CFileUtility::GetFileName(filename);
			statusBarViewer->SetText(1, fichier);
			//statusBarViewer->SetText(2, fichier);
			//Mise à jour de la barre de status
		}

		wxString infos = to_string(bitmap->GetBitmapWidth()) + "x" + to_string(bitmap->GetBitmapHeight());
		statusBarViewer->SetText(0, infos);

		infos = "Regards Viewer : " + CFileUtility::GetFileName(filename);
		statusBarViewer->SetWindowTitle(infos);
	}
	
}

void CMainWindow::SetDatabase(const wxString & folderPath, const wxString & requestSql)
{
#ifdef VIEWER
    this->folderPath = folderPath;
    
    if(folderPath != "")
    {
        GetFolderCredential(folderPath);
    }
    
	if (!RefreshFolder(requestSql))
	{
		OpenFolder();
	}
    else
    {
        firstFileToShow = GetFilePath(0);
    
        if (firstFileToShow != L"")
            LoadPictureInThread(firstFileToShow);
    }
#else
    
    PhotosVector photoVector;
    CSqlFindPhotos sqlFindPhotos;
    sqlFindPhotos.SearchPhotos(&photoVector);
    
    if (photoVector.size() > 0)
    {
        pictureFiles.clear();
        
        for (CPhotos photo : photoVector)
        {
            AddPicturesFiles(photo.GetPath());
            //pictureFiles.push_back(photo.GetPath());
        }
        
        centralWnd->SetListeFile(pictureFiles);
        
        numElement = 0;
        
        firstFileToShow = GetFilePath(0);
        
        if (firstFileToShow != L"")
            LoadPictureInThread(firstFileToShow);
    }

#endif
}

void CMainWindow::Exit()
{
	statusBarViewer->Exit();
}

////////////////////////////////////////////////////////////////////////////////////////////////
///Gestion des événements du menu
////////////////////////////////////////////////////////////////////////////////////////////////
void CMainWindow::OpenFolder()
{
	wxDirDialog dlg(nullptr, "Choose image directory", "",	wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dlg.ShowModal() == wxID_OK)
	{
		wxString path = dlg.GetPath();
		if (viewerParam != nullptr)
			viewerParam->SetCatalogCriteria("");

		SetFolder(path);
        //threadPosPicture = new thread(PositionPicture, this);
        firstFileToShow = GetFilePath(0);
        
        if (firstFileToShow != L"")
            LoadPictureInThread(firstFileToShow);
	}
}




void CMainWindow::SetPictureFile(const wxString & filePath)
{
	wxString folderPath = CFileUtility::GetFolder(filePath);
	SetFolder(folderPath, filePath);
    threadPosPicture = new thread(PositionPicture, this);
    
}

bool CMainWindow::IsFullscreen()
{
	return fullscreen;
}


////////////////////////////////////////////////////////////////////////////////////////////////
///Mise à jour des informations sur les fichiers
////////////////////////////////////////////////////////////////////////////////////////////////
void CMainWindow::SetPicture(CRegardsBitmap * bitmap, const bool &isThumbnail)
{
	if (bitmap != nullptr)
	{
        clock_t tStart = clock();
		filename = bitmap->GetFilename();
		centralWnd->SetBitmap(bitmap, isThumbnail);
		UpdateInfos(bitmap);
        printf("SetPicture Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	}
}

void CMainWindow::ImageSuivante()
{
    clock_t tStart = clock();
    
	numElement++;
	if (numElement == pictureFiles.size())
		numElement = 0;
	
	LoadPicture(numElement);
	centralWnd->SetNumElement(numElement);
    
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

void CMainWindow::ImagePrecedente()
{
	numElement--;
	if (numElement < 0)
		numElement = (int)pictureFiles.size() - 1;

	LoadPicture(numElement);
	centralWnd->SetNumElement(numElement);
}


void CMainWindow::LoadPicture(const int &numElement)
{
    wxString fileToLoad = GetFilePath(numElement);
    if(fileToLoad != firstFileToShow)
    {
        firstFileToShow = fileToLoad;
        LoadPictureInThread(firstFileToShow);
    }
}

void CMainWindow::Reload()
{
	LoadPicture(numElement);
}

void CMainWindow::ImageFin()
{
	numElement = (int)pictureFiles.size() - 1;
	LoadPicture(numElement);
	centralWnd->SetNumElement(numElement);
}

void CMainWindow::ImageDebut()
{
	numElement = 0;
	LoadPicture(numElement);
	centralWnd->SetNumElement(numElement);

}

void CMainWindow::ShowToolbar()
{
	showToolbar = !showToolbar;

	if (!showToolbar)
		centralWnd->HideToolbar();
	else
		centralWnd->ShowToolbar();
}

void CMainWindow::OnKeyDown(wxKeyEvent& event)
{

}

void CMainWindow::SetFullscreenMode()
{
	if (!fullscreen)
	{
		
		fullscreen = true;
		centralWnd->FullscreenMode();
		toolbar->Show(false);
		statusBar->Show(false);
		Resize();

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
///Affichage en mode plein écran
////////////////////////////////////////////////////////////////////////////////////////////////
void CMainWindow::SetFullscreen()
{
	statusBarViewer->SetFullscreen();
}

void CMainWindow::PositionPicture(CMainWindow * mainWnd)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	wxCommandEvent * event = new wxCommandEvent(EVENT_POSTHUMBNAILPICTURE);
	wxQueueEvent(mainWnd, event);
}

void CMainWindow::LoadThumbnail(const wxString &filename)
{
    CSqlThumbnail sqlThumbnail;
    CRegardsBitmap * bitmap = sqlThumbnail.GetPictureThumbnail(filename);
    if(bitmap == nullptr)
    {
        //Chargement du thumbnail de l'image
        CLibPicture libPicture;
        bitmap = libPicture.LoadThumbnail(filename, true);
    }
    
    needToReload = false;

    if (bitmap != nullptr)
    {
        if (bitmap->GetFilename() == firstFileToShow)
            SetPicture(bitmap, true);
    }

}

void CMainWindow::LoadingThumbnail(CThreadPictureData * pictureData)
{
    //loading thumbnail

    
    
	CLibPicture libPicture;
	CRegardsBitmap * bitmap = nullptr;
	if (pictureData != nullptr)
	{
		bitmap = libPicture.LoadThumbnail(pictureData->picture);
		if (bitmap != nullptr)
			bitmap->SetFilename(pictureData->picture);
	}

	if (bitmap != nullptr)
	{
        CBitmapReturn * bitmapReturn = new CBitmapReturn();
        bitmapReturn->isThumbnail = true;
        bitmapReturn->bitmap = bitmap;
		wxCommandEvent * event = new wxCommandEvent(EVENT_SHOWPICTURE);
		event->SetClientData(bitmapReturn);
		wxQueueEvent(pictureData->mainWindow, event);
	}

	wxCommandEvent * event = new wxCommandEvent(EVENT_ENDTHUMBNAILTHREAD);
	wxQueueEvent(pictureData->mainWindow, event);


	delete pictureData;
	pictureData = nullptr;
}

void CMainWindow::LoadingVideo(CThreadVideoData * videoData)
{
    videoData->mainWindow->centralWnd->SetVideo(videoData->video);
	wxCommandEvent * event = new wxCommandEvent(EVENT_ENDVIDEOTHREAD);
    event->SetClientData(videoData);
	wxQueueEvent(videoData->mainWindow, event);

}

void CMainWindow::LoadPictureInThread(const wxString &filename)
{
	CLibPicture libPicture;
	this->filename = filename;
    
    
	if (libPicture.TestIsVideo(filename))
	{
        centralWnd->StartLoadingPicture(numElement);
		centralWnd->SetVideo(filename);
	}
    else if(libPicture.TestIsAnimation(filename))
    {
        centralWnd->SetAnimation(filename);
    }
	else
	{
        
		if (startDiaporama)
		{
			
			CRegardsBitmap * bitmap = libPicture.LoadPicture(filename);
			if (bitmap != nullptr)
				bitmap->SetFilename(filename);

			SetPicture(bitmap, false);
		}
		else
		{
            centralWnd->StartLoadingPicture(numElement);
			LoadingPicture(filename);
		}
	}
}

void CMainWindow::SetScreen()
{
	StopDiaporama();
	if (fullscreen)
	{
		statusBarViewer->SetScreen();
		fullscreen = false;
		centralWnd->ScreenMode();
		toolbar->Show(true);
		statusBar->Show(true);
		this->Resize();
	}
}

void CMainWindow::LoadingNewPicture(CThreadPictureData * pictureData)
{
	CLibPicture libPicture;
	CRegardsBitmap * bitmap = nullptr;
	if (pictureData != nullptr)
	{
		bitmap = libPicture.LoadPicture(pictureData->picture);
		if (bitmap != nullptr)
			bitmap->SetFilename(pictureData->picture);
	}

	if (bitmap != nullptr)
	{
        CBitmapReturn * bitmapReturn = new CBitmapReturn();
        bitmapReturn->myThread = pictureData->myThread;
        bitmapReturn->isThumbnail = false;
        bitmapReturn->bitmap = bitmap;
		wxCommandEvent * event = new wxCommandEvent(EVENT_SHOWPICTURE);
		event->SetClientData(bitmapReturn);
		wxQueueEvent(pictureData->mainWindow, event);
	}

	wxCommandEvent * event = new wxCommandEvent(EVENT_ENDNEWPICTURETHREAD);
	wxQueueEvent(pictureData->mainWindow, event);

	delete pictureData;
	pictureData = nullptr;
}

void CMainWindow::LoadingPicture(const wxString &filenameToShow)
{
   
    if(!multithread)
    {
        /*
        CLibPicture libPicture;
        
        if (libPicture.HasThumbnail(filenameToShow))
        {
            //Test si RAW
            muThumbnail.lock();
            if (threadloadThumbnail == nullptr)
            {
                    CThreadPictureData * thumbnailData = new CThreadPictureData();
                    thumbnailData->mainWindow = this;
                    thumbnailData->picture = filenameToShow;
                    thumbnailData->isVisible = true;
                    threadloadThumbnail = new thread(LoadingThumbnail, thumbnailData);
            }
            muThumbnail.unlock();
        }
        */
        
        LoadThumbnail(filenameToShow);
        
		/*
        muPicture.lock();
        if (threadloadPicture == nullptr)
        {
            CThreadPictureData * pictureData = new CThreadPictureData();
            pictureData->mainWindow = this;
            pictureData->picture = filenameToShow;
            pictureData->isVisible = true;
            threadloadPicture = new thread(LoadingNewPicture, pictureData);
        }
        muPicture.unlock();
		*/
		if (loadPictureTimer->IsRunning())
			loadPictureTimer->Stop();


		loadPictureTimer->Start(10);
    }
    else
    {
        /*
        CLibPicture libPicture;
        
        if (libPicture.HasThumbnail(filenameToShow))
        {
            CThreadPictureData * thumbnailData = new CThreadPictureData();
            thumbnailData->mainWindow = this;
            thumbnailData->picture = filenameToShow;
            thumbnailData->isVisible = true;
            std::thread * threadloadThumbnail = new thread(LoadingThumbnail, thumbnailData);
            thumbnailData->myThread = threadloadThumbnail;
        }
        */
        
        LoadThumbnail(filenameToShow);
        
        
        filenameTimer = filenameToShow;
        
        if (loadPictureTimer->IsRunning())
            loadPictureTimer->Stop();
        
		
        loadPictureTimer->Start(10);
        
        
		/*
        CThreadPictureData * pictureData = new CThreadPictureData();
        pictureData->mainWindow = this;
        pictureData->picture = filenameToShow;
        pictureData->isVisible = true;
        std::thread * threadloadPicture = new thread(LoadingNewPicture, pictureData);
        pictureData->myThread = threadloadPicture;
        */
    }
    


}

void CMainWindow::ShowPicture(wxCommandEvent& event)
{
    needToReload = false;
	CBitmapReturn * pictureData = (CBitmapReturn *)event.GetClientData();
	if (pictureData != nullptr)
	{
		if (pictureData->bitmap->GetFilename() == firstFileToShow)
			SetPicture(pictureData->bitmap, pictureData->isThumbnail);
		else
        {
            if(!pictureData->isThumbnail)
                needToReload = true;
			delete pictureData->bitmap;
        }
	}
    
    if(!pictureData->isThumbnail)
    {
        centralWnd->StopLoadingPicture();
    }
    
    if(pictureData->myThread != nullptr)
        pictureData->myThread->join();
    
    delete pictureData->myThread;
    delete pictureData;
    

}