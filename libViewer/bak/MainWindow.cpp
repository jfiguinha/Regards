 #include "MainWindow.h"
#include <LibResource.h>
#include "CentralWnd.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <PrintEngine.h>
#include <libPicture.h>
#include "ThemeParam.h"
#include "ViewerThemeInit.h"
#include "ViewerTheme.h"
#include "SqlFindPhotos.h"
#include <ImageLoadingFormat.h>
#include <wx/dir.h>
#include <wxmd5.h>
#include <wx/filename.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <FilterData.h>
#include <SqlFindFolderCatalog.h>
#include <wxSVG/SVGDocument.h>
#include "VideoFilter.h"
#include <SQLRemoveData.h>
#include <SqlFolderCatalog.h>
#include <SqlInsertFile.h>
#include <ExportFile.h>
#include <SqlFindCriteria.h>
#include <CopyFileDlg.h>
#include <ShowVideo.h>
#include <FaceRecognition.h>
#include <SqlFacePhoto.h>
#include <SqlFaceDescriptor.h>
#include <SqlFindFacePhoto.h>
#include <SqlFaceRecognition.h>
#include <SqlFaceLabel.h>
#include <jpge.h>
using namespace jpge;
using namespace Regards::Viewer;
const long ONE_MSEC = 1000;
using namespace std;
using namespace Regards::Control;
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
	~CThreadMD5(){};

	wxString filename;
	std::thread * thread;
	CMainWindow * mainWindow;
};

class CStatusText
{
public:

	CStatusText()
	{
		position = 0;
		text = "";
	}

	~CStatusText()
	{
	}

	int position;
	wxString text;
};

//wxDEFINE_EVENT(VIDEO_END_ID, wxCommandEvent);

CMainWindow::CMainWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarViewer)
	: CWindowMain("CMainWindow",parent, id)
{
	sendMessageVideoStop = false;
	loadPicture = false;
	fullscreen = false;
	startDiaporama = false;
	nbProcessMD5 = 0;
	numElement = 0;
	showToolbar = true;
    multithread = true;
    needToReload = false;
	typeAffichage = SHOW_ALL;
	updateCriteria = true;
	updateFolder = false;
	updatePicture = false;
	refreshFolder = false;
	numElementTraitement = 0;
	start = true;
	criteriaSendMessage = false;
	photoCancel = "";    
	processLoadPicture = false;
	videoStart = false;
	nbProcessFacePhoto = 0;
	//threadFacePhoto = nullptr;
    //wxSVGDocument* svgDoc = new wxSVGDocument;
    //svgDoc->Load("/Users/jacques/Pictures/test.svg");
    //wxImage img = svgDoc->Render(40,40,NULL,true,true);
	CSqlFindPhotos sqlFindPhotos;
	sqlFindPhotos.SearchPhotos(&pictures);
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();
	viewerParam = (CViewerParam *)CViewerParamInit::getInstance();
	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetMainToolbarTheme(&theme);
		toolbar = new CToolbar(this, wxID_ANY, theme);
	}

	refreshTimer = new wxTimer(this, wxTIMER_REFRESH);
	diaporamaTimer = new wxTimer(this, wxTIMER_DIAPORAMA);
	centralWnd = new CCentralWnd(this, CENTRALVIEWERWINDOWID, statusBarViewer, pictures);
	this->statusBarViewer = statusBarViewer;


	Connect(wxTIMER_DIAPORAMA, wxEVT_TIMER, wxTimerEventHandler(CMainWindow::OnTimerDiaporama), nullptr, this);
	Connect(wxTIMER_REFRESH, wxEVT_TIMER, wxTimerEventHandler(CMainWindow::OnTimerRefresh), nullptr, this);
	Connect(EVENT_SHOWPICTURE, wxCommandEventHandler(CMainWindow::ShowPicture));
	Connect(EVENT_ENDNEWPICTURETHREAD, wxCommandEventHandler(CMainWindow::EndPictureThread));
	Connect(EVENT_REFRESHLIST, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::RefreshPictureList));
	Connect(wxEVENT_CRITERIASHOWUPDATE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::RefreshCriteriaPictureList));
	Connect(TOOLBAR_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnShowToolbar));
	Connect(VIDEO_END_ID, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnVideoEnd));
	Connect(VIDEO_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::SetVideoPos));
    Connect(VIDEO_START, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnVideoStart));
	Connect(wxEVENT_ADDFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnAddFolder));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CMainWindow::OnIdle));
	Connect(wxEVENT_REMOVEFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnRemoveFolder));
	Connect(wxEVENT_CHANGETYPEAFFICHAGE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::ChangeTypeAffichage));
	Connect(wxEVENT_ONPICTURECLICK, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::OnPictureClick));
    Connect(wxEVT_CRITERIACHANGE, wxCommandEventHandler(CMainWindow::CriteriaChange));
	Connect(wxEVENT_PICTURECLICK, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::PictureClick));
	Connect(wxEVENT_PICTURECLICKMOVE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::PictureClickMove));
	Connect(wxEVENT_PICTUREVIDEOCLICK, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::PictureVideoClick));
	Connect(wxEVENT_REFRESHFOLDER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::InitPictures));
    Connect(wxEVENT_REFRESHPICTURE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::RefreshPicture));
	Connect(wxEVENT_MD5CHECKING, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::Md5Checking));
	Connect(wxEVENT_FACEPHOTOUPDATE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::FacePhotoAdd));
	Connect(wxEVENT_FACEPHOTOADD, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::AddFacePhoto));
	Connect(wxEVENT_FACEINFOSUPDATE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::FaceInfosUpdate));
	Connect(wxEVENT_FACECLICK, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::FaceClick));
    Connect(wxTIMER_REFRESHTIMERSTART, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::RefreshTimer));

	Connect(wxEVENT_SETSTATUSTEXT, wxCommandEventHandler(CMainWindow::OnStatusSetText));
	Connect(wxEVENT_SETRANGEPROGRESSBAR, wxCommandEventHandler(CMainWindow::OnSetRangeProgressBar));
	Connect(wxEVENT_SETVALUEPROGRESSBAR, wxCommandEventHandler(CMainWindow::OnSetValueProgressBar));

	statusBar = new wxStatusBar(this, wxID_ANY, wxSTB_DEFAULT_STYLE, "wxStatusBar");

	int tabWidth[] = { 100, 300, 300, 300 };
	statusBar->SetFieldsCount(4);
	statusBar->SetStatusWidths(4, tabWidth);

	progressBar = new wxGauge(statusBar, wxID_ANY, 200, wxPoint(1000, 0), wxSize(200, statusBar->GetSize().y),
		wxGA_HORIZONTAL);
	progressBar->SetRange(100);
	progressBar->SetValue(50);

	refreshTimer->Start(300000, wxTIMER_ONE_SHOT);

	updatePicture = true;
	loadFacePicture = false;

