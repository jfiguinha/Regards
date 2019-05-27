#include <header.h>
#include "ListFace.h"
#include <MoveFaceDialog.h>
#include <FileUtility.h>
#include <SQLRemoveData.h>
#include <SqlFindCriteria.h>
#include <ParamInit.h>
#include <ThumbnailData.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#include <turbojpeg.h>
#include <LibResource.h>
#include "MainWindow.h"
#include <window_id.h>
#include <CopyFileDlg.h>
#include <ExportFile.h>
#include <ConvertUtility.h>
#include <wx/dir.h>
#include <libPicture.h>
#include <MapSelect.h>
#include <CalendarSelect.h>
#include "SqlFindPhotos.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <FaceRecognition.h>
#include <SqlFacePhoto.h>
#include <SqlFaceDescriptor.h>
#include <SqlFindFacePhoto.h>
#include <SqlFaceRecognition.h>
#include <SqlFaceLabel.h>
#include "FaceInfosUpdate.h"
using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Viewer;

class CThreadFace
{
public:

	CThreadFace()
	{
		thread = nullptr;
		mainWindow = nullptr;
		faceData = nullptr;
	};
	~CThreadFace() {

	};

	CPictureData * LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth = 0, const int &resizeHeight = 0);
	float CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &widthOutput, const int &heightOutput);
	
	wxString filename;
	std::thread * thread;
	wxWindow * mainWindow;
	CFaceLoadData * faceData;
};


float CThreadFace::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &widthOutput, const int &heightOutput)
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


CPictureData * CThreadFace::LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth, const int &resizeHeight)
{
	TRACE();
	CPictureData * pictureData = nullptr;
	CLibPicture libPicture;
	CImageLoadingFormat * imageLoading = libPicture.LoadPicture(filename);
	if (imageLoading != nullptr)
	{
		pictureOK = imageLoading->IsOk();
		if (pictureOK)
		{
			imageLoading->ApplyExifOrientation(1);
			imageLoading->ConvertToRGB24(true);
			//Calcul Resize Size
			pictureData = new CPictureData();
			if (resizeWidth != 0 && resizeHeight != 0)
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

	if (imageLoading != nullptr)
		delete imageLoading;

	imageLoading = nullptr;

	return pictureData;
}




CListFace::CListFace(wxWindow* parent, wxWindowID id)
	: CWindowMain("CListFace",parent, id)
{
	thumbscrollbar = nullptr;
	thumbFaceToolbar = nullptr;
	thumbnailFace = nullptr;
	update = true;
	nbProcessFacePhoto = 0;
	faceData = new CFaceLoadData();
	bool checkValidity = false;
	CViewerParam * config = CViewerParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

	
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeThumbnail themeThumbnail;
		CThemeScrollBar theme;
		viewerTheme->GetScrollTheme(&theme);
		thumbscrollbar = new CScrollbarWnd(this, wxID_ANY);

		viewerTheme->GetThumbnailTheme(&themeThumbnail);
		thumbnailFace = new CThumbnailFace(thumbscrollbar, THUMBNAILFACE, themeThumbnail, checkValidity);
		//thumbnailWindow->Init(typeAffichage);
		thumbscrollbar->SetCentralWindow(thumbnailFace, theme);
	}

	if (viewerTheme != nullptr)
	{
		std::vector<int> value = { 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700 };
		std::vector<int> valuePertinence = { 0, 10, 20, 50, 70, 100, 125, 150, 175, 200, 225, 250, 275, 300 };

		CThemeToolbar theme;
		viewerTheme->GetThumbnailToolbarTheme(theme);
		thumbFaceToolbar = new CThumbnailFaceToolBar(this, wxID_ANY, theme, false);
		thumbFaceToolbar->SetTabValue(value);
		thumbFaceToolbar->SetTrackBarPosition(4);

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

		thumbFacePertinenceToolbar = new CThumbnailFacePertinenceToolBar(this, wxID_ANY, theme, false);//new CThumbnailFacePertinenceToolBar(this, wxID_ANY, theme);
		thumbFacePertinenceToolbar->SetTabValue(valuePertinence);
		thumbFacePertinenceToolbar->SetTrackBarPosition(2);
	}


    if(thumbscrollbar != nullptr)
        thumbscrollbar->Show(true);
    if(thumbFaceToolbar != nullptr)
        thumbFaceToolbar->Show(true);
    if(thumbFacePertinenceToolbar != nullptr)
        thumbFacePertinenceToolbar->Show(true);

	thumbFaceToolbar->Show(true);
	thumbscrollbar->ShowVerticalScroll();
	thumbnailFace->SetNoVScroll(false);
	thumbnailFace->SetCheck(true);
	thumbnailFace->Init();

	Connect(wxEVENT_THUMBNAILZOOMON, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailZoomOn));
	Connect(wxEVENT_THUMBNAILZOOMOFF, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailZoomOff));
	Connect(wxEVENT_THUMBNAILZOOMPOSITION, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailZoomPosition));
	//Connect(wxEVENT_THUMBNAILUPDATE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailUpdate));
	//Connect(wxEVENT_THUMBNAILADD, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailAdd));
	Connect(wxEVENT_THUMBNAILREFRESH, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailRefresh));
	Connect(wxEVENT_THUMBNAILMOVE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailMove));
	Connect(wxEVENT_FACEPHOTOUPDATE, wxCommandEventHandler(CListFace::OnFacePhotoAdd));
	Connect(wxEVENT_FACEPHOTOADD, wxCommandEventHandler(CListFace::OnAddFacePhoto));
	Connect(wxEVENT_REFRESHDATA, wxCommandEventHandler(CListFace::OnRefreshProcess));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CListFace::OnIdle));


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

	update = false;
	processIdle = true;
}


