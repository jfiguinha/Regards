#include <header.h>

#ifndef __NOFACE_DETECTION__
#include "ListFace.h"
#include <ParamInit.h>
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include "MainTheme.h"
#include <libPicture.h>
#include "MainThemeInit.h"
#include <MoveFaceDialog.h>
#include <SqlFacePhoto.h>
#include <SQLRemoveData.h>
#include <DeepLearning.h>
#include <ThumbnailMessage.h>
#include <ImageLoadingFormat.h>
#include <ScrollbarWnd.h>
#include "ThumbnailFace.h"
#include "ThumbnailFaceToolBar.h"
#include "ThumbnailFacePertinenceToolBar.h"
#include "LibResource.h"
#include <SqlFaceRecognition.h>
#include <SqlFindFacePhoto.h>
#include <RegardsConfigParam.h>
#include <wx/progdlg.h>
#include <OpenCVVideoPlayer.h>
extern bool processrecognitionison;
extern bool isOpenCLInitialized;

using namespace Regards::Picture;
using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Video;
using namespace Regards::Viewer;
using namespace Regards::DeepLearning;

extern cv::ocl::OpenCLExecutionContext clExecCtx;

//#define CAFFE

class CThreadFace
{
public:
	CThreadFace()
	{
		thread = nullptr;
		mainWindow = nullptr;
	};

	~CThreadFace()
	{
	};


	wxString filename;
	std::thread* thread = nullptr;
	wxWindow* mainWindow = nullptr;
	wxProgressDialog* dialog = nullptr;
	int nbFace = 0;
	int type = 0;
};