#ifdef WIN32
	this->photoCancel = CFileUtility::GetResourcesFolderPath() + "\\photo_cancel.png";
#else
	this->photoCancel = CFileUtility::GetResourcesFolderPath() + "/photo_cancel.png";
#endif

	refreshFolder = true;
	processIdle = true;
	faceData = new CFaceLoadData();

#ifdef WIN32
	//Face Data Preload
	
	wxString model = CFileUtility::GetResourcesFolderPath() + "\\dlib_face_recognition_resnet_model_v1.dat";
	wxString facelandmark = CFileUtility::GetResourcesFolderPath() + "\\shape_predictor_68_face_landmarks.dat";
#else
	wxString model = CFileUtility::GetResourcesFolderPath() + "/dlib_face_recognition_resnet_model_v1.dat";
	wxString facelandmark = CFileUtility::GetResourcesFolderPath() + "/shape_predictor_68_face_landmarks.dat";

#endif

	faceData->LoadData((const char*)model.mb_str(wxConvUTF8), (const char*)facelandmark.mb_str(wxConvUTF8));
	//centralWnd->ScreenMode();
}

void CMainWindow::RefreshCriteriaPictureList(wxCommandEvent& event)
{
	updateFolder = true;
    processIdle = true;
}

void CMainWindow::OnFacePertinence()
{
	CListFace * listFace = (CListFace *)this->FindWindowById(LISTFACEID);
	if(listFace != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_THUMBNAILUPDATE);
		listFace->GetEventHandler()->AddPendingEvent(evt);
	}
	updateFolder = true;
    processIdle = true;

}

void CMainWindow::OnThumbnailBottom()
{
	wxWindow * window = this->FindWindowById(FILTERPREVIEWSPLITTER);
	if (window)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SETTHUMBNAILBOTTOM);
		window->GetEventHandler()->AddPendingEvent(evt);
	}

	/*
	if (centralWnd != nullptr)
		centralWnd->OnThumbnailBottom();
		*/
}

void CMainWindow::SetFilterInterpolation(const int &filter)
{
	CBitmapWnd * window = (CBitmapWnd *)this->FindWindowById(BITMAPWINDOWVIEWERID);
	if (window)
	{
		window->SetFilterInterpolation(filter);
	}
}

void CMainWindow::OnTimerRefresh(wxTimerEvent& event)
{
	refreshFolder = true;
    processIdle = true;
	refreshTimer->Stop();
}

void CMainWindow::OnThumbnailRight()
{
	wxWindow * window = this->FindWindowById(FILTERPREVIEWSPLITTER);
	if (window)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SETTHUMBNAILRIGHT);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
	/*
	if (centralWnd != nullptr)
		centralWnd->OnThumbnailRight();
		*/
}

void CMainWindow::OnLoadPicture()
{
    CThreadPictureData * pictureData = new CThreadPictureData();
    pictureData->mainWindow = this;
    pictureData->picture = filenameTimer;
    pictureData->isVisible = true;
    thread * threadloadPicture = new thread(LoadingNewPicture, pictureData);
    pictureData->myThread = threadloadPicture; 
	processLoadPicture = true;
}

void CMainWindow::CriteriaChange(wxCommandEvent& event)
{
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


void CMainWindow::ChangeTypeAffichage(wxCommandEvent& event)
{
	typeAffichage = event.GetExtraLong();
	wxWindow * window = this->FindWindowById(LISTPICTUREID);
	if (window)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_CHANGETYPEAFFICHAGE);
		evt.SetExtraLong(typeAffichage);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
    processIdle = true;
}



void CMainWindow::RefreshPictureList(wxCommandEvent& event)
{
	if(typeAffichage > 0)
    {
		updateFolder = true;
        processIdle = true;
    }
}

