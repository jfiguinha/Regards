#include <header.h>
#include "MainWindow.h"
#include <LibResource.h>
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <PrintEngine.h>
#include <libPicture.h>
#include "ThemeParam.h"
#include "MainThemeInit.h"
#include "MainTheme.h"
#include "PanelInfosWnd.h"
#include "SqlFindPhotos.h"
#include <SqlThumbnail.h>
#include "PreviewWnd.h"
#include "Toolbar.h"
#include <StatusBarInterface.h>
#include "CentralWindow.h"
#include "FileUtility.h"
#include "CategoryFolderWindow.h"
#include <VideoControl_soft.h>
#include <ImageLoadingFormat.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include "ImageList.h"
#include <ConvertUtility.h>
#include <window_id.h>
#include <SqlFindFolderCatalog.h>
#include <SQLRemoveData.h>
#include <SqlFolderCatalog.h>
#include <wx/display.h>
#include <SqlInsertFile.h>
#include "StatusText.h"
#include <ThumbnailMessage.h>
#include <SqlThumbnailVideo.h>
#include "FaceInfosUpdate.h"

#include <ShowBitmap.h>
#include "WaitingWindow.h"
#include <wx/stdpaths.h>
#include <ShowVideo.h>
#include <ffmpeg_transcoding.h>
#include <wx/filedlg.h>
#include <CompressionAudioVideoOption.h>
#include <VideoCompressOption.h>
#include <ParamInit.h>
#include "ListFace.h"
#include <wx/busyinfo.h>
#include <OpenCVEffect.h>
//#include <jpge.h>
//using namespace jpge;
using namespace Regards::Picture;
using namespace Regards::Control;
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

	~CThreadMD5()
	{
	};

	wxString filename;
	std::thread* thread;
	wxWindow* mainWindow;
};

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

	~CThreadVideoData()
	{
	}

	CMainWindow* mainWindow;
	wxString video;
};


wxDEFINE_EVENT(wxEVENT_SETSCREEN, wxCommandEvent);

