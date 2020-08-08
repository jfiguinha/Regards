#include <header.h>
#include "ViewerFrame.h"
#include "MainWindow.h"
#include "PertinenceValue.h"
#include <BitmapPrintout.h>
#include "ViewerParamInit.h"
#include "SQLRemoveData.h"
#include <PrintEngine.h>
#include <LibResource.h>
#include <window_id.h>
#include <wx/filename.h>
#include <ConfigRegards.h>
#include "MainThemeInit.h"
#include "ViewerParam.h"
#include "MainTheme.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <FileUtility.h>
#include <SqlFindPhotos.h>
#include <SqlFindFolderCatalog.h>
#include <OpenCLDialog.h>
#include <InterpolationFilterSelect.h>
#include <libPicture.h>
#include <SavePicture.h>
#include <ScannerFrame.h>
#include <OpenCLEngine.h>
#include "ListFace.h"
#include "WaitingWindow.h"
using namespace std;
using namespace Regards::Print;
using namespace Regards::Control;
using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::OpenCL;
#define TIMER_LOADPICTURE 2
#if !wxUSE_PRINTING_ARCHITECTURE
#error "You must set wxUSE_PRINTING_ARCHITECTURE to 1 in setup.h, and recompile the library."
#endif




#include <ctype.h>
#include "wx/metafile.h"
#include "wx/print.h"
#include "wx/printdlg.h"
#include "wx/image.h"
#include "wx/accel.h"

#if wxUSE_POSTSCRIPT
#include "wx/generic/printps.h"
#include "wx/generic/prntdlgg.h"
#endif

#if wxUSE_GRAPHICS_CONTEXT
#include "wx/graphics.h"
#endif

#ifdef __WXMAC__
#include "wx/osx/printdlg.h"
#endif


#ifndef wxHAS_IMAGES_IN_RESOURCES
#ifdef __WXGTK__
#include "../Resource/sample.xpm"
#else
#include "../../Resource/sample.xpm"
#endif
#endif


bool CViewerFrame::viewerMode = true;

using namespace Regards::Viewer;

bool CViewerFrame::GetViewerMode()
{
    return viewerMode;
}


void CViewerFrame::SetViewerMode(const bool &mode)
{
    viewerMode = mode;
}