void CMainWindow::OnVideoStart(wxCommandEvent& event)
{
    //centralWnd->StopLoadingPicture();
	videoStart = true;
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

void CMainWindow::OnStatusSetText(wxCommandEvent& event)
{
	CStatusText * statusText = (CStatusText *)event.GetClientData();
	if(statusText != nullptr)
	{
		statusBar->SetStatusText(statusText->text, statusText->position);
		delete statusText;
	}
}

void CMainWindow::OnSetValueProgressBar(wxCommandEvent& event)
{
	int position = event.GetInt();
	if (progressBar != nullptr)
    {
        if(progressBar->GetRange() > 0)
        {
            if(position >= progressBar->GetRange())
                progressBar->SetValue(progressBar->GetRange() - 1);
            else
                progressBar->SetValue(position);
        
        }
        
    }
}

void CMainWindow::OnSetRangeProgressBar(wxCommandEvent& event)
{
	int range = event.GetInt();
	if (progressBar != nullptr)
		progressBar->SetRange(range);
}

void CMainWindow::SetText(const int &numPos, const wxString &libelle)
{
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
    wxCommandEvent * event = new wxCommandEvent(wxEVENT_SETRANGEPROGRESSBAR);
    event->SetInt(range);
    wxQueueEvent(this, event);
}

void CMainWindow::SetPosProgressBar(const int &position)
{
    wxCommandEvent * event = new wxCommandEvent(wxEVENT_SETVALUEPROGRESSBAR);
    event->SetInt(position);
    wxQueueEvent(this, event);
}

void CMainWindow::EndPictureThread(wxCommandEvent& event)
{
	processLoadPicture = false;

    if(!multithread)
    {
        muPicture.lock();
        threadloadPicture->join();
        delete(threadloadPicture);
        threadloadPicture = nullptr;
        muPicture.unlock();
    }
    
    std::thread * myThread = ( std::thread *)event.GetClientData();
    if (myThread != nullptr)
    {
        myThread->join();
        delete myThread;
    }    
    
    if(needToReload && !multithread)
    {
        LoadingPicture(firstFileToShow);
    }
}

wxString GetFileName(const wxString &nameFile)
{
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


//---------------------------------------------------------------------------------------
//Test si compatible avec une autre Face à déplacer ailleurs
//---------------------------------------------------------------------------------------	
void CMainWindow::AddFacePhoto(wxCommandEvent& event)
{
	if(event.GetClientData() != nullptr)
	{
		CFaceDescriptor * faceDescriptor = (CFaceDescriptor *)event.GetClientData();
		CSqlFaceLabel sqlfaceLabel;
		CFace * face = new CFace();
		bool findFaceCompatible = false;
		CSqlFaceRecognition sqlfaceRecognition;
		CSqlFindFacePhoto sqlfindFacePhoto;

		std::vector<CFaceDescriptor*> listFace = sqlfindFacePhoto.GetUniqueFaceDescriptor(faceDescriptor->numFace);
		for(int i = 0;i < listFace.size();i++)
		{
				
			CFaceDescriptor * facede = listFace[i];	
			float isCompatible = face->IsCompatibleFace(faceDescriptor->descriptor, facede->descriptor);
			if(isCompatible < 0.6)
			{
				sqlfaceRecognition.InsertFaceRecognition(faceDescriptor->numFace, facede->numFace);
				findFaceCompatible = true;
				break;
			}
		}

		for(int i = 0;i < listFace.size();i++)
		{
			CFaceDescriptor * facede = listFace[i];		
			delete facede;
		}

		if(!findFaceCompatible)
		{
			wxString label = "Face number " + to_string(faceDescriptor->numFace);
			sqlfaceRecognition.InsertFaceRecognition(faceDescriptor->numFace, faceDescriptor->numFace);
			sqlfaceLabel.InsertFaceLabel(faceDescriptor->numFace,label, true);
			updateCriteria = true;

		}		

		if(faceDescriptor != nullptr)
			delete faceDescriptor;
			
		delete face;
	}
}


float CMainWindow::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &widthOutput, const int &heightOutput)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float newRatio = 1;

	if (pictureWidth > pictureHeight)
		newRatio = (float)widthOutput / (float)(pictureWidth);
	else
		newRatio = (float)heightOutput / (float)(pictureHeight);

	if ((pictureHeight * newRatio) > heightOutput)
	{
		newRatio = (float)heightOutput / (float)(pictureHeight);
	}
	else
	{
		if ((pictureWidth * newRatio) > widthOutput)
		{
			newRatio = (float)widthOutput / (float)(pictureWidth);
		}
	}

	return newRatio;
}