void CListFace::OnRefreshProcess(wxCommandEvent& event)
{
	processIdle = true;
}

void CListFace::OnIdle(wxIdleEvent& evt)
{
	//TRACE();
	if (processIdle && !endProgram)
		ProcessIdle();
}

void CListFace::ProcessIdle()
{
	TRACE();
	bool hasDoneOneThings = false;
	int nbProcesseur = 1;
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
		nbProcesseur = config->GetFaceProcess();

	CSqlFacePhoto facePhoto;
	vector<wxString> listPhoto = facePhoto.GetPhotoListTreatment();

	//Recherche Nb Fichier non traité pour le visage
	if (nbProcessFacePhoto < nbProcesseur && listPhoto.size() > 0)
	{
		PhotosVector photoList;
		CSqlFindPhotos findphotos;
		findphotos.GetAllPhotos(&photoList);

		CSqlFacePhoto sqlFacePhoto;
		sqlFacePhoto.InsertFaceTreatment(listPhoto.at(0));

		CThreadFace * path = new CThreadFace();
		path->filename = listPhoto.at(0);
		path->mainWindow = this;
		path->faceData = faceData;
		path->thread = new thread(FacialRecognition, path);

		CFaceInfosUpdate * infoUpdate = new CFaceInfosUpdate();
		//infoUpdate->message_2 = "Photo : " + path->filename;
		infoUpdate->message_3 = "Processing : " + to_string(photoList.size() - listPhoto.size()) + "/" + to_string(photoList.size());
		infoUpdate->photolistSize = photoList.size();
		infoUpdate->listPhotoSize = listPhoto.size();

		wxWindow * mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
		if (mainWindow != nullptr)
		{
			wxCommandEvent evt(wxEVENT_FACEINFOSUPDATESTATUSBAR);
			evt.SetClientData(infoUpdate);
			mainWindow->GetEventHandler()->AddPendingEvent(evt);
		}

		nbProcessFacePhoto++;
		
		hasDoneOneThings = true;
	}
	if (updateFaceList)
	{
		thumbnailFace->Init();
		updateFaceList = false;
	}


	if (!hasDoneOneThings)
		processIdle = false;

	if (listPhoto.size() > 0)
		processIdle = true;
}

bool CListFace::GetProcessEnd()
{
	TRACE();
	if (nbProcessFacePhoto > 0)
		return false;

	return true;
}

