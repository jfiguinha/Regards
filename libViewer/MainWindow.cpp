#include <header.h>
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
#include <CopyFileDlg.h>
#include <ShowVideo.h>
#include <FaceRecognition.h>
#include <SqlFacePhoto.h>
#include <SqlFaceDescriptor.h>
#include <SqlFindFacePhoto.h>
#include <SqlFaceRecognition.h>
#include <SqlFaceLabel.h>
#include <StatusText.h>
#include <turbojpeg.h>
//#include <jpge.h>
//using namespace jpge;
using namespace Regards::Viewer;
//const long ONE_MSEC = 1000;
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



wxDEFINE_EVENT(wxEVENT_SETSCREEN, wxCommandEvent);

CMainWindow::CMainWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarViewer)
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
	photoCancel = "";    
	processLoadPicture = false;
	videoStart = false;
	nbProcessFacePhoto = 0;
    imageList = new CImageList();
	//threadFacePhoto = nullptr;
    //wxSVGDocument* svgDoc = new wxSVGDocument;
    //svgDoc->Load("/Users/jacques/Pictures/test.svg");
    //wxImage img = svgDoc->Render(40,40,NULL,true,true);
    PhotosVector pictures;
	CSqlFindPhotos sqlFindPhotos;
	sqlFindPhotos.SearchPhotos(&pictures);
    imageList->SetImageList(pictures);
    
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
	centralWnd = new CCentralWnd(this, CENTRALVIEWERWINDOWID, statusBarViewer, imageList);
	this->statusBarViewer = statusBarViewer;


	Connect(wxTIMER_DIAPORAMA, wxEVT_TIMER, wxTimerEventHandler(CMainWindow::OnTimerDiaporama), nullptr, this);
	Connect(wxTIMER_REFRESH, wxEVT_TIMER, wxTimerEventHandler(CMainWindow::OnTimerRefresh), nullptr, this);
	Connect(wxEVENT_SETSCREEN, wxCommandEventHandler(CMainWindow::SetScreenEvent));
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
    Connect(wxTIMER_DIAPORAMATIMERSTART, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::StartDiaporamaMessage));
	Connect(wxEVENT_SETSTATUSTEXT, wxCommandEventHandler(CMainWindow::OnStatusSetText));
	Connect(wxEVENT_SETRANGEPROGRESSBAR, wxCommandEventHandler(CMainWindow::OnSetRangeProgressBar));
	Connect(wxEVENT_SETVALUEPROGRESSBAR, wxCommandEventHandler(CMainWindow::OnSetValueProgressBar));
    Connect(wxEVT_ANIMATIONTIMERSTART, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::StartAnimation));
    Connect(wxEVT_ANIMATIONTIMERSTOP, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CMainWindow::StopAnimation));
	statusBar = new wxStatusBar(this, wxID_ANY, wxSTB_DEFAULT_STYLE, "wxStatusBar");

	int tabWidth[] = { 100, 300, 300, 300 };
	statusBar->SetFieldsCount(4);
	statusBar->SetStatusWidths(4, tabWidth);

	progressBar = new wxGauge(statusBar, wxID_ANY, 200, wxPoint(1000, 0), wxSize(200, statusBar->GetSize().y),
		wxGA_HORIZONTAL);
	progressBar->SetRange(100);
	progressBar->SetValue(50);

	refreshTimer->Start(300000, wxTIMER_ONE_SHOT);
    //diaporamaTimer->Start(2000, wxTIMER_ONE_SHOT);
	updatePicture = true;
	loadFacePicture = false;

#ifdef WIN32
	this->photoCancel = CFileUtility::GetResourcesFolderPath() + "\\photo_cancel.png";
#else
	this->photoCancel = CFileUtility::GetResourcesFolderPath() + "/photo_cancel.png";
#endif

	refreshFolder = true;
    if(imageList->GetNbElement() > 0)
        processIdle = true;
    else
        processIdle = false;
	faceData = new CFaceLoadData();