CPictureData * CMainWindow::LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth, const int &resizeHeight)
{
	CPictureData * pictureData = nullptr;
	CLibPicture libPicture;
	CImageLoadingFormat * imageLoading = libPicture.LoadPicture(filename);
	if(imageLoading != nullptr)
	{
		pictureOK = imageLoading->IsOk();
		if(pictureOK)
		{
			imageLoading->ApplyExifOrientation(1);
			imageLoading->ConvertToRGB24(true);
			//Calcul Resize Size
			pictureData = new CPictureData();
			if(resizeWidth != 0 && resizeHeight != 0)
			{
				float ratio = CalculPictureRatio(imageLoading->GetWidth(), imageLoading->GetHeight(), resizeWidth, resizeHeight);
				pictureData->width = imageLoading->GetWidth() * ratio;
				pictureData->height = imageLoading->GetHeight() * ratio;
				imageLoading->Resize(pictureData->width, pictureData->height, 1);
			}
			else
			{
				pictureData->width = imageLoading->GetWidth();
				pictureData->height = imageLoading->GetHeight();
			}
						
			unsigned long outputsize = 0;
			int compressMethod = 0;
			uint8_t * data = imageLoading->GetJpegData(outputsize, compressMethod);
			pictureData->data = new uint8_t[outputsize];
			memcpy(pictureData->data, data, outputsize);
			imageLoading->DestroyJpegData(data, compressMethod);

			pictureData->size = outputsize;
						
		}
	}

	if(imageLoading != nullptr)
		delete imageLoading;

	imageLoading = nullptr;

	return pictureData;
}

//---------------------------------------------------------------------------------------
//Test FacialRecognition
//---------------------------------------------------------------------------------------
void CMainWindow::FacialRecognition(void * param)
{
	CThreadMD5 * path = (CThreadMD5 *)param;
	bool pictureOK = false;
	CSqlFaceLabel sqlfaceLabel;
	void * faceData = path->mainWindow->faceData->GetCopyFaceData();
	CSqlFacePhoto sqlfacePhoto;
	CSqlFaceDescriptor sqlfaceDescritor;
	CFace * face = new CFace();
	CPictureData * pictureData = nullptr;
	int nbFaceFound = 0;
    CSqlThumbnail sqlThumbnail;
	CRegardsConfigParam * config = (CRegardsConfigParam*)CParamInit::getInstance();
	int pictureSize = config->GetFaceDetectionPictureSize();

	switch(pictureSize)
	{
		case 0: //Thumbnail Size
			if(sqlThumbnail.TestThumbnail(path->filename))
			{
				pictureData = sqlThumbnail.GetJpegThumbnail(path->filename);
				if(pictureData != nullptr)
					pictureOK = true;
			}
			else
			{
				pictureData = path->mainWindow->LoadPictureToJpeg(path->filename, pictureOK);
			}
			break;
		case 1: //Original Size
			{
				pictureData = path->mainWindow->LoadPictureToJpeg(path->filename, pictureOK);
			}
			break;

		case 2: //SD Format 
			{
				pictureData = path->mainWindow->LoadPictureToJpeg(path->filename, pictureOK, 640, 480);
			}
			break;


		case 3: //HD Format 
			{
				pictureData = path->mainWindow->LoadPictureToJpeg(path->filename, pictureOK, 1280, 720);
			}
			break;

		case 4: //Full HD Format 
			{
				pictureData = path->mainWindow->LoadPictureToJpeg(path->filename, pictureOK, 1920, 1080);
			}
			break;
	}




	if(pictureOK)
	{
		if(pictureData != nullptr)
			face->LoadPictureFromJpegBuffer(pictureData->data, pictureData->size, 0, faceData);
	
		int nbFace = face->GetNbFaceFound();

		if(nbFace > 0)
		{
			for(int i = 0;i < nbFace;i++)
			{
				bool findFaceCompatible = false;
				long width = 0;
				long height = 0;
				double pertinence = 0.0;
				bool result = false;
				uint8_t * data = nullptr;
				uint8_t * dataBitmap = nullptr;
				int size = 0;
				long bitmapSize = 0;

				face->GetFaceData(i, width, height, pertinence, dataBitmap, bitmapSize);
				if(bitmapSize > 0)
				{
					dataBitmap = new uint8_t[bitmapSize];
					face->GetFaceData(i, width, height, pertinence, dataBitmap, bitmapSize);

					size = bitmapSize;
					//Compression au format JPEG 
					data = new uint8_t[bitmapSize];
					result = compress_image_to_jpeg_file_in_memory(data, size, width, height, 4, dataBitmap);
					if(size > 0 && data != nullptr)
					{
						int numFace = 0;
						if(data != nullptr)
						{
							numFace = sqlfacePhoto.InsertFace(path->filename, i, width, height, pertinence, data, size);
							//numFace = sqlfacePhoto.GetNumFace(path->filename, i);
						}
	
						//Récupération des données du visage

						CFaceDescriptor * faceDescriptor = new CFaceDescriptor();
						faceDescriptor->numFace = numFace;
						faceDescriptor->size = 0;
						face->GetFaceDescriptor(i, faceDescriptor->descriptor, faceDescriptor->size);
						if(faceDescriptor->size > 0)
						{
							faceDescriptor->descriptor = new char[faceDescriptor->size+1];
							face->GetFaceDescriptor(i, faceDescriptor->descriptor, faceDescriptor->size);
						}

						sqlfaceDescritor.InsertFaceDescriptor(numFace, faceDescriptor->descriptor, faceDescriptor->size);
				
						if (path->mainWindow != nullptr)
						{
							wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_FACEPHOTOADD);
							evt.SetClientData(faceDescriptor);
							path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
						}

						nbFaceFound++;
					}

				}

				if(data != nullptr)
					delete[] data;

				if(dataBitmap != nullptr)
					delete[] dataBitmap;
			}
		}
	}
	

	if(nbFaceFound == 0)
	{
		int numFace = sqlfacePhoto.InsertFace(path->filename, 0, 0, 0, 0, nullptr, 0);
	}

	if(pictureData != nullptr)
		delete pictureData;

	if(face != nullptr)
		delete face;

	path->mainWindow->faceData->DeleteCopyFaceData(faceData);

	if (path->mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_FACEPHOTOUPDATE);
		evt.SetClientData(path);
		//evt.SetInt(updateCriteria);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CMainWindow::FacePhotoAdd(wxCommandEvent& event)
{
	CListFace * listFace = (CListFace *)this->FindWindowById(LISTFACEID);
	if(listFace != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_THUMBNAILADD);
		listFace->GetEventHandler()->AddPendingEvent(evt);
	}



	CThreadMD5 * path = (CThreadMD5 *)event.GetClientData();
	if (path->thread != nullptr)
	{
		path->thread->join();
		delete(path->thread);
		path->thread = nullptr;	
	}

	if(path != nullptr)
		delete path;

	nbProcessFacePhoto--;
}