CListFace::CListFace(wxWindow* parent, wxWindowID id)
	: CWindowMain("CListFace", parent, id)
{
	wxRect rect;
	thumbscrollbar = nullptr;
	thumbFaceToolbar = nullptr;
	thumbnailFace = nullptr;
	bool checkValidity = false;
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

	std::vector<int> value = {60, 70, 80, 90, 100};
	std::vector<int> valueZoom = {100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600};

	int positionTab = 3;
	if (config != nullptr)
		config->GetSlideFacePos(positionTab);

	CMainTheme* viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetSplitterTheme(&theme);
		windowManager = new CWindowManager(this, wxID_ANY, theme);
	}


	if (viewerTheme != nullptr)
	{
		CThemeThumbnail themeThumbnail;
		CThemeScrollBar theme;
		viewerTheme->GetScrollTheme(&theme);
		viewerTheme->GetThumbnailTheme(&themeThumbnail);
		thumbnailFace = new CThumbnailFace(windowManager, THUMBNAILFACE, themeThumbnail, checkValidity);
		thumbscrollbar = new CScrollbarWnd(windowManager, thumbnailFace, wxID_ANY);
		thumbscrollbar->ShowVerticalScroll();
		thumbnailFace->SetNoVScroll(false);
		thumbnailFace->SetCheck(true);
		thumbnailFace->ChangeTabValue(valueZoom, positionTab);
		thumbnailFace->init();

		windowManager->AddWindow(thumbscrollbar, Pos::wxCENTRAL, false, 0, rect, wxID_ANY, false);
	}

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		//viewerTheme->GetThumbnailToolbarTheme(theme);
		viewerTheme->GetBitmapToolbarTheme(&theme);

		thumbFaceToolbar = new CThumbnailFaceToolBar(windowManager, wxID_ANY, theme, false);
		thumbFaceToolbar->SetTabValue(valueZoom);
		thumbFaceToolbar->SetTrackBarPosition(positionTab - 1);

		windowManager->AddWindow(thumbFaceToolbar, Pos::wxBOTTOM, true, thumbFaceToolbar->GetHeight(), rect, wxID_ANY,
		                         false);
	}

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		int position = 2;
		if (config != nullptr)
		{
			double pertinence = config->GetPertinenceValue();
			int pertinenceValue = pertinence;
			for (int i = 0; i < value.size(); i++)
			{
				if (pertinenceValue == value[i])
					position = i;
			}
		}

		viewerTheme->GetThumbnailToolbarTheme(theme);
		thumbFacePertinenceToolbar = new CThumbnailFacePertinenceToolBar(windowManager, wxID_ANY, theme, false);
		//new CThumbnailFacePertinenceToolBar(this, wxID_ANY, theme);
		thumbFacePertinenceToolbar->SetTabValue(value);
		thumbFacePertinenceToolbar->SetTrackBarPosition(position);
		windowManager->AddWindow(thumbFacePertinenceToolbar, Pos::wxTOP, true, thumbFacePertinenceToolbar->GetHeight(),
		                         rect, wxID_ANY, false);

		/*
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLFACELIST", 1);

		titleBar = new CTitleBar(windowManager, wxID_ANY, this);
		titleBar->SetRefresh(true);
		titleBar->SetTitle(libelle);
		titleBar->SetClosable(false);
		windowManager->AddWindow(titleBar, Pos::wxTOP, true, titleBar->GetHeight(), rect, wxID_ANY, false);
		*/
	}


	Connect(wxEVT_IDLE, wxIdleEventHandler(CListFace::OnIdle));
	Connect(wxEVENT_RESOURCELOAD, wxCommandEventHandler(CListFace::OnResourceLoad));
	Connect(wxEVENT_FACEVIDEOADD, wxCommandEventHandler(CListFace::OnFaceVideoAdd));
	Connect(wxEVENT_FACEPHOTOADD, wxCommandEventHandler(CListFace::OnFacePhotoAdd));
	Connect(wxEVENT_REFRESHFOLDER, wxCommandEventHandler(CListFace::OnRefreshFolder));
	Connect(wxEVENT_THUMBNAILZOOMON, wxCommandEventHandler(CListFace::ThumbnailZoomOn));
	Connect(wxEVENT_THUMBNAILZOOMON, wxCommandEventHandler(CListFace::ThumbnailZoomOn));
	Connect(wxEVENT_THUMBNAILZOOMOFF, wxCommandEventHandler(CListFace::ThumbnailZoomOff));
	Connect(wxEVENT_THUMBNAILZOOMPOSITION, wxCommandEventHandler(CListFace::ThumbnailZoomPosition));
	Connect(wxEVENT_THUMBNAILREFRESH, wxCommandEventHandler(CListFace::ThumbnailRefresh));
	Connect(wxEVENT_THUMBNAILMOVE, wxCommandEventHandler(CListFace::ThumbnailMove));
	Connect(wxEVENT_THUMBNAILFOLDERADD, wxCommandEventHandler(CListFace::ThumbnailFolderAdd));
	Connect(wxEVENT_THUMBNAILREFRESHFACE, wxCommandEventHandler(CListFace::ThumbnailDatabaseRefresh));


	processIdle = true;

	nbProcessFacePhoto = 0;


	isLoadingResource = false;
	listProcessWindow.push_back(this);
}



CThumbnailFace* CListFace::GetThumbnailFace()
{
	return thumbnailFace;
}

vector<int> CListFace::GetFaceSelectID()
{
	return thumbnailFace->GetFaceSelectID();
}

int CListFace::ImageSuivante()
{
	int numItem = -1;
	if (thumbnailFace != nullptr)
		numItem = thumbnailFace->ImageSuivante();
	/*
	if(numItem =- -1)
	{
		int photoId = thumbnailFace->GetNumPhotoId(numItem);
		thumbnailFace->SetActifItem(photoId, false);
	}
	*/
	return numItem;
}


wxString CListFace::GetFilename(const int& numItem)
{
	if (thumbnailFace != nullptr)
		return thumbnailFace->GetFilename(numItem);
	return "";
}