CViewerFrame::CViewerFrame(const wxString& title, const wxPoint& pos, const wxSize& size, IMainInterface * mainInterface, const wxString &fileToOpen)
	: wxFrame(nullptr, wxID_ANY, title, pos, size, wxMAXIMIZE | wxDEFAULT_FRAME_STYLE)
{
	mainWindow = nullptr;
	fullscreen = false;
	onExit = false;
	mainWindowWaiting = nullptr;
	SetIcon(wxICON(sample));
	frameScanner = nullptr;
	viewerParam = new CMainParam();
	CMainParamInit::Initialize(viewerParam);
	this->Maximize();
	viewerTheme = new CMainTheme();
	CMainThemeInit::Initialize(viewerTheme);

	this->mainInterface = mainInterface;
	this->mainInterface->parent = this;

	FolderCatalogVector folderList;
	CSqlFindFolderCatalog folderCatalog;
	folderCatalog.GetFolderCatalog(&folderList, NUMCATALOGID);

	exitTimer = new wxTimer(this, wxTIMER_EXIT);
	Connect(wxTIMER_EXIT, wxEVT_TIMER, wxTimerEventHandler(CViewerFrame::CheckAllProcessEnd), nullptr, this);

	bool folderChange = false;
    
    m_watcher = new wxFileSystemWatcher();
    m_watcher->SetOwner(this); 
    Connect(wxEVT_FSWATCHER, wxFileSystemWatcherEventHandler(CViewerFrame::OnFileSystemModified));    

	//Test de la validité des répertoires
	for (CFolderCatalog folderlocal : folderList)
	{
		if (!wxDirExists(folderlocal.GetFolderPath()))
		{
			//Remove Folder
			CSQLRemoveData::DeleteFolder(folderlocal.GetNumFolder());
			folderChange = true;
		}
        else
        {
            AddFSEntry(folderlocal.GetFolderPath());
        }
	}

	//Test de la validité des fichiers
	PhotosVector photoList;
	CSqlFindPhotos findphotos;
	findphotos.GetAllPhotos(&photoList);
	for (CPhotos photo : photoList)
	{
		if (!wxFileExists(photo.GetPath()))
		{
			//Remove Folder
			CSQLRemoveData::DeletePhoto(photo.GetId());
			folderChange = true;
		}
	}

	if (folderChange)
	{
		CMainParam * viewerParam = (CMainParam *)CMainParamInit::getInstance();
		wxString sqlRequest = viewerParam->GetLastSqlRequest();

		CSqlFindPhotos sqlFindPhotos;
		sqlFindPhotos.SearchPhotos(sqlRequest);
	}

	bool openFirstFile = true;
	//SetIcon(wxIcon(wxT("regards.xpm")));
	if (fileToOpen != "")
		openFirstFile = false;
	mainWindow = new CMainWindow(this, MAINVIEWERWINDOWID, this, openFirstFile);
	
	//mainWindow->Show(true);
	//mainWindowWaiting->Show(false);
	//preview = nullptr;
	m_previewModality = wxPreviewFrame_AppModal;
	loadPictureTimer = new wxTimer(this, TIMER_LOADPICTURE);
	wxMenu *menuFile = new wxMenu;
	wxMenu *menuParameter = new wxMenu;

    
    wxString labelDecreaseIconSize = CLibResource::LoadStringFromResource(L"labelDecreaseIconSize",1);//L"Decrease Icon Size";
    wxString labelDecreaseIconSize_link = CLibResource::LoadStringFromResource(L"labelDecreaseIconSize_link",1);//L"&Decrease Icon Size";
    wxString labelEnlargeIconSize = CLibResource::LoadStringFromResource(L"labelEnlargeIconSize",1);//L"Enlarge Icon Size";
    wxString labelEnlargeIconSize_link = CLibResource::LoadStringFromResource(L"labelEnlargeIconSize_link",1);//L"&Enlarge Icon Size";
    wxString labelConfiguration = CLibResource::LoadStringFromResource(L"labelConfiguration",1);//L"Configuration";
    wxString labelConfiguration_link = CLibResource::LoadStringFromResource(L"labelConfiguration_link",1);//L"&Configuration";
    wxString labelOpenCL = CLibResource::LoadStringFromResource(L"labelOpenCL",1);//L"OpenCL";
    wxString labelOpenCL_link = CLibResource::LoadStringFromResource(L"labelOpenCL_link",1);//L"&OpenCL";
    wxString labelEraseDataBase = CLibResource::LoadStringFromResource(L"labelEraseDataBase",1);//L"Erase Database";
    wxString labelEraseDataBase_link = CLibResource::LoadStringFromResource(L"labelEraseDataBase_link",1);//L"&Erase Database";
    wxString labelThumbnailRight = CLibResource::LoadStringFromResource(L"labelThumbnailRight",1);//L"Right Position";
    wxString labelThumbnailRight_link = CLibResource::LoadStringFromResource(L"labelThumbnailRight_link",1);//L"&Right Position";
    wxString labelThumbnailBottom = CLibResource::LoadStringFromResource(L"labelThumbnailBottom",1);//L"Bottom Position";
    wxString labelThumbnailBottom_link = CLibResource::LoadStringFromResource(L"labelThumbnailBottom_link",1);//L"&Bottom Position";
    wxString labelPageSetup = CLibResource::LoadStringFromResource(L"labelPageSetup",1);//L"Page setup";
    wxString labelPageSetup_link = CLibResource::LoadStringFromResource(L"labelPageSetup_link",1);//L"Page Set&up...";
    wxString labelPageMargins =CLibResource::LoadStringFromResource(L"labelPageMargins",1);// L"Page margins";
    wxString labelPageMargins_link = CLibResource::LoadStringFromResource(L"labelPageMargins_link",1);//L"Page margins...";
    wxString labelFile = CLibResource::LoadStringFromResource(L"labelFile",1);//L"&File";
    wxString labelParameter = CLibResource::LoadStringFromResource(L"labelParameter",1);//L"&Parameter";
    wxString labelSizeIcon = CLibResource::LoadStringFromResource(L"labelSizeIcon",1);//L"&Icon Size";
    wxString labelThumbnail = CLibResource::LoadStringFromResource(L"labelThumbnail",1);//L"&Thumbnail";
    wxString labelHelp = CLibResource::LoadStringFromResource(L"labelHelp",1);//L"&Help";
    
    wxMenu *menuSizeIcon = new wxMenu;
    menuSizeIcon->Append(ID_SIZEICONLESS, labelDecreaseIconSize_link, labelDecreaseIconSize);
    menuSizeIcon->Append(ID_SIZEICONMORE, labelEnlargeIconSize_link, labelEnlargeIconSize);
	menuParameter->Append(ID_Configuration, labelConfiguration_link, labelConfiguration);
	menuParameter->Append(ID_OpenCL, labelOpenCL_link, labelOpenCL);
    //menuParameter->Append(ID_ERASEDATABASE, labelEraseDataBase_link, labelEraseDataBase);
	//menuParameter->Append(ID_INTERPOLATIONFILTER, "&Filter Interpolation", "Filter Interpolation");


	//wxMenu *menuFace = new wxMenu;
	//menuFace->Append(ID_FACEDETECTION, "&Face Detection", "Face Detection");
	//menuFace->Append(ID_CATEGORYDETECTION, "&Category Detection", "Category Detection");
	//menuFile->Append(ID_SCANNER, "&Scanner", "Scanner");
	menuFile->Append(ID_EXPORT, "&Export", "Export");
#ifdef WIN32
	menuFile->Append(ID_ASSOCIATE, "&Associate", "Associate");
#endif
	menuFile->Append(WXPRINT_PAGE_SETUP, labelPageSetup_link, labelPageSetup);
#ifdef __WXMAC__
	menuFile->Append(WXPRINT_PAGE_MARGINS, labelPageMargins_link, labelPageMargins);
#endif
	//menuFile->Append(wxID_PREVIEW, wxT("Print Pre&view"), wxT("Preview"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_PRINT, wxT("&Print..."), wxT("Print"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
    menuHelp->Append(wxID_HELP);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, labelFile);
	menuBar->Append(menuParameter, labelParameter);
	//menuBar->Append(menuFace, "Photo Detection");
    menuBar->Append(menuSizeIcon, labelSizeIcon);

	menuBar->Append(menuHelp, labelHelp);
	SetMenuBar(menuBar);

	SetLabel(wxT("Regards Viewer"));
	//Connect(wxEVT_SIZE, wxSizeEventHandler(CViewerFrame::OnSize));
	Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(CViewerFrame::OnClose));
	Connect(wxEVENT_CLOSESCANNER, wxCommandEventHandler(CViewerFrame::HideScanner));
	Connect(wxID_PRINT, wxEVT_MENU, wxCommandEventHandler(CViewerFrame::OnPrint));
	Connect(ID_EXPORT, wxEVT_MENU, wxCommandEventHandler(CViewerFrame::OnExport));