void CMainWindow::RefreshTimer(wxCommandEvent& event)
{
    refreshTimer->Start();
}

void CMainWindow::ProcessIdle()
{
   // printf("CMainWindow::ProcessIdle() \n");
    bool hasDoneOneThings = true;
	int nbProcesseur = thread::hardware_concurrency() / 2;
    CSqlFacePhoto facePhoto;
    vector<wxString> listPhoto = facePhoto.GetPhotoListTreatment();

	//Recherche Nb Fichier non traité pour le visage
	if(nbProcessFacePhoto < nbProcesseur && listPhoto.size() > 0)
	{
        PhotosVector photoList;
        CSqlFindPhotos findphotos;
        findphotos.GetAllPhotos(&photoList);

        CSqlFacePhoto sqlFacePhoto;
        sqlFacePhoto.InsertFaceTreatment(listPhoto.at(0));

        CThreadMD5 * path = new CThreadMD5();
        path->filename = listPhoto.at(0);
        path->mainWindow = this;
        path->thread = new thread(FacialRecognition, path);	
        if (statusBarViewer != nullptr)
        {
            wxString message = "Photo : " + path->filename;
            statusBarViewer->SetText(2, message);
            message = "Processing : " + to_string(photoList.size() - listPhoto.size()) + "/" + to_string(photoList.size());
            statusBarViewer->SetText(3, message);
            statusBarViewer->SetRangeProgressBar(photoList.size());
            statusBarViewer->SetPosProgressBar(photoList.size() - listPhoto.size());
            nbProcessFacePhoto++;
        }
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
	}
	else if (refreshFolder)
	{
		refreshTimer->Stop();
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

        for (CPhotos photo : pictures)
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
			nbFile += sqlInsertFile.ImportFileFromFolder(folderlocal.GetFolderPath(), folderlocal.GetNumFolder(), fichier);
		}

		if (folderChange || nbFile > 0)
		{
			CViewerParam * viewerParam = (CViewerParam *)CViewerParamInit::getInstance();
			wxString sqlRequest = viewerParam->GetLastSqlRequest();

			CSqlFindPhotos sqlFindPhotos;
			sqlFindPhotos.SearchPhotos(sqlRequest);

			updateFolder = true;
			updateCriteria = true;
		}
		refreshFolder = false;
		//refreshTimer->Start();
        wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxTIMER_REFRESHTIMERSTART);
		this->GetEventHandler()->AddPendingEvent(evt);
	}
	else if (updateFolder)
	{


		CSqlFindPhotos sqlFindPhotos;

		wxString requestSql = centralWnd->GetSqlRequest();

		if (requestSql != "")
		{
			pictures.clear();
			sqlFindPhotos.SearchPhotos(requestSql);
			sqlFindPhotos.SearchPhotosByTypeAffichage(&pictures, typeAffichage, NUMCATALOGID);

			if (pictures.size() == 0)
				sqlFindPhotos.SearchPhotos(&pictures);
		}
		else
			sqlFindPhotos.SearchPhotos(&pictures);

		numElement = 0;

		wxWindow * window = this->FindWindowById(LISTPICTUREID);
		if (window)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SETLISTPICTURE);
			evt.SetClientData(&pictures);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		//centralWnd->SetListeFile(&pictures, typeAffichage);
		updateFolder = false;
		updatePicture = true;

		//refreshTimer->Start();
        wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxTIMER_REFRESHTIMERSTART);
		this->GetEventHandler()->AddPendingEvent(evt);
	}
	else if (updatePicture)
	{
		if (pictures.size() > 0)
		{

			if (filename != L"")
			{
				FindFileIndex(filename);
				firstFileToShow = filename;
			}
			else
				firstFileToShow = GetFilePath(numElement);

			centralWnd->SetNumElement(numElement);

			if (firstFileToShow != L"")
				LoadPictureInThread(firstFileToShow);

			numElementTraitement = 0;
		}
		updatePicture = false;
	}
	else if(loadPicture)
	{
        if(!videoStart)
        {
            wxString photoName = GetFilePath(numElement);
            if (firstFileToShow != photoName)
            {
                firstFileToShow = photoName;
                LoadPictureInThread(firstFileToShow);
            }
            centralWnd->SetNumElement(numElement);
            this->filename = photoName;
            loadPicture = false;
            sendMessageVideoStop = false;          
        }
        else if(videoStart && !sendMessageVideoStop)
        {
            CShowVideo * showVideoWindow = (CShowVideo *)this->FindWindowById(SHOWVIDEOVIEWERID);
            if (showVideoWindow != nullptr)
                showVideoWindow->StopVideo();

            sendMessageVideoStop = true;
        }
    }
	else if (numElementTraitement < pictures.size())
	{
        CPhotos photo = pictures.at(numElementTraitement);
        if (wxFileName::FileExists(photo.GetPath()))
        {
            //Test si thumbnail valide
            CViewerParam * config = CViewerParamInit::getInstance();
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


        wxString message = L"File Checking " + to_string(numElementTraitement) + L"/" + to_string(pictures.size());
        if (statusBarViewer != nullptr)
        {
            statusBarViewer->SetText(3, message);
        }       
	}
    else
        hasDoneOneThings = false;
        
    

    if(!hasDoneOneThings)
        processIdle = false;
        
    if(listPhoto.size() > 0)
        processIdle = true;
}