int CListFace::GetNumItem()
{
	if (thumbnailFace != nullptr)
		return thumbnailFace->GetNumItem();
	return -1;
}

int CListFace::ImagePrecedente()
{
	int numItem = 0;
	if (thumbnailFace != nullptr)
		numItem = thumbnailFace->ImagePrecedente();
	/*
	if(numItem =- -1)
	{
		int photoId = thumbnailFace->GetNumPhotoId(numItem);
		thumbnailFace->SetActifItem(photoId, false);
	}
	*/
	return numItem;
}


void CListFace::ClosePane()
{
}

void CListFace::RefreshPane()
{
	//Delete all face
	if (!isLoadingResource)
	{
		wxString title = CLibResource::LoadStringFromResource(L"LBLSTOPALLPROCESS", 1);
		wxString message = CLibResource::LoadStringFromResource(L"LBLSTOPPROCESS", 1);
		StopAllProcess(title, message, this);

		//Delete all Face data
		CSQLRemoveData::DeleteFaceDatabase();

		SetStopProcess(false);

		processIdle = true;
	}
}

void CListFace::OnResourceLoad(wxCommandEvent& event)
{
	auto path = static_cast<CThreadFace*>(event.GetClientData());
	if (path->thread != nullptr)
	{
		path->thread->join();
		delete(path->thread);
		path->thread = nullptr;
	}

	if (path != nullptr)
		delete path;

	processIdle = true;
	isLoadingResource = false;
	resourceLoaded = true;
}


void CListFace::ThumbnailFolderAdd(wxCommandEvent& event)
{
	processIdle = true;
}

void CListFace::OnFaceVideoAdd(wxCommandEvent& event)
{
	auto path = static_cast<CThreadFace*>(event.GetClientData());
	//int nbFace = 0;
	if (path != nullptr)
	{
		//Update criteria
		if (path->nbFace > 0)
		{
			wxWindow* mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
			auto eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
			wxQueueEvent(mainWnd, eventChange);
		}
	}

	if (path->nbFace > 0)
	{
		wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
		this->GetEventHandler()->AddPendingEvent(evt);
	}
}


void CListFace::OnFacePhotoAdd(wxCommandEvent& event)
{
	auto path = static_cast<CThreadFace*>(event.GetClientData());
	int nbFace = 0;
	int type = 0;
	if (path != nullptr)
	{
		type = path->type;
		if (path->thread != nullptr)
		{
			path->thread->join();
			delete(path->thread);
			path->thread = nullptr;
		}

		nbFace = path->nbFace;

		//Update criteria
		if (path->nbFace > 0)
		{
			wxWindow* mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
			auto eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
			wxQueueEvent(mainWnd, eventChange);
		}

		if (path->dialog != nullptr)
			delete path->dialog;

		if (path != nullptr)
			delete path;
	}

	if (type == 0)
	{
		nbProcessFacePhoto--;

		muListFace.lock();
		nbNbFace += nbFace;
		muListFace.unlock();
	}
	else
	{
		nbProcessFaceRecognition--;

		muListFace.lock();
		nbNbFace--;
		muListFace.unlock();
	}
		

	if (nbFace > 0)
	{
		wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
		this->GetEventHandler()->AddPendingEvent(evt);


	}
	processIdle = true;
}

void CListFace::LoadResource(void* param)
{
	bool openCLCompatible = false;
	bool cudaCompatible = false;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
	{
		if (config->GetIsOpenCLSupport())
			openCLCompatible = true;
		if (config->GetIsCudaSupport())
			cudaCompatible = true;
	}

	auto path = static_cast<CThreadFace*>(param);
	CDeepLearning::LoadRessource(openCLCompatible, cudaCompatible);

	if (path->mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_RESOURCELOAD);
		evt.SetClientData(path);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}