#ifdef WIN32
	//Face Data Preload
	
	wxString model = CFileUtility::GetResourcesFolderPath() + "\\dlib_face_recognition_resnet_model_v1.dat";
    wxString facelandmark = CFileUtility::GetResourcesFolderPath() + "\\shape_predictor_5_face_landmarks.dat";

#else
	wxString model = CFileUtility::GetResourcesFolderPath() + "/dlib_face_recognition_resnet_model_v1.dat";
	//wxString facelandmark = CFileUtility::GetResourcesFolderPath() + "/shape_predictor_68_face_landmarks.dat";
    wxString facelandmark = CFileUtility::GetResourcesFolderPath() + "/shape_predictor_5_face_landmarks.dat";
#endif

	faceData->LoadData((const char*)model.mb_str(wxConvUTF8), (const char*)facelandmark.mb_str(wxConvUTF8));
	//centralWnd->ScreenMode();
	
}

void CMainWindow::StartAnimation(wxCommandEvent& event)
{
    TRACE();
    //this_thread::sleep_for(chrono::milliseconds(1000));
    centralWnd->StartAnimation();
    printf("CMainWindow::StartAnimation \n");
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

void CMainWindow::RefreshCriteriaPictureList(wxCommandEvent& event)
{
    TRACE();
	updateFolder = true;
    processIdle = true;
}

void CMainWindow::OnFacePertinence()
{
    TRACE();
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
    TRACE();
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
    TRACE();
	CBitmapWnd * window = (CBitmapWnd *)this->FindWindowById(BITMAPWINDOWVIEWERID);
	if (window)
	{
		window->SetFilterInterpolation(filter);
	}
}

void CMainWindow::OnTimerRefresh(wxTimerEvent& event)
{
    TRACE();
	refreshFolder = true;
    processIdle = true;
	refreshTimer->Stop();
}

void CMainWindow::OnThumbnailRight()
{
    TRACE();
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
    TRACE();
    /*
    CSqlThumbnail sqlThumbnail;
    CImageLoadingFormat * _loadingPicture = new CImageLoadingFormat();
    CRegardsBitmap * bitmapThumbnail = sqlThumbnail.GetPictureThumbnail(filenameTimer);
    if(bitmapThumbnail != nullptr && bitmapThumbnail->IsValid())
    {
        _loadingPicture->SetPicture(bitmapThumbnail);
        CBitmapReturn * bitmapReturn = new CBitmapReturn();
        bitmapReturn->myThread = nullptr;
        bitmapReturn->isThumbnail = true;
        bitmapReturn->bitmap = _loadingPicture;
        wxCommandEvent * event = new wxCommandEvent(EVENT_SHOWPICTURE);
        event->SetClientData(bitmapReturn);
        wxQueueEvent(this, event);         
    }    
    */
    
    if(!processLoadPicture)
    {
        CThreadPictureData * pictureData = new CThreadPictureData();
        pictureData->mainWindow = this;
        pictureData->picture = filenameTimer;
        pictureData->isVisible = true;
        thread * threadloadPicture = new thread(LoadingNewPicture, pictureData);
        pictureData->myThread = threadloadPicture; 
        processLoadPicture = true;
        threadPictureFilename = filenameTimer;
    }
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


void CMainWindow::ChangeTypeAffichage(wxCommandEvent& event)
{
    TRACE();
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
    TRACE();
	if(typeAffichage > 0)
    {
		updateFolder = true;
        processIdle = true;
    }
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
    cout << "OnSetValueProgressBar Pos : " << position << endl;
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
    cout << "OnSetRangeProgressBar Pos : " << range << endl;
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

void CMainWindow::EndPictureThread(wxCommandEvent& event)
{
    TRACE();
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

    if (startDiaporama)
    {
        int timeDelai = viewerParam->GetDelaiDiaporamaOption();
        diaporamaTimer->Start(timeDelai * 1000, wxTIMER_ONE_SHOT);
    }  
    
    if(threadPictureFilename != firstFileToShow)
    {
        updatePicture = true;
        processIdle = true;
    }
    
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


//---------------------------------------------------------------------------------------
//Test si compatible avec une autre Face à déplacer ailleurs
//---------------------------------------------------------------------------------------	
void CMainWindow::AddFacePhoto(wxCommandEvent& event)
{
    TRACE();
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
    TRACE();
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
    TRACE();
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
    TRACE();
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
				//bool findFaceCompatible = false;
				long width = 0;
				long height = 0;
				double pertinence = 0.0;
				bool result = false;
				uint8_t * data = nullptr;
				uint8_t * dataBitmap = nullptr;
				unsigned long size = 0;
				long bitmapSize = 0;

				face->GetFaceData(i, width, height, pertinence, dataBitmap, bitmapSize);
				if(bitmapSize > 0)
				{
					dataBitmap = new uint8_t[bitmapSize];
					face->GetFaceData(i, width, height, pertinence, dataBitmap, bitmapSize);

					size = bitmapSize;
					//Compression au format JPEG 
					//data = new uint8_t[bitmapSize];
					//result = compress_image_to_jpeg_file_in_memory(data, size, width, height, 4, dataBitmap);
                    
        uint8_t * data = nullptr;
  
	    const int JPEG_QUALITY = 75;

	    tjhandle _jpegCompressor = tjInitCompress();
    
		tjCompress2(_jpegCompressor, dataBitmap, width, 0, height, TJPF_RGBX,
			&data, &size, TJSAMP_444, JPEG_QUALITY,
			TJFLAG_FASTDCT);  

/*
	if (convertToRGB24)
	{
		tjCompress2(_jpegCompressor, dataBitmap, width, 0, height, TJPF_BGRX,
			&data, &outputsize, TJSAMP_444, JPEG_QUALITY,
			TJFLAG_FASTDCT | TJFLAG_BOTTOMUP);
	}
	else
	{
		tjCompress2(_jpegCompressor, dataBitmap, width, 0, height, TJPF_RGBX,
			&data, &outputsize, TJSAMP_444, JPEG_QUALITY,
			TJFLAG_FASTDCT);
	}*/
	      tjDestroy(_jpegCompressor);

                    
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
                {
                    tjFree(data);
                    data = nullptr;
                }
					//delete[] data;

				if(dataBitmap != nullptr)
					delete[] dataBitmap;
			}
		}
	}
	

	if(nbFaceFound == 0)
	{
		sqlfacePhoto.InsertFace(path->filename, 0, 0, 0, 0, nullptr, 0);
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
    TRACE();
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
    TRACE();
    refreshTimer->Start();
}


void CMainWindow::ProcessIdle()
{
    TRACE();
	//muPictureList.lock();
   // printf("CMainWindow::ProcessIdle() \n");
    bool hasDoneOneThings = false;
	//int nbProcesseur = thread::hardware_concurrency() / 2;
	int nbProcesseur = 1;
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
		nbProcesseur = config->GetFaceProcess();
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
        hasDoneOneThings = true;
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
			CViewerParam * viewerParam = (CViewerParam *)CViewerParamInit::getInstance();
			wxString sqlRequest = viewerParam->GetLastSqlRequest();

			CSqlFindPhotos sqlFindPhotos;
			sqlFindPhotos.SearchPhotos(sqlRequest);

			updateFolder = true;
			updateCriteria = true;
		}
		refreshFolder = false;
        wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxTIMER_REFRESHTIMERSTART);
		this->GetEventHandler()->AddPendingEvent(evt);
        hasDoneOneThings = true;
	}
	else if (updateFolder)
	{
        PhotosVector pictures;
		CSqlFindPhotos sqlFindPhotos;
		bool isValid = true;
		//wxString photoName = imageList->GetFilePath(numElement, isValid);
		wxString requestSql = centralWnd->GetSqlRequest();

		if (requestSql != "")
		{
			sqlFindPhotos.SearchPhotos(requestSql);
			sqlFindPhotos.SearchPhotosByTypeAffichage(&pictures, typeAffichage, NUMCATALOGID);

			if (imageList->GetNbElement() == 0)
				sqlFindPhotos.SearchPhotos(&pictures);
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
		

		//centralWnd->SetNumElement(numElement);

		wxWindow * window = this->FindWindowById(LISTPICTUREID);
		if (window)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SETLISTPICTURE);
			evt.SetClientData(imageList);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		//centralWnd->SetListeFile(&pictures, typeAffichage);
		updateFolder = false;
		updatePicture = true;
        wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxTIMER_REFRESHTIMERSTART);
		this->GetEventHandler()->AddPendingEvent(evt);
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
				centralWnd->SetNumElement(numElement);

				if (firstFileToShow != L"")
				{
					wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REFRESHPICTURE);
					filename = firstFileToShow;
					this->GetEventHandler()->AddPendingEvent(evt);
				}
			}
			//LoadPictureInThread(firstFileToShow);

			numElementTraitement = 0;
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
					wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REFRESHPICTURE);
					this->GetEventHandler()->AddPendingEvent(evt);
				}
				centralWnd->SetNumElement(numElement);
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
        }



        wxString message = L"File Checking " + to_string(numElementTraitement) + L"/" + to_string(imageList->GetNbElement());
        if (statusBarViewer != nullptr)
        {
            statusBarViewer->SetText(3, message);
        }   

        hasDoneOneThings = true;
	}
    
    
        
        
	//muPictureList.unlock();

    if(!hasDoneOneThings)
        processIdle = false;
        
    if(listPhoto.size() > 0)
        processIdle = true;
}

