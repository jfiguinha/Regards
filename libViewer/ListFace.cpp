#include <header.h>
#ifndef __NOFACE_DETECTION__
#include "ListFace.h"
#include <ConfigParam.h>
#include <ParamInit.h>
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
#include "FileUtility.h"
#include <MoveFaceDialog.h>
#include <SqlFacePhoto.h>
#include <SqlFaceLabel.h>
#include <DeepLearning.h>
#include <ThumbnailMessage.h>
using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::DeepLearning;

class CThreadFace
{
public:

	CThreadFace()
	{
		thread = nullptr;
		mainWindow = nullptr;
	};
	~CThreadFace() {

	};



	wxString filename;
	std::thread * thread;
	wxWindow * mainWindow;
	int nbFace = 0;
};

CListFace::CListFace(wxWindow* parent, wxWindowID id)
	: CWindowMain("CListFace",parent, id)
{
	wxRect rect;
	thumbscrollbar = nullptr;
	thumbFaceToolbar = nullptr;
	thumbnailFace = nullptr;
	bool checkValidity = false;
	CMainParam * config = CMainParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

	
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();

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
		thumbnailFace->Init();

		windowManager->AddWindow(thumbnailFace, Pos::wxCENTRAL, false, 0, rect, wxID_ANY, false);
	}

	if (viewerTheme != nullptr)
	{
		std::vector<int> value = { 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700 };
		std::vector<int> valuePertinence = { 0, 10, 20, 50, 70, 100, 125, 150, 175, 200, 225, 250, 275, 300 };

		CThemeToolbar theme;
		viewerTheme->GetThumbnailToolbarTheme(theme);
		thumbFaceToolbar = new CThumbnailFaceToolBar(windowManager, wxID_ANY, theme, false);
		thumbFaceToolbar->SetTabValue(value);
		thumbFaceToolbar->SetTrackBarPosition(4);

		windowManager->AddWindow(thumbFaceToolbar, Pos::wxBOTTOM, true, thumbFaceToolbar->GetHeight(), rect, wxID_ANY, false);

		int position = 2;
		if(config != nullptr)
		{
			double pertinence = config->GetPertinenceValue();
			int pertinenceValue = pertinence * 100;
			for(int i = 0;i < valuePertinence.size();i++)
			{
				if(pertinenceValue == valuePertinence[i])
					position = i;
			}
		}

		thumbFacePertinenceToolbar = new CThumbnailFacePertinenceToolBar(windowManager, wxID_ANY, theme, false);//new CThumbnailFacePertinenceToolBar(this, wxID_ANY, theme);
		thumbFacePertinenceToolbar->SetTabValue(valuePertinence);
		thumbFacePertinenceToolbar->SetTrackBarPosition(2);

		windowManager->AddWindow(thumbFacePertinenceToolbar, Pos::wxTOP, true, thumbFacePertinenceToolbar->GetHeight(), rect, wxID_ANY, false);
	}

	
	Connect(wxEVT_IDLE, wxIdleEventHandler(CListFace::OnIdle));
	Connect(wxEVENT_RESOURCELOAD, wxCommandEventHandler(CListFace::OnResourceLoad));
	Connect(wxEVENT_FACEPHOTOADD, wxCommandEventHandler(CListFace::OnFacePhotoAdd));
	Connect(wxEVENT_THUMBNAILZOOMON, wxCommandEventHandler(CListFace::ThumbnailZoomOn));
	Connect(wxEVENT_THUMBNAILZOOMOFF, wxCommandEventHandler(CListFace::ThumbnailZoomOff));
	Connect(wxEVENT_THUMBNAILZOOMPOSITION, wxCommandEventHandler(CListFace::ThumbnailZoomPosition));
	Connect(wxEVENT_THUMBNAILREFRESH, wxCommandEventHandler(CListFace::ThumbnailRefresh));
	Connect(wxEVENT_THUMBNAILMOVE, wxCommandEventHandler(CListFace::ThumbnailMove));
	Connect(wxEVENT_THUMBNAILFOLDERADD, wxCommandEventHandler(CListFace::ThumbnailFolderAdd));

	isLoadingResource = true;
	CThreadFace * path = new CThreadFace();
	path->mainWindow = this;
	path->thread = new thread(LoadResource, path);

	nbProcessFacePhoto = 0;
	processIdle = false;
}


void CListFace::OnResourceLoad(wxCommandEvent& event)
{
	CThreadFace * path = (CThreadFace *)event.GetClientData();
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
}


void CListFace::ThumbnailFolderAdd(wxCommandEvent& event)
{
	processIdle = true;
}

void CListFace::OnFacePhotoAdd(wxCommandEvent& event)
{
	CThreadFace * path = (CThreadFace *)event.GetClientData();

	if (path != nullptr)
	{
		if (path->thread != nullptr)
		{
			path->thread->join();
			delete(path->thread);
			path->thread = nullptr;
		}


		//Update criteria
		if (path->nbFace > 0)
		{
			wxWindow * mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
			wxCommandEvent * eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
			wxQueueEvent(mainWnd, eventChange);
		}



		if (path != nullptr)
			delete path;
	}




	nbProcessFacePhoto--;

	wxCommandEvent evt(wxEVENT_THUMBNAILREFRESH);
	this->GetEventHandler()->AddPendingEvent(evt);

}