void CMainWindow::OnIdle(wxIdleEvent& evt)
{
	StartThread();
  // if (processIdle && !endProgram)
    //    ProcessIdle();
}

void CMainWindow::Md5Checking(wxCommandEvent& event)
{
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
	wxString message = L"File Checking " + to_string(numElementTraitement) + L"/" + to_string(pictures.size());
	if (statusBarViewer != nullptr)
	{
		statusBarViewer->SetText(3, message);
	}
}

void CMainWindow::CheckMD5(void * param)
{
	CThreadMD5 * path = (CThreadMD5 *)param;
	if (path != nullptr)
	{

		//Test if picture
		//CLibPicture libPicture;
		//if (libPicture.TestIsPicture(path->filename))
		//{
			//wxMD5 md5;
			CSqlThumbnail sqlThumbnail;
			//wxString md5file = md5.GetFileMD5(path->filename);
			wxFileName file(path->filename);
			wxULongLong sizeFile = file.GetSize();
			wxString md5file = sizeFile.ToString();


			bool result = sqlThumbnail.TestThumbnail(path->filename, md5file);
			if (!result)
			{
				//Remove thumbnail
				sqlThumbnail.DeleteThumbnail(path->filename);
			}
		//}
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_MD5CHECKING);
		evt.SetClientData(path);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}


}

void CMainWindow::SetSelectFile(const wxString &filename)
{
	this->filename = filename;
}

void CMainWindow::OnTimerDiaporama(wxTimerEvent& event)
{
	ImageSuivante();
}

void CMainWindow::OnVideoEnd(wxCommandEvent& event)
{
	videoStart = false;
	if(startDiaporama)
		ImageSuivante();

}

CMainWindow::~CMainWindow()
{
	if(faceData != nullptr)
		delete(faceData);

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

	if (diaporamaTimer->IsRunning())
		diaporamaTimer->Stop();

	delete(diaporamaTimer);

	if (refreshTimer->IsRunning())
		refreshTimer->Stop();

	delete(refreshTimer);
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

wxString CMainWindow::GetFilePath(const int &numElement)
{
	CPhotos photo = pictures.at(numElement);
	return photo.GetPath();
}

void CMainWindow::FindFileIndex(const wxString & filename)
{
	for (CPhotos file : pictures)
    {
        if (file.GetPath() == filename)
        {
            return;
        }
        else
            numElement++;
    }
    numElement = 0;
}


void CMainWindow::AddFolder(const wxString &folder)
{
	//Indication d'imporation des critères 
	CSqlFolderCatalog sqlFolderCatalog;
	int64_t idFolder = sqlFolderCatalog.GetFolderCatalogId(NUMCATALOGID, folder);
    
    printf("AddFolder : %s \n",folder.ToStdString().c_str());

	if (idFolder == -1)
	{
		idFolder = sqlFolderCatalog.GetOrInsertFolderCatalog(NUMCATALOGID, folder);
		//Insert la liste des photos dans la base de données.
		CSqlInsertFile sqlInsertFile;
		sqlInsertFile.ImportFileFromFolder(folder, idFolder, filename);
	}

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
 	wxString * info = (wxString *)event.GetClientData();
    
    printf("OnAddFolder : %s \n",info->ToStdString().c_str());
    
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
    
    
	CListFace * listFace = (CListFace *)this->FindWindowById(LISTFACEID);
	if(listFace != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_THUMBNAILREMOVE);
		listFace->GetEventHandler()->AddPendingEvent(evt);
	}


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
		}
	}
	delete info;
    
    processIdle = true;
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


void CMainWindow::PictureVideoClick(wxCommandEvent& event)
{
	long timePosition = event.GetExtraLong();
	if (centralWnd != nullptr)
	{
		int64_t pos = event.GetExtraLong();
		centralWnd->SetPosition(timePosition);
	}
}

void CMainWindow::SetVideoPos(wxCommandEvent& event)
{
	if (centralWnd != nullptr)
	{
		int64_t pos = event.GetExtraLong();
		centralWnd->SetVideoPosition(pos);
	}
}