#ifdef WIN32
	Connect(ID_ASSOCIATE, wxEVT_MENU, wxCommandEventHandler(CViewerFrame::OnAssociate));
#endif
	//Connect(ID_SCANNER, wxEVT_MENU, wxCommandEventHandler(CViewerFrame::OnScanner));
	mainWindow->Bind(wxEVT_CHAR_HOOK, &CViewerFrame::OnKeyDown, this);
	
    if (folderList.size() == 0)
    {
        wxString platformName = "";
        CRegardsConfigParam * config = CParamInit::getInstance();
        if (config != nullptr)
        { 
            platformName = config->GetOpenCLPlatformName();
        }
              
		if (config->GetIsOpenCLSupport())
		{
			//OpenCL auto selected
			if (platformName == "")
			{
				wxString deviceName = "";
				int indexDevice = -1;
				OpenCLPlatform * openCLPlatformSelected = nullptr;
				vector<OpenCLPlatform *> listPlatform = COpenCLPlatformList::GetPlatform();

				for (OpenCLPlatform * openCLPlatform : listPlatform)
				{

					platformName == openCLPlatform->platformName;
					openCLPlatformSelected = openCLPlatform;
					break;
				}

				if (openCLPlatformSelected != nullptr)
				{
					vector<OpenCLDevice *> listDevice = COpenCLDeviceList::GetPlatformDevice(openCLPlatformSelected);

					for (OpenCLDevice * openCLDevice : listDevice)
					{
						indexDevice = openCLDevice->deviceIndex;
						deviceName = openCLDevice->deviceName;
						break;
					}
				}



				CRegardsConfigParam* config = CParamInit::getInstance();
				if (config != nullptr)
				{
					config->SetOpenCLPlatformIndex(indexDevice);
					config->SetOpenCLPlatformName(platformName);
				}

				//ShowOpenCLConfiguration(false);
				//Exit();
			}
		}
    }

	if (fileToOpen != "")
	{
		mainWindow->OpenFile(fileToOpen);
	}
	else
	{
		if (folderList.size() == 0)
			mainWindow->OpenFolder();
            
	}
    
    mainInterface->HideAbout();
	Connect(TIMER_LOADPICTURE, wxEVT_TIMER, wxTimerEventHandler(CViewerFrame::OnTimerLoadPicture), nullptr, this);
}


