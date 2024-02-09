#include <header.h>
#include "MainWindow.h"
#include <LibResource.h>
#include "ExportDiaporama.h"
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <libPicture.h>
#include "window_mode_id.h"
#include <ImageLoadingFormat.h>
#include <SqlFaceRecognition.h>
#include "ThumbnailViewerPicture.h"
#include "ThumbnailBuffer.h"
#include "SqlFindPhotos.h"
#include <SqlThumbnail.h>
#include <BitmapWndViewer.h>
#include "Toolbar.h"
#include "ToolbarViewerMode.h"
#include <StatusBarInterface.h>
#include "CentralWindow.h"
#include "FileUtility.h"
#include "CategoryFolderWindow.h"
#include <wx/filename.h>
#include <window_id.h>
#include <SqlFindFolderCatalog.h>
#include <SQLRemoveData.h>
#include <SqlInsertFile.h>
#include "StatusText.h"
#include <ThumbnailMessage.h>
#include <SqlThumbnailVideo.h>
#include "FaceInfosUpdate.h"
#include <ShowElement.h>
#include <wx/filedlg.h>
#include <SqlFaceLabel.h>
#include "SqlFacePhoto.h"
#include <FiltreEffetCPU.h>
#include "CheckVersion.h"
#include <IBitmapWnd.h>
#include <ListPicture.h>
#include <ThumbnailFolder.h>
#include <MainTheme.h>
#include <MainThemeInit.h>
#include <SqlPhotosWithoutThumbnail.h>
#include <ParamInit.h>
#include "FolderProcess.h"
#include <wx/busyinfo.h>
#include "md5check.h"
#include <ImageVideoThumbnail.h>
#include <ThreadLoadingBitmap.h>
using namespace Regards::Picture;
using namespace Regards::Control;
using namespace Regards::Viewer;
using namespace std;
using namespace Regards::Sqlite;

bool firstTime = true;


class CFolderFiles
{
public:
	vector<wxString> pictureFiles;
	wxString folderName;
};

class CThreadVideoData
{
public:
	CThreadVideoData()
	{
		mainWindow = nullptr;
	}

	~CThreadVideoData();

	CMainWindow* mainWindow;
	wxString video;
};



CThreadVideoData::~CThreadVideoData()
{
}

wxDEFINE_EVENT(wxVERSION_UPDATE_EVENT, wxCommandEvent);
wxDEFINE_EVENT(wxEVENT_SETSCREEN, wxCommandEvent);

extern wxImage defaultPicture;