void CMainWindow::FaceClick(wxCommandEvent& event)
{
	int photoId = event.GetExtraLong();
	wxString * photoPath = (wxString *)event.GetClientData();
	if(photoPath != nullptr && *photoPath != firstFileToShow)
	{
		wxString filename;
		bool findElement = false;
		filename = *photoPath;
		delete photoPath;
		numElement = 0;
		for (CPhotos photo : pictures)
		{
			if (filename == photo.GetPath())
			{
				findElement = true;
				break;
			}
			else
				numElement++;
		}

		if(!findElement)
		{
			CThreadPictureData * pictureData = new CThreadPictureData();
			pictureData->mainWindow = this;
			pictureData->picture = filename;
			pictureData->isVisible = true;
			thread * threadloadPicture = new thread(LoadingNewPicture, pictureData);
			pictureData->myThread = threadloadPicture; 
			processLoadPicture = true;

			LoadThumbnail(filename);

			firstFileToShow = filename;
		}

		if(findElement)
		{
			this->numElement = numElement;
			loadPicture = true;
            processIdle = true;
		}
	}
}

void CMainWindow::OnPictureClick(wxCommandEvent& event)
{
	int numElement = 0;
	int photoId = event.GetExtraLong();

	for (CPhotos photo : pictures)
	{
		if (photoId == photo.GetId())
		{
			break;
		}
		else
			numElement++;
	}
	this->numElement = numElement;
	loadPicture = true;
	processIdle = true;
}

void CMainWindow::PictureClickMove(wxCommandEvent& event)
{

	this->numElement = event.GetExtraLong();
	loadPicture = true;
    processIdle = true;
}