void CViewerFrame::ShowScanner()
{
	if (frameScanner != nullptr)
	{
		frameScanner->Show(true);
		frameScanner->Raise();
	}
	else
	{
		frameScanner = new CScannerFrame("Regards PDF", mainInterface, wxPoint(50, 50), wxSize(1200, 800));
		frameScanner->Centre(wxBOTH);
		frameScanner->Show(true);
	}
	frameScanner->OnOpen();
}

void CViewerFrame::OnScanner(wxCommandEvent& event)
{
	ShowScanner();
}

void CViewerFrame::HideScanner(wxCommandEvent& event)
{
	if (frameScanner != nullptr)
	{
		frameScanner->Show(false);
		
	}
}

void CViewerFrame::OnExport(wxCommandEvent& event)
{
	wxString filename = mainWindow->GetFilename();
	if (filename != "")
	{
		CSavePicture::SavePicture(this, nullptr, filename);
	}
}
#ifdef WIN32
void CViewerFrame::OnAssociate(wxCommandEvent& event)
{
	
	wxString path = CFileUtility::GetProgramFolderPath() + "\\associate.exe";
	ShellExecute(this->GetHWND(), L"runas", path, 0, 0, SW_SHOWNORMAL);
	//wxExecute(path, wxEXEC_SYNC);
	/*
	Association associate(this);
	associate.ShowModal();
	if (associate.IsOk())
		associate.AssociateExtension();
	*/

}
#endif
void CViewerFrame::OnPrint(wxCommandEvent& event)
{
	CLibPicture libPicture;
	wxString filename = mainWindow->GetFilename();
	if (filename != "")
	{
		CImageLoadingFormat * image = libPicture.LoadPicture(filename);
		if (image != nullptr)
			PrintPreview(image);
	}
}

bool CViewerFrame::RemoveFSEntry(const wxString& dirPath)
{
     if(m_watcher == NULL)
         return false;
    
     if(wxDirExists(dirPath) == false)
         return false;    
    
     wxFileName dirname(dirPath, "" );
     return m_watcher->Remove(dirname);
}

bool CViewerFrame::AddFSEntry(const wxString& dirPath)
 {
     if(m_watcher == NULL)
         return false;

     if(wxDirExists(dirPath) == false)
         return false;

     wxFileName dirname(dirPath, "" );
     return m_watcher->AddTree(dirname,wxFSW_EVENT_CREATE | wxFSW_EVENT_DELETE | wxFSW_EVENT_RENAME | wxFSW_EVENT_MODIFY);
 }