void CListFace::LoadResource(void * param)
{
	CThreadFace * path = (CThreadFace *)param;

	//load Ressource

#ifdef WIN32
	//Face Data Preload

	wxString config = CFileUtility::GetResourcesFolderPath() + "\\model\\opencv_face_detector.pbtxt";
	wxString weight = CFileUtility::GetResourcesFolderPath() + "\\model\\opencv_face_detector_uint8.pb";
	wxString json = CFileUtility::GetResourcesFolderPath() + "\\model\\rotation_model.json";
	wxString model = CFileUtility::GetResourcesFolderPath() + "\\model\\dlib_face_recognition_resnet_model_v1.dat";

#else

	wxString config = CFileUtility::GetResourcesFolderPath() + "/model/opencv_face_detector.pbtxt";
	wxString weight = CFileUtility::GetResourcesFolderPath() + "/model/opencv_face_detector_uint8.pb";
	wxString json = CFileUtility::GetResourcesFolderPath() + "/model/rotation_model.json";
	wxString model = CFileUtility::GetResourcesFolderPath() + "/model/dlib_face_recognition_resnet_model_v1.dat";
#endif

	CDeepLearning::LoadRessource(config.ToStdString(), weight.ToStdString(), model.ToStdString(), json.ToStdString());


	if (path->mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_RESOURCELOAD);
		evt.SetClientData(path);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

//---------------------------------------------------------------------------------------
//Test FacialRecognition
//---------------------------------------------------------------------------------------
void CListFace::FacialRecognition(void * param)
{
	CThreadFace * path = (CThreadFace *)param;
	bool pictureOK = false;
	vector<int> listFace;
	CPictureData * pictureData = nullptr;
	int nbFaceFound = 0;
	CSqlThumbnail sqlThumbnail;
	if (sqlThumbnail.TestThumbnail(path->filename))
	{
		pictureData = sqlThumbnail.GetJpegThumbnail(path->filename);
		if (pictureData != nullptr)
			pictureOK = true;
	}
	else
	{
		pictureData = CPictureData::LoadPictureToJpeg(path->filename, pictureOK);
	}

	if (pictureData != nullptr)
	{

		
		if (pictureOK)
		{
			listFace = CDeepLearning::FindFace(pictureData);
		}

		for (int numFace : listFace)
		{
			CDeepLearning::FindFaceCompatible(numFace);
		}

		delete pictureData;
	}

	if (path->mainWindow != nullptr)
	{
		path->nbFace = listFace.size();
		wxCommandEvent evt(wxEVENT_FACEPHOTOADD);
		evt.SetClientData(path);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}

}

void CListFace::OnIdle(wxIdleEvent& evt)
{
	if (endProgram)
	{
		processIdle = false;
	}

	StartThread();
}

bool CListFace::GetProcessEnd()
{
	TRACE();
	if (nbProcessFacePhoto > 0 || isLoadingResource)
		return false;
	return true;
}

void CListFace::ProcessIdle()
{
	int nbProcesseur = 1;
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
		nbProcesseur = config->GetFaceProcess();
	//Find picture to examine
	CSqlFacePhoto facePhoto;
	vector<wxString> listPhoto = facePhoto.GetPhotoListTreatment();

	//Find Face 

	if (nbProcessFacePhoto < nbProcesseur && listPhoto.size() > 0)
	{
		CSqlFacePhoto sqlFacePhoto;
		sqlFacePhoto.InsertFaceTreatment(listPhoto.at(0));

		CThreadFace * path = new CThreadFace();
		path->filename = listPhoto.at(0);
		path->mainWindow = this;
		path->thread = new thread(FacialRecognition, path);

		CThumbnailMessage * thumbnailMessage = new CThumbnailMessage();
		thumbnailMessage->nbPhoto = listPhoto.size();
		thumbnailMessage->typeMessage = 0;
		wxWindow * mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
		wxCommandEvent eventChange(wxEVENT_UPDATEMESSAGEFACE);
		eventChange.SetClientData(thumbnailMessage);
		if (mainWnd != nullptr)
			mainWnd->GetEventHandler()->AddPendingEvent(eventChange);
		else
			delete thumbnailMessage;

		nbProcessFacePhoto++;
	}

	//Recognize Face

	if (listPhoto.size() == 0)
	{
		processIdle = false;
	}
}

void CListFace::ThumbnailRefresh(wxCommandEvent& event)
{
	thumbnailFace->Init();
}

void CListFace::ThumbnailMove(wxCommandEvent& event)
{
	vector<CThumbnailData *> listItem;
	thumbnailFace->GetSelectItem(listItem);
	if (listItem.size() > 0)
	{
		//Choix de la Face
		MoveFaceDialog moveFaceDialog(this);
		moveFaceDialog.ShowModal();
		if(moveFaceDialog.IsOk())
		{
			thumbnailFace->MoveFace(moveFaceDialog.GetFaceNameSelected());
		}
	}
}


CListFace::~CListFace()
{
	delete(windowManager);

}

int CListFace::GetThumbnailHeight()
{
	return thumbnailFace->GetIconeHeight() + thumbscrollbar->GetBarHeight();
}

void CListFace::SetActifItem(const int &numItem, const bool &move)
{
	if (thumbnailFace != nullptr)
		thumbnailFace->SetActifItem(numItem, move);
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
		windowManager->SetSize(GetWindowWidth(), GetWindowHeight());
}

#endif