CMainWindow::CMainWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface* statusbar, const bool& openFirstFile, const wxString & fileToOpen)
	: CWindowMain("CMainWindow", parent, id)
{
	fullscreen = false;
	nbProcessMD5 = 0;
	showToolbar = true;
	multithread = true;
	needToReload = false;
	typeAffichage = THUMB_SHOW_ALL;
	updateCriteria = true;

	refreshFolder = false;
	numElementTraitement = 0;
	start = true;
	criteriaSendMessage = false;
	checkVersion = true;
	folderProcess = new CFolderProcess(this);


	CMainTheme* viewerTheme = CMainThemeInit::getInstance();
	viewerParam = CMainParamInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetMainToolbarTheme(&theme);
		toolbar = new CToolbar(this, wxID_ANY, theme, false);

		CThemeToolbar theme_infos;
		viewerTheme->GetInfosToolbarTheme(&theme_infos);
		theme_infos.position = NAVIGATOR_CENTER;
		toolbarViewerMode = new CToolbarViewerMode(this, wxID_ANY, theme_infos, this, false);
		//wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, CToolbarInterface* toolbarInterface, const bool& vertical
	}

	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetSplitterTheme(&theme);
		centralWnd = new CCentralWindow(this, CENTRALVIEWERWINDOWID, theme, false);
	}
	this->statusBarViewer = statusbar;

	/*----------------------------------------------------------------------
	 *
	 * Manage Event
	 *
	 ----------------------------------------------------------------------*/

	Connect(wxEVENT_FACEINFOSUPDATE, wxCommandEventHandler(CMainWindow::OnFaceInfosUpdate));
	Connect(wxEVENT_SETSCREEN, wxCommandEventHandler(CMainWindow::SetScreenEvent));
	Connect(wxEVENT_INFOS, wxCommandEventHandler(CMainWindow::OnUpdateInfos));
	Connect(wxEVENT_REFRESHFOLDERLIST, wxCommandEventHandler(CMainWindow::RefreshFolderList));
	Connect(TOOLBAR_UPDATE_ID, wxCommandEventHandler(CMainWindow::OnShowToolbar));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CMainWindow::OnIdle));

	Connect(wxEVENT_UPDATEFOLDER, wxCommandEventHandler(CMainWindow::OnUpdateFolder));
	Connect(wxEVENT_ONPICTURECLICK, wxCommandEventHandler(CMainWindow::OnPictureClick));
	Connect(wxEVT_CRITERIACHANGE, wxCommandEventHandler(CMainWindow::CriteriaChange));
	Connect(wxEVENT_PICTUREVIDEOCLICK, wxCommandEventHandler(CMainWindow::PictureVideoClick));
	Connect(wxEVENT_REFRESHFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::InitPictures));
	Connect(wxEVENT_REFRESHPICTURE, wxCommandEventHandler(CMainWindow::OnRefreshPicture));
	Connect(wxEVENT_MD5CHECKING, wxCommandEventHandler(CMainWindow::Md5Checking));
	Connect(wxEVENT_SETSTATUSTEXT, wxCommandEventHandler(CMainWindow::OnStatusSetText));
	Connect(wxEVT_EXIT, wxCommandEventHandler(CMainWindow::OnExit));
	Connect(wxEVENT_SETRANGEPROGRESSBAR, wxCommandEventHandler(CMainWindow::OnSetRangeProgressBar));
	Connect(wxEVENT_PRINTPICTURE, wxCommandEventHandler(CMainWindow::PrintPreview));
	Connect(wxEVENT_CRITERIAPHOTOUPDATE, wxCommandEventHandler(CMainWindow::OnCriteriaUpdate));
	Connect(wxEVENT_UPDATESTATUSBARMESSAGE, wxCommandEventHandler(CMainWindow::UpdateStatusBarMessage));
	Connect(wxEVENT_FACEADD, wxCommandEventHandler(CMainWindow::OnFaceAdd));
	Connect(wxEVENT_PRINT, wxCommandEventHandler(CMainWindow::OnPrint));
	Connect(wxEVENT_SETVALUEPROGRESSBAR, wxCommandEventHandler(CMainWindow::OnSetValueProgressBar));
	Connect(wxEVENT_SHOWSCANNER, wxCommandEventHandler(CMainWindow::OnScanner));
	Connect(wxEVENT_OPENFILEORFOLDER, wxCommandEventHandler(CMainWindow::OnOpenFileOrFolder));
	Connect(wxEVENT_EDITFILE, wxCommandEventHandler(CMainWindow::OnEditFile));
	Connect(wxEVENT_EXPORTFILE, wxCommandEventHandler(CMainWindow::OnExportFile));
	Connect(wxEVENT_ENDCOMPRESSION, wxCommandEventHandler(CMainWindow::OnEndDecompressFile));
	Connect(wxEVENT_UPDATETHUMBNAILEXIF, wxCommandEventHandler(CMainWindow::OnUpdateExifThumbnail));
	Connect(wxEVENT_EXPORTDIAPORAMA, wxCommandEventHandler(CMainWindow::OnExportDiaporama));
	Connect(wxEVENT_DELETEFACE, wxCommandEventHandler(CMainWindow::OnDeleteFace));
	Connect(wxEVENT_ICONEUPDATE, wxCommandEventHandler(CMainWindow::UpdateThumbnailIcone));
	Connect(wxVERSION_UPDATE_EVENT, wxCommandEventHandler(CMainWindow::OnVersionUpdate));
	Connect(wxEVENT_UPDATEMESSAGE, wxCommandEventHandler(CMainWindow::UpdateMessage));
	Connect(wxEVENT_REFRESHTHUMBNAIL, wxCommandEventHandler(CMainWindow::OnRefreshThumbnail));
	Connect(wxEVENT_ICONETHUMBNAILGENERATION, wxCommandEventHandler(CMainWindow::OnProcessThumbnail));
	/*----------------------------------------------------------------------
	 *
	 * Manage Event
	 *
	 ----------------------------------------------------------------------*/

	statusBar = new wxStatusBar(this, wxID_ANY, wxSTB_DEFAULT_STYLE, "wxStatusBar");

	int tabWidth[] = {100, 300, 300, 300};
	statusBar->SetFieldsCount(4);
	statusBar->SetStatusWidths(4, tabWidth);

	progressBar = new wxGauge(statusBar, wxID_ANY, 200, wxPoint(1000, 0), wxSize(200, statusBar->GetSize().y),
	                          wxGA_HORIZONTAL);
	progressBar->SetRange(100);
	progressBar->SetValue(50);
	//refreshFolder = true;
	processIdle = true;
	//updateFolder = true;
	listProcessWindow.push_back(this);
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		firstFileToShow = localFilename = config->GetLastShowPicture();
	if (fileToOpen != "")
		firstFileToShow = localFilename = fileToOpen;


	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		faceDetection = regardsParam->GetFaceDetection();
	}

	UpdateFolderStatic();

	versionUpdate = new std::thread(NewVersionAvailable, this);
	isCheckNewVersion = true;

}

void CMainWindow::OnRefreshThumbnail(wxCommandEvent& event)
{
	nbProcess = 0;
	listFile.clear();
	processIdle = true;
}

void CMainWindow::UpdateThumbnailIcone(wxCommandEvent& event)
{
    //printf("CMainWindow::UpdateThumbnailIcone \n");
	nbProcess--;
	auto localevent = new wxCommandEvent(wxEVENT_ICONEUPDATE);
	localevent->SetClientData(event.GetClientData());
	wxQueueEvent(centralWnd, localevent);
}

void CMainWindow::SetPictureMode()
{
	setPictureMode = true;
	processIdle = true;

	if (toolbarViewerMode != nullptr)
		toolbarViewerMode->SetPictureWindowPush();
}

void CMainWindow::SetViewerMode()
{
	setViewerMode = true;
	processIdle = true;

	if (toolbarViewerMode != nullptr)
		toolbarViewerMode->SetViewerWindowPush();
}


void CMainWindow::OnVersionUpdate(wxCommandEvent& event)
{
	cout << "OnVersionUpdate" << endl;

	int hasUpdate = event.GetInt();
	if (hasUpdate)
	{
		if (toolbar != nullptr)
			toolbar->SetUpdateVisible(true);

		
	}

	if (versionUpdate != nullptr)
	{
		versionUpdate->join();
		delete versionUpdate;
		versionUpdate = nullptr;
	}

	isCheckNewVersion = false;

}


void CMainWindow::NewVersionAvailable(void* param)
{
	int hasUpdate = 0;
	CToolbar* toolbar = (CToolbar*)param;
	wxString localVersion = CLibResource::LoadStringFromResource("REGARDSVERSION", 1);
	wxString serverURL = CLibResource::LoadStringFromResource("ADRESSEWEBVERSION", 1);
	CCheckVersion _checkVersion(serverURL);
	wxString serverVersion = _checkVersion.GetLastVersion();
	serverVersion = serverVersion.SubString(0, serverVersion.length() - 2);

	long localValueVersion;
	long localServerVersion;

	localVersion.Replace(".", "");
	serverVersion.Replace(".", "");

	if (!localVersion.ToLong(&localValueVersion)) { /* error! */ }
	if (!serverVersion.ToLong(&localServerVersion)) { /* error! */ }

	printf("serverVersion %d \n", localServerVersion);
	printf("localVersion %d \n", localValueVersion);

	if (serverVersion != "error" && serverVersion != "")
	{
		if (localValueVersion < localServerVersion)
		{
			hasUpdate = 1;
		}
	}

	wxCommandEvent event(wxVERSION_UPDATE_EVENT);
	event.SetInt(hasUpdate);
	wxPostEvent(toolbar, event);
}