CMainWindow::CMainWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface* statusbar, const bool& openFirstFile)
	: CWindowMain("CMainWindow", parent, id)
{
	//loadPicture = true;
	fullscreen = false;
	startDiaporama = false;
	nbProcessMD5 = 0;
	showToolbar = true;
	multithread = true;
	needToReload = false;
	typeAffichage = THUMB_SHOW_ALL;
	updateCriteria = true;
	updateFolder = true;
	refreshFolder = false;
	numElementTraitement = 0;
	start = true;
	criteriaSendMessage = false;
	//videoStart = false;
	checkVersion = true;


	CMainTheme* viewerTheme = CMainThemeInit::getInstance();
	viewerParam = static_cast<CMainParam*>(CMainParamInit::getInstance());
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
		centralWnd = new CCentralWindow(this, CENTRALVIEWERWINDOWID, theme, false);
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
	Connect(TOOLBAR_UPDATE_ID, wxCommandEventHandler(CMainWindow::OnShowToolbar));
	Connect(VIDEO_END_ID, wxCommandEventHandler(CMainWindow::OnVideoEnd));
	Connect(VIDEO_START, wxCommandEventHandler(CMainWindow::OnVideoStart));
	Connect(wxEVENT_ADDFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnAddFolder));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CMainWindow::OnIdle));
	Connect(wxEVENT_REMOVEFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnRemoveFolder));
	Connect(wxEVENT_ONPICTURECLICK, wxCommandEventHandler(CMainWindow::OnPictureClick));
	Connect(wxEVT_CRITERIACHANGE, wxCommandEventHandler(CMainWindow::CriteriaChange));
	Connect(wxEVENT_PICTUREVIDEOCLICK, wxEVT_COMMAND_TEXT_UPDATED,
	        wxCommandEventHandler(CMainWindow::PictureVideoClick));
	Connect(wxEVENT_REFRESHFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::InitPictures));
	Connect(wxEVENT_REFRESHPICTURE, wxCommandEventHandler(CMainWindow::OnRefreshPicture));
	Connect(wxEVENT_MD5CHECKING, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::Md5Checking));
	//Connect(wxTIMER_REFRESHTIMERSTART, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::RefreshTimer));
	Connect(wxTIMER_DIAPORAMATIMERSTART, wxEVT_COMMAND_TEXT_UPDATED,
	        wxCommandEventHandler(CMainWindow::StartDiaporamaMessage));
	Connect(wxEVENT_SETSTATUSTEXT, wxCommandEventHandler(CMainWindow::OnStatusSetText));
	Connect(wxEVT_EXIT, wxCommandEventHandler(CMainWindow::OnExit));
	Connect(wxEVENT_SETRANGEPROGRESSBAR, wxCommandEventHandler(CMainWindow::OnSetRangeProgressBar));
	Connect(wxEVENT_PRINTPICTURE, wxCommandEventHandler(CMainWindow::PrintPreview));
	Connect(wxEVENT_UPDATEMESSAGETHUMBNAIL, wxCommandEventHandler(CMainWindow::UpdateThumbnailMessage));
	Connect(wxEVENT_UPDATEMESSAGECRITERIA, wxCommandEventHandler(CMainWindow::UpdateCriteriaMessage));
	Connect(wxEVENT_UPDATEMESSAGEFACE, wxCommandEventHandler(CMainWindow::UpdateFaceMessage));
	Connect(wxEVENT_PRINT, wxCommandEventHandler(CMainWindow::OnPrint));
	Connect(wxEVENT_SETVALUEPROGRESSBAR, wxCommandEventHandler(CMainWindow::OnSetValueProgressBar));
	Connect(wxEVT_ANIMATIONTIMERSTOP, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::StopAnimation));
	Connect(wxEVENT_SHOWSCANNER, wxCommandEventHandler(CMainWindow::OnScanner));
	Connect(wxEVENT_ENDVIDEOTHUMBNAIL, wxCommandEventHandler(CMainWindow::OnEndThumbnail));
    Connect(wxEVENT_PICTUREPREVIOUS, wxCommandEventHandler(CMainWindow::OnPicturePrevious));
    Connect(wxEVENT_PICTURENEXT, wxCommandEventHandler(CMainWindow::OnPictureNext));
	statusBar = new wxStatusBar(this, wxID_ANY, wxSTB_DEFAULT_STYLE, "wxStatusBar");
	//Connect(wxEVENT_SETLISTPICTURE, wxCommandEventHandler(CMainWindow::SetListeFile));
    Connect(wxEVENT_OPENFILEORFOLDER, wxCommandEventHandler(CMainWindow::OnOpenFileOrFolder));
	Connect(wxEVENT_EDITFILE, wxCommandEventHandler(CMainWindow::OnEditFile));
	Connect(wxEVENT_EXPORTFILE, wxCommandEventHandler(CMainWindow::OnExportFile));
	Connect(wxEVENT_ENDCOMPRESSION, wxCommandEventHandler(CMainWindow::OnEndDecompressFile));
	Connect(wxEVENT_UPDATETHUMBNAILEXIF, wxCommandEventHandler(CMainWindow::OnUpdateExifThumbnail));

	int tabWidth[] = {100, 300, 300, 300};
	statusBar->SetFieldsCount(4);
	statusBar->SetStatusWidths(4, tabWidth);

	progressBar = new wxGauge(statusBar, wxID_ANY, 200, wxPoint(1000, 0), wxSize(200, statusBar->GetSize().y),
	                          wxGA_HORIZONTAL);
	progressBar->SetRange(100);
	progressBar->SetValue(50);
	refreshFolder = true;
	processIdle = true;
	updateFolder = true;
	listProcessWindow.push_back(this);
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		localFilename = config->GetLastShowPicture();
}

void CMainWindow::OnUpdateExifThumbnail(wxCommandEvent& event)
{
	int numPhoto = event.GetInt();
	int rotate = event.GetExtraLong();

	wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
	Regards::OpenCV::COpenCVEffect::LoadAndRotate(thumbnail, rotate);
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
	int ret = event.GetInt();
	if (ffmpegEncoder != nullptr)
	{
		ffmpegEncoder->EndDecodeFile(ret);
		delete ffmpegEncoder;
		ffmpegEncoder = nullptr;
	}
}