void CViewerFrame::OnClose(wxCloseEvent& event)
{
    //onExit = true;
    //mainInterface->Close();
	Exit();
}

void CViewerFrame::OnTimerLoadPicture(wxTimerEvent& event)
{
	mainWindow->ImageSuivante();
	loadPictureTimer->Stop();
}

void CViewerFrame::OnHelp(wxCommandEvent& event)
{
    wxString helpFile = CFileUtility::GetResourcesFolderPath();
    helpFile.Append("//NoticeRegardsViewer.pdf");
    wxLaunchDefaultApplication(helpFile);
}

void CViewerFrame::OnInterpolationFilter(wxCommandEvent& event)
{
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
	{
		CInterpolationFilterSelect filterDialog(this);
		filterDialog.ShowModal();
		if (filterDialog.IsOk())
			config->SetInterpolationType(filterDialog.GetFilterIndex());
	}
}

void CViewerFrame::CheckAllProcessEnd(wxTimerEvent& event)
{
	nbTime++;

	if(nbTime < 50)
	{
		for (CMasterWindow * window : CMasterWindow::listMainWindow)
		{
			if(!window->GetProcessEnd())
			{
				wxString message = window->GetWaitingMessage();
				mainWindowWaiting->SetTexte(message);
				exitTimer->Start(1000, wxTIMER_ONE_SHOT);
				return;
			}
		}
	}

	onExit = true;
	Exit();
}

void CViewerFrame::Exit()
{
    if(!onExit)
    {
		nbTime = 0;
		CWindowMain::SetEndProgram();
		mainWindowWaiting = new CWaitingWindow(this, wxID_ANY);
		mainWindow->Show(false);
		mainWindowWaiting->Show(true);
		mainWindowWaiting->SetSize(0, 0, mainWindow->GetWindowWidth(), mainWindow->GetWindowHeight());
		mainWindowWaiting->Refresh();

		exitTimer->Start(1000, wxTIMER_ONE_SHOT);
	}
	else
	{
        CMainThemeInit::SaveTheme();
		mainInterface->Close();
		onExit = true;

    }
    

}

void CViewerFrame::OnKeyDown(wxKeyEvent& event)
{
	if (event.m_keyCode == WXK_ESCAPE && fullscreen)
	{
		mainWindow->SetScreen();
	}
	else
	{
		switch (event.GetKeyCode())
		{
			case WXK_ESCAPE:
			{
				mainWindow->SetScreen();
			}
			break;

			case WXK_PAGEUP:
			{
                printf("Image Suivante \n");
				mainWindow->ImageSuivante();
			}
			break;

			case WXK_PAGEDOWN:
			{
				mainWindow->ImagePrecedente();
			}
			break;


			case WXK_SPACE:
			{
                printf("Image Suivante \n");
				mainWindow->ImageSuivante();
				//if (!loadPictureTimer->IsRunning())
				//	loadPictureTimer->Start(10);
			}
			break;

			case WXK_END:
			{
				mainWindow->ImageFin();
			}
			break;

			case WXK_HOME:
			{
				mainWindow->ImageDebut();
			}
			break;

			case WXK_F5:
			{
				if (!fullscreen)
				{
					mainWindow->SetFullscreen();
					fullscreen = true;
				}
			}
			break;

		}
	}

	/*
	if (event.m_keyCode == WXK_ESCAPE && fullscreen)
	{
		mainWindow->SetScreen();
	}
	else if (event.m_keyCode == WXK_SPACE)
	{
		//mainWindow->ImageSuivante(); 
	}
	*/
	event.Skip();
}

void CViewerFrame::SetFullscreen()
{
	fullscreen = true;
	this->ShowFullScreen(true);
	mainWindow->SetFullscreenMode();
}

void CViewerFrame::SetScreen()
{
	fullscreen = false;
	this->ShowFullScreen(false);
}

void CViewerFrame::SetWindowTitle(const wxString &libelle)
{
	SetLabel(libelle);
}