void CListFace::ThumbnailRefresh(wxCommandEvent& event)
{
	updateFaceList = true;
	processIdle = true;
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
	delete(thumbnailFace);
	delete(thumbFacePertinenceToolbar);
	delete(thumbFaceToolbar);
	delete(thumbscrollbar);
	delete(faceData);
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
    thumbscrollbar->UpdateScreenRatio();
    thumbFaceToolbar->UpdateScreenRatio();
	thumbnailFace->UpdateScreenRatio();
    this->Resize();
}


void CListFace::Resize()
{
	int pictureWidth = GetWindowWidth();
	int pictureHeight = GetWindowHeight() - thumbFaceToolbar->GetHeight() - thumbFacePertinenceToolbar->GetHeight();
	thumbFacePertinenceToolbar->SetSize(0, 0, GetWindowWidth(), thumbFacePertinenceToolbar->GetHeight());
	thumbscrollbar->SetSize(0, thumbFacePertinenceToolbar->GetHeight(), pictureWidth, pictureHeight);
	thumbFaceToolbar->SetSize(0, thumbFacePertinenceToolbar->GetHeight() + pictureHeight, GetWindowWidth(), thumbFaceToolbar->GetHeight());
}

//---------------------------------------------------------------------------------------
//Test FacialRecognition
//---------------------------------------------------------------------------------------
void CListFace::FacialRecognition(void * param)
{
	TRACE();
	CThreadFace * path = (CThreadFace *)param;
	bool pictureOK = false;
	CSqlFaceLabel sqlfaceLabel;
	void * faceData = path->faceData->GetCopyFaceData();
	CSqlFacePhoto sqlfacePhoto;
	CSqlFaceDescriptor sqlfaceDescritor;
	CFace * face = new CFace();
	CPictureData * pictureData = nullptr;
	int nbFaceFound = 0;
	CSqlThumbnail sqlThumbnail;
	CRegardsConfigParam * config = (CRegardsConfigParam*)CParamInit::getInstance();
	int pictureSize = config->GetFaceDetectionPictureSize();

	switch (pictureSize)
	{
	case 0: //Thumbnail Size
		if (sqlThumbnail.TestThumbnail(path->filename))
		{
			pictureData = sqlThumbnail.GetJpegThumbnail(path->filename);
			if (pictureData != nullptr)
				pictureOK = true;
		}
		else
		{
			pictureData = path->LoadPictureToJpeg(path->filename, pictureOK);
		}
		break;
	case 1: //Original Size
	{
		pictureData = path->LoadPictureToJpeg(path->filename, pictureOK);
	}
	break;

	case 2: //SD Format 
	{
		pictureData = path->LoadPictureToJpeg(path->filename, pictureOK, 640, 480);
	}
	break;


	case 3: //HD Format 
	{
		pictureData = path->LoadPictureToJpeg(path->filename, pictureOK, 1280, 720);
	}
	break;

	case 4: //Full HD Format 
	{
		pictureData = path->LoadPictureToJpeg(path->filename, pictureOK, 1920, 1080);
	}
	break;
	}




	if (pictureOK)
	{
		if (pictureData != nullptr)
			face->LoadPictureFromJpegBuffer(pictureData->data, pictureData->size, 0, faceData);

		int nbFace = face->GetNbFaceFound();

		if (nbFace > 0)
		{
			for (int i = 0; i < nbFace; i++)
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
				if (bitmapSize > 0)
				{
					dataBitmap = new uint8_t[bitmapSize];
					face->GetFaceData(i, width, height, pertinence, dataBitmap, bitmapSize);

					size = bitmapSize;

					uint8_t * data = nullptr;

					const int JPEG_QUALITY = 75;

					tjhandle _jpegCompressor = tjInitCompress();

					tjCompress2(_jpegCompressor, dataBitmap, width, 0, height, TJPF_RGBX,
						&data, &size, TJSAMP_444, JPEG_QUALITY,
						TJFLAG_FASTDCT);

					tjDestroy(_jpegCompressor);


					if (size > 0 && data != nullptr)
					{
						int numFace = 0;
						if (data != nullptr)
						{
							numFace = sqlfacePhoto.InsertFace(path->filename, i, width, height, pertinence, data, size);
							//numFace = sqlfacePhoto.GetNumFace(path->filename, i);
						}

						//Récupération des données du visage

						CFaceDescriptor * faceDescriptor = new CFaceDescriptor();
						faceDescriptor->numFace = numFace;
						faceDescriptor->size = 0;
						face->GetFaceDescriptor(i, faceDescriptor->descriptor, faceDescriptor->size);
						if (faceDescriptor->size > 0)
						{
							faceDescriptor->descriptor = new char[faceDescriptor->size + 1];
							face->GetFaceDescriptor(i, faceDescriptor->descriptor, faceDescriptor->size);
						}

						sqlfaceDescritor.InsertFaceDescriptor(numFace, faceDescriptor->descriptor, faceDescriptor->size);

						if (path->mainWindow != nullptr)
						{
							wxCommandEvent evt(wxEVENT_FACEPHOTOADD);
							evt.SetClientData(faceDescriptor);
							path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
						}

						nbFaceFound++;
					}

				}

				if (data != nullptr)
				{
					tjFree(data);
					data = nullptr;
				}
				//delete[] data;

				if (dataBitmap != nullptr)
					delete[] dataBitmap;
			}
		}
	}


	if (nbFaceFound == 0)
	{
		sqlfacePhoto.InsertFace(path->filename, 0, 0, 0, 0, nullptr, 0);
	}

	if (pictureData != nullptr)
		delete pictureData;

	if (face != nullptr)
		delete face;

	path->faceData->DeleteCopyFaceData(faceData);

	if (path->mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_FACEPHOTOUPDATE);
		evt.SetClientData(path);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CListFace::OnFacePhotoAdd(wxCommandEvent& event)
{
	TRACE();
	wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_THUMBNAILREFRESH);
	this->GetEventHandler()->AddPendingEvent(evt);

	CThreadFace * path = (CThreadFace *)event.GetClientData();
	if (path->thread != nullptr)
	{
		path->thread->join();
		delete(path->thread);
		path->thread = nullptr;
	}

	if (path != nullptr)
		delete path;

	nbProcessFacePhoto--;
}