void CMainWindow::OnExportFile(wxCommandEvent& event)
{


	if (IsVideo())
	{
		wxString savevideofile = CLibResource::LoadStringFromResource(L"LBLSAVEVIDEOFILE", 1);
		wxString filename = CLibResource::LoadStringFromResource(L"LBLFILESNAME", 1);

		wxFileDialog saveFileDialog(nullptr, savevideofile, "", filename,
				"mp4 " + filename + " (*.mp4)|*.mp4|webm " + filename + " (*.webm)|*.webm|mov " + filename + " (*.mov)|*.mov|mkv " + filename + " (*.mkv)|*.mkv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveFileDialog.ShowModal() == wxID_CANCEL)
			return;     // the user changed idea...

		wxString filepath = saveFileDialog.GetPath();
		int index = saveFileDialog.GetFilterIndex();

		wxFileName file_path(filepath);
		wxString extension = file_path.GetExt();

		if(extension != "mp4" && extension != "webm" && extension != "mov" && extension != "mkv")
		{
			switch (index)
			{
			case 0:
				filepath += ".mp4";
				break;
			case 1:
				filepath += ".webm";
				break;
			case 2:
				filepath += ".mov";
				break;
			case 3:
				filepath += ".mkv";
				break;
			}
		}

		CVideoControlSoft * videoWindow = (CVideoControlSoft *)this->FindWindowById(VIDEOCONTROL);
		COpenCLEngine * openCLEngine = videoWindow->GetOpenCLEngine();

		CompressionAudioVideoOption compressAudioVideoOption(this, filename, filepath, openCLEngine);
		compressAudioVideoOption.ShowModal();
		if (compressAudioVideoOption.IsOk())
		{
			
			if (ffmpegEncoder == nullptr)
			{
				CVideoOptionCompress * videoCompressOption = new CVideoOptionCompress();
				compressAudioVideoOption.GetCompressionOption(videoCompressOption);
				//Decoder available
				wxString decoder = "";
				ffmpegEncoder = new CFFmpegTranscoding(decoder, openCLEngine);
				ffmpegEncoder->EncodeFile(this, filename, filepath, videoCompressOption);
				
			}
		}
	}
	else
	{
		CBitmapWndViewer* bitmapWindow = (CBitmapWndViewer*)this->FindWindowById(BITMAPWINDOWVIEWERID);
		if (bitmapWindow != nullptr)
			bitmapWindow->ExportPicture();
	}
}

void CMainWindow::OnEditFile(wxCommandEvent& event)
{
	CMainParam* config = CMainParamInit::getInstance();
	wxString pathProgram = "";
	wxString title = CLibResource::LoadStringFromResource(L"LBLSELECTPICTUREEDITOR", 1);
	if (IsVideo())
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

	if (pathProgram == "")
	{
		wxString allfiles = CLibResource::LoadStringFromResource(L"LBLALLFILES", 1);
		wxFileDialog openFileDialog(nullptr, title, "", "",
			allfiles, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		if (openFileDialog.ShowModal() == wxID_OK)
			pathProgram = openFileDialog.GetPath();
	}

	if (IsVideo())
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

bool CMainWindow::IsVideo()
{
	if (centralWnd != nullptr)
		return centralWnd->IsVideo();
	return false;
}

void CMainWindow::OnPrint(wxCommandEvent& event)
{
	bool showPrintPicture = true;
	if (IsVideo())
	{
		CShowVideo * video = (CShowVideo *)this->FindWindowById(SHOWVIDEOVIEWERID);
		if (video != nullptr)
		{
			if (video->IsPause())
			{
				CRegardsBitmap * image = video->GetVideoBitmap();
				if (image != nullptr)
				{
					statusBarViewer->PrintImagePreview(image);
					showPrintPicture = false;
				}
			}
		}
	}
	if(showPrintPicture)
	{
		CLibPicture libPicture;
		CImageLoadingFormat* image = libPicture.LoadPicture(localFilename);
		if (image != nullptr)
		{
			statusBarViewer->PrintPreview(image);
		}
	}
}


void CMainWindow::UpdateThumbnailMessage(wxCommandEvent& event)
{
	TRACE();
	CThumbnailMessage* thumbnailMessage = static_cast<CThumbnailMessage*>(event.GetClientData());
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

void CMainWindow::UpdateFaceMessage(wxCommandEvent& event)
{
	TRACE();
	CThumbnailMessage* thumbnailMessage = static_cast<CThumbnailMessage*>(event.GetClientData());
	if (thumbnailMessage != nullptr)
	{
		wxString picture = CLibResource::LoadStringFromResource(L"LBLFACEPROCESS", 1);
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
	CThumbnailMessage* thumbnailMessage = static_cast<CThumbnailMessage*>(event.GetClientData());
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
				wxString message = picture + to_string(thumbnailMessage->thumbnailPos) + " / " + to_string(
					thumbnailMessage->nbPhoto);
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
	CRegardsBitmap* bitmap = static_cast<CRegardsBitmap*>(event.GetClientData());

	if (bitmap != nullptr)
	{
		statusBarViewer->PrintImagePreview(bitmap);
	}
}

void CMainWindow::RefreshCriteriaPictureList(wxCommandEvent& event)
{
	TRACE();
	updateFolder = true;
	printf("RefreshCriteriaPictureList /n");
	processIdle = true;
}

void CMainWindow::OnFacePertinence()
{
	TRACE();
	updateFolder = true;
	printf("OnFacePertinence /n");
	processIdle = true;
}

wxString CMainWindow::GetFilename()
{
	return localFilename;
}

void CMainWindow::OnEndPictureLoad(wxCommandEvent& event)
{
	wxString* threadFilename = static_cast<wxString*>(event.GetClientData());
	if (startDiaporama)
	{
		printf("OnEndPictureLoad \n");
		int timeDelai = viewerParam->GetDelaiDiaporamaOption();
		diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
	}

	if (*threadFilename != localFilename && *threadFilename != "")
	{
		centralWnd->LoadPicture(*threadFilename);
	}
	else if(*threadFilename == "")
	{
		printf("toto");
	}

	if (threadFilename != nullptr)
		delete threadFilename;
}

void CMainWindow::CriteriaChange(wxCommandEvent& event)
{
	TRACE();
	//Refresh Criteria
	wxWindow* window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
	if (window)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_UPDATECRITERIA);
		evt.SetExtraLong(1);
		window->GetEventHandler()->AddPendingEvent(evt);
	}

	processIdle = true;
}


void CMainWindow::OnVideoStart(wxCommandEvent& event)
{
	TRACE();

	if (centralWnd != nullptr)
	{
		wxCommandEvent evt(VIDEO_START);
		centralWnd->GetEventHandler()->AddPendingEvent(evt);
	}
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
	CStatusText* statusText = static_cast<CStatusText*>(event.GetClientData());
	if (statusText != nullptr)
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

void CMainWindow::OnSetRangeProgressBar(wxCommandEvent& event)
{
	TRACE();
	int range = event.GetInt();
	// cout << "OnSetRangeProgressBar Pos : " << range << endl;
	if (progressBar != nullptr)
		progressBar->SetRange(range);
}

void CMainWindow::SetText(const int& numPos, const wxString& libelle)
{
	TRACE();
	wxCommandEvent* event = new wxCommandEvent(wxEVENT_SETSTATUSTEXT);
	CStatusText* statusText = new CStatusText();
	statusText->position = numPos;
	statusText->text = libelle;
	event->SetClientData(statusText);
	wxQueueEvent(this, event);

	//statusBar->SetStatusText(libelle, numPos);
}

void CMainWindow::SetRangeProgressBar(const int& range)
{
	TRACE();
	wxCommandEvent* event = new wxCommandEvent(wxEVENT_SETRANGEPROGRESSBAR);
	event->SetInt(range);
	wxQueueEvent(this, event);
}

void CMainWindow::SetPosProgressBar(const int& position)
{
	TRACE();
	wxCommandEvent* event = new wxCommandEvent(wxEVENT_SETVALUEPROGRESSBAR);
	event->SetInt(position);
	wxQueueEvent(this, event);
}

void CMainWindow::StartDiaporamaMessage(wxCommandEvent& event)
{
	TRACE();
	int timeDelai = viewerParam->GetDelaiDiaporamaOption();
	diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
}

wxString GetFileName(const wxString& nameFile)
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

bool CMainWindow::FindNextValidFile()
{
	bool isFound = false;
	wxString lastFile = centralWnd->ImageFin(false);
	do
	{
		auto p = std::find_if(
			pictures.begin(), pictures.end(),
			[&](const auto& val)
			{
				CPhotos photo = static_cast<CPhotos>(val);
				return photo.GetPath() == localFilename;
			}
		);

		if (p != pictures.end())
			isFound = true;

		if (!isFound)
		{
			if (lastFile == localFilename)
				break;

			localFilename = centralWnd->ImageSuivante(false);
		}

	} while (!isFound);

	return isFound;
}

bool CMainWindow::FindPreviousValidFile()
{
	bool isFound = false;
	wxString firstFile = centralWnd->ImageDebut(false);
	do
	{
		auto p = std::find_if(
			pictures.begin(), pictures.end(),
			[&](const auto& val)
			{
				CPhotos photo = static_cast<CPhotos>(val);
				return photo.GetPath() == localFilename;
			}
		);

		if (p != pictures.end())
			isFound = true;

		if (!isFound)
		{
			if (firstFile == localFilename)
				break;

			localFilename = centralWnd->ImagePrecedente(false);
		}

	} while (!isFound);

	return isFound;
}

void CMainWindow::ProcessIdle()
{
	TRACE();
	bool hasDoneOneThings = false;
	int nbProcesseur = 1;

	if (updateCriteria)
	{
		wxWindow* window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
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


		int nbFile = 0;
		//Test de la validité des répertoires

		for (CFolderCatalog folderlocal : folderList)
		{
			wxString fichier;
			CSqlInsertFile sqlInsertFile;
			nbFile += sqlInsertFile.ImportFileFromFolder(folderlocal.GetFolderPath(), folderlocal.GetNumFolder(),
			                                             fichier);
		}

		if (folderChange || nbFile > 0)
		{
			CMainParam* viewerParam = static_cast<CMainParam*>(CMainParamInit::getInstance());
			wxString sqlRequest = viewerParam->GetLastSqlRequest();

			CSqlFindPhotos sqlFindPhotos;
			sqlFindPhotos.SearchPhotos(sqlRequest);

			updateFolder = true;
			updateCriteria = true;
		}
		refreshFolder = false;
		hasDoneOneThings = true;
		numElementTraitement = 0;
	}
	else if (updateFolder)
	{
		wxString requestSql = "";
		pictures.clear();
		CSqlFindPhotos sqlFindPhotos;
		bool isValid = true;

		CCategoryFolderWindow* categoryFolder = static_cast<CCategoryFolderWindow*>(this->FindWindowById(
			CATEGORYFOLDERWINDOWID));
		if (categoryFolder != nullptr)
			requestSql = categoryFolder->GetSqlRequest();

		if (requestSql != "")
		{
			sqlFindPhotos.SearchPhotos(requestSql);
			sqlFindPhotos.SearchPhotosByTypeAffichage(&pictures, typeAffichage, NUMCATALOGID);
		}
		else
			sqlFindPhotos.SearchPhotos(&pictures);

		bool isFound = false;

		if (!isFound && pictures.size() > 0 && localFilename != "")
		{
			isFound = FindNextValidFile();
			if (!isFound)
				isFound = FindPreviousValidFile();
		}
		
		if (!isFound)
			localFilename = pictures[0].GetPath();
		
		centralWnd->SetListeFile(localFilename);

		updateFolder = false;

		numElementTraitement = 0;

		hasDoneOneThings = true;
	}
	else if (numElementTraitement < pictures.size())
	{
		CPhotos photo = pictures[numElementTraitement];

		if (wxFileName::FileExists(photo.GetPath()))
		{
			//Test si thumbnail valide
			CMainParam* config = CMainParamInit::getInstance();
			if (config != nullptr)
			{
				if (config->GetCheckThumbnailValidity() && nbProcessMD5 < nbProcesseur)
				{
					CThreadMD5* path = new CThreadMD5();
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

		wxString label = CLibResource::LoadStringFromResource(L"LBLFILECHECKING", 1);
		wxString message = label + to_string(numElementTraitement) + L"/" + to_string(pictures.size());
		if (statusBarViewer != nullptr)
		{
			statusBarViewer->SetText(3, message);
		}

		hasDoneOneThings = true;
	}

	if (setViewerMode)
	{
		setViewerMode = false;
		wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
		wxCommandEvent event(wxEVENT_SETMODEVIEWER);
		event.SetInt(4);
		wxPostEvent(central, event);
	}

	if (!hasDoneOneThings)
		processIdle = false;
}

void CMainWindow::OnIdle(wxIdleEvent& evt)
{
	//TRACE();
	if (endProgram)
	{
		if (diaporamaTimer->IsRunning())
			diaporamaTimer->Stop();
	}
	StartThread();
}

void CMainWindow::Md5Checking(wxCommandEvent& event)
{
	TRACE();
	CThreadMD5* path = static_cast<CThreadMD5*>(event.GetClientData());
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
	wxString message = label + to_string(numElementTraitement) + L"/" + to_string(pictures.size());
	if (statusBarViewer != nullptr)
	{
		statusBarViewer->SetText(3, message);
	}
}

void CMainWindow::CheckMD5(void* param)
{
	TRACE();
	CThreadMD5* path = static_cast<CThreadMD5*>(param);
	if (path != nullptr)
	{
		CSqlThumbnail sqlThumbnail;
		CSqlThumbnailVideo sqlThumbnailVideo;
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


void CMainWindow::OnTimerDiaporama(wxTimerEvent& event)
{
	printf("OnTimerDiaporama \n");
	TRACE();
	ImageSuivante();
}

void CMainWindow::OnVideoEnd(wxCommandEvent& event)
{
	TRACE();
	if (centralWnd != nullptr)
	{
		//blockImage = false;
		wxCommandEvent evt(VIDEO_END_ID);
		centralWnd->GetEventHandler()->AddPendingEvent(evt);
	}
	if (startDiaporama)
	{
		//int timeDelai = viewerParam->GetDelaiDiaporamaOption();
		diaporamaTimer->Start(100, wxTIMER_ONE_SHOT);
	}
}

CMainWindow::~CMainWindow()
{
	TRACE();


	if (diaporamaTimer->IsRunning())
		diaporamaTimer->Stop();

	delete(diaporamaTimer);

	delete(centralWnd);
	delete(toolbar);
}

void CMainWindow::SaveParameter()
{
	if (centralWnd != nullptr)
		centralWnd->SaveParameter();
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
		toolbar->Refresh();
		centralWnd->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.y, rcAffichageBitmap.width,
		                    rcAffichageBitmap.height);
		centralWnd->Refresh();
		statusBar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.y + rcAffichageBitmap.height, rcAffichageBitmap.width,
		                   sizeStatusBar.y);
		statusBar->Refresh();
	}
	else
	{
		centralWnd->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
		centralWnd->Refresh();
	}
}


void CMainWindow::AddFolder(const wxString& folder)
{
	TRACE();

	wxString msg = "In progress ...";

	wxArrayString files;
	wxDir::GetAllFiles(folder, &files, wxEmptyString, wxDIR_FILES);
	if (files.size() > 0)
		sort(files.begin(), files.end());

	wxProgressDialog dialog("Add Folder", "File import ...", files.Count(), this, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	int updatesize = 0;
	dialog.Update(updatesize, msg);

	//Indication d'imporation des critères 
	CSqlFolderCatalog sqlFolderCatalog;
	int64_t idFolder = sqlFolderCatalog.GetFolderCatalogId(NUMCATALOGID, folder);

	printf("AddFolder : %s \n", CConvertUtility::ConvertToUTF8(folder));

	if (idFolder == -1)
	{
		idFolder = sqlFolderCatalog.GetOrInsertFolderCatalog(NUMCATALOGID, folder);
		//Insert la liste des photos dans la base de données.
		CSqlInsertFile sqlInsertFile;
		sqlInsertFile.AddFileFromFolder(this, dialog, files, folder, idFolder, localFilename);
		printf("CMainWindow::AddFolder : %s \n", CConvertUtility::ConvertToUTF8(localFilename));
	}


	statusBarViewer->AddFSEntry(folder);

	wxWindow* window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
	if (window)
	{
		wxString* newFolder = new wxString(folder);
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SETFOLDER);
		evt.SetClientData(newFolder);
		window->GetEventHandler()->AddPendingEvent(evt);
	}

	window = this->FindWindowById(LISTFACEID);
	if (window)
	{
		wxCommandEvent evt(wxEVENT_THUMBNAILFOLDERADD);
		window->GetEventHandler()->AddPendingEvent(evt);
	}

	dialog.Destroy();
}

void CMainWindow::OnAddFolder(wxCommandEvent& event)
{
	TRACE();
	wxString* info = static_cast<wxString*>(event.GetClientData());

	printf("OnAddFolder : %s \n", CConvertUtility::ConvertToUTF8(*info));


	if (*info != "")
	{
		AddFolder(*info);
		updateCriteria = true;
		updateFolder = true;
		//criteriaSendMessage = true;
	}
	delete info;


	processIdle = true;
}


void CMainWindow::OnRemoveFolder(wxCommandEvent& event)
{
	TRACE();
	wxString* info = static_cast<wxString*>(event.GetClientData());
	if (*info != "")
	{

		//centralWnd->ImageSuivante();
		localFilename = centralWnd->GetFilename();

		wxBusyInfo wait("Please wait, working...");

		wxString title = CLibResource::LoadStringFromResource(L"LBLSTOPALLPROCESS", 1);
		wxString message = CLibResource::LoadStringFromResource(L"LBLSTOPPROCESS", 1);
		StopAllProcess(title, message, this);
		
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
			//criteriaSendMessage = true;

			wxString dir = wxString(*info);
			statusBarViewer->RemoveFSEntry(dir);
		}

		

		//delete mainWindowWaiting;
		SetStopProcess(false);
		this->Show(true);

		CListFace * listFace = (CListFace *)this->FindWindowById(LISTFACEID);
		if (listFace != nullptr)
		{
			wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
			listFace->GetEventHandler()->AddPendingEvent(evt);
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
		CPreviewWnd* previewWindow = static_cast<CPreviewWnd*>(this->FindWindowById(PREVIEWVIEWERID));
		if (previewWindow != nullptr)
			previewWindow->SetDiaporamaMode();

		int timeDelai = viewerParam->GetDelaiDiaporamaOption();
		diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
	}

	startDiaporama = true;
}


void CMainWindow::PictureVideoClick(wxCommandEvent& event)
{
	TRACE();
	long timePosition = event.GetExtraLong();
	if (centralWnd != nullptr)
	{
		centralWnd->SetPosition(timePosition);
	}
}


void CMainWindow::OnPictureClick(wxCommandEvent& event)
{
	TRACE();
	int photoId = event.GetExtraLong();
	wxString filename = "";
	for (CPhotos photo : pictures)
	{
		if (photo.GetId() == photoId)
		{
			filename = photo.GetPath();
			break;
		}
	}

	centralWnd->LoadPicture(filename);
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
		CShowBitmap* showBitmap = static_cast<CShowBitmap*>(this->FindWindowById(SHOWBITMAPVIEWERID));
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
	{
		int timeDelai = viewerParam->GetDelaiDiaporamaOption();
		diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
	}
}

void CMainWindow::StopDiaporama()
{
	TRACE();
	if (startDiaporama)
	{
		CPreviewWnd* previewWindow = static_cast<CPreviewWnd*>(this->FindWindowById(PREVIEWVIEWERID));
		if (previewWindow != nullptr)
			previewWindow->SetNormalMode();

		startDiaporama = false;
		if (diaporamaTimer->IsRunning())
			diaporamaTimer->Stop();
	}
}

void CMainWindow::OnFaceInfosStatusBarUpdate(wxCommandEvent& event)
{
	CFaceInfosUpdate* infoUpdate = static_cast<CFaceInfosUpdate*>(event.GetClientData());
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

#if defined(WIN32) && defined(_DEBUG)
	OutputDebugString(L"CMainWindow::OnUpdateInfos");
	OutputDebugString(L"\n");
#endif

	CPictureInfosMessage* pictureInfos = static_cast<CPictureInfosMessage*>(event.GetClientData());
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
	TRACE();
	if (nbProcessMD5 > 0)
		return false;

	return true;
}

void CMainWindow::OnEndThumbnail(wxCommandEvent& event)
{
	wxString* thumbName = static_cast<wxString*>(event.GetClientData());
	if (*thumbName == localFilename)
		centralWnd->OnEndThumbnail();
	delete thumbName;
}

void CMainWindow::OnScanner(wxCommandEvent& event)
{
	TRACE();
	statusBarViewer->ShowScanner();
}

void CMainWindow::OnExit(wxCommandEvent& event)
{
	TRACE();
	statusBarViewer->Exit();
}

void CMainWindow::OnOpenFileOrFolder(wxCommandEvent& event)
{
    wxString * file = (wxString *)event.GetClientData();
    if(file != nullptr)
    {
        int type = event.GetInt();
        if(type == 1)
            OpenFile(*file);
        else
            OpenFolder(*file);
            
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
		AddFolder(folder);
	}

	updateCriteria = true;
	updateFolder = true;
	processIdle = true;
	setViewerMode = true;

	centralWnd->LoadPicture(fileToOpen);
}

////////////////////////////////////////////////////////////////////////////////////////////////
///Gestion des événements du menu
////////////////////////////////////////////////////////////////////////////////////////////////
bool CMainWindow::OpenFolder(const wxString &path)
{
	TRACE();
    if(wxDirExists(path))
    {
        if (viewerParam != nullptr)
			viewerParam->SetCatalogCriteria("");

		AddFolder(path);

		updateCriteria = true;
		updateFolder = true;

		processIdle = true;
    } 

	return true;

}

bool CMainWindow::IsFullscreen()
{
	TRACE();
	return fullscreen;
}

void CMainWindow::InitPictures(wxCommandEvent& event)
{
	TRACE();
	printf("InitPictures \n");
	refreshFolder = true;
	processIdle = true;
	//updateCriteria = true;
}


void CMainWindow::OnFaceInfosUpdate(wxCommandEvent& event)
{
	TRACE();
	updateCriteria = true;
	printf("OnFaceInfosUpdate /n");
	processIdle = true;
}

void CMainWindow::OnPicturePrevious(wxCommandEvent& event)
{
    ImagePrecedente();
}

void CMainWindow::OnPictureNext(wxCommandEvent& event)
{
    ImageSuivante();
}

void CMainWindow::ImageSuivante()
{
	centralWnd->ImageSuivante();
	localFilename = centralWnd->GetFilename();
}

void CMainWindow::ImagePrecedente()
{
	centralWnd->ImagePrecedente();
	localFilename = centralWnd->GetFilename();
}


void CMainWindow::OnRefreshPicture(wxCommandEvent& event)
{
	localFilename = centralWnd->GetFilename();
	centralWnd->RefreshPicture(localFilename);
}


void CMainWindow::ImageFin()
{
	centralWnd->ImageFin();
	localFilename = centralWnd->GetFilename();
}

void CMainWindow::ImageDebut()
{
	centralWnd->ImageDebut();
	localFilename = centralWnd->GetFilename();
}

void CMainWindow::ShowToolbar()
{
	TRACE();
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
	TRACE();
	bool work = centralWnd->IsCompatibleFullscreen();
	if (work)
		statusBarViewer->SetFullscreen();
	return work;
}

void CMainWindow::SetScreenEvent(wxCommandEvent& event)
{
	TRACE();
	this->Resize();
}

bool CMainWindow::SetFullscreenMode()
{
	bool isWork = false;
	TRACE();
	if (!fullscreen)
	{
		if (centralWnd->FullscreenMode())
		{
			isWork = true;
			fullscreen = true;
			toolbar->Show(false);
			statusBar->Show(false);
			wxCommandEvent event(wxEVENT_SETSCREEN);
			wxPostEvent(this, event);
		}
	}
	return isWork;
}

bool CMainWindow::SetScreen()
{
	bool isWork = false;
	TRACE();
	if (fullscreen)
	{
		if (centralWnd->ScreenMode())
		{
			statusBarViewer->SetScreen();
			fullscreen = false;
			toolbar->Show(true);
			statusBar->Show(true);
			isWork = true;
			wxCommandEvent event(wxEVENT_SETSCREEN);
			wxPostEvent(this, event);
		}

	}
	return isWork;
}