void CMainWindow::ClickShowButton(const int& id, const int& refresh)
{
	switch(id)
	{
		case WINDOW_VIEWER:
		{
			wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
			wxCommandEvent event(wxEVENT_SETMODEVIEWER);
			event.SetInt(1);
			wxPostEvent(central, event);
		}
		break;

		case WINDOW_FACE:
		{
			wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
			wxCommandEvent event(wxEVENT_SETMODEVIEWER);
			event.SetInt(2);
			wxPostEvent(central, event);
		}
		break;

		case WINDOW_EXPLORER:
		{
			wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
			wxCommandEvent event(wxEVENT_SETMODEVIEWER);
			event.SetInt(3);
			wxPostEvent(central, event);
		}
		break;

		case WINDOW_PICTURE:
		{
			wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
			wxCommandEvent event(wxEVENT_SETMODEVIEWER);
			event.SetInt(4);
			wxPostEvent(central, event);
		}
		break;

	}
}


bool CMainWindow::IsVideo()
{
	if (centralWnd != nullptr)
		return centralWnd->IsVideo();

	return false;
}


void CMainWindow::OnFaceAdd(wxCommandEvent& event)
{

	//Modification du titre
	wxTextEntryDialog changeLabel(this,"", "Add a New Person");
	if (changeLabel.ShowModal() == wxID_OK)
	{
		vector<int> * numFace = (vector<int> *)event.GetClientData();
		wxString newLabel = changeLabel.GetValue();
		if (newLabel.size() > 3)
		{
			CSqlFaceLabel sqlFaceLabel;
			sqlFaceLabel.InsertFaceLabel(numFace->at(0), newLabel, 1);

			CSqlFaceRecognition faceRecognition;
			for (int i = 0; i < numFace->size(); i++)
				faceRecognition.MoveFaceRecognition(numFace->at(i), numFace->at(0));

			numFace->clear();
			delete numFace;

			wxWindow* window = this->FindWindowById(LISTFACEID);
			if (window != nullptr)
			{
				wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
				window->GetEventHandler()->AddPendingEvent(evt);
			}

			auto eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
			wxQueueEvent(this, eventChange);
		}
		else
		{
			wxString wronglabelsize = CLibResource::LoadStringFromResource(L"wronglabelsize", 1);
			wxString error_labelsize = CLibResource::LoadStringFromResource(L"erroronlabelsize", 1);
			wxMessageBox(wronglabelsize, error_labelsize, wxICON_ERROR);
		}
	}

}