CViewerFrame::~CViewerFrame()
{
	if (exitTimer->IsRunning())
		exitTimer->Stop();

	delete exitTimer;

	if (loadPictureTimer->IsRunning())
		loadPictureTimer->Stop();

	delete(loadPictureTimer);

	if (mainWindow != nullptr)
		delete(mainWindow);

	if(mainWindowWaiting != nullptr)
		delete(mainWindowWaiting);

	if (viewerTheme != nullptr)
		delete(viewerTheme);

	viewerParam->SaveFile();


	if (viewerParam != nullptr)
		delete(viewerParam);

    
    if(!onExit)
        Exit();
}

void CViewerFrame::SetText(const int &numPos, const wxString &libelle)
{
	if (mainWindow != nullptr)
		mainWindow->SetText(numPos, libelle);
}

void CViewerFrame::SetRangeProgressBar(const int &range)
{
	if (mainWindow != nullptr)
		mainWindow->SetRangeProgressBar(range);
}

void CViewerFrame::SetPosProgressBar(const int &position)
{
	if (mainWindow != nullptr)
		mainWindow->SetPosProgressBar(position);
}

void CViewerFrame::OnConfiguration(wxCommandEvent& event)
{
	CRegardsConfigParam * regardsParam = (CRegardsConfigParam*)CParamInit::getInstance();
	int pictureSize = regardsParam->GetFaceDetectionPictureSize();
	ConfigRegards configFile(this);
	configFile.ShowModal();
	if (configFile.IsOk())
	{
		int newPictureSize = regardsParam->GetFaceDetectionPictureSize();
		if(pictureSize != newPictureSize)
		{
			//Suppression de toutes les Faces
			CSQLRemoveData::DeleteFaceDatabase();
            mainWindow->OnFacePertinence();
			

			wxWindow * window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
			if (window)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_UPDATECRITERIA);
				evt.SetExtraLong(0);
				window->GetEventHandler()->AddPendingEvent(evt);
			}
		}
	}
}

void CViewerFrame::ShowOpenCLConfiguration(const bool &showRestart)
{
	int supportOpenCL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenCL = config->GetIsOpenCLSupport();

	if (supportOpenCL)
	{

		OpenCLDialog configFile(this);
		configFile.ShowModal();
		if (configFile.IsOk())
		{
			wxString labelRestart = CLibResource::LoadStringFromResource(L"labelRestart", 1);//L"&Thumbnail";
			wxString labelInformations = CLibResource::LoadStringFromResource(L"labelInformations", 1);//L"&Help";
			CRegardsConfigParam* config = CParamInit::getInstance();
			if (config != nullptr)
			{
				config->SetOpenCLPlatformIndex(configFile.GetDeviceIndex());
				config->SetOpenCLPlatformName(configFile.GetPlatformName());
			}

			if (showRestart)
				wxMessageBox(labelRestart, labelInformations);
		}
	}
	else
	{
		wxString labelInformations = CLibResource::LoadStringFromResource(L"labelInformations", 1);//L"&Help";
		wxMessageBox("OpenCL device Required", labelInformations);
	}
}

void CViewerFrame::OnOpenCLConfiguration(wxCommandEvent& event)
{
    ShowOpenCLConfiguration(true);
}

void CViewerFrame::OnFileSystemModified(wxFileSystemWatcherEvent& event)
{
    if(mainWindow != nullptr)
    {
        if(!mainWindow->IsVideo())
        {
            printf("OnFileSystemModified \n");
            wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REFRESHFOLDER);
            mainWindow->GetEventHandler()->AddPendingEvent(evt);
        }
    }
}

void CViewerFrame::OnIconSizeLess(wxCommandEvent& event)
{
    float ratio = 1.0;
    CRegardsConfigParam * config = CParamInit::getInstance();
    if(config != nullptr)
    {
        ratio = config->GetIconSizeRatio();
        if(ratio > 1.0)
            ratio = ratio - 0.25;
        config->SetIconSizeRatio(ratio);
        
        mainWindow->UpdateScreenRatio();
        //Refresh();
    }
    
}