void CMainWindow::OnIdle(wxIdleEvent& evt)
{
    //TRACE();
    if(endProgram)
    {
        if (diaporamaTimer->IsRunning())
            diaporamaTimer->Stop();

        if (refreshTimer->IsRunning())
            refreshTimer->Stop();
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
	wxString message = L"File Checking " + to_string(numElementTraitement) + L"/" + to_string(imageList->GetNbElement());
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
    TRACE();
	this->filename = filename;
    centralWnd->HidePanel();
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
    TRACE();
	
	if (viewerParam != nullptr)
	{
		//centralWnd->SetDiaporamaMode();

		//if (viewerParam->GetFullscreenDiaporamaOption())
		//	SetFullscreen();
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
	if (centralWnd != nullptr)
	{
		//int64_t pos = event.GetExtraLong();
		centralWnd->SetPosition(timePosition);
	}
}

void CMainWindow::SetVideoPos(wxCommandEvent& event)
{
    TRACE();
	if (centralWnd != nullptr)
	{
		int64_t pos = event.GetExtraLong();
		centralWnd->SetVideoPosition(pos);
	}
}

void CMainWindow::FaceClick(wxCommandEvent& event)
{
    TRACE();
	//int photoId = event.GetExtraLong();
	wxString * photoPath = (wxString *)event.GetClientData();
	if(photoPath != nullptr && *photoPath != firstFileToShow)
	{
		wxString filename;
		bool findElement = false;
		filename = *photoPath;
		delete photoPath;
		numElement = imageList->FindFileIndex(filename);

		if(!findElement)
		{
            if(!processLoadPicture)
            {
                CThreadPictureData * pictureData = new CThreadPictureData();
                pictureData->mainWindow = this;
                pictureData->picture = filename;
                pictureData->isVisible = true;
                thread * threadloadPicture = new thread(LoadingNewPicture, pictureData);
                pictureData->myThread = threadloadPicture; 
                processLoadPicture = true;
                threadPictureFilename = filenameTimer;
            }
			//LoadThumbnail(filename);

			firstFileToShow = filename;
		}

		if(findElement)
		{
			//this->numElement = numElement;
			loadPicture = true;
            processIdle = true;
		}
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

void CMainWindow::PictureClickMove(wxCommandEvent& event)
{
    TRACE();
	this->numElement = event.GetExtraLong();
	loadPicture = true;
    processIdle = true;
}

void CMainWindow::PictureClick(wxCommandEvent& event)
{
    TRACE();
	this->numElement = event.GetExtraLong();
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

		//centralWnd->SetNormalMode();

		//centralWnd->SetNumElement(numElement);
		


	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
///Mise à jour des informations sur les fichiers
////////////////////////////////////////////////////////////////////////////////////////////////
void CMainWindow::UpdateInfos(CImageLoadingFormat * bitmap)
{
    TRACE();
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
    TRACE();
	if(processLoadPicture || nbProcessMD5 > 0 || nbProcessFacePhoto > 0)
		return false;

	return true;
}


void CMainWindow::Exit()
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


void CMainWindow::FaceInfosUpdate(wxCommandEvent& event)
{
    TRACE();
	updateCriteria = true;
    processIdle = true;
}

void CMainWindow::RefreshPicture(wxCommandEvent& event)
{
    TRACE();
	LoadPictureInThread(this->filename);
    centralWnd->SetNumElement(numElement, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////
///Mise à jour des informations sur les fichiers
////////////////////////////////////////////////////////////////////////////////////////////////
void CMainWindow::SetPicture(CImageLoadingFormat * bitmap, const bool &isThumbnail)
{
    TRACE();
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
    TRACE();
    clock_t tStart = clock();
    
	numElement++;
	if (numElement >= imageList->GetNbElement())
		numElement = 0;

    //LoadThumbnail(numElement);
     
	//centralWnd->SetNumElement(numElement);
    
    printf("Image Suivante %d \n",numElement);
    
	loadPicture = true;
    processIdle = true;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    
    
}

void CMainWindow::ImagePrecedente()
{
    TRACE();
	numElement--;
	if (numElement < 0)
		numElement = (int)imageList->GetNbElement() - 1;
	/*
	LoadPicture(numElement);
	centralWnd->SetNumElement(numElement);
	*/
	loadPicture = true;
    processIdle = true;
}


void CMainWindow::LoadPicture(const int &numElement)
{
    TRACE();
    bool isValid = false;
    wxString fileToLoad = imageList->GetFilePath(numElement,isValid);
    if (isValid)
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
    TRACE();
	LoadPicture(numElement);
}

void CMainWindow::ImageFin()
{
    TRACE();
	/*
	numElement = (int)imageList->GetNbElement() - 1;
	LoadPicture(numElement);
	centralWnd->SetNumElement(numElement);
	*/
	this->numElement = (int)imageList->GetNbElement() - 1;
	loadPicture = true;
    processIdle = true;
}

void CMainWindow::ImageDebut()
{
    TRACE();
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
    TRACE();
	showToolbar = !showToolbar;

	if (!showToolbar)
		centralWnd->HideToolbar();
	else
		centralWnd->ShowToolbar();
}




////////////////////////////////////////////////////////////////////////////////////////////////
///Affichage en mode plein écran
////////////////////////////////////////////////////////////////////////////////////////////////
void CMainWindow::SetFullscreen()
{
    TRACE();
	statusBarViewer->SetFullscreen();
}


void CMainWindow::LoadThumbnail(const int &numElement)
{
    TRACE();
    bool isValid = false;
    wxString filename = imageList->GetFilePath(numElement,isValid);
    if (isValid)
    {
        CSqlThumbnail sqlThumbnail;
        CImageLoadingFormat * _loadingPicture = new CImageLoadingFormat();
        CRegardsBitmap * bitmap = sqlThumbnail.GetPictureThumbnail(filename);
        if(bitmap != nullptr )
        {
            if(bitmap->IsValid())
            {
                _loadingPicture->SetPicture(bitmap);
                if (_loadingPicture->GetFilename() == firstFileToShow)
                    SetPicture(_loadingPicture, true);        
            }
            else
                delete _loadingPicture;
        }
    }
    /*
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
    */
}

void CMainWindow::LoadPictureInThread(const wxString &filename, const bool &load)
{
    TRACE();
    printf("CMainWindow::LoadPictureInThread %s \n", CConvertUtility::ConvertToUTF8(filename));
	if (!wxFileExists(filename))
		this->filename = photoCancel;
	else
		this->filename = filename;

	CLibPicture libPicture;  
    
	if (libPicture.TestIsVideo(filename))
	{
        centralWnd->StartLoadingPicture(numElement);
		centralWnd->SetVideo(filename);
		wxString infos = "Regards Viewer : " + CFileUtility::GetFileName(filename);
		statusBarViewer->SetWindowTitle(infos); 
	}
    else if(libPicture.TestIsAnimation(filename))
    {
        centralWnd->SetAnimation(filename);
        if (startDiaporama)
        {
            wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxTIMER_DIAPORAMATIMERSTART);
            this->GetEventHandler()->AddPendingEvent(evt);
        } 
		wxString infos = "Regards Viewer : " + CFileUtility::GetFileName(filename);
		statusBarViewer->SetWindowTitle(infos);  
    }
	else
	{
        centralWnd->StartLoadingPicture(numElement);
        LoadingPicture(filename);   
	}
 
}

void CMainWindow::SetScreenEvent(wxCommandEvent& event)
{
    TRACE();
    if(event.GetInt() == 1)
    {
        statusBarViewer->SetScreen();
        fullscreen = false;
        centralWnd->ScreenMode();
        toolbar->Show(true);
        statusBar->Show(true);        
    }
    else
    {
        fullscreen = true;
		centralWnd->FullscreenMode();
		toolbar->Show(false);
		statusBar->Show(false);   
    }

    this->Resize();
#ifdef __APPLE__
    wxWindow * window = this->FindWindowById(BITMAPWINDOWVIEWERID);
    wxCommandEvent localEvent(wxEVENT_OPENGLENABLEREFRESH);
    wxPostEvent(window, localEvent);  
#endif
}

void CMainWindow::SetFullscreenMode()
{
    TRACE();
	if (!fullscreen)
	{
		
        wxCommandEvent event(wxEVENT_SETSCREEN);
        event.SetInt(2);
        wxPostEvent(this, event);  

	}
}

void CMainWindow::SetScreen()
{ 
    TRACE();
	if (fullscreen)
	{
        wxCommandEvent event(wxEVENT_SETSCREEN);
        event.SetInt(1);
        wxPostEvent(this, event);         
    }
}

void CMainWindow::LoadingNewPicture(CThreadPictureData * pictureData)
{
    TRACE();
	CLibPicture libPicture;
	CImageLoadingFormat * bitmap = nullptr;
    
    if (pictureData != nullptr)
	{
        //if(!pictureData->mainWindow->startDiaporama)
        //{
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
        //}
        
        
        clock_t tStart = clock();
        bitmap = libPicture.LoadPicture(pictureData->picture);
        int timeToLoad = (int)((clock() - tStart)/(CLOCKS_PER_SEC / 1000));
        printf("Time To Load : %d \n",timeToLoad);
        if(timeToLoad < 300)
        {
            CRegardsConfigParam * config;
            config = CParamInit::getInstance();
            //Test si transition
            if(config != nullptr)
            {
                int numEffect = config->GetEffect();
                if(numEffect > 0)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(300 - timeToLoad));
                }
                    
            }
        }
		
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
    TRACE();
      
    filenameTimer = filenameToShow;
    
	OnLoadPicture();
}

void CMainWindow::ShowPicture(wxCommandEvent& event)
{
    TRACE();
    CBitmapReturn * pictureData = (CBitmapReturn *)event.GetClientData();
    if (pictureData != nullptr)
    {
        if (pictureData->bitmap->GetFilename() == firstFileToShow)
        {
            SetPicture(pictureData->bitmap, pictureData->isThumbnail);

            if (!pictureData->isThumbnail)
                centralWnd->StopLoadingPicture();               
        }
        else
            delete pictureData->bitmap;
            
        delete pictureData;
    }  
    //processIdle = true;

}