void CListFace::FacialDetectionRecognition(void* param)
{

	if (!clExecCtx.empty())
		clExecCtx.bind();

	auto path = static_cast<CThreadFace*>(param);
	wxString filename = path->filename;

	bool fastDetection = true;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		fastDetection = config->GetFastDetectionFace();

	std::vector<int> listFace;
	if (filename == "")
	{
		int i = 0;
		for (int numFace : listFace)
		{
			wxString text = "Face number : " + to_string(i);
			CDeepLearning::FindFaceCompatible(numFace, fastDetection);
			if (false == path->dialog->Update(i++, text))
				break;
		}
	}
	else
	{
		CSqlFindFacePhoto facePhoto;
		listFace = facePhoto.GetListFaceToRecognize();

		//int i = 0;
		for (int numFace : listFace)
		{
			CDeepLearning::FindFaceCompatible(numFace, fastDetection);
			break;
		}
	}

	path->nbFace = listFace.size();

	if (path->mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_FACEPHOTOADD);
		path->type = 1;
		evt.SetClientData(path);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}

}

void CListFace::FacialRecognitionReload()
{
	bool fastDetection = true;
	int nbProcesseur = 1;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
	{
		fastDetection = config->GetFastDetectionFace();
		nbProcesseur = config->GetFaceProcess();
	}


	if (nbProcessFaceRecognition < nbProcesseur)
	{
		CSqlFaceRecognition faceRecognition;
		faceRecognition.DeleteFaceRecognitionDatabase();
		CSqlFindFacePhoto facePhoto;
		std::vector<int> listFace = facePhoto.GetListFaceToRecognize();
		wxProgressDialog dialog("Face Recognition", "", listFace.size(), nullptr,
		                        wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);

		int i = 0;
		for (int numFace : listFace)
		{
			wxString text = "Face number : " + to_string(i);
			CDeepLearning::FindFaceCompatible(numFace, fastDetection);
			if (false == dialog.Update(i++, text))
				break;
		}

		//Update criteria
		if (!listFace.empty())
		{
			wxWindow* mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
			auto eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
			wxQueueEvent(mainWnd, eventChange);

			wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
			this->GetEventHandler()->AddPendingEvent(evt);
		}
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void CListFace::FindFaceCompatible(const vector<int>& listFace)
{
	bool fastDetection = true;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		fastDetection = config->GetFastDetectionFace();

	for (int numFace : listFace)
	{
		CDeepLearning::FindFaceCompatible(numFace, fastDetection);
	}
}

//---------------------------------------------------------------------------------------
//Test FacialRecognition
//---------------------------------------------------------------------------------------
void CListFace::FacialRecognition(void* param)
{
	auto path = static_cast<CThreadFace*>(param);
	bool pictureOK = false;
	CLibPicture libPicture;
	path->nbFace = 0;
	vector<int> listFace;
	int faceVideoDetection = 0;
	bool fastDetection = true;

	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
	{
		fastDetection = config->GetFastDetectionFace();
		faceVideoDetection = config->GetFaceVideoDetection();
	}


	if (faceVideoDetection && libPicture.TestIsVideo(path->filename))
	{
		//Open Frame By Frame to Detect Face
		COpenCVVideoPlayer video(path->filename);
		int width = 0;
		int height = 0;
		path->nbFace = 0;
		CImageLoadingFormat pictureData;
		int totalFrame = video.GetTotalFrame();
		for (int i = 0; i < totalFrame; i++)
		{
			path->nbFace = 0;
			cv::Mat frame = video.GetVideoFrame();
			if (!frame.empty())
			{
				pictureData.SetPicture(frame);
				pictureData.ConvertToBGR();
				pictureData.Flip();
				pictureData.SetFilename(path->filename);
				pictureData.SetOrientation(0);
				listFace = CDeepLearning::FindFace(frame, path->filename, fastDetection);
				path->nbFace = listFace.size();
			}


			CSqlFacePhoto facePhoto;
			for (int numFace : listFace)
			{
				facePhoto.UpdateVideoFace(numFace, i);
			}

			if (path->nbFace > 0)
			{
				if (path->mainWindow != nullptr)
				{
					wxCommandEvent evt(wxEVENT_FACEVIDEOADD);
					evt.SetClientData(path);
					path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
				}
			}
		}
	}
	else
	{
		CImageLoadingFormat* pictureData = libPicture.LoadPictureToBGRA(path->filename, pictureOK);
		if (pictureOK && pictureData != nullptr)
		{
			pictureData->SetFilename(path->filename);

			listFace = CDeepLearning::FindFace(pictureData->GetMatrix().getMat(), path->filename, fastDetection);
			path->nbFace = listFace.size();
		}

		if (pictureData != nullptr)
			delete pictureData;
	}


	if (path->mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_FACEPHOTOADD);
		path->type = 0;
		evt.SetClientData(path);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CListFace::OnIdle(wxIdleEvent& evt)
{
	if (needToRefresh)
	{
		this->Refresh();
		needToRefresh = false;
	}

	if (endProgram)
	{
		processIdle = false;
	}

	StartThread();
}

bool CListFace::GetProcessEnd()
{
	if (nbProcessFacePhoto > 0 || nbProcessFaceRecognition > 0)
		return false;
	return true;
}

void CListFace::ProcessIdle()
{
	int faceDetection = 0;
	CRegardsConfigParam* regardsParam = CParamInit::getInstance();


	if (!isOpenCLInitialized && (regardsParam->GetIsOpenCLSupport() && !regardsParam->GetIsUseCuda()))
	{
		processIdle = true;
		return;
	}

	if (!isLoadingResource && !resourceLoaded)
	{
		isLoadingResource = true;
		auto path = new CThreadFace();
		path->mainWindow = this;
		path->thread = new thread(LoadResource, path);
		return;
	}
	if (isLoadingResource)
	{
		processIdle = true;
		return;
	}


	if (regardsParam != nullptr)
	{
		faceDetection = regardsParam->GetFaceDetection();
	}

	if (!faceDetection)
	{
		processIdle = false;
		return;
	}


	bool sendMessageStatus = true;
	int nbProcesseur = 1;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		nbProcesseur = config->GetFaceProcess();
	//Find picture to examine


	//Find Face 
	int listPhotoSize = 0;
	bool processPhoto = false;
	muListPhoto.lock();

	listPhotoSize = listPhoto.size();

	if (nbProcessFacePhoto < nbProcesseur && listPhotoSize > 0)
	{
		CSqlFacePhoto sqlFacePhoto;
		sqlFacePhoto.InsertFaceTreatment(listPhoto.at(0));

		auto path = new CThreadFace();
		path->filename = listPhoto.at(0);
		path->mainWindow = this;
		path->thread = new thread(FacialRecognition, path);
		nbProcessFacePhoto++;

		processPhoto = true;

		listPhoto.erase(listPhoto.begin());
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	muListPhoto.unlock();

	if(processPhoto && sendMessageStatus)
	{
		auto thumbnailMessage = new CThumbnailMessage();
		thumbnailMessage->nbPhoto = listPhotoSize;
		thumbnailMessage->thumbnailPos = nbProcessFacePhoto;
		thumbnailMessage->nbElement = listPhotoSize;
		thumbnailMessage->typeMessage = 4;
		wxWindow* mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
		wxCommandEvent eventChange(wxEVENT_UPDATESTATUSBARMESSAGE);
		eventChange.SetClientData(thumbnailMessage);
		mainWnd->GetEventHandler()->AddPendingEvent(eventChange);
	}
	
	muListFace.lock();
	int nbFaceLocal = nbNbFace;
	muListFace.unlock();

	if (nbProcessFaceRecognition == 0)
	{
		
		if (nbFaceLocal > 0)
		{
			auto path = new CThreadFace();
			path->mainWindow = this;
			path->filename = "internal";
			path->thread = new thread(FacialDetectionRecognition, path);
			nbProcessFaceRecognition++;

			if (sendMessageStatus)
			{
				if (cleanDatabase)
					CDeepLearning::CleanRecognition();
				cleanDatabase = false;
				auto thumbnailMessage = new CThumbnailMessage();
				thumbnailMessage->nbPhoto = nbFaceLocal;
				thumbnailMessage->thumbnailPos = nbProcessFaceRecognition;
				thumbnailMessage->nbElement = nbFaceLocal;
				thumbnailMessage->typeMessage = 5;
				wxWindow* mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
				wxCommandEvent eventChange(wxEVENT_UPDATESTATUSBARMESSAGE);
				eventChange.SetClientData(thumbnailMessage);
				mainWnd->GetEventHandler()->AddPendingEvent(eventChange);
			}
		}

		
	}

	//Recognize Face

	if (listPhotoSize == 0 && nbFaceLocal == 0)
	{
		processIdle = false;
	}
}

void CListFace::ThumbnailRefresh(wxCommandEvent& event)
{
	cleanDatabase = true;
	thumbnailFace->init();
	processIdle = true;
}

void CListFace::ThumbnailDatabaseRefresh(wxCommandEvent& event)
{
	cleanDatabase = true;
	RefreshPane();
	thumbnailFace->init();
	processIdle = true;
	needToRefresh = true;
}

void CListFace::ThumbnailMove(wxCommandEvent& event)
{
	vector<CThumbnailData*> listItem;
	thumbnailFace->GetSelectItem(listItem);
	if (listItem.size() > 0)
	{
		//Choix de la Face
		MoveFaceDialog moveFaceDialog(this);
		moveFaceDialog.ShowModal();
		if (moveFaceDialog.IsOk())
		{
			thumbnailFace->MoveFace(moveFaceDialog.GetFaceNameSelected());
		}

		//Cleanup Name
	}
}


CListFace::~CListFace()
{
	int positionTab = thumbnailFace->GetTabValue();
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		config->SetSlideFacePos(positionTab);

    printf("~CListFace() \n");
    delete(thumbnailFace);
    delete(thumbFaceToolbar);
    delete(thumbscrollbar);
    delete(thumbFacePertinenceToolbar);
	delete(windowManager);
    
    printf("~CListFace() end \n");
}

int CListFace::GetThumbnailHeight()
{
	return thumbnailFace->GetIconeHeight() + thumbscrollbar->GetBarHeight();
}

void CListFace::SetActifItem(const int& numItem, const bool& move)
{
	if (thumbnailFace != nullptr)
		thumbnailFace->SetActifItem(numItem, move);
}

void CListFace::OnRefreshFolder(wxCommandEvent& event)
{
	//Update Photo List
	muListPhoto.lock();
	CSqlFacePhoto facePhoto;
	listPhoto = facePhoto.GetPhotoListTreatment();
	muListPhoto.unlock();

	muListFace.lock();
	CSqlFindFacePhoto faceRecognition;
	nbNbFace = faceRecognition.GetNbListFaceToRecognize();
	muListFace.unlock();


}

void CListFace::ThumbnailZoomOn(wxCommandEvent& event)
{
	thumbnailFace->ZoomOn();
}

void CListFace::ThumbnailZoomOff(wxCommandEvent& event)
{
	thumbnailFace->ZoomOff();
}

void CListFace::ThumbnailZoomPosition(wxCommandEvent& event)
{
	thumbnailFace->ZoomPosition(event.GetExtraLong());
}

void CListFace::UpdateScreenRatio()
{
	if (windowManager != nullptr)
		windowManager->UpdateScreenRatio();
}

void CListFace::Resize()
{
	if (windowManager != nullptr)
	{
		windowManager->SetSize(GetWindowWidth(), GetWindowHeight());
		needToRefresh = true;
	}
}

#endif