void CViewerFrame::OnIconSizeMore(wxCommandEvent& event)
{
    float ratio = 1.0;
    CRegardsConfigParam * config = CParamInit::getInstance();
    if(config != nullptr)
    {
        ratio = config->GetIconSizeRatio();
        if(ratio < 2.0)
            ratio = ratio + 0.25;
        config->SetIconSizeRatio(ratio);
        
        mainWindow->UpdateScreenRatio();
        //Refresh();
    }
}

void CViewerFrame::OnExit(wxCommandEvent& event)
{
    //onExit = true;
    //mainInterface->Close();
	Exit();

}
void CViewerFrame::OnAbout(wxCommandEvent& event)
{
	mainInterface->ShowAbout();
}

void CViewerFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}

void CViewerFrame::PrintImagePreview(CRegardsBitmap * imageToPrint)
{
	// Pass two printout objects: for preview, and possible printing.
	wxPrintData * g_printData = CPrintEngine::GetPrintData();
	wxPrintDialogData printDialogData(*g_printData);

	wxPrintPreview * preview = new wxPrintPreview(new CBitmapPrintout(imageToPrint), 0, &printDialogData);
	if (!preview->IsOk())
	{
		delete preview;
		wxLogError(wxT("There was a problem previewing.\nPerhaps your current printer is not set correctly?"));
		return;
	}

	wxString picture_print_label = CLibResource::LoadStringFromResource(L"PicturePrintPreview", 1);
	wxPreviewFrame *frame =
		new wxPreviewFrame(preview, this, picture_print_label, wxPoint(100, 100), wxSize(600, 650));
	frame->Centre(wxBOTH);
	frame->InitializeWithModality(m_previewModality);
	frame->Show();

	//delete imageToPrint;
}


void CViewerFrame::PrintPreview(CImageLoadingFormat * imageToPrint)
{
	// Pass two printout objects: for preview, and possible printing.
	wxPrintData * g_printData = CPrintEngine::GetPrintData();
	wxPrintDialogData printDialogData(*g_printData);

	wxPrintPreview * preview = new wxPrintPreview(new CBitmapPrintout(imageToPrint), 0, &printDialogData);
	if (!preview->IsOk())
	{
		delete preview;
		wxLogError(wxT("There was a problem previewing.\nPerhaps your current printer is not set correctly?"));
		return;
	}

    wxString picture_print_label = CLibResource::LoadStringFromResource(L"PicturePrintPreview",1);
	wxPreviewFrame *frame =
		new wxPreviewFrame(preview, this, picture_print_label, wxPoint(100, 100), wxSize(600, 650));
	frame->Centre(wxBOTH);
	frame->InitializeWithModality(m_previewModality);
	frame->Show();

	//delete imageToPrint;
}

void CViewerFrame::OnEraseDatabase(wxCommandEvent& event)
{
    wxString erasedatabase = CLibResource::LoadStringFromResource(L"EraseDatabase",1);
    wxString informations = CLibResource::LoadStringFromResource(L"labelInformations",1);
	if (wxMessageBox(erasedatabase, informations, wxYES_NO | wxICON_WARNING) == wxYES)
	{
		CSQLRemoveData::DeleteCatalog(1);
		if (mainWindow != nullptr)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REFRESHFOLDER);
			mainWindow->GetEventHandler()->AddPendingEvent(evt);
		}

#ifndef __NOFACE_DETECTION__
		CListFace * listFace = (CListFace *)this->FindWindowById(LISTFACEID);
		if(listFace != nullptr)
		{
			wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
			listFace->GetEventHandler()->AddPendingEvent(evt);
		}
#endif
	}
}

void CViewerFrame::OnCategoryDetection(wxCommandEvent& event)
{
	/*
	CCategoryDetectionDlg catergoryDetection(this);
	catergoryDetection.ShowModal();


	CListFace * listFace = (CListFace *)this->FindWindowById(LISTFACEID);
	if (listFace != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_THUMBNAILREFRESH);
		listFace->GetEventHandler()->AddPendingEvent(evt);
	}
	*/
}