void CMainWindow::PictureClick(wxCommandEvent& event)
{
	this->numElement = event.GetExtraLong();
	loadPicture = true;
    processIdle = true;
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
			showBitmap->TransitionEnd();
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
void CMainWindow::UpdateInfos(CImageLoadingFormat * bitmap)
{
	if(bitmap != nullptr && bitmap->IsOk())
	{
		filename = bitmap->GetFilename();
		if (filename[0] != '\0')
		{
			//wxString folder = CFileUtility::GetFolder(filename);
			//wxString fichier = CFileUtility::GetFileName(filename);
			statusBarViewer->SetText(1, filename);
			//statusBarViewer->SetText(2, fichier);
			//Mise à jour de la barre de status
		}

		wxString infos = to_string(bitmap->GetWidth()) + "x" + to_string(bitmap->GetHeight());
		statusBarViewer->SetText(0, infos);

		infos = "Regards Viewer : " + CFileUtility::GetFileName(filename);
		statusBarViewer->SetWindowTitle(infos);
	}
	
}

bool CMainWindow::GetProcessEnd()
{
	if(processLoadPicture || nbProcessMD5 > 0 || nbProcessFacePhoto > 0)
		return false;

	return true;
}


void CMainWindow::Exit()
{
	statusBarViewer->Exit();
}

////////////////////////////////////////////////////////////////////////////////////////////////
///Gestion des événements du menu
////////////////////////////////////////////////////////////////////////////////////////////////
bool CMainWindow::OpenFolder()
{
	wxDirDialog dlg(nullptr, "Choose image directory", "",	wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dlg.ShowModal() == wxID_OK)
	{
		wxString path = dlg.GetPath();
		if (viewerParam != nullptr)
			viewerParam->SetCatalogCriteria("");


		wxWindow * window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
		if (window != nullptr)
		{
			wxString * folder = new wxString(path);
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_CHANGETYPEAFFICHAGE);
			evt.SetClientData(folder);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		AddFolder(path);

		updateCriteria = true;
		updateFolder = true;
        
        processIdle = true;
	}
	return false;
}

bool CMainWindow::IsFullscreen()
{
	return fullscreen;
}

void CMainWindow::InitPictures(wxCommandEvent& event)
{
	refreshFolder = true;
    processIdle = true;
	//updateCriteria = true;
}


void CMainWindow::FaceInfosUpdate(wxCommandEvent& event)
{
	updateCriteria = true;
    processIdle = true;
}

void CMainWindow::RefreshPicture(wxCommandEvent& event)
{
	LoadPictureInThread(this->filename);
    centralWnd->SetNumElement(numElement, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////
///Mise à jour des informations sur les fichiers
////////////////////////////////////////////////////////////////////////////////////////////////
void CMainWindow::SetPicture(CImageLoadingFormat * bitmap, const bool &isThumbnail)
{
	if(bitmap != nullptr && bitmap->IsOk())
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
	if (numElement >= pictures.size())
		numElement = 0;
	/*
	LoadPicture(numElement);
	centralWnd->SetNumElement(numElement);
    */
	loadPicture = true;
    processIdle = true;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

void CMainWindow::ImagePrecedente()
{
	numElement--;
	if (numElement < 0)
		numElement = (int)pictures.size() - 1;
	/*
	LoadPicture(numElement);
	centralWnd->SetNumElement(numElement);
	*/
	loadPicture = true;
    processIdle = true;
}


void CMainWindow::LoadPicture(const int &numElement)
{
    wxString fileToLoad = GetFilePath(numElement);
	if (wxFileExists(fileToLoad))
	{
		if (fileToLoad != firstFileToShow)
		{
			firstFileToShow = fileToLoad;
			LoadPictureInThread(firstFileToShow);
		}
		this->filename = fileToLoad;
	}
	else
	{
		ImageSuivante();
	}
}

void CMainWindow::Reload()
{
	LoadPicture(numElement);
}

void CMainWindow::ImageFin()
{
	/*
	numElement = (int)pictures.size() - 1;
	LoadPicture(numElement);
	centralWnd->SetNumElement(numElement);
	*/
	this->numElement = (int)pictures.size() - 1;
	loadPicture = true;
    processIdle = true;
}

void CMainWindow::ImageDebut()
{
		/*
	numElement = 0;
	LoadPicture(numElement);
	centralWnd->SetNumElement(numElement);
	*/
	this->numElement = 0;
	loadPicture = true;
    processIdle = true;
}

void CMainWindow::ShowToolbar()
{
	showToolbar = !showToolbar;

	if (!showToolbar)
		centralWnd->HideToolbar();
	else
		centralWnd->ShowToolbar();
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

void CMainWindow::LoadThumbnail(const wxString &filename)
{
    CSqlThumbnail sqlThumbnail;
	CImageLoadingFormat * _loadingPicture = new CImageLoadingFormat();
    CRegardsBitmap * bitmap = sqlThumbnail.GetPictureThumbnail(filename);
    if(bitmap == nullptr || !bitmap->IsValid())
    {
        //Chargement du thumbnail de l'image
        CLibPicture libPicture;
        _loadingPicture = libPicture.LoadThumbnail(filename, true);
    }
	else
	{
		_loadingPicture->SetPicture(bitmap);
	}
    
    needToReload = false;

    if (_loadingPicture != nullptr)
    {
        if (_loadingPicture->GetFilename() == firstFileToShow)
			SetPicture(_loadingPicture, true);
		else
			delete _loadingPicture;
    }

}

void CMainWindow::LoadPictureInThread(const wxString &filename, const bool &load)
{
	if (!wxFileExists(filename))
		this->filename = photoCancel;
	else
		this->filename = filename;

	CLibPicture libPicture;  
    
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
        
		if (startDiaporama || load)
		{
			
			CImageLoadingFormat * bitmap = libPicture.LoadPicture(filename);
			if(bitmap != nullptr && bitmap->IsOk())
			{
				if (bitmap != nullptr)
					delete bitmap;
				bitmap = libPicture.LoadPicture(photoCancel);
			}

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
	CImageLoadingFormat * bitmap = nullptr;
    
    if (pictureData != nullptr)
	{
        
        CSqlThumbnail sqlThumbnail;
        CImageLoadingFormat * _loadingPicture = new CImageLoadingFormat();
        CRegardsBitmap * bitmapThumbnail = sqlThumbnail.GetPictureThumbnail(pictureData->picture);
        if(bitmapThumbnail != nullptr && bitmapThumbnail->IsValid())
        {
            _loadingPicture->SetPicture(bitmapThumbnail);
            CBitmapReturn * bitmapReturn = new CBitmapReturn();
            bitmapReturn->myThread = nullptr;
            bitmapReturn->isThumbnail = true;
            bitmapReturn->bitmap = _loadingPicture;
            wxCommandEvent * event = new wxCommandEvent(EVENT_SHOWPICTURE);
            event->SetClientData(bitmapReturn);
            wxQueueEvent(pictureData->mainWindow, event);         
        }
        
		bitmap = libPicture.LoadPicture(pictureData->picture);
		if (bitmap == nullptr || (bitmap->GetWidth() == 0 || bitmap->GetHeight() == 0))
		{
			if (bitmap != nullptr)
				delete bitmap;
#ifdef WIN32
			wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "\\photo_cancel.png";
#else
			wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "/photo_cancel.png";
#endif
			bitmap = libPicture.LoadPicture(photoCancel);
		}
	}

	if (bitmap != nullptr)
	{
        CBitmapReturn * bitmapReturn = new CBitmapReturn();
        bitmapReturn->myThread = nullptr;
        bitmapReturn->isThumbnail = false;
        bitmapReturn->bitmap = bitmap;
		wxCommandEvent * event = new wxCommandEvent(EVENT_SHOWPICTURE);
		event->SetClientData(bitmapReturn);
		wxQueueEvent(pictureData->mainWindow, event);
	}

	wxCommandEvent * event = new wxCommandEvent(EVENT_ENDNEWPICTURETHREAD);
    event->SetClientData(pictureData->myThread);
	wxQueueEvent(pictureData->mainWindow, event);

	delete pictureData;
	pictureData = nullptr;
}

void CMainWindow::LoadingPicture(const wxString &filenameToShow)
{
               
    filenameTimer = filenameToShow;
	OnLoadPicture();
}

void CMainWindow::ShowPicture(wxCommandEvent& event)
{
	if (!updateFolder)
	{
		needToReload = false;
		CBitmapReturn * pictureData = (CBitmapReturn *)event.GetClientData();
		if (pictureData != nullptr)
		{
			if (pictureData->bitmap->GetFilename() == firstFileToShow)
				SetPicture(pictureData->bitmap, pictureData->isThumbnail);
			else
			{
				if (!pictureData->isThumbnail)
					needToReload = true;
				delete pictureData->bitmap;
			}
		}

		if (!pictureData->isThumbnail)
		{
			centralWnd->StopLoadingPicture();
		}

		if (pictureData->myThread != nullptr)
        {
			pictureData->myThread->join();
            delete pictureData->myThread;
        }
		delete pictureData;
	}
    processIdle = true;

}