//---------------------------------------------------------------------------------------
//Test si compatible avec une autre Face à déplacer ailleurs
//---------------------------------------------------------------------------------------	
void CListFace::OnAddFacePhoto(wxCommandEvent& event)
{
	TRACE();
	if (event.GetClientData() != nullptr)
	{
		CFaceDescriptor * faceDescriptor = (CFaceDescriptor *)event.GetClientData();
		CSqlFaceLabel sqlfaceLabel;
		CFace * face = new CFace();
		bool findFaceCompatible = false;
		CSqlFaceRecognition sqlfaceRecognition;
		CSqlFindFacePhoto sqlfindFacePhoto;

		std::vector<CFaceDescriptor*> listFace = sqlfindFacePhoto.GetUniqueFaceDescriptor(faceDescriptor->numFace);
		for (int i = 0; i < listFace.size(); i++)
		{

			CFaceDescriptor * facede = listFace[i];
			float isCompatible = face->IsCompatibleFace(faceDescriptor->descriptor, facede->descriptor);
			if (isCompatible < 0.6)
			{
				sqlfaceRecognition.InsertFaceRecognition(faceDescriptor->numFace, facede->numFace);
				findFaceCompatible = true;
				break;
			}
		}

		for (int i = 0; i < listFace.size(); i++)
		{
			CFaceDescriptor * facede = listFace[i];
			delete facede;
		}

		if (!findFaceCompatible)
		{
			wxString label = "Face number " + to_string(faceDescriptor->numFace);
			sqlfaceRecognition.InsertFaceRecognition(faceDescriptor->numFace, faceDescriptor->numFace);
			sqlfaceLabel.InsertFaceLabel(faceDescriptor->numFace, label, true);
			//updateCriteria = true;
			wxWindow * mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
			if (mainWindow != nullptr)
			{
				wxCommandEvent evt(wxEVENT_FACEINFOSUPDATE);
				mainWindow->GetEventHandler()->AddPendingEvent(evt);
			}
		}

		if (faceDescriptor != nullptr)
			delete faceDescriptor;

		delete face;
	}
}