void CViewerFrame::OnFaceDetection(wxCommandEvent& event)
{
	/*
	CFaceDetectionDlg faceDetection(this);
	faceDetection.Start();
	faceDetection.ShowModal();

	CListFace * listFace = (CListFace *)this->FindWindowById(LISTFACEID);
	if (listFace != nullptr)
	{
		wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
		listFace->GetEventHandler()->AddPendingEvent(evt);
	}

	*/
}

void CViewerFrame::OnPageSetup(wxCommandEvent& WXUNUSED(event))
{
	wxPrintData * g_printData = CPrintEngine::GetPrintData();
	wxPageSetupDialogData * g_pageSetupData = CPrintEngine::GetPageSetupDialogData();
	(*g_pageSetupData) = *g_printData;

	wxPageSetupDialog pageSetupDialog(this, g_pageSetupData);
	pageSetupDialog.ShowModal();

	(*g_printData) = pageSetupDialog.GetPageSetupDialogData().GetPrintData();
	(*g_pageSetupData) = pageSetupDialog.GetPageSetupDialogData();
}

void CViewerFrame::OnFacePertinence(wxCommandEvent& event)
{
	CMainParam * viewerParam = (CMainParam *)CMainParamInit::getInstance();
	double pertinence = 0.0;
	if(viewerParam != nullptr)
	{
		pertinence = viewerParam->GetPertinenceValue();
		PertinenceValue configFile(this);
		configFile.SetValue(pertinence);
		configFile.ShowModal();
		if (configFile.IsOk())
		{
			viewerParam->SetPertinenceValue(configFile.GetValue());
			mainWindow->OnFacePertinence();
		}
	}
}
#ifdef __WXMAC__
void CViewerFrame::OnPageMargins(wxCommandEvent& WXUNUSED(event))
{
    wxPrintData * g_printData = CPrintEngine::GetPrintData();
    wxPageSetupDialogData * g_pageSetupData = CPrintEngine::GetPageSetupDialogData();
    (*g_pageSetupData) = *g_printData;

	wxMacPageMarginsDialog pageMarginsDialog(this, g_pageSetupData);
	pageMarginsDialog.ShowModal();

	(*g_printData) = pageMarginsDialog.GetPageSetupDialogData().GetPrintData();
	(*g_pageSetupData) = pageMarginsDialog.GetPageSetupDialogData();
}
#endif

wxBEGIN_EVENT_TABLE(CViewerFrame, wxFrame)
EVT_MENU(ID_Hello, CViewerFrame::OnHello)
EVT_MENU(wxID_HELP, CViewerFrame::OnHelp)
EVT_MENU(ID_Configuration, CViewerFrame::OnConfiguration)
EVT_MENU(ID_OpenCL, CViewerFrame::OnOpenCLConfiguration)
EVT_MENU(ID_SIZEICONLESS, CViewerFrame::OnIconSizeLess)
EVT_MENU(ID_SIZEICONMORE, CViewerFrame::OnIconSizeMore)
EVT_MENU(ID_FACEDETECTION, CViewerFrame::OnFaceDetection)
EVT_MENU(ID_CATEGORYDETECTION, CViewerFrame::OnCategoryDetection)
EVT_MENU(ID_ERASEDATABASE, CViewerFrame::OnEraseDatabase)
EVT_MENU(ID_INTERPOLATIONFILTER, CViewerFrame::OnInterpolationFilter)
EVT_MENU(wxID_ABOUT, CViewerFrame::OnAbout)
EVT_MENU(WXPRINT_PAGE_SETUP, CViewerFrame::OnPageSetup)
EVT_MENU(wxID_EXIT, CViewerFrame::OnExit)
#ifdef __WXMAC__
	EVT_MENU(WXPRINT_PAGE_MARGINS, CViewerFrame::OnPageMargins)
#endif
wxEND_EVENT_TABLE()