void CMainWindow::OnDeleteFace(wxCommandEvent& event)
{
	wxString information = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
	wxString newVersionAvailable = CLibResource::LoadStringFromResource("LBLNEWVERSIONAVAILABLE", 1);

	int answer = wxMessageBox("Do you want to delete this Person ?", information, wxYES_NO | wxCANCEL, nullptr);
	if (answer == wxYES)
	{
		int faceId = event.GetInt();
		CSqlFacePhoto sqlFacePhoto;
		sqlFacePhoto.DeleteNumFaceMaster(faceId);
		wxWindow* window = this->FindWindowById(LISTFACEID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		auto eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
		wxQueueEvent(this, eventChange);
	}
}

void CMainWindow::OnExportDiaporama(wxCommandEvent& event)
{
	if (exportDiaporama != nullptr)
	{
		exportDiaporama->OnExportDiaporama();
	}
}

void CMainWindow::OnUpdateExifThumbnail(wxCommandEvent& event)
{
	int numPhoto = event.GetInt();
	int rotate = event.GetExtraLong();

	wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
	CFiltreEffetCPU::LoadAndRotate(thumbnail, rotate);
	wxWindow* window = this->FindWindowById(THUMBNAILVIEWERPICTURE);
	if (window != nullptr)
	{
		wxCommandEvent evt(wxEVENT_REFRESHTHUMBNAIL);
		evt.SetInt(numPhoto);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CMainWindow::OnEndDecompressFile(wxCommandEvent& event)
{
	if (exportDiaporama != nullptr)
	{
		int ret = event.GetInt();
		exportDiaporama->OnEndDecompressFile(ret);
		delete exportDiaporama;
		exportDiaporama = nullptr;
	}
}

void CMainWindow::ExportVideo(const wxString& filename)
{
	if (!wxFileExists(filename))
		return;

	if (exportDiaporama != nullptr)
		delete exportDiaporama;

	exportDiaporama = new CExportDiaporama(this);
	exportDiaporama->ExportVideo(filename);
}

void CMainWindow::OnExportFile(wxCommandEvent& event)
{
	if (centralWnd->IsVideo())
	{
#ifdef __APPLE__
		ExportVideo(this->centralWnd->GetFilename());
#else
#ifdef __WXMSW__
		wxString pathProgram = "RegardsVideoConverter.exe \"" + this->centralWnd->GetFilename() + "\"";
#else
		wxString pathProgram = "./RegardsViewer \"" + this->centralWnd->GetFilename() + "\" -p RegardsConverter";
		cout << "Path Program" << pathProgram << endl;
#endif
		wxExecute(pathProgram);
#endif
		
	}
	else
	{
		CBitmapWndViewer* viewer = nullptr;
		auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(BITMAPWINDOWVIEWERID));
		if (bitmapWindow != nullptr)
		{
			viewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
		}

		//auto bitmapWindow = dynamic_cast<CBitmapWndViewer*>(this->FindWindowById(BITMAPWINDOWVIEWERID));
		if (viewer != nullptr)
			viewer->ExportPicture();
	}
}

void CMainWindow::OnEditFile(wxCommandEvent& event)
{
	CMainParam* config = CMainParamInit::getInstance();
	wxString pathProgram = "";
	wxString title = CLibResource::LoadStringFromResource(L"LBLSELECTPICTUREEDITOR", 1);
	if (centralWnd->IsVideo())
	{
		if (config != nullptr)
			pathProgram = config->GetPathForVideoEdit();

		title = CLibResource::LoadStringFromResource(L"LBLSELECTVIDEOEDITOR", 1);
	}
	else
	{
		if (config != nullptr)
			pathProgram = config->GetPathForPictureEdit();
	}

	if (!wxFileExists(pathProgram))
		pathProgram = "";

	if (pathProgram.empty())
	{
		const wxString allfiles = CLibResource::LoadStringFromResource(L"LBLALLFILES", 1);
		wxFileDialog openFileDialog(nullptr, title, "", "",
		                            allfiles, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		if (openFileDialog.ShowModal() == wxID_OK)
			pathProgram = openFileDialog.GetPath();
	}

	if (centralWnd->IsVideo())
	{
		if (config != nullptr)
			config->SetPathForVideoEdit(pathProgram);
	}
	else
	{
		if (config != nullptr)
			config->SetPathForPictureEdit(pathProgram);
	}


	pathProgram = "\"" + pathProgram + "\" \"" + localFilename + "\"";
	wxExecute(pathProgram);
}


void CMainWindow::OnPrint(wxCommandEvent& event)
{
	bool showPrintPicture = true;
	if (centralWnd->IsVideo())
	{
		auto video = static_cast<CShowElement*>(this->FindWindowById(SHOWBITMAPVIEWERID));
		if (video != nullptr)
		{
			if (video->IsPause())
			{
				cv::Mat image = video->GetVideoBitmap();
				if (!image.empty())
				{
					auto imageLoading = new CImageLoadingFormat();
					imageLoading->SetPicture(image);
					statusBarViewer->PrintImagePreview(imageLoading);
					showPrintPicture = false;
				}
			}
		}
	}
	if (showPrintPicture)
	{
		CLibPicture libPicture;
		CImageLoadingFormat* image = libPicture.LoadPicture(localFilename);
		if (image != nullptr)
		{
			statusBarViewer->PrintPreview(image);
		}
	}
}


void CMainWindow::SetDataToStatusBar(void* thumbMessage, const wxString& picture)
{
	const auto thumbnailMessage = static_cast<CThumbnailMessage*>(thumbMessage);
	const wxString message = picture + to_string(thumbnailMessage->nbPhoto);

	if (thumbnailMessage != nullptr)
	{
		if (statusBarViewer != nullptr)
		{
			statusBarViewer->SetRangeProgressBar(thumbnailMessage->nbElement);
			statusBarViewer->SetText(2, message);
			statusBarViewer->SetPosProgressBar(thumbnailMessage->thumbnailPos + 1);
		}
		delete thumbnailMessage;
	}
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::UpdateStatusBarMessage(wxCommandEvent& event)
{
	const auto thumbnailMessage = static_cast<CThumbnailMessage*>(event.GetClientData());
	if (thumbnailMessage != nullptr)
	{
		const int typeMessage = thumbnailMessage->typeMessage;

		switch (typeMessage)
		{
		case 0:
			{
				const wxString picture = CLibResource::LoadStringFromResource(L"LBLCRITERIANBIMAGE", 1);
				SetDataToStatusBar(event.GetClientData(), picture);
			}
			break;

		case 1:
			{
				if (statusBarViewer != nullptr)
				{
					statusBarViewer->SetRangeProgressBar(thumbnailMessage->nbElement);
					statusBarViewer->SetPosProgressBar(0);
				}
				delete thumbnailMessage;
			}
			break;

		case 2:
			{
				const wxString picture = CLibResource::LoadStringFromResource(L"LBLFOLDERPROCESSING", 1);
				SetDataToStatusBar(event.GetClientData(), picture);
			}
			break;

		case 3:
			{
				const wxString picture = CLibResource::LoadStringFromResource(L"LBLPICTURERENDER", 1);
				SetDataToStatusBar(event.GetClientData(), picture);
			}
			break;

		case 4:
			{
				const wxString picture = CLibResource::LoadStringFromResource(L"LBLFACEPROCESS", 1);
				SetDataToStatusBar(event.GetClientData(), picture);
			}
			break;

		case 5:
			{
				const wxString picture = CLibResource::LoadStringFromResource(L"LBLFACERECOGNITIONPROCESS", 1);
				SetDataToStatusBar(event.GetClientData(), picture);
			}
			break;

		case 6:
			{
				const wxString picture = CLibResource::LoadStringFromResource(L"LBLGEOLOCALISATIONGPS", 1);
				SetDataToStatusBar(event.GetClientData(), picture);
			}
		break;

		default: ;
		}
	}
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::PrintPreview(wxCommandEvent& event)
{
	const auto bitmap = static_cast<CImageLoadingFormat*>(event.GetClientData());

	if (bitmap != nullptr)
	{
		statusBarViewer->PrintImagePreview(bitmap);
	}
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::RefreshFolderList(wxCommandEvent& event)
{
	UpdateFolderStatic();
	//processIdle = true;
}

void CMainWindow::OnCriteriaUpdate(wxCommandEvent& event)
{
  
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
       // int typeAffichage = config->GetTypeAffichage();          
        //if(typeAffichage != SHOW_ALL)
        //{
			UpdateFolderStatic();
      //  }

	}
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
wxString CMainWindow::GetFilename()
{
	return localFilename;
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::CriteriaChange(wxCommandEvent& event)
{
	//Refresh Criteria
	wxWindow* window = FindWindowById(CRITERIAFOLDERWINDOWID);
	if (window)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_UPDATECRITERIA);
		evt.SetExtraLong(1);
		window->GetEventHandler()->AddPendingEvent(evt);
	}

	processIdle = true;
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::OnShowToolbar(wxCommandEvent& event)
{
	ShowToolbar();
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::UpdateScreenRatio()
{
	toolbar->UpdateScreenRatio();
	centralWnd->UpdateScreenRatio();
	this->Resize();
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::OnStatusSetText(wxCommandEvent& event)
{
	auto statusText = static_cast<CStatusText*>(event.GetClientData());
	if (statusText != nullptr)
	{
		statusBar->SetStatusText(statusText->text, statusText->position);
		delete statusText;
	}
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::OnSetValueProgressBar(wxCommandEvent& event)
{
	int position = event.GetInt();
	//cout << "OnSetValueProgressBar Pos : " << position << endl;
	if (progressBar != nullptr)
	{
		if (progressBar->GetRange() > 0)
		{
			if (position >= progressBar->GetRange())
				progressBar->SetValue(progressBar->GetRange() - 1);
			else
				progressBar->SetValue(position);

			progressBar->Refresh();
		}
	}
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::OnSetRangeProgressBar(wxCommandEvent& event)
{
	int range = event.GetInt();
	// cout << "OnSetRangeProgressBar Pos : " << range << endl;
	if (progressBar != nullptr)
		progressBar->SetRange(range);
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::SetText(const int& numPos, const wxString& libelle)
{
	auto event = new wxCommandEvent(wxEVENT_SETSTATUSTEXT);
	auto statusText = new CStatusText();
	statusText->position = numPos;
	statusText->text = libelle;
	event->SetClientData(statusText);
	wxQueueEvent(this, event);

	//statusBar->SetStatusText(libelle, numPos);
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::SetRangeProgressBar(const int& range)
{
	auto event = new wxCommandEvent(wxEVENT_SETRANGEPROGRESSBAR);
	event->SetInt(range);
	wxQueueEvent(this, event);
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::SetPosProgressBar(const int& position)
{
	auto event = new wxCommandEvent(wxEVENT_SETVALUEPROGRESSBAR);
	event->SetInt(position);
	wxQueueEvent(this, event);
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
bool CMainWindow::FindNextValidFile()
{
	bool isFound = false;
	wxString lastFile = centralWnd->ImageFin(false);
	do
	{
		isFound = CThumbnailBuffer::FindValidFile(localFilename);

		if (!isFound)
		{
			if (lastFile == localFilename)
				break;

			localFilename = centralWnd->ImageSuivante(false);
		}
	}
	while (!isFound);

	return isFound;
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
bool CMainWindow::FindPreviousValidFile()
{
	bool isFound = false;
	wxString firstFile = centralWnd->ImageDebut(false);
	do
	{
		isFound = CThumbnailBuffer::FindValidFile(localFilename);

		if (!isFound)
		{
			if (firstFile == localFilename)
				break;

			localFilename = centralWnd->ImagePrecedente(false);
		}
	}
	while (!isFound);

	return isFound;
}


void CMainWindow::UpdateFolderStatic()
{
    printf("CMainWindow::UpdateFolderStatic() \n");
    //
	//wxString libelle = CLibResource::LoadStringFromResource(L"LBLBUSYINFO", 1);
	//wxBusyInfo wait(libelle);

	wxBusyCursor busy;
	{
		wxString requestSql = "";
		CSqlFindPhotos sqlFindPhotos;

		auto categoryFolder = static_cast<CCategoryFolderWindow*>(this->FindWindowById(CATEGORYFOLDERWINDOWID));
		if (categoryFolder != nullptr)
			requestSql = categoryFolder->GetSqlRequest();

		if (requestSql != "" && this->GetInit())
		{
			if (oldRequest != requestSql)
				sqlFindPhotos.SearchPhotos(requestSql);
			oldRequest = requestSql;
		}

		int typeAffichage = 0;

		CMainParam* config = CMainParamInit::getInstance();
		if (config != nullptr)
		{
			typeAffichage = config->GetTypeAffichage();
		}


		PhotosVector* _pictures = new PhotosVector();

		sqlFindPhotos.SearchPhotosByCriteriaFolder(_pictures);

		if (firstFileToShow != "")
			localFilename = firstFileToShow;
		else
			localFilename = centralWnd->GetFilename();

		CThumbnailBuffer::InitVectorList(_pictures);

		if (firstFileToShow == "")
		{
			bool isFound = false;

			if (!isFound && CThumbnailBuffer::GetVectorSize() > 0 && localFilename != "")
			{
				isFound = FindNextValidFile();
				if (!isFound)
					isFound = FindPreviousValidFile();
			}

			if (!isFound && CThumbnailBuffer::GetVectorSize() > 0)
				localFilename = CThumbnailBuffer::GetVectorValue(0).GetPath();
		}



		centralWnd->SetListeFile(localFilename, typeAffichage);
		listFile.clear();
		thumbnailPos = 0;
		firstFileToShow = "";
		numElementTraitement = 0;
		nbElementInIconeList = CThumbnailBuffer::GetVectorSize();
		init = true;


		if (faceDetection)
		{
			wxWindow* window = this->FindWindowById(LISTFACEID);
			if (window != nullptr)
			{
				wxCommandEvent evt(wxEVENT_REFRESHFOLDER);
				window->GetEventHandler()->AddPendingEvent(evt);
			}
		}

		if (categoryFolder != nullptr)
		{
			wxCommandEvent evt(wxEVENT_REFRESHFOLDER);
			categoryFolder->GetEventHandler()->AddPendingEvent(evt);
		}
	}

}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::PhotoProcess(CPhotos* photo)
{
	int nbProcesseur = 1;

	if (wxFileName::FileExists(photo->GetPath()))
	{
		//Test si thumbnail valide
		CMainParam* config = CMainParamInit::getInstance();
		if (config != nullptr)
		{
			if (config->GetCheckThumbnailValidity() && nbProcessMD5 < nbProcesseur)
			{
				auto path = new CThreadMD5();
				path->filename = photo->GetPath();
				path->mainWindow = this;
				path->thread = new thread(CMd5Check::CheckMD5, path);
				nbProcessMD5++;
			}
			else
				numElementTraitement++;
		}
	}
	else
	{
		//Remove file
		CSQLRemoveData::DeletePhoto(photo->GetId());
		updateCriteria = true;
		UpdateFolderStatic();
		numElementTraitement++;
		processIdle = true;
	}

	wxString label = CLibResource::LoadStringFromResource(L"LBLFILECHECKING", 1);
	wxString message = label + to_string(numElementTraitement) + L"/" + to_string(CThumbnailBuffer::GetVectorSize());
	if (statusBarViewer != nullptr)
	{
		statusBarViewer->SetText(3, message);
	}
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::ProcessIdle()
{
	bool hasDoneOneThings = false;
	int pictureSize = CThumbnailBuffer::GetVectorSize();

	if (updateCriteria)
	{
		folderProcess->UpdateCriteria(criteriaSendMessage);
		criteriaSendMessage = false;
		updateCriteria = false;
		hasDoneOneThings = true;
	}
	else if (refreshFolder)
	{
		int nbFile = 0;
		bool folderChange = false;
		folderProcess->RefreshFolder(folderChange, nbFile);
		if (folderChange || nbFile > 0)
		{
			UpdateFolderStatic();
			updateCriteria = true;
			processIdle = true;
		}
		refreshFolder = false;
		numElementTraitement = 0;

		hasDoneOneThings = true;
	}
	else if (numElementTraitement < pictureSize)
	{
		CPhotos photo = CThumbnailBuffer::GetVectorValue(numElementTraitement);
		PhotoProcess(&photo);
		hasDoneOneThings = true;
	}

	if (setPictureMode)
	{
		setPictureMode = false;
		wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
		if (central != nullptr)
		{
			wxCommandEvent event(wxEVENT_SETMODEVIEWER);
			event.SetInt(4);
			wxPostEvent(central, event);
		}
	}


	if (setViewerMode)
	{
		setViewerMode = false;
		wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
		if (central != nullptr)
		{
			wxCommandEvent event(wxEVENT_SETMODEVIEWER);
			event.SetInt(1);
			wxPostEvent(central, event);
		}
	}

	ProcessThumbnail();

}

void CMainWindow::ProcessThumbnail()
{
    //printf("CMainWindow::ProcessThumbnail() : nbElementInIconeList %d \n", nbElementInIconeList);
    //printf("CMainWindow::ProcessThumbnail() : nbProcess %d \n", nbProcess);
    
	if (nbElementInIconeList == 0)
	{
		return;
	}

	int nbProcesseur = 1;
	if (CRegardsConfigParam* config = CParamInit::getInstance(); config != nullptr)
		nbProcesseur = config->GetThumbnailProcess();

	if (nbProcess >= nbProcesseur)
		return;

	for (int i = 0; i < photoList.size(); i++)
	{
		wxString path = photoList[i];

		auto event = new wxCommandEvent(wxEVENT_UPDATEMESSAGE);
		event->SetExtraLong(photoList.size());
		wxQueueEvent(this, event);


		std::map<wxString, bool>::iterator it = listFile.find(path);
		if (it == listFile.end())
		{
			ProcessThumbnail(path, 0);
			listFile[path] = true;
		}

		photoList.erase(photoList.begin() + i);

		i--;

		if (nbProcess == nbProcesseur)
			break;
	}


	if (photoList.empty())
	{
		nbElement = 0;
		processIdle = false;
		needToRefresh = true;


		auto event = new wxCommandEvent(wxEVENT_UPDATEMESSAGE);
		event->SetExtraLong(nbElement);
		wxQueueEvent(this, event);


	}
	else
		processIdle = true;
}

void CMainWindow::UpdateMessage(wxCommandEvent& event)
{
	const int nbPhoto = event.GetExtraLong();
	const auto thumbnailMessage = new CThumbnailMessage();
	if (nbPhoto > 0)
	{
		thumbnailMessage->nbPhoto = nbPhoto;
		thumbnailMessage->thumbnailPos = thumbnailPos;
		thumbnailMessage->nbElement = nbElementInIconeList;
		thumbnailMessage->typeMessage = 3;
		wxWindow* mainWnd = FindWindowById(MAINVIEWERWINDOWID);
		if (mainWnd != nullptr)
		{
			wxCommandEvent eventChange(wxEVENT_UPDATESTATUSBARMESSAGE);
			eventChange.SetClientData(thumbnailMessage);
			eventChange.SetInt(3);
			mainWnd->GetEventHandler()->AddPendingEvent(eventChange);
		}
		thumbnailPos++;
	}

	
}

void CMainWindow::OnProcessThumbnail(wxCommandEvent& event)
{
	
	wxString* filename = (wxString*)event.GetClientData();
	wxString localName = wxString(*filename);
	int type = event.GetInt();
	if (type == 1)
	{
		ProcessThumbnail(localName, type);
	}
	else
	{
		std::map<wxString, bool>::iterator it = listFile.find(localName);
		if (it == listFile.end())
		{
			ProcessThumbnail(localName, type);
			listFile[localName] = true;
		}
	}
	delete filename;
}

void CMainWindow::ProcessThumbnail(wxString filename, int type)
{
	if (filename != "")
	{
        nbProcess++;
		auto pLoadBitmap = new CThreadLoadingBitmap();
		pLoadBitmap->filename = filename;
		pLoadBitmap->window = this;
		pLoadBitmap->type = type;
		pLoadBitmap->_thread = new thread(LoadPicture, pLoadBitmap);
	}
	else
	{
		printf("error");
	}

}


void CMainWindow::LoadPicture(void* param)
{
	//std::thread* t1 = nullptr;
	CLibPicture libPicture;
	auto threadLoadingBitmap = static_cast<CThreadLoadingBitmap*>(param);
	if (threadLoadingBitmap == nullptr)
		return;

	if (libPicture.TestIsPDF(threadLoadingBitmap->filename) || libPicture.
		TestIsAnimation(threadLoadingBitmap->filename))
	{

		vector<CImageVideoThumbnail*> listVideo = libPicture.LoadAllVideoThumbnail(threadLoadingBitmap->filename, true, true);

		if (listVideo.size() > 0)
		{
			CSqlThumbnailVideo sqlThumbnailVideo;

			//int selectPicture = listVideo.size() / 2;
			for (int i = 0; i < listVideo.size(); i++)
			{
				CImageVideoThumbnail* bitmap = listVideo[i];
				wxString filename = threadLoadingBitmap->filename; // bitmap->image->GetFilename();

				if (bitmap->image.IsOk())
				{
					wxString localName = sqlThumbnailVideo.InsertThumbnail(filename, bitmap->image.GetWidth(),
						bitmap->image.GetHeight(), i, bitmap->rotation, bitmap->percent,
						bitmap->timePosition);

					bitmap->image.SaveFile(localName, wxBITMAP_TYPE_JPEG);
				}


				if (i == 0)
					threadLoadingBitmap->bitmapIcone = bitmap->image;

			}
			threadLoadingBitmap->isAnimationOrVideo = true;
		}
		else //Not support video
		{
			threadLoadingBitmap->bitmapIcone = defaultPicture;
			wxString filename = threadLoadingBitmap->filename;

			wxBitmap bitmap = wxBitmap(defaultPicture);


			CSqlThumbnailVideo sqlThumbnailVideo;
			wxString localName = sqlThumbnailVideo.InsertThumbnail(filename, bitmap.GetWidth(), bitmap.GetHeight(), 0, 0, 0, 0);
			bitmap.SaveFile(localName, wxBITMAP_TYPE_JPEG);
		}

		for (CImageVideoThumbnail* bitmap : listVideo)
			delete bitmap;

		listVideo.clear();
	}
	else
	{
		CImageLoadingFormat* imageLoad = libPicture.LoadThumbnail(threadLoadingBitmap->filename);
		if (imageLoad != nullptr)
		{
			threadLoadingBitmap->bitmapIcone = imageLoad->GetwxImage();
			delete imageLoad;
		}
	}


	if (threadLoadingBitmap->bitmapIcone.IsOk())
	{
		//Enregistrement en base de données
		CSqlThumbnail sqlThumbnail;
		wxFileName file(threadLoadingBitmap->filename);
		wxULongLong sizeFile = file.GetSize();
		wxString hash = sizeFile.ToString();
		wxString localName = sqlThumbnail.InsertThumbnail(threadLoadingBitmap->filename, threadLoadingBitmap->bitmapIcone.GetWidth(), threadLoadingBitmap->bitmapIcone.GetHeight(), hash);
		if(localName != "")
			threadLoadingBitmap->bitmapIcone.SaveFile(localName, wxBITMAP_TYPE_JPEG);
	}

	auto event = new wxCommandEvent(wxEVENT_ICONEUPDATE);
	event->SetClientData(threadLoadingBitmap);
	wxQueueEvent(threadLoadingBitmap->window, event);
}


//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::OnIdle(wxIdleEvent& evt)
{
	if (needToRefresh)
	{
		this->Refresh();
		needToRefresh = false;
	}

	StartThread();
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::Md5Checking(wxCommandEvent& event)
{
	auto path = static_cast<CThreadMD5*>(event.GetClientData());
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
	wxString label = CLibResource::LoadStringFromResource(L"LBLFILECHECKING", 1);
	wxString message = label + to_string(numElementTraitement) + L"/" + to_string(CThumbnailBuffer::GetVectorSize());
	if (statusBarViewer != nullptr)
	{
		statusBarViewer->SetText(3, message);
	}
}


//---------------------------------------------------------------
//
//---------------------------------------------------------------
CMainWindow::~CMainWindow()
{
    delete(toolbarViewerMode);
    delete(progressBar);
	delete(statusBar);
	delete(centralWnd);
	delete(toolbar);
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMainWindow::SaveParameter()
{
	if (centralWnd != nullptr)
		centralWnd->SaveParameter();

}

void CMainWindow::Resize()
{
	if (!fullscreen)
	{
		const wxSize sizeStatusBar = statusBar->GetSize();

		toolbar->SetSize(0, 0, GetWindowWidth(), toolbar->GetNavigatorHeight());
		toolbar->Refresh();



		toolbarViewerMode->SetSize(0, toolbar->GetNavigatorHeight(), GetWindowWidth(), toolbarViewerMode->GetNavigatorHeight());
		toolbarViewerMode->Refresh();

		centralWnd->SetSize(0, toolbar->GetNavigatorHeight() + toolbarViewerMode->GetNavigatorHeight(), GetWindowWidth(), GetWindowHeight() - (toolbarViewerMode->GetNavigatorHeight() + toolbar->GetNavigatorHeight() + sizeStatusBar.y));
		centralWnd->Refresh();

		statusBar->SetSize(0, GetWindowHeight() - sizeStatusBar.y, GetWindowWidth(),sizeStatusBar.y);
		statusBar->Refresh();
	}
	else
	{
		centralWnd->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
		centralWnd->Refresh();
	}
}

void CMainWindow::PictureVideoClick(wxCommandEvent& event)
{
	const long timePosition = event.GetExtraLong();
	if (centralWnd != nullptr)
	{
		centralWnd->SetPosition(timePosition);
	}
}


void CMainWindow::OnPictureClick(wxCommandEvent& event)
{
	const int photoId = event.GetExtraLong();
	wxString filename = CThumbnailBuffer::FindPhotoById(photoId);
	centralWnd->LoadPicture(filename);
}

void CMainWindow::OnUpdateFolder(wxCommandEvent& event)
{
	int typeId = event.GetInt();

	if (typeId == 0)
	{
		const auto newPath = static_cast<wxString*>(event.GetClientData());
		if (newPath != nullptr)
		{
			firstFileToShow = *newPath;
			delete newPath;
		}
	}


	updateCriteria = true;


	//wxString libelle = CLibResource::LoadStringFromResource(L"LBLBUSYINFO", 1);
	wxBusyCursor busy;
	{
		photoList.clear();
		CSqlPhotosWithoutThumbnail sqlPhoto;
		sqlPhoto.GetPhotoList(&photoList, 0);
	}


	UpdateFolderStatic();
	processIdle = true;
	//this->Show(true);
}



void CMainWindow::TransitionEnd()
{
	centralWnd->TransitionEnd();

	//if (!centralWnd->IsDiaporamaStart())
	//{
		auto showBitmap = static_cast<CShowElement*>(this->FindWindowById(SHOWBITMAPVIEWERID));
		if (showBitmap != nullptr)
		{
			showBitmap->TransitionEnd();
		}
	//}
}


void CMainWindow::OnUpdateInfos(wxCommandEvent& event)
{
#if defined(WIN32) && defined(_DEBUG)
	OutputDebugString(L"CMainWindow::OnUpdateInfos");
	OutputDebugString(L"\n");
#endif

	auto pictureInfos = static_cast<CPictureInfosMessage*>(event.GetClientData());
	if (pictureInfos != nullptr)
	{
		wxString filename = pictureInfos->filename;
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
	if (nbProcessMD5 > 0)
		return false;

	return true;
}

void CMainWindow::OnScanner(wxCommandEvent& event)
{
	statusBarViewer->ShowScanner();
}

void CMainWindow::OnExit(wxCommandEvent& event)
{
	statusBarViewer->Exit();
}

void CMainWindow::OnOpenFileOrFolder(wxCommandEvent& event)
{
	auto file = static_cast<wxString*>(event.GetClientData());
	if (file != nullptr)
	{
		init = true;
		int type = event.GetInt();
		if (type == 1)
			OpenFile(*file);
		else
			OpenFolder(*file);

		wxWindow* window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
		if (window != nullptr)
		{
			wxString * filePath = new wxString(*file);
			wxCommandEvent evt(wxEVENT_SELCHANGED);
			evt.SetExtraLong(1);
			evt.SetInt(1);
			evt.SetClientData(filePath);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		delete file;
	}
}

void CMainWindow::OpenFile(const wxString& fileToOpen)
{
	FolderCatalogVector folderList;
	CSqlFindFolderCatalog folderCatalog;
	folderCatalog.GetFolderCatalog(&folderList, NUMCATALOGID);
	bool find = false;
	wxFileName filename(fileToOpen);
	wxString folder = filename.GetPath();

	for (CFolderCatalog folderlocal : folderList)
	{
		if (folder == folderlocal.GetFolderPath())
		{
			find = true;
			break;
		}
	}

	if (!find)
	{
		OpenFolder(folder);
	}

	updateCriteria = true;
	UpdateFolderStatic();
	processIdle = true;

	centralWnd->LoadPicture(fileToOpen);
}

////////////////////////////////////////////////////////////////////////////////////////////////
///Gestion des événements du menu
////////////////////////////////////////////////////////////////////////////////////////////////
bool CMainWindow::OpenFolder(const wxString& path)
{
	if (wxDirExists(path))
	{
		if (viewerParam != nullptr)
			viewerParam->SetCatalogCriteria("");

		wxWindow* window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
		if (window != nullptr)
		{
			auto newFolder = new wxString(path);
			wxCommandEvent evt(wxEVENT_SETFOLDER);
			evt.SetClientData(newFolder);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
	}

	return true;
}

bool CMainWindow::IsFullscreen()
{
	return fullscreen;
}

void CMainWindow::InitPictures(wxCommandEvent& event)
{
	printf("InitPictures \n");
	refreshFolder = true;
	processIdle = true;
}


void CMainWindow::OnFaceInfosUpdate(wxCommandEvent& event)
{
	updateCriteria = true;
	printf("OnFaceInfosUpdate /n");
	processIdle = true;
}

void CMainWindow::OnRefreshPicture(wxCommandEvent& event)
{
	localFilename = centralWnd->GetFilename();
	centralWnd->LoadPicture(localFilename, true);
}


void CMainWindow::ShowToolbar()
{
	showToolbar = !showToolbar;
	if (centralWnd != nullptr)
	{
		if (!showToolbar)
			centralWnd->HideToolbar();
		else
			centralWnd->ShowToolbar();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
///Affichage en mode plein écran
////////////////////////////////////////////////////////////////////////////////////////////////
bool CMainWindow::SetFullscreen()
{
	const bool work = centralWnd->IsCompatibleFullscreen();
	if (work)
		statusBarViewer->SetFullscreen();
	return work;
}

void CMainWindow::SetScreenEvent(wxCommandEvent& event)
{
	this->Resize();
}

bool CMainWindow::SetFullscreenMode()
{
	bool is_work = false;

	if (!fullscreen)
	{
		if (centralWnd->FullscreenMode())
		{
			is_work = true;
			fullscreen = true;
			toolbarViewerMode->Show(false);
			toolbar->Show(false);
			statusBar->Show(false);
			wxCommandEvent event(wxEVENT_SETSCREEN);
			wxPostEvent(this, event);
		}
	}
	return is_work;
}

bool CMainWindow::SetScreen()
{
	bool isWork = false;

	if (fullscreen)
	{
		if (centralWnd->ScreenMode())
		{
			statusBarViewer->SetScreen();
			fullscreen = false;
			toolbar->Show(true);
			toolbarViewerMode->Show(true);
			statusBar->Show(true);
			isWork = true;
			wxCommandEvent event(wxEVENT_SETSCREEN);
			wxPostEvent(this, event);
		}
	}
	return isWork;
}
